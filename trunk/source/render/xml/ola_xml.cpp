#include <map>

#include "ola_xml.h"

#include "tinyxml.h"

class OlaXmlNodeImpl;
class OlaXmlDocumentImpl : public OlaXmlDocument
{
	typedef std::map<TiXmlNode*,OlaXmlNode*> NodeWrapper;
public:
	OlaXmlDocumentImpl()
	{

	}

	virtual ~OlaXmlDocumentImpl()
	{
		NodeWrapper::iterator i = mWrapper.begin();
		while(i != mWrapper.end())
		{
			delete i->second;
			i++;
		}
	}

	OlaXmlNode* wrap_node(TiXmlNode* ti_node);

	virtual bool load(const char*)
	{
		return false;
	}

	virtual bool parse(const char* data)
	{
		xml.Parse(data);
		bool b = xml.Error();
		return !b;
	}

	virtual OlaXmlDocument* ownerDoc() 
	{
		return 0;
	};

	virtual OlaXmlNode* fisrtChild()
	{
		TiXmlNode* root = xml.FirstChild();
		return wrap_node(root);
	}

	virtual OlaXmlNode* lastChild()
	{
		TiXmlNode* root = xml.FirstChild();
		return wrap_node(root);
	}
	virtual OlaXmlNode* selectFirst(const char*)
	{
		TiXmlNode* root = xml.FirstChild();
		return wrap_node(root);
	}
	virtual OlaXmlNode* nextSibling(const char*)
	{
		return 0;
	}
	virtual OlaXmlNode* nextSibling()
	{
		return 0;
	}
	virtual OlaXmlNode* parent()
	{
		return 0;
	}

	virtual int childNum()
	{
		return 0;
	}
	virtual const char* innerText()
	{
		return 0;
	}
	virtual const char* name()
	{
		return 0;
	}
	virtual const char* attribute(const char*)
	{
		return 0;
	}

	NodeWrapper mWrapper;
	TiXmlDocument xml;
};

class OlaXmlNodeImpl : public OlaXmlNode
{
public:
	OlaXmlNodeImpl(OlaXmlDocumentImpl* doc,TiXmlNode* ti_node):
	mDoc(doc),mTiXmlNode(ti_node)
	{

	}
	virtual ~OlaXmlNodeImpl()
	{

	}
	virtual OlaXmlDocument* ownerDoc() 
	{
		return mDoc;
	};
	virtual OlaXmlNode* fisrtChild()
	{
		TiXmlNode* ti = mTiXmlNode->FirstChild();
		return mDoc->wrap_node(ti);
	}

	virtual OlaXmlNode* lastChild()
	{
		TiXmlNode* ti = mTiXmlNode->LastChild();
		return mDoc->wrap_node(ti);
	}
	virtual OlaXmlNode* selectFirst(const char* name)
	{
		TiXmlNode* ti = mTiXmlNode->FirstChild(name);
		return mDoc->wrap_node(ti);
	}
	virtual OlaXmlNode* nextSibling(const char* name)
	{
		TiXmlNode* ti = mTiXmlNode->NextSibling(name);
		return mDoc->wrap_node(ti);
	}
	virtual OlaXmlNode* nextSibling()
	{
		TiXmlNode* ti = mTiXmlNode->NextSibling(mTiXmlNode->Value());
		return mDoc->wrap_node(ti);
	}
	virtual OlaXmlNode* parent()
	{
		TiXmlNode* ti = mTiXmlNode->Parent();
		return mDoc->wrap_node(ti);
	}

	virtual int childNum()
	{
		return 0;
	}
	virtual const char* innerText()
	{
		return mTiXmlNode->ToElement()->GetText();
	}
	virtual const char* name()
	{
		return mTiXmlNode->Value();
	}
	virtual const char* attribute(const char* name)
	{
		return mTiXmlNode->ToElement()->Attribute(name);
	}

	OlaXmlDocumentImpl* mDoc;
	TiXmlNode* mTiXmlNode;
};

OlaXmlNode* OlaXmlDocumentImpl::wrap_node(TiXmlNode* ti_node)
{
	OlaXmlNode* res = 0;
	if(ti_node == 0)
		return res;

	NodeWrapper::iterator i = mWrapper.find(ti_node);
	if(i == mWrapper.end())
	{
		res = new OlaXmlNodeImpl(this,ti_node);
		mWrapper[ti_node] = res;
	}
	else
	{
		res = i->second;
	}
	return res;
}

OlaXmlDocument* OlaXmlDocument::CreateXmlDocument()
{
	return new OlaXmlDocumentImpl();
}