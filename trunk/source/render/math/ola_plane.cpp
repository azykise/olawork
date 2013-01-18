// Copyright (C) 2004 Id Software, Inc.
//

#include "ola_vector.h"
#include "ola_matrix.h"
#include "ola_mathbase.h"
#include "ola_plane.h"
#pragma hdrstop
#pragma warning( push )

#pragma warning(disable : 4100)				// unreferenced formal parameter
#pragma warning(disable : 4244)				// conversion to smaller type, possible loss of data
#pragma warning(disable : 4714)				// function marked as __forceinline not inlined
#pragma warning(disable : 4311)				//  pointer truncation from 'void *' to 'int'
#pragma warning(disable : 4312)				// conversion from 'int' to 'void *' of greater size
#pragma warning(disable : 4267)				// conversion from 'size_t' to 'int', possible loss of data
olaPlane plane_origin( 0.0f, 0.0f, 0.0f, 0.0f );


olaPlane::olaPlane( void ) {
}

olaPlane::olaPlane( float a, float b, float c, float d ) {
	this->a = a;
	this->b = b;
	this->c = c;
	this->d = d;
}

olaPlane::olaPlane( const olaVec3 &normal, const float dist ) {
	this->a = normal.x;
	this->b = normal.y;
	this->c = normal.z;
	this->d = -dist;
}

float olaPlane::operator[]( int index ) const {
	return ( &a )[ index ];
}

float& olaPlane::operator[]( int index ) {
	return ( &a )[ index ];
}

olaPlane olaPlane::operator-() const {
	return olaPlane( -a, -b, -c, -d );
}

olaPlane &olaPlane::operator=( const olaVec3 &v ) { 
	a = v.x;
	b = v.y;
	c = v.z;
	d = 0;
	return *this;
}

olaPlane olaPlane::operator+( const olaPlane &p ) const {
	return olaPlane( a + p.a, b + p.b, c + p.c, d + p.d );
}

olaPlane olaPlane::operator-( const olaPlane &p ) const {
	return olaPlane( a - p.a, b - p.b, c - p.c, d - p.d );
}

olaPlane &olaPlane::operator*=( const olaMat3 &m ) {
	Normal() *= m;
	return *this;
}

bool olaPlane::Compare( const olaPlane &p ) const {
	return ( a == p.a && b == p.b && c == p.c && d == p.d );
}

bool olaPlane::Compare( const olaPlane &p, const float epsilon ) const {
	if ( olaMath::Fabs( a - p.a ) > epsilon ) {
		return false;
	}

	if ( olaMath::Fabs( b - p.b ) > epsilon ) {
		return false;
	}

	if ( olaMath::Fabs( c - p.c ) > epsilon ) {
		return false;
	}

	if ( olaMath::Fabs( d - p.d ) > epsilon ) {
		return false;
	}

	return true;
}

bool olaPlane::Compare( const olaPlane &p, const float normalEps, const float distEps ) const {
	if ( olaMath::Fabs( d - p.d ) > distEps ) {
		return false;
	}
	if ( !Normal().Compare( p.Normal(), normalEps ) ) {
		return false;
	}
	return true;
}

bool olaPlane::operator==( const olaPlane &p ) const {
	return Compare( p );
}

bool olaPlane::operator!=( const olaPlane &p ) const {
	return !Compare( p );
}

void olaPlane::Zero( void ) {
	a = b = c = d = 0.0f;
}

void olaPlane::SetNormal( const float _x,const float _y,const float _z)
{
	a = _x;
	b = _y;
	c = _z;
}

void olaPlane::SetNormal( const olaVec3 &normal ) {
	a = normal.x;
	b = normal.y;
	c = normal.z;
}

const olaVec3 &olaPlane::Normal( void ) const {
	return *reinterpret_cast<const olaVec3 *>(&a);
}

olaVec3 &olaPlane::Normal( void ) {
	return *reinterpret_cast<olaVec3 *>(&a);
}

float olaPlane::Normalize( bool fixDegenerate ) {
	float length = reinterpret_cast<olaVec3 *>(&a)->Normalize();

	if ( fixDegenerate ) {
		FixDegenerateNormal();
	}
	return length;
}

bool olaPlane::FixDegenerateNormal( void ) {
	return Normal().FixDegenerateNormal();
}

bool olaPlane::FixDegeneracies( float distEpsilon ) {
	bool fixedNormal = FixDegenerateNormal();
	// only fix dist if the normal was degenerate
	if ( fixedNormal ) {
		if ( olaMath::Fabs( d - olaMath::Rint( d ) ) < distEpsilon ) {
			d = olaMath::Rint( d );
		}
	}
	return fixedNormal;
}

float olaPlane::Dist( void ) const {
	return -d;
}

void olaPlane::SetDist( const float dist ) {
	d = -dist;
}

bool olaPlane::FromPoints( const olaVec3 &p1, const olaVec3 &p2, const olaVec3 &p3, bool fixDegenerate ) {
	Normal() = (p1 - p2).Cross( p3 - p2 );
	if ( Normalize( fixDegenerate ) == 0.0f ) {
		return false;
	}
	d = -( Normal() * p2 );
	return true;
}

bool olaPlane::FromVecs( const olaVec3 &dir1, const olaVec3 &dir2, const olaVec3 &p, bool fixDegenerate ) {
	Normal() = dir1.Cross( dir2 );
	if ( Normalize( fixDegenerate ) == 0.0f ) {
		return false;
	}
	d = -( Normal() * p );
	return true;
}

void olaPlane::FitThroughPoint( const olaVec3 &p ) {
	d = -( Normal() * p );
}

olaPlane olaPlane::Translate( const olaVec3 &translation ) const {
	return olaPlane( a, b, c, d - translation * Normal() );
}

olaPlane &olaPlane::TranslateSelf( const olaVec3 &translation ) {
	d -= translation * Normal();
	return *this;
}

olaPlane olaPlane::Rotate( const olaVec3 &origin, const olaMat3 &axis ) const {
	olaPlane p;
	p.Normal() = Normal() * axis;
	p.d = d + origin * Normal() - origin * p.Normal();
	return p;
}

olaPlane &olaPlane::RotateSelf( const olaVec3 &origin, const olaMat3 &axis ) {
	d += origin * Normal();
	Normal() *= axis;
	d -= origin * Normal();
	return *this;
}

float olaPlane::Distance( const olaVec3 &v ) const {
	return a * v.x + b * v.y + c * v.z + d;
}

int olaPlane::Side( const olaVec3 &v, const float epsilon ) const {
	float dist = Distance( v );
	if ( dist > epsilon ) {
		return PLANESIDE_FRONT;
	}
	else if ( dist < -epsilon ) {
		return PLANESIDE_BACK;
	}
	else {
		return PLANESIDE_ON;
	}
}

bool olaPlane::LineIntersection( const olaVec3 &start, const olaVec3 &end ) const {
	float d1, d2, fraction;

	d1 = Normal() * start + d;
	d2 = Normal() * end + d;
	if ( d1 == d2 ) {
		return false;
	}
	if ( d1 > 0.0f && d2 > 0.0f ) {
		return false;
	}
	if ( d1 < 0.0f && d2 < 0.0f ) {
		return false;
	}
	fraction = ( d1 / ( d1 - d2 ) );
	return ( fraction >= 0.0f && fraction <= 1.0f );
}

bool olaPlane::RayIntersection( const olaVec3 &start, const olaVec3 &dir, float &scale ) const {
	float d1, d2;

	d1 = Normal() * start + d;
	d2 = Normal() * dir;
	if ( d2 == 0.0f ) {
		return false;
	}
	scale = -( d1 / d2 );
	return true;
}

int olaPlane::GetDimension( void ) const {
	return 4;
}

const olaVec4 &olaPlane::ToVec4( void ) const {
	return *reinterpret_cast<const olaVec4 *>(&a);
}

olaVec4 &olaPlane::ToVec4( void ) {
	return *reinterpret_cast<olaVec4 *>(&a);
}

const float *olaPlane::ToFloatPtr( void ) const {
	return reinterpret_cast<const float *>(&a);
}

float *olaPlane::ToFloatPtr( void ) {
	return reinterpret_cast<float *>(&a);
}

/*
================
idPlane::Type
================
*/
int olaPlane::Type( void ) const {
	if ( Normal()[0] == 0.0f ) {
		if ( Normal()[1] == 0.0f ) {
			return Normal()[2] > 0.0f ? PLANETYPE_Z : PLANETYPE_NEGZ;
		}
		else if ( Normal()[2] == 0.0f ) {
			return Normal()[1] > 0.0f ? PLANETYPE_Y : PLANETYPE_NEGY;
		}
		else {
			return PLANETYPE_ZEROX;
		}
	}
	else if ( Normal()[1] == 0.0f ) {
		if ( Normal()[2] == 0.0f ) {
			return Normal()[0] > 0.0f ? PLANETYPE_X : PLANETYPE_NEGX;
		}
		else {
			return PLANETYPE_ZEROY;
		}
	}
	else if ( Normal()[2] == 0.0f ) {
		return PLANETYPE_ZEROZ;
	}
	else {
		return PLANETYPE_NONAXIAL;
	}
}

/*
================
idPlane::HeightFit
================
*/
bool olaPlane::HeightFit( const olaVec3 *points, const int numPoints ) {
	int i;
	float sumXX = 0.0f, sumXY = 0.0f, sumXZ = 0.0f;
	float sumYY = 0.0f, sumYZ = 0.0f;
	olaVec3 sum, average, dir;

	if ( numPoints == 1 ) {
		a = 0.0f;
		b = 0.0f;
		c = 1.0f;
		d = -points[0].z;
		return true;
	}
	if ( numPoints == 2 ) {
		dir = points[1] - points[0];
		Normal() = dir.Cross( olaVec3( 0, 0, 1 ) ).Cross( dir );
		Normalize();
		d = -( Normal() * points[0] );
		return true;
	}

	sum.Zero();
	for ( i = 0; i < numPoints; i++) {
		sum += points[i];
	}
	average = sum / numPoints;

	for ( i = 0; i < numPoints; i++ ) {
		dir = points[i] - average;
		sumXX += dir.x * dir.x;
		sumXY += dir.x * dir.y;
		sumXZ += dir.x * dir.z;
		sumYY += dir.y * dir.y;
		sumYZ += dir.y * dir.z;
	}

	olaMat2 m( sumXX, sumXY, sumXY, sumYY );
	if ( !m.InverseSelf() ) {
		return false;
	}

	a = - sumXZ * m[0][0] - sumYZ * m[0][1];
	b = - sumXZ * m[1][0] - sumYZ * m[1][1];
	c = 1.0f;
	Normalize();
	d = -( a * average.x + b * average.y + c * average.z );
	return true;
}

/*
================
idPlane::PlaneIntersection
================
*/
bool olaPlane::PlaneIntersection( const olaPlane &plane, olaVec3 &start, olaVec3 &dir ) const {
	double n00, n01, n11, det, invDet, f0, f1;

	n00 = Normal().LengthSqr();
	n01 = Normal() * plane.Normal();
	n11 = plane.Normal().LengthSqr();
	det = n00 * n11 - n01 * n01;

	if ( olaMath::Fabs(det) < 1e-6f ) {
		return false;
	}

	invDet = 1.0f / det;
	f0 = ( n01 * plane.d - n11 * d ) * invDet;
	f1 = ( n01 * d - n00 * plane.d ) * invDet;

	dir = Normal().Cross( plane.Normal() );
	start = f0 * Normal() + f1 * plane.Normal();
	return true;
}

#pragma warning (pop)
