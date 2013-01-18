// Copyright (C) 2004 Id Software, Inc.
//

#ifndef __MATH_QUAT_H__
#define __MATH_QUAT_H__

/*
===============================================================================

	Quaternion

===============================================================================
*/

#include "ola_mathbase.h"

class olaVec3;
class idAngles;
class idRotation;
class olaMat3;
class olaMat4;
class olaCQuat;

class olaQuat {
public:
	float			x;
	float			y;
	float			z;
	float			w;

					olaQuat( void );
					olaQuat( float x, float y, float z, float w );

	void 			Set( float x, float y, float z, float w );

	float			operator[]( int index ) const;
	float &			operator[]( int index );
	olaQuat			operator-() const;
	olaQuat &		operator=( const olaQuat &a );
	olaQuat			operator+( const olaQuat &a ) const;
	olaQuat &		operator+=( const olaQuat &a );
	olaQuat			operator-( const olaQuat &a ) const;
	olaQuat &		operator-=( const olaQuat &a );
	olaQuat			operator*( const olaQuat &a ) const;
	olaVec3			operator*( const olaVec3 &a ) const;
	olaQuat			operator*( float a ) const;
	olaQuat &		operator*=( const olaQuat &a );
	olaQuat &		operator*=( float a );

	friend olaQuat	operator*( const float a, const olaQuat &b );
	friend olaVec3	operator*( const olaVec3 &a, const olaQuat &b );

	bool			Compare( const olaQuat &a ) const;						// exact compare, no epsilon
	bool			Compare( const olaQuat &a, const float epsilon ) const;	// compare with epsilon
	bool			operator==(	const olaQuat &a ) const;					// exact compare, no epsilon
	bool			operator!=(	const olaQuat &a ) const;					// exact compare, no epsilon

	olaQuat			Inverse( void ) const;
	float			Length( void ) const;
	olaQuat &		Normalize( void );

	float			CalcW( void ) const;
	int				GetDimension( void ) const;

	idAngles		ToAngles( void ) const;
	idRotation		ToRotation( void ) const;
	olaMat3			ToMat3( void ) const;
	olaMat4			ToMat4( void ) const;
	olaCQuat			ToCQuat( void ) const;
	olaVec3			ToAngularVelocity( void ) const;
	const float *	ToFloatPtr( void ) const;
	float *			ToFloatPtr( void );
	const char *	ToString( int precision = 2 ) const;

	olaQuat &		Slerp( const olaQuat &from, const olaQuat &to, float t );

	olaQuat &		FromAxisAngle(olaVec3& axis,float degree); 

	olaQuat&		Conjugate();
};


/*
===============================================================================

	Compressed quaternion

===============================================================================
*/

class olaCQuat {
public:
	float			x;
	float			y;
	float			z;

					olaCQuat( void );
					olaCQuat( float x, float y, float z );

	void 			Set( float x, float y, float z );

	float			operator[]( int index ) const;
	float &			operator[]( int index );

	bool			Compare( const olaCQuat &a ) const;						// exact compare, no epsilon
	bool			Compare( const olaCQuat &a, const float epsilon ) const;	// compare with epsilon
	bool			operator==(	const olaCQuat &a ) const;					// exact compare, no epsilon
	bool			operator!=(	const olaCQuat &a ) const;					// exact compare, no epsilon

	int				GetDimension( void ) const;

	idAngles		ToAngles( void ) const;
	idRotation		ToRotation( void ) const;
	olaMat3			ToMat3( void ) const;
	olaMat4			ToMat4( void ) const;
	olaQuat			ToQuat( void ) const;
	const float *	ToFloatPtr( void ) const;
	float *			ToFloatPtr( void );
	const char *	ToString( int precision = 2 ) const;
};

#endif /* !__MATH_QUAT_H__ */
