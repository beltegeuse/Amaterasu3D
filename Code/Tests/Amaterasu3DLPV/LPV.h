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
#include <Graphics/SceneGraph/Group.h>
#include <Graphics/Camera/CameraAbstract.h>
#include <Math/Vector2.h>
#include <Math/Vector3.h>
#include <Math/Matrix4.h>

class LPV
{
public:
	/*
	 * Attributes
	 */
	/////////////
	// Grid attributes
	/////////////
	int m_NbCellDim;
	float* m_CellSize; ///< Array for each cascade
	int m_NbPropagationStep;
	int m_NbCascadedLevels;
	Math::TVector2I m_TextureSize;
	Math::TVector2I m_TextureRepeat;
	Math::TVector3F* m_GirdPosition; ///< Array for each cascade
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
	SceneGraph::Group** m_GridModels; ///< To draw all grid

public:
	/*
	 * Constructors and Destructors
	 */
	LPV(int nbCells = 32, int sizeCells = 16, int propagationSteps = 8, int nbLevels = 2);
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
		// WARNING : Don't forgot to update the uniform update on Lighting pass
		//FIXME: MODIFY METHODS !!!
		shader->SetUniformVectorArray("LPVPosition[0]", m_NbCascadedLevels, m_GirdPosition);
		shader->SetUniformVector("LPVSize",Math::TVector4F(m_TextureSize.x,m_TextureSize.y,m_TextureRepeat.x,m_TextureRepeat.y));
		shader->SetUniform1fv("LPVCellSize[0]",m_NbCascadedLevels, m_CellSize);
		shader->SetUniform1i("LPVNbCell", m_NbCellDim);
	}

	void DrawGrids();
	void DrawGrid(int level);
	int GetNumberCascade() const
	{ return m_NbCascadedLevels; }

	//TODO: Can fusion Inject methods
	///////////////////////////////////
	/// Injection VPL Pass
	///////////////////////////////////
	// TODO: Add the injection scheme
	void InjectVPLFromLight(LightShaders& shader, SceneGraph::DrawObject& samples);
	void BeginInjectionVPLPass(int level);
	void EndInjectionVPLPass();
	///////////////////////////////////
	/// Injection Geometry Pass
	///////////////////////////////////
	void InjectGeometryFromLight(LightShaders& shader, SceneGraph::DrawObject& samples);
	void BeginInjectionGeometryPass(int level);
	void EndInjectionGeometryPass();
	///////////////////////////////////
	/// Propagation Pass
	///////////////////////////////////
	void ComputePropagation(int nbSteps);
	///////////////////////////////////
	/// Lighting pass
	///////////////////////////////////
	void ShowDebugPropagation(TShaderPtr GBuffer, int PropagatedShow);
	Math::TVector3F GetGridPosition(int cascadeLevel)
	{
		Assert(cascadeLevel >= 0 && cascadeLevel < m_NbCascadedLevels);
		return m_GirdPosition[cascadeLevel];
	}

private:
	void GenerateGridModels();
	void CreateGridModel(SceneGraph::Group** model, int nbCellDim, int CellSize );


};

#endif /* LPV_H_ */
