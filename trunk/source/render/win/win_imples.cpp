#include "win_impls.h"
#include "../ola_mesh.h"

#include "../ola_resourcemng.h"
#include "../ola_skeleton.h"
#include "../ola_mesh.h"
#include "../ola_skin.h"
#include "../ola_sklmodel.h"
#include "../ola_action.h"
#include "../ola_action_res.h"
#include "../ola_model.h"

#include "../impls/ola_geometry_impl.h"
#include "../impls/ola_transform_impl.h"

OlaBoneImpl::OlaBoneImpl( OlaBone* bone ,OlaSkeletonImpl* skl):
mBone(bone),
mSkeleton(skl)
{

}

OlaBoneImpl::~OlaBoneImpl()
{

}

int OlaBoneImpl::id()
{
	return mBone->index;
}


float* OlaBoneImpl::matrix()
{
	return mSkeleton->mParent->boneState(mBone->index)->matrix_cur.ToFloatPtr();
}

const char* OlaBoneImpl::name()
{
	return mBone->name.c_str();
}

ola::IBone* OlaBoneImpl::sub( int idx )
{
	return (mChildren[idx]);
}

OlaStaticModelImpl::OlaStaticModelImpl(OlaMeshRenderer* model,OlaRender* render):
mGeometry(0),
mTransform(0)
{
	mTransform = new OlaTransformImpl();
	mGeometry = new OlaGeometryImpl(model,render);	

	mTransform->transform()->pushKernelObj(model);
}

OlaStaticModelImpl::~OlaStaticModelImpl()
{	
	mTransform->transform()->popKernelObj(mGeometry->meshrender());
	delete mGeometry;
	delete mTransform;
}

ola::IGeometry* OlaStaticModelImpl::geometry()
{
	return mGeometry;
}


ola::ITransform* OlaStaticModelImpl::transform()
{
	return mTransform;
}


OlaSkeletonImpl::OlaSkeletonImpl( OlaSkeletonModel* skl ):
mParent(skl),
mRoot(0)
{

}

OlaSkeletonImpl::~OlaSkeletonImpl()
{
	for (size_t i = 0 ; i < mBones.size() ; i++)
	{
		delete mBones[i];
	}	
	mBones.clear();
}


void OlaSkeletonImpl::_buildSkeleton()
{
	OlaSkeleton* skeleton = mParent->skeleton();

	mBones.resize(skeleton->bones().size(),0);
	for (size_t i = 0 ; i < skeleton->bones().size() ; i++)
	{
		OlaBone* bone = skeleton->bones().at(i);
		OlaBoneImpl* impl = new OlaBoneImpl(bone,this);
		mBones[bone->index] = impl;
	}

	for (size_t i = 0 ; i < mBones.size() ; i++)
	{
		OlaBoneImpl* impl = mBones[i];
		
		if (impl->mBone->parent)
		{
			OlaBoneImpl* parent = mBones[impl->mBone->parent->index];
			impl->mParent = parent;
			parent->mChildren.push_back(impl);
		}
		else
		{
			mRoot = impl;
			impl->mParent = 0;			
		}
	}
}

const char* OlaSkeletonImpl::filename()
{
	return 0;
}

int OlaSkeletonImpl::boneNum()
{
	return mParent->skeleton()->bones().size();
}

ola::IBone* OlaSkeletonImpl::bone( int id )
{
	return mBones[id];
}

ola::IBone* OlaSkeletonImpl::root()
{
	return mRoot;
}

OlaRenderableImpl::OlaRenderableImpl()
{

}

OlaRenderableImpl::~OlaRenderableImpl()
{
	clear();
}


void OlaRenderableImpl::clear()
{
	for (size_t i = 0 ; i < mRenderOps.size() ; i++)
	{
		OlaRenderOp* op = mRenderOps.at(i);
		delete op;
	}
	mRenderOps.clear();
}

ola::aabb* OlaStaticModelImpl::aabb()
{
	mAABB.minv.x = mAABB.minv.y = mAABB.minv.z = olaMath::INFINITY;
	mAABB.maxv.x = mAABB.maxv.y = mAABB.maxv.z = -1.0f * olaMath::INFINITY;

	OlaMesh* mesh = mGeometry->meshrender()->mesh();

	OlaMesh::SubMeshList& submeshs = mesh->submeshs();
	OlaMesh::SubMeshList::iterator i = submeshs.begin();
	while( i != submeshs.end())
	{
		OlaSubMesh* sub_mesh = *i;

		float* minv = sub_mesh->minv();
		float* maxv = sub_mesh->maxv();

		mAABB.minv.x = minv[0] < mAABB.minv.x ? minv[0] : mAABB.minv.x;
		mAABB.minv.y = minv[1] < mAABB.minv.y ? minv[1] : mAABB.minv.y;
		mAABB.minv.z = minv[2] < mAABB.minv.z ? minv[2] : mAABB.minv.z;

		mAABB.maxv.x = maxv[0] > mAABB.maxv.x ? maxv[0] : mAABB.maxv.x;
		mAABB.maxv.y = maxv[1] > mAABB.maxv.y ? maxv[1] : mAABB.maxv.y;
		mAABB.maxv.z = maxv[2] > mAABB.maxv.z ? maxv[2] : mAABB.maxv.z;

		i++;
	}

	return &mAABB;
}

OlaCharacterImpl::OlaCharacterImpl( OlaSkeletonModel* model ):
mSkeletonModel(model),
mRenderer(0)
{
	mRenderer = new OlaRenderableImpl();
	
	OlaMesh::SubMeshList& submeh_list = model->skin()->mesh()->submeshs();
	OlaMesh::SubMeshList::iterator i = submeh_list.begin();
	while(i != submeh_list.end())
	{
		OlaRenderOp* op = new OlaRenderOp(*i);
		mRenderer->mRenderOps.push_back(op);
		i++;
	}

	mSkeleton = new OlaSkeletonImpl(model);
	mSkeleton->_buildSkeleton();
}

OlaCharacterImpl::~OlaCharacterImpl()
{
	delete mRenderer;
	delete mSkeleton;
}

ola::IRenderable* OlaCharacterImpl::renderer()
{
	return mRenderer;
}

ola::aabb* OlaCharacterImpl::aabb()
{
	return 0;
}

void OlaCharacterImpl::update( float elasped )
{
	mSkeletonModel->update(elasped);
}


void OlaCharacterImpl::setPassedTime( float t )
{
	mSkeletonModel->setPassedTime(t);
}


void OlaCharacterImpl::setAction(int channel, const char* action_name )
{
	mSkeletonModel->setAction(action_name);
}

int OlaCharacterImpl::getActionFrameNumber( int channel )
{
	OlaAction* action = mSkeletonModel->currentAction(channel);

	if (action && action->actionResource())
	{
		return action->actionResource()->clipNum();
	}
	return 0;
}

float OlaCharacterImpl::getActionTime( int channel )
{
	OlaAction* action = mSkeletonModel->currentAction(channel);

	if (action)
	{
		return action->totalTime();
	}

	return 0;
}


const char* OlaCharacterImpl::getActionName( int channel )
{
	OlaAction* action = mSkeletonModel->currentAction(channel);

	if (action)
	{
		return action->name();
	}

	return 0;
}

OlaSceneImpl::OlaSceneImpl( OlaRenderScene* scene,OlaRenderSceneMng* mng ):
mScene(scene),
mSceneMng(mng)
{

}

OlaSceneImpl::~OlaSceneImpl()
{
	if (mScene)
	{
		OlaRenderScene* current_scene = mSceneMng->scene();
		if (current_scene == mScene)
		{
			mSceneMng->setRenderScene(0);
		}
		mScene->release();
		delete mScene;
		mScene = 0;
	}	
}

const char* OlaSceneImpl::name()
{
	return mScene->name().c_str();
}

void OlaSceneImpl::attach( ola::ITransform* transform )
{
	OlaTransformImpl* impl = static_cast<OlaTransformImpl*>(transform);
	impl->mSceneImpl = this;
	mScene->attachObj(impl->transform());
}
