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

#ifndef MANUALMIPMAPPING_H_
#define MANUALMIPMAPPING_H_

#include <Graphics/FBO.h>
#include <Graphics/Shaders/Shader.h>
#include <Graphics/Texture.h>

namespace ama3D
{
class ManualMipmapping
{
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
	FBO* CreateFBO(FBOTextureBufferParam p, const std::string& buffername,
			int size);
	void DrawQuad();
};
}

#endif /* MANUALMIPMAPPING_H_ */
