#include "scene.h"
#include "light.h"
#include "model.h"
#include "material.h"
#include "primitive.h"
#include "render_system.h"
#include "tool.h"

namespace Engine
{
	Scene::Scene(System::String^ name,ola::IScene* scene,RenderSystem^ rendersys)
	{
		mName = name;
		mRenderSys = rendersys;
		mScene = scene;
	}

	Scene:: ~Scene()
	{
		this->!Scene();
	}

	Scene::!Scene()
	{
		if (mScene)
		{
			mRenderSys->RenderCore->releaseObject(mScene);
			mScene = 0;
		}
	}
}