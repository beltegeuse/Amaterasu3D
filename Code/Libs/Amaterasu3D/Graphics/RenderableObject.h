/*
 * DrawableObjects.h
 *
 *  Created on: Jun 29, 2011
 *      Author: adrien
 */

#ifndef DRAWABLEOBJECTS_H_
#define DRAWABLEOBJECTS_H_

class RenderableObject {
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
	typedef std::map<ShaderAttributType, ModelBuffer> BufferMap;
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

	void SetDrawMode(GLenum mode);
};

#endif /* DRAWABLEOBJECTS_H_ */
