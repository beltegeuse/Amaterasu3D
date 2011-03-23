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
#include <math.h>

DeferredLighting::DeferredLighting(Window* win) :
	m_debug_mode(false),
	m_window(win)
{
	// Load shader
	// * Point Light
	m_point_light_shader = glShaderManager::Instance().LoadShader("DeferredPointLight.shader");;
	// * Spot Light
	m_spot_light_shader = glShaderManager::Instance().LoadShader("DeferredSpotLight.shader");
	// * simple shader for the shadow map
	m_simple_shader = glShaderManager::Instance().LoadShader("ShadowMap.shader");
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
	m_FBO_graphics->GetTexture("Position")->activateMultiTex(CUSTOM_TEXTURE+3);

	for(int i = 0; i < m_spots_lights.size(); i++)
	{
		// Generate the Shadow Map
		// * Transformations
		Math::CMatrix4 oldProjectionMatrix = MatrixManagement::Instance().GetMatrix(PROJECTION_MATRIX);
		Math::CMatrix4 oldViewMatrix = MatrixManagement::Instance().GetMatrix(VIEW_MATRIX);
		Math::CMatrix4 LightViewMatrix;
		LightViewMatrix.LookAt(m_spots_lights[i].Position, m_spots_lights[i].Direction);
		Math::CMatrix4 LightProjectionMatrix;
		LightProjectionMatrix.PerspectiveFOV(m_spots_lights[i].LightCutOff, 800.0/600.0, 1.0, m_spots_lights[i].LightRaduis);
		MatrixManagement::Instance().SetProjectionMatrix(LightProjectionMatrix);
		MatrixManagement::Instance().SetViewMatrix(LightViewMatrix);
		// * Draw the scene
//		glEnable(GL_CULL_FACE);
//		glCullFace(GL_BACK);
		m_simple_shader->begin();
		m_window->GetSceneRoot().Draw(); // Draw the scene
		m_simple_shader->end();
//		glDisable(GL_CULL_FACE);
		// * Revert transformations
		MatrixManagement::Instance().SetProjectionMatrix(oldProjectionMatrix);
		MatrixManagement::Instance().SetViewMatrix(oldViewMatrix);
		// * Give Depth Texture
		m_simple_shader->GetFBO()->GetTexture("Depth")->activateMultiTex(CUSTOM_TEXTURE+4);

		// Go to spot pass
		m_spot_light_shader->begin();
		// * Light propreties
		m_spot_light_shader->setUniform1f("LightRaduis",m_spots_lights[i].LightRaduis);
		m_spot_light_shader->setUniform1f("LightCutOff", cos(m_spots_lights[i].LightCutOff *(M_PI / 180.0)));
		m_spot_light_shader->setUniform1f("LightIntensity", m_spots_lights[i].LightIntensity);
		m_spot_light_shader->setUniform3f("LightPosition", m_spots_lights[i].Position.x, m_spots_lights[i].Position.y, m_spots_lights[i].Position.z);
		m_spot_light_shader->setUniform3f("LightSpotDirection", m_spots_lights[i].Direction.x, m_spots_lights[i].Direction.y, m_spots_lights[i].Direction.z);
		m_spot_light_shader->setUniform3f("LightColor", m_spots_lights[i].LightColor.R, m_spots_lights[i].LightColor.G, m_spots_lights[i].LightColor.B);
		// * Shadow Map propreties
		m_spot_light_shader->setUniformMatrix4fv("LightViewMatrix", LightViewMatrix);
		m_spot_light_shader->setUniformMatrix4fv("LightProjectionMatrix", LightProjectionMatrix);
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

		m_spot_light_shader->end();
	}

	// Desactive differents buffers
	m_FBO_graphics->GetTexture("Diffuse")->desactivateMultiTex(CUSTOM_TEXTURE+0);
	m_FBO_graphics->GetTexture("Specular")->desactivateMultiTex(CUSTOM_TEXTURE+1);
	m_FBO_graphics->GetTexture("Normal")->desactivateMultiTex(CUSTOM_TEXTURE+2);
	m_FBO_graphics->GetTexture("Position")->desactivateMultiTex(CUSTOM_TEXTURE+3);
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
	m_point_light_shader->begin();
	for(int i = 0; i < m_points_lights.size(); i++)
	{
		// Setup all light informations
		m_point_light_shader->setUniform1f("LightRaduis", m_points_lights[i].LightRaduis);
		m_point_light_shader->setUniform1f("LightIntensity", m_points_lights[i].LightIntensity);
		m_point_light_shader->setUniform3f("LightColor", m_points_lights[i].LightColor.R, m_points_lights[i].LightColor.G, m_points_lights[i].LightColor.B);
		m_point_light_shader->setUniform3f("LightPosition", m_points_lights[i].Position.x,m_points_lights[i].Position.y,m_points_lights[i].Position.z);

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
	m_point_light_shader->end();
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
	// Points light pass
	//PointLightPass();
	SpotLightPass();

	glBlendFunc(GL_SRC_ALPHA,GL_ZERO);
	glDisable(GL_BLEND);
}

bool DeferredLighting::isDebugMode() const
{
	return m_debug_mode;
}

void DeferredLighting::SetDebugMode(bool v)
{
	m_debug_mode = v;
	m_point_light_shader->begin();
	m_point_light_shader->setUniform1i("DebugMode",m_debug_mode);
	m_point_light_shader->end();
	m_spot_light_shader->begin();
	m_spot_light_shader->setUniform1i("DebugMode",m_debug_mode);
	m_spot_light_shader->end();
}
