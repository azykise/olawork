#ifdef WIN32
#pragma warning(disable:4996)
#endif

#if defined WIN32
#include "GLES2/gl2.h"
#elif defined __ANDROID_API__
#include "GLES2/gl2.h"
#elif defined __APPLE__
#include <OpenGLES/ES2/gl.h>
#endif

#include "../ola_string.h"
#include "../ola_assetmng.h"

#include "ola_glsl.h"

const char TOKEN_PROGRAM_START_VERT[] = "#program vert";
const char TOKEN_PROGRAM_START_FRAG[] = "#program frag";
const char TOKEN_PROGRAM_END[] = "#end";

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

bool GetProgramString(OlaArray<olastring>& lines,olastring& outProgStr,const char* PROG_TOKEN)
{
	bool start = false;
	for (unsigned int i = 0 ; i < lines.size() ; i++)
	{
		olastring& s = lines[i];

		if(!strcmp(s.c_str(),PROG_TOKEN))
		{
			start = true;
			continue;
		}

		if(start)
		{
			if(!strcmp(s.c_str(),TOKEN_PROGRAM_END))
			{
				break;
			}

			outProgStr.append(s);
		}
	}	
	return start;
}

bool OlaGLSL::load( const char* file_name , bool file )
{
	_clear();

	olastring _data;

	if (file)
	{
		mFilename = olastring(file_name);
		OlaAsset* asset = OlaAssetLoader::instance()->load(file_name,false);
		olastring fdata(asset->data);
		delete asset;

		_data = fdata;
	}
	else
	{
		olastring fdata(file_name);
		_data = fdata;
	}

	OlaArray<olastring> outlines;

	int n = OlaUtility::readStringLines(_data,outlines);
	
	olastring vert_program;
	GetProgramString(outlines,vert_program,TOKEN_PROGRAM_START_VERT);

	olastring frag_program;
	GetProgramString(outlines,frag_program,TOKEN_PROGRAM_START_FRAG);

	const char *vstrs[1] = { 0 };
	vstrs[0] = vert_program.c_str();
	mVertHandle = _loadShader(vstrs,GL_VERTEX_SHADER);

	const char *fstrs[1] = { 0 };
	fstrs[0] = frag_program.c_str();
	mFragHandle = _loadShader(fstrs,GL_FRAGMENT_SHADER);

	

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