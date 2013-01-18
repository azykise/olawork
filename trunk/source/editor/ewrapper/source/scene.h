#pragma once

#include "pre_define.h"

namespace Engine
{
	ref class RenderSystem;
	ref class PrimitiveBorderCube;
	ref class PrimitiveLine;
	ref class Light;

	public ref class Scene
	{
	public:
		Scene(System::String^ name,ola::IScene* scene,RenderSystem^ rendersys);
		virtual ~Scene();
		!Scene();
		
		ola::IScene* scene(){return mScene;}

	protected:
		System::String^ mName;
		ola::IScene* mScene;
		RenderSystem^ mRenderSys;
	};
}