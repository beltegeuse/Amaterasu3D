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
#ifndef LIGHTINGSTRUCTURES_H_
#define LIGHTINGSTRUCTURES_H_

#include <Math/Vector3.h>
#include <Graphics/Color.h>

//XXX Add heritage structure

struct PointLight
{
	Math::TVector3F Position;
	Color LightColor;
	float LightRaduis;
	float LightIntensity;
};

struct SpotLight
{
	Math::TVector3F Position;
	Math::TVector3F Direction;
	Color LightColor;
	float LightIntensity;
	float LightRaduis;
	float LightCutOff;
};

// FIXME: Raduis ?????
struct SunLight
{
	Math::TVector3F Position;
	Math::TVector3F Direction;
	Color LightColor;
	float LightIntensity;
	float LightRaduis;
};

#endif /* LIGHTINGSTRUCTURES_H_ */
