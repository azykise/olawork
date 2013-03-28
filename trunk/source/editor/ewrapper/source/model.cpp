#include "model.h"
#include "material.h"
#include "primitive.h"
#include "render_system.h"
#include "tool.h"

namespace Engine
{
	StaticModel::StaticModel(System::String^ name,ola::IStaticModel* model,RenderSystem^ rendersys)
	{
		Model = model;
		Name = name;
		RenderSys = rendersys;

		const ola::aabb* paabb = model->aabb();
		float w = paabb->maxv.x - paabb->minv.x;
		float l = paabb->maxv.y - paabb->minv.y;
		float h = paabb->maxv.z - paabb->minv.z;

		float cx = (paabb->maxv.x + paabb->minv.x) / 2;
		float cy = (paabb->maxv.y + paabb->minv.y) / 2;
		float cz = (paabb->maxv.z + paabb->minv.z) / 2;

		AABB = rendersys->createPrimitiveBorderCube(gcnew Vector3F(cx, cy , cz),w,l,h);
	}

	StaticModel::~StaticModel()
	{
		this->!StaticModel();
	}

	StaticModel::!StaticModel()
	{
		if(Model)
		{
			RenderSys->RenderCore->releaseObject(Model);
			Model = 0;
		}

		if(AABB != nullptr)
		{
			delete AABB;
			AABB = nullptr;
		}
	}

	void StaticModel::reloadMaterials()
	{
		if(Model)
		{
			ola::IGeometry* geo = Model->geometry();
			int num = geo->submeshNum();

			for (int i = 0 ; i < num ; i++)
			{
				ola::IMaterial* material = geo->submeshMaterial(i);
				material->reload();
			}
		}
	}

	void StaticModel::render()
	{
		RenderSys->RenderCore->pushRender(Model);
		RenderSys->RenderCore->pushRender(AABB->getPrimitive());
	}

	Bone::Bone(ola::IBone* bonePtr)	
	{
		BonePtr = bonePtr;
		Name = gcnew System::String(BonePtr->name());
		Children = gcnew System::Collections::Generic::List<Bone^>();
		Lines = gcnew System::Collections::Generic::List<PrimitiveLine^>();
	}

	Action::Action()
	{
		Name = gcnew System::String("");
		TotalTime = 0;
		FrameNumber = 0;
	}

	void BuildSkeleton(Bone^ bone,ola::IBone* obone,RenderSystem^ rendersys)
	{
		int sub_num = obone->subNum();
		for (int i = 0 ; i < sub_num ; i++)
		{
			ola::IBone* sub_obone = obone->sub(i);
			Bone^ sub_bone = gcnew Bone(sub_obone);
			PrimitiveLine^ line = rendersys->createPrimitiveLine(gcnew Vector3F(0,0,0),gcnew Vector3F(0,1,0));
			bone->Children->Add(sub_bone);
			bone->Lines->Add(line);
		}		

		for (int i = 0; i < bone->Children->Count ; i++)
		{
			Bone^ sub_bone = bone->Children[i];
			BuildSkeleton(sub_bone,sub_bone->BonePtr,rendersys);
		}
	}

	DynamicModel::DynamicModel( System::String^ name,ola::ICharacter* chr,RenderSystem^ rendersys )
	{
		Name = name;
		Charactor = chr;
		RenderSys = rendersys;

		mCurrentAction = gcnew Action();
		mCurrentAction->FrameNumber = Charactor->getActionFrameNumber(0);
		mCurrentAction->TotalTime = Charactor->getActionTime(0);
		mCurrentAction->Name = gcnew System::String(Charactor->getActionName(0));
		
		mDefaultActionName = gcnew System::String(Charactor->getActionName(0));

		ola::ISkeleton* skeleton = chr->skeleton();
		RootBone = gcnew Bone(skeleton->root());

		BuildSkeleton(RootBone,RootBone->BonePtr,rendersys);
	}

	void DynamicModel::setAction(System::String^ action_name)
	{
		Charactor->setAction(0,Tool::getStringPtr(action_name));

		mCurrentAction = gcnew Action();
		mCurrentAction->FrameNumber = Charactor->getActionFrameNumber(0);
		mCurrentAction->TotalTime = Charactor->getActionTime(0);
		mCurrentAction->Name = gcnew System::String(Charactor->getActionName(0));

	}

	DynamicModel:: ~DynamicModel()
	{

	}

	DynamicModel::!DynamicModel()
	{
		if(Charactor)
		{
			RenderSys->RenderCore->releaseObject(Charactor);
			Charactor = 0;
		}
	}

	void DynamicModel::update(float elasped)
	{

	}

	void DynamicModel::setPassedTime(float passed_time)
	{
		if (Charactor)
		{
			Charactor->setPassedTime(passed_time);
		}
	}

	void RenderSkeletonLines(Bone^ bone,RenderSystem^ rendersys)
	{		
		float* mat0 = bone->BonePtr->matrix();
		for (int i = 0 ; i < bone->Children->Count ; i++)
		{
			Bone^ sub = bone->Children[i];

			PrimitiveLine^ line = bone->Lines[i];			
			
			float* mat1 = sub->BonePtr->matrix();
			
			line->setLine(mat0[3],mat0[7],mat0[11],mat1[3],mat1[7],mat1[11]);			
			
			if (sub->Name->CompareTo("Bip01 Footsteps") == 0)
			{

			}
			else
				rendersys->RenderCore->pushRender(line->getPrimitive());

			RenderSkeletonLines(sub,rendersys);
		}
	}

	void DynamicModel::render()
	{
		if (Charactor)
		{
			RenderSkeletonLines(RootBone,RenderSys);
			RenderSys->RenderCore->pushRender(Charactor->renderer());
		}
	}
}