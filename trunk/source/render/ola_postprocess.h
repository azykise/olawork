#ifndef _OLA_POST_PROCESS_H__
#define _OLA_POST_PROCESS_H__

#include <vector>
#include <map>

#include "math/ola_math.h"
#include "ola_string.h"

#include "ola_texture.h"
#include "ola_mesh.h"

class OlaRTParam
{
public:
	enum RTPARAM_TYPE
	{
		FRAME_BUFFER = 0,
		COLOR_BUFFER,
		DEPTH_BUFFER,
		STENCIL_BUFFER,

		RTPARAM_TYPE_END
	};
};

class OlaRenderTarget : public OlaTexture
{
public:
	OlaRenderTarget(const char* name);
	~OlaRenderTarget();

	virtual bool initialize(int w ,int h) = 0;	

	virtual int  readTga(OlaAsset* asset){return 0;}
	virtual bool fill(const unsigned char* rgba,int w,int h,int bpp){return false;}
	virtual bool filltest(){return false;}

	void resize(int w,int h);

	olastring& name(){return mName;}
	virtual void* handle() = 0;

protected:
	olastring mName;
};

class OlaMaterial;
class OlaScreenQuadChain;
class OlaScreenQuad : public OlaSubMesh
{
public:

	OlaScreenQuad(const char* name,OlaScreenQuadChain* parent);
	virtual ~OlaScreenQuad();

	virtual bool load(OlaAsset* asset) {return false;}
	virtual bool load(OlaASE::geomobj* geo,MeshFaceList& faces) {return false;}	

	void setIndex(int idx){mIndex = idx;}
	int  index(){return mIndex;}

	olastring& name(){return mName;}

	int pixelW(){return mPixelW;}
	int pixelH(){return mPixelH;}

	void setPixelW(int w){mPixelW = w;}
	void setPixelH(int h){mPixelH = h;}

protected:
	
	int mPixelW;	//控制后处理面积 用这个直去设置glviewport可以控制ps渲染区域的大小
	int mPixelH;	

	int mIndex;
	OlaScreenQuadChain* mParent;
};

class OlaScreenQuadChain
{
public:
	typedef std::vector<OlaScreenQuad*> ScreenQuadList;
	typedef std::map<olastring,OlaScreenQuad*> ScreenQuadTable;
public:
	OlaScreenQuadChain();
	~OlaScreenQuadChain();

	void initialize();
	void release();

	OlaScreenQuad* creatScreenQuad(const char* name,OlaMaterial* mat);
	OlaScreenQuad* getScreenQuad(const char* name);

	ScreenQuadList& list(){return mQuadList;}

	olaMat4& matrix(){return mQuadMatrix;}

protected:
	olaMat4 mQuadMatrix;
	ScreenQuadTable mQuads;
	ScreenQuadList mQuadList;
};

#endif