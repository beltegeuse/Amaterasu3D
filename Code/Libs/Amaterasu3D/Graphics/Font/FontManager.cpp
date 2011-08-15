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
// --- Std includes
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
// --- ama3D includes
#include <Debug/Exceptions.h>
#include <Graphics/RenderableObject.h>
#include <Graphics/Shaders/Shader.h>
#include <System/MediaManager.h>
// --- Other includes
#ifdef MSVC
#include <windows.h>
#endif
#include <GL/gl.h>

namespace ama3D
{

//////////////////////////////////////////
// CFontCharacter
//////////////////////////////////////////
CFont::CFontCharacter::CFontCharacter(TiXmlElement* element, const ama3D::Math::TVector2F & texSize)
{
	// Get element definition
	TinyXMLGetAttributeValue<int>(element,"id",&m_ID);
	TinyXMLGetAttributeValue<int>(element,"width",&m_Size.x);
	TinyXMLGetAttributeValue<int>(element,"height",&m_Size.y);
	TinyXMLGetAttributeValue<int>(element,"x",&m_Position.x);
	TinyXMLGetAttributeValue<int>(element,"y",&m_Position.y);
	TinyXMLGetAttributeValue<double>(element,"xoffset",&m_Offset.x);
	TinyXMLGetAttributeValue<double>(element,"yoffset",&m_Offset.y);
	TinyXMLGetAttributeValue<double>(element,"xadvance",&m_xadvance);
	TinyXMLGetAttributeValue<double>(element,"xmin",&m_MinPos.x);
	TinyXMLGetAttributeValue<double>(element,"ymin",&m_MinPos.y);
	TinyXMLGetAttributeValue<double>(element,"xmax",&m_MaxPos.x);
	TinyXMLGetAttributeValue<double>(element,"ymax",&m_MaxPos.y);

	// Fill buffer
	float* vertexBuffer = new float[8];
	// TODO: See how to do
	vertexBuffer[0] = m_MinPos.x*1;
	vertexBuffer[1] = -m_MinPos.y*1;
	vertexBuffer[2] = m_MinPos.x*1;
	vertexBuffer[3] = -m_MaxPos.y*1;
	vertexBuffer[4] = m_MaxPos.x*1;
	vertexBuffer[5] = -m_MaxPos.y*1;
	vertexBuffer[6] = m_MaxPos.x*1;
	vertexBuffer[7] = -m_MinPos.y*1;
	ama3D::RenderableObject::RenderableBuffer buffer;
	buffer.buffer = vertexBuffer;
	buffer.dimension = 2;
	buffer.size = 8;
	buffer.owner = true;
	m_Buffer.AddBuffer(buffer, ama3D::VERTEX_ATTRIBUT);
	// Create Texcoord buffer
	float* uvBuffer = new float[8];
	uvBuffer[0] = (m_Position.x/texSize.x);
	uvBuffer[1] = 1 - ((m_Position.y+m_Size.y)/texSize.y);
	uvBuffer[2] = (m_Position.x/texSize.x);
	uvBuffer[3] = 1 - ((m_Position.y)/texSize.y);
	uvBuffer[4] = ((m_Position.x+m_Size.x)/texSize.x);
	uvBuffer[5] = 1 - ((m_Position.y)/texSize.y);
	uvBuffer[6] = ((m_Position.x+m_Size.x)/texSize.x);
	uvBuffer[7] = 1 - ((m_Position.y+m_Size.y)/texSize.y);
	buffer.buffer = uvBuffer;
	m_Buffer.AddBuffer(buffer, ama3D::TEXCOORD_ATTRIBUT);
	// Create Index buffer
	unsigned int* indiceBuffer = new unsigned int[6];
	indiceBuffer[0] = 0;
	indiceBuffer[1] = 1;
	indiceBuffer[2] = 2;
	indiceBuffer[3] = 0;
	indiceBuffer[4] = 2;
	indiceBuffer[5] = 3;
	m_Buffer.SetIndiceBuffer(indiceBuffer, 6);
	m_Buffer.CompileBuffers();
}

void CFont::CFontCharacter::Render()
{
	m_Buffer.Draw();
}

//////////////////////////////////////////
//
//////////////////////////////////////////
CFont::CFont(const std::string& filename):
	m_isInitialize(false)
{
	ama3D::CFile file = ama3D::CMediaManager::Instance().FindMedia(filename);
	m_FontFile = file.Fullname();
	ama3D::Logger::Log() << "[INFO] Load Font file : " << m_FontFile << "\n";
	TiXmlDocument doc(m_FontFile);
	if (!doc.LoadFile())
	{
		ama3D::Logger::Log() << "[ERROR] TinyXML error : " << doc.ErrorDesc() << "\n";
		throw ama3D::CLoadingFailed(m_FontFile, "unable to load xml with TinyXML");
	}

	TiXmlHandle hdl(&doc);
	TiXmlElement *fontNode = hdl.FirstChild("Font").ToElement();
	if(fontNode == 0)
	{
		throw CLoadingFailed(m_FontFile, "unable to find the Font node entry");
	}
	TinyXMLGetAttributeValue<std::string>(fontNode,"name",&m_FontName);
}

CFont::~CFont()
{
	if(m_isInitialize)
	{
		for(std::map<int, CFontCharacter*>::iterator it = m_Characteres.begin(); it != m_Characteres.end(); ++it)
		{
			delete it->second;
		}
	}
}

void CFont::Render(const CGraphicString& gstring)
{
	if(!m_isInitialize)
	{
		LoadFile();
		m_isInitialize = true;
	}

	m_FontsTex->activateMultiTex(ama3D::CUSTOM_TEXTURE+0);
	std::map<int, CFontCharacter*>::const_iterator element;
	CFontCharacter* currentElement;
	ama3D::Math::CMatrix4 transMat;
	float xOff = 0.0;
	for (std::string::const_iterator it = gstring.Text.begin(); it != gstring.Text.end(); ++it)
	{
		element = m_Characteres.find((int)*it);
		if(element == m_Characteres.end())
			currentElement = m_Characteres[32]; // Not found
		else
			currentElement = element->second;
		transMat.SetTranslation(gstring.Position.x+xOff,gstring.Position.y,0);
		ama3D::CMatrixManager::Instance().PushMatrix(transMat);
		currentElement->Render();
		ama3D::CMatrixManager::Instance().PopMatrix();
		xOff += currentElement->GetXStep()*1.10;
	}
	m_FontsTex->desactivateMultiTex(ama3D::CUSTOM_TEXTURE+0);
}


void CFont::LoadFile()
{
	ama3D::Logger::Log() << "[INFO] Load Font file : " << m_FontFile << "\n";
	TiXmlDocument doc(m_FontFile);
	if (!doc.LoadFile())
	{
		ama3D::Logger::Log() << "[ERROR] TinyXML error : " << doc.ErrorDesc() << "\n";
		throw ama3D::CLoadingFailed(m_FontFile, "unable to load xml with TinyXML");
	}

	TiXmlHandle hdl(&doc);
	TiXmlElement *fontNode = hdl.FirstChild("Font").ToElement();
	if(fontNode == 0)
	{
		throw CLoadingFailed(m_FontFile, "unable to find the Font node entry");
	}

	std::string fileNameTex;
	int texWidth, texHeight;
	TinyXMLGetAttributeValue<std::string>(fontNode,"filename",&fileNameTex);
	TinyXMLGetAttributeValue<int>(fontNode,"width",&texWidth);
	TinyXMLGetAttributeValue<int>(fontNode,"height",&texHeight);

	ama3D::Logger::Log() << "      * Load Texture : " << fileNameTex<< "\n";
	m_FontsTex = ama3D::Texture::LoadFromFile(fileNameTex);

	ama3D::Logger::Log() << "      * Load Fonts ... ";
	TiXmlElement *facesNode = fontNode->FirstChildElement("Faces");
	TiXmlElement *faceNode = facesNode->FirstChildElement("Face");
	int i = 0;
	while(faceNode)
	{
		CFontCharacter * face = new CFontCharacter(faceNode, ama3D::Math::TVector2F(texWidth, texHeight));
		m_Characteres[face->GetID()] = face;
		faceNode = faceNode->NextSiblingElement();
		i++;
	}
	ama3D::Logger::Log() << i << " Font loaded ! \n";
}

//////////////////////////////////////////
// CFontManager
//////////////////////////////////////////
SINGLETON_IMPL(CFontManager)
CFontManager::CFontManager() :
	m_Initialize(false)
{
}

CFontManager::~CFontManager()
{
	// TODO: Verify no memory leak on texture font
	UnloadFonts();
}

void CFontManager::LoadFont(const std::string& filename)
{
	CFont* font = new CFont(filename);
	TPolices::iterator it = m_polices.find(font->GetFontName());

	// La police est absente
	if (it == m_polices.end())
	{
		Logger::Log() << "[INFO] Add new font : " << filename << " -> " <<  font->GetFontName() << "\n";
		m_polices[font->GetFontName()] = font;
	}
}

void CFontManager::UnloadFonts()
{
	TPolices::iterator it = m_polices.begin();

	while (it != m_polices.end())
	{
		delete it->second;
		++it;
	}
}


void CFontManager::RenderText(const CGraphicString& gstring)
{
	if(!m_Initialize)
	{
		// Shader loading ...
		m_FontShader = ama3D::CShaderManager::Instance().LoadShader("2DDrawFont.shader");
		m_Initialize = true;
	}

	TPolices::iterator it = m_polices.find(gstring.FontAlias);
	if(it == m_polices.end())
	{
		//Logger::Log() << "[Warning] Can't Draw Graphic String [Reason : Font \"" << gstring.FontAlias << "\" isn't found ! \n";
		return;
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	m_FontShader->Begin();
	m_FontShader->SetUniform1i("FontEffectType", 4);
	m_FontShader->SetUniformVector("FontColor", Math::TVector4F(gstring.Color.GetRed(),gstring.Color.GetGreen(),gstring.Color.GetBlue(), gstring.Color.GetAlpha()));
	m_FontShader->SetUniformVector("FontEffectColor", ama3D::Math::TVector4F(1.0,1.0,1.0,1.0));

	it->second->Render(gstring);

	m_FontShader->End();
	glDisable(GL_BLEND);
}

}
