// Copyright (C) 2004 Id Software, Inc.
//


#include "ola_vector.h"
#include "ola_matrix.h"
#include "ola_quat.h"
#pragma hdrstop

/*
=====================
olaQuat::ToMat3
=====================
*/
olaMat3 olaQuat::ToMat3( void ) const {
	olaMat3	mat;
	float	wx, wy, wz;
	float	xx, yy, yz;
	float	xy, xz, zz;
	float	x2, y2, z2;

	x2 = x + x;
	y2 = y + y;
	z2 = z + z;

	xx = x * x2;
	xy = x * y2;
	xz = x * z2;

	yy = y * y2;
	yz = y * z2;
	zz = z * z2;

	wx = w * x2;
	wy = w * y2;
	wz = w * z2;

	mat[ 0 ][ 0 ] = 1.0f - ( yy + zz );
	mat[ 0 ][ 1 ] = xy - wz;
	mat[ 0 ][ 2 ] = xz + wy;

	mat[ 1 ][ 0 ] = xy + wz;
	mat[ 1 ][ 1 ] = 1.0f - ( xx + zz );
	mat[ 1 ][ 2 ] = yz - wx;

	mat[ 2 ][ 0 ] = xz - wy;
	mat[ 2 ][ 1 ] = yz + wx;
	mat[ 2 ][ 2 ] = 1.0f - ( xx + yy );

	return mat;
}

/*
=====================
olaQuat::ToMat4
=====================
*/
olaMat4 olaQuat::ToMat4( void ) const {
	return ToMat3().ToMat4();
}

/*
=====================
olaQuat::ToCQuat
=====================
*/
olaCQuat olaQuat::ToCQuat( void ) const {
	if ( w < 0.0f ) {
		return olaCQuat( -x, -y, -z );
	}
	return olaCQuat( x, y, z );
}

/*
============
olaQuat::ToAngularVelocity
============
*/
olaVec3 olaQuat::ToAngularVelocity( void ) const {
	olaVec3 vec;

	vec.x = x;
	vec.y = y;
	vec.z = z;
	vec.Normalize();
	return vec * olaMath::ACos( w );
}


/*
=====================
olaQuat::Slerp

Spherical linear interpolation between two quaternions.
=====================
*/

olaQuat& olaQuat::FromAxisAngle(olaVec3& axis,float degree)
{	
	float theta = olaMath::M_DEG2RAD* degree * 0.5f;
	float sin_theta = olaMath::Sin(theta);

	x = sin_theta * axis.x;
	y = sin_theta * axis.y;
	z = sin_theta * axis.z;

	w = olaMath::Cos(theta);

	return *this;
}	

olaQuat& olaQuat::FromAxisAngle(float _x,float _y,float _z,float degree)
{
	float theta = olaMath::M_DEG2RAD* degree * 0.5f;
	float sin_theta = olaMath::Sin(theta);

	x = sin_theta * _x;
	y = sin_theta * _y;
	z = sin_theta * _z;

	w = olaMath::Cos(theta);

	return *this;
}


olaQuat &olaQuat::Slerp( const olaQuat &from, const olaQuat &to, float t ) {
	olaQuat	temp;
	float	omega, cosom, sinom, scale0, scale1;

	if ( t <= 0.0f ) {
		*this = from;
		return *this;
	}

	if ( t >= 1.0f ) {
		*this = to;
		return *this;
	}

	if ( from == to ) {
		*this = to;
		return *this;
	}

	cosom = from.x * to.x + from.y * to.y + from.z * to.z + from.w * to.w;
	if ( cosom < 0.0f ) {
		temp = -to;
		cosom = -cosom;
	} else {
		temp = to;
	}

	if ( ( 1.0f - cosom ) > 1e-6f ) {
#if 0
		omega = acos( cosom );
		sinom = 1.0f / sin( omega );
		scale0 = sin( ( 1.0f - t ) * omega ) * sinom;
		scale1 = sin( t * omega ) * sinom;
#else
		scale0 = 1.0f - cosom * cosom;
		sinom = olaMath::InvSqrt( scale0 );
		omega = olaMath::ATan16( scale0 * sinom, cosom );
		scale0 = olaMath::Sin16( ( 1.0f - t ) * omega ) * sinom;
		scale1 = olaMath::Sin16( t * omega ) * sinom;
#endif
	} else {
		scale0 = 1.0f - t;
		scale1 = t;
	}

	*this = ( scale0 * from ) + ( scale1 * temp );
	return *this;
}


olaQuat::olaQuat( void ) {
}

olaQuat::olaQuat( float x, float y, float z, float w ) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

float olaQuat::operator[]( int index ) const {
	assert( ( index >= 0 ) && ( index < 4 ) );
	return ( &x )[ index ];
}

float& olaQuat::operator[]( int index ) {
	assert( ( index >= 0 ) && ( index < 4 ) );
	return ( &x )[ index ];
}

olaQuat olaQuat::operator-() const {
	return olaQuat( -x, -y, -z, -w );
}

olaQuat &olaQuat::operator=( const olaQuat &a ) {
	x = a.x;
	y = a.y;
	z = a.z;
	w = a.w;

	return *this;
}

olaQuat olaQuat::operator+( const olaQuat &a ) const {
	return olaQuat( x + a.x, y + a.y, z + a.z, w + a.w );
}

olaQuat& olaQuat::operator+=( const olaQuat &a ) {
	x += a.x;
	y += a.y;
	z += a.z;
	w += a.w;

	return *this;
}

olaQuat olaQuat::operator-( const olaQuat &a ) const {
	return olaQuat( x - a.x, y - a.y, z - a.z, w - a.w );
}

olaQuat& olaQuat::operator-=( const olaQuat &a ) {
	x -= a.x;
	y -= a.y;
	z -= a.z;
	w -= a.w;

	return *this;
}

olaQuat olaQuat::operator*( const olaQuat &a ) const {
	return olaQuat(	w*a.x + x*a.w + y*a.z - z*a.y,
					w*a.y + y*a.w + z*a.x - x*a.z,
					w*a.z + z*a.w + x*a.y - y*a.x,
					w*a.w - x*a.x - y*a.y - z*a.z );
}

olaVec3 olaQuat::operator*( const olaVec3 &a ) const {
#if 0
	// it's faster to do the conversion to a 3x3 matrix and multiply the vector by this 3x3 matrix
	return ( ToMat3() * a );
#else
	// result = this->Inverse() * olaQuat( a.x, a.y, a.z, 0.0f ) * (*this)
	float xxzz = x*x - z*z;
	float wwyy = w*w - y*y;

	float xw2 = x*w*2.0f;
	float xy2 = x*y*2.0f;
	float xz2 = x*z*2.0f;
	float yw2 = y*w*2.0f;
	float yz2 = y*z*2.0f;
	float zw2 = z*w*2.0f;

	return olaVec3(
		(xxzz + wwyy)*a.x		+ (xy2 + zw2)*a.y		+ (xz2 - yw2)*a.z,
		(xy2 - zw2)*a.x			+ (xxzz - wwyy)*a.y		+ (yz2 + xw2)*a.z,
		(xz2 + yw2)*a.x			+ (yz2 - xw2)*a.y		+ (wwyy - xxzz)*a.z
	);
#endif
}

olaQuat olaQuat::operator*( float a ) const {
	return olaQuat( x * a, y * a, z * a, w * a );
}

olaQuat operator*( const float a, const olaQuat &b ) {
	return b * a;
}

olaVec3 operator*( const olaVec3 &a, const olaQuat &b ) {
	return b * a;
}

olaQuat& olaQuat::operator*=( const olaQuat &a ) {
	*this = *this * a;

	return *this;
}

olaQuat& olaQuat::operator*=( float a ) {
	x *= a;
	y *= a;
	z *= a;
	w *= a;

	return *this;
}

bool olaQuat::Compare( const olaQuat &a ) const {
	return ( ( x == a.x ) && ( y == a.y ) && ( z == a.z ) && ( w == a.w ) );
}

bool olaQuat::Compare( const olaQuat &a, const float epsilon ) const {
	if ( olaMath::Fabs( x - a.x ) > epsilon ) {
		return false;
	}
	if ( olaMath::Fabs( y - a.y ) > epsilon ) {
		return false;
	}
	if ( olaMath::Fabs( z - a.z ) > epsilon ) {
		return false;
	}
	if ( olaMath::Fabs( w - a.w ) > epsilon ) {
		return false;
	}
	return true;
}

bool olaQuat::operator==( const olaQuat &a ) const {
	return Compare( a );
}

bool olaQuat::operator!=( const olaQuat &a ) const {
	return !Compare( a );
}

void olaQuat::Set( float x, float y, float z, float w ) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

olaQuat olaQuat::Inverse( void ) const {
	return olaQuat( -x, -y, -z, w );
}

float olaQuat::Length( void ) const {
	float len;

	len = x * x + y * y + z * z + w * w;
	return olaMath::Sqrt( len );
}

olaQuat& olaQuat::Normalize( void ) {
	float len;
	float ilength;

	len = this->Length();
	if ( len ) {
		ilength = 1 / len;
		x *= ilength;
		y *= ilength;
		z *= ilength;
		w *= ilength;
	}
	return *this;
}

float olaQuat::CalcW( void ) const {
	// take the absolute value because floating point rounding may cause the dot of x,y,z to be larger than 1
	return sqrt( fabs( 1.0f - ( x * x + y * y + z * z ) ) );
}

int olaQuat::GetDimension( void ) const {
	return 4;
}

const float *olaQuat::ToFloatPtr( void ) const {
	return &x;
}

float *olaQuat::ToFloatPtr( void ) {
	return &x;
}

olaQuat& olaQuat::Conjugate()
{
	x *= -1.0f;
	y *= -1.0f;
	z *= -1.0f;
	return *this;
}


/*
=============
idCQuat::ToMat3
=============
*/
olaMat3 olaCQuat::ToMat3( void ) const {
	return ToQuat().ToMat3();
}

/*
=============
idCQuat::ToMat4
=============
*/
olaMat4 olaCQuat::ToMat4( void ) const {
	return ToQuat().ToMat4();
}


olaCQuat::olaCQuat( void ) {
}

olaCQuat::olaCQuat( float x, float y, float z ) {
	this->x = x;
	this->y = y;
	this->z = z;
}

void olaCQuat::Set( float x, float y, float z ) {
	this->x = x;
	this->y = y;
	this->z = z;
}

float olaCQuat::operator[]( int index ) const {
	assert( ( index >= 0 ) && ( index < 3 ) );
	return ( &x )[ index ];
}

float& olaCQuat::operator[]( int index ) {
	assert( ( index >= 0 ) && ( index < 3 ) );
	return ( &x )[ index ];
}

bool olaCQuat::Compare( const olaCQuat &a ) const {
	return ( ( x == a.x ) && ( y == a.y ) && ( z == a.z ) );
}

bool olaCQuat::Compare( const olaCQuat &a, const float epsilon ) const {
	if ( olaMath::Fabs( x - a.x ) > epsilon ) {
		return false;
	}
	if ( olaMath::Fabs( y - a.y ) > epsilon ) {
		return false;
	}
	if ( olaMath::Fabs( z - a.z ) > epsilon ) {
		return false;
	}
	return true;
}

bool olaCQuat::operator==( const olaCQuat &a ) const {
	return Compare( a );
}

bool olaCQuat::operator!=( const olaCQuat &a ) const {
	return !Compare( a );
}

int olaCQuat::GetDimension( void ) const {
	return 3;
}

olaQuat olaCQuat::ToQuat( void ) const {
	// take the absolute value because floating point rounding may cause the dot of x,y,z to be larger than 1
	return olaQuat( x, y, z, sqrt( fabs( 1.0f - ( x * x + y * y + z * z ) ) ) );
}

const float *olaCQuat::ToFloatPtr( void ) const {
	return &x;
}

float *olaCQuat::ToFloatPtr( void ) {
	return &x;
}

