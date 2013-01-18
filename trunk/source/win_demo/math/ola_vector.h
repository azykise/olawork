// Copyright (C) 2004 Id Software, Inc.
//

#ifndef __MATH_VECTOR_H__
#define __MATH_VECTOR_H__

#include "ola_mathbase.h"

/*
===============================================================================

  Vector classes

===============================================================================
*/

#define VECTOR_EPSILON		0.001f

class idAngles;
class olaPolar3;
class olaMat3;

//===============================================================
//
//	olaVec2 - 2D vector
//
//===============================================================

class olaVec2 {
public:
	float			x;
	float			y;

					olaVec2( void );
					explicit olaVec2( const float x, const float y );

	void 			Set( const float x, const float y );
	void			Zero( void );

	float			operator[]( int index ) const;
	float &			operator[]( int index );
	olaVec2			operator-() const;
	float			operator*( const olaVec2 &a ) const;
	olaVec2			operator*( const float a ) const;
	olaVec2			operator/( const float a ) const;
	olaVec2			operator+( const olaVec2 &a ) const;
	olaVec2			operator-( const olaVec2 &a ) const;
	olaVec2 &		operator+=( const olaVec2 &a );
	olaVec2 &		operator-=( const olaVec2 &a );
	olaVec2 &		operator/=( const olaVec2 &a );
	olaVec2 &		operator/=( const float a );
	olaVec2 &		operator*=( const float a );

	friend olaVec2	operator*( const float a, const olaVec2 b );

	bool			Compare( const olaVec2 &a ) const;							// exact compare, no epsilon
	bool			Compare( const olaVec2 &a, const float epsilon ) const;		// compare with epsilon
	bool			operator==(	const olaVec2 &a ) const;						// exact compare, no epsilon
	bool			operator!=(	const olaVec2 &a ) const;						// exact compare, no epsilon

	float			Length( void ) const;
	float			LengthFast( void ) const;
	float			LengthSqr( void ) const;
	float			Normalize( void );			// returns length
	float			NormalizeFast( void );		// returns length
	olaVec2 &		Truncate( float length );	// cap length
	void			Clamp( const olaVec2 &min, const olaVec2 &max );
	void			Snap( void );				// snap to closest integer value
	void			SnapInt( void );			// snap towards integer (floor)

	int				GetDimension( void ) const;

	const float *	ToFloatPtr( void ) const;
	float *			ToFloatPtr( void );
	const char *	ToString( int precision = 2 ) const;

	void			Lerp( const olaVec2 &v1, const olaVec2 &v2, const float l );
};

extern olaVec2 vec2_origin;
#define vec2_zero vec2_origin

//===============================================================
//
//	olaVec3 - 3D vector
//
//===============================================================

class olaVec3 {
public:	
	float			x;
	float			y;
	float			z;

					olaVec3( void );
					explicit olaVec3( const float x, const float y, const float z );

	void 			Set( const float x, const float y, const float z );
	void			Zero( void );

	float			operator[]( const int index ) const;
	float &			operator[]( const int index );
	olaVec3			operator-() const;
	olaVec3 &		operator=( const olaVec3 &a );		// required because of a msvc 6 & 7 bug
	float			operator*( const olaVec3 &a ) const;
	olaVec3			operator*( const float a ) const;
	olaVec3			operator/( const float a ) const;
	olaVec3			operator+( const olaVec3 &a ) const;
	olaVec3			operator-( const olaVec3 &a ) const;
	olaVec3 &		operator+=( const olaVec3 &a );
	olaVec3 &		operator-=( const olaVec3 &a );
	olaVec3 &		operator/=( const olaVec3 &a );
	olaVec3 &		operator/=( const float a );
	olaVec3 &		operator*=( const float a );

	friend olaVec3	operator*( const float a, const olaVec3 b );

	bool			Compare( const olaVec3 &a ) const;							// exact compare, no epsilon
	bool			Compare( const olaVec3 &a, const float epsilon ) const;		// compare with epsilon
	bool			operator==(	const olaVec3 &a ) const;						// exact compare, no epsilon
	bool			operator!=(	const olaVec3 &a ) const;						// exact compare, no epsilon

	bool			FixDegenerateNormal( void );	// fix degenerate axial cases
	bool			FixDenormals( void );			// change tiny numbers to zero

	olaVec3			Cross( const olaVec3 &a ) const;
	olaVec3 &		Cross( const olaVec3 &a, const olaVec3 &b );
	float			Length( void ) const;
	float			LengthSqr( void ) const;
	float			LengthFast( void ) const;
	float			Normalize( void );				// returns length
	float			NormalizeFast( void );			// returns length
	olaVec3 &		Truncate( float length );		// cap length
	void			Clamp( const olaVec3 &min, const olaVec3 &max );
	void			Snap( void );					// snap to closest integer value
	void			SnapInt( void );				// snap towards integer (floor)

	int				GetDimension( void ) const;

	float			ToYaw( void ) const;
	float			ToPitch( void ) const;
	idAngles		ToAngles( void ) const;
	olaPolar3		ToPolar( void ) const;
	olaMat3			ToMat3( void ) const;		// vector should be normalized
	const olaVec2 &	ToVec2( void ) const;
	olaVec2 &		ToVec2( void );
	const float *	ToFloatPtr( void ) const;
	float *			ToFloatPtr( void );
	const char *	ToString( int precision = 2 ) const;

	void			NormalVectors( olaVec3 &left, olaVec3 &down ) const;	// vector should be normalized
	void			OrthogonalBasis( olaVec3 &left, olaVec3 &up ) const;

	void			ProjectOntoPlane( const olaVec3 &normal, const float overBounce = 1.0f );
	bool			ProjectAlongPlane( const olaVec3 &normal, const float epsilon, const float overBounce = 1.0f );
	void			ProjectSelfOntoSphere( const float radius );

	void			Lerp( const olaVec3 &v1, const olaVec3 &v2, const float l );
	void			SLerp( const olaVec3 &v1, const olaVec3 &v2, const float l );
};

extern olaVec3 vec3_origin;
#define vec3_zero vec3_origin


//===============================================================
//
//	olaVec4 - 4D vector
//
//===============================================================

class olaVec4 {
public:	
	float			x;
	float			y;
	float			z;
	float			w;

					olaVec4( void );
					explicit olaVec4( const float x, const float y, const float z, const float w );

	void 			Set( const float x, const float y, const float z, const float w );
	void			Zero( void );

	float			operator[]( const int index ) const;
	float &			operator[]( const int index );
	olaVec4			operator-() const;
	float			operator*( const olaVec4 &a ) const;
	olaVec4			operator*( const float a ) const;
	olaVec4			operator/( const float a ) const;
	olaVec4			operator+( const olaVec4 &a ) const;
	olaVec4			operator-( const olaVec4 &a ) const;
	olaVec4 &		operator+=( const olaVec4 &a );
	olaVec4 &		operator-=( const olaVec4 &a );
	olaVec4 &		operator/=( const olaVec4 &a );
	olaVec4 &		operator/=( const float a );
	olaVec4 &		operator*=( const float a );

	friend olaVec4	operator*( const float a, const olaVec4 b );

	bool			Compare( const olaVec4 &a ) const;							// exact compare, no epsilon
	bool			Compare( const olaVec4 &a, const float epsilon ) const;		// compare with epsilon
	bool			operator==(	const olaVec4 &a ) const;						// exact compare, no epsilon
	bool			operator!=(	const olaVec4 &a ) const;						// exact compare, no epsilon

	float			Length( void ) const;
	float			LengthSqr( void ) const;
	float			Normalize( void );			// returns length
	float			NormalizeFast( void );		// returns length

	int				GetDimension( void ) const;

	const olaVec2 &	ToVec2( void ) const;
	olaVec2 &		ToVec2( void );
	const olaVec3 &	ToVec3( void ) const;
	olaVec3 &		ToVec3( void );
	const float *	ToFloatPtr( void ) const;
	float *			ToFloatPtr( void );
	const char *	ToString( int precision = 2 ) const;

	void			Lerp( const olaVec4 &v1, const olaVec4 &v2, const float l );
	void			NoHomo();
};

extern olaVec4 vec4_origin;
#define vec4_zero vec4_origin;

//===============================================================
//
//	idPolar3
//
//===============================================================

class olaPolar3 {
public:	
	float			radius, theta, phi;

					olaPolar3( void );
					explicit olaPolar3( const float radius, const float theta, const float phi );

	void 			Set( const float radius, const float theta, const float phi );

	float			operator[]( const int index ) const;
	float &			operator[]( const int index );
	olaPolar3		operator-() const;
	olaPolar3 &		operator=( const olaPolar3 &a );

	olaVec3			ToVec3( void ) const;
};


/*
===============================================================================

	Old 3D vector macros, should no longer be used.

===============================================================================
*/

#define DotProduct( a, b)			((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VectorSubtract( a, b, c )	((c)[0]=(a)[0]-(b)[0],(c)[1]=(a)[1]-(b)[1],(c)[2]=(a)[2]-(b)[2])
#define VectorAdd( a, b, c )		((c)[0]=(a)[0]+(b)[0],(c)[1]=(a)[1]+(b)[1],(c)[2]=(a)[2]+(b)[2])
#define	VectorScale( v, s, o )		((o)[0]=(v)[0]*(s),(o)[1]=(v)[1]*(s),(o)[2]=(v)[2]*(s))
#define	VectorMA( v, s, b, o )		((o)[0]=(v)[0]+(b)[0]*(s),(o)[1]=(v)[1]+(b)[1]*(s),(o)[2]=(v)[2]+(b)[2]*(s))
#define VectorCopy( a, b )			((b)[0]=(a)[0],(b)[1]=(a)[1],(b)[2]=(a)[2])


#endif /* !__MATH_VECTOR_H__ */
