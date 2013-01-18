#ifndef _OLA_INCLUDE_COMMON_INTERFACE_H__
#define _OLA_INCLUDE_COMMON_INTERFACE_H__

namespace ola
{
	struct tVec3
	{
		tVec3(float _x,float _y,float _z):x(_x),y(_y),z(_z){}
		float x,y,z;
	};

	struct tAABB
	{
		tAABB():maxv(0,0,0),minv(0,0,0){}
		tVec3 maxv;
		tVec3 minv;
	};	

	struct tQuat
	{
		tQuat(float _x,float _y,float _z,float _w):x(_x),y(_y),z(_z),w(_w){}
		float x,y,z,w;
	};

	class ITransform
	{
	public:
		virtual void getPosition(tVec3* out_pos) = 0;
		virtual void setPosition(float x,float y,float z) = 0;

		virtual void getRotation(tQuat* out_rot) = 0;
		virtual void setRotation(float axisx,float axisy,float axisz,float degree) = 0;

		virtual ITransform* parent() = 0;
		virtual ITransform** subs(int* out_num) = 0;

		virtual ITransform* setParent(ITransform* p) = 0;		

		virtual void transform(float* out_mat44) = 0;
	};
}

#endif