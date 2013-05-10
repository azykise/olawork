#ifndef _OLA_INCLUDE_ENGINE_INTERFACE_H__
#define _OLA_INCLUDE_ENGINE_INTERFACE_H__

//#include <windows.h>

namespace ola
{
	struct vec3
	{
		vec3(float _x,float _y,float _z):x(_x),y(_y),z(_z){}
		float x,y,z;
	};

	struct aabb
	{
		aabb():maxv(0,0,0),minv(0,0,0){}
		vec3 maxv;
		vec3 minv;
	};	

	struct quat
	{
		quat(float _x,float _y,float _z,float _w):x(_x),y(_y),z(_z),w(_w){}
		float x,y,z,w;
	};

	struct RenderWindowInfo
	{
		char name[128];
		unsigned int hwnd;
		int width;
		int height;
		bool full;
	};

	class IObject
	{
	public:
		virtual ~IObject(){};	
	};

	class IRenderWindow : public IObject
	{
	public:	
		virtual ~IRenderWindow(){}
		virtual void setBackgroundColor(float r,float g,float b,float a) = 0;

		virtual void resizeRenderWindow(RenderWindowInfo*) = 0;

		virtual int getWidth() = 0;
		virtual int getHeight() = 0;

		virtual int getIndex() = 0;
	};

	enum PrimitiveType
	{
		PMT_UNKNOWN = -1,
		PMT_LINE,			//直线
	};

	class IPrimitive : public IObject 
	{
	public:
		virtual ~IPrimitive(){};

		virtual unsigned long long getID() = 0;

		virtual PrimitiveType getType() = 0;

		//length 为 数组 vec3s 的长度
		virtual void setVertex(float* vec3s,int length) = 0;

		virtual void setColor(float r,float g,float b,float a) = 0;

		virtual void setIndex(int* idxs,int length) = 0;
	};

	class IResource
	{
	public:
		virtual ~IResource(){};

		virtual const char* getResourceFilename() = 0;	

		virtual void reload() = 0;

	};

	class IRenderable
	{
	public:
		virtual ~IRenderable(){};
	};

	class IMaterial : public IResource
	{
	public:
		virtual const char* name() = 0;
	};

	class IGeometry : public IResource
	{
	public:
		virtual int submeshNum() = 0;

		virtual IMaterial* submeshMaterial(int idx) = 0;
	};

	class IBone
	{
	public:
		virtual ~IBone(){}

		virtual const char* name() = 0;

		virtual int id() = 0;		

		virtual float* matrix() = 0; //4x4 world space matrix

		virtual IBone* parent() = 0;

		virtual int subNum() = 0;
		virtual IBone* sub(int idx) = 0;
	};

	class ISkeleton : public IResource
	{
	public:
		virtual int boneNum() = 0;

		virtual IBone* bone(int id) = 0;

		virtual IBone* root() = 0;
	};

	class IScene;
	class ITransform
	{
	public:
		virtual IScene* scene() = 0;
		virtual void detachScene() = 0;	

		virtual void getPosition(vec3* out_pos) = 0;
		virtual void setPosition(float x,float y,float z) = 0;

		virtual void getRotation(quat* out_rot) = 0;
		virtual void setRotation(float axisx,float axisy,float axisz,float degree) = 0;
	};

	class ILight : public IObject
	{
	public:
		virtual ITransform* transform() = 0;

		virtual void setTargetPos(float x,float y,float z) = 0;
		virtual void getTargetPos(vec3* out_pos) = 0;
	};

	//!对应于一个dml
	class IStaticModel : public IObject 
	{
	public:
		virtual IGeometry* geometry() = 0;

		virtual ITransform* transform() = 0;

		virtual aabb* aabb() = 0;
	};

	//!animated model 对应于一个chr
	class ICharacter: public IObject 
	{
	public:
		virtual void setPassedTime(float t) = 0;

		virtual void update(float elasped) = 0;	

		virtual void setAction(int channel,const char* action_name) = 0;

		virtual int getActionFrameNumber(int channel) = 0;
		virtual float getActionTime(int channel) = 0;
		virtual const char* getActionName(int channel) = 0;

		virtual IRenderable* renderer() = 0;

		virtual ISkeleton* skeleton() = 0;

		virtual ITransform* transform() = 0;

		virtual aabb* aabb() = 0;
	};

	//!一个角色 可以换装 对应于一个bpt,多个chr
	class IActor : public ICharacter
	{
	public:
		virtual void setBodyPart(const char* bpt_name) = 0;
	};

	struct tViewParam
	{
		float pos[3];
		float lookat[3];
		float fovy;
		float aspect;
		float near_dist;
		float far_dist;	
	};

	class IScene : public IObject
	{
	public:
		enum SCENE_TYPE
		{
			SCENE_SMALL = 0,
			SCENE_OUTDOOR,
			SCENE_INDOOR,
			SCENE_TYPE_END
		};
	public:
		virtual const char* name() = 0;

		virtual void attach(ITransform* transform) = 0;
	};

	class IRenderCore
	{
	public:
		virtual bool initialize(unsigned int hwnd) = 0;
		virtual void release() = 0;

		virtual void beginDraw() = 0;
		virtual void drawSceneActors() = 0;
		virtual void endDraw() = 0;

		virtual void getView(tViewParam* vp) = 0;
		virtual void setView(tViewParam* vp) = 0;
		virtual void setCamera(int index,vec3& pos,vec3& tag) = 0;
		virtual void getCamera(int index,vec3& pos,vec3& tag) = 0;
		virtual void getViewMatrix(float* mat) = 0;
		virtual void getProjMatrix(float* mat) = 0;

		virtual IRenderWindow* createWindow(RenderWindowInfo* info) = 0;
		virtual void setWindow(IRenderWindow* wnd) = 0;

		virtual void pushRender(IPrimitive* r) = 0;

		virtual void pushHardwareSelect(IPrimitive* primitive,unsigned int sel_id) = 0;

		virtual void beginHardwareSelect(int x,int y,int w,int h) = 0;
		virtual unsigned int* endHardwareSelect(int& num) = 0;

		virtual IScene* createScene(const char* name,IScene::SCENE_TYPE t = IScene::SCENE_SMALL) = 0;
		virtual void setRenderingScene(IScene* scene) = 0;

		virtual IPrimitive*	createPrimitive(PrimitiveType type)= 0;
		virtual IStaticModel* createStaticModel(const char* dml_filename) = 0;
		virtual ICharacter* createCharacter(const char* chr_filename) = 0;
		virtual IActor* createActor(const char* chr_filename) = 0;
		virtual ILight* createLight(const char* name) = 0;

		virtual void releaseObject(IObject* object) = 0;

		virtual void clearResourceCache(const char* res_name) = 0;
		virtual int getResourceNum(const char* res_name) = 0;
	};
}

#endif