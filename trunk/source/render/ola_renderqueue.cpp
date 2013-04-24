#include <assert.h>

#include "ola_renderqueue.h"

#include "ola_material.h"
#include "ola_mesh.h"
#include "model.h"
#include "ola_resourcemng.h"

OlaRenderGroup::~OlaRenderGroup()
{
	RenderOpQueueTable::iterator i = mOpQueues.begin();
	while (i != mOpQueues.end())
	{
		delete i->second;
		i++;
	}
	mOpQueues.clear();
}

void OlaRenderGroup::clear()
{
	RenderOpQueueTable::iterator i = mOpQueues.begin();
	while (i != mOpQueues.end())
	{
		i->second->clear();
		i++;
	}
	mOpQueues.clear();
}

void OlaRenderGroup::groupRenderOp(OlaRenderOp* op)
{
	OlaShader* shader = op->material()->shader();
	if(shader)
	{
		RenderOpQueue* queue = 0;

		RenderOpQueueTable::iterator i = mOpQueues.find(shader);
		if( i == mOpQueues.end())
		{
			queue = new RenderOpQueue();
			mOpQueues[shader] = queue;
		}
		else
		{
			queue = i->second;
		}

		queue->push_back(op);
	}
}

class OlaShadowMapRenderGroup : public OlaRenderGroup
{
public:
	OlaShadowMapRenderGroup()
	{

	}

	virtual void initialize(OlaResourceMng* res_mng)
	{
		OlaMaterial* shadowmap_mat = res_mng->getShadowcastMaterial();
		shadowmap_mat->addRef();

		mShadowMapShader = shadowmap_mat->shader();
		mShadowMapQueue = new RenderOpQueue();
		mOpQueues[mShadowMapShader] = mShadowMapQueue;
	}

	virtual void groupRenderOp(OlaRenderOp* op)
	{
		mShadowMapQueue->push_back(op);
	}

	virtual void clear()
	{
		mOpQueues[mShadowMapShader]->clear();
	}

	RenderOpQueue* mShadowMapQueue;
	OlaShader* mShadowMapShader;
};

class OlaPrimitiveRenderGroup : public OlaRenderGroup , public OlaPrimitiveGroup
{
public:
	OlaPrimitiveRenderGroup():
	mWireFrameMat(0)
	{

	}

	virtual void initialize(OlaResourceMng* res_mng)
	{
		mWireFrameMat = res_mng->getPrimitiveMaterial();
	}

	virtual void groupRenderOp(OlaRenderOp* op)
	{
		assert(0 && "no op import in primgroup!");
	}

	virtual void clear()
	{
		mPrims.clear();
	}

	virtual PrimitiveList& prims()
	{
		return mPrims;
	}

	virtual OlaMaterial* mat()
	{
		return mWireFrameMat;
	}

	OlaMaterial* mWireFrameMat;

	PrimitiveList mPrims;
};

OlaRenderQueue::OlaRenderQueue():
mTotalTris(0),
mPrimGroup(0)
{
	memset(mRenderGroups,0,GROUP_TYPE_END * sizeof(OlaRenderGroup*));
}

OlaRenderQueue::~OlaRenderQueue()
{
	release();
}

void OlaRenderQueue::initGroup(GROUP_TYPE type,OlaResourceMng* res_mng)
{
	switch (type)
	{
		case SHADOWCAST_GROUP:
			mRenderGroups[SHADOWCAST_GROUP] = new OlaShadowMapRenderGroup();
			mRenderGroups[SHADOWCAST_GROUP]->initialize(res_mng);
			break;
		case SOILD_GROUP:
			mRenderGroups[SOILD_GROUP] = new OlaRenderGroup();
			mRenderGroups[SOILD_GROUP]->initialize(res_mng);
			break;
		case TRANSPARENCY_GROUP:
			
			mRenderGroups[TRANSPARENCY_GROUP] = new OlaRenderGroup();
			mRenderGroups[TRANSPARENCY_GROUP]->initialize(res_mng);
			break;
	}		

	if(type == PRIM_GROUP)
	{
		OlaPrimitiveRenderGroup* p = new OlaPrimitiveRenderGroup();
		mRenderGroups[PRIM_GROUP] = p; 
		mRenderGroups[PRIM_GROUP]->initialize(res_mng);
		mPrimGroup = p;
	}
}

void OlaRenderQueue::release()
{
	if(mRenderGroups[SHADOWCAST_GROUP])
		delete mRenderGroups[SHADOWCAST_GROUP];
	if(mRenderGroups[SOILD_GROUP])
		delete mRenderGroups[SOILD_GROUP];
	if(mRenderGroups[PRIM_GROUP])
	{
		delete mRenderGroups[PRIM_GROUP];
		mPrimGroup = 0;
	}
	if(mRenderGroups[TRANSPARENCY_GROUP])
		delete mRenderGroups[TRANSPARENCY_GROUP];

	memset(mRenderGroups,0,GROUP_TYPE_END * sizeof(OlaRenderGroup*));
}

void OlaRenderQueue::clear(GROUP_TYPE type)
{
	switch (type)
	{
		case SHADOWCAST_GROUP:
			mRenderGroups[SHADOWCAST_GROUP]->clear();
			break;
		case SOILD_GROUP:
			mRenderGroups[SOILD_GROUP]->clear();
			break;
		case PRIM_GROUP:
			mRenderGroups[PRIM_GROUP]->clear();
			break;
		case TRANSPARENCY_GROUP:
			mRenderGroups[TRANSPARENCY_GROUP]->clear();
			break;
	}	
    mTotalTris = 0;
}

void OlaRenderQueue::pushToRener(OlaRenderOp* op)
{
	OlaMaterial* material = op->material();

	unsigned int shadow_cast = material->paramentValue<unsigned int>(OlaMaterialParam::CAST_SHADOW);
	if(shadow_cast && mRenderGroups[SHADOWCAST_GROUP])
	{
		mRenderGroups[SHADOWCAST_GROUP]->groupRenderOp(op);
	}

	switch(op->drawmode)
	{
		case OlaRenderParam::DRAWCALL_PRIM_MODE::PRIM_LINE:
			mRenderGroups[PRIM_GROUP]->groupRenderOp(op);
			break;
		case OlaRenderParam::DRAWCALL_PRIM_MODE::PRIM_TRILIST:
			mRenderGroups[SOILD_GROUP]->groupRenderOp(op);
			mTotalTris += op->ib->num_i() / 3;
			break;
	}

}

