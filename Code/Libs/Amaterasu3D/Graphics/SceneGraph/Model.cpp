//==========================================================
// Amaterasu3D - perceptual 3D engine
//
// Copyright (C) 2004-2005 Adrien Gruson
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
#include <gl/glew.h>
#include <gl/gl.h>

#include "Model.h"
#include <Debug/Exceptions.h>
#include <Logger/Logger.h>
#include <Graphics/GLSLShader.h>

namespace SceneGraph
{

Model::Model() :
		m_indices_buffers(NULL),
		m_indices(NULL),
		m_is_compiled(false),
		m_IsInstance(false)
{
}

Model::~Model()
{
	if(m_IsInstance)
		return;

	glDeleteBuffers(m_buffers.size()+1, m_indices_buffers);
	delete[] m_indices_buffers;
	for(BufferMap::iterator it = m_buffers.begin(); it != m_buffers.end(); it++)
	{
		if(it->second.owner)
			delete[] it->second.buffer;
	}
}

void Model::SetIndiceBuffer(unsigned int* buffer, int size)
{
	m_indices = buffer;
	m_indices_size = size;
}

void Model::AddBuffer(ModelBuffer buffer, ShaderAttributType type)
{
	Assert(!m_is_compiled);
	m_buffers[type] = buffer;
}

void Model::AddTextureMap(TextureType type, TTexturePtr texture)
{
	m_textures_map[type] = texture;
}

void Model::AddMaterial(MaterialType type, Color color)
{
	m_material_map[type] = color;
}

void Model::CompileBuffers()
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

void Model::Draw()
{
	// pas de shader
	if(!CShaderManager::Instance().activedShader())
	{
		Logger::Log() << "[Warning] No actived shader. Nothings to render ... \n";
	}
	// Material activation
	for(MaterialMap::iterator it = m_material_map.begin(); it != m_material_map.end(); it++)
	{
		if(!CShaderManager::Instance().currentShader()->materialAvailable(it->first))
			continue;
		CShaderManager::Instance().currentShader()->setMaterialValue(it->first,it->second);
	}
	// Textures activation
	for(TexturesMap::iterator it = m_textures_map.begin(); it != m_textures_map.end(); it++)
	{
		if(!CShaderManager::Instance().currentShader()->textureAvailable(it->first))
			continue;
		it->second->activateMultiTex(it->first);
	}
	// Buffer activation
	int i = 1;
	for(BufferMap::iterator it = m_buffers.begin(); it != m_buffers.end(); it++)
	{
		if(!CShaderManager::Instance().currentShader()->attributAvailable(it->first))
			continue;
		GLCheck(glBindBuffer(GL_ARRAY_BUFFER, m_indices_buffers[i]));
		glEnableVertexAttribArray (it->first);
		GLCheck(glVertexAttribPointer(it->first, it->second.dimension, GL_FLOAT, GL_FALSE, it->second.dimension * sizeof(float), 0));
		i++;
	}
	// Drawing
	CShaderManager::Instance().currentShader()->OnDraw();
	GLCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indices_buffers[0]));
	GLCheck(glDrawElements(GL_TRIANGLES, m_indices_size, GL_UNSIGNED_INT, 0));
	// Buffer desactivation
	for(BufferMap::iterator it = m_buffers.begin(); it != m_buffers.end(); it++)
	{
		if(!CShaderManager::Instance().currentShader()->attributAvailable(it->first))
			continue;
		glDisableVertexAttribArray (it->first);
	}
	// Textures desactivations
	for(TexturesMap::reverse_iterator it = m_textures_map.rbegin(); it != m_textures_map.rend(); it++)
	{
		if(!CShaderManager::Instance().currentShader()->textureAvailable(it->first))
			continue;
		it->second->desactivateMultiTex(it->first);
	}
}

bool Model::IsInstance(const Model& model) const
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
	for(BufferMap::const_iterator it = m_buffers.begin(); it != m_buffers.end(); it++)
	{

		BufferMap::const_iterator it2 = model.m_buffers.find(it->first);
		Logger::Log() << "[INFO] INSTANCE : " << it->first << " buffer check instance ... \n";
		for(int i = 0; i < it->second.size; i++)
		{
			if(it->second.buffer[i] != it2->second.buffer[i])
			{
				Logger::Log() << "[INFO] Buffer difference detected : " << i
						<< " ( " << it->second.buffer[i] << " != " << it2->second.buffer[i] << " )\n";
				return false;
			}
		}
	}
	// All tests is Ok so Model is an instance
	return true;
}

void Model::SetInstance(Model& model) const
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
		ModelBuffer buffer = it->second;
		// Specify is not the owner
		buffer.owner = false;
		model.m_buffers[it->first] = buffer;
	}
	model.m_IsInstance = true;
	model.m_is_compiled = true;
	model.m_indices_buffers = m_indices_buffers;
}

} // namespace SceneGraph
