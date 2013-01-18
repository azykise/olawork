#ifndef _OLA_INCLUDE_SCENE_INTERFACE_H__
#define _OLA_INCLUDE_SCENE_INTERFACE_H__

#include "ola_icommon.h"
#include "ola_irendercore.h"

namespace ola
{
	class IScene;
	class ISceneCell
	{
	public:
		virtual void combineShape(tAABB* aabb) = 0;
		virtual void combineShape(tVec3* vec3s,int type) = 0;
		virtual void combineShape(float radius) = 0;

		virtual IScene* scene() = 0;

		virtual void detach() = 0;

		virtual ITransform* transform() = 0;

		virtual void aabb(tAABB* out_aabb) = 0;
		virtual void obb(tVec3* out_obb) = 0;
		virtual float radius() = 0;

		virtual void setGroupID(int group_id) = 0;
		virtual int groupID() = 0;		

		virtual bool visible() = 0;
	};

	class IScene
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
		
		virtual ISceneCell* createCell() = 0;

		virtual void attachCell(ISceneCell* cell) = 0;
 
		virtual void setCamera(ICamera* cam) = 0;

		virtual void update() = 0;

		virtual ISceneCell** visibleCells(int group_id,int* num) = 0;
	};	
}

#endif