#include "ola_dml.h"
#include "../xml/ola_xml.h"

#include "../ola_ase.h"
#include "../ola_mesh.h"
#include "../ola_assetmng.h"

bool OlaDMLParser::parseDMLInfoFromData( const char* data,int len,tDmlFileInfo* outDmlInfo )
{
	OlaXmlDocument* xml = OlaXmlDocument::CreateXmlDocument();
	assert( xml->parse(data) && "dml parse error" );

	OlaXmlNode* model_node = xml->fisrtChild();
	assert(model_node && "no model_node!");

	OlaXmlNode* geom_node = model_node->selectFirst("geometry");
	assert(geom_node && "no geom_node!");
	const char* mesh_name = geom_node->attribute("resource");
	outDmlInfo->ASEFullname = mesh_name;

	OlaXmlNode* mat_node = model_node->selectFirst("material");
	assert(geom_node && "no mat_node!");
	
	const char* mat_numstr = mat_node->attribute("num");
	int mat_num = atoi(mat_numstr);
	outDmlInfo->MeshMatsFullname.resize(mat_num);

	OlaXmlNode* submat_node = mat_node->selectFirst("submat");
	while(submat_node)
	{
		const char* idxstr = submat_node->attribute("index");
		int idx = atoi(idxstr);

		olastring submat_res = submat_node->attribute("resource");
		outDmlInfo->MeshMatsFullname[idx] = submat_res;

		submat_node = submat_node->nextSibling("submat");
	}	

	return true;
}

bool OlaDMLParser::fillDML( tDmlFileInfo* dmlInfo,tDmlResult* dml )
{
	OlaAsset* asset = OlaAssetLoader::instance()->load(dmlInfo->ASEFullname.c_str(),false);

	dml->Mesh = new OlaMesh(dmlInfo->ASEFullname.c_str());

	OlaMeshParser parser;
	parser.parseMeshFromData(asset->data,asset->length,dml->Mesh);

	delete asset;

	return true;
}

OlaASE::matobj* _GetMaterialLeaf(OlaASE::matobj* matnode, int submat_id)
{
	if (!matnode) return 0;

	if (matnode->classname)
	{
		if (!strcmp(matnode->classname,"\"Multi/Sub-Object\""))
			return _GetMaterialLeaf(matnode->submat[submat_id],submat_id);
	}

	if (!matnode->submat.size() || submat_id >= (int)matnode->submat.size() )//no sub material , it's a standard material
		return matnode;

	return _GetMaterialLeaf(matnode->submat[submat_id],submat_id);
}

const char*	_GetMeshShaderName(int matid, int submat_id, OlaASE::mat_array& matlib)
{
	if (matid<0)//no material on it
		return 0;

	OlaASE::matobj* mat = 0;
	OlaASE::matobj* matnode = matlib[matid];


	//get the root material node
	mat = _GetMaterialLeaf(matnode,submat_id);

	if (!mat)
		return 0;

	return mat->name;
}

typedef OlaMap<int,const char*> SubMaterialTable;
bool FillSubMesh( OlaSubMesh* submesh,OlaASE::geomobj* geo0,OlaMeshParser::MeshFaceList& faces );
bool OlaMeshParser::parseMeshFromData( const char* data,int len,OlaMesh* outMesh )
{
	OlaASE::model* model = OlaASE::load(data,len,1.0f);

	if(!model || model->geom.size() == 0)
	{
		lg("ase parse failed , asset == 0\n");
		return false;
	}

	SubMaterialTable submats; //find out the sub materials , every submat will subdevision a submesh

	OlaASE::geomobj* geo0 = model->geom[0]; //art spec: one ase file have only one mesh inside

	for (unsigned int i = 0 ; i < geo0->numf ; i++)
	{
		int submat_id = geo0->fsubmat[i];

		SubMaterialTable::iterator submat_i = submats.find(submat_id);
		if (submat_i == submats.end())
		{
			const char* shadername = _GetMeshShaderName(geo0->matidx,submat_id,model->mat);
			submats[submat_id] = shadername;
			//submats
		}
		else
			continue;
	}

	SubMaterialTable::iterator submat_i = submats.begin();
	while(submat_i != submats.end())
	{
		int submat_id = submat_i->first;
		const char* submat_name = submat_i->second;		

		MeshFaceList faces;

		for (unsigned int i = 0; i < geo0->numf ; ++i)
		{
			int face_submat_id = geo0->fsubmat[i];
			if(face_submat_id == submat_id)
			{
				faces.push_back(i);
			}
		}

		olastring _submesh_name(submat_name);
		olastring submesh_name(_submesh_name.c_str(),1,_submesh_name.length() - 1);

		OlaSubMesh* submesh = new OlaSubMesh(submesh_name.c_str());
		
		FillSubMesh(submesh,geo0,faces);

		outMesh->submeshs().push_back(submesh);

		submat_i++;
	}
	
	delete model;

	return true;
}

struct tOlaFixedVertex
{
	float pos[3];
	float nor[3];
	float uv0[2];
	float tan[3];
	float bnr[3];
};

bool FillSubMesh( OlaSubMesh* submesh,OlaASE::geomobj* geo0,OlaMeshParser::MeshFaceList& faces )
{
	int num_face = faces.size();
	int num_vert = num_face * 3;

	assert( geo0->v && geo0->n && geo0->t && 
			geo0->bn && geo0->tn && "error mesh import!");

	OlaVB::ElementList elements;
	elements.push_back(OlaVBParam::ELEMENT_POS);
	elements.push_back(OlaVBParam::ELEMENT_NOR);
	elements.push_back(OlaVBParam::ELEMENT_UV0);
	elements.push_back(OlaVBParam::ELEMENT_TAN);
	elements.push_back(OlaVBParam::ELEMENT_BNR);

	submesh->vb()->initialize(elements,num_vert,OlaVBParam::VBO_STATIC);
	tOlaFixedVertex* vert_data = (tOlaFixedVertex*)(submesh->vb()->lock());

	for (int i = 0 ; i < num_face ; i++)
	{
		int face_index = faces[i] * 3;
		int idx = i * 3;
		int idx_v0 = geo0->f[face_index + 0] * 3;
		int idx_v1 = geo0->f[face_index + 1] * 3;
		int idx_v2 = geo0->f[face_index + 2] * 3;

		vert_data[idx + 0].pos[0] = geo0->v[idx_v0 + 0];
		vert_data[idx + 0].pos[1] = geo0->v[idx_v0 + 1];
		vert_data[idx + 0].pos[2] = geo0->v[idx_v0 + 2];

		vert_data[idx + 1].pos[0] = geo0->v[idx_v1 + 0];
		vert_data[idx + 1].pos[1] = geo0->v[idx_v1 + 1];
		vert_data[idx + 1].pos[2] = geo0->v[idx_v1 + 2];

		vert_data[idx + 2].pos[0] = geo0->v[idx_v2 + 0];
		vert_data[idx + 2].pos[1] = geo0->v[idx_v2 + 1];
		vert_data[idx + 2].pos[2] = geo0->v[idx_v2 + 2];
	}

	for (int i = 0 ; i < num_face ; i++)
	{
		int face_index = faces[i] * 3;
		int idx = i * 3;
		int idx_uv0 = geo0->tf[face_index + 0] * 3;
		int idx_uv1 = geo0->tf[face_index + 1] * 3;
		int idx_uv2 = geo0->tf[face_index + 2] * 3;

		vert_data[idx + 0].uv0[0] = geo0->t[idx_uv0 + 0];
		vert_data[idx + 0].uv0[1] = geo0->t[idx_uv0 + 1];

		vert_data[idx + 1].uv0[0] = geo0->t[idx_uv1 + 0];
		vert_data[idx + 1].uv0[1] = geo0->t[idx_uv1 + 1];

		vert_data[idx + 2].uv0[0] = geo0->t[idx_uv2 + 0];
		vert_data[idx + 2].uv0[1] = geo0->t[idx_uv2 + 1];
	}

	for (int i = 0 ; i < num_face ; i++)
	{
		int face_index = faces[i] * 3;
		int idx = i * 3;
		int idx_n0 = geo0->fvn[face_index + 0] * 3;
		int idx_n1 = geo0->fvn[face_index + 1] * 3;
		int idx_n2 = geo0->fvn[face_index + 2] * 3;

		//--
		vert_data[idx + 0].nor[0] = geo0->n[idx_n0 + 0];
		vert_data[idx + 0].nor[1] = geo0->n[idx_n0 + 1];
		vert_data[idx + 0].nor[2] = geo0->n[idx_n0 + 2];

		vert_data[idx + 0].tan[0] = geo0->tn[idx_n0 + 0];
		vert_data[idx + 0].tan[1] = geo0->tn[idx_n0 + 1];
		vert_data[idx + 0].tan[2] = geo0->tn[idx_n0 + 2];

		vert_data[idx + 0].bnr[0] = geo0->bn[idx_n0 + 0];
		vert_data[idx + 0].bnr[1] = geo0->bn[idx_n0 + 1];
		vert_data[idx + 0].bnr[2] = geo0->bn[idx_n0 + 2];

		//--
		vert_data[idx + 1].nor[0] = geo0->n[idx_n1 + 0];
		vert_data[idx + 1].nor[1] = geo0->n[idx_n1 + 1];
		vert_data[idx + 1].nor[2] = geo0->n[idx_n1 + 2];

		vert_data[idx + 1].tan[0] = geo0->tn[idx_n1 + 0];
		vert_data[idx + 1].tan[1] = geo0->tn[idx_n1 + 1];
		vert_data[idx + 1].tan[2] = geo0->tn[idx_n1 + 2];

		vert_data[idx + 1].bnr[0] = geo0->bn[idx_n1 + 0];
		vert_data[idx + 1].bnr[1] = geo0->bn[idx_n1 + 1];
		vert_data[idx + 1].bnr[2] = geo0->bn[idx_n1 + 2];

		//--
		vert_data[idx + 2].nor[0] = geo0->n[idx_n2 + 0];
		vert_data[idx + 2].nor[1] = geo0->n[idx_n2 + 1];
		vert_data[idx + 2].nor[2] = geo0->n[idx_n2 + 2];

		vert_data[idx + 2].tan[0] = geo0->tn[idx_n2 + 0];
		vert_data[idx + 2].tan[1] = geo0->tn[idx_n2 + 1];
		vert_data[idx + 2].tan[2] = geo0->tn[idx_n2 + 2];

		vert_data[idx + 2].bnr[0] = geo0->bn[idx_n2 + 0];
		vert_data[idx + 2].bnr[1] = geo0->bn[idx_n2 + 1];
		vert_data[idx + 2].bnr[2] = geo0->bn[idx_n2 + 2];
	}	

	submesh->vb()->unlock();

	submesh->ib()->initialize(num_vert);
	unsigned short* index = submesh->ib()->lock();
	for (int i = 0 ; i < num_vert ; i++)
	{
		index[i] = i;
	}
	submesh->ib()->unlock();

	//if(geo0->numvbone != 0 && geo0->numskelbone != 0)
	//{
	//	mVertexSkinInfos.resize(num_vert);
	//	for (int i = 0 ; i < num_face ; i++)
	//	{
	//		int face_index = faces[i] * 3;
	//		int idx = i * 3;
	//		int idx_v0 = geo0->f[face_index + 0];
	//		int idx_v1 = geo0->f[face_index + 1];
	//		int idx_v2 = geo0->f[face_index + 2];

	//		tVSkinInfo s0,s1,s2;

	//		OlaASE::vbonedata& vbone0 = geo0->vbones[idx_v0];
	//		copyVBoneToSkinInfo(&vbone0,&s0);

	//		OlaASE::vbonedata& vbone1 = geo0->vbones[idx_v1];
	//		copyVBoneToSkinInfo(&vbone1,&s1);

	//		OlaASE::vbonedata& vbone2 = geo0->vbones[idx_v2];
	//		copyVBoneToSkinInfo(&vbone2,&s2);

	//		mVertexSkinInfos[idx + 0] = s0;
	//		mVertexSkinInfos[idx + 1] = s1;
	//		mVertexSkinInfos[idx + 2] = s2;
	//	}

	//	for (int i = 0 ; i < geo0->numskelbone ; i++)
	//	{
	//		OlaASE::skeletonbone* sb= geo0->skelbones + i;

	//		tSBone* sbone = new tSBone();
	//		sbone->id_ase = sb->id;
	//		sbone->name = olastring(sb->name);
	//		mSkeletonBones.push_back(sbone);
	//	}
	//}

	for (int i = 0 ; i < num_vert ; i++)
	{
		submesh->minv()[0] = vert_data[i].pos[0] < submesh->minv()[0] ? vert_data[i].pos[0] : submesh->minv()[0];
		submesh->minv()[1] = vert_data[i].pos[1] < submesh->minv()[1] ? vert_data[i].pos[1] : submesh->minv()[1];
		submesh->minv()[2] = vert_data[i].pos[2] < submesh->minv()[2] ? vert_data[i].pos[2] : submesh->minv()[2];

		submesh->maxv()[0] = vert_data[i].pos[0] > submesh->maxv()[0] ? vert_data[i].pos[0] : submesh->maxv()[0];
		submesh->maxv()[1] = vert_data[i].pos[1] > submesh->maxv()[1] ? vert_data[i].pos[1] : submesh->maxv()[1];
		submesh->maxv()[2] = vert_data[i].pos[2] > submesh->maxv()[2] ? vert_data[i].pos[2] : submesh->maxv()[2];
	}
	return true;
}
