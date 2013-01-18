// Copyright (C) 2004 Id Software, Inc.
//

#ifndef __MATH_OLA_PLANE_H__
#define __MATH_OLA_PLANE_H__

/*
===============================================================================

3D plane with equation: a * x + b * y + c * z + d = 0

===============================================================================
*/

#include "ola_mathbase.h"

class olaVec3;
class olaMat3;
class olaVec4;

#define	ON_EPSILON					0.1f
#define DEGENERATE_DIST_EPSILON		1e-4f

#define	SIDE_FRONT					0
#define	SIDE_BACK					1
#define	SIDE_ON						2
#define	SIDE_CROSS					3

// plane sides
#define PLANESIDE_FRONT				0
#define PLANESIDE_BACK				1
#define PLANESIDE_ON				2
#define PLANESIDE_CROSS				3

// plane types
#define PLANETYPE_X					0
#define PLANETYPE_Y					1
#define PLANETYPE_Z					2
#define PLANETYPE_NEGX				3
#define PLANETYPE_NEGY				4
#define PLANETYPE_NEGZ				5
#define PLANETYPE_TRUEAXIAL			6	// all types < 6 are true axial planes
#define PLANETYPE_ZEROX				6
#define PLANETYPE_ZEROY				7
#define PLANETYPE_ZEROZ				8
#define PLANETYPE_NONAXIAL			9

class olaPlane {
public:
	olaPlane( void );
	olaPlane( float a, float b, float c, float d );
	olaPlane( const olaVec3 &normal, const float dist );

	float			operator[]( int index ) const;
	float &			operator[]( int index );
	olaPlane		operator-() const;						// flips plane
	olaPlane &		operator=( const olaVec3 &v );			// sets normal and sets idPlane::d to zero
	olaPlane		operator+( const olaPlane &p ) const;	// add plane equations
	olaPlane		operator-( const olaPlane &p ) const;	// subtract plane equations
	olaPlane &		operator*=( const olaMat3 &m );			// Normal() *= m

	bool			Compare( const olaPlane &p ) const;						// exact compare, no epsilon
	bool			Compare( const olaPlane &p, const float epsilon ) const;	// compare with epsilon
	bool			Compare( const olaPlane &p, const float normalEps, const float distEps ) const;	// compare with epsilon
	bool			operator==(	const olaPlane &p ) const;					// exact compare, no epsilon
	bool			operator!=(	const olaPlane &p ) const;					// exact compare, no epsilon

	void			Zero( void );							// zero plane
	void			SetNormal( const olaVec3 &normal );		// sets the normal
	void			SetNormal( const float _x,const float _y,const float _z);// sets the normal
	const olaVec3 &	Normal( void ) const;					// reference to const normal
	olaVec3 &		Normal( void );							// reference to normal
	float			Normalize( bool fixDegenerate = true );	// only normalizes the plane normal, does not adjust d
	bool			FixDegenerateNormal( void );			// fix degenerate normal
	bool			FixDegeneracies( float distEpsilon );	// fix degenerate normal and dist
	float			Dist( void ) const;						// returns: -d
	void			SetDist( const float dist );			// sets: d = -dist
	int				Type( void ) const;						// returns plane type

	bool			FromPoints( const olaVec3 &p1, const olaVec3 &p2, const olaVec3 &p3, bool fixDegenerate = true );
	bool			FromVecs( const olaVec3 &dir1, const olaVec3 &dir2, const olaVec3 &p, bool fixDegenerate = true );
	void			FitThroughPoint( const olaVec3 &p );	// assumes normal is valid
	bool			HeightFit( const olaVec3 *points, const int numPoints );
	olaPlane		Translate( const olaVec3 &translation ) const;
	olaPlane &		TranslateSelf( const olaVec3 &translation );
	olaPlane		Rotate( const olaVec3 &origin, const olaMat3 &axis ) const;
	olaPlane &		RotateSelf( const olaVec3 &origin, const olaMat3 &axis );

	float			Distance( const olaVec3 &v ) const;
	int				Side( const olaVec3 &v, const float epsilon = 0.0f ) const;

	bool			LineIntersection( const olaVec3 &start, const olaVec3 &end ) const;
	// intersection point is start + dir * scale
	bool			RayIntersection( const olaVec3 &start, const olaVec3 &dir, float &scale ) const;
	bool			PlaneIntersection( const olaPlane &plane, olaVec3 &start, olaVec3 &dir ) const;

	int				GetDimension( void ) const;

	const olaVec4 &	ToVec4( void ) const;
	olaVec4 &		ToVec4( void );
	const float *	ToFloatPtr( void ) const;
	float *			ToFloatPtr( void );

private:
	float			a;
	float			b;
	float			c;
	float			d;
};

extern olaPlane plane_origin;
#define plane_zero plane_origin


#endif /* !__MATH_PLANE_H__ */
