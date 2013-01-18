#pragma once

#include "pre_define.h"

namespace Engine
{
	ref class RenderSystem;
	ref class PrimitiveBorderCube;
	ref class PrimitiveLine;

	public ref class StaticModel
	{
	public:
		StaticModel(System::String^ name,ola::IStaticModel* model,RenderSystem^ rendersys);
		virtual ~StaticModel();
		!StaticModel();

		void reloadMaterials();

		void render();

		System::String^ Name;
		RenderSystem^ RenderSys;
		ola::IStaticModel* Model;
		PrimitiveBorderCube^ AABB;
	};

	public ref class Action
	{
	public:
		Action();
		System::String^ Name;
		int FrameNumber;
		float TotalTime;
	};

	public ref class Bone
	{
	public:
		Bone(ola::IBone* bonePtr);

		System::String^ Name;
		System::Collections::Generic::List<Bone^>^ Children;
		System::Collections::Generic::List<PrimitiveLine^>^ Lines;
		ola::IBone* BonePtr;
	};

	public ref class DynamicModel 
	{
	public:
		DynamicModel(System::String^ name,ola::ICharacter* chr,RenderSystem^ rendersys);
		virtual ~DynamicModel();
		!DynamicModel();

		void setPassedTime(float passed_time);

		void setAction(System::String^ action_name);

		void update(float elasped);

		void render();		

		property Action^ CurrentAction
		{
			Action^ get(){return mCurrentAction;}
		};

		property System::String^ DefActionName
		{
			System::String^ get(){return mDefaultActionName;}
		};

		System::String^ Name;
		RenderSystem^ RenderSys;
		ola::ICharacter* Charactor;		
		Bone^ RootBone;
		
	protected:
		System::String^ mDefaultActionName;
		Action^ mCurrentAction;
	};
}