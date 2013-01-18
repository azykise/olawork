#ifndef _OLA_INCLUDE_RENDER_QUEUE_H__
#define _OLA_INCLUDE_RENDER_QUEUE_H__

#include <vector>
#include <map>

class OlaMaterial;
class OlaSubMesh;
class OlaRenderOp;
class OlaShader;
class OlaResourceMng;
class OlaPrimitive;
class CModel;

//!��Ⱦ�� һ����Ⱦ��ʹ��һ����Ⱦ���� ���Ұ��ղ��ʽ�Op���� 
class OlaRenderGroup
{	
public:
	typedef std::vector<OlaRenderOp*> RenderOpQueue;
	typedef std::map<OlaShader*,RenderOpQueue*> RenderOpQueueTable;
public:
	OlaRenderGroup(){};
	virtual ~OlaRenderGroup();

	virtual void sort(){};
	virtual void clear();

	virtual void initialize(OlaResourceMng* res_mng){};
	virtual void groupRenderOp(OlaRenderOp* op);

	RenderOpQueueTable& opqueues()	{return mOpQueues;};

protected:
	RenderOpQueueTable mOpQueues;
};

class OlaPrimitiveGroup
{
public:
	typedef std::vector<OlaPrimitive*> PrimitiveList;
public:
	virtual PrimitiveList& prims() = 0;

	virtual OlaMaterial* mat() = 0;
};

//!��Ⱦ���� ������һ����Ⱦ�� ��ʵ����һ��OlaRenderGroupMng
class OlaRenderQueue
{
public:
	enum GROUP_TYPE
	{
		SHADOWCAST_GROUP = 0,
		SOILD_GROUP,
		PRIM_GROUP,
		TRANSPARENCY_GROUP,
		GROUP_TYPE_END,
	};
public:

	OlaRenderQueue();
	~OlaRenderQueue();
	
	void initGroup(GROUP_TYPE type,OlaResourceMng* res_mng);
	void release();
	void clear(GROUP_TYPE type);

	void pushToRener(OlaRenderOp* op);
	OlaRenderGroup* group(OlaRenderQueue::GROUP_TYPE type){return mRenderGroups[type];}

	OlaPrimitiveGroup* primGroup(){return mPrimGroup;}

    int totalTris(){return mTotalTris;}
    
protected:

    int mTotalTris ;
	OlaRenderGroup* mRenderGroups[GROUP_TYPE_END]; //fixme ����Ҫ��������ʾ ֱ�����ÿһ��group����һ������ ���߸ɴ�дһ���� ���һ�����ݼ�������̬û����
	
	OlaPrimitiveGroup* mPrimGroup;
};

#endif