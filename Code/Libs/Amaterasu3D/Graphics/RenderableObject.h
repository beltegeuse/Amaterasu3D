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

#ifndef DRAWABLEOBJECTS_H_
#define DRAWABLEOBJECTS_H_

// Amaterasu3D includes
#include <Graphics/Texture.h>
#include <Graphics/Shaders/Shader.h>

// Other includes
#include <GL/gl.h>
namespace ama3D
{

class RenderableObject
{
public:
	/*
	 * Structure definition
	 */
	struct RenderableBuffer
	{
		float* buffer; // Data of the buffer
		int size; // The size of the data buffer
		int dimension; // Dimension of the data
		bool owner; // To know if Assimp Mesh is the owner of the buffer
	};
private:
	/*
	 * Attributes
	 */
	// OpenGL Buffers indices (VBO)
	unsigned int* m_indices_buffers;
	unsigned int* m_indices;
	int m_indices_size;
	// All data
	typedef std::map<TextureType, TTexturePtr> TexturesMap;
	typedef std::map<int, RenderableObject::RenderableBuffer> BufferMap;
	typedef std::map<MaterialType, Color> MaterialMap;
	BufferMap m_buffers;
	TexturesMap m_textures_map;
	MaterialMap m_material_map;
	// To know if all buffers is compiled
	bool m_is_compiled;
	bool m_IsInstance;
	GLenum m_DrawMode;
public:
	RenderableObject();
	virtual ~RenderableObject();

	/*
	 * Public methods
	 */
	void SetIndiceBuffer(unsigned int* buffer, int size);
	// Add differents buffers
	void AddBuffer(RenderableObject::RenderableBuffer buffer,
			int typeID);
	void AddTextureMap(TextureType type, TTexturePtr texture);
	void AddMaterial(MaterialType type, Color color);
	// Compile all buffers
	void CompileBuffers();
	// Draw all buffers
	virtual void Draw();
	// To know if it's an Instance (only buffers)
	bool IsInstance(RenderableObject& model); //FIXME: Re-add const
	// To clean up all buffers and link to the instance one
	// Model is the object to set on Instance
	void SetInstance(RenderableObject& model) const;

	void SetDrawMode(GLenum mode);
};

}

#endif /* DRAWABLEOBJECTS_H_ */
