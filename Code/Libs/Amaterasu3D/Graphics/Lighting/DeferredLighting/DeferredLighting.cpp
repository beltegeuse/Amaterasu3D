#include "DeferredLighting.h"

DeferredLighting::DeferredLighting()
{
	// Load shader
	m_point_light_shader = glShaderManager::Instance().LoadShader("DeferredPointLight.shader");
}

DeferredLighting::~DeferredLighting()
{
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
	for(int i = 0; i < m_points_light.size(); i++)
	{
		// Setup all light informations
		m_point_light_shader->setUniform1f("LightRaduis", m_points_light[i].LightRaduis);
		m_point_light_shader->setUniform1f("LightIntensity", m_points_light[i].LightIntensity);
		m_point_light_shader->setUniform3f("LightColor", m_points_light[i].LightColor.R, m_points_light[i].LightColor.G, m_points_light[i].LightColor.B);
		m_point_light_shader->setUniform3f("LightPosition", m_points_light[i].Position.x,m_points_light[i].Position.y,m_points_light[i].Position.z);
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
//	glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	// Points light pass
	PointLightPass();

//	glDisable(GL_BLEND);
}
