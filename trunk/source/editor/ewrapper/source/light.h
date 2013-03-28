#pragma once

#include "pre_define.h"

namespace Engine
{
	ref class RenderSystem;
	ref class PrimitiveBorderCube;
	ref class PrimitiveLine;
	ref class Scene;

	public ref class Light
	{
	public:
		Light(System::String^ name,ola::ILight* light,RenderSystem^ rendersys);
		virtual ~Light();
		!Light();

		property Vector3F^ SrcPos
		{
			Vector3F^ get()
			{
				ola::vec3 v(0,0,0);
				mLight->transform()->getPosition(&v);
				mSrcPos->X = v.x;
				mSrcPos->Y = v.y;
				mSrcPos->Z = v.z;
				return mSrcPos;
			}
			void set(Vector3F^ p)
			{
				mSrcPos = p;
				mLight->transform()->setPosition(mSrcPos->X,mSrcPos->Y,mSrcPos->Z);
			}
		};
		property Vector3F^ DstPos
		{
			Vector3F^ get()
			{
				ola::vec3 v(0,0,0);
				mLight->transform()->getPosition(&v);
				mDstPos->X = v.x;
				mDstPos->Y = v.y;
				mDstPos->Z = v.z;
				return mDstPos;
			}
			void set(Vector3F^ p)
			{
				mDstPos = p;
				mLight->setTargetPos(mDstPos->X,mDstPos->Y,mDstPos->Z);
			}
		}

		void attach(Scene^ scene);
		void detach();

	protected:

		System::String^ mName;
		ola::ILight* mLight;
		RenderSystem^ mRenderSys;

		Vector3F^ mSrcPos;
		Vector3F^ mDstPos;
	};
}