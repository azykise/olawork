#ifndef _OLA_INCLUDE_TESTGAME_INTERFACE_H__
#define _OLA_INCLUDE_TESTGAME_INTERFACE_H__

#include "ola_engine.h"

//面向具体游戏逻辑的中间层接口
//不同游戏的主要改动都在这里发生
namespace ola
{
	class IGameObj
	{
	public:
		virtual ~IGameObj(){}
	};

	class IGameEntity : public IGameObj
	{
	public:
		virtual void setName() = 0;

		virtual const char* name() = 0;

		virtual ITransform* transform() = 0;		

		virtual ISceneCell* scenecell() = 0;
	};

	class IGameCamera : public IGameEntity
	{
	public:
		virtual IViewFrustum* camera() = 0;
	};

	class IGameActor : public IGameEntity
	{
	public:
		virtual update(float elasped_sec) = 0;
		virtual IRenderable* renderable() = 0;
	};

	class IGame
	{
	public:
		virtual IRenderCore* renderCore() = 0;

		virtual IScene* createScene(const char* name, IScene::SCENE_TYPE t) = 0;		

		virtual IGameCamera* createCamera() = 0;

		virtual IGameActor* createActor() = 0;

		virtual void deleteGameObject(IGameObj* obj) = 0;
	};

}

#endif