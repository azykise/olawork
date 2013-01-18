#include "ola_skeleton.h"
#include "ola_ase.h"
#include "ola_resourcemng.h"

OlaBone::OlaBone():
name("none"),
index(-1),
parent(0)
{
	matrix_iorg.Identity();
	matrix_org.Identity();

	pos_r.Set(0,0,0);
	rot_r.Set(0,0,0,1);
}

OlaBone::~OlaBone()
{

}

OlaSkeleton::OlaSkeleton(const char* name):
mName(name),
mRoot(0),
mRootID(0)
{

}

OlaSkeleton::~OlaSkeleton()
{
	for (int i = 0 ; i < mBones.size() ; i++)
	{
		OlaBone* b = mBones.at(i);
		delete b;
	}
	mBones.clear();
}

bool OlaSkeleton::load(OlaAsset* asset)
{
	OlaASE::model* model = OlaASE::load((char*)asset->data,asset->length,1.0f);
	assert( model && model->geom.size() != 0 && "ase parse failed , OlaSkeleton::load !!");

	std::map<OlaBone*,olastring> parent_map;
	std::map<olastring,OlaBone*> name_map;

	for (size_t i = 0 ; i < model->geom.size() ; i++)
	{
		OlaASE::geomobj* geo = model->geom[i];
		olastring bone_name(geo->name);		
		
		std::map<olastring,OlaBone*>::iterator bi = name_map.find(bone_name);
		if(bi == name_map.end())
		{
			OlaBone* nbone = new OlaBone();
			nbone->name = bone_name;
			name_map[bone_name] = nbone;
		}

		OlaBone* bone = name_map[bone_name];
		//memcpy(bone->matrix_org.ToFloatPtr(),geo->tm,sizeof(float) * 16);
		bone->matrix_org.FromFloatsColumnMajor(geo->tm);
		bone->matrix_iorg = bone->matrix_org.Inverse();

		if(geo->parent != 0)
		{
			olastring parent_name(geo->parent->name);
			bi = name_map.find(parent_name);
			if(bi == name_map.end())
			{
				OlaBone* pbone = new OlaBone();
				pbone->name = parent_name;
				name_map[parent_name] = pbone;
			}
			OlaBone* parent_bone = name_map[parent_name];
			bone->parent = parent_bone;
			parent_bone->children.push_back(bone);			
		}
	}

	int id = 0;
	mBones.resize(name_map.size(),0);
	std::map<olastring,OlaBone*>::iterator bi = name_map.begin();
	while( bi != name_map.end())
	{		
		OlaASE::geomobj* bone_obj = 0;
		OlaASE::geomobj* parent_obj = 0;
		OlaBone* bone = bi->second;

		for (size_t i = 0 ; i < model->geom.size() ; i++)
		{
			OlaASE::geomobj* obj = model->geom[i];
			if(!strcmp(bone->name.c_str(),obj->name))
				bone_obj = obj;
			else if( bone->parent && !strcmp(bone->parent->name.c_str(),obj->name))
				parent_obj = obj;
		}

		olaVec3 scale;

		if (bone->parent == 0)
		{
			if(mRoot == 0)
			{
				mRoot = bone;
				mRootID = id;
			}
			else
				assert("more than one root bone!");

			bone->matrix_org.Decompose(scale,bone->rot_r,bone->pos_r);
		}
		else
		{
			olaMat4 m = bone->parent->matrix_iorg * bone->matrix_org;
			m.Decompose(scale,bone->rot_r,bone->pos_r);
		}
		
		bone->index = id;
		mBones[id] = bone;

		id++;
		bi++;
	}

	delete model;

	assert( mRoot && "no skeleton root bone !");

	return true;
}

OlaBone* OlaSkeleton::bone(olastring& name)
{
	for (int i = 0 ; i < mBones.size() ; i++)
	{
		OlaBone* b = mBones.at(i);
		if(b->name == name)
			return b;
	}
	return 0;
}

void OlaSkeleton::_setBoneRelativeMatrix( OlaBone* bone )
{
	assert( bone->parent && "no parent bone !" );


	
}
