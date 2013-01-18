/*********************************************************************NVMH1****
File:
nv_ase.cpp

Copyright (C) 1999, 2000 NVIDIA Corporation
This file is provided without support, instruction, or implied warranty of any
kind.  NVIDIA makes no guarantee of its fitness for a particular purpose and is
not liable under any circumstances for any damages or loss whatsoever arising
from the use or inability to use this file or items derived from it.

Comments:


******************************************************************************/
#pragma warning(disable:4996)
#ifndef _WIN32
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <ctype.h>
#endif

#include "ola_ase.h"

#include <assert.h>
#include <math.h>

// File mapper name
#define NV_ASE_MAPPER_NAME "NvASEFileMapper"

#define TOKEN_SIZE 256
#ifndef MAX_PATH
#define MAX_PATH 4096 
#endif
#ifndef _WIN32
static void OutputDebugString (const char * debugstr) {
	fprintf (stderr, "%s", debugstr);
}
#define _stricmp strcasecmp
#endif

#include "math/ola_asemath.h"

namespace OlaASE
{
	int     g_stream_cnt;
	int     g_stream_max_cnt;
	char    g_token[TOKEN_SIZE];
	float   g_scale_hack = 0.0002f;
	char    g_keyword[TOKEN_SIZE];
	char    g_leftover[TOKEN_SIZE];

	// ase tokens
	const char tok_close[]              = "}";
	const char tok_group[]              = "GROUP";
	const char tok_geomobject[]         = "GEOMOBJECT";
	const char tok_helperobject[]       = "HELPEROBJECT";
	const char tok_scene[]              = "SCENE";
	const char tok_shapeobject[]        = "SHAPEOBJECT";
	const char tok_cameraobject[]       = "CAMERAOBJECT";
	const char tok_lightobject[]        = "LIGHTOBJECT";

	const char tok_node_name[]          = "NODE_NAME";
	const char tok_node_parent[]        = "NODE_PARENT";
	const char tok_node_tm[]            = "NODE_TM";

	const char tok_tm_animation[]       = "TM_ANIMATION";
	const char tok_tm_row0[]            = "TM_ROW0";
	const char tok_tm_row1[]            = "TM_ROW1";
	const char tok_tm_row2[]            = "TM_ROW2";
	const char tok_tm_row3[]            = "TM_ROW3";
	const char tok_pos[]				= "TM_POS";
	const char tok_rotaxis[]			= "TM_ROTAXIS";
	const char tok_rotangle[]			= "TM_ROTANGLE";

	const char tok_mesh[]               = "MESH";
	const char tok_mesh_numvertices[]   = "MESH_NUMVERTEX";
	const char tok_mesh_numfaces[]      = "MESH_NUMFACES";
	const char tok_mesh_numtvertices[]  = "MESH_NUMTVERTEX";
	const char tok_mesh_numtvertlist[]  = "MESH_TVERTLIST";
	const char tok_mesh_numcvertex[]    = "MESH_NUMCVERTEX";
	const char tok_mesh_numtvfaces[]    = "MESH_NUMTVFACES";
	const char tok_mesh_vertexlist[]    = "MESH_VERTEX_LIST";
	const char tok_mesh_facelist[]      = "MESH_FACE_LIST";
	const char tok_mesh_normals[]       = "MESH_NORMALS";
	const char tok_mesh_tangents[]		= "MESH_TANGENTS";	
	const char tok_mesh_tfacelist[]     = "MESH_TFACELIST";
	const char tok_mesh_tvert[]         = "MESH_TVERT";
	const char tok_mesh_vertex[]        = "MESH_VERTEX";
	const char tok_mesh_face[]          = "MESH_FACE";
	const char tok_mesh_facenormal[]    = "MESH_FACENORMAL";
	const char tok_mesh_vertexnormal[]  = "MESH_VERTEXNORMAL";
	const char tok_mesh_facetangent[]   = "MESH_FACETANGENT";
	const char tok_mesh_vertextangent[] = "MESH_VERTEXTANGENT";
	const char tok_mesh_vertexbnormal[] = "MESH_VERTEXBNORMAL";
	const char tok_mesh_tface[]         = "MESH_TFACE";    
	const char tok_mesh_facemaplist[]   = "MESH_FACEMAPLIST";
	const char tok_mesh_facemap[]       = "MESH_FACEMAP";
	const char tok_mesh_facemapvert[]   = "MESH_FACEMAPVERT";
	const char tok_mesh_animation[]     = "MESH_ANIMATION";
	const char tok_mesh_cvertlist[]     = "MESH_CVERTLIST";
	const char tok_mesh_vertcol[]       = "MESH_VERTCOL";
	const char tok_mesh_numcvvertex[]   = "MESH_NUMCVERTEX";
	const char tok_mesh_numcvfaces[]    = "MESH_NUMCVFACES";
	const char tok_mesh_cfacelist[]     = "MESH_CFACELIST";
	const char tok_mesh_cface[]         = "MESH_CFACE";
	const char tok_mesh_vertexbone[]	= "MESH_VERTEXBONE";
	const char tok_mesh_vertexbone_num[]= "MESH_VERTEXBONE_NUM";

	const char tok_mesh_skeleton[]		= "MESH_SKELETON";
	const char tok_skeleton_bonenum[]	= "SKELETON_BONE_NUM";
	const char tok_skeleton_bone[]		= "SKELETON_BONE";

	// custom tokens to export patch data
	const char tok_patchmesh[]              = "PATCHMESH";
	const char tok_patchmesh_numverts[]     = "PATCHMESH_NUMVERTS";
	const char tok_patchmesh_numvecs[]      = "PATCHMESH_NUMVECS";
	const char tok_patchmesh_numpatches[]   = "PATCHMESH_NUMPATCHES";
	const char tok_patchmesh_numedges[]     = "PATCHMESH_NUMEDGES";
	const char tok_patchmesh_vertex_list[]  = "PATCHMESH_VERTEXLIST";
	const char tok_patchmesh_vertex[]       = "PATCHMESH_VERTEX";
	const char tok_patchmesh_edge_list[]    = "PATCHMESH_EDGE_LIST";
	const char tok_patchmesh_edge[]         = "PATCHMESH_EDGE";
	const char tok_patchmesh_vector_list[]  = "PATCHMESH_VECTOR_LIST";
	const char tok_patchmesh_vector[]       = "PATCHMESH_VECTOR";

	const char tok_patchmesh_patch_list[]   = "PATCHMESH_PATCH_LIST";
	const char tok_patchmesh_patch[]        = "PATCHMESH_PATCH";

	const char tok_patchmesh_tvchannel_list[] = "PATCHMESH_TVCHANNEL_LIST";
	const char tok_patchmesh_numtvchannels[] = "PATCHMESH_NUMTVCHANNELS";
	const char tok_patchmesh_tvchannel[]    = "PATCHMESH_TVCHANNEL";

	const char tok_patchmesh_tvvert_list[]  = "PATCHMESH_TVVERTLIST";
	const char tok_patchmesh_numtvverts[]   = "PATCHMESH_NUMTVVERTS";
	const char tok_patchmesh_tvvert[]       = "PATCHMESH_TVVERT";
	const char tok_patchmesh_tvpatch_list[] = "PATCHMESH_TVPATCH_TVPATCH_LIST";
	const char tok_patchmesh_tvpatch[]      = "PATCHMESH_TVPATCH";


	// Custom tokens to export Physique data
	const char tok_mesh_vertex_weightlist[] = "VERTEX_WEIGHTLIST";
	const char tok_mesh_vertex_weight[] = "VERTEX_WEIGHT";

	const char tok_control_pos_track[]  = "CONTROL_POS_TRACK";    
	const char tok_control_rot_track[]  = "CONTROL_ROT_TRACK";    
	const char tok_control_scale_track[] = "CONTROL_SCALE_TRACK";    
	const char tok_control_pos_sample[] = "CONTROL_POS_SAMPLE";
	const char tok_control_rot_sample[] = "CONTROL_ROT_SAMPLE";

	const char tok_scene_tickperfrm[]		= "SCENE_TICKSPERFRAME";
	const char tok_scene_firstfrm[]			= "SCENE_FIRSTFRAME";
	const char tok_scene_lastfrm[]			= "SCENE_LASTFRAME";
	const char tok_scene_framespeed[]		= "SCENE_FRAMESPEED";

	const char tok_material_ref[]       = "MATERIAL_REF";
	const char tok_material_list[]      = "MATERIAL_LIST";
	const char tok_material_count[]     = "MATERIAL_COUNT";
	const char tok_material[]           = "MATERIAL";
	const char tok_material_name[]      = "MATERIAL_NAME";
	const char tok_material_class[]     = "MATERIAL_CLASS";
	const char tok_material_ambient[]   = "MATERIAL_AMBIENT";
	const char tok_material_diffuse[]   = "MATERIAL_DIFFUSE";
	const char tok_material_specular[]  = "MATERIAL_SPECULAR";
	const char tok_material_shine[]     = "MATERIAL_SHINE";
	const char tok_material_shinestrength[] = "MATERIAL_SHINESTRENGTH";
	const char tok_material_transparency[] = "MATERIAL_TRANSPARENCY";
	const char tok_material_wiresize[]  = "MATERIAL_WIRESIZE";
	const char tok_material_shading[]   = "MATERIAL_SHADING";
	const char tok_material_xp_falloff[] = "MATERIAL_XP_FALLOFF";
	const char tok_material_selfillum[] = "MATERIAL_SELFILLUM";
	const char tok_material_twosided[]  = "MATERIAL_TWOSIDED";
	const char tok_material_falloff[]   = "MATERIAL_FALLOFF";
	const char tok_material_xp_type[]   = "MATERIAL_XP_TYPE";
	const char tok_submaterial[]        = "SUBMATERIAL";

	const char tok_map_generic[]        = "MAP_GENERIC";
	const char tok_map_ambient[]        = "MAP_AMBIENT";
	const char tok_map_diffuse[]        = "MAP_DIFFUSE";
	const char tok_map_specular[]       = "MAP_SPECULAR";
	const char tok_map_shine[]          = "MAP_SHINE";
	const char tok_map_shinestrength[]  = "MAP_SHINESTRENGTH";
	const char tok_map_selfillum[]      = "MAP_SELFILLUM";
	const char tok_map_opacity[]        = "MAP_OPACITY";
	const char tok_map_filtercolor[]    = "MAP_FILTERCOLOR";
	const char tok_map_bump[]           = "MAP_BUMP";
	const char tok_map_reflect[]        = "MAP_REFLECT";
	const char tok_map_refract[]        = "MAP_REFRACT";

	const char tok_map_name[]           = "MAP_NAME";
	const char tok_map_class[]          = "MAP_CLASS";
	const char tok_map_subno[]          = "MAP_SUBNO";
	const char tok_map_amount[]         = "MAP_AMOUNT";
	const char tok_map_type[]           = "MAP_TYPE";

	const char tok_map_bitmap[]         = "BITMAP";

	const char tok_map_u_offset[]       = "UVW_U_OFFSET";
	const char tok_map_v_offset[]       = "UVW_V_OFFSET";
	const char tok_map_u_tiling[]       = "UVW_U_TILING";
	const char tok_map_v_tiling[]       = "UVW_V_TILING";
	const char tok_map_angle[]          = "UVW_ANGLE";
	const char tok_map_blur[]           = "UVW_BLUR";
	const char tok_map_blur_offset[]    = "UVW_BLUR_OFFSET";
	const char tok_map_noise_amt[]      = "UVW_NOUSE_AMT"; // This mispelled token is valid!
	const char tok_map_noise_size[]     = "UVW_NOISE_SIZE";
	const char tok_map_noise_level[]    = "UVW_NOISE_LEVEL";
	const char tok_map_noise_phase[]    = "UVW_NOISE_PHASE";
	const char tok_map_bitmap_filter[]  = "BITMAP_FILTER";
	const char tok_map_class_bitmap[]   = "\"Bitmap\"";
	const char tok_map_class_noise[]    = "\"Noise\"";
	const char tok_map_class_mask[]     = "\"Mask\"";

	const char tok_jpeg_ext0[]           = "jpeg";
	const char tok_jpeg_ext1[]           = "jpg";
	const char tok_targa_ext[]           = "tga";

	const char tok_mesh_mappingchannel[] ="MESH_MAPPINGCHANNEL";
	const char tok_mesh_vertexbones[]	= "MESH_VERTEXBONES";

	// function decl
	bool ase_gettoken(const char * stream, char * token);
	int ase_parse(const char * stream, model * m);
	int ase_geomobject(const char * stream, model * m);
	int ase_passthru(const char * stream, model * m);
	int ase_tmobject(const char * stream, model * m, geomobj * geom);

	int ase_patchmesh(const char * stream, geomobj * geom);
	int ase_patchmesh_vertex_list(const char * stream, geomobj * geom);
	int ase_patchmesh_vector_list(const char * stream, geomobj * geom);
	int ase_patchmesh_edge_list(const char * stream, geomobj * geom);
	int ase_patchmesh_patch_list(const char * stream, geomobj * geom);
	int ase_patchmesh_tvchannel_list(const char * stream, geomobj * geom);
	int ase_patchmesh_tvchannel(const char * stream, geomobj * geom, int tvchannel_idx);
	int ase_patchmesh_tvpatch_list(const char * stream, geomobj * geom, int tvchannel_idx);
	int ase_patchmesh_tvvert_list(const char * stream, geomobj * geom, int tvchannel_idx);

	int ase_mesh(const char * stream, geomobj * geom);
	int ase_tm_animation(const char * stream, geomobj * geom);
	int ase_rottrack(const char * stream,animdata * anim);
	int ase_postrack(const char * stream,animdata * anim);
	int ase_vertex_weight(const char * stream, model * m, geomobj * geom);
	int ase_texturevertexlist(const char * stream, geomobj * geom);
	int ase_vertexlist(const char * stream, geomobj * geom);
	int ase_facelist(const char * stream, geomobj * geom);
	int ase_facemaplist(const char * stream, geomobj * geom);
	int ase_facemapvert(const char * stream, geomobj * geom, int index);
	int ase_normals(const char * stream, geomobj * geom);
	int ase_tangents(const char * stream, geomobj * geom);
	int ase_texturefacelist(const char * stream, geomobj * geom);
	int ase_texturevertexlist_idx(const char * stream, geomobj * geom,int tc_idx,unsigned int number_of_elem); // ???

	int ase_skip(const char * stream);
	int ase_material_list(const char * stream, model * m);
	int ase_material(const char * stream, matobj * mat);
	int ase_map(const char * stream, mapobj * map);

	int ase_mapchannel(const char* stream,geomobj* geom,int mapIdx);
	int ase_vertexbones(const char* stream, geomobj* geom);
	int ase_skeleton_bones(const char* stream, geomobj* geom);

	int ase_scene(const char* stream,model*m);
	//	void load_mat(model * m, matobj * mat);
	//	void load_map(model * m, map_array * marray);
	//	int load_map_file(const char * filename, texdata * txdata);

	void convert_world_to_local(model * m);
	void compute_local_tm(geomobj * geom);

	// implementation
	texdata::~texdata()
	{
		if (pixels)
		{
			delete [] pixels;
			pixels = NULL;
		}

		if (name)
			delete [] name;
		name = 0;
	}

	animdata::~animdata()
	{
		if (numpos)
		{
			delete [] pos;
			numpos = 0;
		}

		if (numrot)
		{
			delete [] rot;
			numrot = 0;
		}

		if(posframetime)
		{
			delete [] posframetime;
			posframetime = 0;
		}

		if(rotframetime)
		{
			delete [] rotframetime;
			rotframetime = 0;
		}
	}

	mapobj::~mapobj()
	{
		map_it it = map_generic.begin();
		while (it != map_generic.end())
		{
			delete *it;
			++it;
		}
		if (name)
			delete [] name;
		name = 0;
		if (classname)
			delete [] classname;
		classname = 0;
		if (bitmap)
			delete [] bitmap;
		bitmap = 0;
		if (map_type)
			delete [] map_type;
		map_type = 0;
		if (bitmap_filter)
			delete [] bitmap_filter;
		bitmap_filter = 0;
	}

	matobj::~matobj()
	{
		map_it it = map_generic.begin();
		while (it != map_generic.end())
		{
			delete *it;
			++it;
		}

		it = map_diffuse.begin();
		while (it != map_diffuse.end())
		{
			delete *it;
			++it;
		}

		it = map_specular.begin();
		while (it != map_specular.end())
		{
			delete *it;
			++it;
		}

		it = map_bump.begin();
		while (it != map_bump.end())
		{
			delete *it;
			++it;
		}

		it = map_selfillum.begin();
		while (it != map_selfillum.end())
		{
			delete *it;
			++it;
		}

		it = map_reflect.begin();
		while (it != map_reflect.end())
		{
			delete *it;
			++it;
		}

		it = map_shine.begin();
		while (it != map_shine.end())
		{
			delete *it;
			++it;
		}

		it = map_shinestrength.begin();
		while (it != map_shinestrength.end())
		{
			delete *it;
			++it;
		}

		it = map_opacity.begin();
		while (it != map_opacity.end())
		{
			delete *it;
			++it;
		}

		it = map_refract.begin();
		while (it != map_refract.end())
		{
			delete *it;
			++it;
		}

		it = map_ambient.begin();
		while (it != map_ambient.end())
		{
			delete *it;
			++it;
		}

		it = map_filtercolor.begin();
		while (it != map_filtercolor.end())
		{
			delete *it;
			++it;
		}

		mat_it mit = submat.begin();
		while (mit != submat.end())
		{
			delete *mit;
			++mit;
		}
		if (name)
			delete [] name;
		name = 0;
		if (classname)
			delete [] classname;
		classname = 0;
		if (shader)
			delete [] shader;
		shader = 0;
		if (xp_type)
			delete [] xp_type;
		xp_type = 0;
	}


	geomobj::geomobj() : name(0), parent(0), numn(0), numt(0), numv(0), numc(0), fmapv(0), numf(0), numtf(0), anim(0), matidx(-1), numbv(0), 
		vbones(0), numvbone(0) , skelbones(0), numskelbone(0),tn(0),numtn(0),bn(0),numbn(0),ftn(0),fbn(0)
	{   
		tm[0] = 1.0f;tm[1] = 0.0f;tm[2] = 0.0f;tm[3] = 0.0f;
		tm[4] = 0.0f;tm[5] = 1.0f;tm[6] = 0.0f;tm[7] = 0.0f;
		tm[8] = 0.0f;tm[9] = 0.0f;tm[10] = 1.0f;tm[11] = 0.0f;
		tm[12] = 0.0f;tm[13] = 0.0f;tm[14] = 0.0f;tm[15] = 1.0f;
	}

	geomobj::~geomobj() 
	{
		if (numv)
		{
			delete [] v;
			numv = 0;
		}

		if (numt)
		{
			delete [] t;
			numt = 0;
		}

		if (numf)
		{
			delete [] f;
			delete [] smg;
			delete [] fsubmat;
			numf = 0;
		}

		if (numn)
		{
			delete [] fn;
			delete [] fvn;
			delete [] n;
			numn = 0;
		}

		if (numc)
		{
			delete [] c;
			numc = 0;
		}

		if (numtf)
		{
			delete [] tf;
			numtf = 0;
		}

		if (fmapv)
		{
			delete [] fmapv;
			fmapv = NULL;
		}

		if (anim)
		{
			delete anim;
			anim = NULL;
		}

		if (numbv)
		{
			delete [] bv;
			delete [] bmatref;
			delete [] vbv;
			numbv = 0;
		}
		if (name)
			delete [] name;
		name = 0;

		if (vbones)
		{
			delete [] vbones;
			vbones = 0;
			numvbone = 0;
		}

		if(skelbones)
		{
			delete [] skelbones;
			skelbones = 0;
			numskelbone = 0;
		}

		if(tn)
		{
			delete [] tn;
			tn = 0;
			numtn = 0;
		}

		if(bn)
		{
			delete [] bn;
			bn = 0;
			numbn = 0;
		}

		if(ftn)
			delete [] ftn;

		if(fbn)
			delete [] fbn;
	}

	model::~model()
	{
		geom_it git = geom.begin();
		while (git != geom.end())
		{
			delete *git;
			++git;
		}

		mat_it mit = mat.begin();
		while (mit != mat.end())
		{
			delete *mit;
			++mit;
		}

		tex_it tit = tex.begin();
		while (tit != tex.end())
		{
			delete (*tit).second;
			++tit;
		}
		if (name)
			delete [] name;
		name = 0;
	};

	model * load(const char * buf, unsigned int bufsize, float scale)
	{
		int result(0);
		g_stream_max_cnt = bufsize;
		g_stream_cnt = 0;
		g_scale_hack = scale;
		model * m = new model;

		result = ase_parse(buf,m);

		if (result == 0)
		{
			geom_it rit;
			geom_it git = m->geom.begin();
			while (git != m->geom.end())
			{
				// In case we have a group as parent
				if ((*git)->parent == NULL)
				{
					// Lets amke sure it hasn't already been added 
					bool found = false;
					rit = m->root_geom.begin();
					while (rit != m->root_geom.end() && found)
					{
						if (!strcmp((*rit)->name, (*git)->name))
							found = true;
						++rit;
					}
					if (found == false)
						m->root_geom.push_back(*git);
				}
				++git;
			}

			convert_world_to_local(m);

			rit = m->root_geom.begin();
			while(rit != m->root_geom.end())
			{
				compute_local_tm(*rit);
				++rit;
			}      
		}
		else
		{
			delete m;
			m = NULL;
		}

		return m;
	}

	void compute_local_tm(geomobj * geom)
	{
		if (geom->parent)
		{
			asemath::matrix4f matparent(geom->parent->tm);
			matparent = matparent.inverse();
			asemath::matrix4f mat(geom->tm);
			mat = matparent * mat;
			memcpy(geom->rtm,mat.get_value(),sizeof(float) * 16);
		}

		geom_it it = geom->children.begin();
		while (it != geom->children.end())
		{
			compute_local_tm(*it);
			++it;
		}
	}

	void convert_world_to_local(model * m)
	{
		unsigned int i;
		int i_times_3;
		geomobj * geom = NULL;
		geom_it it = m->geom.begin();
		while (it != m->geom.end())
		{
			geom = *it;
			asemath::matrix4f mat(geom->tm);
			asemath::matrix4f invmat = mat.inverse();
			for (i = 0; i < geom->numv; ++i)
			{
				i_times_3 = i * 3;
				asemath::vec3f vec(&geom->v[i_times_3]);
				invmat.mult_matrix_vec(vec);
				geom->v[i_times_3] = vec[0];
				geom->v[i_times_3 + 1] = vec[1];
				geom->v[i_times_3 + 2] = vec[2];
			}
			// According to the MAX SDK Doc, normals are in object space
			/*
			for (i = 0; i < geom->numn; ++i)
			{
			i_times_3 = i * 3;
			asemath::vec3f vec(&geom->n[i_times_3]);
			invmat.mult_matrix_dir(vec);
			geom->n[i_times_3] = vec[0];
			geom->n[i_times_3 + 1] = vec[1];
			geom->n[i_times_3 + 2] = vec[2];
			}
			*/
			++it;
		}
	}

	int ase_parse(const char * stream, model * m)
	{
		int  result = 0;

		while (ase_gettoken(stream, g_token))
		{
			sscanf(g_token, "%s %s",g_keyword, g_leftover);
			if (!strcmp(g_keyword,tok_group))
			{
				result |= ase_passthru(stream,m);
			}
			else if (!strcmp(g_keyword,tok_geomobject))
			{
				result |= ase_geomobject(stream,m);
			}
			else if (!strcmp(g_keyword,tok_helperobject))
			{
				result |= ase_geomobject(stream,m);
			}
			else if (!strcmp(g_keyword,tok_material_list))
			{
				result |= ase_material_list(stream,m);
			}
			else if (!strcmp(g_keyword,tok_scene))
			{
				result |= ase_scene(stream,m);
			}
		}
		return result;
	}

	bool ase_gettoken(const char * stream, char * token)
	{
		int cnt;

		while ( g_stream_cnt < g_stream_max_cnt)
			if ((!isprint(stream[g_stream_cnt])) || stream[g_stream_cnt] == ' ') {
				g_stream_cnt++;
			} else {
				break;
			}
			if (g_stream_cnt<g_stream_max_cnt) {
				switch(stream[g_stream_cnt]) 
				{
				case '*':
					g_stream_cnt++;
					cnt = g_stream_cnt;
					while (stream[cnt] != '\n' && cnt < g_stream_max_cnt)
						cnt++;

					memcpy(token,stream + g_stream_cnt, cnt - g_stream_cnt);
					token[ cnt - g_stream_cnt - 1] = '\0';
					g_stream_cnt = cnt;
					return true;
				case '}':
					while (stream[g_stream_cnt] != '\n' && g_stream_cnt < g_stream_max_cnt)
						g_stream_cnt++;
					token[0] = '}';
					token[1] = '\0';
					return true;
				default:
					while (stream[g_stream_cnt] != '\n' && g_stream_cnt < g_stream_max_cnt)
						g_stream_cnt++;
					/* eat up rest of line */
					break;
				}
			}
			return false;
	}

	int ase_scene(const char* stream,model*m)
	{
		int result = 0;
		while (ase_gettoken(stream, g_token))
		{
			sscanf(g_token, "%s %s",g_keyword, g_leftover);
			if (!strcmp(g_keyword,tok_scene_firstfrm))
			{
				sscanf(g_leftover, "%d",&m->firstframe);
			}
			else if (!strcmp(g_keyword,tok_scene_lastfrm))
			{
				sscanf(g_leftover, "%d",&m->lastframe);
			}
			else if (!strcmp(g_keyword,tok_scene_framespeed))
			{
				sscanf(g_leftover, "%d",&m->framespeed);
			}
			else if (!strcmp(g_keyword,tok_scene_tickperfrm))
			{
				sscanf(g_leftover, "%d",&m->tickperframe);
			}

			else if (!strcmp(g_keyword,"}"))
			{

				break;
			}
		}
		return result;
	}

	int ase_material_list(const char * stream, model * m)
	{
		int result = 0;
		unsigned int num_materials = 0;

		while (ase_gettoken(stream, g_token))
		{
			sscanf(g_token, "%s %s",g_keyword, g_leftover);
			if (!strcmp(g_keyword,tok_material_count))
			{
				sscanf(g_leftover, "%d",&num_materials);
			}
			else if (!strcmp(g_keyword,tok_material))
			{
				matobj * mat = new matobj;
				result |= ase_material(stream , mat);
				m->mat.push_back(mat);
			}
			else if (!strcmp(g_keyword,"}"))
			{
				assert(m->mat.size() == num_materials);
				break;
			}
		}

		return result;
	}

	int ase_material(const char * stream, matobj * mat)
	{
		int result = 0;
		char buf[256];

		while (ase_gettoken(stream, g_token))
		{
			sscanf(g_token, "%s %s",g_keyword, g_leftover);
			if (!strcmp(g_keyword,tok_material_name))
			{
				const char * pStart = strchr(g_token, '\"');
				const char * pEnd = pStart + 1;
				const char * pit = pEnd;
				while (*pit != '\0' )
				{
					if (*pit == '"')
						pEnd = pit;
					++pit;
				}
				int size = pEnd - pStart + 1;
				if (size>256)
					fprintf (stderr,"Buffer Overrun Detected");
				memcpy(buf,pStart,size);
				buf[size] = '\0';

				mat->name = new char[strlen(buf) + 1];
				strcpy(mat->name,buf);
			}
			else if (!strcmp(g_keyword,tok_material_class))
			{
				const char * pStart = strchr(g_token, '\"');
				const char * pEnd = pStart + 1;
				const char * pit = pEnd;
				while (*pit != '\0' )
				{
					if (*pit == '"')
						pEnd = pit;
					++pit;
				}
				if (pEnd-pStart+1>256)
					fprintf (stderr,"Buffer Overrun Detected");

				memcpy(buf,pStart,pEnd - pStart + 1);
				buf[pEnd - pStart + 1] = '\0';

				mat->classname = new char[strlen(buf) + 1];
				strcpy(mat->classname,buf);
			}
			else if (!strcmp(g_keyword,tok_material_ambient))
			{
				sscanf(g_token, "%s %f %f %f",g_keyword, &mat->ambient[0],&mat->ambient[1],&mat->ambient[2]);
			}
			else if (!strcmp(g_keyword,tok_material_diffuse))
			{
				sscanf(g_token, "%s %f %f %f",g_keyword, &mat->diffuse[0],&mat->diffuse[1],&mat->diffuse[2]);
			}
			else if (!strcmp(g_keyword,tok_material_specular))
			{
				sscanf(g_token, "%s %f %f %f",g_keyword, &mat->specular[0],&mat->specular[1],&mat->specular[2]);
			}
			else if (!strcmp(g_keyword,tok_material_shine))
			{
				sscanf(g_token, "%s %f",g_keyword, &mat->shine);
			}
			else if (!strcmp(g_keyword,tok_material_shinestrength))
			{
				sscanf(g_token, "%s %f",g_keyword, &mat->shinestrength);
			}
			else if (!strcmp(g_keyword,tok_material_transparency))
			{
				sscanf(g_token, "%s %f",g_keyword, &mat->transparency);
			}
			else if (!strcmp(g_keyword,tok_material_wiresize))
			{
				sscanf(g_token, "%s %f",g_keyword, &mat->wiresize);
			}
			else if (!strcmp(g_keyword,tok_material_shading))
			{
				mat->shader = new char[strlen(g_leftover) + 1];
				strcpy(mat->shader, g_leftover);
			}
			else if (!strcmp(g_keyword,tok_material_xp_falloff))
			{
				sscanf(g_token, "%s %f",g_keyword, &mat->xp_falloff);
			}
			else if (!strcmp(g_keyword,tok_material_selfillum))
			{
				sscanf(g_token, "%s %f",g_keyword, &mat->selfillum);
			}
			else if (!strcmp(g_keyword,tok_material_twosided))
			{
				mat->twosided = true;
			}
			else if (!strcmp(g_keyword,tok_material_falloff))
			{
				if (!strcmp(g_leftover,"In"))
					mat->falloff = true; // In
				else
					mat->falloff = false; // Out
			}
			else if (!strcmp(g_keyword,tok_material_xp_type))
			{
				mat->xp_type = new char[strlen(g_leftover) + 1];
				strcpy(mat->xp_type, g_leftover);
			}
			else if (!strcmp(g_keyword,tok_map_diffuse))
			{
				mapobj * map = new mapobj;
				result |= ase_map(stream, map);
				mat->map_diffuse.push_back(map);
			}
			else if (!strcmp(g_keyword,tok_map_ambient))
			{
				mapobj * map = new mapobj;
				result |= ase_map(stream, map);
				mat->map_ambient.push_back(map);
			}
			else if (!strcmp(g_keyword,tok_map_generic))
			{
				mapobj * map = new mapobj;
				result |= ase_map(stream, map);
				mat->map_generic.push_back(map);
			}
			else if (!strcmp(g_keyword,tok_map_specular))
			{
				mapobj * map = new mapobj;
				result |= ase_map(stream, map);
				mat->map_specular.push_back(map);
			}
			else if (!strcmp(g_keyword,tok_map_shine))
			{
				mapobj * map = new mapobj;
				result |= ase_map(stream, map);
				mat->map_shine.push_back(map);
			}
			else if (!strcmp(g_keyword,tok_map_shinestrength))
			{
				mapobj * map = new mapobj;
				result |= ase_map(stream, map);
				mat->map_shinestrength.push_back(map);
			}
			else if (!strcmp(g_keyword,tok_map_bump))
			{
				mapobj * map = new mapobj;
				result |= ase_map(stream, map);
				mat->map_bump.push_back(map);
			}
			else if (!strcmp(g_keyword,tok_map_selfillum))
			{
				mapobj * map = new mapobj;
				result |= ase_map(stream, map);
				mat->map_selfillum.push_back(map);
			}
			else if (!strcmp(g_keyword,tok_map_reflect))
			{
				mapobj * map = new mapobj;
				result |= ase_map(stream, map);
				mat->map_reflect.push_back(map);
			}
			else if (!strcmp(g_keyword,tok_map_refract))
			{
				mapobj * map = new mapobj;
				result |= ase_map(stream, map);
				mat->map_refract.push_back(map);
			}
			else if (!strcmp(g_keyword,tok_map_filtercolor))
			{
				mapobj * map = new mapobj;
				result |= ase_map(stream, map);
				mat->map_filtercolor.push_back(map);
			}
			else if (!strcmp(g_keyword,tok_map_opacity))
			{
				mapobj * map = new mapobj;
				result |= ase_map(stream, map);
				mat->map_opacity.push_back(map);
			}
			else if (!strcmp(g_keyword,tok_submaterial))
			{
				matobj * submat = new matobj;
				result |= ase_material(stream, submat);
				mat->submat.push_back(submat);
			}
			else if (!strcmp(g_keyword,"}"))
			{
				mat->diffuse[3] = 1.0f - mat->transparency;
				mat->specular[3] = 1.0;// - mat->transparency;
				break;
			}
		}
		return result;
	}

	int ase_map(const char * stream, mapobj * map)
	{
		char buf[256];
		int result = 0;

		while (ase_gettoken(stream, g_token))
		{
			sscanf(g_token, "%s %s",g_keyword, g_leftover);
			if (!strcmp(g_keyword,tok_map_name))
			{
				const char * pStart = strchr(g_token, '\"');
				const char * pEnd = pStart + 1;
				const char * pit = pEnd;
				while (*pit != '\0' )
				{
					if (*pit == '"')
						pEnd = pit;
					++pit;
				}
				if (pEnd-pStart+1>256)
					fprintf (stderr,"Buffer Overrun Detected");

				memcpy(buf,pStart,pEnd - pStart + 1);
				buf[pEnd - pStart + 1] = '\0';

				map->name = new char[strlen(buf) + 1];
				strcpy(map->name,buf);

			}
			else if (!strcmp(g_keyword,tok_map_class))
			{
				const char * pStart = strchr(g_token, '\"');
				const char * pEnd = pStart + 1;
				const char * pit = pEnd;
				while (*pit != '\0' )
				{
					if (*pit == '"')
						pEnd = pit;
					++pit;
				}
				if (pEnd-pStart+1>256)
					fprintf (stderr,"Buffer Overrun Detected");

				memcpy(buf,pStart,pEnd - pStart + 1);
				buf[pEnd - pStart + 1] = '\0';

				map->classname = new char[strlen(buf) + 1];
				strcpy(map->classname,buf);
			}
			else if (!strcmp(g_keyword,tok_map_subno))
			{
				sscanf(g_token, "%s %d",g_keyword, &map->subno);
			}
			else if (!strcmp(g_keyword,tok_map_amount))
			{
				sscanf(g_token, "%s %f",g_keyword, &map->amount);
			}
			else if (!strcmp(g_keyword,tok_map_bitmap))
			{
				char * pbuf = &g_leftover[1];
				g_leftover[strlen(g_leftover) - 1] = '\0';
				map->bitmap = new char[strlen(pbuf)+1];
				strcpy(map->bitmap,pbuf);
			}
			else if (!strcmp(g_keyword,tok_map_type))
			{
				map->map_type = new char[strlen(g_leftover) + 1];
				strcpy(map->map_type, g_leftover);
			}
			else if (!strcmp(g_keyword,tok_map_u_offset))
			{
				sscanf(g_token, "%s %f",g_keyword, &map->u_offset);
			}
			else if (!strcmp(g_keyword,tok_map_v_offset))
			{
				sscanf(g_token, "%s %f",g_keyword, &map->v_offset);
			}
			else if (!strcmp(g_keyword,tok_map_u_tiling))
			{
				sscanf(g_token, "%s %f",g_keyword, &map->u_tiling);
			}
			else if (!strcmp(g_keyword,tok_map_v_tiling))
			{
				sscanf(g_token, "%s %f",g_keyword, &map->v_tiling);
			}
			else if (!strcmp(g_keyword,tok_map_angle))
			{
				sscanf(g_token, "%s %f",g_keyword, &map->angle);
			}
			else if (!strcmp(g_keyword,tok_map_blur))
			{
				sscanf(g_token, "%s %f",g_keyword, &map->blur);
			}
			else if (!strcmp(g_keyword,tok_map_blur_offset))
			{
				sscanf(g_token, "%s %f",g_keyword, &map->blur_offset);
			}
			else if (!strcmp(g_keyword,tok_map_noise_amt))
			{
				sscanf(g_token, "%s %f",g_keyword, &map->noise_amt);
			}
			else if (!strcmp(g_keyword,tok_map_noise_size))
			{
				sscanf(g_token, "%s %f",g_keyword, &map->noise_size);
			}
			else if (!strcmp(g_keyword,tok_map_noise_level))
			{
				sscanf(g_token, "%s %d",g_keyword, &map->noise_level);
			}
			else if (!strcmp(g_keyword,tok_map_noise_phase))
			{
				sscanf(g_token, "%s %f",g_keyword, &map->noise_phase);
			}
			else if (!strcmp(g_keyword,tok_map_bitmap_filter))
			{
				map->bitmap_filter = new char[strlen(g_leftover) + 1];
				strcpy(map->bitmap_filter, g_leftover);
			}
			else if (!strcmp(g_keyword,tok_map_generic))
			{
				mapobj * submap = new mapobj;
				result |= ase_map(stream,submap);
				map->map_generic.push_back(submap);
			}
			else if (!strcmp(g_keyword,tok_close))
			{
				break;
			}
		}
		return result;
	}

	geomobj * get_geomobj(model * m, const char * name)
	{
		geomobj * geom = NULL;
		geom_it it = m->geom.begin();
		while (geom == NULL && it != m->geom.end())
		{
			if (!strcmp((*it)->name,name))
				geom = *it;
			++it;
		}
		return geom;
	}

	int ase_geomobject(const char * stream, model * m)
	{
		char buf[256];
		int result = 0;

		geomobj * parent = NULL;
		geomobj * geom = NULL;

		while (ase_gettoken(stream, g_token))
		{
			sscanf(g_token, "%s %s",g_keyword, g_leftover);
			if (!strcmp(g_keyword,tok_node_name))
			{
				const char * pStart = strchr(g_token, '\"') + 1;
				const char * pEnd = strrchr(g_token, '\"');
				if (pEnd-pStart+1>256)
					fprintf (stderr,"Buffer Overrun Detected");

				memcpy(buf,pStart,pEnd - pStart);
				buf[pEnd - pStart] = '\0';

				geom = get_geomobj(m,buf);
				if (geom == NULL)
				{
					geom = new geomobj;
					geom->name = new char[strlen(buf) + 1];
					strcpy(geom->name,buf);
				}
			}
			else if (!strcmp(g_keyword,tok_node_parent))
			{
				const char * pStart = strchr(g_token, '\"') + 1;
				const char * pEnd = strrchr(g_token, '\"');
				if (pEnd-pStart+1>256)
					fprintf (stderr,"Buffer Overrun Detected");

				memcpy(buf,pStart,pEnd - pStart);
				buf[pEnd - pStart] = '\0';

				parent = get_geomobj(m,buf);
				if (parent == NULL)
				{
					parent = new geomobj;
					parent->name = new char[strlen(buf) + 1];
					strcpy(parent->name,buf);
					m->geom.push_back(parent);    
				}
				geom->parent = parent;
				parent->children.push_back(geom);
			} 
			else if (!strcmp(g_keyword,tok_node_tm))
			{
				assert(geom);
				result |= ase_tmobject(stream,m,geom);
			}
			else if (!strcmp(g_keyword,tok_mesh))
			{
				assert(geom);
				result |= ase_mesh(stream, geom);
			}
			else if (!strcmp(g_keyword,tok_patchmesh))
			{
				assert(geom);
				result |= ase_patchmesh(stream, geom);
			}
			else if (!strcmp(g_keyword,tok_tm_animation))
			{
				assert(geom);
				result |= ase_tm_animation(stream, geom);
			}
			else if (!strcmp(g_keyword,tok_mesh_vertex_weightlist))
			{
				assert(geom);
				result |= ase_vertex_weight(stream,m,geom);
			}
			else if (!strcmp(g_keyword,tok_material_ref))
			{
				assert(geom);
				sscanf(g_leftover,"%d",&geom->matidx );
			}
			else if (!strcmp(g_keyword,tok_close))
			{
				assert(geom);
				m->geom.push_back(geom);

				if (parent == NULL)
					m->root_geom.push_back(geom);

				break;
			}
		}
		return result;
	}

	int ase_tmobject(const char * stream, model * m, geomobj * geom)
	{
		int result = 0;

		// We transpose while loading the row major to get a column major
		while (ase_gettoken(stream, g_token))
		{
			sscanf(g_token, "%s %s",g_keyword, g_leftover);
			/*
			if (!strcmp(g_keyword,tok_node_name))
			{
			const char * pStart = strchr(token, '\"');
			} 
			else */
			if (!strcmp(g_keyword,tok_tm_row0))
			{
				sscanf(g_token, "%s %f %f %f",g_keyword, &geom->tm[0], &geom->tm[1], &geom->tm[2]);
			}
			else if (!strcmp(g_keyword,tok_tm_row1))
			{
				sscanf(g_token, "%s %f %f %f",g_keyword, &geom->tm[4], &geom->tm[5], &geom->tm[6]);
			}
			else if (!strcmp(g_keyword,tok_tm_row2))
			{
				sscanf(g_token, "%s %f %f %f",g_keyword, &geom->tm[8], &geom->tm[9], &geom->tm[10]);
			}
			else if (!strcmp(g_keyword,tok_tm_row3))
			{
				sscanf(g_token, "%s %f %f %f",g_keyword, &geom->tm[12], &geom->tm[13], &geom->tm[14]);
				geom->tm[12] *= g_scale_hack;
				geom->tm[13] *= g_scale_hack;
				geom->tm[14] *= g_scale_hack;
			}
			else if(!strcmp(g_keyword,tok_pos))
			{
				sscanf(g_token, "%s %f %f %f",g_keyword, &geom->pos[0], &geom->pos[1], &geom->pos[2]);
			}
			else if(!strcmp(g_keyword,tok_rotaxis))
			{
				float x,y,z;
				sscanf(g_token, "%s %f %f %f",g_keyword, &x, &y, &z);
				geom->rot[0] = x;
				geom->rot[1] = y;
				geom->rot[2] = z;

			}
			else if(!strcmp(g_keyword,tok_rotangle))
			{
				sscanf(g_token, "%s %f",g_keyword, &geom->rot[3]);
			}
			else if (!strcmp(g_keyword,tok_close))
			{
				geom->tm[3] = 0.0f;
				geom->tm[7] = 0.0f;
				geom->tm[11] = 0.0f;
				geom->tm[15] = 1.0f;

				// store the inverse...Assume this is a rigid body tm
				asemath::matrix4f mat(geom->tm);
				asemath::matrix4f invmat = mat.inverse();
				memcpy(geom->bone_offset_tm,invmat.get_value(),sizeof(float) * 16);
				break;
			}
		}
		return result;
	}

	int ase_patchmesh(const char * stream, geomobj * geom)
	{
		int result = 0;

		geom->type = geomobj::patched;
		while (ase_gettoken(stream, g_token))
		{
			sscanf(g_token, "%s %s",g_keyword, g_leftover);
			// control vertices
			if (!strcmp(g_keyword,tok_patchmesh_vertex_list))
			{
				result |= ase_patchmesh_vertex_list(stream,geom);
			} 
			else if (!strcmp(g_keyword,tok_patchmesh_vector_list))
			{
				result |= ase_patchmesh_vector_list(stream,geom);
			} 
			else if (!strcmp(g_keyword,tok_patchmesh_edge_list))
			{
				result |= ase_patchmesh_edge_list(stream,geom);
			} 
			else if (!strcmp(g_keyword,tok_patchmesh_patch_list))
			{
				result |= ase_patchmesh_patch_list(stream,geom);
			} 
			else if (!strcmp(g_keyword,tok_patchmesh_tvchannel_list))
			{
				result |= ase_patchmesh_tvchannel_list(stream,geom);
			} 
			else if (!strcmp(g_keyword,tok_close))
			{
				break;
			}
		}
		return result;
	}

	int ase_patchmesh_vertex_list(const char * stream, geomobj * geom)
	{
		int result = 0;
		int idx, idx_times_3;
		float x, y, z;

		while (ase_gettoken(stream, g_token))
		{
			sscanf(g_token, "%s %s",g_keyword, g_leftover);

			if (!strcmp(g_keyword,tok_patchmesh_vertex))
			{
				sscanf(g_token, "%s %d %f %f %f",g_keyword, &idx, &x, &y, &z);
				idx_times_3 = idx * 3;
				geom->verts[idx_times_3] = x * g_scale_hack;
				geom->verts[idx_times_3 + 1] = y * g_scale_hack;
				geom->verts[idx_times_3 + 2] = z * g_scale_hack;
			}
			else if (!strcmp(g_keyword,tok_patchmesh_numverts))
			{
				sscanf(g_token, "%s %d",g_keyword, &geom->numverts);
				geom->verts = new float[geom->numverts * 3];
			} 
			else if (!strcmp(g_keyword,tok_close))
			{
				break;
			}
		}
		return result;
	}

	int ase_patchmesh_vector_list(const char * stream, geomobj * geom)
	{
		int result = 0;
		int idx, idx_times_3;
		float x, y, z;

		while (ase_gettoken(stream, g_token))
		{
			sscanf(g_token, "%s %s",g_keyword, g_leftover);

			if (!strcmp(g_keyword,tok_patchmesh_vector))
			{
				sscanf(g_token, "%s %d %f %f %f",g_keyword, &idx, &x, &y, &z);
				idx_times_3 = idx * 3;
				geom->vecs[idx_times_3] = x * g_scale_hack;
				geom->vecs[idx_times_3 + 1] = y * g_scale_hack;
				geom->vecs[idx_times_3 + 2] = z * g_scale_hack;
			}
			else if (!strcmp(g_keyword,tok_patchmesh_numvecs))
			{
				sscanf(g_token, "%s %d",g_keyword, &geom->numvecs);
				geom->vecs = new float[geom->numvecs * 3];
			} 
			else if (!strcmp(g_keyword,tok_close))
			{
				break;
			}
		}
		return result;
	}

	int ase_patchmesh_edge_list(const char * stream, geomobj * geom)
	{
		int result = 0;
		int idx, idx_times_4;
		int i0, i1, i2, i3;

		while (ase_gettoken(stream, g_token))
		{
			sscanf(g_token, "%s %s",g_keyword, g_leftover);

			if (!strcmp(g_keyword,tok_patchmesh_edge))
			{
				sscanf(g_token, "%s %d %d %d %d %d",g_keyword, &idx, &i0, &i1, &i2, &i3);
				idx_times_4 = idx * 4;
				geom->edges[idx_times_4] = i0;
				geom->edges[idx_times_4 + 1] = i1;
				geom->edges[idx_times_4 + 2] = i2;
				geom->edges[idx_times_4 + 3] = i3;
			}
			else if (!strcmp(g_keyword,tok_patchmesh_numedges))
			{
				sscanf(g_token, "%s %d",g_keyword, &geom->numedges);
				geom->edges = new int[geom->numedges * 4];
			} 
			else if (!strcmp(g_keyword,tok_close))
			{
				break;
			}
		}
		return result;
	}

	int ase_patchmesh_patch_list(const char * stream, geomobj * geom)
	{
		int result = 0;
		int idx;
		int e0, e1, e2, e3;
		int i0, i1, i2, i3;
		int smg,numedges;
		int mtlid;

		while (ase_gettoken(stream, g_token))
		{
			sscanf(g_token, "%s %s",g_keyword, g_leftover);

			if (!strcmp(g_keyword,tok_patchmesh_patch))
			{
				sscanf(g_token, "%s %d %d %d %d %d %d %d %d %d %d, %d, %d",g_keyword, &idx, &numedges, &e0, &e1, &e2, &e3, &i0, &i1, &i2, &i3, &smg, &mtlid);
				geom->patches[idx].numedges = numedges;
				geom->patches[idx].edge[0] = e0;
				geom->patches[idx].edge[1] = e1;
				geom->patches[idx].edge[2] = e2;
				geom->patches[idx].edge[3] = e3;
				geom->patches[idx].interior[0] = i0;
				geom->patches[idx].interior[1] = i1;
				geom->patches[idx].interior[2] = i2;
				geom->patches[idx].interior[3] = i3;
				geom->patches[idx].smg = smg;
				geom->patches[idx].mtlid = mtlid;
			}
			else if (!strcmp(g_keyword,tok_patchmesh_numpatches))
			{
				sscanf(g_token, "%s %d",g_keyword, &geom->numpatches);
				geom->patches = new patch[geom->numpatches];
			} 
			else if (!strcmp(g_keyword,tok_close))
			{
				break;
			}
		}
		return result;
	}

	int ase_patchmesh_tvchannel_list(const char * stream, geomobj * geom)
	{
		int result = 0;
		int tvchannel_idx= 0;
		while (ase_gettoken(stream, g_token))
		{
			sscanf(g_token, "%s %s",g_keyword, g_leftover);

			if (!strcmp(g_keyword,tok_patchmesh_tvchannel))
			{
				result |= ase_patchmesh_tvchannel(stream, geom, tvchannel_idx);
				tvchannel_idx++;
			}
			else if (!strcmp(g_keyword,tok_patchmesh_numtvchannels))
			{
				sscanf(g_token, "%s %d",g_keyword, &geom->numtvchannels);

				geom->numtvverts = new int[geom->numtvchannels];
				geom->tvverts = new float*[geom->numtvchannels];
				geom->tvpatches = new int*[geom->numtvchannels];
			} 
			else if (!strcmp(g_keyword,tok_close))
			{
				break;
			}
		}
		return result;
	}

	int ase_patchmesh_tvchannel(const char * stream, geomobj * geom, int tvchannel_idx)
	{
		int result = 0;

		while (ase_gettoken(stream, g_token))
		{
			sscanf(g_token, "%s %s",g_keyword, g_leftover);

			if (!strcmp(g_keyword,tok_patchmesh_tvvert_list))
			{
				result |= ase_patchmesh_tvvert_list(stream, geom, tvchannel_idx);
			}
			else if (!strcmp(g_keyword,tok_patchmesh_tvpatch_list))
			{
				result |= ase_patchmesh_tvpatch_list(stream, geom, tvchannel_idx);
			} 
			else if (!strcmp(g_keyword,tok_close))
			{
				break;
			}
		}
		return result;
	}

	int ase_patchmesh_tvvert_list(const char * stream, geomobj * geom, int tvchannel_idx)
	{
		int result = 0;
		int idx, idx_times_3;
		float u,v,w;

		while (ase_gettoken(stream, g_token))
		{
			sscanf(g_token, "%s %s",g_keyword, g_leftover);

			if (!strcmp(g_keyword,tok_patchmesh_tvvert))
			{
				sscanf(g_token, "%s %d %f %f %f",g_keyword, &idx, &u, &v, &w);
				idx_times_3 = idx * 3;
				geom->tvverts[tvchannel_idx][idx_times_3] = u;
				geom->tvverts[tvchannel_idx][idx_times_3 + 1] = -v;
				geom->tvverts[tvchannel_idx][idx_times_3 + 2] = w;
			}
			else if (!strcmp(g_keyword,tok_patchmesh_numtvverts))
			{
				sscanf(g_token, "%s %d",g_keyword, &geom->numtvverts[tvchannel_idx]);
				geom->tvverts[tvchannel_idx] = new float[geom->numtvverts[tvchannel_idx] * 3];
				if (geom->numtvverts[tvchannel_idx])
					geom->tvpatches[tvchannel_idx] = new int[geom->numtvverts[tvchannel_idx] * 4];
			}
			else if (!strcmp(g_keyword,tok_close))
			{
				break;
			}
		}
		return result;
	}

	int ase_patchmesh_tvpatch_list(const char * stream, geomobj * geom, int tvchannel_idx)
	{
		int result = 0;
		int idx, idx_times_4;
		int i0, i1, i2, i3;

		while (ase_gettoken(stream, g_token))
		{
			sscanf(g_token, "%s %s",g_keyword, g_leftover);

			if (!strcmp(g_keyword,tok_patchmesh_tvpatch))
			{
				sscanf(g_token, "%s %d %d %d %d %d",g_keyword, &idx, &i0, &i1, &i2, &i3);
				idx_times_4 = idx * 4;
				geom->tvpatches[tvchannel_idx][idx_times_4] = i0;
				geom->tvpatches[tvchannel_idx][idx_times_4 + 1] = i1;
				geom->tvpatches[tvchannel_idx][idx_times_4 + 2] = i2;
				geom->tvpatches[tvchannel_idx][idx_times_4 + 3] = i3;
			}
			else if (!strcmp(g_keyword,tok_close))
			{
				break;
			}
		}
		return result;
	}

	int ase_mesh(const char * stream, geomobj * geom)
	{
		int result = 0;

		std::vector<std::string> token_vec;

		geom->type = geomobj::polygonal;
		while (ase_gettoken(stream, g_token))
		{
			std::string s = g_keyword;
			token_vec.push_back(s);

			char* p = g_keyword;

			sscanf(g_token, "%s %s",g_keyword, g_leftover);
			// number of vertices
			if (!strcmp(g_keyword,tok_mesh_numvertices))
			{
				sscanf(g_leftover, "%d",&geom->numv);
				if (geom->numv)
					geom->v = new float[geom->numv * 3];
			} 
			// number of faces indices
			else if (!strcmp(g_keyword,tok_mesh_numfaces))
			{
				sscanf(g_leftover, "%d",&geom->numf);
				if (geom->numf)
				{
					geom->f = new unsigned int[geom->numf * 3];
					geom->fsubmat = new unsigned int[geom->numf];
					geom->smg = new unsigned int[geom->numf];
				}
			}
			// number of texture coords
			else if (!strcmp(g_keyword,tok_mesh_numtvertices))
			{
				sscanf(g_leftover, "%d",&geom->numt );
				if (geom->numt)
					geom->t = new float[geom->numt * 3];
			}
			else if (!strcmp(g_keyword,tok_mesh_numtvertlist))
			{
				result |= ase_texturevertexlist(stream,geom);
			}
			// vertex color
			else if (!strcmp(g_keyword,tok_mesh_numcvertex))
			{
				sscanf(g_leftover, "%d",&geom->numc );
				if (geom->numc)
					geom->c = new float[geom->numc * 4];
				//ase_vertexcolorlist(stream,geom);
			}
			else if (!strcmp(g_keyword,tok_mesh_numtvfaces))
			{
				sscanf(g_leftover, "%d",&geom->numtf );
				if (geom->numtf)
					geom->tf = new unsigned int[geom->numtf * 3];
			}
			else if (!strcmp(g_keyword,tok_mesh_vertexlist))
			{
				result |= ase_vertexlist(stream, geom);
			}
			else if (!strcmp(g_keyword,tok_mesh_facelist))
			{
				result |= ase_facelist(stream, geom);
			}
			else if (!strcmp(g_keyword,tok_mesh_normals))
			{
				if (geom->numf)
				{
					geom->fn = new float[geom->numf * 3];
					geom->fvn = new unsigned int[geom->numf * 3];
				}

				result |= ase_normals(stream,geom);
			}
			else if (!strcmp(g_keyword,tok_mesh_tangents))
			{
				if (geom->numf)
				{
					geom->ftn = new unsigned int[geom->numf * 3];
					geom->fbn = new unsigned int[geom->numf * 3];
				}			

				result |= ase_tangents(stream,geom);
			}
			else if (!strcmp(g_keyword,tok_mesh_facemaplist))
			{
				if (geom->numf)
					geom->fmapv = new unsigned int[geom->numf * 3 * 3];
				result |= ase_facemaplist(stream,geom);
			}
			else if (!strcmp(g_keyword,tok_mesh_tfacelist))
			{
				result |= ase_texturefacelist(stream,geom);
			}
			else if (!strcmp(g_keyword,tok_mesh_mappingchannel))
			{
				int mapIdx=0;
				sscanf(g_leftover, "%d",&mapIdx);
				result |= ase_mapchannel(stream,geom,mapIdx-1); //max是从1开始算的
			}
			else if (!strcmp(g_keyword,tok_mesh_vertexbones))
			{
				result |= ase_vertexbones(stream,geom);
			}
			else if( !strcmp(g_keyword,tok_mesh_skeleton) )
			{
				result |= ase_skeleton_bones(stream,geom);
			}
			else if (!strcmp(g_keyword,tok_close))
			{
				break;
			}
		}
		return result;
	}

	int ase_facelist(const char * stream, geomobj * geom)
	{
		int result = 0;

		while (ase_gettoken(stream, g_token))
		{
			sscanf(g_token, "%s %s",g_keyword, g_leftover);
			if (!strcmp(g_keyword,tok_mesh_face))
			{
				int    i1, i2, i3;
				unsigned int    index;
				int    AB, BC, CA;
				int    smg;
				int    submatidx;

				sscanf(g_token, "%s %d: A: %d  B: %d  C: %d AB: %d BC: %d CA: %d *MESH_SMOOTHING %d",
					g_keyword, &index, &i1, &i2, &i3, &AB, &BC, &CA, &smg);

				assert(index < geom->numf);

				char * pBuf = strrchr(g_token,'*');
				sscanf(pBuf,"*MESH_MTLID %d", &submatidx);

				assert(submatidx >= 0);
				geom->fsubmat[index] = submatidx;
				//assert(smg >= 0);
				geom->smg[index] = (smg == -1) ? 0 : smg;
				index *= 3;
				geom->f[index] = i1;
				geom->f[index + 1] = i2;
				geom->f[index + 2] = i3;
			}
			else if (!strcmp(g_keyword,tok_close))
			{
				break;
			}
		}   
		return result;
	}

	int ase_vertexlist(const char * stream, geomobj * geom)
	{
		char g_keyword[32];
		char g_leftover[256];
		int result = 0;
		float x, y, z;
		unsigned int   index;

		while (ase_gettoken(stream, g_token))
		{
			sscanf(g_token, "%s %s",g_keyword, g_leftover);
			if (!strcmp(g_keyword,tok_mesh_vertex))
			{
				sscanf(g_token, "%s %d %f %f %f",g_keyword, &index, &x, &y, &z);
				assert(geom->numv > index);

				index *= 3;
				geom->v[index ] = x * g_scale_hack;
				geom->v[index + 1] = y * g_scale_hack;
				geom->v[index + 2] = z * g_scale_hack;
			}
			else if (!strcmp(g_keyword,tok_close))
			{
				break;
			}
		}   
		return result;
	}

	int ase_skeleton_bones(const char* stream, geomobj* geom)
	{
		int result = 0;
		while (ase_gettoken(stream, g_token))
		{
			sscanf(g_token, "%s %s",g_keyword, g_leftover);
			if(!strcmp(g_keyword,tok_skeleton_bonenum))
			{
				int num = 0;
				sscanf(g_token, "%s %d",g_keyword, &num);
				geom->skelbones = new skeletonbone[num];
				geom->numskelbone = num;
			}
			else if (!strcmp(g_keyword,tok_skeleton_bone))
			{
				skeletonbone* skb = new skeletonbone();
				int index = 0;
				sscanf(g_token, "%s ID:%d",g_keyword, &index);	
				skb->id = index;
				geom->skelbones[index].id = index;

				char* pStart = strchr(g_token,'[') + 1;
				char* pEnd = strrchr(g_token,']');
				char* name = geom->skelbones[index].name;
				memcpy(name,pStart,pEnd - pStart);
				name[pEnd - pStart] = '\0';

				delete skb;
			}
			else if(!strcmp(g_keyword,tok_close))
			{
				break;
			}
		}

		return result;
	}

	int ase_vertexbones(const char* stream, geomobj* geom)
	{
		int result = 0;
		
		while (ase_gettoken(stream, g_token))
		{
			sscanf(g_token, "%s %s",g_keyword, g_leftover);
			if(!strcmp(g_keyword,tok_mesh_vertexbone_num))
			{
				int num = 0;
				sscanf(g_token, "%s %d",g_keyword, &num);
				geom->vbones = new vbonedata[num];
				geom->numvbone = num;
			}
			else if (!strcmp(g_keyword,tok_mesh_vertexbone))
			{				
				int index = 0;
				int error = 0;
				int id0,id1,id2,id3;
				float w0,w1,w2,w3;
				sscanf(g_token, "%s	%d	A:%d %f	B:%d %f	C:%d %f	D:%d %f	O:%d",
				g_keyword, &index, 
				&id0, &w0, 
				&id1, &w1, 
				&id2, &w2,  
				&id3, &w3, 
				&error);

				vbonedata& vbd = geom->vbones[index];
				vbd.bone_id[0] = id0;	vbd.bone_id[1] = id1;
				vbd.bone_id[2] = id2;	vbd.bone_id[3] = id3;

				vbd.weight[0] = w0;	vbd.weight[1] = w1;
				vbd.weight[2] = w2;	vbd.weight[3] = w3;
			}
			else if(!strcmp(g_keyword,tok_close))
			{
				break;
			}
		}

		return result;
	}

	int ase_normals(const char * stream, geomobj * geom)
	{
		float x, y, z;
		unsigned int   index;
		int result = 0;

		int faceindex;
		int inc;
		std::vector<float>   normals;
		while (ase_gettoken(stream, g_token))
		{
			sscanf(g_token, "%s %s",g_keyword, g_leftover);
			if (!strcmp(g_keyword,tok_mesh_facenormal))
			{
				sscanf(g_token, "%s %d %f %f %f",g_keyword, &index, &x, &y, &z);

				assert(index < geom->numf);
				index *= 3;
				faceindex = index;
				geom->fn[index ] = x;
				geom->fn[index + 1] = y;
				geom->fn[index + 2] = z;
				inc = 0;
			}
			else if (!strcmp(g_keyword,tok_mesh_vertexnormal))
			{
				sscanf(g_token, "%s %d %f %f %f",g_keyword, &index, &x, &y, &z);

				normals.push_back(x);
				normals.push_back(y);
				normals.push_back(z);
				geom->fvn[faceindex + inc] = normals.size() / 3 - 1;
				++inc;
			}
			else if (!strcmp(g_keyword,tok_close))
			{
				geom->numn = normals.size() / 3;
				geom->n = new float[geom->numn * 3];

				for (unsigned int i= 0; i < normals.size(); ++i)
					geom->n[i] = normals[i];
				break;
			}
		}
		return result;
	}

	int ase_tangents(const char * stream, geomobj * geom)
	{
		float x, y, z;
		unsigned int   index;
		int result = 0;

		int faceindex;
		int inctn,incbn;
		std::vector<float>   tangents;
		std::vector<float>   bnormals;
		while (ase_gettoken(stream, g_token))
		{
			sscanf(g_token, "%s %s",g_keyword, g_leftover);
			if (!strcmp(g_keyword,tok_mesh_facetangent))
			{
				sscanf(g_token, "%s %d",g_keyword, &index);
				faceindex = index;
				inctn = 0;
				incbn = 0;
			}
			else if (!strcmp(g_keyword,tok_mesh_vertextangent))
			{
				sscanf(g_token, "%s %d %f %f %f",g_keyword, &index, &x, &y, &z);

				tangents.push_back(x);
				tangents.push_back(y);
				tangents.push_back(z);
				geom->ftn[faceindex + inctn] = tangents.size() / 3 - 1;
				++inctn;
			}	
			else if (!strcmp(g_keyword,tok_mesh_vertexbnormal))
			{
				sscanf(g_token, "%s %d %f %f %f",g_keyword, &index, &x, &y, &z);

				bnormals.push_back(x);
				bnormals.push_back(y);
				bnormals.push_back(z);
				geom->ftn[faceindex + incbn] = bnormals.size() / 3 - 1;
				++incbn;
			}
			else if (!strcmp(g_keyword,tok_close))
			{
				geom->numtn = tangents.size() / 3;
				geom->tn = new float[geom->numtn * 3];

				for (unsigned int i= 0; i < tangents.size(); ++i)
					geom->tn[i] = tangents[i];

				geom->numbn = bnormals.size() / 3;
				geom->bn = new float[geom->numbn * 3];

				for (unsigned int i= 0; i < bnormals.size(); ++i)
					geom->bn[i] = bnormals[i];

				break;
			}
		}
		return result;
	}

	int ase_facemaplist(const char * stream, geomobj * geom)
	{
		char g_keyword[32];
		char g_leftover[256];
		int result = 0;

		while (ase_gettoken(stream, g_token))
		{
			sscanf(g_token, "%s %s",g_keyword, g_leftover);
			if (!strcmp(g_keyword, tok_mesh_facemap))
			{
				int index;
				sscanf(g_token, "%s %d",g_keyword, &index);
				result |= ase_facemapvert(stream, geom, index);
			}
			else if (!strcmp(g_keyword,tok_close))
			{
				break;
			}
		}   
		return result;
	}

	int ase_facemapvert(const char * stream, geomobj * geom, int index)
	{
		int result = 0;
		int inc = 0;

		while (ase_gettoken(stream, g_token))
		{
			sscanf(g_token, "%s %s",g_keyword, g_leftover);
			if (!strcmp(g_keyword, tok_mesh_facemapvert))
			{
				unsigned int a, b, c;
				sscanf(g_token, "%s %d %d %d",g_keyword, &a, &b, &c);
				geom->fmapv[geom->f[index * 3 + inc]] = a;
				geom->fmapv[geom->f[index * 3 + inc] + 1] = b;
				geom->fmapv[geom->f[index * 3 + inc] + 2] = c;
				inc++; 
			}
			else if (!strcmp(g_keyword,tok_close))
			{
				break;
			}
		}   
		return result;
	}

	int ase_texturevertexlist(const char * stream, geomobj * geom)
	{
		int result = 0;
		float x, y, z;
		int   index;

		while (ase_gettoken(stream, g_token))
		{
			sscanf(g_token, "%s %s",g_keyword, g_leftover);
			if (!strcmp(g_keyword, tok_mesh_tvert))
			{
				sscanf(g_token, "%s %d %f %f %f",g_keyword, &index, &x, &y, &z);
				index *= 3;
				geom->t[index + 0] = x;
				geom->t[index + 1] = y; 
				geom->t[index + 2] = z;
			}
			else if (!strcmp(g_keyword,tok_close))
			{
				break;
			}
		}   
		return result;
	}

	int ase_texturefacelist(const char * stream, geomobj * geom)
	{
		int result = 0;
		int x, y, z;
		unsigned int   index;

		while (ase_gettoken(stream, g_token))
		{
			sscanf(g_token, "%s %s",g_keyword, g_leftover);
			if (!strcmp(g_keyword,tok_mesh_tface))
			{
				sscanf(g_token, "%s %d %d %d %d",g_keyword, &index, &x, &y, &z);
				assert(index < geom->numf);
				index *= 3;
				geom->tf[index] = x;
				geom->tf[index + 1] = y;
				geom->tf[index + 2] = z;
			}
			else if (!strcmp(g_keyword,tok_close))
			{
				break;
			}
		}   
		return result;
	}

	int ase_texturevertexlist_idx(const char * stream, geomobj * geom,int tc_idx,unsigned int number_of_elem) //by sssa2000 090924
	{
		int result = 0;
		while (ase_gettoken(stream, g_token))
		{
			sscanf(g_token, "%s %s",g_keyword, g_leftover);
			if (!strcmp(g_keyword, tok_mesh_tvert))
			{
				int index;
				float x,y,z;
				sscanf(g_token, "%s %d %f %f %f",g_keyword, &index, &x, &y, &z);
			} 
			else if (!strcmp(g_keyword,tok_close))
			{
				break;
			}
		}   
		return result;
	}

	int ase_tm_animation(const char * stream, geomobj * geom)
	{
		int result = 0;

		geom->anim = new animdata;

		while (ase_gettoken(stream, g_token))
		{
			sscanf(g_token, "%s %s",g_keyword, g_leftover);
			if (!strcmp(g_keyword,tok_control_pos_track))
				result |= ase_postrack(stream,geom->anim);
			else if (!strcmp(g_keyword,tok_control_rot_track))
				result |= ase_rottrack(stream,geom->anim);
			else if (!strcmp(g_keyword,tok_control_scale_track))
				result |= ase_skip(stream);
			else if (!strcmp(g_keyword,tok_close))
			{
				break;
			}
		}
		return result;
	}

	int ase_postrack(const char * stream,animdata * anim)
	{
		int result = 0;

		std::vector<float> posstack;
		std::vector<int> framestack;
		while (ase_gettoken(stream, g_token))
		{
			sscanf(g_token, "%s %s",g_keyword, g_leftover);
			if (!strcmp(g_keyword,tok_control_pos_sample))
			{
				float x,y,z;
				int t;

				sscanf(g_token,"%s %d %f %f %f",g_keyword,&t, &x, &y, &z);
				posstack.push_back(x * g_scale_hack);
				posstack.push_back(y * g_scale_hack);
				posstack.push_back(z * g_scale_hack);
				framestack.push_back(t);
			}
			else if (!strcmp(g_keyword,tok_close))
			{
				std::vector<float>::iterator it = posstack.begin();
				anim->numpos = posstack.size() / 3;
				anim->pos = new float[anim->numpos * 3];
				float * ppos = anim->pos;
				while (it != posstack.end())
				{
					*ppos = *it;
					++ppos;
					++it;
				}

				std::vector<int>::iterator ft = framestack.begin();
				anim->posframetime = new int[anim->numpos];
				int * ppf = anim->posframetime;
				while(ft != framestack.end())
				{
					*ppf = *ft;
					++ppf;
					++ft;
				}

				break;
			}
		}
		return result;
	}

	int ase_rottrack(const char * stream,animdata * anim)
	{
		int result = 0;

		std::vector<float> rotstack;
		std::vector<int> framestack;
		while (ase_gettoken(stream, g_token))
		{
			sscanf(g_token, "%s %s",g_keyword, g_leftover);
			if (!strcmp(g_keyword,tok_control_rot_sample))
			{
				float x,y,z,w;
				int t;

				sscanf(g_token,"%s %d %f %f %f %f",g_keyword, &t, &x, &y, &z, &w);
				asemath::quaternionf q(x,y,z,w);
				q.conjugate();

				rotstack.push_back(q[0]);
				rotstack.push_back(q[1]);
				rotstack.push_back(q[2]);
				rotstack.push_back(q[3]);
				framestack.push_back(t);
			}
			else if (!strcmp(g_keyword,tok_close))
			{
				std::vector<float>::iterator it = rotstack.begin();
				anim->numrot = rotstack.size() / 4;
				anim->rot = new float[anim->numrot * 4];				
				float * prot = anim->rot;
				while (it != rotstack.end())
				{
					*prot = *it;
					++prot;
					++it;
				}

				std::vector<int>::iterator ft = framestack.begin();
				anim->rotframetime = new int[anim->numrot];
				int * prf = anim->rotframetime;
				while(ft != framestack.end())
				{
					*prf = *ft;
					++prf;
					++ft;
				}

				break;
			}
		}
		return result;
	}

	int ase_vertex_weight(const char * stream, model * m, geomobj * geom)
	{
		char buf[256];
		int result = 0;
		float weight;
		float off_x, off_y, off_z;
		int v_idx;
		int i_times_4;

		std::vector<float>          bvstack;
		std::vector<float*>         bvmatstack;
		std::vector<geomobj*>       bvgeomstack;
		std::vector<unsigned int>   vbvstack;

		while (ase_gettoken(stream, g_token))
		{
			sscanf(g_token, "%s %s",g_keyword, g_leftover);
			if (!strcmp(g_keyword, tok_mesh_vertex_weight))
			{
				const char * pStart = strchr(g_token, '\"');
				sscanf(g_token, "%s %d",g_keyword,&v_idx);
				const char * pEnd = strrchr(g_token, '\"');
				if (pEnd-pStart+1>256)
					fprintf (stderr,"Buffer Overrun Detected");
				strncpy(buf,pStart,pEnd - pStart + 1);
				buf[pEnd - pStart + 1] = '\0';

				sscanf(pEnd + 1,"%f %f %f %f", &off_x, &off_y, &off_z, &weight);

				geomobj * geomref = get_geomobj(m, buf);
				// the reference must exist!
				if (geomref == NULL)
				{
					geomref = new geomobj;
					geomref->name = new char[strlen(buf) + 1];
					strcpy(geomref->name,buf);
					m->geom.push_back(geomref);
				}

				// push back all data
				bvgeomstack.push_back(geomref);
				bvmatstack.push_back(geomref->tm);

				vbvstack.push_back(v_idx);

				bvstack.push_back(off_x * g_scale_hack);
				bvstack.push_back(off_y * g_scale_hack);
				bvstack.push_back(off_z * g_scale_hack);
				bvstack.push_back(weight);
			}
			else if (!strcmp(g_keyword,"}"))
			{
				geom->numbv = bvstack.size() / 4;
				if (geom->numbv)
				{
					geom->bv = new float[4 * geom->numbv];
					geom->bmatref = new float*[geom->numbv];
					geom->bgeomref = new geomobj*[geom->numbv];
					geom->vbv = new unsigned int[geom->numbv];

					for (unsigned int i = 0; i < geom->numbv; ++i)
					{
						i_times_4 = i * 4;
						geom->bv[i_times_4] = bvstack[i_times_4];
						geom->bv[i_times_4 + 1] = bvstack[i_times_4 + 1];
						geom->bv[i_times_4 + 2] = bvstack[i_times_4 + 2];
						geom->bv[i_times_4 + 3] = bvstack[i_times_4 + 3];

						geom->vbv[i] = vbvstack[i];

						geom->bmatref[i] = bvmatstack[i];

						geom->bgeomref[i] = bvgeomstack[i];
					}
				}
				break;
			}
		}   
		return result;
	}

	int ase_passthru(const char * stream, model * m)
	{
		int result = 0;

		int back_stream_cnt = g_stream_cnt;
		while (ase_gettoken(stream, g_token))
		{
			sscanf(g_token, "%s %s",g_keyword, g_leftover);
			if (!strcmp(g_keyword,tok_close))
			{
				break;
			}
			else
			{
				g_stream_cnt = back_stream_cnt;
				result |= ase_parse(stream, m);
			}
		}

		return result;
	}

	int ase_skip(const char * stream)
	{
		int result = 0;

		while (ase_gettoken(stream, g_token))
		{
			sscanf(g_token, "%s %s",g_keyword, g_leftover);
			if (!strcmp(g_keyword,tok_close))
			{
				break;
			}
		}
		return result;
	}

	int ase_texturevertexlist_null(const char * stream, geomobj * geom)
	{
		int result = 0;
		float x, y, z;
		int   index;


		while (ase_gettoken(stream, g_token))
		{
			sscanf(g_token, "%s %s",g_keyword, g_leftover);
			if (!strcmp(g_keyword, tok_mesh_tvert))
			{
				sscanf(g_token, "%s %d %f %f %f",g_keyword, &index, &x, &y, &z);
			} 
			else if (!strcmp(g_keyword,tok_close))
			{
				break;
			}
		}   
		return result;
	}
	int ase_texturefacelist_null(const char * stream, geomobj * geom)
	{
		int result = 0;
		int x, y, z;
		int   index;

		while (ase_gettoken(stream, g_token))
		{
			sscanf(g_token, "%s %s",g_keyword, g_leftover);
			if (!strcmp(g_keyword,tok_mesh_tface))
			{
				sscanf(g_token, "%s %d %d %d %d",g_keyword, &index, &x, &y, &z);
			}
			else if (!strcmp(g_keyword,tok_close))
			{
				break;
			}
		}   
		return result;
	}
	int ase_mapchannel(const char* stream,geomobj* geom,int mapIdx)
	{
		int result = 0;
		while (ase_gettoken(stream,g_token))
		{
			sscanf(g_token,"%s %s",g_keyword,g_leftover);
			if (!strcmp(g_keyword,tok_mesh_numtvertices))
			{
				int numt2;
				sscanf(g_leftover, "%d",&numt2);
				if (numt2)
				{
					ase_texturevertexlist_idx(stream,geom,mapIdx,numt2);
				}
			}
			else if (!strcmp(g_keyword,tok_mesh_numtvertlist))
			{
				result |= ase_texturevertexlist_null(stream,geom);
			}
			else if ( !strcmp(g_keyword,tok_mesh_numtvfaces) )
			{
			}
			else if ( !strcmp(g_keyword,tok_mesh_tfacelist) )
			{
				result |= ase_texturefacelist_null(stream,geom);
			}
			else if (!strcmp(g_keyword,tok_close))
			{
				break;
			}
		}
		return result;
	}
}

