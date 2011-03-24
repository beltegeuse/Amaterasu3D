#pragma once

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
