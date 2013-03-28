#include "ola_camera.h"

const float PI = 3.141592654;

OlaVFrustum::OlaVFrustum():
m_fovy(PI/4),
m_aspect(4/3),
m_near_dist(1),
m_far_dist(1000)
{
	m_upDir.Set(0,0,1);
	m_eyePt.Set(0,-20,0);
	m_lookPt.Set(0,0,0);

	m_view_mat.Identity();
	m_proj_mat.Identity();

	updateView();
	updateProj();
}

OlaVFrustum::~OlaVFrustum()
{

}

olaVec3 OlaVFrustum::getViewDir()
{
	olaVec3 vd = m_lookPt - m_eyePt;
	vd.Normalize();
	return vd;
}

void OlaVFrustum::updateView()
{	
	//z轴向上的相机
	olaVec3 vDir = getViewDir();	

	static olaVec3 zAxis(0.0f,0.0f,1.0f);

	m_upDir = !FLOATNOTZERO(vDir.x) || !FLOATNOTZERO(vDir.y) ? zAxis : olaVec3(0.0f,1.0f,0.0f);		

	m_upDir = zAxis;

	olaVec3 zaxis = -1.0f * vDir;
	olaVec3 xaxis = m_upDir.Cross(zaxis);
	xaxis.Normalize();
	olaVec3 yaxis = zaxis.Cross(xaxis);

	m_view_mat.Identity();
	m_view_mat[0][0] = xaxis.x; m_view_mat[0][1] = xaxis.y; m_view_mat[0][2] = xaxis.z; m_view_mat[0][3] = -1.0f * xaxis * m_eyePt;
	m_view_mat[1][0] = yaxis.x; m_view_mat[1][1] = yaxis.y; m_view_mat[1][2] = yaxis.z; m_view_mat[1][3] = -1.0f * yaxis * m_eyePt;
	m_view_mat[2][0] = zaxis.x; m_view_mat[2][1] = zaxis.y; m_view_mat[2][2] = zaxis.z; m_view_mat[2][3] = -1.0f * zaxis * m_eyePt;
	m_view_mat[3][0] = 0;		m_view_mat[3][1] = 0;		m_view_mat[3][2] = 0;		m_view_mat[3][3] = 1;
}

void OlaVFrustum::updateProj()
{	
	//右手坐标系的透视矩阵
	float yscale = 1/olaMath::Tan(m_fovy/2);
	float xscale = yscale/m_aspect;

	m_proj_mat.Identity();

	m_proj_mat[0][0] = xscale;
	m_proj_mat[1][1] = yscale;
	m_proj_mat[2][2] = ( m_far_dist + m_near_dist) / (m_near_dist - m_far_dist);
	m_proj_mat[2][3] = (2.0 * m_near_dist * m_far_dist) / (m_near_dist - m_far_dist);
	m_proj_mat[3][2] = -1;	
	m_proj_mat[3][3] = 0;
}

void OlaVFrustum::toViewParam(tViewParam* vp)
{
	vp->pos[0] = m_eyePt.x;
	vp->pos[1] = m_eyePt.y;
	vp->pos[2] = m_eyePt.z;

	vp->lookat[0] = m_lookPt.x;
	vp->lookat[1] = m_lookPt.y;
	vp->lookat[2] = m_lookPt.z;

	vp->aspect = m_aspect;
	vp->far_dist = m_far_dist;
	vp->near_dist = m_near_dist;
	vp->fovy = m_fovy;	
}

void OlaVFrustum::fromViewParam(tViewParam* vp)
{
	m_eyePt.x = vp->pos[0];
	m_eyePt.y = vp->pos[1];
	m_eyePt.z = vp->pos[2];

	m_lookPt.x = vp->lookat[0];
	m_lookPt.y = vp->lookat[1];
	m_lookPt.z = vp->lookat[2];

	m_aspect = vp->aspect;
	m_far_dist = vp->far_dist;
	m_near_dist = vp->near_dist;
	m_fovy = vp->fovy;

	updateProj();
	updateView();
}
