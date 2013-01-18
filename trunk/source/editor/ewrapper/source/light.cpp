#include "light.h"
#include "scene.h"
#include "model.h"
#include "material.h"
#include "primitive.h"
#include "render_system.h"
#include "tool.h"

namespace Engine
{
	Light::Light( System::String^ name,ola::ILight* light,RenderSystem^ rendersys )
	{
		mLight = light;
		mRenderSys = rendersys;
		mName = name;

		ola::tVec3 v(0,0,0);
		mLight->transform()->getPosition(&v);
		mSrcPos = gcnew Vector3F(v.x,v.y,v.z);

		mLight->getTargetPos(&v);
		mDstPos = gcnew Vector3F(v.x,v.y,v.z);
	}

	Light::~Light()
	{
		this->!Light();
	}

	Light::!Light()
	{
		if (mLight)
		{
			detach();
			mRenderSys->RenderCore->releaseObject(mLight);
			mLight = 0;
		}
	}

	void Light::attach( Scene^ scene )
	{
		scene->scene()->attachEntry(mLight->entry());
	}

	void Light::detach()
	{
		mLight->entry()->detachScene();
	}

}