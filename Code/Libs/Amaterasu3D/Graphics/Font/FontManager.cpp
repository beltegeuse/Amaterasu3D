/*
	This file is part of Floculate.
	Copyright (C) 2008  Bill Whitacre

	Floculate is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Floculate is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "FontManager.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <GL/gl.h>

#include <ft2build.h>
#include <freetype2/freetype/ftglyph.h>

#include <Debug/Exceptions.h>

SINGLETON_IMPL(CFontManager)

// ==== Quelques fonctions inlines
static float _textrgba[4]={1.0f,1.0f,1.0f,1.0f};

inline static unsigned int _pow2(unsigned int i)
{
	register unsigned int p2;
	for (p2=1; p2<i; p2<<=1);
	return p2;
}

static int make_glyph_texture(rat_glyph_font *gf,rat_texture_font *tf,unsigned char ch)
{
	register unsigned int i,j;
	FT_Face face=gf->face;
	unsigned int *textures=tf->textures;
	unsigned int width,height;
	float texx,texy;

	if (FT_Load_Glyph(face,FT_Get_Char_Index(face,ch),FT_LOAD_DEFAULT))
		return 0;

	FT_Glyph glyph;
	if (FT_Get_Glyph(face->glyph,&glyph))
		return 0;

	FT_Glyph_To_Bitmap(&glyph,ft_render_mode_normal,0,1);
	FT_BitmapGlyph bitmap_glyph=(FT_BitmapGlyph)glyph;

	FT_Bitmap bitmap=bitmap_glyph->bitmap;

	width=_pow2(bitmap.width);
	height=_pow2(bitmap.rows);

	GLubyte* expanded_data=(GLubyte *)malloc(sizeof(GLubyte)*2*width*height);

	for (j=0; j<height;j++)
	{
		for (i=0; i<width; i++)
		{
			expanded_data[2*(i+j*width)]=
					expanded_data[2*(i+j*width)+1]=
							(i>=bitmap.width||j>=bitmap.rows)?
									0:bitmap.buffer[i+bitmap.width*j];
		}
	}

	glBindTexture(GL_TEXTURE_2D,textures[ch]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D,0,GL_ALPHA16,width,height,
			0,GL_LUMINANCE_ALPHA,GL_UNSIGNED_BYTE,expanded_data);

	free((void *)expanded_data);

	tf->wids[ch]=(float)(face->glyph->advance.x>>6);
	tf->hoss[ch]=(float)((face->glyph->metrics.horiBearingY-face->glyph->metrics.height)>>6);

	tf->qvws[ch]=bitmap.width;
	tf->qvhs[ch]=bitmap.rows;

	tf->qtws[ch]=(float)bitmap.width/(float)width;
	tf->qths[ch]=(float)bitmap.rows/(float)height;

	return 1;
}

// CFontManager

CFontManager::CFontManager()
{
	rat_start_font_system();

	// Code Charly
	m_coordHG = Math::TVector2F(0.0,0.0);
	m_hauteur = 25.0;
	m_profondeur = -1.0;
}

CFontManager::~CFontManager()
{
	UnloadFonts();
	rat_stop_font_system();
}

void CFontManager::LoadFont(const std::string& FontName, const std::string& alias)
{
	TPolices::iterator it = m_polices.find(alias);

	// La police est absente
	if(it == m_polices.end())
	{
		m_polices[alias] = rat_glyph_font_load(FontName.c_str(), 30);
	}
}

void CFontManager::UnloadFonts()
{
	TPolices::iterator it = m_polices.begin();

	while(it != m_polices.end())
	{
		rat_glyph_font_destroy(it->second);
		++it;
	}
}

rat_texture_font * CFontManager::GetTexture(const std::string& alias)
{
	TPolices::iterator it = m_polices.find(alias);

	if(it == m_polices.end())
	{
		std::cout << "Alias : " << alias << std::endl;
		throw CException("[CFontManager] Get texture ! impossible de trouver l'alias.");
	}

	return rat_texture_font_from_glyph_font(it->second);
}

void CFontManager::DeleteTexture(rat_texture_font * a)
{
	rat_texture_font_destroy(a);
}

int CFontManager::rat_start_font_system()
{
	return !(FT_Init_FreeType(&m_freetype_lib));
}

void CFontManager::rat_stop_font_system()
{
	FT_Done_FreeType(m_freetype_lib);
}

rat_glyph_font *CFontManager::rat_glyph_font_load(const char *filename,int pt)
{
	rat_glyph_font *font=(rat_glyph_font *)malloc(sizeof(rat_glyph_font));

	printf("Loading font from file \"%s\" at ptsize %i...",filename,pt);

	// load the font from the file
	if (FT_New_Face(m_freetype_lib,filename,0,&(font->face)))
	{
		printf("failed load!\n");
		free((void *)font);
		return NULL;
	}

	// freetype measures fonts in 64ths of pixels, which
	// I will never understand.  6 left bit shift multiplies
	// the pt size by 64.
	FT_Set_Char_Size(font->face,pt<<6,pt<<6,96,96);
	font->pt=pt;

	printf("done.\n");
	return font;
}

void CFontManager::rat_glyph_font_destroy(rat_glyph_font *font)
{
	printf("Destroying glyph font...");
	FT_Done_Face(font->face);
	free((void *)font);
	printf("done.\n");
}

rat_texture_font *CFontManager::rat_texture_font_from_glyph_font(rat_glyph_font *font)
{
	register unsigned char i;
	rat_texture_font *tf=(rat_texture_font *)malloc(sizeof(rat_texture_font));

	tf->pt=font->pt;

	// prepare the OpenGL textures / display lists
	tf->wids=(float *)malloc(sizeof(float)*255);
	tf->hoss=(float *)malloc(sizeof(float)*255);
	tf->qvws=(int *)malloc(sizeof(int)*255);
	tf->qvhs=(int *)malloc(sizeof(int)*255);
	tf->qtws=(float *)malloc(sizeof(float)*255);
	tf->qths=(float *)malloc(sizeof(float)*255);
	tf->textures=(unsigned int *)malloc(sizeof(unsigned int)*255);
	glGenTextures(255,tf->textures);

	for (i=0;i<255;i++)
	{
		if (!make_glyph_texture(font,tf,i))
		{
			glDeleteTextures(255,tf->textures);
			free((void *)tf->textures);
			free((void *)tf->wids);
			free((void *)tf->hoss);
			free((void *)tf->qvws);
			free((void *)tf->qvhs);
			free((void *)tf->qtws);
			free((void *)tf->qths);
			free((void *)tf);
			return NULL;
		}
	}

	return tf;
}

void CFontManager::rat_texture_font_destroy(rat_texture_font *font)
{
	glDeleteTextures(255,font->textures);
	free((void *)font->wids);
	free((void *)font->textures);
	free((void *)font->qvws);
	free((void *)font->qvhs);
	free((void *)font->qtws);
	free((void *)font->qths);
	free((void *)font);
}

void CFontManager::rat_set_text_color(float *rgba)
{
	memcpy(_textrgba,rgba,4*sizeof(float));
}

void CFontManager::rat_get_text_color(float *rgba)
{
	memcpy(rgba,_textrgba,4*sizeof(float));
}

float CFontManager::rat_texture_font_height(rat_texture_font *font)
{
	return font->pt;
}

float CFontManager::rat_texture_font_text_length(rat_texture_font *font,char *text)
{
	register float len=0;
	char *ch=text;
	for (; *ch; ch++) len+=font->wids[*ch];
	return len;
}

float CFontManager::rat_texture_font_glyph_length(rat_texture_font *font,char ch)
{
	return font->wids[ch];
}

void CFontManager::rat_texture_font_render_text(rat_texture_font *font,float x,float y, char *text)
{
	char *ch;

	glPushAttrib(GL_LIST_BIT|GL_CURRENT_BIT|GL_ENABLE_BIT|GL_TRANSFORM_BIT);
	glMatrixMode(GL_MODELVIEW);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glColor4fv(_textrgba);

	glPushMatrix();
	glScalef(1,-1,1);
	// Code Charly
	// On calcule le facteur de reduction de notre texte :
	float ratio = m_hauteur /rat_texture_font_height(font);
	float m_largeur = ratio * rat_texture_font_text_length(font,text);
	//			float m_largeur = 1000.0;

	Math::TVector2F coordHG = Math::TVector2F(x,y);
	Math::TVector2F coordBD = Math::TVector2F(x + m_largeur,y + m_hauteur);

	// Calcul des coordonnees OpenGL correspondant aux coordonnees de la fenetre
	//FIXME: Acces aux dimensions de l'ecran
	float windowX = 800;
	float windowY = 600;

	// On centre la coordonnee HG
	coordHG.x -= windowX/2.0;
	coordHG.y -= windowY/2.0;

	// On change de repere
	coordHG.x = coordHG.x * -2.0 * m_profondeur/windowX;
	coordHG.y = coordHG.y * 2.0 * m_profondeur/windowY;

	// On centre la coordonnee BD
	coordBD.x -= windowX/2.0;
	coordBD.y -= windowY/2.0;

	// On change de repere
	coordBD.x = coordBD.x * -2.0 * m_profondeur/windowX;
	coordBD.y = coordBD.y * 2.0 * m_profondeur/windowY;


	// Donnees pour le parcours :
	float parcouru = 0.0; // Pour stocker la largeur parcourue

	for (ch=text; *ch; ch++){
		glPushMatrix();
		glLoadIdentity();
		glDepthMask(0);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,font->textures[*ch]);

		// On calcule les coordonnees la lettre courante
		Math::TVector2F HG = coordHG;
		float ratio2 = (coordBD.x - coordHG.x) / (m_largeur * ratio);
		float parcouru2 = parcouru * ratio2;
		HG.x += parcouru2;

		Math::TVector2F BD = coordBD;
		char c = *ch;
		float largeurLettre = ratio * rat_texture_font_glyph_length(font,c);
		float largeurLettre2 = ratio2 * largeurLettre;
		BD.x = HG.x + largeurLettre2;

		// On met a jour la variable parcouru
		parcouru += largeurLettre;

		float largeur = BD.x - HG.x;
		float hauteur = BD.y - HG.y;

		glTranslatef(HG.x + largeur/2, HG.y + hauteur/2, 0);
		glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f(0.0,0.0);
		glVertex3f(-largeur/2, -hauteur/2, m_profondeur);

		glTexCoord2f(0.0,font->qths[*ch]);
		glVertex3f(-largeur/2, hauteur/2, m_profondeur);

		glTexCoord2f(font->qtws[*ch],0.0);
		glVertex3f(largeur/2, -hauteur/2, m_profondeur);

		glTexCoord2f(font->qtws[*ch],font->qths[*ch]);
		glVertex3f(largeur/2, hauteur/2, m_profondeur);
		glEnd();
		glDepthMask(GL_TRUE);



		// Fin Code Charly
		glPopMatrix();
		//glTranslatef(font->wids[*ch],0,0);
	}
	glPopMatrix();
	glPopAttrib();
}

// upper left corner is always zero
void CFontManager::rat_texture_font_render_text_notform(rat_texture_font *font,char *text)
{
	char *ch;

	glPushAttrib(GL_LIST_BIT|GL_CURRENT_BIT|GL_ENABLE_BIT|GL_TRANSFORM_BIT);
	glMatrixMode(GL_MODELVIEW);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glColor4fv(_textrgba);

	glPushMatrix();
	for (ch=text; *ch; ch++)
	{
		glBindTexture(GL_TEXTURE_2D,font->textures[*ch]);
		glBegin(GL_QUADS);
		glTexCoord2f(0,0);
		glVertex2f(0,font->qvhs[*ch]);

		glTexCoord2f(0,font->qths[*ch]);
		glVertex2f(0,0);

		glTexCoord2f(font->qtws[*ch],font->qths[*ch]);
		glVertex2f(font->qvws[*ch],0);

		glTexCoord2f(font->qtws[*ch],0);
		glVertex2f(font->qvws[*ch],font->qvhs[*ch]);
		glEnd();
		glTranslatef(font->wids[*ch],0,0);
	}
	glPopMatrix();
	glPopAttrib();
}
