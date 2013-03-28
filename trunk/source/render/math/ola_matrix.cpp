// Copyright (C) 2004 Id Software, Inc.
//


#pragma hdrstop
#include "ola_vector.h"
#include "ola_matrix.h"
#include "ola_mathbase.h"
#include "ola_quat.h"
#include "ola_decompose.h"
#pragma warning( push )

#pragma warning(disable : 4100)				// unreferenced formal parameter
#pragma warning(disable : 4244)				// conversion to smaller type, possible loss of data
#pragma warning(disable : 4714)				// function marked as __forceinline not inlined
//#pragma warning(disable : 4311)				//  pointer truncation from 'void *' to 'int'
//#pragma warning(disable : 4312)				// conversion from 'int' to 'void *' of greater size
//#pragma warning(disable : 4267)				// conversion from 'size_t' to 'int', possible loss of data
//===============================================================
//
//	olaMat2
//
//===============================================================

olaMat2 mat2_zero( olaVec2( 0, 0 ), olaVec2( 0, 0 ) );
olaMat2 mat2_identity( olaVec2( 1, 0 ), olaVec2( 0, 1 ) );

/*
============
olaMat2::InverseSelf
============
*/
bool olaMat2::InverseSelf( void ) {
	// 2+4 = 6 multiplications
	//		 1 division
	double det, invDet, a;

	det = mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];

	if ( olaMath::Fabs( det ) < MATRIX_INVERSE_EPSILON ) {
		return false;
	}

	invDet = 1.0f / det;

	a = mat[0][0];
	mat[0][0] =   mat[1][1] * invDet;
	mat[0][1] = - mat[0][1] * invDet;
	mat[1][0] = - mat[1][0] * invDet;
	mat[1][1] =   a * invDet;

	return true;
}

/*
============
olaMat2::InverseFastSelf
============
*/
bool olaMat2::InverseFastSelf( void ) {
#if 1
	// 2+4 = 6 multiplications
	//		 1 division
	double det, invDet, a;

	det = mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];

	if ( olaMath::Fabs( det ) < MATRIX_INVERSE_EPSILON ) {
		return false;
	}

	invDet = 1.0f / det;

	a = mat[0][0];
	mat[0][0] =   mat[1][1] * invDet;
	mat[0][1] = - mat[0][1] * invDet;
	mat[1][0] = - mat[1][0] * invDet;
	mat[1][1] =   a * invDet;

	return true;
#else
	// 2*4 = 8 multiplications
	//		 2 division
	float *mat = reinterpret_cast<float *>(this);
	double d, di;
	float s;

	di = mat[0];
	s = di;
	mat[0*2+0] = d = 1.0f / di;
	mat[0*2+1] *= d;
	d = -d;
	mat[1*2+0] *= d;
	d = mat[1*2+0] * di;
	mat[1*2+1] += mat[0*2+1] * d;
	di = mat[1*2+1];
	s *= di;
	mat[1*2+1] = d = 1.0f / di;
	mat[1*2+0] *= d;
	d = -d;
	mat[0*2+1] *= d;
	d = mat[0*2+1] * di;
	mat[0*2+0] += mat[1*2+0] * d;

	return ( s != 0.0f && !FLOAT_IS_NAN( s ) );
#endif
}


olaMat2::olaMat2( void ) {
}

olaMat2::olaMat2( const olaVec2 &x, const olaVec2 &y ) {
	mat[ 0 ].x = x.x; mat[ 0 ].y = x.y;
	mat[ 1 ].x = y.x; mat[ 1 ].y = y.y;
}

olaMat2::olaMat2( const float xx, const float xy, const float yx, const float yy ) {
	mat[ 0 ].x = xx; mat[ 0 ].y = xy;
	mat[ 1 ].x = yx; mat[ 1 ].y = yy;
}

olaMat2::olaMat2( const float src[ 2 ][ 2 ] ) {
	memcpy( mat, src, 2 * 2 * sizeof( float ) );
}

const olaVec2 &olaMat2::operator[]( int index ) const {
	//assert( ( index >= 0 ) && ( index < 2 ) );
	return mat[ index ];
}

olaVec2 &olaMat2::operator[]( int index ) {
	//assert( ( index >= 0 ) && ( index < 2 ) );
	return mat[ index ];
}

olaMat2 olaMat2::operator-() const {
	return olaMat2(	-mat[0][0], -mat[0][1],
					-mat[1][0], -mat[1][1] );
}

olaVec2 olaMat2::operator*( const olaVec2 &vec ) const {
	return olaVec2(
		mat[ 0 ].x * vec.x + mat[ 0 ].y * vec.y,
		mat[ 1 ].x * vec.x + mat[ 1 ].y * vec.y );
}

olaMat2 olaMat2::operator*( const olaMat2 &a ) const {
	return olaMat2(
		mat[0].x * a[0].x + mat[0].y * a[1].x,
		mat[0].x * a[0].y + mat[0].y * a[1].y,
		mat[1].x * a[0].x + mat[1].y * a[1].x,
		mat[1].x * a[0].y + mat[1].y * a[1].y );
}

olaMat2 olaMat2::operator*( const float a ) const {
	return olaMat2(
		mat[0].x * a, mat[0].y * a, 
		mat[1].x * a, mat[1].y * a );
}

olaMat2 olaMat2::operator+( const olaMat2 &a ) const {
	return olaMat2(
		mat[0].x + a[0].x, mat[0].y + a[0].y, 
		mat[1].x + a[1].x, mat[1].y + a[1].y );
}
    
olaMat2 olaMat2::operator-( const olaMat2 &a ) const {
	return olaMat2(
		mat[0].x - a[0].x, mat[0].y - a[0].y,
		mat[1].x - a[1].x, mat[1].y - a[1].y );
}

olaMat2 &olaMat2::operator*=( const float a ) {
	mat[0].x *= a; mat[0].y *= a;
	mat[1].x *= a; mat[1].y *= a;

    return *this;
}

olaMat2 &olaMat2::operator*=( const olaMat2 &a ) {
	float x, y;
	x = mat[0].x; y = mat[0].y;
	mat[0].x = x * a[0].x + y * a[1].x;
	mat[0].y = x * a[0].y + y * a[1].y;
	x = mat[1].x; y = mat[1].y;
	mat[1].x = x * a[0].x + y * a[1].x;
	mat[1].y = x * a[0].y + y * a[1].y;
	return *this;
}

olaMat2 &olaMat2::operator+=( const olaMat2 &a ) {
	mat[0].x += a[0].x; mat[0].y += a[0].y;
	mat[1].x += a[1].x; mat[1].y += a[1].y;

    return *this;
}

olaMat2 &olaMat2::operator-=( const olaMat2 &a ) {
	mat[0].x -= a[0].x; mat[0].y -= a[0].y;
	mat[1].x -= a[1].x; mat[1].y -= a[1].y;

    return *this;
}

olaVec2 operator*( const olaVec2 &vec, const olaMat2 &mat ) {
	return mat * vec;
}

olaMat2 operator*( const float a, olaMat2 const &mat ) {
	return mat * a;
}

olaVec2 &operator*=( olaVec2 &vec, const olaMat2 &mat ) {
	vec = mat * vec;
	return vec;
}

bool olaMat2::Compare( const olaMat2 &a ) const {
	if ( mat[0].Compare( a[0] ) &&
		mat[1].Compare( a[1] ) ) {
		return true;
	}
	return false;
}

bool olaMat2::Compare( const olaMat2 &a, const float epsilon ) const {
	if ( mat[0].Compare( a[0], epsilon ) &&
		mat[1].Compare( a[1], epsilon ) ) {
		return true;
	}
	return false;
}

bool olaMat2::operator==( const olaMat2 &a ) const {
	return Compare( a );
}

bool olaMat2::operator!=( const olaMat2 &a ) const {
	return !Compare( a );
}

void olaMat2::Zero( void ) {
	mat[0].Zero();
	mat[1].Zero();
}

void olaMat2::Identity( void ) {
	*this = mat2_identity;
}

bool olaMat2::IsIdentity( const float epsilon ) const {
	return Compare( mat2_identity, epsilon );
}

bool olaMat2::IsSymmetric( const float epsilon ) const {
	return ( olaMath::Fabs( mat[0][1] - mat[1][0] ) < epsilon );
}

bool olaMat2::IsDiagonal( const float epsilon ) const {
	if ( olaMath::Fabs( mat[0][1] ) > epsilon ||
		olaMath::Fabs( mat[1][0] ) > epsilon ) {
		return false;
	}
	return true;
}

float olaMat2::Trace( void ) const {
	return ( mat[0][0] + mat[1][1] );
}

float olaMat2::Determinant( void ) const {
	return mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];
}

olaMat2 olaMat2::Transpose( void ) const {
	return olaMat2(	mat[0][0], mat[1][0],
					mat[0][1], mat[1][1] );
}

olaMat2 &olaMat2::TransposeSelf( void ) {
	float tmp;

	tmp = mat[0][1];
	mat[0][1] = mat[1][0];
	mat[1][0] = tmp;

	return *this;
}

olaMat2 olaMat2::Inverse( void ) const {
	olaMat2 invMat;

	invMat = *this;
	int r = invMat.InverseSelf();
	assert( r );
	return invMat;
}

olaMat2 olaMat2::InverseFast( void ) const {
	olaMat2 invMat;

	invMat = *this;
	int r = invMat.InverseFastSelf();
	assert( r );
	return invMat;
}

int olaMat2::GetDimension( void ) const {
	return 4;
}

const float *olaMat2::ToFloatPtr( void ) const {
	return mat[0].ToFloatPtr();
}

float *olaMat2::ToFloatPtr( void ) {
	return mat[0].ToFloatPtr();
}

//===============================================================
//
//	olaMat3
//
//===============================================================

olaMat3 mat3_zero( olaVec3( 0, 0, 0 ), olaVec3( 0, 0, 0 ), olaVec3( 0, 0, 0 ) );
olaMat3 mat3_identity( olaVec3( 1, 0, 0 ), olaVec3( 0, 1, 0 ), olaVec3( 0, 0, 1 ) );

/*
============
olaMat3::ToQuat
============
*/
olaQuat olaMat3::ToQuat( void ) const {
	olaQuat		q;
	float		trace;
	float		s;
	float		t;
	int     	i;
	int			j;
	int			k;

	static int 	next[ 3 ] = { 1, 2, 0 };

	trace = mat[ 0 ][ 0 ] + mat[ 1 ][ 1 ] + mat[ 2 ][ 2 ];

	if ( trace > 0.0f ) {

		t = trace + 1.0f;
		s = olaMath::InvSqrt( t ) * 0.5f;

		q[3] = s * t;
		q[0] = ( mat[ 2 ][ 1 ] - mat[ 1 ][ 2 ] ) * s;
		q[1] = ( mat[ 0 ][ 2 ] - mat[ 2 ][ 0 ] ) * s;
		q[2] = ( mat[ 1 ][ 0 ] - mat[ 0 ][ 1 ] ) * s;

	} else {

		i = 0;
		if ( mat[ 1 ][ 1 ] > mat[ 0 ][ 0 ] ) {
			i = 1;
		}
		if ( mat[ 2 ][ 2 ] > mat[ i ][ i ] ) {
			i = 2;
		}
		j = next[ i ];
		k = next[ j ];

		t = ( mat[ i ][ i ] - ( mat[ j ][ j ] + mat[ k ][ k ] ) ) + 1.0f;
		s = olaMath::InvSqrt( t ) * 0.5f;

		q[i] = s * t;
		q[3] = ( mat[ k ][ j ] - mat[ j ][ k ] ) * s;
		q[j] = ( mat[ j ][ i ] + mat[ i ][ j ] ) * s;
		q[k] = ( mat[ k ][ i ] + mat[ i ][ k ] ) * s;
	}
	return q;
}

/*
============
olaMat3::ToCQuat
============
*/
olaCQuat olaMat3::ToCQuat( void ) const {
	olaQuat q = ToQuat();
	if ( q.w < 0.0f ) {
		return olaCQuat( -q.x, -q.y, -q.z );
	}
	return olaCQuat( q.x, q.y, q.z );
}

/*
============
olaMat3::Determinant
============
*/
float olaMat3::Determinant( void ) const {

	float det2_12_01 = mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0];
	float det2_12_02 = mat[1][0] * mat[2][2] - mat[1][2] * mat[2][0];
	float det2_12_12 = mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1];

	return mat[0][0] * det2_12_12 - mat[0][1] * det2_12_02 + mat[0][2] * det2_12_01;
}

/*
============
olaMat3::InverseSelf
============
*/
bool olaMat3::InverseSelf( void ) {
	// 18+3+9 = 30 multiplications
	//			 1 division
	olaMat3 inverse;
	double det, invDet;

	inverse[0][0] = mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1];
	inverse[1][0] = mat[1][2] * mat[2][0] - mat[1][0] * mat[2][2];
	inverse[2][0] = mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0];

	det = mat[0][0] * inverse[0][0] + mat[0][1] * inverse[1][0] + mat[0][2] * inverse[2][0];

	if ( olaMath::Fabs( det ) < MATRIX_INVERSE_EPSILON ) {
		return false;
	}

	invDet = 1.0f / det;

	inverse[0][1] = mat[0][2] * mat[2][1] - mat[0][1] * mat[2][2];
	inverse[0][2] = mat[0][1] * mat[1][2] - mat[0][2] * mat[1][1];
	inverse[1][1] = mat[0][0] * mat[2][2] - mat[0][2] * mat[2][0];
	inverse[1][2] = mat[0][2] * mat[1][0] - mat[0][0] * mat[1][2];
	inverse[2][1] = mat[0][1] * mat[2][0] - mat[0][0] * mat[2][1];
	inverse[2][2] = mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];

	mat[0][0] = inverse[0][0] * invDet;
	mat[0][1] = inverse[0][1] * invDet;
	mat[0][2] = inverse[0][2] * invDet;

	mat[1][0] = inverse[1][0] * invDet;
	mat[1][1] = inverse[1][1] * invDet;
	mat[1][2] = inverse[1][2] * invDet;

	mat[2][0] = inverse[2][0] * invDet;
	mat[2][1] = inverse[2][1] * invDet;
	mat[2][2] = inverse[2][2] * invDet;

	return true;
}

/*
============
olaMat3::InverseFastSelf
============
*/
bool olaMat3::InverseFastSelf( void ) {
#if 1
	// 18+3+9 = 30 multiplications
	//			 1 division
	olaMat3 inverse;
	double det, invDet;

	inverse[0][0] = mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1];
	inverse[1][0] = mat[1][2] * mat[2][0] - mat[1][0] * mat[2][2];
	inverse[2][0] = mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0];

	det = mat[0][0] * inverse[0][0] + mat[0][1] * inverse[1][0] + mat[0][2] * inverse[2][0];

	if ( olaMath::Fabs( det ) < MATRIX_INVERSE_EPSILON ) {
		return false;
	}

	invDet = 1.0f / det;

	inverse[0][1] = mat[0][2] * mat[2][1] - mat[0][1] * mat[2][2];
	inverse[0][2] = mat[0][1] * mat[1][2] - mat[0][2] * mat[1][1];
	inverse[1][1] = mat[0][0] * mat[2][2] - mat[0][2] * mat[2][0];
	inverse[1][2] = mat[0][2] * mat[1][0] - mat[0][0] * mat[1][2];
	inverse[2][1] = mat[0][1] * mat[2][0] - mat[0][0] * mat[2][1];
	inverse[2][2] = mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];

	mat[0][0] = inverse[0][0] * invDet;
	mat[0][1] = inverse[0][1] * invDet;
	mat[0][2] = inverse[0][2] * invDet;

	mat[1][0] = inverse[1][0] * invDet;
	mat[1][1] = inverse[1][1] * invDet;
	mat[1][2] = inverse[1][2] * invDet;

	mat[2][0] = inverse[2][0] * invDet;
	mat[2][1] = inverse[2][1] * invDet;
	mat[2][2] = inverse[2][2] * invDet;

	return true;
#elif 0
	// 3*10 = 30 multiplications
	//		   3 divisions
	float *mat = reinterpret_cast<float *>(this);
	float s;
	double d, di;

	di = mat[0];
	s = di;
	mat[0] = d = 1.0f / di;
	mat[1] *= d;
	mat[2] *= d;
	d = -d;
	mat[3] *= d;
	mat[6] *= d;
	d = mat[3] * di;
	mat[4] += mat[1] * d;
	mat[5] += mat[2] * d;
	d = mat[6] * di;
	mat[7] += mat[1] * d;
	mat[8] += mat[2] * d;
	di = mat[4];
	s *= di;
	mat[4] = d = 1.0f / di;
	mat[3] *= d;
	mat[5] *= d;
	d = -d;
	mat[1] *= d;
	mat[7] *= d;
	d = mat[1] * di;
	mat[0] += mat[3] * d;
	mat[2] += mat[5] * d;
	d = mat[7] * di;
	mat[6] += mat[3] * d;
	mat[8] += mat[5] * d;
	di = mat[8];
	s *= di;
	mat[8] = d = 1.0f / di;
	mat[6] *= d;
	mat[7] *= d;
	d = -d;
	mat[2] *= d;
	mat[5] *= d;
	d = mat[2] * di;
	mat[0] += mat[6] * d;
	mat[1] += mat[7] * d;
	d = mat[5] * di;
	mat[3] += mat[6] * d;
	mat[4] += mat[7] * d;

	return ( s != 0.0f && !FLOAT_IS_NAN( s ) );
#else
	//	4*2+4*4 = 24 multiplications
	//		2*1 =  2 divisions
	olaMat2 r0;
	float r1[2], r2[2], r3;
	float det, invDet;
	float *mat = reinterpret_cast<float *>(this);

	// r0 = m0.Inverse();	// 2x2
	det = mat[0*3+0] * mat[1*3+1] - mat[0*3+1] * mat[1*3+0];

	if ( olaMath::Fabs( det ) < MATRIX_INVERSE_EPSILON ) {
		return false;
	}

	invDet = 1.0f / det;

	r0[0][0] =   mat[1*3+1] * invDet;
	r0[0][1] = - mat[0*3+1] * invDet;
	r0[1][0] = - mat[1*3+0] * invDet;
	r0[1][1] =   mat[0*3+0] * invDet;

	// r1 = r0 * m1;		// 2x1 = 2x2 * 2x1
	r1[0] = r0[0][0] * mat[0*3+2] + r0[0][1] * mat[1*3+2];
	r1[1] = r0[1][0] * mat[0*3+2] + r0[1][1] * mat[1*3+2];

	// r2 = m2 * r1;		// 1x1 = 1x2 * 2x1
	r2[0] = mat[2*3+0] * r1[0] + mat[2*3+1] * r1[1];

	// r3 = r2 - m3;		// 1x1 = 1x1 - 1x1
	r3 = r2[0] - mat[2*3+2];

	// r3.InverseSelf();
	if ( olaMath::Fabs( r3 ) < MATRIX_INVERSE_EPSILON ) {
		return false;
	}

	r3 = 1.0f / r3;

	// r2 = m2 * r0;		// 1x2 = 1x2 * 2x2
	r2[0] = mat[2*3+0] * r0[0][0] + mat[2*3+1] * r0[1][0];
	r2[1] = mat[2*3+0] * r0[0][1] + mat[2*3+1] * r0[1][1];

	// m2 = r3 * r2;		// 1x2 = 1x1 * 1x2
	mat[2*3+0] = r3 * r2[0];
	mat[2*3+1] = r3 * r2[1];

	// m0 = r0 - r1 * m2;	// 2x2 - 2x1 * 1x2
	mat[0*3+0] = r0[0][0] - r1[0] * mat[2*3+0];
	mat[0*3+1] = r0[0][1] - r1[0] * mat[2*3+1];
	mat[1*3+0] = r0[1][0] - r1[1] * mat[2*3+0];
	mat[1*3+1] = r0[1][1] - r1[1] * mat[2*3+1];

	// m1 = r1 * r3;		// 2x1 = 2x1 * 1x1
	mat[0*3+2] = r1[0] * r3;
	mat[1*3+2] = r1[1] * r3;

	// m3 = -r3;
	mat[2*3+2] = -r3;

	return true;
#endif
}

/*
============
olaMat3::InertiaTranslate
============
*/
olaMat3 olaMat3::InertiaTranslate( const float mass, const olaVec3 &centerOfMass, const olaVec3 &translation ) const {
	olaMat3 m;
	olaVec3 newCenter;

	newCenter = centerOfMass + translation;

	m[0][0] = mass * ( ( centerOfMass[1] * centerOfMass[1] + centerOfMass[2] * centerOfMass[2] )
				- ( newCenter[1] * newCenter[1] + newCenter[2] * newCenter[2] ) );
	m[1][1] = mass * ( ( centerOfMass[0] * centerOfMass[0] + centerOfMass[2] * centerOfMass[2] )
				- ( newCenter[0] * newCenter[0] + newCenter[2] * newCenter[2] ) );
	m[2][2] = mass * ( ( centerOfMass[0] * centerOfMass[0] + centerOfMass[1] * centerOfMass[1] )
				- ( newCenter[0] * newCenter[0] + newCenter[1] * newCenter[1] ) );

	m[0][1] = m[1][0] = mass * ( newCenter[0] * newCenter[1] - centerOfMass[0] * centerOfMass[1] );
	m[1][2] = m[2][1] = mass * ( newCenter[1] * newCenter[2] - centerOfMass[1] * centerOfMass[2] );
	m[0][2] = m[2][0] = mass * ( newCenter[0] * newCenter[2] - centerOfMass[0] * centerOfMass[2] );

	return (*this) + m;
}

/*
============
olaMat3::InertiaTranslateSelf
============
*/
olaMat3 &olaMat3::InertiaTranslateSelf( const float mass, const olaVec3 &centerOfMass, const olaVec3 &translation ) {
	olaMat3 m;
	olaVec3 newCenter;

	newCenter = centerOfMass + translation;

	m[0][0] = mass * ( ( centerOfMass[1] * centerOfMass[1] + centerOfMass[2] * centerOfMass[2] )
				- ( newCenter[1] * newCenter[1] + newCenter[2] * newCenter[2] ) );
	m[1][1] = mass * ( ( centerOfMass[0] * centerOfMass[0] + centerOfMass[2] * centerOfMass[2] )
				- ( newCenter[0] * newCenter[0] + newCenter[2] * newCenter[2] ) );
	m[2][2] = mass * ( ( centerOfMass[0] * centerOfMass[0] + centerOfMass[1] * centerOfMass[1] )
				- ( newCenter[0] * newCenter[0] + newCenter[1] * newCenter[1] ) );

	m[0][1] = m[1][0] = mass * ( newCenter[0] * newCenter[1] - centerOfMass[0] * centerOfMass[1] );
	m[1][2] = m[2][1] = mass * ( newCenter[1] * newCenter[2] - centerOfMass[1] * centerOfMass[2] );
	m[0][2] = m[2][0] = mass * ( newCenter[0] * newCenter[2] - centerOfMass[0] * centerOfMass[2] );

	(*this) += m;

	return (*this);
}

/*
============
olaMat3::InertiaRotate
============
*/
olaMat3 olaMat3::InertiaRotate( const olaMat3 &rotation ) const {
	// NOTE: the rotation matrix is stored column-major
	return rotation.Transpose() * (*this) * rotation;
}

/*
============
olaMat3::InertiaRotateSelf
============
*/
olaMat3 &olaMat3::InertiaRotateSelf( const olaMat3 &rotation ) {
	// NOTE: the rotation matrix is stored column-major
	*this = rotation.Transpose() * (*this) * rotation;
	return *this;
}


olaMat3::olaMat3( void ) {
}

olaMat3::olaMat3( const olaVec3 &x, const olaVec3 &y, const olaVec3 &z ) {
	mat[ 0 ].x = x.x; mat[ 0 ].y = x.y; mat[ 0 ].z = x.z;
	mat[ 1 ].x = y.x; mat[ 1 ].y = y.y; mat[ 1 ].z = y.z;
	mat[ 2 ].x = z.x; mat[ 2 ].y = z.y; mat[ 2 ].z = z.z;
}

olaMat3::olaMat3( const float xx, const float xy, const float xz, const float yx, const float yy, const float yz, const float zx, const float zy, const float zz ) {
	mat[ 0 ].x = xx; mat[ 0 ].y = xy; mat[ 0 ].z = xz;
	mat[ 1 ].x = yx; mat[ 1 ].y = yy; mat[ 1 ].z = yz;
	mat[ 2 ].x = zx; mat[ 2 ].y = zy; mat[ 2 ].z = zz;
}

olaMat3::olaMat3( const float src[ 3 ][ 3 ] ) {
	memcpy( mat, src, 3 * 3 * sizeof( float ) );
}

const olaVec3 &olaMat3::operator[]( int index ) const {
	//assert( ( index >= 0 ) && ( index < 3 ) );
	return mat[ index ];
}

olaVec3 &olaMat3::operator[]( int index ) {
	//assert( ( index >= 0 ) && ( index < 3 ) );
	return mat[ index ];
}

olaMat3 olaMat3::operator-() const {
	return olaMat3(	-mat[0][0], -mat[0][1], -mat[0][2],
					-mat[1][0], -mat[1][1], -mat[1][2],
					-mat[2][0], -mat[2][1], -mat[2][2] );
}

olaVec3 olaMat3::operator*( const olaVec3 &vec ) const {
	return olaVec3(
		mat[ 0 ].x * vec.x + mat[ 1 ].x * vec.y + mat[ 2 ].x * vec.z,
		mat[ 0 ].y * vec.x + mat[ 1 ].y * vec.y + mat[ 2 ].y * vec.z,
		mat[ 0 ].z * vec.x + mat[ 1 ].z * vec.y + mat[ 2 ].z * vec.z );
}

olaMat3 olaMat3::operator*( const olaMat3 &a ) const {
	int i, j;
	const float *m1Ptr, *m2Ptr;
	float *dstPtr;
	olaMat3 dst;

	m1Ptr = reinterpret_cast<const float *>(this);
	m2Ptr = reinterpret_cast<const float *>(&a);
	dstPtr = reinterpret_cast<float *>(&dst);

	for ( i = 0; i < 3; i++ ) {
		for ( j = 0; j < 3; j++ ) {
			*dstPtr = m1Ptr[0] * m2Ptr[ 0 * 3 + j ]
					+ m1Ptr[1] * m2Ptr[ 1 * 3 + j ]
					+ m1Ptr[2] * m2Ptr[ 2 * 3 + j ];
			dstPtr++;
		}
		m1Ptr += 3;
	}
	return dst;
}

olaMat3 olaMat3::operator*( const float a ) const {
	return olaMat3(
		mat[0].x * a, mat[0].y * a, mat[0].z * a,
		mat[1].x * a, mat[1].y * a, mat[1].z * a,
		mat[2].x * a, mat[2].y * a, mat[2].z * a );
}

olaMat3 olaMat3::operator+( const olaMat3 &a ) const {
	return olaMat3(
		mat[0].x + a[0].x, mat[0].y + a[0].y, mat[0].z + a[0].z,
		mat[1].x + a[1].x, mat[1].y + a[1].y, mat[1].z + a[1].z,
		mat[2].x + a[2].x, mat[2].y + a[2].y, mat[2].z + a[2].z );
}
    
olaMat3 olaMat3::operator-( const olaMat3 &a ) const {
	return olaMat3(
		mat[0].x - a[0].x, mat[0].y - a[0].y, mat[0].z - a[0].z,
		mat[1].x - a[1].x, mat[1].y - a[1].y, mat[1].z - a[1].z,
		mat[2].x - a[2].x, mat[2].y - a[2].y, mat[2].z - a[2].z );
}

olaMat3 &olaMat3::operator*=( const float a ) {
	mat[0].x *= a; mat[0].y *= a; mat[0].z *= a;
	mat[1].x *= a; mat[1].y *= a; mat[1].z *= a; 
	mat[2].x *= a; mat[2].y *= a; mat[2].z *= a;

    return *this;
}

olaMat3 &olaMat3::operator*=( const olaMat3 &a ) {
	int i, j;
	const float *m2Ptr;
	float *m1Ptr, dst[3];

	m1Ptr = reinterpret_cast<float *>(this);
	m2Ptr = reinterpret_cast<const float *>(&a);

	for ( i = 0; i < 3; i++ ) {
		for ( j = 0; j < 3; j++ ) {
			dst[j]  = m1Ptr[0] * m2Ptr[ 0 * 3 + j ]
					+ m1Ptr[1] * m2Ptr[ 1 * 3 + j ]
					+ m1Ptr[2] * m2Ptr[ 2 * 3 + j ];
		}
		m1Ptr[0] = dst[0]; m1Ptr[1] = dst[1]; m1Ptr[2] = dst[2];
		m1Ptr += 3;
	}
	return *this;
}

olaMat3 &olaMat3::operator+=( const olaMat3 &a ) {
	mat[0].x += a[0].x; mat[0].y += a[0].y; mat[0].z += a[0].z;
	mat[1].x += a[1].x; mat[1].y += a[1].y; mat[1].z += a[1].z;
	mat[2].x += a[2].x; mat[2].y += a[2].y; mat[2].z += a[2].z;

    return *this;
}

olaMat3 &olaMat3::operator-=( const olaMat3 &a ) {
	mat[0].x -= a[0].x; mat[0].y -= a[0].y; mat[0].z -= a[0].z;
	mat[1].x -= a[1].x; mat[1].y -= a[1].y; mat[1].z -= a[1].z;
	mat[2].x -= a[2].x; mat[2].y -= a[2].y; mat[2].z -= a[2].z;

    return *this;
}

olaVec3 operator*( const olaVec3 &vec, const olaMat3 &mat ) {
	return mat * vec;
}

olaMat3 operator*( const float a, const olaMat3 &mat ) {
	return mat * a;
}

olaVec3 &operator*=( olaVec3 &vec, const olaMat3 &mat ) {
	float x = mat[ 0 ].x * vec.x + mat[ 1 ].x * vec.y + mat[ 2 ].x * vec.z;
	float y = mat[ 0 ].y * vec.x + mat[ 1 ].y * vec.y + mat[ 2 ].y * vec.z;
	vec.z = mat[ 0 ].z * vec.x + mat[ 1 ].z * vec.y + mat[ 2 ].z * vec.z;
	vec.x = x;
	vec.y = y;
	return vec;
}

bool olaMat3::Compare( const olaMat3 &a ) const {
	if ( mat[0].Compare( a[0] ) &&
		mat[1].Compare( a[1] ) &&
		mat[2].Compare( a[2] ) ) {
		return true;
	}
	return false;
}

bool olaMat3::Compare( const olaMat3 &a, const float epsilon ) const {
	if ( mat[0].Compare( a[0], epsilon ) &&
		mat[1].Compare( a[1], epsilon ) &&
		mat[2].Compare( a[2], epsilon ) ) {
		return true;
	}
	return false;
}

bool olaMat3::operator==( const olaMat3 &a ) const {
	return Compare( a );
}

bool olaMat3::operator!=( const olaMat3 &a ) const {
	return !Compare( a );
}

void olaMat3::Zero( void ) {
	memset( mat, 0, sizeof( olaMat3 ) );
}

void olaMat3::Identity( void ) {
	*this = mat3_identity;
}

bool olaMat3::IsIdentity( const float epsilon ) const {
	return Compare( mat3_identity, epsilon );
}

bool olaMat3::IsSymmetric( const float epsilon ) const {
	if ( olaMath::Fabs( mat[0][1] - mat[1][0] ) > epsilon ) {
		return false;
	}
	if ( olaMath::Fabs( mat[0][2] - mat[2][0] ) > epsilon ) {
		return false;
	}
	if ( olaMath::Fabs( mat[1][2] - mat[2][1] ) > epsilon ) {
		return false;
	}
	return true;
}

bool olaMat3::IsDiagonal( const float epsilon ) const {
	if ( olaMath::Fabs( mat[0][1] ) > epsilon ||
		olaMath::Fabs( mat[0][2] ) > epsilon ||
		olaMath::Fabs( mat[1][0] ) > epsilon ||
		olaMath::Fabs( mat[1][2] ) > epsilon ||
		olaMath::Fabs( mat[2][0] ) > epsilon ||
		olaMath::Fabs( mat[2][1] ) > epsilon ) {
		return false;
	}
	return true;
}

bool olaMat3::IsRotated( void ) const {
	return !Compare( mat3_identity );
}

void olaMat3::ProjectVector( const olaVec3 &src, olaVec3 &dst ) const {
	dst.x = src * mat[ 0 ];
	dst.y = src * mat[ 1 ];
	dst.z = src * mat[ 2 ];
}

void olaMat3::UnprojectVector( const olaVec3 &src, olaVec3 &dst ) const {
	dst = mat[ 0 ] * src.x + mat[ 1 ] * src.y + mat[ 2 ] * src.z;
}

bool olaMat3::FixDegeneracies( void ) {
	bool r = mat[0].FixDegenerateNormal();
	r |= mat[1].FixDegenerateNormal();
	r |= mat[2].FixDegenerateNormal();
	return r;
}

bool olaMat3::FixDenormals( void ) {
	bool r = mat[0].FixDenormals();
	r |= mat[1].FixDenormals();
	r |= mat[2].FixDenormals();
	return r;
}

float olaMat3::Trace( void ) const {
	return ( mat[0][0] + mat[1][1] + mat[2][2] );
}

olaMat3 olaMat3::OrthoNormalize( void ) const {
	olaMat3 ortho;

	ortho = *this;
	ortho[ 0 ].Normalize();
	ortho[ 2 ].Cross( mat[ 0 ], mat[ 1 ] );
	ortho[ 2 ].Normalize();
	ortho[ 1 ].Cross( mat[ 2 ], mat[ 0 ] );
	ortho[ 1 ].Normalize();
	return ortho;
}

olaMat3 &olaMat3::OrthoNormalizeSelf( void ) {
	mat[ 0 ].Normalize();
	mat[ 2 ].Cross( mat[ 0 ], mat[ 1 ] );
	mat[ 2 ].Normalize();
	mat[ 1 ].Cross( mat[ 2 ], mat[ 0 ] );
	mat[ 1 ].Normalize();
	return *this;
}

olaMat3 olaMat3::Transpose( void ) const {
	return olaMat3(	mat[0][0], mat[1][0], mat[2][0],
					mat[0][1], mat[1][1], mat[2][1],
					mat[0][2], mat[1][2], mat[2][2] );
}

olaMat3 &olaMat3::TransposeSelf( void ) {
	float tmp0, tmp1, tmp2;

	tmp0 = mat[0][1];
	mat[0][1] = mat[1][0];
	mat[1][0] = tmp0;
	tmp1 = mat[0][2];
	mat[0][2] = mat[2][0];
	mat[2][0] = tmp1;
	tmp2 = mat[1][2];
	mat[1][2] = mat[2][1];
	mat[2][1] = tmp2;

	return *this;
}

olaMat3 olaMat3::Inverse( void ) const {
	olaMat3 invMat;

	invMat = *this;
	int r = invMat.InverseSelf();
	assert( r );
	return invMat;
}

olaMat3 olaMat3::InverseFast( void ) const {
	olaMat3 invMat;

	invMat = *this;
	int r = invMat.InverseFastSelf();
	assert( r );
	return invMat;
}

olaMat3 olaMat3::TransposeMultiply( const olaMat3 &b ) const {
	return olaMat3(	mat[0].x * b[0].x + mat[1].x * b[1].x + mat[2].x * b[2].x,
					mat[0].x * b[0].y + mat[1].x * b[1].y + mat[2].x * b[2].y,
					mat[0].x * b[0].z + mat[1].x * b[1].z + mat[2].x * b[2].z,
					mat[0].y * b[0].x + mat[1].y * b[1].x + mat[2].y * b[2].x,
					mat[0].y * b[0].y + mat[1].y * b[1].y + mat[2].y * b[2].y,
					mat[0].y * b[0].z + mat[1].y * b[1].z + mat[2].y * b[2].z,
					mat[0].z * b[0].x + mat[1].z * b[1].x + mat[2].z * b[2].x,
					mat[0].z * b[0].y + mat[1].z * b[1].y + mat[2].z * b[2].y,
					mat[0].z * b[0].z + mat[1].z * b[1].z + mat[2].z * b[2].z );
}

void TransposeMultiply( const olaMat3 &transpose, const olaMat3 &b, olaMat3 &dst ) {
	dst[0].x = transpose[0].x * b[0].x + transpose[1].x * b[1].x + transpose[2].x * b[2].x;
	dst[0].y = transpose[0].x * b[0].y + transpose[1].x * b[1].y + transpose[2].x * b[2].y;
	dst[0].z = transpose[0].x * b[0].z + transpose[1].x * b[1].z + transpose[2].x * b[2].z;
	dst[1].x = transpose[0].y * b[0].x + transpose[1].y * b[1].x + transpose[2].y * b[2].x;
	dst[1].y = transpose[0].y * b[0].y + transpose[1].y * b[1].y + transpose[2].y * b[2].y;
	dst[1].z = transpose[0].y * b[0].z + transpose[1].y * b[1].z + transpose[2].y * b[2].z;
	dst[2].x = transpose[0].z * b[0].x + transpose[1].z * b[1].x + transpose[2].z * b[2].x;
	dst[2].y = transpose[0].z * b[0].y + transpose[1].z * b[1].y + transpose[2].z * b[2].y;
	dst[2].z = transpose[0].z * b[0].z + transpose[1].z * b[1].z + transpose[2].z * b[2].z;
}

olaMat3 SkewSymmetric( olaVec3 const &src ) {
	return olaMat3( 0.0f, -src.z,  src.y, src.z,   0.0f, -src.x, -src.y,  src.x,   0.0f );
}

int olaMat3::GetDimension( void ) const {
	return 9;
}

const float *olaMat3::ToFloatPtr( void ) const {
	return mat[0].ToFloatPtr();
}

float *olaMat3::ToFloatPtr( void ) {
	return mat[0].ToFloatPtr();
}

void olaMat3::CopyToFloatArrayColumn(float* m33)
{
	//m33[0]  = mat[0][0];
	//m33[1]  = mat[0][1];
	//m33[2]  = mat[0][2];

	//m33[4]  = mat[1][0];
	//m33[5]  = mat[1][1];
	//m33[6]  = mat[1][2];

	//m33[8]  = mat[2][0];
	//m33[9]  = mat[2][1];
	//m33[10] = mat[2][2];

	m33[0]  = mat[0][0];
	m33[1]  = mat[1][0];
	m33[2]  = mat[2][0];

	m33[4]  = mat[0][1];
	m33[5]  = mat[1][1];
	m33[6]  = mat[2][1];

	m33[8]  = mat[0][2];
	m33[9]  = mat[1][2];
	m33[10] = mat[2][2];

/*	m33[12] = mat[0][3];
	m33[13] = mat[1][3];
	m33[14] = mat[2][3];*/	
}

//===============================================================
//
//	olaMat4
//
//===============================================================

olaMat4 mat4_zero( olaVec4( 0, 0, 0, 0 ), olaVec4( 0, 0, 0, 0 ), olaVec4( 0, 0, 0, 0 ), olaVec4( 0, 0, 0, 0 ) );
olaMat4 mat4_identity( olaVec4( 1, 0, 0, 0 ), olaVec4( 0, 1, 0, 0 ), olaVec4( 0, 0, 1, 0 ), olaVec4( 0, 0, 0, 1 ) );

/*
============
olaMat4::Transpose
============
*/
olaMat4 olaMat4::Transpose( void ) const {
	olaMat4	transpose;
	int		i, j;
   
	for( i = 0; i < 4; i++ ) {
		for( j = 0; j < 4; j++ ) {
			transpose[ i ][ j ] = mat[ j ][ i ];
        }
	}
	return transpose;
}

/*
============
olaMat4::TransposeSelf
============
*/
olaMat4 &olaMat4::TransposeSelf( void ) {
	float	temp;
	int		i, j;
   
	for( i = 0; i < 4; i++ ) {
		for( j = i + 1; j < 4; j++ ) {
			temp = mat[ i ][ j ];
			mat[ i ][ j ] = mat[ j ][ i ];
			mat[ j ][ i ] = temp;
        }
	}
	return *this;
}

/*
============
olaMat4::Determinant
============
*/
float olaMat4::Determinant( void ) const {

	// 2x2 sub-determinants
	float det2_01_01 = mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];
	float det2_01_02 = mat[0][0] * mat[1][2] - mat[0][2] * mat[1][0];
	float det2_01_03 = mat[0][0] * mat[1][3] - mat[0][3] * mat[1][0];
	float det2_01_12 = mat[0][1] * mat[1][2] - mat[0][2] * mat[1][1];
	float det2_01_13 = mat[0][1] * mat[1][3] - mat[0][3] * mat[1][1];
	float det2_01_23 = mat[0][2] * mat[1][3] - mat[0][3] * mat[1][2];

	// 3x3 sub-determinants
	float det3_201_012 = mat[2][0] * det2_01_12 - mat[2][1] * det2_01_02 + mat[2][2] * det2_01_01;
	float det3_201_013 = mat[2][0] * det2_01_13 - mat[2][1] * det2_01_03 + mat[2][3] * det2_01_01;
	float det3_201_023 = mat[2][0] * det2_01_23 - mat[2][2] * det2_01_03 + mat[2][3] * det2_01_02;
	float det3_201_123 = mat[2][1] * det2_01_23 - mat[2][2] * det2_01_13 + mat[2][3] * det2_01_12;

	return ( - det3_201_123 * mat[3][0] + det3_201_023 * mat[3][1] - det3_201_013 * mat[3][2] + det3_201_012 * mat[3][3] );
}

/*
============
olaMat4::InverseSelf
============
*/
bool olaMat4::InverseSelf( void ) {
	// 84+4+16 = 104 multiplications
	//			   1 division
	double det, invDet;

	// 2x2 sub-determinants required to calculate 4x4 determinant
	float det2_01_01 = mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];
	float det2_01_02 = mat[0][0] * mat[1][2] - mat[0][2] * mat[1][0];
	float det2_01_03 = mat[0][0] * mat[1][3] - mat[0][3] * mat[1][0];
	float det2_01_12 = mat[0][1] * mat[1][2] - mat[0][2] * mat[1][1];
	float det2_01_13 = mat[0][1] * mat[1][3] - mat[0][3] * mat[1][1];
	float det2_01_23 = mat[0][2] * mat[1][3] - mat[0][3] * mat[1][2];

	// 3x3 sub-determinants required to calculate 4x4 determinant
	float det3_201_012 = mat[2][0] * det2_01_12 - mat[2][1] * det2_01_02 + mat[2][2] * det2_01_01;
	float det3_201_013 = mat[2][0] * det2_01_13 - mat[2][1] * det2_01_03 + mat[2][3] * det2_01_01;
	float det3_201_023 = mat[2][0] * det2_01_23 - mat[2][2] * det2_01_03 + mat[2][3] * det2_01_02;
	float det3_201_123 = mat[2][1] * det2_01_23 - mat[2][2] * det2_01_13 + mat[2][3] * det2_01_12;

	det = ( - det3_201_123 * mat[3][0] + det3_201_023 * mat[3][1] - det3_201_013 * mat[3][2] + det3_201_012 * mat[3][3] );

	if ( olaMath::Fabs( det ) < MATRIX_INVERSE_EPSILON ) {
		return false;
	}

	invDet = 1.0f / det;

	// remaining 2x2 sub-determinants
	float det2_03_01 = mat[0][0] * mat[3][1] - mat[0][1] * mat[3][0];
	float det2_03_02 = mat[0][0] * mat[3][2] - mat[0][2] * mat[3][0];
	float det2_03_03 = mat[0][0] * mat[3][3] - mat[0][3] * mat[3][0];
	float det2_03_12 = mat[0][1] * mat[3][2] - mat[0][2] * mat[3][1];
	float det2_03_13 = mat[0][1] * mat[3][3] - mat[0][3] * mat[3][1];
	float det2_03_23 = mat[0][2] * mat[3][3] - mat[0][3] * mat[3][2];

	float det2_13_01 = mat[1][0] * mat[3][1] - mat[1][1] * mat[3][0];
	float det2_13_02 = mat[1][0] * mat[3][2] - mat[1][2] * mat[3][0];
	float det2_13_03 = mat[1][0] * mat[3][3] - mat[1][3] * mat[3][0];
	float det2_13_12 = mat[1][1] * mat[3][2] - mat[1][2] * mat[3][1];
	float det2_13_13 = mat[1][1] * mat[3][3] - mat[1][3] * mat[3][1];
	float det2_13_23 = mat[1][2] * mat[3][3] - mat[1][3] * mat[3][2];

	// remaining 3x3 sub-determinants
	float det3_203_012 = mat[2][0] * det2_03_12 - mat[2][1] * det2_03_02 + mat[2][2] * det2_03_01;
	float det3_203_013 = mat[2][0] * det2_03_13 - mat[2][1] * det2_03_03 + mat[2][3] * det2_03_01;
	float det3_203_023 = mat[2][0] * det2_03_23 - mat[2][2] * det2_03_03 + mat[2][3] * det2_03_02;
	float det3_203_123 = mat[2][1] * det2_03_23 - mat[2][2] * det2_03_13 + mat[2][3] * det2_03_12;

	float det3_213_012 = mat[2][0] * det2_13_12 - mat[2][1] * det2_13_02 + mat[2][2] * det2_13_01;
	float det3_213_013 = mat[2][0] * det2_13_13 - mat[2][1] * det2_13_03 + mat[2][3] * det2_13_01;
	float det3_213_023 = mat[2][0] * det2_13_23 - mat[2][2] * det2_13_03 + mat[2][3] * det2_13_02;
	float det3_213_123 = mat[2][1] * det2_13_23 - mat[2][2] * det2_13_13 + mat[2][3] * det2_13_12;

	float det3_301_012 = mat[3][0] * det2_01_12 - mat[3][1] * det2_01_02 + mat[3][2] * det2_01_01;
	float det3_301_013 = mat[3][0] * det2_01_13 - mat[3][1] * det2_01_03 + mat[3][3] * det2_01_01;
	float det3_301_023 = mat[3][0] * det2_01_23 - mat[3][2] * det2_01_03 + mat[3][3] * det2_01_02;
	float det3_301_123 = mat[3][1] * det2_01_23 - mat[3][2] * det2_01_13 + mat[3][3] * det2_01_12;

	mat[0][0] =	- det3_213_123 * invDet;
	mat[1][0] = + det3_213_023 * invDet;
	mat[2][0] = - det3_213_013 * invDet;
	mat[3][0] = + det3_213_012 * invDet;

	mat[0][1] = + det3_203_123 * invDet;
	mat[1][1] = - det3_203_023 * invDet;
	mat[2][1] = + det3_203_013 * invDet;
	mat[3][1] = - det3_203_012 * invDet;

	mat[0][2] = + det3_301_123 * invDet;
	mat[1][2] = - det3_301_023 * invDet;
	mat[2][2] = + det3_301_013 * invDet;
	mat[3][2] = - det3_301_012 * invDet;

	mat[0][3] = - det3_201_123 * invDet;
	mat[1][3] = + det3_201_023 * invDet;
	mat[2][3] = - det3_201_013 * invDet;
	mat[3][3] = + det3_201_012 * invDet;

	return true;
}

/*
============
olaMat4::InverseFastSelf
============
*/
bool olaMat4::InverseFastSelf( void ) {
#if 0
	// 84+4+16 = 104 multiplications
	//			   1 division
	double det, invDet;

	// 2x2 sub-determinants required to calculate 4x4 determinant
	float det2_01_01 = mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];
	float det2_01_02 = mat[0][0] * mat[1][2] - mat[0][2] * mat[1][0];
	float det2_01_03 = mat[0][0] * mat[1][3] - mat[0][3] * mat[1][0];
	float det2_01_12 = mat[0][1] * mat[1][2] - mat[0][2] * mat[1][1];
	float det2_01_13 = mat[0][1] * mat[1][3] - mat[0][3] * mat[1][1];
	float det2_01_23 = mat[0][2] * mat[1][3] - mat[0][3] * mat[1][2];

	// 3x3 sub-determinants required to calculate 4x4 determinant
	float det3_201_012 = mat[2][0] * det2_01_12 - mat[2][1] * det2_01_02 + mat[2][2] * det2_01_01;
	float det3_201_013 = mat[2][0] * det2_01_13 - mat[2][1] * det2_01_03 + mat[2][3] * det2_01_01;
	float det3_201_023 = mat[2][0] * det2_01_23 - mat[2][2] * det2_01_03 + mat[2][3] * det2_01_02;
	float det3_201_123 = mat[2][1] * det2_01_23 - mat[2][2] * det2_01_13 + mat[2][3] * det2_01_12;

	det = ( - det3_201_123 * mat[3][0] + det3_201_023 * mat[3][1] - det3_201_013 * mat[3][2] + det3_201_012 * mat[3][3] );

	if ( olaMath::Fabs( det ) < MATRIX_INVERSE_EPSILON ) {
		return false;
	}

	invDet = 1.0f / det;

	// remaining 2x2 sub-determinants
	float det2_03_01 = mat[0][0] * mat[3][1] - mat[0][1] * mat[3][0];
	float det2_03_02 = mat[0][0] * mat[3][2] - mat[0][2] * mat[3][0];
	float det2_03_03 = mat[0][0] * mat[3][3] - mat[0][3] * mat[3][0];
	float det2_03_12 = mat[0][1] * mat[3][2] - mat[0][2] * mat[3][1];
	float det2_03_13 = mat[0][1] * mat[3][3] - mat[0][3] * mat[3][1];
	float det2_03_23 = mat[0][2] * mat[3][3] - mat[0][3] * mat[3][2];

	float det2_13_01 = mat[1][0] * mat[3][1] - mat[1][1] * mat[3][0];
	float det2_13_02 = mat[1][0] * mat[3][2] - mat[1][2] * mat[3][0];
	float det2_13_03 = mat[1][0] * mat[3][3] - mat[1][3] * mat[3][0];
	float det2_13_12 = mat[1][1] * mat[3][2] - mat[1][2] * mat[3][1];
	float det2_13_13 = mat[1][1] * mat[3][3] - mat[1][3] * mat[3][1];
	float det2_13_23 = mat[1][2] * mat[3][3] - mat[1][3] * mat[3][2];

	// remaining 3x3 sub-determinants
	float det3_203_012 = mat[2][0] * det2_03_12 - mat[2][1] * det2_03_02 + mat[2][2] * det2_03_01;
	float det3_203_013 = mat[2][0] * det2_03_13 - mat[2][1] * det2_03_03 + mat[2][3] * det2_03_01;
	float det3_203_023 = mat[2][0] * det2_03_23 - mat[2][2] * det2_03_03 + mat[2][3] * det2_03_02;
	float det3_203_123 = mat[2][1] * det2_03_23 - mat[2][2] * det2_03_13 + mat[2][3] * det2_03_12;

	float det3_213_012 = mat[2][0] * det2_13_12 - mat[2][1] * det2_13_02 + mat[2][2] * det2_13_01;
	float det3_213_013 = mat[2][0] * det2_13_13 - mat[2][1] * det2_13_03 + mat[2][3] * det2_13_01;
	float det3_213_023 = mat[2][0] * det2_13_23 - mat[2][2] * det2_13_03 + mat[2][3] * det2_13_02;
	float det3_213_123 = mat[2][1] * det2_13_23 - mat[2][2] * det2_13_13 + mat[2][3] * det2_13_12;

	float det3_301_012 = mat[3][0] * det2_01_12 - mat[3][1] * det2_01_02 + mat[3][2] * det2_01_01;
	float det3_301_013 = mat[3][0] * det2_01_13 - mat[3][1] * det2_01_03 + mat[3][3] * det2_01_01;
	float det3_301_023 = mat[3][0] * det2_01_23 - mat[3][2] * det2_01_03 + mat[3][3] * det2_01_02;
	float det3_301_123 = mat[3][1] * det2_01_23 - mat[3][2] * det2_01_13 + mat[3][3] * det2_01_12;

	mat[0][0] =	- det3_213_123 * invDet;
	mat[1][0] = + det3_213_023 * invDet;
	mat[2][0] = - det3_213_013 * invDet;
	mat[3][0] = + det3_213_012 * invDet;

	mat[0][1] = + det3_203_123 * invDet;
	mat[1][1] = - det3_203_023 * invDet;
	mat[2][1] = + det3_203_013 * invDet;
	mat[3][1] = - det3_203_012 * invDet;

	mat[0][2] = + det3_301_123 * invDet;
	mat[1][2] = - det3_301_023 * invDet;
	mat[2][2] = + det3_301_013 * invDet;
	mat[3][2] = - det3_301_012 * invDet;

	mat[0][3] = - det3_201_123 * invDet;
	mat[1][3] = + det3_201_023 * invDet;
	mat[2][3] = - det3_201_013 * invDet;
	mat[3][3] = + det3_201_012 * invDet;

	return true;
#elif 0
	// 4*18 = 72 multiplications
	//		   4 divisions
	float *mat = reinterpret_cast<float *>(this);
	float s;
	double d, di;

	di = mat[0];
	s = di;
	mat[0] = d = 1.0f / di;
	mat[1] *= d;
	mat[2] *= d;
	mat[3] *= d;
	d = -d;
	mat[4] *= d;
	mat[8] *= d;
	mat[12] *= d;
	d = mat[4] * di;
	mat[5] += mat[1] * d;
	mat[6] += mat[2] * d;
	mat[7] += mat[3] * d;
	d = mat[8] * di;
	mat[9] += mat[1] * d;
	mat[10] += mat[2] * d;
	mat[11] += mat[3] * d;
	d = mat[12] * di;
	mat[13] += mat[1] * d;
	mat[14] += mat[2] * d;
	mat[15] += mat[3] * d;
	di = mat[5];
	s *= di;
	mat[5] = d = 1.0f / di;
	mat[4] *= d;
	mat[6] *= d;
	mat[7] *= d;
	d = -d;
	mat[1] *= d;
	mat[9] *= d;
	mat[13] *= d;
	d = mat[1] * di;
	mat[0] += mat[4] * d;
	mat[2] += mat[6] * d;
	mat[3] += mat[7] * d;
	d = mat[9] * di;
	mat[8] += mat[4] * d;
	mat[10] += mat[6] * d;
	mat[11] += mat[7] * d;
	d = mat[13] * di;
	mat[12] += mat[4] * d;
	mat[14] += mat[6] * d;
	mat[15] += mat[7] * d;
	di = mat[10];
	s *= di;
	mat[10] = d = 1.0f / di;
	mat[8] *= d;
	mat[9] *= d;
	mat[11] *= d;
	d = -d;
	mat[2] *= d;
	mat[6] *= d;
	mat[14] *= d;
	d = mat[2] * di;
	mat[0] += mat[8] * d;
	mat[1] += mat[9] * d;
	mat[3] += mat[11] * d;
	d = mat[6] * di;
	mat[4] += mat[8] * d;
	mat[5] += mat[9] * d;
	mat[7] += mat[11] * d;
	d = mat[14] * di;
	mat[12] += mat[8] * d;
	mat[13] += mat[9] * d;
	mat[15] += mat[11] * d;
	di = mat[15];
	s *= di;
	mat[15] = d = 1.0f / di;
	mat[12] *= d;
	mat[13] *= d;
	mat[14] *= d;
	d = -d;
	mat[3] *= d;
	mat[7] *= d;
	mat[11] *= d;
	d = mat[3] * di;
	mat[0] += mat[12] * d;
	mat[1] += mat[13] * d;
	mat[2] += mat[14] * d;
	d = mat[7] * di;
	mat[4] += mat[12] * d;
	mat[5] += mat[13] * d;
	mat[6] += mat[14] * d;
	d = mat[11] * di;
	mat[8] += mat[12] * d;
	mat[9] += mat[13] * d;
	mat[10] += mat[14] * d;

	return ( s != 0.0f && !FLOAT_IS_NAN( s ) );
#else
	//	6*8+2*6 = 60 multiplications
	//		2*1 =  2 divisions
	olaMat2 r0, r1, r2, r3;
	float a, det, invDet;
	float *mat = reinterpret_cast<float *>(this);

	// r0 = m0.Inverse();
	det = mat[0*4+0] * mat[1*4+1] - mat[0*4+1] * mat[1*4+0];

	if ( olaMath::Fabs( det ) < MATRIX_INVERSE_EPSILON ) {
		return false;
	}

	invDet = 1.0f / det;

	r0[0][0] =   mat[1*4+1] * invDet;
	r0[0][1] = - mat[0*4+1] * invDet;
	r0[1][0] = - mat[1*4+0] * invDet;
	r0[1][1] =   mat[0*4+0] * invDet;

	// r1 = r0 * m1;
	r1[0][0] = r0[0][0] * mat[0*4+2] + r0[0][1] * mat[1*4+2];
	r1[0][1] = r0[0][0] * mat[0*4+3] + r0[0][1] * mat[1*4+3];
	r1[1][0] = r0[1][0] * mat[0*4+2] + r0[1][1] * mat[1*4+2];
	r1[1][1] = r0[1][0] * mat[0*4+3] + r0[1][1] * mat[1*4+3];

	// r2 = m2 * r1;
	r2[0][0] = mat[2*4+0] * r1[0][0] + mat[2*4+1] * r1[1][0];
	r2[0][1] = mat[2*4+0] * r1[0][1] + mat[2*4+1] * r1[1][1];
	r2[1][0] = mat[3*4+0] * r1[0][0] + mat[3*4+1] * r1[1][0];
	r2[1][1] = mat[3*4+0] * r1[0][1] + mat[3*4+1] * r1[1][1];

	// r3 = r2 - m3;
	r3[0][0] = r2[0][0] - mat[2*4+2];
	r3[0][1] = r2[0][1] - mat[2*4+3];
	r3[1][0] = r2[1][0] - mat[3*4+2];
	r3[1][1] = r2[1][1] - mat[3*4+3];

	// r3.InverseSelf();
	det = r3[0][0] * r3[1][1] - r3[0][1] * r3[1][0];

	if ( olaMath::Fabs( det ) < MATRIX_INVERSE_EPSILON ) {
		return false;
	}

	invDet = 1.0f / det;

	a = r3[0][0];
	r3[0][0] =   r3[1][1] * invDet;
	r3[0][1] = - r3[0][1] * invDet;
	r3[1][0] = - r3[1][0] * invDet;
	r3[1][1] =   a * invDet;

	// r2 = m2 * r0;
	r2[0][0] = mat[2*4+0] * r0[0][0] + mat[2*4+1] * r0[1][0];
	r2[0][1] = mat[2*4+0] * r0[0][1] + mat[2*4+1] * r0[1][1];
	r2[1][0] = mat[3*4+0] * r0[0][0] + mat[3*4+1] * r0[1][0];
	r2[1][1] = mat[3*4+0] * r0[0][1] + mat[3*4+1] * r0[1][1];

	// m2 = r3 * r2;
	mat[2*4+0] = r3[0][0] * r2[0][0] + r3[0][1] * r2[1][0];
	mat[2*4+1] = r3[0][0] * r2[0][1] + r3[0][1] * r2[1][1];
	mat[3*4+0] = r3[1][0] * r2[0][0] + r3[1][1] * r2[1][0];
	mat[3*4+1] = r3[1][0] * r2[0][1] + r3[1][1] * r2[1][1];

	// m0 = r0 - r1 * m2;
	mat[0*4+0] = r0[0][0] - r1[0][0] * mat[2*4+0] - r1[0][1] * mat[3*4+0];
	mat[0*4+1] = r0[0][1] - r1[0][0] * mat[2*4+1] - r1[0][1] * mat[3*4+1];
	mat[1*4+0] = r0[1][0] - r1[1][0] * mat[2*4+0] - r1[1][1] * mat[3*4+0];
	mat[1*4+1] = r0[1][1] - r1[1][0] * mat[2*4+1] - r1[1][1] * mat[3*4+1];

	// m1 = r1 * r3;
	mat[0*4+2] = r1[0][0] * r3[0][0] + r1[0][1] * r3[1][0];
	mat[0*4+3] = r1[0][0] * r3[0][1] + r1[0][1] * r3[1][1];
	mat[1*4+2] = r1[1][0] * r3[0][0] + r1[1][1] * r3[1][0];
	mat[1*4+3] = r1[1][0] * r3[0][1] + r1[1][1] * r3[1][1];

	// m3 = -r3;
	mat[2*4+2] = -r3[0][0];
	mat[2*4+3] = -r3[0][1];
	mat[3*4+2] = -r3[1][0];
	mat[3*4+3] = -r3[1][1];

	return true;
#endif
}


olaMat4::olaMat4( void ) {
}

olaMat4::olaMat4( const olaVec4 &x, const olaVec4 &y, const olaVec4 &z, const olaVec4 &w ) {
	mat[ 0 ] = x;
	mat[ 1 ] = y;
	mat[ 2 ] = z;
	mat[ 3 ] = w;
}

olaMat4::olaMat4( const float xx, const float xy, const float xz, const float xw,
							const float yx, const float yy, const float yz, const float yw,
							const float zx, const float zy, const float zz, const float zw,
							const float wx, const float wy, const float wz, const float ww ) {
	mat[0][0] = xx; mat[0][1] = xy; mat[0][2] = xz; mat[0][3] = xw;
	mat[1][0] = yx; mat[1][1] = yy; mat[1][2] = yz; mat[1][3] = yw;
	mat[2][0] = zx; mat[2][1] = zy; mat[2][2] = zz; mat[2][3] = zw;
	mat[3][0] = wx; mat[3][1] = wy; mat[3][2] = wz; mat[3][3] = ww;
}

olaMat4::olaMat4( const olaMat3 &rotation, const olaVec3 &translation ) {
	// NOTE: olaMat3 is transposed because it is column-major
	mat[ 0 ][ 0 ] = rotation[0][0];
	mat[ 0 ][ 1 ] = rotation[1][0];
	mat[ 0 ][ 2 ] = rotation[2][0];
	mat[ 0 ][ 3 ] = translation[0];
	mat[ 1 ][ 0 ] = rotation[0][1];
	mat[ 1 ][ 1 ] = rotation[1][1];
	mat[ 1 ][ 2 ] = rotation[2][1];
	mat[ 1 ][ 3 ] = translation[1];
	mat[ 2 ][ 0 ] = rotation[0][2];
	mat[ 2 ][ 1 ] = rotation[1][2];
	mat[ 2 ][ 2 ] = rotation[2][2];
	mat[ 2 ][ 3 ] = translation[2];
	mat[ 3 ][ 0 ] = 0.0f;
	mat[ 3 ][ 1 ] = 0.0f;
	mat[ 3 ][ 2 ] = 0.0f;
	mat[ 3 ][ 3 ] = 1.0f;
}

olaMat4::olaMat4( const float src[ 4 ][ 4 ] ) {
	memcpy( mat, src, 4 * 4 * sizeof( float ) );
}

const olaVec4 &olaMat4::operator[]( int index ) const {
	//assert( ( index >= 0 ) && ( index < 4 ) );
	return mat[ index ];
}

olaVec4 &olaMat4::operator[]( int index ) {
	//assert( ( index >= 0 ) && ( index < 4 ) );
	return mat[ index ];
}

olaMat4 olaMat4::operator*( const float a ) const {
	return olaMat4(
		mat[0].x * a, mat[0].y * a, mat[0].z * a, mat[0].w * a,
		mat[1].x * a, mat[1].y * a, mat[1].z * a, mat[1].w * a,
		mat[2].x * a, mat[2].y * a, mat[2].z * a, mat[2].w * a,
		mat[3].x * a, mat[3].y * a, mat[3].z * a, mat[3].w * a );
}

olaVec4 olaMat4::operator*( const olaVec4 &vec ) const {
	return olaVec4(
		mat[ 0 ].x * vec.x + mat[ 0 ].y * vec.y + mat[ 0 ].z * vec.z + mat[ 0 ].w * vec.w,
		mat[ 1 ].x * vec.x + mat[ 1 ].y * vec.y + mat[ 1 ].z * vec.z + mat[ 1 ].w * vec.w,
		mat[ 2 ].x * vec.x + mat[ 2 ].y * vec.y + mat[ 2 ].z * vec.z + mat[ 2 ].w * vec.w,
		mat[ 3 ].x * vec.x + mat[ 3 ].y * vec.y + mat[ 3 ].z * vec.z + mat[ 3 ].w * vec.w );
}

olaVec3 olaMat4::operator*( const olaVec3 &vec ) const {
	float s = mat[ 3 ].x * vec.x + mat[ 3 ].y * vec.y + mat[ 3 ].z * vec.z + mat[ 3 ].w;
	if ( s == 0.0f ) {
		return olaVec3( 0.0f, 0.0f, 0.0f );
	}
	if ( s == 1.0f ) {
		return olaVec3(
			mat[ 0 ].x * vec.x + mat[ 0 ].y * vec.y + mat[ 0 ].z * vec.z + mat[ 0 ].w,
			mat[ 1 ].x * vec.x + mat[ 1 ].y * vec.y + mat[ 1 ].z * vec.z + mat[ 1 ].w,
			mat[ 2 ].x * vec.x + mat[ 2 ].y * vec.y + mat[ 2 ].z * vec.z + mat[ 2 ].w );
	}
	else {
		float invS = 1.0f / s;
		return olaVec3(
			(mat[ 0 ].x * vec.x + mat[ 0 ].y * vec.y + mat[ 0 ].z * vec.z + mat[ 0 ].w) * invS,
			(mat[ 1 ].x * vec.x + mat[ 1 ].y * vec.y + mat[ 1 ].z * vec.z + mat[ 1 ].w) * invS,
			(mat[ 2 ].x * vec.x + mat[ 2 ].y * vec.y + mat[ 2 ].z * vec.z + mat[ 2 ].w) * invS );
	}
}

olaMat4 olaMat4::operator*( const olaMat4 &a ) const {
	int i, j;
	const float *m1Ptr, *m2Ptr;
	float *dstPtr;
	olaMat4 dst;

	m1Ptr = reinterpret_cast<const float *>(this);
	m2Ptr = reinterpret_cast<const float *>(&a);
	dstPtr = reinterpret_cast<float *>(&dst);

	for ( i = 0; i < 4; i++ ) {
		for ( j = 0; j < 4; j++ ) {
			*dstPtr = m1Ptr[0] * m2Ptr[ 0 * 4 + j ]
					+ m1Ptr[1] * m2Ptr[ 1 * 4 + j ]
					+ m1Ptr[2] * m2Ptr[ 2 * 4 + j ]
					+ m1Ptr[3] * m2Ptr[ 3 * 4 + j ];
			dstPtr++;
		}
		m1Ptr += 4;
	}
	return dst;
}

olaMat4 olaMat4::operator+( const olaMat4 &a ) const {
	return olaMat4( 
		mat[0].x + a[0].x, mat[0].y + a[0].y, mat[0].z + a[0].z, mat[0].w + a[0].w,
		mat[1].x + a[1].x, mat[1].y + a[1].y, mat[1].z + a[1].z, mat[1].w + a[1].w,
		mat[2].x + a[2].x, mat[2].y + a[2].y, mat[2].z + a[2].z, mat[2].w + a[2].w,
		mat[3].x + a[3].x, mat[3].y + a[3].y, mat[3].z + a[3].z, mat[3].w + a[3].w );
}
    
olaMat4 olaMat4::operator-( const olaMat4 &a ) const {
	return olaMat4( 
		mat[0].x - a[0].x, mat[0].y - a[0].y, mat[0].z - a[0].z, mat[0].w - a[0].w,
		mat[1].x - a[1].x, mat[1].y - a[1].y, mat[1].z - a[1].z, mat[1].w - a[1].w,
		mat[2].x - a[2].x, mat[2].y - a[2].y, mat[2].z - a[2].z, mat[2].w - a[2].w,
		mat[3].x - a[3].x, mat[3].y - a[3].y, mat[3].z - a[3].z, mat[3].w - a[3].w );
}

olaMat4 &olaMat4::operator*=( const float a ) {
	mat[0].x *= a; mat[0].y *= a; mat[0].z *= a; mat[0].w *= a;
	mat[1].x *= a; mat[1].y *= a; mat[1].z *= a; mat[1].w *= a;
	mat[2].x *= a; mat[2].y *= a; mat[2].z *= a; mat[2].w *= a;
	mat[3].x *= a; mat[3].y *= a; mat[3].z *= a; mat[3].w *= a;
    return *this;
}

olaMat4 &olaMat4::operator*=( const olaMat4 &a ) {
	*this = (*this) * a;
	return *this;
}

olaMat4 &olaMat4::operator+=( const olaMat4 &a ) {
	mat[0].x += a[0].x; mat[0].y += a[0].y; mat[0].z += a[0].z; mat[0].w += a[0].w;
	mat[1].x += a[1].x; mat[1].y += a[1].y; mat[1].z += a[1].z; mat[1].w += a[1].w;
	mat[2].x += a[2].x; mat[2].y += a[2].y; mat[2].z += a[2].z; mat[2].w += a[2].w;
	mat[3].x += a[3].x; mat[3].y += a[3].y; mat[3].z += a[3].z; mat[3].w += a[3].w;
    return *this;
}

olaMat4 &olaMat4::operator-=( const olaMat4 &a ) {
	mat[0].x -= a[0].x; mat[0].y -= a[0].y; mat[0].z -= a[0].z; mat[0].w -= a[0].w;
	mat[1].x -= a[1].x; mat[1].y -= a[1].y; mat[1].z -= a[1].z; mat[1].w -= a[1].w;
	mat[2].x -= a[2].x; mat[2].y -= a[2].y; mat[2].z -= a[2].z; mat[2].w -= a[2].w;
	mat[3].x -= a[3].x; mat[3].y -= a[3].y; mat[3].z -= a[3].z; mat[3].w -= a[3].w;
    return *this;
}

olaMat4 operator*( const float a, const olaMat4 &mat ) {
	return mat * a;
}

olaVec4 operator*( const olaVec4 &vec, const olaMat4 &mat ) {
	return mat * vec;
}

olaVec3 operator*( const olaVec3 &vec, const olaMat4 &mat ) {
	return mat * vec;
}

olaVec4 &operator*=( olaVec4 &vec, const olaMat4 &mat ) {
	vec = mat * vec;
	return vec;
}

olaVec3 &operator*=( olaVec3 &vec, const olaMat4 &mat ) {
	vec = mat * vec;
	return vec;
}

bool olaMat4::Compare( const olaMat4 &a ) const {
	dword i;
	const float *ptr1, *ptr2;

	ptr1 = reinterpret_cast<const float *>(mat);
	ptr2 = reinterpret_cast<const float *>(a.mat);
	for ( i = 0; i < 4*4; i++ ) {
		if ( ptr1[i] != ptr2[i] ) {
			return false;
		}
	}
	return true;
}

olaVec3 olaMat4::TransformNormal(olaVec3& vec)
{
	return olaVec3(
		mat[ 0 ].x * vec.x + mat[ 0 ].y * vec.y + mat[ 0 ].z * vec.z ,
		mat[ 1 ].x * vec.x + mat[ 1 ].y * vec.y + mat[ 1 ].z * vec.z ,
		mat[ 2 ].x * vec.x + mat[ 2 ].y * vec.y + mat[ 2 ].z * vec.z );
}

olaVec3 olaMat4::TransformCoord(olaVec3& vec)
{
	return olaVec3(
		mat[ 0 ].x * vec.x + mat[ 0 ].y * vec.y + mat[ 0 ].z * vec.z + mat[ 0 ].w,
		mat[ 1 ].x * vec.x + mat[ 1 ].y * vec.y + mat[ 1 ].z * vec.z + mat[ 1 ].w,
		mat[ 2 ].x * vec.x + mat[ 2 ].y * vec.y + mat[ 2 ].z * vec.z + mat[ 2 ].w );
}

bool olaMat4::Compare( const olaMat4 &a, const float epsilon ) const {
	dword i;
	const float *ptr1, *ptr2;

	ptr1 = reinterpret_cast<const float *>(mat);
	ptr2 = reinterpret_cast<const float *>(a.mat);
	for ( i = 0; i < 4*4; i++ ) {
		if ( olaMath::Fabs( ptr1[i] - ptr2[i] ) > epsilon ) {
			return false;
		}
	}
	return true;
}

bool olaMat4::operator==( const olaMat4 &a ) const {
	return Compare( a );
}

bool olaMat4::operator!=( const olaMat4 &a ) const {
	return !Compare( a );
}

void olaMat4::Zero( void ) {
	memset( mat, 0, sizeof( olaMat4 ) );
}

void olaMat4::Identity( void ) {
	*this = mat4_identity;
}

bool olaMat4::IsIdentity( const float epsilon ) const {
	return Compare( mat4_identity, epsilon );
}

bool olaMat4::IsSymmetric( const float epsilon ) const {
	for ( int i = 1; i < 4; i++ ) {
		for ( int j = 0; j < i; j++ ) {
			if ( olaMath::Fabs( mat[i][j] - mat[j][i] ) > epsilon ) {
				return false;
			}
		}
	}
	return true;
}

bool olaMat4::IsDiagonal( const float epsilon ) const {
	for ( int i = 0; i < 4; i++ ) {
		for ( int j = 0; j < 4; j++ ) {
			if ( i != j && olaMath::Fabs( mat[i][j] ) > epsilon ) {
				return false;
			}
		}
	}
	return true;
}

bool olaMat4::IsRotated( void ) const {
	if ( !mat[ 0 ][ 1 ] && !mat[ 0 ][ 2 ] &&
		!mat[ 1 ][ 0 ] && !mat[ 1 ][ 2 ] &&
		!mat[ 2 ][ 0 ] && !mat[ 2 ][ 1 ] ) {
		return false;
	}
	return true;
}

void olaMat4::ProjectVector( const olaVec4 &src, olaVec4 &dst ) const {
	dst.x = src * mat[ 0 ];
	dst.y = src * mat[ 1 ];
	dst.z = src * mat[ 2 ];
	dst.w = src * mat[ 3 ];
}

void olaMat4::UnprojectVector( const olaVec4 &src, olaVec4 &dst ) const {
	dst = mat[ 0 ] * src.x + mat[ 1 ] * src.y + mat[ 2 ] * src.z + mat[ 3 ] * src.w;
}

float olaMat4::Trace( void ) const {
	return ( mat[0][0] + mat[1][1] + mat[2][2] + mat[3][3] );
}

olaMat4 olaMat4::Inverse( void ) const {
	olaMat4 invMat;

	invMat = *this;
	int r = invMat.InverseSelf();
	assert( r );
	return invMat;
}

olaMat4 olaMat4::InverseFast( void ) const {
	olaMat4 invMat;

	invMat = *this;
	int r = invMat.InverseFastSelf();
	assert( r );
	return invMat;
}

olaMat4 olaMat3::ToMat4( void ) const {
	// NOTE: olaMat3 is transposed because it is column-major
	return olaMat4(	mat[0][0],	mat[1][0],	mat[2][0],	0.0f,
					mat[0][1],	mat[1][1],	mat[2][1],	0.0f,
					mat[0][2],	mat[1][2],	mat[2][2],	0.0f,
					0.0f,		0.0f,		0.0f,		1.0f );
}

int olaMat4::GetDimension( void ) const {
	return 16;
}

const float *olaMat4::ToFloatPtr( void ) const {
	return mat[0].ToFloatPtr();
}

float *olaMat4::ToFloatPtr( void ) {
	return mat[0].ToFloatPtr();
}

olaMat3 olaMat4::ToMatrix3(void) const {
	// NOTE: olaMat3 is transposed because it is column-major
	olaMat3 m3;
	m3[0][0] = mat[ 0 ][ 0 ];
	m3[1][0] = mat[ 0 ][ 1 ];
	m3[2][0] = mat[ 0 ][ 2 ];
	m3[0][1] = mat[ 1 ][ 0 ];
	m3[1][1] = mat[ 1 ][ 1 ];
	m3[2][1] = mat[ 1 ][ 2 ];
	m3[0][2] = mat[ 2 ][ 0 ];
	m3[1][2] = mat[ 2 ][ 1 ];
	m3[2][2] = mat[ 2 ][ 2 ];
	return m3;
}

void olaMat4::CopyToFloatArrayColumn(float* m44)
{
	m44[0]  = mat[0][0];
	m44[1]  = mat[1][0];
	m44[2]  = mat[2][0];
	m44[3]  = mat[3][0];

	m44[4]  = mat[0][1];
	m44[5]  = mat[1][1];
	m44[6]  = mat[2][1];
	m44[7]  = mat[3][1];

	m44[8]  = mat[0][2];
	m44[9]  = mat[1][2];
	m44[10] = mat[2][2];
	m44[11] = mat[3][2];

	m44[12] = mat[0][3];
	m44[13] = mat[1][3];
	m44[14] = mat[2][3];
	m44[15] = mat[3][3];	
}

void olaMat4::FromFloatsColumnMajor(float* pIn16)
{
	for (int i=0; i<4; ++i)
	{
		mat[i].x = pIn16[i];
		mat[i].y = pIn16[i+4];
		mat[i].z = pIn16[i+8];
		mat[i].w = pIn16[i+12];
	}

}

void olaMat4::Compose(const olaQuat& rotation, const olaVec3& translation)
{
	olaMat3 matRot = rotation.ToMat3();
	mat[0].Set(matRot[0].x, matRot[0].y, matRot[0].z, translation.x);
	mat[1].Set(matRot[1].x, matRot[1].y, matRot[1].z, translation.y);
	mat[2].Set(matRot[2].x, matRot[2].y, matRot[2].z, translation.z);
	mat[3].Set(0.0f, 0.0f, 0.0f, 1.0f);
}

void olaMat4::Decompose(olaVec3& scale, olaQuat& rotation, olaVec3& position)const
{
	olaMat4 temp = *this;

	scale.x = olaMath::Sqrt(mat[0].x*mat[0].x+mat[1].x*mat[1].x+mat[2].x*mat[2].x);
	scale.y = olaMath::Sqrt(mat[0].y*mat[0].y+mat[1].y*mat[1].y+mat[2].y*mat[2].y);
	scale.z = olaMath::Sqrt(mat[0].z*mat[0].z+mat[1].z*mat[1].z+mat[2].z*mat[2].z);

	temp[0].x /= scale.x;
	temp[1].x /= scale.x;
	temp[2].x /= scale.x;
	temp[0].y /= scale.y;
	temp[1].y /= scale.y;
	temp[2].y /= scale.y;
	temp[0].z /= scale.z;
	temp[1].z /= scale.z;
	temp[2].z /= scale.z;

	position.x = temp[0].w;
	position.y = temp[1].w;
	position.z = temp[2].w;
	olaMat3 rot; //oldMat3 is column majored!!!!

	rot[0].Set(temp[0].x, temp[0].y, temp[0].z);
	rot[1].Set(temp[1].x, temp[1].y, temp[1].z);
	rot[2].Set(temp[2].x, temp[2].y, temp[2].z);
	rotation = rot.ToQuat();

	olaVec3 column0 = rot[0];
	olaVec3 column1 = rot[1];
	olaVec3 column2 = rot[2];
	olaVec3 row0(column0.x, column1.x, column2.x);
	olaVec3 row1(column0.y, column1.y, column2.y);
	olaVec3 row2(column0.z, column1.z, column2.z);
	float len = column0.Length();
	len = column1.Length();
	len = column2.Length();
	len = row0.Length();
	len = row1.Length();
	len = row2.Length();

	//H3DMat3 rot2 = rotation.ToMat3();
	//int a = 0;
}

void olaMat4::Multiply(const olaMat4& left, const olaMat4& right)
{
	int i, j;
	const float *m1Ptr, *m2Ptr;
	float *dstPtr;

	m1Ptr = reinterpret_cast<const float *>(&left);
	m2Ptr = reinterpret_cast<const float *>(&right);
	dstPtr = reinterpret_cast<float *>(this);

	for ( i = 0; i < 4; i++ ) {
		for ( j = 0; j < 4; j++ ) {
			*dstPtr = m1Ptr[0] * m2Ptr[ 0 * 4 + j ]
			+ m1Ptr[1] * m2Ptr[ 1 * 4 + j ]
			+ m1Ptr[2] * m2Ptr[ 2 * 4 + j ]
			+ m1Ptr[3] * m2Ptr[ 3 * 4 + j ];
			dstPtr++;
		}
		m1Ptr += 4;
	}
}

#pragma warning (pop)