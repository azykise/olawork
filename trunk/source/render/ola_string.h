// Copyright (C) 2004 Id Software, Inc.
//

#pragma warning(disable:4996)

#ifndef __STR_H__
#define __STR_H__
#include <assert.h>
#include <stdio.h>
#include <string.h>

#ifdef __GNUC__
#define ola_attribute(x) __attribute__(x)
#else
#define ola_attribute(x)  
#endif

#define OLA_INLINE __forceinline

/*
===============================================================================

	Character string

===============================================================================
*/

// these library functions should not be used for cross platform compatibility
//#define strcmp			idStr::Cmp		// use_idStr_Cmp
//#define strncmp			use_idStr_Cmpn
//#define StrCmpN			use_idStr_Cmpn
//#define strcmpi			use_idStr_Icmp
//#define StrCmpI			use_idStr_Icmp
//#define stricmp			idStr::Icmp		// use_idStr_Icmp
//#define _stricmp		use_idStr_Icmp
//#define strcasecmp		use_idStr_Icmp
//#define strnicmp		use_idStr_Icmpn
//#define _strnicmp		use_idStr_Icmpn
//#define _memicmp		use_idStr_Icmpn
//#define StrCmpNI		use_idStr_Icmpn
//#define snprintf		use_idStr_snPrintf
//#define _snprintf		use_idStr_snPrintf
//#define vsnprintf		use_idStr_vsnPrintf
//#define _vsnprintf		use_idStr_vsnPrintf


#ifndef FILE_HASH_SIZE
#define FILE_HASH_SIZE		1024
#endif

// color escape character
const int C_COLOR_ESCAPE			= '^';
const int C_COLOR_DEFAULT			= '0';
const int C_COLOR_RED				= '1';
const int C_COLOR_GREEN				= '2';
const int C_COLOR_YELLOW			= '3';
const int C_COLOR_BLUE				= '4';
const int C_COLOR_CYAN				= '5';
const int C_COLOR_MAGENTA			= '6';
const int C_COLOR_WHITE				= '7';
const int C_COLOR_GRAY				= '8';
const int C_COLOR_BLACK				= '9';

// color escape string
#define S_COLOR_DEFAULT				"^0"
#define S_COLOR_RED					"^1"
#define S_COLOR_GREEN				"^2"
#define S_COLOR_YELLOW				"^3"
#define S_COLOR_BLUE				"^4"
#define S_COLOR_CYAN				"^5"
#define S_COLOR_MAGENTA				"^6"
#define S_COLOR_WHITE				"^7"
#define S_COLOR_GRAY				"^8"
#define S_COLOR_BLACK				"^9"

// make idStr a multiple of 16 bytes long
// don't make too large to keep memory requirements to a minimum
const int STR_ALLOC_BASE			= 20;
const int STR_ALLOC_GRAN			= 32;

typedef enum {
	MEASURE_SIZE = 0,
	MEASURE_BANDWIDTH
} Measure_t;

class olastring {

public:
						olastring( void );
						olastring( const olastring &text );
						olastring( const olastring &text, int start, int end );
						olastring( const char *text );
						olastring( const char *text, int start, int end );
						explicit olastring( const bool b );
						explicit olastring( const char c );
						explicit olastring( const int i );
						explicit olastring( const unsigned u );
						explicit olastring( const float f );
						~olastring( void );

	size_t				size( void ) const;
	const char *		c_str( void ) const;
	operator			const char *( void ) const;
	operator			const char *( void );

	char				operator[]( int index ) const;
	char &				operator[]( int index );

	void				operator=( const olastring &text );
	void				operator=( const char *text );

	friend olastring	operator+( const olastring &a, const olastring &b );
	friend olastring	operator+( const olastring &a, const char *b );
	friend olastring	operator+( const char *a, const olastring &b );

	friend olastring	operator+( const olastring &a, const float b );
	friend olastring	operator+( const olastring &a, const int b );
	friend olastring	operator+( const olastring &a, const unsigned b );
	friend olastring	operator+( const olastring &a, const bool b );
	friend olastring	operator+( const olastring &a, const char b );

	olastring &			operator+=( const olastring &a );
	olastring &			operator+=( const char *a );
	olastring &			operator+=( const float a );
	olastring &			operator+=( const char a );
	olastring &			operator+=( const int a );
	olastring &			operator+=( const unsigned a );
	olastring &			operator+=( const bool a );

						// case sensitive compare
	friend bool			operator==( const olastring &a, const olastring &b );
	friend bool			operator==( const olastring &a, const char *b );
	friend bool			operator==( const char *a, const olastring &b );

						// case sensitive compare
	friend bool			operator!=( const olastring &a, const olastring &b );
	friend bool			operator!=( const olastring &a, const char *b );
	friend bool			operator!=( const char *a, const olastring &b );

	friend bool			operator<( const olastring &a, const olastring &b );

						// case sensitive compare
	int					cmp( const char *text ) const;
	int					cmpn( const char *text, int n ) const;
	int					cmpPrefix( const char *text ) const;

						// case insensitive compare
	int					icmp( const char *text ) const;
	int					icmpn( const char *text, int n ) const;
	int					icmpPrefix( const char *text ) const;

						// case insensitive compare ignoring color
	int					icmpNoColor( const char *text ) const;

						// compares paths and makes sure folders come first
	int					icmpPath( const char *text ) const;
	int					icmpnPath( const char *text, int n ) const;
	int					icmpPrefixPath( const char *text ) const;

	int					length( void ) const;
	int					allocated( void ) const;
	void				empty( void );
	bool				isEmpty( void ) const;
	void				clear( void );
	void				append( const char a );
	void				append( const olastring &text );
	void				append( const char *text );
	void				append( const char *text, int len );
	void				insert( const char a, int index );
	void				insert( const char *text, int index );
	void				toLower( void );
	void				toUpper( void );
	bool				isNumeric( void ) const;
	bool				isColor( void ) const;
	bool				hasLower( void ) const;
	bool				hasUpper( void ) const;
	int					lengthWithoutColors( void ) const;
	olastring &			removeColors( void );
	void				capLength( int );
	void				fill( const char ch, int newlen );

	int					find( const char c, int start = 0, int end = -1 ) const;
	int					find( const char *text, bool casesensitive = true, int start = 0, int end = -1 ) const;
	bool				filter( const char *filter, bool casesensitive ) const;
	int					last( const char c ) const;						// return the index to the last occurance of 'c', returns -1 if not found
	const char *		left( int len, olastring &result ) const;			// store the leftmost 'len' characters in the result
	const char *		right( int len, olastring &result ) const;			// store the rightmost 'len' characters in the result
	const char *		mid( int start, int len, olastring &result ) const;	// store 'len' characters starting at 'start' in result
	olastring			left( int len ) const;							// return the leftmost 'len' characters
	olastring			right( int len ) const;							// return the rightmost 'len' characters
	olastring			mid( int start, int len ) const;				// return 'len' characters starting at 'start'
	void				stripLeading( const char c );					// strip char from front as many times as the char occurs
	void				stripLeading( const char *string );				// strip string from front as many times as the string occurs
	bool				stripLeadingOnce( const char *string );			// strip string from front just once if it occurs
	void				stripTrailing( const char c );					// strip char from end as many times as the char occurs
	void				stripTrailing( const char *string );			// strip string from end as many times as the string occurs
	bool				stripTrailingOnce( const char *string );		// strip string from end just once if it occurs
	void				strip( const char c );							// strip char from front and end as many times as the char occurs
	void				strip( const char *string );					// strip string from front and end as many times as the string occurs
	void				stripTrailingWhitespace( void );				// strip trailing white space characters
	olastring &			stripQuotes( void );							// strip quotes around string
	void				replace( const char *old, const char *nw );

	// file name methods
	int					fileNameHash( void ) const;						// hash key for the filename (skips extension)
	olastring &			backSlashesToSlashes( void );					// convert slashes
	olastring &			setFileExtension( const char *extension );		// set the given file extension
	olastring &			stripFileExtension( void );						// remove any file extension
	olastring &			stripAbsoluteFileExtension( void );				// remove any file extension looking from front (useful if there are multiple .'s)
	olastring &			defaultFileExtension( const char *extension );	// if there's no file extension use the default
	olastring &			defaultPath( const char *basepath );			// if there's no path use the default
	void				appendPath( const char *text );					// append a partial path
	olastring &			stripFilename( void );							// remove the filename from a path
	olastring &			stripPath( void );								// remove the path from the filename
	void				extractFilePath( olastring &dest ) const;			// copy the file path to another string
	void				extractFileName( olastring &dest ) const;			// copy the filename to another string
	void				extractFileBase( olastring &dest ) const;			// copy the filename minus the extension to another string
	void				extractFileExtension( olastring &dest ) const;		// copy the file extension to another string
	bool				checkExtension( const char *ext );

	// char * methods to replace library functions
	static int			length( const char *s );
	static char *		toLower( char *s );
	static char *		toUpper( char *s );
	static bool			isNumeric( const char *s );
	static bool			isColor( const char *s );
	static bool			hasLower( const char *s );
	static bool			hasUpper( const char *s );
	static int			lengthWithoutColors( const char *s );
	static char *		removeColors( char *s );
	static int			cmp( const char *s1, const char *s2 );
	static int			cmpn( const char *s1, const char *s2, int n );
	static int			icmp( const char *s1, const char *s2 );
	static int			icmpn( const char *s1, const char *s2, int n );
	static int			icmpNoColor( const char *s1, const char *s2 );
	static int			icmpPath( const char *s1, const char *s2 );			// compares paths and makes sure folders come first
	static int			icmpnPath( const char *s1, const char *s2, int n );	// compares paths and makes sure folders come first
	static void			append( char *dest, int size, const char *src );
	static void			copynz( char *dest, const char *src, int destsize );
	static int			snPrintf( char *dest, int size, const char *fmt, ... ) ola_attribute((format(printf,3,4)));
	static int			vsnPrintf( char *dest, int size, const char *fmt, va_list argptr );
	static int			findChar( const char *str, const char c, int start = 0, int end = -1 );
	static int			findText( const char *str, const char *text, bool casesensitive = true, int start = 0, int end = -1 );
	static bool			filter( const char *filter, const char *name, bool casesensitive );
	static void			stripMediaName( const char *name, olastring &mediaName );
	static bool			checkExtension( const char *name, const char *ext );
	static const char *	floatArrayToString( const float *array, const int length, const int precision );

	// hash keys
	static int			hash( const char *string );
	static int			hash( const char *string, int length );
	static int			iHash( const char *string );					// case insensitive
	static int			iHash( const char *string, int length );		// case insensitive

	// character methods
	static char			toLower( char c );
	static char			toUpper( char c );
	static bool			charIsPrintable( int c );
	static bool			charIsLower( int c );
	static bool			charIsUpper( int c );
	static bool			charIsAlpha( int c );
	static bool			charIsNumeric( int c );
	static bool			charIsNewLine( char c );
	static bool			charIsTab( char c );

	friend int			sprintf( olastring &dest, const char *fmt, ... );
	friend int			vsprintf( olastring &dest, const char *fmt, va_list ap );

	void				reAllocate( int amount, bool keepold );				// reallocate string data buffer
	void				freeData( void );									// free allocated string memory

						// format value in the given measurement with the best unit, returns the best unit
	int					bestUnit( const char *format, float value, Measure_t measure );
						// format value in the requested unit and measurement
	void				setUnit( const char *format, float value, int unit, Measure_t measure );

	static void			initMemory( void );
	static void			shutdownMemory( void );
	static void			purgeMemory( void );


	int					dynamicMemoryUsed() const;
	static olastring	formatNumber( int number );

protected:
	int					len;
	char *				data;
	int					alloced;
	char				baseBuffer[ STR_ALLOC_BASE ];

	void				Init( void );										// initialize string using base buffer
	void				EnsureAlloced( int amount, bool keepold = true );	// ensure string data buffer is large anough
};

char *					va( const char *fmt, ... ) ola_attribute((format(printf,1,2)));


OLA_INLINE void olastring::EnsureAlloced( int amount, bool keepold ) {
	if ( amount > alloced ) {
		reAllocate( amount, keepold );
	}
}

OLA_INLINE void olastring::Init( void ) {
	len = 0;
	alloced = STR_ALLOC_BASE;
	data = baseBuffer;
	data[ 0 ] = '\0';
#ifdef ID_DEBUG_UNINITIALIZED_MEMORY
	memset( baseBuffer, 0, sizeof( baseBuffer ) );
#endif
}

OLA_INLINE olastring::olastring( void ) {
	Init();
}

OLA_INLINE olastring::olastring( const olastring &text ) {
	int l;

	Init();
	l = text.length();
	EnsureAlloced( l + 1 );
	strcpy( data, text.data );
	len = l;
}

OLA_INLINE olastring::olastring( const olastring &text, int start, int end ) {
	int i;
	int l;

	Init();
	if ( end > text.length() ) {
		end = text.length();
	}
	if ( start > text.length() ) {
		start = text.length();
	} else if ( start < 0 ) {
		start = 0;
	}

	l = end - start;
	if ( l < 0 ) {
		l = 0;
	}

	EnsureAlloced( l + 1 );

	for ( i = 0; i < l; i++ ) {
		data[ i ] = text[ start + i ];
	}

	data[ l ] = '\0';
	len = l;
}

OLA_INLINE olastring::olastring( const char *text ) {
	int l;

	Init();
	if ( text ) {
		l = strlen( text );
		EnsureAlloced( l + 1 );
		strcpy( data, text );
		len = l;
	}
}

OLA_INLINE olastring::olastring( const char *text, int start, int end ) {
	int i;
	int l = strlen( text );

	Init();
	if ( end > l ) {
		end = l;
	}
	if ( start > l ) {
		start = l;
	} else if ( start < 0 ) {
		start = 0;
	}

	l = end - start;
	if ( l < 0 ) {
		l = 0;
	}

	EnsureAlloced( l + 1 );

	for ( i = 0; i < l; i++ ) {
		data[ i ] = text[ start + i ];
	}

	data[ l ] = '\0';
	len = l;
}

OLA_INLINE olastring::olastring( const bool b ) {
	Init();
	EnsureAlloced( 2 );
	data[ 0 ] = b ? '1' : '0';
	data[ 1 ] = '\0';
	len = 1;
}

OLA_INLINE olastring::olastring( const char c ) {
	Init();
	EnsureAlloced( 2 );
	data[ 0 ] = c;
	data[ 1 ] = '\0';
	len = 1;
}

OLA_INLINE olastring::olastring( const int i ) {
	char text[ 64 ];
	int l;

	Init();
	l = sprintf( text, "%d", i );
	EnsureAlloced( l + 1 );
	strcpy( data, text );
	len = l;
}

OLA_INLINE olastring::olastring( const unsigned u ) {
	char text[ 64 ];
	int l;

	Init();
	l = sprintf( text, "%u", u );
	EnsureAlloced( l + 1 );
	strcpy( data, text );
	len = l;
}

OLA_INLINE olastring::olastring( const float f ) {
	char text[ 64 ];
	int l;

	Init();
	l = olastring::snPrintf( text, sizeof( text ), "%f", f );
	while( l > 0 && text[l-1] == '0' ) text[--l] = '\0';
	while( l > 0 && text[l-1] == '.' ) text[--l] = '\0';
	EnsureAlloced( l + 1 );
	strcpy( data, text );
	len = l;
}

OLA_INLINE olastring::~olastring( void ) {
	freeData();
}

OLA_INLINE size_t olastring::size( void ) const {
	return sizeof( *this ) + allocated();
}

OLA_INLINE const char *olastring::c_str( void ) const {
	return data;
}

OLA_INLINE olastring::operator const char *( void ) {
	return c_str();
}

OLA_INLINE olastring::operator const char *( void ) const {
	return c_str();
}

OLA_INLINE char olastring::operator[]( int index ) const {
	assert( ( index >= 0 ) && ( index <= len ) );
	return data[ index ];
}

OLA_INLINE char &olastring::operator[]( int index ) {
	assert( ( index >= 0 ) && ( index <= len ) );
	return data[ index ];
}

OLA_INLINE void olastring::operator=( const olastring &text ) {
	int l;

	l = text.length();
	EnsureAlloced( l + 1, false );
	memcpy( data, text.data, l );
	data[l] = '\0';
	len = l;
}

OLA_INLINE olastring operator+( const olastring &a, const olastring &b ) {
	olastring result( a );
	result.append( b );
	return result;
}

OLA_INLINE olastring operator+( const olastring &a, const char *b ) {
	olastring result( a );
	result.append( b );
	return result;
}

OLA_INLINE olastring operator+( const char *a, const olastring &b ) {
	olastring result( a );
	result.append( b );
	return result;
}

OLA_INLINE olastring operator+( const olastring &a, const bool b ) {
	olastring result( a );
	result.append( b ? "true" : "false" );
	return result;
}

OLA_INLINE olastring operator+( const olastring &a, const char b ) {
	olastring result( a );
	result.append( b );
	return result;
}

OLA_INLINE olastring operator+( const olastring &a, const float b ) {
	char	text[ 64 ];
	olastring	result( a );

	sprintf( text, "%f", b );
	result.append( text );

	return result;
}

OLA_INLINE olastring operator+( const olastring &a, const int b ) {
	char	text[ 64 ];
	olastring	result( a );

	sprintf( text, "%d", b );
	result.append( text );

	return result;
}

OLA_INLINE olastring operator+( const olastring &a, const unsigned b ) {
	char	text[ 64 ];
	olastring	result( a );

	sprintf( text, "%u", b );
	result.append( text );

	return result;
}

OLA_INLINE olastring &olastring::operator+=( const float a ) {
	char text[ 64 ];

	sprintf( text, "%f", a );
	append( text );

	return *this;
}

OLA_INLINE olastring &olastring::operator+=( const int a ) {
	char text[ 64 ];

	sprintf( text, "%d", a );
	append( text );

	return *this;
}

OLA_INLINE olastring &olastring::operator+=( const unsigned a ) {
	char text[ 64 ];

	sprintf( text, "%u", a );
	append( text );

	return *this;
}

OLA_INLINE olastring &olastring::operator+=( const olastring &a ) {
	append( a );
	return *this;
}

OLA_INLINE olastring &olastring::operator+=( const char *a ) {
	append( a );
	return *this;
}

OLA_INLINE olastring &olastring::operator+=( const char a ) {
	append( a );
	return *this;
}

OLA_INLINE olastring &olastring::operator+=( const bool a ) {
	append( a ? "true" : "false" );
	return *this;
}

OLA_INLINE bool operator==( const olastring &a, const olastring &b ) {
	return ( !olastring::cmp( a.data, b.data ) );
}

OLA_INLINE bool operator<( const olastring &a, const olastring &b ) {
	int h1 = olastring::hash(a.c_str());
	int h2 = olastring::hash(b.c_str());
	return ( h1 < h2 );
}

OLA_INLINE bool operator==( const olastring &a, const char *b ) {
	assert( b );
	return ( !olastring::cmp( a.data, b ) );
}

OLA_INLINE bool operator==( const char *a, const olastring &b ) {
	assert( a );
	return ( !olastring::cmp( a, b.data ) );
}

OLA_INLINE bool operator!=( const olastring &a, const olastring &b ) {
	return !( a == b );
}

OLA_INLINE bool operator!=( const olastring &a, const char *b ) {
	return !( a == b );
}

OLA_INLINE bool operator!=( const char *a, const olastring &b ) {
	return !( a == b );
}

OLA_INLINE int olastring::cmp( const char *text ) const {
	assert( text );
	return olastring::cmp( data, text );
}

OLA_INLINE int olastring::cmpn( const char *text, int n ) const {
	assert( text );
	return olastring::cmpn( data, text, n );
}

OLA_INLINE int olastring::cmpPrefix( const char *text ) const {
	assert( text );
	return olastring::cmpn( data, text, strlen( text ) );
}

OLA_INLINE int olastring::icmp( const char *text ) const {
	assert( text );
	return olastring::icmp( data, text );
}

OLA_INLINE int olastring::icmpn( const char *text, int n ) const {
	assert( text );
	return olastring::icmpn( data, text, n );
}

OLA_INLINE int olastring::icmpPrefix( const char *text ) const {
	assert( text );
	return olastring::icmpn( data, text, strlen( text ) );
}

OLA_INLINE int olastring::icmpNoColor( const char *text ) const {
	assert( text );
	return olastring::icmpNoColor( data, text );
}

OLA_INLINE int olastring::icmpPath( const char *text ) const {
	assert( text );
	return olastring::icmpPath( data, text );
}

OLA_INLINE int olastring::icmpnPath( const char *text, int n ) const {
	assert( text );
	return olastring::icmpnPath( data, text, n );
}

OLA_INLINE int olastring::icmpPrefixPath( const char *text ) const {
	assert( text );
	return olastring::icmpnPath( data, text, strlen( text ) );
}

OLA_INLINE int olastring::length( void ) const {
	return len;
}

OLA_INLINE int olastring::allocated( void ) const {
	if ( data != baseBuffer ) {
		return alloced;
	} else {
		return 0;
	}
}

OLA_INLINE void olastring::empty( void ) {
	EnsureAlloced( 1 );
	data[ 0 ] = '\0';
	len = 0;
}

OLA_INLINE bool olastring::isEmpty( void ) const {
	return ( olastring::cmp( data, "" ) == 0 );
}

OLA_INLINE void olastring::clear( void ) {
	freeData();
	Init();
}

OLA_INLINE void olastring::append( const char a ) {
	EnsureAlloced( len + 2 );
	data[ len ] = a;
	len++;
	data[ len ] = '\0';
}

OLA_INLINE void olastring::append( const olastring &text ) {
	int newLen;
	int i;

	newLen = len + text.length();
	EnsureAlloced( newLen + 1 );
	for ( i = 0; i < text.len; i++ ) {
		data[ len + i ] = text[ i ];
	}
	len = newLen;
	data[ len ] = '\0';
}

OLA_INLINE void olastring::append( const char *text ) {
	int newLen;
	int i;

	if ( text ) {
		newLen = len + strlen( text );
		EnsureAlloced( newLen + 1 );
		for ( i = 0; text[ i ]; i++ ) {
			data[ len + i ] = text[ i ];
		}
		len = newLen;
		data[ len ] = '\0';
	}
}

OLA_INLINE void olastring::append( const char *text, int l ) {
	int newLen;
	int i;

	if ( text && l ) {
		newLen = len + l;
		EnsureAlloced( newLen + 1 );
		for ( i = 0; text[ i ] && i < l; i++ ) {
			data[ len + i ] = text[ i ];
		}
		len = newLen;
		data[ len ] = '\0';
	}
}

OLA_INLINE void olastring::insert( const char a, int index ) {
	int i, l;

	if ( index < 0 ) {
		index = 0;
	} else if ( index > len ) {
		index = len;
	}

	l = 1;
	EnsureAlloced( len + l + 1 );
	for ( i = len; i >= index; i-- ) {
		data[i+l] = data[i];
	}
	data[index] = a;
	len++;
}

OLA_INLINE void olastring::insert( const char *text, int index ) {
	int i, l;

	if ( index < 0 ) {
		index = 0;
	} else if ( index > len ) {
		index = len;
	}

	l = strlen( text );
	EnsureAlloced( len + l + 1 );
	for ( i = len; i >= index; i-- ) {
		data[i+l] = data[i];
	}
	for ( i = 0; i < l; i++ ) {
		data[index+i] = text[i];
	}
	len += l;
}

OLA_INLINE void olastring::toLower( void ) {
	for (int i = 0; data[i]; i++ ) {
		if ( charIsUpper( data[i] ) ) {
			data[i] += ( 'a' - 'A' );
		}
	}
}

OLA_INLINE void olastring::toUpper( void ) {
	for (int i = 0; data[i]; i++ ) {
		if ( charIsLower( data[i] ) ) {
			data[i] -= ( 'a' - 'A' );
		}
	}
}

OLA_INLINE bool olastring::isNumeric( void ) const {
	return olastring::isNumeric( data );
}

OLA_INLINE bool olastring::isColor( void ) const {
	return olastring::isColor( data );
}

OLA_INLINE bool olastring::hasLower( void ) const {
	return olastring::hasLower( data );
}

OLA_INLINE bool olastring::hasUpper( void ) const {
	return olastring::hasUpper( data );
}

OLA_INLINE olastring &olastring::removeColors( void ) {
	olastring::removeColors( data );
	len = length( data );
	return *this;
}

OLA_INLINE int olastring::lengthWithoutColors( void ) const {
	return olastring::lengthWithoutColors( data );
}

OLA_INLINE void olastring::capLength( int newlen ) {
	if ( len <= newlen ) {
		return;
	}
	data[ newlen ] = 0;
	len = newlen;
}

OLA_INLINE void olastring::fill( const char ch, int newlen ) {
	EnsureAlloced( newlen + 1 );
	len = newlen;
	memset( data, ch, len );
	data[ len ] = 0;
}

OLA_INLINE int olastring::find( const char c, int start, int end ) const {
	if ( end == -1 ) {
		end = len;
	}
	return olastring::findChar( data, c, start, end );
}

OLA_INLINE int olastring::find( const char *text, bool casesensitive, int start, int end ) const {
	if ( end == -1 ) {
		end = len;
	}
	return olastring::findText( data, text, casesensitive, start, end );
}

OLA_INLINE bool olastring::filter( const char *filter, bool casesensitive ) const {
	return olastring::filter( filter, data, casesensitive );
}

OLA_INLINE const char *olastring::left( int len, olastring &result ) const {
	return mid( 0, len, result );
}

OLA_INLINE const char *olastring::right( int len, olastring &result ) const {
	if ( len >= length() ) {
		result = *this;
		return result;
	}
	return mid( length() - len, len, result );
}

OLA_INLINE olastring olastring::left( int len ) const {
	return mid( 0, len );
}

OLA_INLINE olastring olastring::right( int len ) const {
	if ( len >= length() ) {
		return *this;
	}
	return mid( length() - len, len );
}

OLA_INLINE void olastring::strip( const char c ) {
	stripLeading( c );
	stripTrailing( c );
}

OLA_INLINE void olastring::strip( const char *string ) {
	stripLeading( string );
	stripTrailing( string );
}

OLA_INLINE bool olastring::checkExtension( const char *ext ) {
	return olastring::checkExtension( data, ext );
}

OLA_INLINE int olastring::length( const char *s ) {
	int i;
	for ( i = 0; s[i]; i++ ) {}
	return i;
}

OLA_INLINE char *olastring::toLower( char *s ) {
	for ( int i = 0; s[i]; i++ ) {
		if ( charIsUpper( s[i] ) ) {
			s[i] += ( 'a' - 'A' );
		}
	}
	return s;
}

OLA_INLINE char *olastring::toUpper( char *s ) {
	for ( int i = 0; s[i]; i++ ) {
		if ( charIsLower( s[i] ) ) {
			s[i] -= ( 'a' - 'A' );
		}
	}
	return s;
}

OLA_INLINE int olastring::hash( const char *string ) {
	int i, hash = 0;
	for ( i = 0; *string != '\0'; i++ ) {
		hash += ( *string++ ) * ( i + 119 );
	}
	return hash;
}

OLA_INLINE int olastring::hash( const char *string, int length ) {
	int i, hash = 0;
	for ( i = 0; i < length; i++ ) {
		hash += ( *string++ ) * ( i + 119 );
	}
	return hash;
}

OLA_INLINE int olastring::iHash( const char *string ) {
	int i, hash = 0;
	for( i = 0; *string != '\0'; i++ ) {
		hash += toLower( *string++ ) * ( i + 119 );
	}
	return hash;
}

OLA_INLINE int olastring::iHash( const char *string, int length ) {
	int i, hash = 0;
	for ( i = 0; i < length; i++ ) {
		hash += toLower( *string++ ) * ( i + 119 );
	}
	return hash;
}

OLA_INLINE bool olastring::isColor( const char *s ) {
	return ( s[0] == C_COLOR_ESCAPE && s[1] != '\0' && s[1] != ' ' );
}

OLA_INLINE char olastring::toLower( char c ) {
	if ( c <= 'Z' && c >= 'A' ) {
		return ( c + ( 'a' - 'A' ) );
	}
	return c;
}

OLA_INLINE char olastring::toUpper( char c ) {
	if ( c >= 'a' && c <= 'z' ) {
		return ( c - ( 'a' - 'A' ) );
	}
	return c;
}

OLA_INLINE bool olastring::charIsPrintable( int c ) {
	// test for regular ascii and western European high-ascii chars
	return ( c >= 0x20 && c <= 0x7E ) || ( c >= 0xA1 && c <= 0xFF );
}

OLA_INLINE bool olastring::charIsLower( int c ) {
	// test for regular ascii and western European high-ascii chars
	return ( c >= 'a' && c <= 'z' ) || ( c >= 0xE0 && c <= 0xFF );
}

OLA_INLINE bool olastring::charIsUpper( int c ) {
	// test for regular ascii and western European high-ascii chars
	return ( c <= 'Z' && c >= 'A' ) || ( c >= 0xC0 && c <= 0xDF );
}

OLA_INLINE bool olastring::charIsAlpha( int c ) {
	// test for regular ascii and western European high-ascii chars
	return ( ( c >= 'a' && c <= 'z' ) || ( c >= 'A' && c <= 'Z' ) ||
			 ( c >= 0xC0 && c <= 0xFF ) );
}

OLA_INLINE bool olastring::charIsNumeric( int c ) {
	return ( c <= '9' && c >= '0' );
}

OLA_INLINE bool olastring::charIsNewLine( char c ) {
	return ( c == '\n' || c == '\r' || c == '\v' );
}

OLA_INLINE bool olastring::charIsTab( char c ) {
	return ( c == '\t' );
}


OLA_INLINE int olastring::dynamicMemoryUsed() const {
	return ( data == baseBuffer ) ? 0 : alloced;
}

#endif /* !__STR_H__ */
