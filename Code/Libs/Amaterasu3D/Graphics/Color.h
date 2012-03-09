//==========================================================
// Yes::Engine - Free C++ 3D engine
//
// Copyright (C) 2004-2005 Laurent Gomila
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
// E-mail : laurent.gom@gmail.com
//==========================================================

#ifndef COLOR_H
#define COLOR_H


//==========================================================
// En-tetes
//==========================================================
#include <iostream>
#include "glm/glm.hpp"

namespace ama3D
{
class Color
{
public:
	glm::vec4 C;

	Color(float r = 1.0, float g = 1.0, float b = 1.0, float a = 1.0) :
			C(r,g,b,a)
	{
	}

	glm::vec4 ToVec4() const
	{
		return C;
	}
};

////////////////////////////////////////////////////////////
/// Classe manipulant les couleurs
////////////////////////////////////////////////////////////
class CColor
{
public:

	//----------------------------------------------------------------
	// Constructeur par defaut
	//----------------------------------------------------------------
	CColor(unsigned long Color = 0xFFFFFFFF);

	//----------------------------------------------------------------
	// Constructeur e partir de 4 composantes
	//----------------------------------------------------------------
	CColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a =
			0xFF);

	//----------------------------------------------------------------
	// Renvoie la couleur en nuance de gris
	//----------------------------------------------------------------
	unsigned char ToGrey() const;

	//----------------------------------------------------------------
	// Renvoie la couleur sous forme ARGB
	//----------------------------------------------------------------
	unsigned long ToARGB() const;

	//----------------------------------------------------------------
	// Renvoie la couleur sous forme AGBR
	//----------------------------------------------------------------
	unsigned long ToABGR() const;

	//----------------------------------------------------------------
	// Renvoie la couleur sous forme RGBA
	//----------------------------------------------------------------
	unsigned long ToRGBA() const;

	//----------------------------------------------------------------
	// Get le canal alpha
	//----------------------------------------------------------------
	unsigned char GetAlpha() const;

	//----------------------------------------------------------------
	// Get le canal rouge
	//----------------------------------------------------------------
	unsigned char GetRed() const;

	//----------------------------------------------------------------
	// Get le canal vert
	//----------------------------------------------------------------
	unsigned char GetGreen() const;

	//----------------------------------------------------------------
	// Get le canal bleu
	//----------------------------------------------------------------
	unsigned char GetBlue() const;

	//----------------------------------------------------------------
	// Nouvelle couleur sous forme de 4 floats [0..1]
	//----------------------------------------------------------------
	void SetFloats(float r, float g, float b, float a = 1.0f);

	//----------------------------------------------------------------
	// Nouvelle couleur sous forme de 4 unsigned char [0..255]
	//----------------------------------------------------------------
	void Set(unsigned char r, unsigned char g, unsigned char b,
			unsigned char a = 0xFF);

	//----------------------------------------------------------------
	// Convertit en 4 floats
	//----------------------------------------------------------------
	void ToFloat(float Dest[]) const;

	//----------------------------------------------------------------
	// Operateur ==
	//----------------------------------------------------------------
	bool operator ==(const CColor& c) const;

	//----------------------------------------------------------------
	// Operateur !=
	//----------------------------------------------------------------
	bool operator !=(const CColor& c) const;

	//----------------------------------------------------------------
	// Operator +=
	//----------------------------------------------------------------
	const CColor& operator +=(const CColor& c);

	//----------------------------------------------------------------
	// Operator -=
	//----------------------------------------------------------------
	const CColor& operator -=(const CColor& c);

	//----------------------------------------------------------------
	// Operator +
	//----------------------------------------------------------------
	CColor operator +(const CColor& c) const;

	//----------------------------------------------------------------
	// Operator -
	//----------------------------------------------------------------
	CColor operator -(const CColor& c) const;

	//----------------------------------------------------------------
	// Operator *
	//----------------------------------------------------------------
	CColor operator *(float v) const;

	//----------------------------------------------------------------
	// Operator *=
	//----------------------------------------------------------------
	const CColor& operator *=(float v);

	//----------------------------------------------------------------
	// Operator /
	//----------------------------------------------------------------
	CColor operator /(float v) const;

	//----------------------------------------------------------------
	// Operator /=
	//----------------------------------------------------------------
	const CColor& operator /=(float v);

	//----------------------------------------------------------------
	// Ajoute deux couleurs
	//----------------------------------------------------------------
	CColor Add(const CColor& c) const;

	//----------------------------------------------------------------
	// Module deux couleurs
	//----------------------------------------------------------------
	CColor Modulate(const CColor& c) const;

	//----------------------------------------------------------------
	// Couleurs predefines
	//----------------------------------------------------------------
	static const CColor White; ///< Couleur predefine : blanc
	static const CColor Black; ///< Couleur predefine : noir
	static const CColor Red; ///< Couleur predefine : rouge
	static const CColor Green; ///< Couleur predefine : vert
	static const CColor Blue; ///< Couleur predefine : bleu

	//----------------------------------------------------------------
	// Surcharges des Operators pour les flux
	//----------------------------------------------------------------
	friend std::istream& operator >>(std::istream& Stream, CColor& Color);
	friend std::ostream& operator <<(std::ostream& Stream, const CColor& Color);

private:

	//----------------------------------------------------------------
	// Nouvelle couleur sous forme de 4 int [0..255] - sert a clamper
	// les valeurs
	//----------------------------------------------------------------
	void SetInt(int r, int g, int b, int a = 0xFF);

	//----------------------------------------------------------------
	// Donnees membres
	//----------------------------------------------------------------
	unsigned long m_Color; ///< Couleur sous forme ARGB 32 bits
};
}

#endif // COLOR_H
