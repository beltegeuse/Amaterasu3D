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

#ifndef LPV_H_
#define LPV_H_

// Amaterasu includes
#include <Graphics/Shaders/Shader.h>
#include <Graphics/Lighting/LightShaders.h>
#include <Graphics/SceneGraph/DrawObject.h>
#include <Graphics/Camera/CameraAbstract.h>
#include <Math/Vector2.h>
#include <Math/Vector3.h>
#include <Math/Matrix4.h>

class LPV
{
protected:
	/*
	 * Attributes
	 */
	/////////////
	// Grid attributes
	/////////////
	int m_NbCellDim;
	int m_CellSize;
	int m_NbPropagationStep;
	Math::TVector2I m_TextureSize;
	Math::TVector2I m_TextureRepeat;
	Math::TVector3F m_GirdPosition;
	/////////////
	// Shaders
	/////////////
	// Injection pass
	TShaderPtr m_LPVInjectGeomerty;
	TShaderPtr m_LPVInjectVPL;
	// Diffusion pass
	TShaderPtr m_LPVPropagationShader;
	TShaderPtr m_LPVBlend; // TODO: Delete blending pass
	// Lighting passes
	TShaderPtr m_LPVLightingShader;
	/////////////////
	// Graphics resources
	/////////////////
	// TODO: Do the same as CryTek (Double buffering technique)
	FBO** m_PropagationFBOs; ///< To store all intermediary computation

public:
	/*
	 * Constructors and Destructors
	 */
	LPV(int nbCells = 32, int sizeCells = 10, int propagationSteps = 8);
	virtual ~LPV();

	/*
	 * Public methods
	 */
	void Initialize();
	void ComputeGridPosition(CameraAbstract* Camera);

	FBO* GetFBO()
	{
		return m_LPVBlend->GetFBO();
	}
	void SetGridInformations(TShaderPtr shader)
	{
		shader->SetUniformMatrix4fv("LPVMatrix", GetInvGridMatrix());
		shader->SetUniformVector("LPVSize",Math::TVector4F(m_TextureSize.x,m_TextureSize.y,m_TextureRepeat.x,m_TextureRepeat.y));
		shader->SetUniformVector("LPVCellSize",Math::TVector4F(m_CellSize,m_CellSize,m_CellSize,m_NbCellDim));
	}

	//TODO: Can fusion Inject methods
	///////////////////////////////////
	/// Injection VPL Pass
	///////////////////////////////////
	// TODO: Add the injection scheme
	void InjectVPLFromLight(LightShaders& shader, SceneGraph::DrawObject& samples);
	void BeginInjectionVPLPass();
	void EndInjectionVPLPass();
	///////////////////////////////////
	/// Injection Geometry Pass
	///////////////////////////////////
	void InjectGeometryFromLight(LightShaders& shader, SceneGraph::DrawObject& samples);
	void BeginInjectionGeometryPass();
	void EndInjectionGeometryPass();
	///////////////////////////////////
	/// Propagation Pass
	///////////////////////////////////
	void ComputePropagation(int nbSteps);
	///////////////////////////////////
	/// Lighting pass
	///////////////////////////////////
	void ShowDebugPropagation(TShaderPtr GBuffer, int PropagatedShow);
	Math::CMatrix4 GetGridMatrix()
	{
		Math::CMatrix4 mat;
		mat.SetTranslation(m_GirdPosition.x,m_GirdPosition.y,m_GirdPosition.z);
		return mat;
	}
private:
	//TODO: Change that ....
	Math::CMatrix4 GetInvGridMatrix()
	{
		Math::CMatrix4 mat;
		mat.SetTranslation(-m_GirdPosition.x,-m_GirdPosition.y,-m_GirdPosition.z);
		return mat;
	}


};

#endif /* LPV_H_ */
