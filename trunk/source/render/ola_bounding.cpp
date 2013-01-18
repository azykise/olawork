#include "ola_bounding.h"

void olaAABB::set( olaVec3& _min,olaVec3& _max )
{
	min = _min;
	max = _max;

	//set the positions of the vertices
	vertices[0].Set(min.x, min.y, min.z);
	vertices[1].Set(min.x, min.y, max.z);
	vertices[2].Set(min.x, max.y, min.z);
	vertices[3].Set(min.x, max.y, max.z);
	vertices[4].Set(max.x, min.y, min.z);
	vertices[5].Set(max.x, min.y, max.z);
	vertices[6].Set(max.x, max.y, min.z);
	vertices[7].Set(max.x, max.y, max.z);

	//set up the planes
	planes[BOUNDING_BOX_NEG_X_PLANE].SetNormal(1.0f, 0.0f, 0.0f);
	planes[BOUNDING_BOX_NEG_X_PLANE].SetDist(-min.x);

	planes[BOUNDING_BOX_POS_X_PLANE].SetNormal(-1.0f, 0.0f, 0.0f);
	planes[BOUNDING_BOX_POS_X_PLANE].SetDist(max.x);

	planes[BOUNDING_BOX_NEG_Y_PLANE].SetNormal(0.0f, 1.0f, 0.0f);
	planes[BOUNDING_BOX_NEG_Y_PLANE].SetDist(-min.y);

	planes[BOUNDING_BOX_POS_Y_PLANE].SetNormal(0.0f,-1.0f, 0.0f);
	planes[BOUNDING_BOX_POS_Y_PLANE].SetDist(max.y);

	planes[BOUNDING_BOX_NEG_Z_PLANE].SetNormal(0.0f, 0.0f, 1.0f);
	planes[BOUNDING_BOX_NEG_Z_PLANE].SetDist(-min.z);

	planes[BOUNDING_BOX_POS_Z_PLANE].SetNormal(0.0f, 0.0f,-1.0f);
	planes[BOUNDING_BOX_POS_Z_PLANE].SetDist(max.z);
}

void olaAABB::merge( olaAABB& _ab )
{
	olaVec3 _min(0.0f,0.0f,0.0f);
	olaVec3 _max(0.0f,0.0f,0.0f);

	_min.x = min.x < _ab.min.x ? min.x : _ab.min.x;
	_min.y = min.y < _ab.min.y ? min.x : _ab.min.y;
	_min.z = min.z < _ab.min.z ? min.z : _ab.min.z;

	_max.x = _max.x > _ab.max.x ? _max.x : _ab.max.x;
	_max.y = _max.y > _ab.max.y ? _max.y : _ab.max.y;
	_max.z = _max.z > _ab.max.z ? _max.z : _ab.max.z;

	this->set(_min,_max);
}

bool olaAABB::intersect( olaVec3& _v )
{
	for(int i=0; i<BOUNDING_BOX_PLANE_END; ++i)
	{
		if(planes[i].Side(_v)==PLANESIDE_BACK)
			return true;
	}
	return false;
}

bool olaAABB::intersect( olaAABB& _aabb )
{
	for (int i = 0 ; i < VERTEX_NUM ; i++)
	{
		if (intersect(_aabb.vertices[i]))
			return true;
	}
	return false;
}

bool olaAABB::intersect( olaSphere& _sp )
{
	return false;
}

bool olaAABB::intersect( olaPlane& _p )
{
	bool bup = false;
	for (int i = 0 ; i < VERTEX_NUM ; i++)
	{
		if(planes[i].Side(vertices[i])==PLANESIDE_ON)
			return true;
		else if (planes[i].Side(vertices[i])==PLANESIDE_FRONT)
			bup = true;
	}

	for (int i = 0 ; i < VERTEX_NUM ; i++)
	{
		if(planes[i].Side(vertices[i])==PLANESIDE_BACK && bup)
			return true;
	}

	return false;
}



void olaSphere::set( olaVec3& c,float r )
{
	center = c;
	radius = r;
}
