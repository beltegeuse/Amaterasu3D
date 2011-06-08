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

#include "LPV.h"

// STL includes
#include <algorithm>

// Amaterasu includes
#include <Utilities/Util.h>
#include <Debug/Exceptions.h>
#include <Math/SphericalCoordinates.h>
#include <Graphics/SceneGraph/Model.h>


/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
///////////// LPV Class
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

LPV::LPV(int nbCells, int sizeCells, int propagationSteps, int nbLevels) :
m_NbCellDim(nbCells), m_CellSize(NULL),m_NbPropagationStep(propagationSteps), m_NbCascadedLevels(nbLevels)
{
	// Compute the texture size required
	// * Repeat (map 3d texture into 2d)
	int Taille = sqrt(m_NbCellDim);
	m_TextureRepeat.x = NearestPowerOfTwo(Taille);
	m_TextureRepeat.y = m_NbCellDim/m_TextureRepeat.x;
	// * Final size of the texture
	m_TextureSize = m_TextureRepeat*m_NbCellDim;
	// * Fit the texture for cascaded version
	m_TextureSize.y = m_TextureSize.y*m_NbCascadedLevels;
	// * Grid position and cell size
	m_CellSize = new float[m_NbCascadedLevels];
	m_GirdPosition = new Math::TVector3F[m_NbCascadedLevels];
	for(int i = 0; i < m_NbCascadedLevels; i++)
	{
		m_CellSize[i] = sizeCells / (std::pow(2,i)); // Compute automatically the size of each cascade
		m_GirdPosition[i] = Math::TVector3F(-98.0,-98.0,-198.0);
	}

}

LPV::~LPV()
{
	// Nothings to do (thanks to smart pointers)
	//FIXME: Delete intermediary buffers (be careful with 0 id buffer)
	delete[] m_GirdPosition;
	delete[] m_CellSize;
	delete[] m_GridModels; // TODO: Check Sceengraph destruction
}

void LPV::Initialize()
{
	///////////////////////////////
	// Shader loading
	///////////////////////////////
	// Injection shaders
	m_LPVInjectVPL = CShaderManager::Instance().LoadShader("LPVInjectVPL.shader");
	m_LPVInjectGeomerty = CShaderManager::Instance().LoadShader("LPVInjectGeometry.shader");
	// Propagation shaders
	m_LPVPropagationShader = CShaderManager::Instance().LoadShader("LPVPropagation.shader");
	m_LPVBlend = CShaderManager::Instance().LoadShader("LPVBlend.shader");
	// Lighting shaders
	m_LPVLightingShader = CShaderManager::Instance().LoadShader("LPVLighting.shader");
	///////////////////////////////
	// Update size FBO
	///////////////////////////////
	m_LPVInjectVPL->GetFBO()->SetSize(m_TextureSize);
	m_LPVInjectGeomerty->GetFBO()->SetSize(m_TextureSize);
	m_LPVPropagationShader->GetFBO()->SetSize(m_TextureSize);
	m_LPVBlend->GetFBO()->SetSize(m_TextureSize);
	//////////////////////////////////
	// Create Graphics resources
	//////////////////////////////////
	m_PropagationFBOs = new FBO*[m_NbPropagationStep];
	m_PropagationFBOs[0] = m_LPVPropagationShader->GetFBO(); // No copy the first
	for(int i = 1; i < m_NbPropagationStep; i++)
	{
		Logger::Log() << "[INFO] Create Copy Propagation FBO : " << i << "\n";
		m_PropagationFBOs[i] = m_LPVPropagationShader->GetFBO()->Copy();
	}
	/////////////////////////////
	// Create model
	/////////////////////////////
	GenerateGridModels();
}

void LPV::BeginInjectionVPLPass(int level)
{
	Assert(level >= 0 && level < m_NbCascadedLevels);
	// Change OpenGL states
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE,GL_ONE);
	glDisable(GL_DEPTH_TEST);
	// Enable shader and send uniform values
	m_LPVInjectVPL->Begin();
//	m_LPVInjectVPL->SetUniform1i("CurrentLevel", level);
	SetGridInformations(m_LPVInjectVPL);
}
void LPV::EndInjectionVPLPass()
{
	// Close the shader
	m_LPVInjectVPL->End();
	// Restore OpenGL states
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

void LPV::InjectVPLFromLight(LightShaders& light, SceneGraph::DrawObject& samples)
{
	light.GetFBO()->GetTexture("Flux")->activateMultiTex(CUSTOM_TEXTURE+0);
	light.GetFBO()->GetTexture("Depth")->activateMultiTex(CUSTOM_TEXTURE+1);
	light.GetFBO()->GetTexture("Normal")->activateMultiTex(CUSTOM_TEXTURE+2);
	ShaderHelperUniformPosition(m_LPVInjectVPL, light.GetProjectionMatrix(), light.GetViewMatrix(), 1.0, light.LightRaduis);
	samples.Draw();
	light.GetFBO()->GetTexture("Flux")->desactivateMultiTex(CUSTOM_TEXTURE+0);
	light.GetFBO()->GetTexture("Depth")->desactivateMultiTex(CUSTOM_TEXTURE+1);
	light.GetFBO()->GetTexture("Normal")->desactivateMultiTex(CUSTOM_TEXTURE+2);
}

void LPV::InjectGeometryFromLight(LightShaders& shader, SceneGraph::DrawObject& samples)
{
	shader.GetFBO()->GetTexture("Depth")->activateMultiTex(CUSTOM_TEXTURE+2);
	shader.GetFBO()->GetTexture("Normal")->activateMultiTex(CUSTOM_TEXTURE+1);
	ShaderHelperUniformPosition(m_LPVInjectGeomerty, shader.GetProjectionMatrix(), shader.GetViewMatrix(), 1.0, shader.LightRaduis);
	m_LPVInjectGeomerty->SetUniformVector("ObsPosition", shader.Position);
	samples.Draw();
	shader.GetFBO()->GetTexture("Depth")->desactivateMultiTex(CUSTOM_TEXTURE+2);
	shader.GetFBO()->GetTexture("Normal")->desactivateMultiTex(CUSTOM_TEXTURE+1);
}

void LPV::BeginInjectionGeometryPass(int level)
{
	Assert(level >= 0 && level < m_NbCascadedLevels);
	// Set OpenGL States
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_ONE,GL_ONE);
	// Enable shader and send uniform values
	m_LPVInjectGeomerty->Begin();
//	m_LPVInjectGeomerty->SetUniform1i("CurrentLevel", level);
	SetGridInformations(m_LPVInjectGeomerty);
}

void LPV::EndInjectionGeometryPass()
{
	m_LPVInjectGeomerty->End();
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

void LPV::ComputePropagation(int nbSteps)
{
	Assert(nbSteps >= 0);
	int steps = std::min(nbSteps, m_NbPropagationStep);
	for(int i = 0; i < steps; i++)
	{
		m_LPVPropagationShader->SetFBO(m_PropagationFBOs[i], false);

		m_LPVPropagationShader->Begin();

		m_LPVPropagationShader->SetUniformVector("LPVSize",Math::TVector4F(m_TextureSize.x,m_TextureSize.y,m_TextureRepeat.x,m_TextureRepeat.y));
		m_LPVPropagationShader->SetUniform1i("LPVNbCell", m_NbCellDim);
		m_LPVPropagationShader->SetUniform1i("DoOcclusion", true);
		m_LPVInjectGeomerty->GetFBO()->GetTexture("Grid")->activateMultiTex(CUSTOM_TEXTURE+3);

		if(i == 0)
		{
			m_LPVInjectVPL->GetFBO()->GetTexture("GridRed")->activateMultiTex(CUSTOM_TEXTURE+0);
			m_LPVInjectVPL->GetFBO()->GetTexture("GridGreen")->activateMultiTex(CUSTOM_TEXTURE+1);
			m_LPVInjectVPL->GetFBO()->GetTexture("GridBlue")->activateMultiTex(CUSTOM_TEXTURE+2);
		}
		else
		{
			m_PropagationFBOs[i-1]->GetTexture("GridRed")->activateMultiTex(CUSTOM_TEXTURE+0);
			m_PropagationFBOs[i-1]->GetTexture("GridGreen")->activateMultiTex(CUSTOM_TEXTURE+1);
			m_PropagationFBOs[i-1]->GetTexture("GridBlue")->activateMultiTex(CUSTOM_TEXTURE+2);
		}

		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0);
			glVertex2f(-1.0, -1.0);
			glTexCoord2f(0.0, 1.0);
			glVertex2f(-1.0, 1.0);
			glTexCoord2f(1.0, 1.0);
			glVertex2f(1.0, 1.0);
			glTexCoord2f(1.0, 0.0);
			glVertex2f(1.0, -1.0);
		glEnd();

		if(i == 0)
		{
			m_LPVInjectVPL->GetFBO()->GetTexture("GridRed")->desactivateMultiTex(CUSTOM_TEXTURE+0);
			m_LPVInjectVPL->GetFBO()->GetTexture("GridGreen")->desactivateMultiTex(CUSTOM_TEXTURE+1);
			m_LPVInjectVPL->GetFBO()->GetTexture("GridBlue")->desactivateMultiTex(CUSTOM_TEXTURE+2);
		}
		else
		{
			m_PropagationFBOs[i-1]->GetTexture("GridRed")->desactivateMultiTex(CUSTOM_TEXTURE+0);
			m_PropagationFBOs[i-1]->GetTexture("GridGreen")->desactivateMultiTex(CUSTOM_TEXTURE+1);
			m_PropagationFBOs[i-1]->GetTexture("GridBlue")->desactivateMultiTex(CUSTOM_TEXTURE+2);
		}
		m_LPVInjectGeomerty->GetFBO()->GetTexture("Grid")->desactivateMultiTex(CUSTOM_TEXTURE+3);
		m_LPVPropagationShader->End();
	}
	m_LPVBlend->Begin();
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_ONE,GL_ONE);
	for(int i = 0; i < steps+1; i++)
	{
		if(i == 0)
		{
			m_LPVInjectVPL->GetFBO()->GetTexture("GridRed")->activateMultiTex(CUSTOM_TEXTURE+0);
			m_LPVInjectVPL->GetFBO()->GetTexture("GridGreen")->activateMultiTex(CUSTOM_TEXTURE+1);
			m_LPVInjectVPL->GetFBO()->GetTexture("GridBlue")->activateMultiTex(CUSTOM_TEXTURE+2);
		}
		else
		{
			m_PropagationFBOs[i-1]->GetTexture("GridRed")->activateMultiTex(CUSTOM_TEXTURE+0);
			m_PropagationFBOs[i-1]->GetTexture("GridGreen")->activateMultiTex(CUSTOM_TEXTURE+1);
			m_PropagationFBOs[i-1]->GetTexture("GridBlue")->activateMultiTex(CUSTOM_TEXTURE+2);
		}
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0);
			glVertex2f(-1.0, -1.0);
			glTexCoord2f(0.0, 1.0);
			glVertex2f(-1.0, 1.0);
			glTexCoord2f(1.0, 1.0);
			glVertex2f(1.0, 1.0);
			glTexCoord2f(1.0, 0.0);
			glVertex2f(1.0, -1.0);
		glEnd();
		if(i == 0)
		{
			m_LPVInjectVPL->GetFBO()->GetTexture("GridRed")->desactivateMultiTex(CUSTOM_TEXTURE+0);
			m_LPVInjectVPL->GetFBO()->GetTexture("GridGreen")->desactivateMultiTex(CUSTOM_TEXTURE+1);
			m_LPVInjectVPL->GetFBO()->GetTexture("GridBlue")->desactivateMultiTex(CUSTOM_TEXTURE+2);
		}
		else
		{
			m_PropagationFBOs[i-1]->GetTexture("GridRed")->desactivateMultiTex(CUSTOM_TEXTURE+0);
			m_PropagationFBOs[i-1]->GetTexture("GridGreen")->desactivateMultiTex(CUSTOM_TEXTURE+1);
			m_PropagationFBOs[i-1]->GetTexture("GridBlue")->desactivateMultiTex(CUSTOM_TEXTURE+2);
		}
	}
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	m_LPVBlend->End();
}

void LPV::ShowDebugPropagation(TShaderPtr GBuffer, int PropagatedShow)
{
	// ******* 4th Step : Filtrage pass
	// WARNING : Don't forgot to add uniform
	m_LPVLightingShader->Begin();
	GBuffer->GetFBO()->GetTexture("Depth")->activateMultiTex(CUSTOM_TEXTURE+0);
	GBuffer->GetFBO()->GetTexture("Normal")->activateMultiTex(CUSTOM_TEXTURE+1);
	GBuffer->GetFBO()->GetTexture("Diffuse")->activateMultiTex(CUSTOM_TEXTURE+5);
	if(PropagatedShow < 0)
	{
		m_LPVInjectVPL->GetFBO()->GetTexture("GridRed")->activateMultiTex(CUSTOM_TEXTURE+2);
		m_LPVInjectVPL->GetFBO()->GetTexture("GridGreen")->activateMultiTex(CUSTOM_TEXTURE+3);
		m_LPVInjectVPL->GetFBO()->GetTexture("GridBlue")->activateMultiTex(CUSTOM_TEXTURE+4);
	}
	else
	{
		m_PropagationFBOs[PropagatedShow]->GetTexture("GridRed")->activateMultiTex(CUSTOM_TEXTURE+2);
		m_PropagationFBOs[PropagatedShow]->GetTexture("GridGreen")->activateMultiTex(CUSTOM_TEXTURE+3);
		m_PropagationFBOs[PropagatedShow]->GetTexture("GridBlue")->activateMultiTex(CUSTOM_TEXTURE+4);
	}
	m_LPVBlend->GetFBO()->GetTexture("GridRed")->activateMultiTex(CUSTOM_TEXTURE+2);
	m_LPVBlend->GetFBO()->GetTexture("GridGreen")->activateMultiTex(CUSTOM_TEXTURE+3);
	m_LPVBlend->GetFBO()->GetTexture("GridBlue")->activateMultiTex(CUSTOM_TEXTURE+4);
	SetGridInformations(m_LPVLightingShader);
	//m_LPVLightingShader->SetUniform1i("EnableTrilinearInterpolation",true);
	ShaderHelperUniformPositionFromView(m_LPVLightingShader);
	// Draw ...
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex2f(-1.0, -1.0);
		glTexCoord2f(0.0, 1.0);
		glVertex2f(-1.0, 1.0);
		glTexCoord2f(1.0, 1.0);
		glVertex2f(1.0, 1.0);
		glTexCoord2f(1.0, 0.0);
		glVertex2f(1.0, -1.0);
	glEnd();
	//FIXME: Same pattern ???
	m_LPVBlend->GetFBO()->GetTexture("GridRed")->desactivateMultiTex(CUSTOM_TEXTURE+2);
	m_LPVBlend->GetFBO()->GetTexture("GridGreen")->desactivateMultiTex(CUSTOM_TEXTURE+3);
	m_LPVBlend->GetFBO()->GetTexture("GridBlue")->desactivateMultiTex(CUSTOM_TEXTURE+4);
	GBuffer->GetFBO()->GetTexture("Depth")->desactivateMultiTex(CUSTOM_TEXTURE+0);
	GBuffer->GetFBO()->GetTexture("Normal")->desactivateMultiTex(CUSTOM_TEXTURE+1);
	GBuffer->GetFBO()->GetTexture("Diffuse")->desactivateMultiTex(CUSTOM_TEXTURE+5);
	m_LPVLightingShader->End();
}


void LPV::ComputeGridPosition(CameraAbstract* Camera)
{
	//////////////////////////////////
	// Compute Coordinates with orientation
	//////////////////////////////////
	Math::TVector3F cameraDirection = Camera->GetTarget() - Camera->GetPosition();
	cameraDirection.Normalize(); // To be sure ...
	// Inverse the direction to project on the Cube
	cameraDirection = -cameraDirection;

	for(int i = 0; i < m_NbCascadedLevels; i++)
	{
		const float borderFactor = (m_NbCellDim/4)*m_CellSize[i];

		//////////////////////////////////
		// Compute Coordinates with position
		//////////////////////////////////
		// Place on the center of the LPV
		m_GirdPosition[i] = Camera->GetPosition() - (2*borderFactor)*Math::TVector3F(1.0,1.0,1.0);

		const float cubeBorder = 1.0 / sqrt(2);

		// \forgot Inverse vector coordinates to fit OpenGL representation
		Math::TVector3F cubeCoordinates = Math::TVector3F(
				std::max(std::min(cubeBorder, cameraDirection.x),-cubeBorder),
				std::max(std::min(cubeBorder, cameraDirection.y),-cubeBorder),
				std::max(std::min(cubeBorder, cameraDirection.z),-cubeBorder)) / cubeBorder;

		m_GirdPosition[i] -= cubeCoordinates*borderFactor;

		///////////////////////////////////////////
		// Snapping
		///////////////////////////////////////////
		m_GirdPosition[i] = Math::TVector3F(
				floor(m_GirdPosition[i].x / m_CellSize[i])*m_CellSize[i],
				floor(m_GirdPosition[i].y / m_CellSize[i])*m_CellSize[i],
				floor(m_GirdPosition[i].z / m_CellSize[i])*m_CellSize[i]);
	}
}

void LPV::GenerateGridModels()
{
	Logger::Log() << "[INFO] Generate Grid models \n";
	m_GridModels = new SceneGraph::Group*[m_NbCascadedLevels];
	for(int i = 0; i < m_NbCascadedLevels; i++)
	{
		Logger::Log() << "   * Generate Level : " <<  i << "\n";
		CreateGridModel(&m_GridModels[i], m_NbCellDim, m_CellSize[i]);
	}
	Logger::Log() << "[INFO] Generate Grid models (END) \n";
}

void LPV::CreateGridModel(SceneGraph::Group** GirdModel, int nbCellDim, int CellSize )
{
	// Allocation des buffers
	float * vertexBuffer = new float[3*nbCellDim*nbCellDim*3*2];
	float * colorBuffer = new float[3*nbCellDim*nbCellDim*3*2];
	unsigned int* indiceBuffer = new unsigned int[3*nbCellDim*nbCellDim*2];
	unsigned int i = 0;
	Color color(1.0,1.0,1.0);
	// Fill in buffers
	for(int z=0;z<nbCellDim;z++){
		for(int x=0;x<nbCellDim;x++){
			vertexBuffer[i] = x*CellSize;
			vertexBuffer[i+1] = 0;
			vertexBuffer[i+2] = z*CellSize;
			colorBuffer[i] = color.R;
			colorBuffer[i+1] = color.G;
			colorBuffer[i+2] = color.B;
			i += 3;

			vertexBuffer[i] = x*CellSize;
			vertexBuffer[i+1] = nbCellDim*CellSize;
			vertexBuffer[i+2] = z*CellSize;
			colorBuffer[i] = color.R;
			colorBuffer[i+1] = color.G;
			colorBuffer[i+2] = color.B;
			i += 3;
		}

		for(int y=0;y<nbCellDim;y++){
			vertexBuffer[i] = 0;
			vertexBuffer[i+1] = y*CellSize;
			vertexBuffer[i+2] = z*CellSize;
			colorBuffer[i] = color.R;
			colorBuffer[i+1] = color.G;
			colorBuffer[i+2] = color.B;
			i += 3;

			vertexBuffer[i] = nbCellDim*CellSize;
			vertexBuffer[i+1] = y*CellSize;
			vertexBuffer[i+2] = z*CellSize;
			colorBuffer[i] = color.R;
			colorBuffer[i+1] = color.G;
			colorBuffer[i+2] = color.B;
			i += 3;
		}
	}

	for(int y=0;y<nbCellDim;y++){
		for(int x=0;x<nbCellDim;x++){
			vertexBuffer[i] = x*CellSize;
			vertexBuffer[i+1] = y*CellSize;
			vertexBuffer[i+2] = 0;
			colorBuffer[i] = color.R;
			colorBuffer[i+1] = color.G;
			colorBuffer[i+2] = color.B;
			i += 3;

			vertexBuffer[i] = x*CellSize;
			vertexBuffer[i+1] = y*CellSize;
			vertexBuffer[i+2] = nbCellDim*CellSize;
			colorBuffer[i] = color.R;
			colorBuffer[i+1] = color.G;
			colorBuffer[i+2] = color.B;
			i += 3;
		}
	}

	for(int l=0; l < 3*nbCellDim*nbCellDim*2; l++)
	{
		indiceBuffer[l] = l;
	}

	SceneGraph::Model* model = new SceneGraph::Model;
	model->SetDrawMode(GL_LINES);
	model->SetIndiceBuffer(indiceBuffer, 3*nbCellDim*nbCellDim*2);
	SceneGraph::ModelBuffer buffer;
	buffer.buffer = vertexBuffer;
	buffer.size = 3*nbCellDim*nbCellDim*3*2;
	buffer.dimension = 3;
	buffer.owner = true;
	model->AddBuffer(buffer, VERTEX_ATTRIBUT);
	buffer.buffer = colorBuffer;
	model->AddBuffer(buffer, COLOR_ATTRIBUT);
	model->CompileBuffers();
	model->AddMaterial(DIFFUSE_MATERIAL,color);

	(*GirdModel) = new SceneGraph::Group;
	(*GirdModel)->AddChild(model);

}

void LPV::DrawGrids()
{
	for(int i = 0; i < m_NbCascadedLevels; i++)
	{
		DrawGrid(i);
	}
}

void LPV::DrawGrid(int level)
{
	Assert(level >= 0 && level < m_NbCascadedLevels);
	Math::CMatrix4 matGrid;
	Math::TVector3F gridPos = GetGridPosition(level);
	matGrid.SetTranslation(gridPos.x,gridPos.y,gridPos.z);
	m_GridModels[level]->LoadTransformMatrix(matGrid);
	m_GridModels[level]->Draw();
}

/////////////////////////////
/// Old code
/////////////////////////////
// Compute Transform grid matrix
//Math::CMatrix4 transGrid;
//{
//	Math::CMatrix4 rotationGird;
//	Math::CMatrix4 rotationGird2;
//	Math::TVector3F directionView = m_Camera->GetTarget() - m_Camera->GetPosition();
//	directionView.Normalize();
//	Math::SphericalCoordinates gridSphericalCoords(Math::TVector3F(directionView.x, directionView.z, directionView.y));
//	rotationGird.SetRotationY(gridSphericalCoords.GetTheta());
//	rotationGird2.SetRotationZ(gridSphericalCoords.GetPhy()-(M_PI/2.0));
//	//Logger::Log() << "Dir : " << m_Camera->GetTarget() - m_Camera->GetPosition() << " Theta : " << gridSphericalCoords.GetTheta() << " Phy : " << gridSphericalCoords.GetPhy() << "\n";
//	//Math::CMatrix4 transGrid = MatrixManager.GetMatrix(VIEW_MATRIX);
//	Math::TVector3F cameraPos = m_Camera->GetPosition();
//	transGrid.SetTranslation(cameraPos.x,cameraPos.y,cameraPos.z);
//	Math::CMatrix4 offsetGrid;
//	offsetGrid.SetTranslation(-8*m_CellSize.x,-(m_NbCellDim/2)*m_CellSize.y,-(m_NbCellDim/2)*m_CellSize.z);
//	transGrid = offsetGrid*rotationGird2*rotationGird*transGrid;
//}

