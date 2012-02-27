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

//==========================================================
// En-tetes
//==========================================================
#include "Color.h"

namespace ama3D
{
/////////////////////////////////////////////////////////////
// Donnees membres statiques
/////////////////////////////////////////////////////////////
const CColor CColor::White(255, 255, 255);
const CColor CColor::Black(0, 0, 0);
const CColor CColor::Red(255, 0, 0);
const CColor CColor::Green(0, 255, 0);
const CColor CColor::Blue(0, 0, 255);

/////////////////////////////////////////////////////////////
/// Constructeur par defaut
///
/// \param Color : Couleur sous forme ARGB 32 bits
///
////////////////////////////////////////////////////////////
CColor::CColor(unsigned long Color) :
		m_Color(Color)
{

}

/////////////////////////////////////////////////////////////
/// Constructeur a partir de composantes 
///
/// \param r : Canal rouge
/// \param g : Canal vert
/// \param b : Canal bleu
/// \param a : Canal alpha
///
////////////////////////////////////////////////////////////
CColor::CColor(unsigned char r, unsigned char g, unsigned char b,
		unsigned char a)
{
	Set(r, g, b, a);
}

/////////////////////////////////////////////////////////////
/// Met a jour la couleur a partir de 4 composantes flottantes
///
/// \param r : Canal rouge
/// \param g : Canal vert
/// \param b : Canal bleu
/// \param a : Canal alpha
///
////////////////////////////////////////////////////////////
void CColor::SetFloats(float r, float g, float b, float a)
{
	int R = static_cast<int>(r * 255.0f);int G = static_cast<int>(g * 255.0f);
	int B = static_cast<int>(b * 255.0f);
	int A = static_cast<int>(a * 255.0f);

	SetInt(R, G, B, A);
}

/////////////////////////////////////////////////////////////
/// Met a jour la couleur a partir de 4 composantes
///
/// \param r : Canal rouge
/// \param g : Canal vert
/// \param b : Canal bleu
/// \param a : Canal alpha
///
////////////////////////////////////////////////////////////
void CColor::Set(unsigned char r, unsigned char g, unsigned char b,
		unsigned char a)
{
	m_Color = (a << 24) | (r << 16) | (g << 8) | (b << 0);
}

/////////////////////////////////////////////////////////////
/// Met a jour la couleur a partir de 4 composantes
/// Utilise en interne - sert a clamper les composantes
///
/// \param r : Canal rouge
/// \param g : Canal vert
/// \param b : Canal bleu
/// \param a : Canal alpha
///
////////////////////////////////////////////////////////////
void CColor::SetInt(int r, int g, int b, int a)
{
	unsigned char R = (r >= 0) ? (r <= 255 ? r : 255) : 0;
	unsigned char G = (g >= 0) ? (g <= 255 ? g : 255) : 0;
	unsigned char B = (b >= 0) ? (b <= 255 ? b : 255) : 0;
	unsigned char A = (a >= 0) ? (a <= 255 ? a : 255) : 0;

	Set(R, G, B, A);
}

/////////////////////////////////////////////////////////////
/// Surcharge de l'operateur == pour comparer deux couleurs
///
/// \param c : Couleur a comparer
///
/// \return True si les deux couleurs sont identiques
///
////////////////////////////////////////////////////////////
bool CColor::operator ==(const CColor& c) const
{
	return m_Color == c.m_Color;
}

/////////////////////////////////////////////////////////////
/// Surcharge de l'operateur != pour comparer deux couleurs
///
/// \param c : Couleur a comparer
///
/// \return True si les deux couleurs sont differentes
///
////////////////////////////////////////////////////////////
bool CColor::operator!=(const CColor& c) const
{
	return !(*this == c);
}

/////////////////////////////////////////////////////////////
/// Surcharge de l'operateur +=
///
/// \param c : Couleur a ajouter
///
/// \return Nouvelle couleur
///
////////////////////////////////////////////////////////////
const CColor& CColor::operator +=(const CColor& c)
{
	int R = GetRed() + c.GetRed();
	int G = GetGreen() + c.GetGreen();
	int B = GetBlue() + c.GetBlue();
	int A = GetAlpha() + c.GetAlpha();

	SetInt(R, G, B, A);

	return *this;
}

/////////////////////////////////////////////////////////////
/// Surcharge de l'operateur -=
///
/// \param c : Couleur a soustraire
///
/// \return Nouvelle couleur
///
////////////////////////////////////////////////////////////
const CColor& CColor::operator -=(const CColor& c)
{
	int R = GetRed() - c.GetRed();
	int G = GetGreen() - c.GetGreen();
	int B = GetBlue() - c.GetBlue();
	int A = GetAlpha() - c.GetAlpha();

	SetInt(R, G, B, A);

	return *this;
}

/////////////////////////////////////////////////////////////
/// Surcharge de l'operateur +
///
/// \param c : Couleur a ajouter
///
/// \return Nouvelle couleur
///
////////////////////////////////////////////////////////////
CColor CColor::operator +(const CColor& c) const
{
	int R = GetRed() + c.GetRed();
	int G = GetGreen() + c.GetGreen();
	int B = GetBlue() + c.GetBlue();
	int A = GetAlpha() + c.GetAlpha();

	CColor Ret;
	Ret.SetInt(R, G, B, A);

	return Ret;
}

/////////////////////////////////////////////////////////////
/// Surcharge de l'operateur -
///
/// \param c : Couleur a soustraire
///
/// \return Nouvelle couleur
///
////////////////////////////////////////////////////////////
CColor CColor::operator -(const CColor& c) const
{
	int R = GetRed() - c.GetRed();
	int G = GetGreen() - c.GetGreen();
	int B = GetBlue() - c.GetBlue();
	int A = GetAlpha() - c.GetAlpha();

	CColor Ret;
	Ret.SetInt(R, G, B, A);

	return Ret;
}

/////////////////////////////////////////////////////////////
/// Surcharge de l'operateur *
///
/// \param v : Multiplicateur
///
/// \return Nouvelle couleur
///
////////////////////////////////////////////////////////////
CColor CColor::operator *(float v) const
{
	int R = static_cast<int>(GetRed() * v);int G = static_cast<int>(GetGreen() * v);
	int B = static_cast<int>(GetBlue()  * v);
	int A = static_cast<int>(GetAlpha() * v);

    CColor Ret;
    Ret.SetInt(R, G, B, A);

	return Ret;
}

/////////////////////////////////////////////////////////////
/// Surcharge de l'operateur *=
///
/// \param v : Multiplicateur
///
/// \return Nouvelle couleur
///
////////////////////////////////////////////////////////////
const CColor& CColor::operator *=(float v)
{
	int R = static_cast<int>(GetRed() * v);int G = static_cast<int>(GetGreen() * v);
	int B = static_cast<int>(GetBlue() * v);
	int A = static_cast<int>(GetAlpha() * v);

	SetInt(R, G, B, A);

	return *this;
}

/////////////////////////////////////////////////////////////
/// Surcharge de l'operateur /
///
/// \param v : Diviseur
///
/// \return Nouvelle couleur
///
////////////////////////////////////////////////////////////
CColor CColor::operator /(float v) const
{
	return *this * (1.0f / v);
}

/////////////////////////////////////////////////////////////
/// Surcharge de l'operateur /=
///
/// \param v : Diviseur
///
/// \return Nouvelle couleur
///
////////////////////////////////////////////////////////////
const CColor& CColor::operator /=(float v)
{
	return *this *= (1.0f / v);
}

/////////////////////////////////////////////////////////////
/// Ajoute deux couleurs
///
/// \param c : Couleur a ajouter
///
/// \return Somme des deux couleurs
///
////////////////////////////////////////////////////////////
CColor CColor::Add(const CColor& c) const
{
	return *this + c;
}

/////////////////////////////////////////////////////////////
/// Module deux couleurs
///
/// \param c : Couleur a moduler
///
/// \return Modulation des deux couleurs
///
////////////////////////////////////////////////////////////
CColor CColor::Modulate(const CColor& c) const
{
	unsigned char R = static_cast<unsigned char>(GetRed() * c.GetRed() / 255);unsigned char G = static_cast<unsigned char>(GetGreen() * c.GetGreen() / 255);
	unsigned char B = static_cast<unsigned char>(GetBlue() * c.GetBlue() / 255);
	unsigned char A = static_cast<unsigned char>(GetAlpha() * c.GetAlpha() / 255);

	return CColor(R, G, B, A);
}

/////////////////////////////////////////////////////////////
/// Recupere le canal alpha
///
/// \return Valeur du canal alpha
///
////////////////////////////////////////////////////////////
unsigned char CColor::GetAlpha() const
{
	return static_cast<unsigned char>((m_Color & 0xFF000000) >> 24);
}

/////////////////////////////////////////////////////////////
/// Recupere le canal rouge
///
/// \return Valeur du canal rouge
///
////////////////////////////////////////////////////////////
unsigned char CColor::GetRed() const
{
	return static_cast<unsigned char>((m_Color & 0x00FF0000) >> 16);
}

/////////////////////////////////////////////////////////////
/// Recupere le canal vert
///
/// \return Valeur du canal vert
///
////////////////////////////////////////////////////////////
unsigned char CColor::GetGreen() const
{
	return static_cast<unsigned char>((m_Color & 0x0000FF00) >> 8);
}

/////////////////////////////////////////////////////////////
/// Recupere le canal bleu
///
/// \return Valeur du canal bleu
///
////////////////////////////////////////////////////////////
unsigned char CColor::GetBlue() const
{
	return static_cast<unsigned char>((m_Color & 0x000000FF) >> 0);
}

/////////////////////////////////////////////////////////////
/// Convertit en 4 floats RGBA
///
/// \param Dest : Tableau de destination
///
////////////////////////////////////////////////////////////
void CColor::ToFloat(float Dest[]) const
{
	Dest[0] = GetRed() / 255.0f;
	Dest[1] = GetGreen() / 255.0f;
	Dest[2] = GetBlue() / 255.0f;
	Dest[3] = GetAlpha() / 255.0f;
}

/////////////////////////////////////////////////////////////
/// Renvoie la couleur en nuance de gris
///
/// \return Octet representant le niveau de gris associe
///
////////////////////////////////////////////////////////////
unsigned char CColor::ToGrey() const
{
	return static_cast<unsigned char>(GetRed() * 0.30 + GetGreen() * 0.59
			+ GetBlue() * 0.11);
}

/////////////////////////////////////////////////////////////
/// Renvoie la couleur dans le format ARGB
///
/// \return Couleur ARGB
///
////////////////////////////////////////////////////////////
unsigned long CColor::ToARGB() const
{
	return (GetAlpha() << 24) | (GetRed() << 16) | (GetGreen() << 8)
			| (GetBlue() << 0);
}

/////////////////////////////////////////////////////////////
/// Renvoie la couleur dans le format ABGR
///
/// \return Couleur ABGR
///
////////////////////////////////////////////////////////////
unsigned long CColor::ToABGR() const
{
	return (GetAlpha() << 24) | (GetBlue() << 16) | (GetGreen() << 8)
			| (GetRed() << 0);
}

/////////////////////////////////////////////////////////////
/// Renvoie la couleur dans le format RGBA
///
/// \return Couleur RGBA
///
////////////////////////////////////////////////////////////
unsigned long CColor::ToRGBA() const
{
	return (GetRed() << 24) | (GetGreen() << 16) | (GetBlue() << 8)
			| (GetAlpha() << 0);
}

/////////////////////////////////////////////////////////////
/// Surcharge de l'operateur >> entre un flux et une couleur
///
/// \param Stream : Flux d'entree
/// \param Color :  Couleur
///
/// \return Reference sur le flux d'entree
///
////////////////////////////////////////////////////////////
std::istream& operator >>(std::istream& Stream, CColor& Color)
{
	int R, G, B, A;
	Stream >> R >> G >> B >> A;
	Color.SetInt(R, G, B, A);

	return Stream;
}

/////////////////////////////////////////////////////////////
/// Surcharge de l'operateur << entre un flux et une couleur
///
/// \param Stream : Flux de sortie
/// \param Color :  Couleur
///
/// \return Reference sur le flux de sortie
///
////////////////////////////////////////////////////////////
std::ostream& operator <<(std::ostream& Stream, const CColor& Color)
{
	return Stream << static_cast<int>(Color.GetRed()) << " "
			<< static_cast<int>(Color.GetGreen()) << " "
			<< static_cast<int>(Color.GetBlue()) << " "
			<< static_cast<int>(Color.GetAlpha());
}
}
