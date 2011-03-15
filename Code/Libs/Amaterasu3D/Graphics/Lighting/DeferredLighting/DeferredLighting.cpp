#include "DeferredLighting.h"
#include <math.h>

DeferredLighting::DeferredLighting() :
	m_debug_mode(false)
{
	// Load shader
	// * Point Light
	m_point_light_shader = glShaderManager::Instance().LoadShader("DeferredPointLight.shader");;
	// * Spot Light
	m_spot_light_shader = glShaderManager::Instance().LoadShader("DeferredSpotLight.shader");

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
	// Go to spot pass
	m_spot_light_shader->begin();
	for(int i = 0; i < m_spots_lights.size(); i++)
	{
		m_spot_light_shader->setUniform1f("LightRaduis",m_spots_lights[i].LightRaduis);
		m_spot_light_shader->setUniform1f("LightCutOff", cos(m_spots_lights[i].LightCutOff *(M_PI / 180.0)));
		m_spot_light_shader->setUniform1f("LightIntensity", m_spots_lights[i].LightIntensity);
		m_spot_light_shader->setUniform3f("LightPosition", m_spots_lights[i].Position.x, m_spots_lights[i].Position.y, m_spots_lights[i].Position.z);
		m_spot_light_shader->setUniform3f("LightSpotDirection", m_spots_lights[i].Direction.x, m_spots_lights[i].Direction.y, m_spots_lights[i].Direction.z);
		m_spot_light_shader->setUniform3f("LightColor", m_spots_lights[i].LightColor.R, m_spots_lights[i].LightColor.G, m_spots_lights[i].LightColor.B);

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
	m_spot_light_shader->end();
	// Desactive differents buffers
	m_FBO_graphics->GetTexture("Diffuse")->desactivateMultiTex(CUSTOM_TEXTURE+0);
	m_FBO_graphics->GetTexture("Specular")->desactivateMultiTex(CUSTOM_TEXTURE+1);
	m_FBO_graphics->GetTexture("Normal")->desactivateMultiTex(CUSTOM_TEXTURE+2);
	m_FBO_graphics->GetTexture("Position")->desactivateMultiTex(CUSTOM_TEXTURE+3);
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
	PointLightPass();
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
