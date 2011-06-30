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

#ifndef GBUFFERSHADER_H_
#define GBUFFERSHADER_H_

#include <Graphics/Shaders/Shader.h>

class GBufferShader: public Shader
{
private:
	// Attributes
	// Differents use for setup the shader
	bool m_use_texCoord;
	bool m_use_texNormal;
	bool m_use_texDiffuse;
	bool m_use_texSpecular;
	bool m_use_tangants;
public:
	GBufferShader(ShaderUnit * VertexShader, ShaderUnit * FragmentShader,
			ShaderUnit * GeometryShader = 0);
	virtual ~GBufferShader();

	virtual void OnDraw();
	virtual bool IsAttributAvailable(ShaderAttributType type);
	virtual bool IsTextureAvailable(TextureType type);
};

#endif /* GBUFFERSHADERS_H_ */
