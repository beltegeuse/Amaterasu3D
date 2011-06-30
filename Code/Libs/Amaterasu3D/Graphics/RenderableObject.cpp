//==========================================================
// Amaterasu3D - perceptual 3D engine
//
// Copyright (C) 2010-2011 Adrien Gruson
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc.,
// 59 Temple Place - Suite 330,
// Boston, MA  02111-1307, USA.
//
// E-mail : adrien.gruson@gmail.com
//==========================================================

#include "RenderableObject.h"

RenderableObject::RenderableObject() :
m_indices_buffers(NULL),
m_indices(NULL),
m_is_compiled(false),
m_IsInstance(false),
m_DrawMode(GL_TRIANGLES)
{
}

RenderableObject::~RenderableObject() {
	if(m_IsInstance)
		return;

	Logger::Log() << "[DEBUG] Delete RenderableObject : \n";

	glDeleteBuffers(m_buffers.size()+1, m_indices_buffers);
	delete[] m_indices_buffers;
	for(BufferMap::iterator it = m_buffers.begin(); it != m_buffers.end(); it++)
	{
		if(it->second.owner)
		{
			Logger::Log() << "[DEBUG] * Delete buffer dim : " << it->second.dimension << "\n";
			delete[] it->second.buffer;
		}
	}
}

void RenderableObject::SetIndiceBuffer(unsigned int* buffer, int size)
{
	m_indices = buffer;
	m_indices_size = size;
}

void RenderableObject::AddBuffer(RenderableObject::RenderableBuffer buffer, ShaderAttributType type)
{
	Assert(!m_is_compiled);
	m_buffers[type] = buffer;
}

void RenderableObject::AddTextureMap(TextureType type, TTexturePtr texture)
{
	m_textures_map[type] = texture;
}

void RenderableObject::AddMaterial(MaterialType type, Color color)
{
	m_material_map[type] = color;
}

void RenderableObject::CompileBuffers()
{
	Logger::Log() << "[INFO] Generate " << m_buffers.size()+1 << " buffers ... \n";
	m_is_compiled = true;
	m_indices_buffers = new unsigned int[m_buffers.size()+1];
	GLCheck(glGenBuffers( m_buffers.size()+1, m_indices_buffers ));
	Logger::Log() << "  * indice id : " << m_indices_buffers[0] << "\n";
	// Add index buffer
	Logger::Log() << "   * load indices buffers ... " << m_indices_buffers[0] << "\n";
	GLCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indices_buffers[0]));
	GLCheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*m_indices_size, m_indices, GL_STATIC_DRAW));
	// Add all others buffers
	int i = 1;
	for(BufferMap::iterator it = m_buffers.begin(); it != m_buffers.end(); it++)
	{
		Logger::Log() << "   * load other buffers ... " << m_indices_buffers[i] << "\n";
		GLCheck(glBindBuffer(GL_ARRAY_BUFFER, m_indices_buffers[i]));
		GLCheck(glBufferData(GL_ARRAY_BUFFER, sizeof(float)*it->second.size, it->second.buffer, GL_STATIC_DRAW));
		i++;
	}
}

void RenderableObject::Draw()
{
	// pas de shader
	if(!CShaderManager::Instance().activedShader())
	{
		Logger::Log() << "[Warning] No actived shader. Nothings to render ... \n";
	}
	// Material activation
	for(MaterialMap::iterator it = m_material_map.begin(); it != m_material_map.end(); it++)
	{
		if(!CShaderManager::Instance().currentShader()->IsMaterialAvailable(it->first))
			continue;
		CShaderManager::Instance().currentShader()->SetMaterialValue(it->first,it->second);
	}
	// Textures activation
	for(TexturesMap::iterator it = m_textures_map.begin(); it != m_textures_map.end(); it++)
	{
		if(!CShaderManager::Instance().currentShader()->IsTextureAvailable(it->first))
			continue;
		it->second->activateMultiTex(it->first);
	}
	// Buffer activation
	int i = 1;
	for(BufferMap::iterator it = m_buffers.begin(); it != m_buffers.end(); it++)
	{
		if(!CShaderManager::Instance().currentShader()->IsAttributAvailable(it->first))
			continue;
		GLCheck(glBindBuffer(GL_ARRAY_BUFFER, m_indices_buffers[i]));
		glEnableVertexAttribArray (it->first);
		GLCheck(glVertexAttribPointer(it->first, it->second.dimension, GL_FLOAT, GL_FALSE, it->second.dimension * sizeof(float), 0));
		i++;
	}
	// Drawing
	CShaderManager::Instance().currentShader()->OnDraw();
	GLCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indices_buffers[0]));
	GLCheck(glDrawElements(m_DrawMode, m_indices_size, GL_UNSIGNED_INT, 0));
	// Buffer desactivation
	for(BufferMap::iterator it = m_buffers.begin(); it != m_buffers.end(); it++)
	{
		if(!CShaderManager::Instance().currentShader()->IsAttributAvailable(it->first))
			continue;
		glDisableVertexAttribArray (it->first);
	}
	// Textures desactivations
	for(TexturesMap::reverse_iterator it = m_textures_map.rbegin(); it != m_textures_map.rend(); it++)
	{
		if(!CShaderManager::Instance().currentShader()->IsTextureAvailable(it->first))
			continue;
		it->second->desactivateMultiTex(it->first);
	}
}

void RenderableObject::SetDrawMode(GLenum mode)
{
	m_DrawMode = mode;
}

bool RenderableObject::IsInstance(RenderableObject& model)
{
	// First test is the number of indices
	if(m_indices_size != model.m_indices_size)
		return false;
	// Second test is the number of buffers
	if(m_buffers.size() != model.m_buffers.size())
		return false;
	// Third test buffers size
	for(BufferMap::const_iterator it = m_buffers.begin(); it != m_buffers.end(); it++)
	{
		BufferMap::const_iterator it2 = model.m_buffers.find(it->first);
		if(it2 == model.m_buffers.end())
			return false;
		if(it2->second.dimension != it->second.dimension)
			return false;
		if(it2->second.size != it->second.size)
			return false;
	}
	// Last step : Check buffer contents
	// * Check if they needs to compute the transformation
	bool needTransformation = false;
	Math::CMatrix4 transformationMatrix;
	if(m_buffers.find(VERTEX_ATTRIBUT) != m_buffers.end() && m_buffers[VERTEX_ATTRIBUT].size >= 11)
	{
		Logger::Log() << "[INFO] Try to find the transformations ... \n";
		RenderableBuffer b1 = m_buffers[VERTEX_ATTRIBUT];
		RenderableBuffer b2 = model.m_buffers[VERTEX_ATTRIBUT];
		if(b1.buffer[0] != b2.buffer[0])
		{
			needTransformation = true;
			// Frist matrix
			Math::TVector3F v1(b1.buffer[0], b1.buffer[1], b1.buffer[2]);
			Math::TVector3F v2(b1.buffer[3], b1.buffer[4], b1.buffer[5]);
			Math::TVector3F v3(b1.buffer[6], b1.buffer[7], b1.buffer[8]);
			Math::TVector3F v4 = v1 + ((v2-v1)^(v3-v1));
			Math::CMatrix4 t1(v1.x, v2.x, v3.x, v4.x,
					          v1.y, v2.y, v3.y, v4.y,
					          v1.z, v2.z, v3.z, v4.z,
					          1, 1, 1, 1);
			// second matrix
			Math::TVector3F v1p(b2.buffer[0], b2.buffer[1], b2.buffer[2]);
			Math::TVector3F v2p(b2.buffer[3], b2.buffer[4], b2.buffer[5]);
			Math::TVector3F v3p(b2.buffer[6], b2.buffer[7], b2.buffer[8]);
			Math::TVector3F v4p = v1p + ((v2p-v1p)^(v3p-v1p));
			Math::CMatrix4 t2(v1p.x, v2p.x, v3p.x, v4p.x,
							  v1p.y, v2p.y, v3p.y, v4p.y,
							  v1p.z, v2p.z, v3p.z, v4p.z,
							  1, 1, 1, 1);

			transformationMatrix = t1.Inverse()*t2;
			Logger::Log() << "[INFO] Transform matrice : \n" << transformationMatrix;
		}
	}
	for(BufferMap::const_iterator it = m_buffers.begin(); it != m_buffers.end(); it++)
	{

		BufferMap::const_iterator it2 = model.m_buffers.find(it->first);
		Logger::Log() << "[INFO] INSTANCE : " << it->first << " buffer check instance ... \n";
		for(int i = 0; i < it->second.size; i++)
		{
			if(!needTransformation)
			{
				if(it->second.buffer[i] != it2->second.buffer[i])
				{
					Logger::Log() << "[INFO] Buffer difference detected : " << i
							<< " ( " << it->second.buffer[i] << " != " << it2->second.buffer[i] << " )\n";
					return false;
				}
			}
			else
			{
				if(it->first == VERTEX_ATTRIBUT || it->first == NORMAL_ATTRIBUT || it->first == TANGENT_ATTRIBUT || it->first == BITANGENT_ATTRIBUT)
				{
					// Construct the Vector
					Math::TVector4F v1(it->second.buffer[i],it->second.buffer[i+1],it->second.buffer[i+2],1.0);
					Math::TVector4F v2(it2->second.buffer[i],it2->second.buffer[i+1],it2->second.buffer[i+2],1.0);
					Math::TVector4F vTrans = transformationMatrix.Transform(v1);
					vTrans /= vTrans.w;
					if(v2 != vTrans)
					{
						Logger::Log() << "[INFO] Buffer difference detected : " << i
									  << " ( " << v2 << " != " << vTrans << " )\n";
						return false;
					}
					// Must add two steps
					i += 2;
				}
				else
				{
					// XXX: Somes copy from other tests
					if(it->second.buffer[i] != it2->second.buffer[i])
					{
						Logger::Log() << "[INFO] Buffer difference detected : " << i
								<< " ( " << it->second.buffer[i] << " != " << it2->second.buffer[i] << " )\n";
						return false;
					}
				}
			}
		}
	}
	// All tests is Ok so Model is an instance
	return true;
}

void RenderableObject::SetInstance(RenderableObject& model) const
{
	// Clear all model buffers
	for(BufferMap::iterator it = model.m_buffers.begin(); it != model.m_buffers.end(); it++)
	{
		if(it->second.owner)
		{
			delete[] it->second.buffer;
		}
	}
	model.m_buffers.erase(model.m_buffers.begin(), model.m_buffers.end());
	// Set all new buffers
	for(BufferMap::const_iterator it = m_buffers.begin(); it != m_buffers.end(); it++)
	{
		RenderableBuffer buffer = it->second;
		// Specify is not the owner
		buffer.owner = false;
		model.m_buffers[it->first] = buffer;
	}
	model.m_IsInstance = true;
	model.m_is_compiled = true;
	model.m_indices_buffers = m_indices_buffers;
}
