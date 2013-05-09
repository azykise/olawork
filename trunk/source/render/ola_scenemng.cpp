#include <stdio.h>

#ifdef WIN32
#pragma warning(disable:4996)
#endif

#include "ola_primitive.h"

#include "ola_scenemng.h"

#include "ola_resourcemng.h"
#include "ola_assetmng.h"
#include "model.h"
#include "ola_entity.h"
#include "ola_sklmodel.h"
#include "ola_light.h"

#include "xml/tinyxml.h"
#include "xml/ola_xml.h"

#include "ola_skeleton.h"
#include "ola_animation.h"
#include "ola_action.h"

#include "ola_render.h"

OlaRenderSceneMng::OlaRenderSceneMng(OlaRender* render):
draw_all(false),
mRender(render),
mGirlSkeleton(0),
mScene0(0),
mCurrentScene(0)
{

}

OlaRenderSceneMng::~OlaRenderSceneMng()
{
	clear();
}

void OlaRenderSceneMng::initialize()
{
	mScene0 = newSmallScene("scene0",this);
	mScene0->initialize();
	mCurrentScene = mScene0;

	mGirlSkeleton = mRender->getResourceMng()->getSkeleton("girl_skeleton","art\\role\\skeleton\\girl_skeleton.ase");
}


void OlaRenderSceneMng::clear()
{
	ModelList::iterator m_i = mModels.begin();
	while (m_i != mModels.end())
	{
		delete *m_i;
		m_i++;
	}
	mModels.clear();

	PrimitiveList::iterator p_i = mPrims.begin();
	while (p_i != mPrims.end())
	{
		delete *p_i;
		p_i++;
	}
	mPrims.end();

	SkeletonModelList::iterator sk_i = mSkeletonModels.begin();
	while(sk_i != mSkeletonModels.end())
	{
		delete *sk_i;
		sk_i++;
	}
	mSkeletonModels.clear();

	if(mScene0)
	{
		delete mScene0;
		mScene0 = 0;
	}

	mCurrentScene = 0;
}

const char SCENE_ELEMENT_NUM[] = "num";
const char SCENE_ELEMENT_TYPE[] = "type";
const char SCENE_ELEMENT_NAME[] = "name";
const char SCENE_ELEMENT_POS[] = "position";
const char SCENE_ELEMENT_ROT[] = "rotation";
const char SCENE_ELEMENT_SCA[] = "scale";
const char SCENE_ELEMENT_RES[] = "resource";
const char SCENE_ELEMENT_MAX_TRI[] = "max_tri";

const char SCENE_ROOT[] = "ola_scene";
const char SCENE_RESOURCES[] = "resources";
const char SCENE_NODES[] = "nodes";
const char SCENE_NODE[] = "node";

void OlaRenderSceneMng::loadScene(const char* filename)
{
	//OlaRender* render = mRender;
	//OlaAsset* xml_asset = render->getLoader()->load(filename);

	//const char* p_xml = xml_asset->data;

	//TiXmlDocument xml;
	//xml.Parse(p_xml);
	//if(xml.Error())
	//{
	//	lg("scene xml %s parse error");
	//}

	//TiXmlElement* scene_node = xml.FirstChildElement(SCENE_ROOT);

	////TiXmlElement* resources_node = scene_node->FirstChildElement(SCENE_RESOURCES);
	////TiXmlElement* res_node = resources_node->FirstChildElement(SCENE_ELEMENT_RES);
	////while(res_node)
	////{
	////	const char* res_name = res_node->Attribute(SCENE_ELEMENT_NAME);
	////	olastring ase_name(res_name);
	////	olastring mat_name = ase_name.getPrefix().append(".mat");

	////	OlaSubMesh* mesh = render->getMesh(ase_name.c_str());
	////	OlaMaterial* mat = render->getMaterial(mat_name.c_str());
	////	
	////	res_node = res_node->NextSiblingElement(SCENE_ELEMENT_RES);
	////}

	//int i = 0;
	//int scene_max_tri = 0;
	//int scene_tris = 0;
	//TiXmlElement* nodes = scene_node->FirstChildElement(SCENE_NODES);

	//const char* max_tri_att = nodes->Attribute(SCENE_ELEMENT_MAX_TRI);
	//if(max_tri_att != 0)
	//{
	//	sscanf(max_tri_att,"%d",&scene_max_tri);
	//}

	//TiXmlElement* node = nodes->FirstChildElement(SCENE_NODE);
	//while (node)
	//{
	//	const char* name = node->Attribute(SCENE_ELEMENT_NAME);
	//	const char* type = node->Attribute(SCENE_ELEMENT_TYPE);

	//	//olastring _name(name);

	//	//if (strcmp(name,"beach001_ground001") == 0 ||
	//	//	strcmp(name,"beach001_sea001") == 0 ||
	//	//	strcmp(name,"beach001_sky001") == 0 ||
	//	//	/*strcmp(name,"beach001_stage06") == 0 ||*/
	//	//	//strcmp(name,"beach001_coconut19") == 0 ||
	//	//	strcmp(name,"beach001_coconut67") == 0
	//	//	//_name.contain(olastring("beach001_coconut"))
	//	//	)
	//	//{
	//	//	
	//	//}
	//	//else
	//	//{
	//	//	node = node->NextSiblingElement(SCENE_NODE);
	//	//	continue;
	//	//}

	//	//if(strcmp(name,"beach001_sea001") == 0)
	//	//{
	//	//	node = node->NextSiblingElement(SCENE_NODE);
	//	//	continue;;
	//	//}

	//	//if(i++ == 50)
	//	//	break;
	//	
	//	TiXmlElement* pos = node->FirstChildElement(SCENE_ELEMENT_POS);
	//	TiXmlElement* rot = node->FirstChildElement(SCENE_ELEMENT_ROT);
	//	TiXmlElement* sca = node->FirstChildElement(SCENE_ELEMENT_SCA);
	//	TiXmlElement* res = node->FirstChildElement(SCENE_ELEMENT_RES);

	//	const char* pos_str = pos->GetText();
	//	const char* rot_str = rot->GetText();
	//	const char* sca_str = sca->GetText();
	//	const char* res_str = res->GetText();

	//	float px,py,pz;
	//	sscanf(pos_str,"%f %f %f",&px,&py,&pz);

	//	float qx,qy,qz,qw;
	//	sscanf(rot_str,"%f %f %f %f",&qx,&qy,&qz,&qw);

	//	float sx,sy,sz;
	//	sscanf(sca_str,"%f %f %f",&sx,&sy,&sz);

	//	olastring mdl_name(name);
	//	olastring ase_name(res_str);
	//	olastring mat_name = ase_name.getPrefix().append(".mat");

	//	OlaMesh* mesh = render->getMesh(ase_name.c_str());
	//	
	//	if(max_tri_att != 0)
	//	{
	//		OlaMesh::SubMeshList& submeshs = mesh->submeshs();
	//		OlaMesh::SubMeshList::iterator submesh_i = submeshs.begin();
	//		while(submesh_i != submeshs.end())
	//		{
	//			scene_tris += (*submesh_i)->ib()->num_i() / 3;
	//			submesh_i++;
	//		}

	//		if(scene_tris / 10000 >= scene_max_tri)
	//		{
	//			break;
	//		}
	//	}

	//	OlaMesh::SubMeshList& submat_list = mesh->submeshs();
	//	OlaMesh::SubMeshList::iterator submesh_i = submat_list.begin();
	//	while(submesh_i != submat_list.end())
	//	{
	//		OlaSubMesh* submesh = *submesh_i;			
	//		OlaMaterial* submesh_mat = submesh->material();
	//		if(submesh_mat == 0)
	//		{
	//			OlaMaterial* material = render->getMaterial(mat_name.c_str(),submesh->name().c_str());		
	//			submesh->setMaterial(material);
	//		}
	//		
	//		submesh_i++;
	//	}

	//	CModel* model = new CModel(mdl_name,mesh);

	//	olaQuat q(qx ,qy ,qz ,qw);	
	//	q.Conjugate();
	//	q.Normalize();

	//	model->setRotation(q.x,q.y,q.z,q.w);
	//	model->setScale(sx,sy,sz);
	//	model->setPostition(px,py,pz);
	//	
	//	mModels.push_back(model);

	//	node = node->NextSiblingElement(SCENE_NODE);
	//}
	draw_all = true;
}

#include "parser/ola_dml.h"

const char DML_ROOT[] = "ola_model";
const char DML_GEO[] = "geometry";
const char DML_MAT[] = "material";
const char DML_RESOURCE[] = "resource";
CModel* OlaRenderSceneMng::loadModelFromDML(const char* filename)
{
	//OlaRender* render = mRender;
	//OlaAsset* xml_asset = render->getLoader()->load(filename);

	//olastring dml_filename(filename);

	//tDmlFileInfo dmlInfo;



	//OlaDMLParser parser;
	//parser.parseDMLInfoFromData(xml_asset->data,xml_asset->length,&dmlInfo);
	//parser.fillDML(&dmlInfo,&dml);

	//OlaMesh::SubMeshList& submat_list = dml.Mesh->submeshs();
	//for (unsigned int i = 0 ; i < submat_list.size() ; i++ )
	//{
		//OlaSubMesh* submesh = submat_list[i];
		//olastring& mat_file = dmlInfo.MeshMatsFullname[0];
		//OlaMaterial* submesh_mat = submesh->material();
		//if(submesh_mat == 0)
		//{
		//	OlaMaterial* material = render->getMaterial(mat_file.c_str(),submesh->name().c_str());
		//	assert(material && "no material instance");
		//	submesh->setMaterial(material);
		//}
	//}

	//CModel* model = new CModel(dml_filename,0);
	//return model;

	return 0;
}

const char CHR_ROOT[] = "ola_character";
const char CHR_GEO[] = "geometry";
const char CHR_MAT[] = "material";
const char CHR_SKL[] = "skeleton";
const char CHR_ANI[] = "animation";
const char CHR_NAME[] = "name";
const char CHR_RESOURCE[] = "resource";
OlaSkeletonModel* OlaRenderSceneMng::loadSkeletonModelFromCHR(const char* filename )
{
	OlaAsset* xml_asset = mRender->getLoader()->load(filename);
	const char* p_xml = xml_asset->data;
	TiXmlDocument xml;
	xml.Parse(p_xml);
	assert( !xml.Error() && "chr parse error" );

	TiXmlElement* model_node = xml.FirstChildElement(CHR_ROOT);
	assert(model_node && "no model_node!");

	TiXmlElement* geom_node = model_node->FirstChildElement(CHR_GEO);
	assert(geom_node && "no geom_node!");
	const char* mesh_name = geom_node->Attribute(CHR_RESOURCE);

	TiXmlElement* mat_node = model_node->FirstChildElement(CHR_MAT);
	assert(geom_node && "no mat_node!");
	const char* mat_name = mat_node->Attribute(CHR_RESOURCE);
	
	TiXmlElement* skl_node = model_node->FirstChildElement(CHR_SKL);
	assert(geom_node && "no mat_node!");
	const char* skl_name = skl_node->Attribute(CHR_RESOURCE);

	TiXmlElement* ani_node = model_node->FirstChildElement(CHR_ANI);
	assert(geom_node && "no mat_node!");
	const char* ani_name = ani_node->Attribute(CHR_RESOURCE);

	OlaMesh* mesh = mRender->getMesh(mesh_name);
	OlaMesh::SubMeshList& submat_list = mesh->submeshs();
	OlaMesh::SubMeshList::iterator submesh_i = submat_list.begin();
	while(submesh_i != submat_list.end())
	{
		OlaSubMesh* submesh = *submesh_i;			
		OlaMaterial* submesh_mat = submesh->material();
		if(submesh_mat == 0)
		{
			OlaMaterial* material = mRender->getMaterial(mat_name,submesh->name().c_str());
			assert(material && "no material instance");
			submesh->setMaterial(material);
		}
		
		submesh_i++;
	}
	OlaSkeleton* skeleton = mGirlSkeleton;
	assert(skeleton && "no skeleton !");
	
	OlaSkeletonModel* model = new OlaSkeletonModel(skeleton,mRender->getResourceMng());

	model->setCpuSkin(mesh);

	if (strlen(ani_name) != 0)
	{
		model->setAction(ani_name);
	}	
	
	mSkeletonModels.push_back(model);

	return model;
}

OlaRenderScene* OlaRenderSceneMng::createScene( const char* name,SCENE_TYPE t )
{
	OlaRenderScene* scene = 0;
	switch(t)
	{
	case SCENE_SMALL:
		scene = newSmallScene(name,this);
		scene->initialize();
		break;
	}
	return scene;
}

void OlaRenderSceneMng::setRenderScene( OlaRenderScene* scene )
{
	mCurrentScene = scene ? scene : mScene0;
}


