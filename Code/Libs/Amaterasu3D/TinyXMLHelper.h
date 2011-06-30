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

#pragma once

// OpenGL includes
#include <GL/glew.h>

#include <string>
#include <tinyxml.h>
#include <Debug/Exceptions.h>

// Declaration of TinyXML error
struct CTinyXMLException : public CException
{
	CTinyXMLException(const std::string& Message) :
		CException("[TinyXML] "+Message)
	{}
};

struct CTinyXMLAttributException : public CTinyXMLException
{
	CTinyXMLAttributException(const std::string& attributName, int tinyXMLerror) :
		CTinyXMLException("")
	{
		if(tinyXMLerror == TIXML_WRONG_TYPE)
			m_Message += " can't get the value of the attribute : " + attributName;
		else if(tinyXMLerror == TIXML_NO_ATTRIBUTE)
			m_Message += " can't found the attribute : " + attributName;
		else
			m_Message += " unknow error on the attribute : " + attributName;
	}
};

// Function helper to check TinyXML node
template< typename T> inline void TinyXMLGetAttributeValue(TiXmlElement* element, const std::string nameAttribute, T* value)
{
	int error = element->QueryValueAttribute<T>(nameAttribute, value);
	if(error != TIXML_SUCCESS)
		throw CTinyXMLAttributException(nameAttribute,error);
}

inline GLenum OpenGLEnumFromString(const std::string& name)
{
	if(name == "Line")
		return GL_LINES;
	else if(name == "LineStrip")
		return GL_LINE_STRIP;
	else if(name == "Point")
		return GL_POINTS;
	else if(name == "Triangle")
		return GL_TRIANGLES;
	else if(name == "TriangleStrip")
		return GL_TRIANGLE_STRIP;
	else if(name == "LineAdjacency")
		return GL_LINES_ADJACENCY_EXT;
	else if(name == "TriangleAdjacency")
		return GL_TRIANGLES_ADJACENCY_EXT;
	else
		throw CException("Unknow conversion in GLenum : "+name);
}
