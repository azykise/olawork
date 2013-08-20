#include "ola_tpl.h"

#include "../xml/ola_xml.h"

#include "../ola_device.h"
#include "../ola_assetmng.h"

#include "../ola_shaderfx.h"
#include "../ola_material.h"
#include "../ola_shaderpool.h"
#include "../ola_texturepool.h"

bool ReadFXCodeFromGLSL(OlaXmlNode* GLSLNode,olastring& outVert,olastring& outFrag);

OlaTPLParser::OlaTPLParser( tResourcePools* ps ):
mPools(ps)
{

}

bool OlaTPLParser::parseTPLFromData( const char* data,int len,tTplFileInfo* outTplInfo )
{
	OlaXmlDocument* xml = OlaXmlDocument::CreateXmlDocument();
	assert( xml->parse(data) && "mat parse error" );

	OlaXmlNode* shader_node = xml->fisrtChild();
	assert(shader_node && "no shader_node!");

	OlaXmlNode* head_node = shader_node->selectFirst("head");
	const char* shader_name = head_node->attribute("name");
	const char* fx_type = head_node->attribute("fx_type");

	outTplInfo->FxType = fx_type;

	OlaXmlNode* prop_node = shader_node->selectFirst("propertys");
	while(prop_node)
	{
		const char* prop_name = prop_node->name();
		const char* prop_type = prop_node->attribute("type");
		const char* prop_bind = prop_node->attribute("bind");
		const char* prop_value = prop_node->attribute("value");
		tTplPropertyInfo pinfo;
		pinfo.Name = prop_name;
		pinfo.Type = prop_type;
		pinfo.Bind = prop_bind;
		pinfo.Value = prop_value;
		outTplInfo->Propertys.push_back(pinfo);

		prop_node = prop_node->nextSibling("propertys");
	}

	OlaXmlNode* state_node = shader_node->selectFirst("states");
	while(state_node)
	{
		const char* state_name = state_node->name();
		const char* state_value = state_node->attribute("value");
		tTplRenderStateInfo rinfo;
		rinfo.Name = state_name;
		rinfo.Value = state_value;
		outTplInfo->RenderStates.push_back(rinfo);

		state_node = state_node->nextSibling("states");
	}

	OlaXmlNode* fxcodes_node = shader_node->selectFirst("fxcodes");

	if (strcmp(fx_type,"glsl") == 0)
	{		
		ReadFXCodeFromGLSL(fxcodes_node,outTplInfo->VertCodes,outTplInfo->FragCodes);
	}

	delete xml;

	return true;
}

bool OlaTPLParser::fillTPL( tTplFileInfo* matInfo,OlaShader* outShader )
{
	return false;
}


static bool ReadFXCodeFromGLSL(OlaXmlNode* GLSLNode,olastring& outVert,olastring& outFrag)
{
	const char* NEW_LINE = "/n";

	OlaArray<olastring> attributes;
	OlaArray<olastring> uniforms;
	OlaArray<olastring> varyings;
	OlaArray<olastring> verts;
	OlaArray<olastring> frags;

	OlaXmlNode* code_node = GLSLNode->fisrtChild();
	while(code_node)
	{
		const char* node_name = code_node->name();

		if (strcmp(node_name,"attribute") == 0)
		{
			const char* type = code_node->attribute("type");
			const char* name = code_node->attribute("name");

			olastring attri("attribute");
			attri.append(" ");
			attri.append(type);
			attri.append(" ");
			attri.append(name);
			attri.append(";");
			attributes.push_back(attri);
		}
		else if(strcmp(node_name,"uniform") == 0)
		{
			const char* type = code_node->attribute("type");
			const char* name = code_node->attribute("name");
			olastring unif("uniform");
			unif.append(" ");
			unif.append(type);
			unif.append(" ");
			unif.append(name);
			unif.append(";");
			uniforms.push_back(unif);
		}
		else if(strcmp(node_name,"varying") == 0)
		{
			const char* type = code_node->attribute("type");
			const char* name = code_node->attribute("name");
			olastring vary("varying");
			vary.append(" ");
			vary.append(type);
			vary.append(" ");
			vary.append(name);
			vary.append(";");
			varyings.push_back(vary);
		}
		else if(strcmp(node_name,"vert") == 0)
		{
			const char* value = code_node->innerText();
			olastring vert_main = "void main(){";
			vert_main.append(value);
			vert_main.append("}");
			verts.push_back(vert_main);
		}
		else if(strcmp(node_name,"frag") == 0)
		{
			const char* value = code_node->innerText();
			olastring frag_main = "void main(){";
			frag_main.append(value);
			frag_main.append("}");
			verts.push_back(frag_main);
		}

		code_node = code_node->nextSibling();
	}

	for (unsigned int i = 0 ; i < attributes.size(); i++)
	{
		outVert.append(attributes[i].accessData());
		outVert.append(NEW_LINE);
	}

	for (unsigned int i = 0 ; i < uniforms.size(); i++)
	{
		outVert.append(uniforms[i].accessData());
		outVert.append(NEW_LINE);
	}

	for (unsigned int i = 0 ; i < attributes.size(); i++)
	{
		outVert.append(attributes[i].accessData());
		outVert.append(NEW_LINE);
	}

	for (unsigned int i = 0 ; i < varyings.size(); i++)
	{
		outVert.append(varyings[i].accessData());
		outVert.append(NEW_LINE);
	}

	for (unsigned int i = 0 ; i < verts.size(); i++)
	{
		outVert.append(verts[i].accessData());
		outVert.append(NEW_LINE);
	}

	for (unsigned int i = 0 ; i < uniforms.size(); i++)
	{
		outFrag.append(uniforms[i].accessData());
		outFrag.append(NEW_LINE);
	}

	for (unsigned int i = 0 ; i < varyings.size(); i++)
	{
		outFrag.append(varyings[i].accessData());
		outFrag.append(NEW_LINE);
	}

	for (unsigned int i = 0 ; i < frags.size(); i++)
	{
		outFrag.append(frags[i].accessData());
		outFrag.append(NEW_LINE);
	}

	return true;
}