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
// --- TinyXML
#include <tinyxml.h>
#include <TinyXMLHelper.h>
// --- Amaterasu3D
#include <Graphics/RenderableObject.h>
#include <Singleton.h>
#include <Math/Vector2.h>
#include <Utilities/File.h>
#include <Debug/Exceptions.h>
#include <Graphics/Texture.h>
#include <Graphics/MatrixManagement.h>
#include <Graphics/Font/GraphicString.h>
// --- Std
#include <map>

namespace ama3D
{

class Shader;
typedef CSmartPtr<Shader, CResourceCOM> TShaderPtr;

class CFont
{
private:
	class CFontCharacter
	{
	private:
		/*
		 * Attributes
		 */
		// Char description
		int m_ID;
		ama3D::Math::TVector2I m_Size;
		ama3D::Math::TVector2I m_Position;
		ama3D::Math::TVector2F m_Offset;
		ama3D::Math::TVector2F m_MinPos;
		ama3D::Math::TVector2F m_MaxPos;
		double m_xadvance;
		// Char drawable
		RenderableObject m_Buffer;
	public:
		CFontCharacter(TiXmlElement* element, const ama3D::Math::TVector2F & texSize);

		// Accesseurs
		int GetID() { return m_ID; }
		const ama3D::Math::TVector2I& GetSize() { return m_Size; }
		int GetXStep() { return m_xadvance; }

		// Public methods
		void Render();
	};

public: // TODO: Need to be private
	/*
	 * Attributes
	 */
	ama3D::TTexturePtr m_FontsTex;
	std::map<int, CFontCharacter*> m_Characteres;
	bool m_isInitialize;
	std::string m_FontName;
	std::string m_FontFile;
	/*
	 * Private methods
	 */
	void LoadFile();
	TiXmlElement* LoadXMLFile();
public:
	/*
	 * Constructors & Destructors
	 */
	CFont(const std::string& filename);
	~CFont();
	/*
	 * Public methods
	 */
	void Initialize();
	void Render(const CGraphicString& gstring);

	/*
	 * Access to data
	 */
	const std::string& GetFontName() { return m_FontName; }
};

////////////////////////////////////////////////////////////
/// Gestionnaire de fonts / texte
////////////////////////////////////////////////////////////
class CFontManager: public CSingleton<CFontManager>
{
private:
	//----------------------------------------------------------
	// Constantes
	//----------------------------------------------------------
	static const unsigned int NbCharMax = 512;

	//----------------------------------------------------------
	// Donnees membres
	//----------------------------------------------------------
	// === Pour sauvegarder les polices
	typedef std::map<std::string,CFont*> TPolices;
	TPolices m_polices;
	ama3D::TShaderPtr m_FontShader;
	bool m_Initialize;

	MAKE_SINGLETON(CFontManager)
public:

	//----------------------------------------------------------
	// Methodes pour charger et decharger des fonts
	//----------------------------------------------------------
	void LoadFont(const std::string& filename);
	void UnloadFonts();

	void RenderText(const CGraphicString& gstring);

private:
	/*
	 * Constructor & Destructor
	 */
	CFontManager();
	~CFontManager();

};

}

#endif // FONTMANAGER_H
