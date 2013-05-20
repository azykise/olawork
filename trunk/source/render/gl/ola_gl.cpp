#include <assert.h>

#if defined WIN32
#include "GLES2/gl2.h"
#include "EGL/egl.h"
#elif defined __ANDROID_API__
#include "GLES2/gl2.h"
#elif defined __APPLE__
#include <OpenGLES/ES2/gl.h>
#endif

#include "../ola_util.h"
#include "../math/ola_math.h"
#include "../ola_material.h"
#include "../ola_render.h"
#include "../ola_geobuffer.h"
#include "../ola_fxconst.h"

#include "ola_gl.h"
#include "ola_glib.h"
#include "ola_glvb.h"
#include "ola_glsl.h"
#include "ola_glrt.h"
#include "ola_gltexture.h"
#include "ola_glswbuffer.h"

#define lgGLError(x) 	if(glGetError() != GL_NO_ERROR){lg(x);}

OlaGLDevice::OlaGLDevice():
mEGLDisplay(0),
mEGLContext(0),
mEGLSurface(0)
{
	mEGLConfigs[0] = 0;
}

void OlaGLDevice::_createGLRenderContext(unsigned int h)
{
	EGLint majorVersion;
	EGLint minorVersion;
	mEGLDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	assert( mEGLDisplay != EGL_NO_DISPLAY && "Unable to open connection to local windowing system \n");

	assert( eglInitialize(mEGLDisplay, &majorVersion, &minorVersion) && "Unable to initialize EGL");

	EGLint aEGLAttributes[] = {
		EGL_RED_SIZE, 5,
		EGL_GREEN_SIZE, 6,
		EGL_BLUE_SIZE, 7,
		EGL_DEPTH_SIZE, 16,
		EGL_SURFACE_TYPE,
		EGL_WINDOW_BIT,
		EGL_RENDERABLE_TYPE, 
		EGL_OPENGL_ES2_BIT,
		EGL_NONE
	};

	EGLint aEGLContextAttributes[] = {
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};

	EGLint cEGLConfigs;

	assert(eglChooseConfig(mEGLDisplay, aEGLAttributes, mEGLConfigs, 1, &cEGLConfigs) && "eglChooseConfig failed !");

	assert( cEGLConfigs && "No EGL configurations were returned.\n");

	mEGLContext = eglCreateContext(mEGLDisplay, mEGLConfigs[0], EGL_NO_CONTEXT, aEGLContextAttributes);
	assert( mEGLContext != EGL_NO_CONTEXT && "Failed to create EGL context.\n");

	mEGLSurface = eglCreateWindowSurface(mEGLDisplay, mEGLConfigs[0], (HWND)h, 0);
	if (mEGLSurface = EGL_NO_SURFACE) {
		lg("Failed to create EGL surface.\n");
		exit(-1);
	}

	if(!eglMakeCurrent(mEGLDisplay, mEGLSurface, mEGLSurface, mEGLContext))
		lg("eglMakeCurrent Failed \n !!!");	
}


OlaGLDevice::~OlaGLDevice()
{
	if (mEGLSurface)
	{
		eglDestroySurface(mEGLDisplay,mEGLSurface);
		mEGLSurface = 0;
	}

	if (mEGLContext)
	{
		eglDestroyContext(mEGLDisplay,mEGLContext);
		mEGLContext = 0;
	}
}

OlaVB* OlaGLDevice::spawnVB()
{
	return new OlaGLVB();
}

OlaIB* OlaGLDevice::spawnIB()
{
	return new OlaGLIB();
}

OlaTexture* OlaGLDevice::spawnTexture()
{
	return new OlaGLTexture();
}

OlaSwapBuffer* OlaGLDevice::spawnSwapBuffer()
{
	return new OlaEGLBufferWin(this);
}

OlaShaderFX* OlaGLDevice::spawnShaderFX()
{
	return new OlaGLSL();
}

OlaRenderTarget* OlaGLDevice::spawnRenderTarget( const char* name )
{
	if (strcmp(name,"gl_default_rt") == 0)
	{
		return new OlaGLOriginRT();
	}
	else
		return new OlaGLRenderTarget(name);
}

void OlaGLDevice::setRenderTarget( OlaRenderTarget* rt )
{
 	int handle = *static_cast<int*>(rt->handle());
	glBindFramebuffer(GL_FRAMEBUFFER,handle);  
	glViewport(0,0,rt->width(),rt->height());
	glClearColor(0,0,1.0,0);
	glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT); 	
}

void OlaGLDevice::bindRenderState( OlaShader* shader , OlaGlobalFXConst* shaderconst)
{
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW); 
	glEnable(GL_DEPTH_TEST);

	GLuint program = *static_cast<GLuint*>(shader->program());

	int pos_handle = shader->handle(OlaMaterialParam::POSITION);
	int nor_handle = shader->handle(OlaMaterialParam::NORMAL);
	int uv0_handle = shader->handle(OlaMaterialParam::UV0);	
	int tan_handle = shader->handle(OlaMaterialParam::TANGENT);
	int bnr_handle = shader->handle(OlaMaterialParam::BINORMAL);

	int sample0_handle = shader->handle(OlaMaterialParam::SAMPLE_DIFFUSE);	
	int sample1_handle = shader->handle(OlaMaterialParam::SAMPLE_NORMAL);
	int sample2_handle = shader->handle(OlaMaterialParam::SAMPLE_SPECULAR);

	int eye_handle = shader->handle(OlaMaterialParam::EYE_POS);

	int ld0_handle = shader->handle(OlaMaterialParam::LIGHT_DIR0);
	int lp0_handle = shader->handle(OlaMaterialParam::LIGHT_POS0);

	glUseProgram(program);	

	if(pos_handle != -1)
	{
		glEnableVertexAttribArray(pos_handle);
		lgGLError("glEnableVertexAttribArray(pos_handle); \n");
	}

	if(nor_handle != -1)
	{
		glEnableVertexAttribArray(nor_handle);
		lgGLError("glEnableVertexAttribArray(nor_handle); \n");		
	}

	if(uv0_handle != -1)
	{
		glEnableVertexAttribArray(uv0_handle);
		lgGLError("glEnableVertexAttribArray(uv0_handle); \n");
	}

	if (tan_handle != -1)
	{
		glEnableVertexAttribArray(tan_handle);
		lgGLError("glEnableVertexAttribArray(uv0_handle); \n");
	}

	if (bnr_handle != -1)
	{
		glEnableVertexAttribArray(bnr_handle);
		lgGLError("glEnableVertexAttribArray(uv0_handle); \n");
	}

	if(eye_handle != -1)
	{
		glUniform3f(eye_handle,shaderconst->EyePos.x,shaderconst->EyePos.y,shaderconst->EyePos.z);
	}

	if(ld0_handle != -1)
	{
		olaVec3 light_dir(0,0,0);
		if (shaderconst->LightNum != 0)
		{
			light_dir = shaderconst->LightParam[0].dst - shaderconst->LightParam[0].pos;
			light_dir.Normalize();
		}
		glUniform3f(ld0_handle,light_dir.x,light_dir.y,light_dir.z);
	}

	if(lp0_handle != -1)
	{
		olaVec3 light_pos(0,0,0);
		if (shaderconst->LightNum != 0)
		{
			glUniform3f(lp0_handle,shaderconst->LightParam[0].pos.x,shaderconst->LightParam[0].pos.y,shaderconst->LightParam[0].pos.z);
		}		
	}

	if(sample0_handle != -1)
	{
		glUniform1i(sample0_handle,0);	
	}

	if(sample1_handle != -1)
	{
		glUniform1i(sample1_handle,1);
	}

	if (sample2_handle != -1)
	{
		glUniform1i(sample2_handle,2);
	}	
}

void OlaGLDevice::wipeRenderState( OlaShader* shader )
{
	int pos_handle = shader->handle(OlaMaterialParam::POSITION);
	int nor_handle = shader->handle(OlaMaterialParam::NORMAL);
	int uv0_handle = shader->handle(OlaMaterialParam::UV0);
	int tan_handle = shader->handle(OlaMaterialParam::TANGENT);
	int bnr_handle = shader->handle(OlaMaterialParam::BINORMAL);

	if(pos_handle != -1)
	{
		glDisableVertexAttribArray(pos_handle);
	}

	if(nor_handle != -1)
	{
		glDisableVertexAttribArray(nor_handle);
	}

	if(uv0_handle != -1)
	{
		glDisableVertexAttribArray(uv0_handle);
	}

	if(tan_handle != -1)
	{
		glDisableVertexAttribArray(tan_handle);
	}

	if(bnr_handle != -1)
	{
		glDisableVertexAttribArray(bnr_handle);
	}	
}

void OlaGLDevice::flushMaterialInfo( OlaMaterial* material )
{
	OlaShader* shader = material->shader();

	int c0_handle = shader->handle(OlaMaterialParam::COLOR0);
	olaVec4* c0 = material->paramentValue<olaVec4*>(OlaMaterialParam::USER_COLOR0);
	if(c0_handle != -1 && c0)
	{
		glUniform4f(c0_handle,c0->x,c0->y,c0->z,c0->w);
	}

	void* no_cull = material->paramentValue<void*>(OlaMaterialParam::NO_CULLFACE);
	if(no_cull)
	{
		glDisable(GL_CULL_FACE);
	}
	else
	{
		glEnable(GL_CULL_FACE);
	}

	OlaTexture* diffuse0 = material->paramentValue<OlaTexture*>(OlaMaterialParam::DIFFUSE0);	
	int diffuse_handle = -1;	
	if(diffuse0)
	{
		diffuse_handle = *static_cast<int*>(diffuse0->handle());
	}

	if(diffuse_handle != -1)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,diffuse_handle);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR ); // GL_LINEAR GL_NEAREST GL_NEAREST_MIPMAP_NEAREST
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //GL_CLAMP_TO_EDGE GL_REPEAT
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		lgGLError("glBindTexture(GL_TEXTURE_2D,diffuse_handle); \n");
	}

	OlaTexture* normal0 = material->paramentValue<OlaTexture*>(OlaMaterialParam::NORMAL0);
	int normal_handle = -1;
	if (normal0)
	{
		normal_handle = *static_cast<int*>(normal0->handle());
	}
	if (normal_handle != -1)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D,normal_handle);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR ); // GL_LINEAR GL_NEAREST GL_NEAREST_MIPMAP_NEAREST
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //GL_CLAMP_TO_EDGE GL_REPEAT
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		lgGLError("glBindTexture(GL_TEXTURE_2D,normal_handle); \n");
	}

	OlaTexture* sepcular0 = material->paramentValue<OlaTexture*>(OlaMaterialParam::SPECULAR0);
	int specular_handle = -1;
	if (sepcular0)
	{
		specular_handle = *static_cast<int*>(sepcular0->handle());
	}
	if (specular_handle != -1)
	{
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D,specular_handle);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST ); // GL_LINEAR GL_NEAREST GL_NEAREST_MIPMAP_NEAREST
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //GL_CLAMP_TO_EDGE GL_REPEAT
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		lgGLError("glBindTexture(GL_TEXTURE_2D,specular_handle); \n");
	}
}

void OlaGLDevice::drawCall( OlaRenderOp* op , OlaGlobalFXConst* shaderconst)
{
	static olaMat4 mv,mvp,mw;
	
	static float Smodel[16];
	static float Smv[16];
	static float Smvp[16];
	static float Smwi[16];

	OlaShader* shader = op->material()->shader();

	int pos_handle = shader->handle(OlaMaterialParam::POSITION);
	int nor_handle = shader->handle(OlaMaterialParam::NORMAL);
	int uv0_handle = shader->handle(OlaMaterialParam::UV0);	
	int tan_handle = shader->handle(OlaMaterialParam::TANGENT);
	int bnr_handle = shader->handle(OlaMaterialParam::BINORMAL);

	int mvp_handle = shader->handle(OlaMaterialParam::MVP);
	int mti_handle = shader->handle(OlaMaterialParam::MV_INVERSE);
	int mod_handle = shader->handle(OlaMaterialParam::MODEL);
	int mv_handle = shader->handle(OlaMaterialParam::MV);
 
	op->worldtrans.CopyToFloatArrayColumn(Smodel);

	mv = shaderconst->ViewMat * op->worldtrans;
	mv.CopyToFloatArrayColumn(Smv);

	mvp = shaderconst->ProjMat * mv;
	mvp.CopyToFloatArrayColumn(Smvp);	
	
	mw = op->worldtrans;
	mw[0][3] = 0.0f;mw[1][3] = 0.0f;
	mw[2][3] = 0.0f;mw[3][3] = 1.0f;
	mw.Inverse().Transpose().CopyToFloatArrayColumn(Smwi);	

	unsigned int handle_vb = *static_cast<unsigned int*>(op->vb->handle());
	unsigned int handle_ib = *static_cast<unsigned int*>(op->ib->handle());

	OlaRenderParam::DRAWCALL_PRIM_MODE prim_mode = op->drawmode;

	int stride = op->vb->stride();
	int num_index = op->ib->num_i();

	glBindBuffer(GL_ARRAY_BUFFER,handle_vb);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle_ib);
	
	GLuint offset = op->vb->elementOffset(OlaVBParam::ELEMENT_POS);
	if(pos_handle != -1 && offset != -1)
	{		
		glVertexAttribPointer(pos_handle, 3, GL_FLOAT, GL_FALSE, stride, (const void*)offset);
		lgGLError("glVertexAttribPointer(pos_handle, 3, GL_FLOAT, GL_FALSE, stride, (const void*)offset); \n");	
	}

	offset =op->vb->elementOffset(OlaVBParam::ELEMENT_NOR);
	if(nor_handle != -1 && offset != -1)
	{
		glVertexAttribPointer(nor_handle, 3, GL_FLOAT, GL_FALSE, stride,(const void*)offset);
		lgGLError("glVertexAttribPointer(nor_handle, 3, GL_FLOAT, GL_FALSE, stride,(const void*)offset); \n");
	}

	offset =op->vb->elementOffset(OlaVBParam::ELEMENT_UV0);
	if(uv0_handle != -1 && offset != -1)
	{
		glVertexAttribPointer(uv0_handle, 2, GL_FLOAT, GL_FALSE, stride, (const void*)offset);
		lgGLError("glVertexAttribPointer(uv0_handle, 2, GL_FLOAT, GL_FALSE, stride, (const void*)offset); \n");
	}

	offset =op->vb->elementOffset(OlaVBParam::ELEMENT_TAN);
	if(tan_handle != -1 && offset != -1)
	{
		glVertexAttribPointer(tan_handle, 3, GL_FLOAT, GL_FALSE, stride, (const void*)offset);
		lgGLError("glVertexAttribPointer(tan_handle, 3, GL_FLOAT, GL_FALSE, stride, (const void*)offset); \n");
	}

	offset =op->vb->elementOffset(OlaVBParam::ELEMENT_BNR);
	if(bnr_handle != -1 && offset != -1)
	{
		glVertexAttribPointer(bnr_handle, 3, GL_FLOAT, GL_FALSE, stride, (const void*)offset);
		lgGLError("glVertexAttribPointer(bnr_handle, 3, GL_FLOAT, GL_FALSE, stride, (const void*)offset); \n");
	}

	if(mv_handle != -1)
	{
		glUniformMatrix4fv(mv_handle,1,GL_FALSE,Smv);
	}
	if(mvp_handle != -1)
	{
		glUniformMatrix4fv(mvp_handle,1,GL_FALSE,Smvp);
	}	
	if(mod_handle != -1)
	{
		glUniformMatrix4fv(mod_handle,1,GL_FALSE,Smodel);
	}	
	if(mti_handle != -1)
	{
		glUniformMatrix4fv(mti_handle,1,GL_FALSE,Smwi);
	}

	switch(prim_mode)
	{
		case OlaRenderParam::DRAWCALL_PRIM_MODE::PRIM_TRILIST:
			glDrawElements(GL_TRIANGLES, num_index, GL_UNSIGNED_SHORT, 0);
			break;
		case OlaRenderParam::DRAWCALL_PRIM_MODE::PRIM_LINE:
			glDrawElements(GL_LINES, num_index, GL_UNSIGNED_SHORT, 0);
			break;
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D,0);	
}

void OlaGLDevice::setSwapBuffer( OlaSwapBuffer* swb )
{
	OlaEGLBufferWin* glbuffer = static_cast<OlaEGLBufferWin*>(swb);
	void* egl_surface = swb->handle();

	EGLContext cur_context = eglGetCurrentContext();

	if(cur_context == glbuffer->mEGLContext)
		return;

	if(!eglMakeCurrent(mEGLDisplay, egl_surface, egl_surface, glbuffer->mEGLContext))
	{
		lg("eglMakeCurrent Failed \n !!!");
	}
}

void OlaGLDevice::init(unsigned int h)
{
#if defined WIN32
	_createGLRenderContext(h);
#endif
}

void OlaGLDevice::setViewport( int x,int y,int w,int h )
{
	glViewport(x,y,w,h);
}

void OlaGLDevice::clearViewport( float r,float g,float b,float a )
{
	glClearColor(0,0,0,0);
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(r,g,b,a);
	glClear(GL_COLOR_BUFFER_BIT); 
}

