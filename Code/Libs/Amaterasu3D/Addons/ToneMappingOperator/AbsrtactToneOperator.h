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

#ifndef ABSRTACTTONEOPERATOR_H_
#define ABSRTACTTONEOPERATOR_H_

#include <Graphics/FBO.h>

namespace ama3D
{
class AbsrtactToneOperator
{
public:
	AbsrtactToneOperator();
	virtual ~AbsrtactToneOperator();

	/*
	 * Virtual pure methods
	 */
	virtual Texture* GetLDRTexture() = 0;
	virtual void Compress(Texture* HDRBuffer) = 0;
	virtual void DrawDebug() = 0;
};
}

#endif /* ABSRTACTTONEOPERATOR_H_ */
