/*
 * IXMLSerialisable.h
 *
 *  Created on: Jun 9, 2011
 *      Author: adrien
 */

#ifndef IXMLSERIALISABLE_H_
#define IXMLSERIALISABLE_H_

#include <tinyxml.h>
#include <TinyXMLHelper.h>

class IXMLSerialisable
{
public:
	virtual ~IXMLSerialisable() {};

	virtual void WriteXML(TiXmlElement * element) const;
	virtual void void ReadXML(TiXmlElement * element);
};


#endif /* IXMLSERIALISABLE_H_ */
