#include "ola_mathbase.h"

#pragma hdrstop


const float	olaMath::PI				= 3.14159265358979323846f;
const float	olaMath::TWO_PI			= 2.0f * PI;
const float	olaMath::HALF_PI			= 0.5f * PI;
const float	olaMath::ONEFOURTH_PI	= 0.25f * PI;
const float olaMath::E				= 2.71828182845904523536f;
const float olaMath::SQRT_TWO		= 1.41421356237309504880f;
const float olaMath::SQRT_THREE		= 1.73205080756887729352f;
const float	olaMath::SQRT_1OVER2		= 0.70710678118654752440f;
const float	olaMath::SQRT_1OVER3		= 0.57735026918962576450f;
const float	olaMath::M_DEG2RAD		= PI / 180.0f;
const float	olaMath::M_RAD2DEG		= 180.0f / PI;
const float	olaMath::M_SEC2MS		= 1000.0f;
const float	olaMath::M_MS2SEC		= 0.001f;
const float	olaMath::INFINITY		= 1e30f;
//const float olaMath::FLT_EPSILON		= 1.192092896e-07f;

bool		olaMath::initialized		= false;
dword		olaMath::iSqrt[SQRT_TABLE_SIZE];		// inverse square root lookup table

/*
===============
olaMath::Init
===============
*/
void olaMath::Init( void ) {
    union _flint fi, fo;

    for ( int i = 0; i < SQRT_TABLE_SIZE; i++ ) {
        fi.i	 = ((EXP_BIAS-1) << EXP_POS) | (i << LOOKUP_POS);
        fo.f	 = (float)( 1.0 / sqrt( fi.f ) );
        iSqrt[i] = ((dword)(((fo.i + (1<<(SEED_POS-2))) >> SEED_POS) & 0xFF))<<SEED_POS;
    }
    
	iSqrt[SQRT_TABLE_SIZE / 2] = ((dword)(0xFF))<<(SEED_POS); 

	initialized = true;
}

/*
================
olaMath::FloatToBits
================
*/
int olaMath::FloatToBits( float f, int exponentBits, int mantissaBits ) {
	int i, sign, exponent, mantissa, value;

	assert( exponentBits >= 2 && exponentBits <= 8 );
	assert( mantissaBits >= 2 && mantissaBits <= 23 );

	int maxBits = ( ( ( 1 << ( exponentBits - 1 ) ) - 1 ) << mantissaBits ) | ( ( 1 << mantissaBits ) - 1 );
	int minBits = ( ( ( 1 <<   exponentBits       ) - 2 ) << mantissaBits ) | 1;

	float max = BitsToFloat( maxBits, exponentBits, mantissaBits );
	float min = BitsToFloat( minBits, exponentBits, mantissaBits );

	if ( f >= 0.0f ) {
		if ( f >= max ) {
			return maxBits;
		} else if ( f <= min ) {
			return minBits;
		}
	} else {
		if ( f <= -max ) {
			return ( maxBits | ( 1 << ( exponentBits + mantissaBits ) ) );
		} else if ( f >= -min ) {
			return ( minBits | ( 1 << ( exponentBits + mantissaBits ) ) );
		}
	}

	exponentBits--;
	i = *reinterpret_cast<int *>(&f);
	sign = ( i >> IEEE_FLT_SIGN_BIT ) & 1;
	exponent = ( ( i >> IEEE_FLT_MANTISSA_BITS ) & ( ( 1 << IEEE_FLT_EXPONENT_BITS ) - 1 ) ) - IEEE_FLT_EXPONENT_BIAS;
	mantissa = i & ( ( 1 << IEEE_FLT_MANTISSA_BITS ) - 1 );
	value = sign << ( 1 + exponentBits + mantissaBits );
	value |= ( ( INTSIGNBITSET( exponent ) << exponentBits ) | ( abs( exponent ) & ( ( 1 << exponentBits ) - 1 ) ) ) << mantissaBits;
	value |= mantissa >> ( IEEE_FLT_MANTISSA_BITS - mantissaBits );
	return value;
}

/*
================
olaMath::BitsToFloat
================
*/
float olaMath::BitsToFloat( int i, int exponentBits, int mantissaBits ) {
	static int exponentSign[2] = { 1, -1 };
	int sign, exponent, mantissa, value;

	assert( exponentBits >= 2 && exponentBits <= 8 );
	assert( mantissaBits >= 2 && mantissaBits <= 23 );

	exponentBits--;
	sign = i >> ( 1 + exponentBits + mantissaBits );
	exponent = ( ( i >> mantissaBits ) & ( ( 1 << exponentBits ) - 1 ) ) * exponentSign[( i >> ( exponentBits + mantissaBits ) ) & 1];
	mantissa = ( i & ( ( 1 << mantissaBits ) - 1 ) ) << ( IEEE_FLT_MANTISSA_BITS - mantissaBits );
	value = sign << IEEE_FLT_SIGN_BIT | ( exponent + IEEE_FLT_EXPONENT_BIAS ) << IEEE_FLT_MANTISSA_BITS | mantissa;
	return *reinterpret_cast<float *>(&value);
}


float olaMath::RSqrt( float x ) {

	long i;
	float y, r;

	y = x * 0.5f;
	i = *reinterpret_cast<long *>( &x );
	i = 0x5f3759df - ( i >> 1 );
	r = *reinterpret_cast<float *>( &i );
	r = r * ( 1.5f - r * r * y );
	return r;
}

float olaMath::InvSqrt16( float x ) {

	dword a = ((union _flint*)(&x))->i;
	union _flint seed;

	assert( initialized );

	double y = x * 0.5f;
	seed.i = (( ( (3*EXP_BIAS-1) - ( (a >> EXP_POS) & 0xFF) ) >> 1)<<EXP_POS) | iSqrt[(a >> (EXP_POS-LOOKUP_BITS)) & LOOKUP_MASK];
	double r = seed.f;
	r = r * ( 1.5f - r * r * y );
	return (float) r;
}

float olaMath::InvSqrt( float x ) {

	dword a = ((union _flint*)(&x))->i;
	union _flint seed;

	assert( initialized );

	double y = x * 0.5f;
	seed.i = (( ( (3*EXP_BIAS-1) - ( (a >> EXP_POS) & 0xFF) ) >> 1)<<EXP_POS) | iSqrt[(a >> (EXP_POS-LOOKUP_BITS)) & LOOKUP_MASK];
	double r = seed.f;
	r = r * ( 1.5f - r * r * y );
	r = r * ( 1.5f - r * r * y );
	return (float) r;
}

double olaMath::InvSqrt64( float x ) {
	dword a = ((union _flint*)(&x))->i;
	union _flint seed;

	assert( initialized );

	double y = x * 0.5f;
	seed.i = (( ( (3*EXP_BIAS-1) - ( (a >> EXP_POS) & 0xFF) ) >> 1)<<EXP_POS) | iSqrt[(a >> (EXP_POS-LOOKUP_BITS)) & LOOKUP_MASK];
	double r = seed.f;
	r = r * ( 1.5f - r * r * y );
	r = r * ( 1.5f - r * r * y );
	r = r * ( 1.5f - r * r * y );
	return r;
}

float olaMath::Sqrt16( float x ) {
	return x * InvSqrt16( x );
}

float olaMath::Sqrt( float x ) {
	return x * InvSqrt( x );
}

double olaMath::Sqrt64( float x ) {
	return x * InvSqrt64( x );
}

float olaMath::Sin( float a ) {
	return sinf( a );
}

float olaMath::Sin16( float a ) {
	float s;

	if ( ( a < 0.0f ) || ( a >= TWO_PI ) ) {
		a -= floorf( a / TWO_PI ) * TWO_PI;
	}
#if 1
	if ( a < PI ) {
		if ( a > HALF_PI ) {
			a = PI - a;
		}
	} else {
		if ( a > PI + HALF_PI ) {
			a = a - TWO_PI;
		} else {
			a = PI - a;
		}
	}
#else
	a = PI - a;
	if ( fabs( a ) >= HALF_PI ) {
		a = ( ( a < 0.0f ) ? -PI : PI ) - a;
	}
#endif
	s = a * a;
	return a * ( ( ( ( ( -2.39e-08f * s + 2.7526e-06f ) * s - 1.98409e-04f ) * s + 8.3333315e-03f ) * s - 1.666666664e-01f ) * s + 1.0f );
}

double olaMath::Sin64( float a ) {
	return sin( a );
}

float olaMath::Cos( float a ) {
	return cosf( a );
}

float olaMath::Cos16( float a ) {
	float s, d;

	if ( ( a < 0.0f ) || ( a >= TWO_PI ) ) {
		a -= floorf( a / TWO_PI ) * TWO_PI;
	}
#if 1
	if ( a < PI ) {
		if ( a > HALF_PI ) {
			a = PI - a;
			d = -1.0f;
		} else {
			d = 1.0f;
		}
	} else {
		if ( a > PI + HALF_PI ) {
			a = a - TWO_PI;
			d = 1.0f;
		} else {
			a = PI - a;
			d = -1.0f;
		}
	}
#else
	a = PI - a;
	if ( fabs( a ) >= HALF_PI ) {
		a = ( ( a < 0.0f ) ? -PI : PI ) - a;
		d = 1.0f;
	} else {
		d = -1.0f;
	}
#endif
	s = a * a;
	return d * ( ( ( ( ( -2.605e-07f * s + 2.47609e-05f ) * s - 1.3888397e-03f ) * s + 4.16666418e-02f ) * s - 4.999999963e-01f ) * s + 1.0f );
}

double olaMath::Cos64( float a ) {
	return cos( a );
}

void olaMath::SinCos( float a, float &s, float &c ) {
	s = sinf( a );
	c = cosf( a );

}

void olaMath::SinCos16( float a, float &s, float &c ) {
	float t, d;

	if ( ( a < 0.0f ) || ( a >= olaMath::TWO_PI ) ) {
		a -= floorf( a / olaMath::TWO_PI ) * olaMath::TWO_PI;
	}
#if 1
	if ( a < PI ) {
		if ( a > HALF_PI ) {
			a = PI - a;
			d = -1.0f;
		} else {
			d = 1.0f;
		}
	} else {
		if ( a > PI + HALF_PI ) {
			a = a - TWO_PI;
			d = 1.0f;
		} else {
			a = PI - a;
			d = -1.0f;
		}
	}
#else
	a = PI - a;
	if ( fabs( a ) >= HALF_PI ) {
		a = ( ( a < 0.0f ) ? -PI : PI ) - a;
		d = 1.0f;
	} else {
		d = -1.0f;
	}
#endif
	t = a * a;
	s = a * ( ( ( ( ( -2.39e-08f * t + 2.7526e-06f ) * t - 1.98409e-04f ) * t + 8.3333315e-03f ) * t - 1.666666664e-01f ) * t + 1.0f );
	c = d * ( ( ( ( ( -2.605e-07f * t + 2.47609e-05f ) * t - 1.3888397e-03f ) * t + 4.16666418e-02f ) * t - 4.999999963e-01f ) * t + 1.0f );
}

void olaMath::SinCos64( float a, double &s, double &c ) {
#ifdef _WIN32
	_asm {
		fld		a
		fsincos
		mov		ecx, c
		mov		edx, s
		fstp	qword ptr [ecx]
		fstp	qword ptr [edx]
	}
#else
	s = sin( a );
	c = cos( a );
#endif
}

float olaMath::Tan( float a ) {
	return tanf( a );
}

float olaMath::Tan16( float a ) {
	float s;
	bool reciprocal;

	if ( ( a < 0.0f ) || ( a >= PI ) ) {
		a -= floorf( a / PI ) * PI;
	}
#if 1
	if ( a < HALF_PI ) {
		if ( a > ONEFOURTH_PI ) {
			a = HALF_PI - a;
			reciprocal = true;
		} else {
			reciprocal = false;
		}
	} else {
		if ( a > HALF_PI + ONEFOURTH_PI ) {
			a = a - PI;
			reciprocal = false;
		} else {
			a = HALF_PI - a;
			reciprocal = true;
		}
	}
#else
	a = HALF_PI - a;
	if ( fabs( a ) >= ONEFOURTH_PI ) {
		a = ( ( a < 0.0f ) ? -HALF_PI : HALF_PI ) - a;
		reciprocal = false;
	} else {
		reciprocal = true;
	}
#endif
	s = a * a;
	s = a * ( ( ( ( ( ( 9.5168091e-03f * s + 2.900525e-03f ) * s + 2.45650893e-02f ) * s + 5.33740603e-02f ) * s + 1.333923995e-01f ) * s + 3.333314036e-01f ) * s + 1.0f );
	if ( reciprocal ) {
		return 1.0f / s;
	} else {
		return s;
	}
}

double olaMath::Tan64( float a ) {
	return tan( a );
}

float olaMath::ASin( float a ) {
	if ( a <= -1.0f ) {
		return -HALF_PI;
	}
	if ( a >= 1.0f ) {
		return HALF_PI;
	}
	return asinf( a );
}

float olaMath::ASin16( float a ) {
	if ( FLOATSIGNBITSET( a ) ) {
		if ( a <= -1.0f ) {
			return -HALF_PI;
		}
		a = fabs( a );
		return ( ( ( -0.0187293f * a + 0.0742610f ) * a - 0.2121144f ) * a + 1.5707288f ) * sqrt( 1.0f - a ) - HALF_PI;
	} else {
		if ( a >= 1.0f ) {
			return HALF_PI;
		}
		return HALF_PI - ( ( ( -0.0187293f * a + 0.0742610f ) * a - 0.2121144f ) * a + 1.5707288f ) * sqrt( 1.0f - a );
	}
}

double olaMath::ASin64( float a ) {
	if ( a <= -1.0f ) {
		return -HALF_PI;
	}
	if ( a >= 1.0f ) {
		return HALF_PI;
	}
	return asin( a );
}

float olaMath::ACos( float a ) {
	if ( a <= -1.0f ) {
		return PI;
	}
	if ( a >= 1.0f ) {
		return 0.0f;
	}
	return acosf( a );
}

float olaMath::ACos16( float a ) {
	if ( FLOATSIGNBITSET( a ) ) {
		if ( a <= -1.0f ) {
			return PI;
		}
		a = fabs( a );
		return PI - ( ( ( -0.0187293f * a + 0.0742610f ) * a - 0.2121144f ) * a + 1.5707288f ) * sqrt( 1.0f - a );
	} else {
		if ( a >= 1.0f ) {
			return 0.0f;
		}
		return ( ( ( -0.0187293f * a + 0.0742610f ) * a - 0.2121144f ) * a + 1.5707288f ) * sqrt( 1.0f - a );
	}
}

double olaMath::ACos64( float a ) {
	if ( a <= -1.0f ) {
		return PI;
	}
	if ( a >= 1.0f ) {
		return 0.0f;
	}
	return acos( a );
}

float olaMath::ATan( float a ) {
	return atanf( a );
}

float olaMath::ATan16( float a ) {
	float s;

	if ( fabs( a ) > 1.0f ) {
		a = 1.0f / a;
		s = a * a;
		s = - ( ( ( ( ( ( ( ( ( 0.0028662257f * s - 0.0161657367f ) * s + 0.0429096138f ) * s - 0.0752896400f )
				* s + 0.1065626393f ) * s - 0.1420889944f ) * s + 0.1999355085f ) * s - 0.3333314528f ) * s ) + 1.0f ) * a;
		if ( FLOATSIGNBITSET( a ) ) {
			return s - HALF_PI;
		} else {
			return s + HALF_PI;
		}
	} else {
		s = a * a;
		return ( ( ( ( ( ( ( ( ( 0.0028662257f * s - 0.0161657367f ) * s + 0.0429096138f ) * s - 0.0752896400f )
			* s + 0.1065626393f ) * s - 0.1420889944f ) * s + 0.1999355085f ) * s - 0.3333314528f ) * s ) + 1.0f ) * a;
	}
}

double olaMath::ATan64( float a ) {
	return atan( a );
}

float olaMath::ATan( float y, float x ) {
	return atan2f( y, x );
}

float olaMath::ATan16( float y, float x ) {
	float a, s;

	if ( fabs( y ) > fabs( x ) ) {
		a = x / y;
		s = a * a;
		s = - ( ( ( ( ( ( ( ( ( 0.0028662257f * s - 0.0161657367f ) * s + 0.0429096138f ) * s - 0.0752896400f )
				* s + 0.1065626393f ) * s - 0.1420889944f ) * s + 0.1999355085f ) * s - 0.3333314528f ) * s ) + 1.0f ) * a;
		if ( FLOATSIGNBITSET( a ) ) {
			return s - HALF_PI;
		} else {
			return s + HALF_PI;
		}
	} else {
		a = y / x;
		s = a * a;
		return ( ( ( ( ( ( ( ( ( 0.0028662257f * s - 0.0161657367f ) * s + 0.0429096138f ) * s - 0.0752896400f )
			* s + 0.1065626393f ) * s - 0.1420889944f ) * s + 0.1999355085f ) * s - 0.3333314528f ) * s ) + 1.0f ) * a;
	}
}

double olaMath::ATan64( float y, float x ) {
	return atan2( y, x );
}

float olaMath::Pow( float x, float y ) {
	return powf( x, y );
}

float olaMath::Pow16( float x, float y ) {
	return Exp16( y * Log16( x ) );
}

double olaMath::Pow64( float x, float y ) {
	return pow( x, y );
}

float olaMath::Exp( float f ) {
	return expf( f );
}

float olaMath::Exp16( float f ) {
	int i, s, e, m, exponent;
	float x, x2, y, p, q;

	x = f * 1.44269504088896340f;		// multiply with ( 1 / log( 2 ) )
#if 1
	i = *reinterpret_cast<int *>(&x);
	s = ( i >> IEEE_FLT_SIGN_BIT );
	e = ( ( i >> IEEE_FLT_MANTISSA_BITS ) & ( ( 1 << IEEE_FLT_EXPONENT_BITS ) - 1 ) ) - IEEE_FLT_EXPONENT_BIAS;
	m = ( i & ( ( 1 << IEEE_FLT_MANTISSA_BITS ) - 1 ) ) | ( 1 << IEEE_FLT_MANTISSA_BITS );
	i = ( ( m >> ( IEEE_FLT_MANTISSA_BITS - e ) ) & ~( e >> 31 ) ) ^ s;
#else
	i = (int) x;
	if ( x < 0.0f ) {
		i--;
	}
#endif
	exponent = ( i + IEEE_FLT_EXPONENT_BIAS ) << IEEE_FLT_MANTISSA_BITS;
	y = *reinterpret_cast<float *>(&exponent);
	x -= (float) i;
	if ( x >= 0.5f ) {
		x -= 0.5f;
		y *= 1.4142135623730950488f;	// multiply with sqrt( 2 )
	}
	x2 = x * x;
	p = x * ( 7.2152891511493f + x2 * 0.0576900723731f );
	q = 20.8189237930062f + x2;
	x = y * ( q + p ) / ( q - p );
	return x;
}

double olaMath::Exp64( float f ) {
	return exp( f );
}

float olaMath::Log( float f ) {
	return logf( f );
}

float olaMath::Log16( float f ) {
	int i, exponent;
	float y, y2;

	i = *reinterpret_cast<int *>(&f);
	exponent = ( ( i >> IEEE_FLT_MANTISSA_BITS ) & ( ( 1 << IEEE_FLT_EXPONENT_BITS ) - 1 ) ) - IEEE_FLT_EXPONENT_BIAS;
	i -= ( exponent + 1 ) << IEEE_FLT_MANTISSA_BITS;	// get value in the range [.5, 1>
	y = *reinterpret_cast<float *>(&i);
	y *= 1.4142135623730950488f;						// multiply with sqrt( 2 )
	y = ( y - 1.0f ) / ( y + 1.0f );
	y2 = y * y;
	y = y * ( 2.000000000046727f + y2 * ( 0.666666635059382f + y2 * ( 0.4000059794795f + y2 * ( 0.28525381498f + y2 * 0.2376245609f ) ) ) );
	y += 0.693147180559945f * ( (float)exponent + 0.5f );
	return y;
}

double olaMath::Log64( float f ) {
	return log( f );
}

int olaMath::IPow( int x, int y ) {
	int r; for( r = x; y > 1; y-- ) { r *= x; } return r;
}

int olaMath::ILog2( float f ) {
	return ( ( (*reinterpret_cast<int *>(&f)) >> IEEE_FLT_MANTISSA_BITS ) & ( ( 1 << IEEE_FLT_EXPONENT_BITS ) - 1 ) ) - IEEE_FLT_EXPONENT_BIAS;
}

int olaMath::ILog2( int i ) {
	return ILog2( (float)i );
}

int olaMath::BitsForFloat( float f ) {
	return ILog2( f ) + 1;
}

int olaMath::BitsForInteger( int i ) {
	return ILog2( (float)i ) + 1;
}

int olaMath::MaskForFloatSign( float f ) {
	return ( (*reinterpret_cast<int *>(&f)) >> 31 );
}

int olaMath::MaskForIntegerSign( int i ) {
	return ( i >> 31 );
}

int olaMath::FloorPowerOfTwo( int x ) {
	return CeilPowerOfTwo( x ) >> 1;
}

int olaMath::CeilPowerOfTwo( int x ) {
	x--;
	x |= x >> 1;
	x |= x >> 2;
	x |= x >> 4;
	x |= x >> 8;
	x |= x >> 16;
	x++;
	return x;
}

bool olaMath::IsPowerOfTwo( int x ) {
	return ( x & ( x - 1 ) ) == 0 && x > 0;
}

int olaMath::BitCount( int x ) {
	x -= ( ( x >> 1 ) & 0x55555555 );
	x = ( ( ( x >> 2 ) & 0x33333333 ) + ( x & 0x33333333 ) );
	x = ( ( ( x >> 4 ) + x ) & 0x0f0f0f0f );
	x += ( x >> 8 );
	return ( ( x + ( x >> 16 ) ) & 0x0000003f );
}

int olaMath::BitReverse( int x ) {
	x = ( ( ( x >> 1 ) & 0x55555555 ) | ( ( x & 0x55555555 ) << 1 ) );
	x = ( ( ( x >> 2 ) & 0x33333333 ) | ( ( x & 0x33333333 ) << 2 ) );
	x = ( ( ( x >> 4 ) & 0x0f0f0f0f ) | ( ( x & 0x0f0f0f0f ) << 4 ) );
	x = ( ( ( x >> 8 ) & 0x00ff00ff ) | ( ( x & 0x00ff00ff ) << 8 ) );
	return ( ( x >> 16 ) | ( x << 16 ) );
}

int olaMath::Abs( int x ) {
   int y = x >> 31;
   return ( ( x ^ y ) - y );
}

float olaMath::Fabs( float f ) {
	int tmp = *reinterpret_cast<int *>( &f );
	tmp &= 0x7FFFFFFF;
	return *reinterpret_cast<float *>( &tmp );
}

float olaMath::Floor( float f ) {
	return floorf( f );
}

float olaMath::Ceil( float f ) {
	return ceilf( f );
}

float olaMath::Rint( float f ) {
	return floorf( f + 0.5f );
}

int olaMath::Ftoi( float f ) {
	return (int) f;
}

int olaMath::FtoiFast( float f ) {
#ifdef _WIN32
	int i;
	__asm fld		f
	__asm fistp		i		// use default rouding mode (round nearest)
	return i;
#elif 0						// round chop (C/C++ standard)
	int i, s, e, m, shift;
	i = *reinterpret_cast<int *>(&f);
	s = i >> IEEE_FLT_SIGN_BIT;
	e = ( ( i >> IEEE_FLT_MANTISSA_BITS ) & ( ( 1 << IEEE_FLT_EXPONENT_BITS ) - 1 ) ) - IEEE_FLT_EXPONENT_BIAS;
	m = ( i & ( ( 1 << IEEE_FLT_MANTISSA_BITS ) - 1 ) ) | ( 1 << IEEE_FLT_MANTISSA_BITS );
	shift = e - IEEE_FLT_MANTISSA_BITS;
	return ( ( ( ( m >> -shift ) | ( m << shift ) ) & ~( e >> 31 ) ) ^ s ) - s;
//#elif defined( __i386__ )
#elif 0
	int i = 0;
	__asm__ __volatile__ (
						  "fld %1\n" \
						  "fistp %0\n" \
						  : "=m" (i) \
						  : "m" (f) );
	return i;
#else
	return (int) f;
#endif
}

unsigned long olaMath::Ftol( float f ) {
	return (unsigned long) f;
}

unsigned long olaMath::FtolFast( float f ) {
#ifdef _WIN32
	// FIXME: this overflows on 31bits still .. same as FtoiFast
	unsigned long i;
	__asm fld		f
	__asm fistp		i		// use default rouding mode (round nearest)
	return i;
#elif 0						// round chop (C/C++ standard)
	int i, s, e, m, shift;
	i = *reinterpret_cast<int *>(&f);
	s = i >> IEEE_FLT_SIGN_BIT;
	e = ( ( i >> IEEE_FLT_MANTISSA_BITS ) & ( ( 1 << IEEE_FLT_EXPONENT_BITS ) - 1 ) ) - IEEE_FLT_EXPONENT_BIAS;
	m = ( i & ( ( 1 << IEEE_FLT_MANTISSA_BITS ) - 1 ) ) | ( 1 << IEEE_FLT_MANTISSA_BITS );
	shift = e - IEEE_FLT_MANTISSA_BITS;
	return ( ( ( ( m >> -shift ) | ( m << shift ) ) & ~( e >> 31 ) ) ^ s ) - s;
//#elif defined( __i386__ )
#elif 0
	// for some reason, on gcc I need to make sure i == 0 before performing a fistp
	int i = 0;
	__asm__ __volatile__ (
						  "fld %1\n" \
						  "fistp %0\n" \
						  : "=m" (i) \
						  : "m" (f) );
	return i;
#else
	return (unsigned long) f;
#endif
}

signed char olaMath::ClampChar( int i ) {
	if ( i < -128 ) {
		return -128;
	}
	if ( i > 127 ) {
		return 127;
	}
	return i;
}

signed short olaMath::ClampShort( int i ) {
	if ( i < -32768 ) {
		return -32768;
	}
	if ( i > 32767 ) {
		return 32767;
	}
	return i;
}

int olaMath::ClampInt( int min, int max, int value ) {
	if ( value < min ) {
		return min;
	}
	if ( value > max ) {
		return max;
	}
	return value;
}

float olaMath::ClampFloat( float min, float max, float value ) {
	if ( value < min ) {
		return min;
	}
	if ( value > max ) {
		return max;
	}
	return value;
}

float olaMath::AngleNormalize360( float angle ) {
	if ( ( angle >= 360.0f ) || ( angle < 0.0f ) ) {
		angle -= floor( angle / 360.0f ) * 360.0f;
	}
	return angle;
}

float olaMath::AngleNormalize180( float angle ) {
	angle = AngleNormalize360( angle );
	if ( angle > 180.0f ) {
		angle -= 360.0f;
	}
	return angle;
}

float olaMath::AngleDelta( float angle1, float angle2 ) {
	return AngleNormalize180( angle1 - angle2 );
}

int olaMath::FloatHash( const float *array, const int numFloats ) {
	int i, hash = 0;
	const int *ptr;

	ptr = reinterpret_cast<const int *>( array );
	for ( i = 0; i < numFloats; i++ ) {
		hash ^= ptr[i];
	}
	return hash;
}
