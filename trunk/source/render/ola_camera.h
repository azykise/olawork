#ifndef __OLA__CAMREA_H__
#define __OLA__CAMREA_H__

#include "math/ola_math.h"

struct tViewParams
{
	float pos[3];
	float lookat[3];
	float fovy;
	float aspect;
	float near_dist;
	float far_dist;	
};

class OlaVFrustum
{
public:
	OlaVFrustum();

	virtual ~OlaVFrustum();

	void setEyePt(olaVec3& eyePt)	{m_eyePt = eyePt;	updateView();}
	void setLookPt(olaVec3& lookPt)	{m_lookPt = lookPt;	updateView();}
	void setUpDir(olaVec3& upDir)	{m_upDir = upDir;	updateView();}
	void setFOVY(float fovy)		{m_fovy = fovy;		updateProj();}
	void setAspect(float asp)		{m_aspect = asp;	updateProj();}
	void setNearDist(float nd)		{m_near_dist = nd;	updateProj();}
	void setFarDist(float fd)		{m_far_dist = fd;	updateProj();}

	olaVec3 getEyePt()					{return m_eyePt;}
	olaVec3 getLookPt()				{return m_lookPt;}
	olaVec3 getUpDir()					{return m_upDir;}

	olaVec3 getViewDir();				
	
	olaMat4& getViewMatrix()			{return m_view_mat;}
	olaMat4& getProjMatrix()			{return m_proj_mat;}

	olaVec3 getCorner(int n)			{return n>=0 && n < 8 ? m_corners[n] : olaVec3(0,0,0);}

	void toViewParam(tViewParams* vp);
	void fromViewParam(tViewParams* vp);

protected:

	void updateView();
	void updateProj();

	olaVec3 m_eyePt;
	olaVec3 m_lookPt;

	olaVec3 m_upDir;

	olaMat4 m_view_mat;
	olaMat4 m_proj_mat;

	float m_fovy;
	float m_aspect;
	float m_near_dist;
	float m_far_dist;	

	olaVec3  m_corners[8];
};

#endif