#include "ola_tpl.h"

#include "../xml/ola_xml.h"

#include "../ola_device.h"
#include "../ola_assetmng.h"

#include "../ola_shaderfx.h"
#include "../ola_material.h"
#include "../ola_shaderpool.h"
#include "../ola_texturepool.h"

olastring ReadFXCodeFromGLSL(OlaXmlNode* GLSLNode);

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
		outTplInfo->FXCodes = ReadFXCodeFromGLSL(fxcodes_node);
	}

	delete xml;

	return true;
}

bool OlaTPLParser::fillTPL( tTplFileInfo* matInfo,OlaShader* outShader )
{
	return false;
}


static olastring ReadFXCodeFromGLSL(OlaXmlNode* GLSLNode)
{
	olastring glsl_code;

	return glsl_code;
}