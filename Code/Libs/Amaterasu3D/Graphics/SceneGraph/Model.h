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
struct ModelBuffer
{
	float* buffer; // Data of the buffer
	int size; // The size of the data buffer
	int dimension; // Dimension of the data
	bool owner; // To know if Assimp Mesh is the owner of the buffer
};

class Model : public DrawObject
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
	typedef std::map<ShaderAttributType, ModelBuffer> BufferMap;
	typedef std::map<MaterialType, Color> MaterialMap;
	BufferMap m_buffers;
	TexturesMap m_textures_map;
	MaterialMap m_material_map;
	// To know if all buffers is compiled
	bool m_is_compiled;
	bool m_IsInstance;

public:
	/*
	 * Constructor & Destructor
	 */
	Model();
	virtual ~Model();

	/*
	 * Public methods
	 */
	void SetIndiceBuffer(unsigned int* buffer, int size);
	// Add differents buffers
	void AddBuffer(ModelBuffer buffer, ShaderAttributType type);
	void AddTextureMap(TextureType type, TTexturePtr texture);
	void AddMaterial(MaterialType type, Color color);
	// Compile all buffers
	void CompileBuffers();
	// Draw all buffers
	virtual void Draw();
	// To know if it's an Instance (only buffers)
	bool IsInstance(Model& model); //FIXME: Re-add const
	// To clean up all buffers and link to the instance one
	// Model is the object to set on Instance
	void SetInstance(Model& model) const;

};
}
#endif /* ASSIMPMESH_H_ */
