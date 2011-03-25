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

#ifndef DEFERREDLIGHTING_H_
#define DEFERREDLIGHTING_H_

#include <Graphics/Lighting/LightingPassAbstract.h>
#include <Graphics/GLSLShader.h>
#include <Graphics/SceneGraph/Group.h>

class DeferredLighting : public LightingPassAbstract
{
private:
	// Attributs
	TShaderPtr m_point_light_shader;
	TShaderPtr m_spot_light_shader;
	TShaderPtr m_simple_shader;
	SceneGraph::Group& m_Scene;
	bool m_debug_mode;
public:
	DeferredLighting(SceneGraph::Group& scene);
	virtual ~DeferredLighting();

	virtual void ComputeIllumination();

	// DebugMode
	bool isDebugMode() const;
	void SetDebugMode(bool v);
protected:
	void PointLightPass();
	void SpotLightPass();
};

#endif /* DEFERREDLIGHTING_H_ */
