// Copyright (C) 2004 Id Software, Inc.
//


#pragma hdrstop
#include "ola_vector.h"
#include "ola_matrix.h"
#include "ola_mathbase.h"
olaVec2 vec2_origin( 0.0f, 0.0f );
olaVec3 vec3_origin( 0.0f, 0.0f, 0.0f );
olaVec4 vec4_origin( 0.0f, 0.0f, 0.0f, 0.0f );


//===============================================================
//
//	olaVec2
//
//===============================================================
/*
=============
Lerp

Linearly inperpolates one vector to another.
=============
*/
void olaVec2::Lerp( const olaVec2 &v1, const olaVec2 &v2, const float l ) {
	if ( l <= 0.0f ) {
		(*this) = v1;
	} else if ( l >= 1.0f ) {
		(*this) = v2;
	} else {
		(*this) = v1 + l * ( v2 - v1 );
	}
}

olaVec2::olaVec2( void ) {
}

olaVec2::olaVec2( const float x, const float y ) {
	this->x = x;
	this->y = y;
}

void olaVec2::Set( const float x, const float y ) {
	this->x = x;
	this->y = y;
}

void olaVec2::Zero( void ) {
	x = y = 0.0f;
}

bool olaVec2::Compare( const olaVec2 &a ) const {
	return ( ( x == a.x ) && ( y == a.y ) );
}

bool olaVec2::Compare( const olaVec2 &a, const float epsilon ) const {
	if ( olaMath::Fabs( x - a.x ) > epsilon ) {
		return false;
	}
			
	if ( olaMath::Fabs( y - a.y ) > epsilon ) {
		return false;
	}

	return true;
}

bool olaVec2::operator==( const olaVec2 &a ) const {
	return Compare( a );
}

bool olaVec2::operator!=( const olaVec2 &a ) const {
	return !Compare( a );
}

float olaVec2::operator[]( int index ) const {
	return ( &x )[ index ];
}

float& olaVec2::operator[]( int index ) {
	return ( &x )[ index ];
}

float olaVec2::Length( void ) const {
	return ( float )olaMath::Sqrt( x * x + y * y );
}

float olaVec2::LengthFast( void ) const {
	float sqrLength;

	sqrLength = x * x + y * y;
	return sqrLength * olaMath::RSqrt( sqrLength );
}

float olaVec2::LengthSqr( void ) const {
	return ( x * x + y * y );
}

float olaVec2::Normalize( void ) {
	float sqrLength, invLength;

	sqrLength = x * x + y * y;
	invLength = olaMath::InvSqrt( sqrLength );
	x *= invLength;
	y *= invLength;
	return invLength * sqrLength;
}

float olaVec2::NormalizeFast( void ) {
	float lengthSqr, invLength;

	lengthSqr = x * x + y * y;
	invLength = olaMath::RSqrt( lengthSqr );
	x *= invLength;
	y *= invLength;
	return invLength * lengthSqr;
}

olaVec2 &olaVec2::Truncate( float length ) {
	float length2;
	float ilength;

	if ( !length ) {
		Zero();
	}
	else {
		length2 = LengthSqr();
		if ( length2 > length * length ) {
			ilength = length * olaMath::InvSqrt( length2 );
			x *= ilength;
			y *= ilength;
		}
	}

	return *this;
}

void olaVec2::Clamp( const olaVec2 &min, const olaVec2 &max ) {
	if ( x < min.x ) {
		x = min.x;
	} else if ( x > max.x ) {
		x = max.x;
	}
	if ( y < min.y ) {
		y = min.y;
	} else if ( y > max.y ) {
		y = max.y;
	}
}

void olaVec2::Snap( void ) {
	x = floor( x + 0.5f );
	y = floor( y + 0.5f );
}

void olaVec2::SnapInt( void ) {
	x = float( int( x ) );
	y = float( int( y ) );
}

olaVec2 olaVec2::operator-() const {
	return olaVec2( -x, -y );
}
	
olaVec2 olaVec2::operator-( const olaVec2 &a ) const {
	return olaVec2( x - a.x, y - a.y );
}

float olaVec2::operator*( const olaVec2 &a ) const {
	return x * a.x + y * a.y;
}

olaVec2 olaVec2::operator*( const float a ) const {
	return olaVec2( x * a, y * a );
}

olaVec2 olaVec2::operator/( const float a ) const {
	float inva = 1.0f / a;
	return olaVec2( x * inva, y * inva );
}

olaVec2 operator*( const float a, const olaVec2 b ) {
	return olaVec2( b.x * a, b.y * a );
}

olaVec2 olaVec2::operator+( const olaVec2 &a ) const {
	return olaVec2( x + a.x, y + a.y );
}

olaVec2 &olaVec2::operator+=( const olaVec2 &a ) {
	x += a.x;
	y += a.y;

	return *this;
}

olaVec2 &olaVec2::operator/=( const olaVec2 &a ) {
	x /= a.x;
	y /= a.y;

	return *this;
}

olaVec2 &olaVec2::operator/=( const float a ) {
	float inva = 1.0f / a;
	x *= inva;
	y *= inva;

	return *this;
}

olaVec2 &olaVec2::operator-=( const olaVec2 &a ) {
	x -= a.x;
	y -= a.y;

	return *this;
}

olaVec2 &olaVec2::operator*=( const float a ) {
	x *= a;
	y *= a;

	return *this;
}

int olaVec2::GetDimension( void ) const {
	return 2;
}

const float *olaVec2::ToFloatPtr( void ) const {
	return &x;
}

float *olaVec2::ToFloatPtr( void ) {
	return &x;
}


//===============================================================
//
//	olaVec3
//
//===============================================================

/*
=============
olaVec3::ToYaw
=============
*/
float olaVec3::ToYaw( void ) const {
	float yaw;
	
	if ( ( y == 0.0f ) && ( x == 0.0f ) ) {
		yaw = 0.0f;
	} else {
		yaw = RAD2DEG( atan2( y, x ) );
		if ( yaw < 0.0f ) {
			yaw += 360.0f;
		}
	}

	return yaw;
}

/*
=============
olaVec3::ToPitch
=============
*/
float olaVec3::ToPitch( void ) const {
	float	forward;
	float	pitch;
	
	if ( ( x == 0.0f ) && ( y == 0.0f ) ) {
		if ( z > 0.0f ) {
			pitch = 90.0f;
		} else {
			pitch = 270.0f;
		}
	} else {
		forward = ( float )olaMath::Sqrt( x * x + y * y );
		pitch = RAD2DEG( atan2( z, forward ) );
		if ( pitch < 0.0f ) {
			pitch += 360.0f;
		}
	}

	return pitch;
}

/*
=============
olaVec3::ToPolar
=============
*/
olaPolar3 olaVec3::ToPolar( void ) const {
	float forward;
	float yaw;
	float pitch;
	
	if ( ( x == 0.0f ) && ( y == 0.0f ) ) {
		yaw = 0.0f;
		if ( z > 0.0f ) {
			pitch = 90.0f;
		} else {
			pitch = 270.0f;
		}
	} else {
		yaw = RAD2DEG( atan2( y, x ) );
		if ( yaw < 0.0f ) {
			yaw += 360.0f;
		}

		forward = ( float )olaMath::Sqrt( x * x + y * y );
		pitch = RAD2DEG( atan2( z, forward ) );
		if ( pitch < 0.0f ) {
			pitch += 360.0f;
		}
	}
	return olaPolar3( olaMath::Sqrt( x * x + y * y + z * z ), yaw, -pitch );
}

/*
=============
olaVec3::ToMat3
=============
*/
olaMat3 olaVec3::ToMat3( void ) const {
	olaMat3	mat;
	float	d;

	mat[0] = *this;
	d = x * x + y * y;
	if ( !d ) {
		mat[1][0] = 1.0f;
		mat[1][1] = 0.0f;
		mat[1][2] = 0.0f;
	} else {
		d = olaMath::InvSqrt( d );
		mat[1][0] = -y * d;
		mat[1][1] = x * d;
		mat[1][2] = 0.0f;
	}
	mat[2] = Cross( mat[1] );

	return mat;
}

/*
=============
Lerp

Linearly inperpolates one vector to another.
=============
*/
void olaVec3::Lerp( const olaVec3 &v1, const olaVec3 &v2, const float l ) {
	if ( l <= 0.0f ) {
		(*this) = v1;
	} else if ( l >= 1.0f ) {
		(*this) = v2;
	} else {
		(*this) = v1 + l * ( v2 - v1 );
	}
}

/*
=============
SLerp

Spherical linear interpolation from v1 to v2.
Vectors are expected to be normalized.
=============
*/
#define LERP_DELTA 1e-6

void olaVec3::SLerp( const olaVec3 &v1, const olaVec3 &v2, const float t ) {
	float omega, cosom, sinom, scale0, scale1;

	if ( t <= 0.0f ) {
		(*this) = v1;
		return;
	} else if ( t >= 1.0f ) {
		(*this) = v2;
		return;
	}

	cosom = v1 * v2;
	if ( ( 1.0f - cosom ) > LERP_DELTA ) {
		omega = acos( cosom );
		sinom = sin( omega );
		scale0 = sin( ( 1.0f - t ) * omega ) / sinom;
		scale1 = sin( t * omega ) / sinom;
	} else {
		scale0 = 1.0f - t;
		scale1 = t;
	}

	(*this) = ( v1 * scale0 + v2 * scale1 );
}

/*
=============
ProjectSelfOntoSphere

Projects the z component onto a sphere.
=============
*/
void olaVec3::ProjectSelfOntoSphere( const float radius ) {
	float rsqr = radius * radius;
	float len = Length();
	if ( len  < rsqr * 0.5f ) {
		z = sqrt( rsqr - len );
	} else {
		z = rsqr / ( 2.0f * sqrt( len ) );
	}
}

olaVec3::olaVec3( void ) {
}

olaVec3::olaVec3( const float x, const float y, const float z ) {
	this->x = x;
	this->y = y;
	this->z = z;
}

float olaVec3::operator[]( const int index ) const {
	return ( &x )[ index ];
}

float &olaVec3::operator[]( const int index ) {
	return ( &x )[ index ];
}

void olaVec3::Set( const float x, const float y, const float z ) {
	this->x = x;
	this->y = y;
	this->z = z;
}

void olaVec3::Zero( void ) {
	x = y = z = 0.0f;
}

olaVec3 olaVec3::operator-() const {
	return olaVec3( -x, -y, -z );
}

olaVec3 &olaVec3::operator=( const olaVec3 &a ) {
	x = a.x;
	y = a.y;
	z = a.z;
	return *this;
}

olaVec3 olaVec3::operator-( const olaVec3 &a ) const {
	return olaVec3( x - a.x, y - a.y, z - a.z );
}

float olaVec3::operator*( const olaVec3 &a ) const {
	return x * a.x + y * a.y + z * a.z;
}

olaVec3 olaVec3::operator*( const float a ) const {
	return olaVec3( x * a, y * a, z * a );
}

olaVec3 olaVec3::operator/( const float a ) const {
	float inva = 1.0f / a;
	return olaVec3( x * inva, y * inva, z * inva );
}

olaVec3 operator*( const float a, const olaVec3 b ) {
	return olaVec3( b.x * a, b.y * a, b.z * a );
}

olaVec3 olaVec3::operator+( const olaVec3 &a ) const {
	return olaVec3( x + a.x, y + a.y, z + a.z );
}

olaVec3 &olaVec3::operator+=( const olaVec3 &a ) {
	x += a.x;
	y += a.y;
	z += a.z;

	return *this;
}

olaVec3 &olaVec3::operator/=( const olaVec3 &a ) {
	x /= a.x;
	y /= a.y;
	z /= a.z;

	return *this;
}

olaVec3 &olaVec3::operator/=( const float a ) {
	float inva = 1.0f / a;
	x *= inva;
	y *= inva;
	z *= inva;

	return *this;
}

olaVec3 &olaVec3::operator-=( const olaVec3 &a ) {
	x -= a.x;
	y -= a.y;
	z -= a.z;

	return *this;
}

olaVec3 &olaVec3::operator*=( const float a ) {
	x *= a;
	y *= a;
	z *= a;

	return *this;
}

bool olaVec3::Compare( const olaVec3 &a ) const {
	return ( ( x == a.x ) && ( y == a.y ) && ( z == a.z ) );
}

bool olaVec3::Compare( const olaVec3 &a, const float epsilon ) const {
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

bool olaVec3::operator==( const olaVec3 &a ) const {
	return Compare( a );
}

bool olaVec3::operator!=( const olaVec3 &a ) const {
	return !Compare( a );
}

float olaVec3::NormalizeFast( void ) {
	float sqrLength, invLength;

	sqrLength = x * x + y * y + z * z;
	invLength = olaMath::RSqrt( sqrLength );
	x *= invLength;
	y *= invLength;
	z *= invLength;
	return invLength * sqrLength;
}

bool olaVec3::FixDegenerateNormal( void ) {
	if ( x == 0.0f ) {
		if ( y == 0.0f ) {
			if ( z > 0.0f ) {
				if ( z != 1.0f ) {
					z = 1.0f;
					return true;
				}
			} else {
				if ( z != -1.0f ) {
					z = -1.0f;
					return true;
				}
			}
			return false;
		} else if ( z == 0.0f ) {
			if ( y > 0.0f ) {
				if ( y != 1.0f ) {
					y = 1.0f;
					return true;
				}
			} else {
				if ( y != -1.0f ) {
					y = -1.0f;
					return true;
				}
			}
			return false;
		}
	} else if ( y == 0.0f ) {
		if ( z == 0.0f ) {
			if ( x > 0.0f ) {
				if ( x != 1.0f ) {
					x = 1.0f;
					return true;
				}
			} else {
				if ( x != -1.0f ) {
					x = -1.0f;
					return true;
				}
			}
			return false;
		}
	}
	if ( olaMath::Fabs( x ) == 1.0f ) {
		if ( y != 0.0f || z != 0.0f ) {
			y = z = 0.0f;
			return true;
		}
		return false;
	} else if ( olaMath::Fabs( y ) == 1.0f ) {
		if ( x != 0.0f || z != 0.0f ) {
			x = z = 0.0f;
			return true;
		}
		return false;
	} else if ( olaMath::Fabs( z ) == 1.0f ) {
		if ( x != 0.0f || y != 0.0f ) {
			x = y = 0.0f;
			return true;
		}
		return false;
	}
	return false;
}

bool olaVec3::FixDenormals( void ) {
	bool denormal = false;
	if ( fabs( x ) < 1e-30f ) {
		x = 0.0f;
		denormal = true;
	}
	if ( fabs( y ) < 1e-30f ) {
		y = 0.0f;
		denormal = true;
	}
	if ( fabs( z ) < 1e-30f ) {
		z = 0.0f;
		denormal = true;
	}
	return denormal;
}

olaVec3 olaVec3::Cross( const olaVec3 &a ) const {
	return olaVec3( y * a.z - z * a.y, z * a.x - x * a.z, x * a.y - y * a.x );
}

olaVec3 &olaVec3::Cross( const olaVec3 &a, const olaVec3 &b ) {
	x = a.y * b.z - a.z * b.y;
	y = a.z * b.x - a.x * b.z;
	z = a.x * b.y - a.y * b.x;

	return *this;
}

float olaVec3::Length( void ) const {
	return ( float )olaMath::Sqrt( x * x + y * y + z * z );
}

float olaVec3::LengthSqr( void ) const {
	return ( x * x + y * y + z * z );
}

float olaVec3::LengthFast( void ) const {
	float sqrLength;

	sqrLength = x * x + y * y + z * z;
	return sqrLength * olaMath::RSqrt( sqrLength );
}

float olaVec3::Normalize( void ) {
	float sqrLength, invLength;

	sqrLength = x * x + y * y + z * z;
	invLength = olaMath::InvSqrt( sqrLength );
	x *= invLength;
	y *= invLength;
	z *= invLength;
	return invLength * sqrLength;
}

olaVec3 &olaVec3::Truncate( float length ) {
	float length2;
	float ilength;

	if ( !length ) {
		Zero();
	}
	else {
		length2 = LengthSqr();
		if ( length2 > length * length ) {
			ilength = length * olaMath::InvSqrt( length2 );
			x *= ilength;
			y *= ilength;
			z *= ilength;
		}
	}

	return *this;
}

void olaVec3::Clamp( const olaVec3 &min, const olaVec3 &max ) {
	if ( x < min.x ) {
		x = min.x;
	} else if ( x > max.x ) {
		x = max.x;
	}
	if ( y < min.y ) {
		y = min.y;
	} else if ( y > max.y ) {
		y = max.y;
	}
	if ( z < min.z ) {
		z = min.z;
	} else if ( z > max.z ) {
		z = max.z;
	}
}

void olaVec3::Snap( void ) {
	x = floor( x + 0.5f );
	y = floor( y + 0.5f );
	z = floor( z + 0.5f );
}

void olaVec3::SnapInt( void ) {
	x = float( int( x ) );
	y = float( int( y ) );
	z = float( int( z ) );
}

int olaVec3::GetDimension( void ) const {
	return 3;
}

const olaVec2 &olaVec3::ToVec2( void ) const {
	return *reinterpret_cast<const olaVec2 *>(this);
}

olaVec2 &olaVec3::ToVec2( void ) {
	return *reinterpret_cast<olaVec2 *>(this);
}

const float *olaVec3::ToFloatPtr( void ) const {
	return &x;
}

float *olaVec3::ToFloatPtr( void ) {
	return &x;
}

void olaVec3::NormalVectors( olaVec3 &left, olaVec3 &down ) const {
	float d;

	d = x * x + y * y;
	if ( !d ) {
		left[0] = 1;
		left[1] = 0;
		left[2] = 0;
	} else {
		d = olaMath::InvSqrt( d );
		left[0] = -y * d;
		left[1] = x * d;
		left[2] = 0;
	}
	down = left.Cross( *this );
}

void olaVec3::OrthogonalBasis( olaVec3 &left, olaVec3 &up ) const {
	float l, s;

	if ( olaMath::Fabs( z ) > 0.7f ) {
		l = y * y + z * z;
		s = olaMath::InvSqrt( l );
		up[0] = 0;
		up[1] = z * s;
		up[2] = -y * s;
		left[0] = l * s;
		left[1] = -x * up[2];
		left[2] = x * up[1];
	}
	else {
		l = x * x + y * y;
		s = olaMath::InvSqrt( l );
		left[0] = -y * s;
		left[1] = x * s;
		left[2] = 0;
		up[0] = -z * left[1];
		up[1] = z * left[0];
		up[2] = l * s;
	}
}

void olaVec3::ProjectOntoPlane( const olaVec3 &normal, const float overBounce ) {
	float backoff;
	
	backoff = *this * normal;
	
	if ( overBounce != 1.0 ) {
		if ( backoff < 0 ) {
			backoff *= overBounce;
		} else {
			backoff /= overBounce;
		}
	}

	*this -= backoff * normal;
}

bool olaVec3::ProjectAlongPlane( const olaVec3 &normal, const float epsilon, const float overBounce ) {
	olaVec3 cross;
	float len;

	cross = this->Cross( normal ).Cross( (*this) );
	// normalize so a fixed epsilon can be used
	cross.Normalize();
	len = normal * cross;
	if ( olaMath::Fabs( len ) < epsilon ) {
		return false;
	}
	cross *= overBounce * ( normal * (*this) ) / len;
	(*this) -= cross;
	return true;
}


//===============================================================
//
//	olaVec4
//
//===============================================================

/*
=============
Lerp

Linearly inperpolates one vector to another.
=============
*/
void olaVec4::Lerp( const olaVec4 &v1, const olaVec4 &v2, const float l ) {
	if ( l <= 0.0f ) {
		(*this) = v1;
	} else if ( l >= 1.0f ) {
		(*this) = v2;
	} else {
		(*this) = v1 + l * ( v2 - v1 );
	}
}

olaVec4::olaVec4( void ) {
}

olaVec4::olaVec4( const float x, const float y, const float z, const float w ) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

void olaVec4::Set( const float x, const float y, const float z, const float w ) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

void olaVec4::Zero( void ) {
	x = y = z = w = 0.0f;
}

void olaVec4::NoHomo()
{
	x /= w;	y /= w;	z /= w; w /= w;
}

float olaVec4::operator[]( int index ) const {
	return ( &x )[ index ];
}

float& olaVec4::operator[]( int index ) {
	return ( &x )[ index ];
}

olaVec4 olaVec4::operator-() const {
	return olaVec4( -x, -y, -z, -w );
}

olaVec4 olaVec4::operator-( const olaVec4 &a ) const {
	return olaVec4( x - a.x, y - a.y, z - a.z, w - a.w );
}

float olaVec4::operator*( const olaVec4 &a ) const {
	return x * a.x + y * a.y + z * a.z + w * a.w;
}

olaVec4 olaVec4::operator*( const float a ) const {
	return olaVec4( x * a, y * a, z * a, w * a );
}

olaVec4 olaVec4::operator/( const float a ) const {
	float inva = 1.0f / a;
	return olaVec4( x * inva, y * inva, z * inva, w * inva );
}

olaVec4 operator*( const float a, const olaVec4 b ) {
	return olaVec4( b.x * a, b.y * a, b.z * a, b.w * a );
}

olaVec4 olaVec4::operator+( const olaVec4 &a ) const {
	return olaVec4( x + a.x, y + a.y, z + a.z, w + a.w );
}

olaVec4 &olaVec4::operator+=( const olaVec4 &a ) {
	x += a.x;
	y += a.y;
	z += a.z;
	w += a.w;

	return *this;
}

olaVec4 &olaVec4::operator/=( const olaVec4 &a ) {
	x /= a.x;
	y /= a.y;
	z /= a.z;
	w /= a.w;

	return *this;
}

olaVec4 &olaVec4::operator/=( const float a ) {
	float inva = 1.0f / a;
	x *= inva;
	y *= inva;
	z *= inva;
	w *= inva;

	return *this;
}

olaVec4 &olaVec4::operator-=( const olaVec4 &a ) {
	x -= a.x;
	y -= a.y;
	z -= a.z;
	w -= a.w;

	return *this;
}

olaVec4 &olaVec4::operator*=( const float a ) {
	x *= a;
	y *= a;
	z *= a;
	w *= a;

	return *this;
}

bool olaVec4::Compare( const olaVec4 &a ) const {
	return ( ( x == a.x ) && ( y == a.y ) && ( z == a.z ) && w == a.w );
}

bool olaVec4::Compare( const olaVec4 &a, const float epsilon ) const {
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

bool olaVec4::operator==( const olaVec4 &a ) const {
	return Compare( a );
}

bool olaVec4::operator!=( const olaVec4 &a ) const {
	return !Compare( a );
}

float olaVec4::Length( void ) const {
	return ( float )olaMath::Sqrt( x * x + y * y + z * z + w * w );
}

float olaVec4::LengthSqr( void ) const {
	return ( x * x + y * y + z * z + w * w );
}

float olaVec4::Normalize( void ) {
	float sqrLength, invLength;

	sqrLength = x * x + y * y + z * z + w * w;
	invLength = olaMath::InvSqrt( sqrLength );
	x *= invLength;
	y *= invLength;
	z *= invLength;
	w *= invLength;
	return invLength * sqrLength;
}

float olaVec4::NormalizeFast( void ) {
	float sqrLength, invLength;

	sqrLength = x * x + y * y + z * z + w * w;
	invLength = olaMath::RSqrt( sqrLength );
	x *= invLength;
	y *= invLength;
	z *= invLength;
	w *= invLength;
	return invLength * sqrLength;
}

int olaVec4::GetDimension( void ) const {
	return 4;
}

const olaVec2 &olaVec4::ToVec2( void ) const {
	return *reinterpret_cast<const olaVec2 *>(this);
}

olaVec2 &olaVec4::ToVec2( void ) {
	return *reinterpret_cast<olaVec2 *>(this);
}

const olaVec3 &olaVec4::ToVec3( void ) const {
	return *reinterpret_cast<const olaVec3 *>(this);
}

olaVec3 &olaVec4::ToVec3( void ) {
	return *reinterpret_cast<olaVec3 *>(this);
}

const float *olaVec4::ToFloatPtr( void ) const {
	return &x;
}

float *olaVec4::ToFloatPtr( void ) {
	return &x;
}

olaPolar3::olaPolar3( void ) {
}

olaPolar3::olaPolar3( const float radius, const float theta, const float phi ) {
	assert( radius > 0 );
	this->radius = radius;
	this->theta = theta;
	this->phi = phi;
}
	
void olaPolar3::Set( const float radius, const float theta, const float phi ) {
	assert( radius > 0 );
	this->radius = radius;
	this->theta = theta;
	this->phi = phi;
}

float olaPolar3::operator[]( const int index ) const {
	return ( &radius )[ index ];
}

float &olaPolar3::operator[]( const int index ) {
	return ( &radius )[ index ];
}

olaPolar3 olaPolar3::operator-() const {
	return olaPolar3( radius, -theta, -phi );
}

olaPolar3 &olaPolar3::operator=( const olaPolar3 &a ) { 
	radius = a.radius;
	theta = a.theta;
	phi = a.phi;
	return *this;
}

olaVec3 olaPolar3::ToVec3( void ) const {
	float sp, cp, st, ct;
	olaMath::SinCos( phi, sp, cp );
	olaMath::SinCos( theta, st, ct );
 	return olaVec3( cp * radius * ct, cp * radius * st, radius * sp );
}

