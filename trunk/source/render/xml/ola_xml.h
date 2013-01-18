#ifndef _OLA_INCLUDE_XML_H__
#define _OLA_INCLUDE_XML_H__

class OlaXmlDocument;
class OlaXmlNode
{
public:
	virtual ~OlaXmlNode(){};
	virtual OlaXmlDocument* ownerDoc() = 0;

	virtual OlaXmlNode* fisrtChild() = 0;
	virtual OlaXmlNode* lastChild() = 0;
	virtual OlaXmlNode* selectFirst(const char*) = 0;
	virtual OlaXmlNode* nextSibling(const char*) = 0;
	virtual OlaXmlNode* nextSibling() = 0;
	virtual OlaXmlNode* parent() = 0;	

	virtual int childNum() = 0;
	virtual const char* innerText() = 0;
	virtual const char* name() = 0;
	virtual const char* attribute(const char*) = 0;
};

class OlaXmlDocument : public OlaXmlNode
{
public:
	static OlaXmlDocument* CreateXmlDocument();
public:
	OlaXmlDocument(){};
	virtual ~OlaXmlDocument(){};

	virtual bool load(const char*) = 0;	
	virtual bool parse(const char*) = 0;
};

#endif