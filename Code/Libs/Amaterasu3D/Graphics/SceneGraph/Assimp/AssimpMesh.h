/*
 * AssimpMesh.h
 *
 *  Created on: Mar 7, 2011
 *      Author: Adrien
 */

#ifndef ASSIMPMESH_H_
#define ASSIMPMESH_H_

#include <Graphics/Texture.h>
#include <Graphics/SceneGraph/DrawObject.h>
#include <Enum.h>
#include <map>

namespace SceneGraph
{

struct AssimpMeshBuffer
{
	float* buffer;
	int size;
	int dimension;
};

class AssimpMesh : public DrawObject
{
private:
	unsigned int* m_indices_buffers;
	unsigned int* m_indices;
	int m_indices_size;
	typedef std::map<TextureType, TTexturePtr> TexturesMap;
	typedef std::map<ShaderAttributType, AssimpMeshBuffer> BufferMap;
	BufferMap m_buffers;
	TexturesMap m_textures_map;
	bool m_is_compiled;
public:
	AssimpMesh();
	virtual ~AssimpMesh();
	// Compile all buffers
	// Add buffers
	void SetIndiceBuffer(unsigned int* buffer, int size);
	void AddBuffer(AssimpMeshBuffer buffer, ShaderAttributType type);
	void AddTextureMap(TextureType type, TTexturePtr texture);
	void CompileBuffers();
	// Draw all buffers
	virtual void Draw();

};
}
#endif /* ASSIMPMESH_H_ */
