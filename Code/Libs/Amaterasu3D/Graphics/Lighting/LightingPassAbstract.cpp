#include "LightingPassAbstract.h"

LightingPassAbstract::LightingPassAbstract() {
}

LightingPassAbstract::~LightingPassAbstract() {
}

void LightingPassAbstract::SetFBOGraphicBuffer(FBO* fbo)
{
	m_FBO_graphics = fbo;
}

void LightingPassAbstract::AddPointLight(const PointLight& light)
{
	m_points_lights.push_back(light);
}

void LightingPassAbstract::AddSpotLight(const SpotLight& light)
{
	m_spots_lights.push_back(light);
}
