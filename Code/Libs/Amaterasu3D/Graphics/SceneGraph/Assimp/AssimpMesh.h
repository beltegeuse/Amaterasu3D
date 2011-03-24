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
#include <Graphics/Color.h>
#include <Enum.h>
#include <map>

namespace SceneGraph
{

/*
 * \brief This structure represent a data buffer
 */
struct AssimpMeshBuffer
{
	float* buffer; // Data of the buffer
	int size; // The size of the data buffer
	int dimension; // Dimension of the data
	bool owner; // To know if Assimp Mesh is the owner of the buffer
};

class AssimpMesh : public DrawObject
{
private:
	/*
	 * Attributs
	 */
	// OpenGL Buffers indices (VBO)
	unsigned int* m_indices_buffers;
	unsigned int* m_indices;
	int m_indices_size;
	// All data
	typedef std::map<TextureType, TTexturePtr> TexturesMap;
	typedef std::map<ShaderAttributType, AssimpMeshBuffer> BufferMap;
	typedef std::map<MaterialType, Color> MaterialMap;
	BufferMap m_buffers;
	TexturesMap m_textures_map;
	MaterialMap m_material_map;
	// To know if all buffers is compiled
	bool m_is_compiled;

public:
	/*
	 * Constructor & Destructor
	 */
	AssimpMesh();
	virtual ~AssimpMesh();

	/*
	 * Public methods
	 */
	void SetIndiceBuffer(unsigned int* buffer, int size);
	// Add differents buffers
	void AddBuffer(AssimpMeshBuffer buffer, ShaderAttributType type);
	void AddTextureMap(TextureType type, TTexturePtr texture);
	void AddMaterial(MaterialType type, Color color);
	// Compile all buffers
	void CompileBuffers();
	// Draw all buffers
	virtual void Draw();

};
}
#endif /* ASSIMPMESH_H_ */
