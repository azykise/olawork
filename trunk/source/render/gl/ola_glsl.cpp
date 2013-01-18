#ifdef WIN32
#pragma warning(disable:4996)
#endif

#if defined WIN32
#include <GLES2/gl2.h>
#elif defined __ANDROID_API__
#include <GLES2/gl2.h>
#elif defined __APPLE__
#include <OpenGLES/ES2/gl.h>
#endif

#include "../ola_string.h"
#include "../ola_assetmng.h"

#include "ola_glsl.h"

const char TOKEN_PROGRAM_START_VERT[] = "#program vert";
const char TOKEN_PROGRAM_START_FRAG[] = "#program frag";

const char TOKEN_PROGRAM_END_VERT[] = "#end";
const char TOKEN_PROGRAM_END_FRAG[] = "#end";

class GLSLReader
{
public:
	GLSLReader(const char* _data):data(_data)
	{
	
	}

	bool parse()
	{
		int vert_s = data.find(TOKEN_PROGRAM_START_VERT,true,0,-1);
		if(vert_s == -1)
			return false;

		int vert_e = data.find(TOKEN_PROGRAM_END_VERT,true,vert_s,-1);
		if(vert_e == -1)
			return false;

		int frag_s = data.find(TOKEN_PROGRAM_START_FRAG,true,0,-1);
		if(frag_s == -1)
			return false;

		int frag_e = data.find(TOKEN_PROGRAM_END_FRAG,true,frag_s,-1);
		if(frag_e == -1)
			return false;

		vert = olastring(data.c_str(),vert_s + strlen(TOKEN_PROGRAM_START_VERT),vert_e);
		frag = olastring(data.c_str(),frag_s + strlen(TOKEN_PROGRAM_START_FRAG),frag_e);

		return true;
	}

	olastring data;
	olastring vert;
	olastring frag;
};

OlaGLSL::OlaGLSL():
mFragHandle(0),
mVertHandle(0),
mProgHandle(0)
{

}

OlaGLSL::~OlaGLSL()
{
	_clear();
}

void OlaGLSL::_clear()
{
	if(mProgHandle)
	{
		glDeleteProgram(mProgHandle);
		mProgHandle = 0;
	}

	if (mVertHandle)
	{
		glDeleteShader(mVertHandle);
		mVertHandle = 0;
	}

	if (mFragHandle)
	{
		glDeleteShader(mFragHandle);
		mFragHandle = 0;
	}
}

#include "../ola_util.h"
unsigned int _loadShader(const char** data,GLenum type)
{
	unsigned int pShader = 0;
	pShader = glCreateShader(type);
	if(glGetError() != GL_NO_ERROR)
	{
		lg("glCreateShader failed \n");
	}

	glShaderSource(pShader, 1, data, 0);
	if(glGetError() != GL_NO_ERROR)
	{
		lg("glShaderSource failed \n");
	}

	GLint iStatus;
	glCompileShader(pShader);
	glGetShaderiv(pShader, GL_COMPILE_STATUS, &iStatus);
	if(iStatus != GL_TRUE) {
		GLint iLen;
		char *sErrorLog = 0;

		glGetShaderiv(pShader, GL_INFO_LOG_LENGTH, &iLen);
		sErrorLog = (char*)malloc(iLen);
		glGetShaderInfoLog(pShader, iLen, NULL, sErrorLog);
		lg("glShader Error: %s \n",sErrorLog);
		free(sErrorLog);
	}

	return pShader;
}

bool OlaGLSL::load( const char* file_name )
{
	_clear();

	mFilename = olastring(file_name);

	OlaAsset* asset = OlaAssetLoader::instance()->load(file_name,false);

	char* data = (char*)(asset->data);

	GLSLReader gr(data);
	gr.parse();

	const char *vstrs[1] = { 0 };
	vstrs[0] = gr.vert.c_str();
	mVertHandle = _loadShader(vstrs,GL_VERTEX_SHADER);

	const char *fstrs[1] = { 0 };
	fstrs[0] = gr.frag.c_str();
	mFragHandle = _loadShader(fstrs,GL_FRAGMENT_SHADER);

	delete asset;

	mProgHandle = glCreateProgram();
	glAttachShader(mProgHandle,mVertHandle);
	glAttachShader(mProgHandle,mFragHandle);
	glLinkProgram(mProgHandle);
	if(glGetError() != GL_NO_ERROR)
	{
		lg("glLinkProgram failed shader: \n");
	}

	return true;
}

unsigned int OlaGLSL::handleByName( const char* name ,void* p0)
{
	return 0;
}

extern const char gGlobalShaderConstName[OlaMaterialParam::SHADER_VARS_END][MAX_MATERIAL_PARAM_STR_SIZE];
int OlaGLSL::requestHandle( OlaMaterialParam::SHADER_VARS type )
{
	const char* var_name = gGlobalShaderConstName[type];
	unsigned int program = mProgHandle;
	int handle = -1;
	if(type < OlaMaterialParam::SHADER_ATTRIB_END)
	{
		handle = glGetAttribLocation(program,var_name);
		if(glGetError() != GL_NO_ERROR)
			lg("glGetAttribLocation %s failed \n",var_name);		
	}
	else if(type > OlaMaterialParam::SHADER_ATTRIB_END && type < OlaMaterialParam::SHADER_VARS_END)
	{	
		handle = glGetUniformLocation(program,var_name);
		if(glGetError() != GL_NO_ERROR)
			lg("glGetUniformLocation %s failed \n",var_name);		
	}
	else 
	{
		// not possible
	}

	return handle;
}

OlaShaderFX* _loadOlaGLSL(const char* filename)
{
	OlaGLSL* glsl = new OlaGLSL();
	glsl->load(filename);
	return glsl;
}