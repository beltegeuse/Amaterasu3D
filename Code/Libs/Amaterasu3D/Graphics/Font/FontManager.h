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

#ifndef FONTMANAGER_H
#define FONTMANAGER_H

//==========================================================
// En-tetes
//==========================================================
#include <ft2build.h>
#include <freetype2/freetype/ftbitmap.h>

#include <Singleton.h>
#include <Math/Vector2.h>

#include <map>

struct rat_texture_font
{
	float pt, *wids, *hoss;
	int *qvws, *qvhs; // widths heights ?
	float *qtws, *qths;
	unsigned int *textures;
	float ascend;
};

struct rat_glyph_font
{
	float pt;
	FT_Face face;
};

struct PoliceData
{
	rat_glyph_font* Glyph;
	rat_texture_font* Texture;
};

class GraphicsEngine;
////////////////////////////////////////////////////////////
/// Gestionnaire de fonts / texte
////////////////////////////////////////////////////////////
class CFontManager: public CSingleton<CFontManager>
{
MAKE_SINGLETON(CFontManager)
public:

	//----------------------------------------------------------
	// Methodes pour charger et decharger des fonts
	//----------------------------------------------------------
	void LoadFont(const std::string& FontName, const std::string& alias);
	void UnloadFonts();

	//----------------------------------------------------------
	// Pour recuperer ou supprimer des textures
	//----------------------------------------------------------
	rat_texture_font * GetTexture(const std::string& alias);
	void DeleteTexture(rat_texture_font * a);

	// ===== Des methodes pour modifier les couleurs ... etc.
	void rat_set_text_color(float *rgba);
	void rat_get_text_color(float *rgba);
	float rat_texture_font_height(rat_texture_font *font);
	float rat_texture_font_text_length(rat_texture_font *font, char *text);
	float rat_texture_font_glyph_length(rat_texture_font *font, char ch);

	// ===== Render
	void rat_texture_font_render_text(rat_texture_font *font, float x, float y,
			char *text, int size);
	void rat_texture_font_render_text_notform(rat_texture_font *font,
			char *text);

	//----------------------------------------------------------
	// Initialise les donnees
	//----------------------------------------------------------
	void Initialize();

private:

	//----------------------------------------------------------
	// Constructeur par defaut
	//----------------------------------------------------------
	CFontManager();

	//----------------------------------------------------------
	// Destructeur
	//----------------------------------------------------------
	~CFontManager();

	// ===== Les methodes pour les initialisations et destructions
	int rat_start_font_system();
	void rat_stop_font_system();

	// ==== Pour charger ou detruire des polices
	rat_glyph_font *rat_glyph_font_load(const char *filename, int pt);
	void rat_glyph_font_destroy(rat_glyph_font *font);

	// ==== Quelques methodes de chargement pour les textures
	rat_texture_font *rat_texture_font_from_glyph_font(rat_glyph_font *font);
	void rat_texture_font_destroy(rat_texture_font *font);

	//----------------------------------------------------------
	// Constantes
	//----------------------------------------------------------
	static const unsigned int NbCharMax = 512;

	//----------------------------------------------------------
	// Donnees membres
	//----------------------------------------------------------
	// === Instance de Freetype
	FT_Library m_freetype_lib;
	// === Pour sauvegarder les polices
	typedef std::map<std::string, PoliceData> TPolices;
	TPolices m_polices;

	// Attributs Charly :
	Math::TVector2F m_coordHG;
	float m_profondeur;
};

//} // namespace Yes

#endif // FONTMANAGER_H
