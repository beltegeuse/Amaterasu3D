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
	m_points_light.add(light);
}
