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
