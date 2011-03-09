#include <gl/glew.h>
#include <gl/gl.h>

#include "AssimpMesh.h"
#include <Debug/Exceptions.h>
#include <Logger/Logger.h>
#include <Graphics/GLSLShader.h>

namespace SceneGraph
{

AssimpMesh::AssimpMesh() :
		m_indices_buffers(NULL),
		m_indices(NULL),
		m_is_compiled(false)
{
}

AssimpMesh::~AssimpMesh()
{
	//FIXME: Si mise en place du cache, alors attention a la destructions des differents objets.
	// solution : Smart pointeurs
	glDeleteBuffers(m_buffers.size()+1, m_indices_buffers);
	delete[] m_indices_buffers;
	for(BufferMap::iterator it = m_buffers.begin(); it != m_buffers.end(); it++)
	{
		if(it->second.owner)
			delete[] it->second.buffer;
	}
}

void AssimpMesh::SetIndiceBuffer(unsigned int* buffer, int size)
{
	m_indices = buffer;
	m_indices_size = size;
}

void AssimpMesh::AddBuffer(AssimpMeshBuffer buffer, ShaderAttributType type)
{
	Assert(!m_is_compiled);
	m_buffers[type] = buffer;
}

void AssimpMesh::AddTextureMap(TextureType type, TTexturePtr texture)
{
	m_textures_map[type] = texture;
}

void AssimpMesh::CompileBuffers()
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

void AssimpMesh::Draw()
{
	// pas de shader
	if(!glShaderManager::Instance().activedShader())
	{
		Logger::Log() << "[Warning] No actived shader. Nothings to render ... \n";
	}
	// Textures activation
	for(TexturesMap::iterator it = m_textures_map.begin(); it != m_textures_map.end(); it++)
	{
		if(!glShaderManager::Instance().currentShader()->textureAvailable(it->first))
			continue;
//		Logger::Log() << "Activate : " << (int) it->first << "\n";
		it->second->activateMultiTex(it->first);
	}
	// Buffer activation
	int i = 1;
	for(BufferMap::iterator it = m_buffers.begin(); it != m_buffers.end(); it++)
	{
		if(!glShaderManager::Instance().currentShader()->attributAvailable(it->first))
			continue;
//		Logger::Log() << "BIND Buffer : " << m_indices_buffers[i] << "\n";
		GLCheck(glBindBuffer(GL_ARRAY_BUFFER, m_indices_buffers[i]));
		glEnableVertexAttribArray (it->first);
		GLCheck(glVertexAttribPointer(it->first, it->second.dimension, GL_FLOAT, GL_FALSE, it->second.dimension * sizeof(float), 0));
		i++;
	}
	// Drawing
	GLCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indices_buffers[0]));
	GLCheck(glDrawElements(GL_TRIANGLES, m_indices_size, GL_UNSIGNED_INT, 0));
	// Buffer desactivation
	for(BufferMap::iterator it = m_buffers.begin(); it != m_buffers.end(); it++)
	{
		if(!glShaderManager::Instance().currentShader()->attributAvailable(it->first))
			continue;
		glDisableVertexAttribArray (it->first);
	}
	// Textures desactivations
	for(TexturesMap::iterator it = m_textures_map.begin(); it != m_textures_map.end(); it++)
	{
		if(!glShaderManager::Instance().currentShader()->textureAvailable(it->first))
			continue;
//		Logger::Log() << "Desactivate : " << (int) it->first << "\n";
		it->second->desactivateMultiTex(it->first);
	}
}

} // namespace SceneGraph
