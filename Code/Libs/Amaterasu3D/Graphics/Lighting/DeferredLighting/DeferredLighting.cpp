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
#include "DeferredLighting.h"
#include <Graphics/MatrixManagement.h>
#include <Graphics/SceneManager.h>
#include <System/SettingsManager.h>
#include <math.h>

DeferredLighting::DeferredLighting() :
	m_debug_mode(false)
{
	// Load shader
	// * Point Light
	m_point_light_shader = CShaderManager::Instance().LoadShader("DeferredPointLight.shader");;
	// * Spot Light
	m_spot_light_shader = CShaderManager::Instance().LoadShader("DeferredSpotLight.shader");
	// * simple shader for the shadow map
	m_simple_shader = CShaderManager::Instance().LoadShader("ShadowMap.shader");
	SetDebugMode(m_debug_mode);
}

DeferredLighting::~DeferredLighting()
{
}

void DeferredLighting::SpotLightPass()
{
	// Active differents buffers
	m_FBO_graphics->GetTexture("Diffuse")->activateMultiTex(CUSTOM_TEXTURE+0);
	m_FBO_graphics->GetTexture("Specular")->activateMultiTex(CUSTOM_TEXTURE+1);
	m_FBO_graphics->GetTexture("Normal")->activateMultiTex(CUSTOM_TEXTURE+2);
	m_FBO_graphics->GetTexture("Depth")->activateMultiTex(CUSTOM_TEXTURE+3);

	// Update Info
	m_spot_light_shader->Begin();
	ShaderHelperUniformPositionFromView(m_spot_light_shader);
	m_spot_light_shader->End();

	for(int i = 0; i < m_spots_lights.size(); i++)
	{
		// Generate the Shadow Map
		// * Transformations
		Math::CMatrix4 oldProjectionMatrix = CMatrixManager::Instance().GetMatrix(PROJECTION_MATRIX);
		Math::CMatrix4 oldViewMatrix = CMatrixManager::Instance().GetMatrix(VIEW_MATRIX);
		Math::CMatrix4 LightViewMatrix;
		LightViewMatrix.LookAt(m_spots_lights[i].Position, m_spots_lights[i].Direction);
		Math::CMatrix4 LightProjectionMatrix;
		CMatrixManager::Instance().SetProjectionMatrix(Math::CMatrix4::CreatePerspectiveFOV(m_spots_lights[i].LightCutOff, 512.0/512.0, 1.0, m_spots_lights[i].LightRaduis));
		CMatrixManager::Instance().SetViewMatrix(LightViewMatrix);
		// * Draw the scene
//		glEnable(GL_CULL_FACE);
//		glCullFace(GL_BACK);
		m_simple_shader->Begin();
		CSceneManager::Instance().RenderAll(); // Draw the scene
		m_simple_shader->End();
//		glDisable(GL_CULL_FACE);
		// * Revert transformations
		CMatrixManager::Instance().SetProjectionMatrix(oldProjectionMatrix);
		CMatrixManager::Instance().SetViewMatrix(oldViewMatrix);
		// * Give Depth Texture
		m_simple_shader->GetFBO()->GetTexture("Depth")->activateMultiTex(CUSTOM_TEXTURE+4);

//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Go to spot pass
		m_spot_light_shader->Begin();
		// * Light propreties
		m_spot_light_shader->SetUniform1f("LightRaduis",m_spots_lights[i].LightRaduis);
		m_spot_light_shader->SetUniform1f("LightCutOff", cos(m_spots_lights[i].LightCutOff *(M_PI / 180.0)));
		m_spot_light_shader->SetUniform1f("LightIntensity", m_spots_lights[i].LightIntensity);
		m_spot_light_shader->SetUniformVector("LightPosition", m_spots_lights[i].Position);
		m_spot_light_shader->SetUniformVector("LightSpotDirection", m_spots_lights[i].Direction);
		m_spot_light_shader->SetUniformColor("LightColor", m_spots_lights[i].LightColor);
		// * Shadow Map propreties
		m_spot_light_shader->SetUniformMatrix4fv("LightViewMatrix", LightViewMatrix);
		m_spot_light_shader->SetUniformMatrix4fv("LightProjectionMatrix", LightProjectionMatrix);
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

		m_spot_light_shader->End();
	}

	// Desactive differents buffers
	m_FBO_graphics->GetTexture("Diffuse")->desactivateMultiTex(CUSTOM_TEXTURE+0);
	m_FBO_graphics->GetTexture("Specular")->desactivateMultiTex(CUSTOM_TEXTURE+1);
	m_FBO_graphics->GetTexture("Normal")->desactivateMultiTex(CUSTOM_TEXTURE+2);
	m_FBO_graphics->GetTexture("Depth")->desactivateMultiTex(CUSTOM_TEXTURE+3);
	m_simple_shader->GetFBO()->GetTexture("Depth")->desactivateMultiTex(CUSTOM_TEXTURE+4);
}

void DeferredLighting::PointLightPass()
{
	// Active differents buffers
	m_FBO_graphics->GetTexture("Diffuse")->activateMultiTex(CUSTOM_TEXTURE+0);
	m_FBO_graphics->GetTexture("Specular")->activateMultiTex(CUSTOM_TEXTURE+1);
	m_FBO_graphics->GetTexture("Normal")->activateMultiTex(CUSTOM_TEXTURE+2);
	m_FBO_graphics->GetTexture("Position")->activateMultiTex(CUSTOM_TEXTURE+3);
	// Go light pass
	m_point_light_shader->Begin();
	for(int i = 0; i < m_points_lights.size(); i++)
	{
		// Setup all light informations
		m_point_light_shader->SetUniform1f("LightRaduis", m_points_lights[i].LightRaduis);
		m_point_light_shader->SetUniform1f("LightIntensity", m_points_lights[i].LightIntensity);
		m_point_light_shader->SetUniformColor("LightColor", m_points_lights[i].LightColor);
		m_point_light_shader->SetUniformVector("LightPosition", m_points_lights[i].Position);

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
	}
	m_point_light_shader->End();
	// Desactive differents buffers
	m_FBO_graphics->GetTexture("Diffuse")->desactivateMultiTex(CUSTOM_TEXTURE+0);
	m_FBO_graphics->GetTexture("Specular")->desactivateMultiTex(CUSTOM_TEXTURE+1);
	m_FBO_graphics->GetTexture("Normal")->desactivateMultiTex(CUSTOM_TEXTURE+2);
	m_FBO_graphics->GetTexture("Position")->desactivateMultiTex(CUSTOM_TEXTURE+3);
}

void DeferredLighting::ComputeIllumination()
{
	// Initialise OpenGL states
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
//    glDisable(GL_DEPTH_TEST);
	// Points light pass
	//PointLightPass();
	SpotLightPass();

	/*if(m_debug_mode)
		m_simple_shader->GetFBO()->DrawDebug();*/

	glBlendFunc(GL_SRC_ALPHA,GL_ZERO);
	glDisable(GL_BLEND);
//	glEnable(GL_DEPTH_TEST);
}

bool DeferredLighting::isDebugMode() const
{
	return m_debug_mode;
}

void DeferredLighting::SetDebugMode(bool v)
{
	m_debug_mode = v;
	m_point_light_shader->Begin();
	m_point_light_shader->SetUniform1i("DebugMode",m_debug_mode);
	m_point_light_shader->End();
	m_spot_light_shader->Begin();
	m_spot_light_shader->SetUniform1i("DebugMode",m_debug_mode);
	m_spot_light_shader->End();
}
