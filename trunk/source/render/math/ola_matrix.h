// Copyright (C) 2004 Id Software, Inc.
//

#ifndef __MATH_MATRIX_H__
#define __MATH_MATRIX_H__

/*
===============================================================================

  Matrix classes, all matrices are row-major except olaMat3

===============================================================================
*/

#define MATRIX_INVERSE_EPSILON		1e-14
#define MATRIX_EPSILON				1e-6

#include "ola_mathbase.h"

class idAngles;
class olaQuat;
class olaCQuat;
class idRotation;
class olaMat4;

//===============================================================
//
//	olaMat2 - 2x2 matrix
//
//===============================================================

class olaMat2 {
public:
					olaMat2( void );
					explicit olaMat2( const olaVec2 &x, const olaVec2 &y );
					explicit olaMat2( const float xx, const float xy, const float yx, const float yy );
					explicit olaMat2( const float src[ 2 ][ 2 ] );

	const olaVec2 &	operator[]( int index ) const;
	olaVec2 &		operator[]( int index );
	olaMat2			operator-() const;
	olaMat2			operator*( const float a ) const;
	olaVec2			operator*( const olaVec2 &vec ) const;
	olaMat2			operator*( const olaMat2 &a ) const;
	olaMat2			operator+( const olaMat2 &a ) const;
	olaMat2			operator-( const olaMat2 &a ) const;
	olaMat2 &		operator*=( const float a );
	olaMat2 &		operator*=( const olaMat2 &a );
	olaMat2 &		operator+=( const olaMat2 &a );
	olaMat2 &		operator-=( const olaMat2 &a );

	friend olaMat2	operator*( const float a, const olaMat2 &mat );
	friend olaVec2	operator*( const olaVec2 &vec, const olaMat2 &mat );
	friend olaVec2 &	operator*=( olaVec2 &vec, const olaMat2 &mat );

	bool			Compare( const olaMat2 &a ) const;						// exact compare, no epsilon
	bool			Compare( const olaMat2 &a, const float epsilon ) const;	// compare with epsilon
	bool			operator==( const olaMat2 &a ) const;					// exact compare, no epsilon
	bool			operator!=( const olaMat2 &a ) const;					// exact compare, no epsilon

	void			Zero( void );
	void			Identity( void );
	bool			IsIdentity( const float epsilon = MATRIX_EPSILON ) const;
	bool			IsSymmetric( const float epsilon = MATRIX_EPSILON ) const;
	bool			IsDiagonal( const float epsilon = MATRIX_EPSILON ) const;

	float			Trace( void ) const;
	float			Determinant( void ) const;
	olaMat2			Transpose( void ) const;	// returns transpose
	olaMat2 &		TransposeSelf( void );
	olaMat2			Inverse( void ) const;		// returns the inverse ( m * m.Inverse() = identity )
	bool			InverseSelf( void );		// returns false if determinant is zero
	olaMat2			InverseFast( void ) const;	// returns the inverse ( m * m.Inverse() = identity )
	bool			InverseFastSelf( void );	// returns false if determinant is zero

	int				GetDimension( void ) const;

	const float *	ToFloatPtr( void ) const;
	float *			ToFloatPtr( void );
	const char *	ToString( int precision = 2 ) const;

private:
	olaVec2			mat[ 2 ];
};

extern olaMat2 mat2_zero;
extern olaMat2 mat2_identity;
#define mat2_default	mat2_identity

//===============================================================
//
//	olaMat3 - 3x3 matrix
//
//	NOTE:	matrix is column-major
//
//===============================================================

class olaMat3 {
public:
					olaMat3( void );
					explicit olaMat3( const olaVec3 &x, const olaVec3 &y, const olaVec3 &z );
					explicit olaMat3( const float xx, const float xy, const float xz, const float yx, const float yy, const float yz, const float zx, const float zy, const float zz );
					explicit olaMat3( const float src[ 3 ][ 3 ] );

	const olaVec3 &	operator[]( int index ) const;
	olaVec3 &		operator[]( int index );
	olaMat3			operator-() const;
	olaMat3			operator*( const float a ) const;
	olaVec3			operator*( const olaVec3 &vec ) const;
	olaMat3			operator*( const olaMat3 &a ) const;
	olaMat3			operator+( const olaMat3 &a ) const;
	olaMat3			operator-( const olaMat3 &a ) const;
	olaMat3 &		operator*=( const float a );
	olaMat3 &		operator*=( const olaMat3 &a );
	olaMat3 &		operator+=( const olaMat3 &a );
	olaMat3 &		operator-=( const olaMat3 &a );

	friend olaMat3	operator*( const float a, const olaMat3 &mat );
	friend olaVec3	operator*( const olaVec3 &vec, const olaMat3 &mat );
	friend olaVec3 &	operator*=( olaVec3 &vec, const olaMat3 &mat );

	bool			Compare( const olaMat3 &a ) const;						// exact compare, no epsilon
	bool			Compare( const olaMat3 &a, const float epsilon ) const;	// compare with epsilon
	bool			operator==( const olaMat3 &a ) const;					// exact compare, no epsilon
	bool			operator!=( const olaMat3 &a ) const;					// exact compare, no epsilon

	void			Zero( void );
	void			Identity( void );
	bool			IsIdentity( const float epsilon = MATRIX_EPSILON ) const;
	bool			IsSymmetric( const float epsilon = MATRIX_EPSILON ) const;
	bool			IsDiagonal( const float epsilon = MATRIX_EPSILON ) const;
	bool			IsRotated( void ) const;

	void			ProjectVector( const olaVec3 &src, olaVec3 &dst ) const;
	void			UnprojectVector( const olaVec3 &src, olaVec3 &dst ) const;

	bool			FixDegeneracies( void );	// fix degenerate axial cases
	bool			FixDenormals( void );		// change tiny numbers to zero

	float			Trace( void ) const;
	float			Determinant( void ) const;
	olaMat3			OrthoNormalize( void ) const;
	olaMat3 &		OrthoNormalizeSelf( void );
	olaMat3			Transpose( void ) const;	// returns transpose
	olaMat3 &		TransposeSelf( void );
	olaMat3			Inverse( void ) const;		// returns the inverse ( m * m.Inverse() = identity )
	bool			InverseSelf( void );		// returns false if determinant is zero
	olaMat3			InverseFast( void ) const;	// returns the inverse ( m * m.Inverse() = identity )
	bool			InverseFastSelf( void );	// returns false if determinant is zero
	olaMat3			TransposeMultiply( const olaMat3 &b ) const;

	olaMat3			InertiaTranslate( const float mass, const olaVec3 &centerOfMass, const olaVec3 &translation ) const;
	olaMat3 &		InertiaTranslateSelf( const float mass, const olaVec3 &centerOfMass, const olaVec3 &translation );
	olaMat3			InertiaRotate( const olaMat3 &rotation ) const;
	olaMat3 &		InertiaRotateSelf( const olaMat3 &rotation );

	int				GetDimension( void ) const;

	idAngles		ToAngles( void ) const;
	olaQuat			ToQuat( void ) const;
	olaCQuat			ToCQuat( void ) const;
	idRotation		ToRotation( void ) const;
	olaMat4			ToMat4( void ) const;
	olaVec3			ToAngularVelocity( void ) const;
	const float *	ToFloatPtr( void ) const;
	float *			ToFloatPtr( void );
	const char *	ToString( int precision = 2 ) const;

	friend void		TransposeMultiply( const olaMat3 &inv, const olaMat3 &b, olaMat3 &dst );
	friend olaMat3	SkewSymmetric( olaVec3 const &src );

	void			CopyToFloatArrayColumn(float* m33);

private:
	olaVec3			mat[ 3 ];
};

extern olaMat3 mat3_zero;
extern olaMat3 mat3_identity;
#define mat3_default	mat3_identity



//===============================================================
//
//	olaMat4 - 4x4 matrix
//
//===============================================================

class olaMat4 {
public:
					olaMat4( void );
					explicit olaMat4( const olaVec4 &x, const olaVec4 &y, const olaVec4 &z, const olaVec4 &w );
					explicit olaMat4(const float xx, const float xy, const float xz, const float xw,
									const float yx, const float yy, const float yz, const float yw,
									const float zx, const float zy, const float zz, const float zw,
									const float wx, const float wy, const float wz, const float ww );
					explicit olaMat4( const olaMat3 &rotation, const olaVec3 &translation );
					explicit olaMat4( const float src[ 4 ][ 4 ] );

	const olaVec4 &	operator[]( int index ) const;
	olaVec4 &		operator[]( int index );
	olaMat4			operator*( const float a ) const;
	olaVec4			operator*( const olaVec4 &vec ) const;
	olaVec3			operator*( const olaVec3 &vec ) const;
	olaMat4			operator*( const olaMat4 &a ) const;
	olaMat4			operator+( const olaMat4 &a ) const;
	olaMat4			operator-( const olaMat4 &a ) const;
	olaMat4 &		operator*=( const float a );
	olaMat4 &		operator*=( const olaMat4 &a );
	olaMat4 &		operator+=( const olaMat4 &a );
	olaMat4 &		operator-=( const olaMat4 &a );

	friend olaMat4	operator*( const float a, const olaMat4 &mat );
	friend olaVec4	operator*( const olaVec4 &vec, const olaMat4 &mat );
	friend olaVec3	operator*( const olaVec3 &vec, const olaMat4 &mat );
	friend olaVec4 &	operator*=( olaVec4 &vec, const olaMat4 &mat );
	friend olaVec3 &	operator*=( olaVec3 &vec, const olaMat4 &mat );

	bool			Compare( const olaMat4 &a ) const;						// exact compare, no epsilon
	bool			Compare( const olaMat4 &a, const float epsilon ) const;	// compare with epsilon
	bool			operator==( const olaMat4 &a ) const;					// exact compare, no epsilon
	bool			operator!=( const olaMat4 &a ) const;					// exact compare, no epsilon

	void			Zero( void );
	void			Identity( void );
	bool			IsIdentity( const float epsilon = MATRIX_EPSILON ) const;
	bool			IsSymmetric( const float epsilon = MATRIX_EPSILON ) const;
	bool			IsDiagonal( const float epsilon = MATRIX_EPSILON ) const;
	bool			IsRotated( void ) const;

	void			ProjectVector( const olaVec4 &src, olaVec4 &dst ) const;
	void			UnprojectVector( const olaVec4 &src, olaVec4 &dst ) const;

	float			Trace( void ) const;
	float			Determinant( void ) const;
	olaMat4			Transpose( void ) const;	// returns transpose
	olaMat4 &		TransposeSelf( void );
	olaMat4			Inverse( void ) const;		// returns the inverse ( m * m.Inverse() = identity )
	bool			InverseSelf( void );		// returns false if determinant is zero
	olaMat4			InverseFast( void ) const;	// returns the inverse ( m * m.Inverse() = identity )
	bool			InverseFastSelf( void );	// returns false if determinant is zero
	olaMat4			TransposeMultiply( const olaMat4 &b ) const;

	int				GetDimension( void ) const;

	const float *	ToFloatPtr( void ) const;
	float *			ToFloatPtr( void );
	const char *	ToString( int precision = 2 ) const;
	olaMat3			ToMatrix3(void)const;

	olaVec3			TransformNormal(olaVec3& vec);
	olaVec3			TransformCoord(olaVec3& vec);

	void			Compose(const olaQuat& rotation, const olaVec3& translation);
	void			Decompose(olaVec3& scale, olaQuat& rotation, olaVec3& position)const;

	void			Multiply(const olaMat4& left, const olaMat4& right);

	void			CopyToFloatArrayColumn(float* m44);
	void			FromFloatsColumnMajor(float* pOut);
	void			Set(const float* m44);
private:
	olaVec4			mat[ 4 ];
};

extern olaMat4 mat4_zero;
extern olaMat4 mat4_identity;
#define mat4_default	mat4_identity

#endif /* !__MATH_MATRIX_H__ */
