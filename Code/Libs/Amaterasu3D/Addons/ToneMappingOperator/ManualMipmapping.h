/*
 * ManualMipmapping.h
 *
 *  Created on: Jun 7, 2011
 *      Author: adrien
 */

#ifndef MANUALMIPMAPPING_H_
#define MANUALMIPMAPPING_H_

#include <Graphics/FBO.h>
#include <Graphics/Shaders/Shader.h>
#include <Graphics/Texture.h>

class ManualMipmapping {
private:
	/*
	 * Attributes
	 */

	// Info shaders
	int m_NbLevels;
	int m_InitialSize;

	// Shaders
	TShaderPtr m_ManualMipmappingShader;
	TShaderPtr m_DownSamplerShader;

	// FBO for manual mipmapping
	FBO** m_Mipmaps;

public:
	/*
	 * Constructors and destructors
	 */
	ManualMipmapping(int initialSize);
	virtual ~ManualMipmapping();

	/*
	 * Public methods
	 */
	// Get informations methods
	int NumberLevels();
	Texture* GetLevel(int l);

	// Compute methods
	void Compute(Texture* texture);
	void Initialize();

private:
	/*
	 * Private methods
	 */
	FBO* CreateFBO(FBOTextureBufferParam p, const std::string& buffername, int size);
	void DrawQuad();
};

#endif /* MANUALMIPMAPPING_H_ */
