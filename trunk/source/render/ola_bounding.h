#ifndef _OLA_INCLUDE_BOUNDING_H__
#define _OLA_INCLUDE_BOUNDING_H__

#include "math/ola_math.h"

struct olaSphere;
struct olaOBB;

struct olaAABB
{
	enum BOUNDING_BOX_PLANES
	{
		BOUNDING_BOX_NEG_X_PLANE=0,
		BOUNDING_BOX_POS_X_PLANE,
		BOUNDING_BOX_NEG_Y_PLANE,
		BOUNDING_BOX_POS_Y_PLANE,
		BOUNDING_BOX_POS_Z_PLANE,
		BOUNDING_BOX_NEG_Z_PLANE,
		BOUNDING_BOX_PLANE_END
	};

	void set(olaVec3& _min,olaVec3& _max);

	void merge(olaAABB& _ab);
	
	bool intersect(olaVec3& _v);
	bool intersect(olaAABB& _aabb);
	bool intersect(olaSphere& _sp);
	bool intersect(olaPlane& _p);

	static const int VERTEX_NUM = 8;
	olaVec3 min;
	olaVec3 max;
	olaVec3 vertices[VERTEX_NUM];
	olaPlane planes[BOUNDING_BOX_PLANE_END];
};

struct olaSphere
{
	void set(olaVec3& c,float r);
	olaVec3 center;
	float radius;
};

struct olaOBB
{

};

#endif