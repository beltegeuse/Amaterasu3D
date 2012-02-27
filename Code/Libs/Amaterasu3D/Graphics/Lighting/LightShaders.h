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

#ifndef LIGHTSHADERS_H_
#define LIGHTSHADERS_H_

// Amaterasu includes
#include <Graphics/Shaders/Shader.h>
#include <Graphics/Lighting/LightingStructures.h>
#include <Graphics/MatrixManagement.h>
#include <Graphics/SceneManager.h>
#include "glm/glm.hpp"

#include <cmath>

namespace ama3D
{
//TODO: Generaliser
class LightShaders: public SpotLight
{
private:
	/*
	 * Attributes
	 */
	TShaderPtr m_RSMShader;
public:
	/*
	 * Constructors and destructors
	 */
	LightShaders()
	{
	}
	virtual ~LightShaders()
	{
	}

	/*
	 * Public methods
	 */
	// To initialize the shader
	void Initialize()
	{
		//TODO
		m_RSMShader = CShaderManager::Instance().LoadShader(
				"RefectiveShadowMapSpot.shader");
	}
	// Matrices methods
	glm::mat4x4 GetViewMatrix() const
	{
		glm::mat4x4 LightViewMatrix;
		LightViewMatrix.LookAt(Position, Direction);
		return LightViewMatrix;
	}
	glm::mat4x4 GetProjectionMatrix() const
	{
		glm::mat4x4 LightProjectionMatrix;
		LightProjectionMatrix = glm::mat4x4::CreatePerspectiveFOV(70.0, 1.0,
				1.0, LightRaduis);
		return LightProjectionMatrix;
	}

	void Compute()
	{
		// Fill in RSM spot buffers
		// * Matrix Setup
		glm::mat4x4 oldProjectionMatrix;
		glm::mat4x4 oldViewMatrix;
		// * Save old transformations
		oldProjectionMatrix = CMatrixManager::Instance().GetMatrix(
				PROJECTION_MATRIX);
		oldViewMatrix = CMatrixManager::Instance().GetMatrix(VIEW_MATRIX);
		// * Go to the camera view
		CMatrixManager::Instance().SetProjectionMatrix(GetProjectionMatrix());
		CMatrixManager::Instance().SetViewMatrix(GetViewMatrix());
		// * Enable Shader
		m_RSMShader->Begin();
		// *** Send all Uniform values

		/*
		 *  uniform vec3 LightPosition;
		 uniform vec4 LightColor;
		 uniform vec3 LightSpotDirection;
		 uniform float LightRaduis;
		 uniform float LightIntensity;
		 uniform float LightCutOff; // cos value
		 */

		m_RSMShader->SetUniform1f("LightRaduis", LightRaduis);
		m_RSMShader->SetUniform1f("LightIntensity", LightIntensity);
		m_RSMShader->SetUniform1f("LightCutOff",
				cos(LightCutOff * (M_PI / 180.0)));
		m_RSMShader->SetUniformVector("LightPosition", Position);
		m_RSMShader->SetUniformVector("LightSpotDirection", Direction);
		m_RSMShader->SetUniformColor("LightColor", LightColor);
		// * Draw the scene
		CSceneManager::Instance().RenderAll();
		m_RSMShader->End();
		// * Revert transformations
		CMatrixManager::Instance().SetProjectionMatrix(oldProjectionMatrix);
		CMatrixManager::Instance().SetViewMatrix(oldViewMatrix);
	}

	FBO* GetFBO()
	{
		return m_RSMShader->GetFBO();
	}

};

}

#endif /* LIGHTSHADERS_H_ */
