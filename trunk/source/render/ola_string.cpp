// Copyright (C) 2004 Id Software, Inc.
//

//#include "precompiled.h"
#pragma hdrstop

#define INTSIGNBITNOTSET(i)		((~((const unsigned long)(i))) >> 31)

#include "ola_string.h"

#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>

//#ifndef ID_REDIRECT_NEWDELETE
//	#define USE_STRING_DATA_ALLOCATOR
//#endif

#ifdef USE_STRING_DATA_ALLOCATOR
static idDynamicBlockAlloc<char, 1<<18, 128>	stringDataAllocator;
#endif

const char *units[2][4] =
{
	{ "B", "KB", "MB", "GB" },
	{ "B/s", "KB/s", "MB/s", "GB/s" }
};

/*
============
olastring::ReAllocate
============
*/
void olastring::reAllocate( int amount, bool keepold ) {
	char	*newbuffer;
	int		newsize;
	int		mod;

	//assert( data );
	assert( amount > 0 );

	mod = amount % STR_ALLOC_GRAN;
	if ( !mod ) {
		newsize = amount;
	}
	else {
		newsize = amount + STR_ALLOC_GRAN - mod;
	}
	alloced = newsize;

#ifdef USE_STRING_DATA_ALLOCATOR
	newbuffer = stringDataAllocator.Alloc( alloced );
#else
	newbuffer = new char[ alloced ];
#endif
	if ( keepold && data ) {
		data[ len ] = '\0';
		strcpy( newbuffer, data );
	}

	if ( data && data != baseBuffer ) {
#ifdef USE_STRING_DATA_ALLOCATOR
		stringDataAllocator.Free( data );
#else
		delete [] data;
#endif
	}

	data = newbuffer;
}

/*
============
olastring::FreeData
============
*/
void olastring::freeData( void ) {
	if ( data && data != baseBuffer ) {
#ifdef USE_STRING_DATA_ALLOCATOR
		stringDataAllocator.Free( data );
#else
		delete[] data;
#endif
		data = baseBuffer;
	}
}

/*
============
olastring::operator=
============
*/
void olastring::operator=( const char *text ) {
	int l;
	int diff;
	int i;

	if ( !text ) {
		// safe behaviour if 0
		EnsureAlloced( 1, false );
		data[ 0 ] = '\0';
		len = 0;
		return;
	}

	if ( text == data ) {
		return; // copying same thing
	}

	// check if we're aliasing
	if ( text >= data && text <= data + len ) {
		diff = text - data;

		assert( strlen( text ) < (unsigned)len );

		for ( i = 0; text[ i ]; i++ ) {
			data[ i ] = text[ i ];
		}

		data[ i ] = '\0';

		len -= diff;

		return;
	}

	l = strlen( text );
	EnsureAlloced( l + 1, false );
	strcpy( data, text );
	len = l;
}

/*
============
olastring::FindChar

returns -1 if not found otherwise the index of the char
============
*/
int olastring::findChar( const char *str, const char c, int start, int end ) {
	int i;

	if ( end == -1 ) {
		end = strlen( str ) - 1;
	}
	for ( i = start; i <= end; i++ ) {
		if ( str[i] == c ) {
			return i;
		}
	}
	return -1;
}

/*
============
olastring::FindText

returns -1 if not found otherwise the index of the text
============
*/
int olastring::findText( const char *str, const char *text, bool casesensitive, int start, int end ) {
	int l, i, j;

	if ( end == -1 ) {
		end = strlen( str );
	}
	l = end - strlen( text );
	for ( i = start; i <= l; i++ ) {
		if ( casesensitive ) {
			for ( j = 0; text[j]; j++ ) {
				if ( str[i+j] != text[j] ) {
					break;
				}
			}
		} else {
			for ( j = 0; text[j]; j++ ) {
				if ( ::toupper( str[i+j] ) != ::toupper( text[j] ) ) {
					break;
				}
			}
		}
		if ( !text[j] ) {
			return i;
		}
	}
	return -1;
}

/*
============
olastring::Filter

Returns true if the string conforms the given filter.
Several metacharacter may be used in the filter.

*          match any string of zero or more characters
?          match any single character
[abc...]   match any of the enclosed characters; a hyphen can
           be used to specify a range (e.g. a-z, A-Z, 0-9)

============
*/
bool olastring::filter( const char *filter, const char *name, bool casesensitive ) {
	olastring buf;
	int i, found, index;

	while(*filter) {
		if (*filter == '*') {
			filter++;
			buf.empty();
			for (i = 0; *filter; i++) {
				if ( *filter == '*' || *filter == '?' || (*filter == '[' && *(filter+1) != '[') ) {
					break;
				}
				buf += *filter;
				if ( *filter == '[' ) {
					filter++;
				}
				filter++;
			}
			if ( buf.length() ) {
				index = olastring(name).find( buf.c_str(), casesensitive );
				if ( index == -1 ) {
					return false;
				}
				name += index + strlen(buf);
			}
		}
		else if (*filter == '?') {
			filter++;
			name++;
		}
		else if (*filter == '[') {
			if ( *(filter+1) == '[' ) {
				if ( *name != '[' ) {
					return false;
				}
				filter += 2;
				name++;
			}
			else {
				filter++;
				found = false;
				while(*filter && !found) {
					if (*filter == ']' && *(filter+1) != ']') {
						break;
					}
					if (*(filter+1) == '-' && *(filter+2) && (*(filter+2) != ']' || *(filter+3) == ']')) {
						if (casesensitive) {
							if (*name >= *filter && *name <= *(filter+2)) {
								found = true;
							}
						}
						else {
							if ( ::toupper(*name) >= ::toupper(*filter) && ::toupper(*name) <= ::toupper(*(filter+2)) ) {
								found = true;
							}
						}
						filter += 3;
					}
					else {
						if (casesensitive) {
							if (*filter == *name) {
								found = true;
							}
						}
						else {
							if ( ::toupper(*filter) == ::toupper(*name) ) {
								found = true;
							}
						}
						filter++;
					}
				}
				if (!found) {
					return false;
				}
				while(*filter) {
					if ( *filter == ']' && *(filter+1) != ']' ) {
						break;
					}
					filter++;
				}
				filter++;
				name++;
			}
		}
		else {
			if (casesensitive) {
				if (*filter != *name) {
					return false;
				}
			}
			else {
				if ( ::toupper(*filter) != ::toupper(*name) ) {
					return false;
				}
			}
			filter++;
			name++;
		}
	}
	return true;
}

/*
=============
olastring::StripMediaName

  makes the string lower case, replaces backslashes with forward slashes, and removes extension
=============
*/
void olastring::stripMediaName( const char *name, olastring &mediaName ) {
	char c;

	mediaName.empty();

	for ( c = *name; c; c = *(++name) ) {
		// truncate at an extension
		if ( c == '.' ) {
			break;
		}
		// convert backslashes to forward slashes
		if ( c == '\\' ) {
			mediaName.append( '/' );
		} else {
			mediaName.append( olastring::toLower( c ) );
		}
	}
}

/*
=============
olastring::CheckExtension
=============
*/
bool olastring::checkExtension( const char *name, const char *ext ) {
	const char *s1 = name + length( name ) - 1;
	const char *s2 = ext + length( ext ) - 1;
	int c1, c2, d;

	do {
		c1 = *s1--;
		c2 = *s2--;

		d = c1 - c2;
		while( d ) {
			if ( c1 <= 'Z' && c1 >= 'A' ) {
				d += ('a' - 'A');
				if ( !d ) {
					break;
				}
			}
			if ( c2 <= 'Z' && c2 >= 'A' ) {
				d -= ('a' - 'A');
				if ( !d ) {
					break;
				}
			}
			return false;
		}
	} while( s1 > name && s2 > ext );

	return ( s1 >= name );
}

/*
=============
olastring::FloatArrayToString
=============
*/
const char *olastring::floatArrayToString( const float *array, const int length, const int precision ) {
	static int index = 0;
	static char str[4][16384];	// in case called by nested functions
	int i, n;
	char format[16], *s;

	// use an array of string so that multiple calls won't collide
	s = str[ index ];
	index = (index + 1) & 3;

	olastring::snPrintf( format, sizeof( format ), "%%.%df", precision );
	n = olastring::snPrintf( s, sizeof( str[0] ), format, array[0] );
	if ( precision > 0 ) {
		while( n > 0 && s[n-1] == '0' ) s[--n] = '\0';
		while( n > 0 && s[n-1] == '.' ) s[--n] = '\0';
	}
	olastring::snPrintf( format, sizeof( format ), " %%.%df", precision );
	for ( i = 1; i < length; i++ ) {
		n += olastring::snPrintf( s + n, sizeof( str[0] ) - n, format, array[i] );
		if ( precision > 0 ) {
			while( n > 0 && s[n-1] == '0' ) s[--n] = '\0';
			while( n > 0 && s[n-1] == '.' ) s[--n] = '\0';
		}
	}
	return s;
}

/*
============
olastring::Last

returns -1 if not found otherwise the index of the char
============
*/
int olastring::last( const char c ) const {
	int i;
	
	for( i = length(); i > 0; i-- ) {
		if ( data[ i - 1 ] == c ) {
			return i - 1;
		}
	}

	return -1;
}

/*
============
olastring::StripLeading
============
*/
void olastring::stripLeading( const char c ) {
	while( data[ 0 ] == c ) {
		memmove( &data[ 0 ], &data[ 1 ], len );
		len--;
	}
}

/*
============
olastring::StripLeading
============
*/
void olastring::stripLeading( const char *string ) {
	int l;

	l = strlen( string );
	if ( l > 0 ) {
		while ( !cmpn( string, l ) ) {
			memmove( data, data + l, len - l + 1 );
			len -= l;
		}
	}
}

/*
============
olastring::StripLeadingOnce
============
*/
bool olastring::stripLeadingOnce( const char *string ) {
	int l;

	l = strlen( string );
	if ( ( l > 0 ) && !cmpn( string, l ) ) {
		memmove( data, data + l, len - l + 1 );
		len -= l;
		return true;
	}
	return false;
}

/*
============
olastring::StripTrailing
============
*/
void olastring::stripTrailing( const char c ) {
	int i;
	
	for( i = length(); i > 0 && data[ i - 1 ] == c; i-- ) {
		data[ i - 1 ] = '\0';
		len--;
	}
}

/*
============
olastring::StripLeading
============
*/
void olastring::stripTrailing( const char *string ) {
	int l;

	l = strlen( string );
	if ( l > 0 ) {
		while ( ( len >= l ) && !cmpn( string, data + len - l, l ) ) {
			len -= l;
			data[len] = '\0';
		}
	}
}

/*
============
olastring::StripTrailingOnce
============
*/
bool olastring::stripTrailingOnce( const char *string ) {
	int l;

	l = strlen( string );
	if ( ( l > 0 ) && ( len >= l ) && !cmpn( string, data + len - l, l ) ) {
		len -= l;
		data[len] = '\0';
		return true;
	}
	return false;
}

/*
============
olastring::Replace
============
*/
void olastring::replace( const char *old, const char *nw ) {
	int		oldLen, newLen, i, j, count;
	olastring	oldString( data );

	oldLen = strlen( old );
	newLen = strlen( nw );

	// Work out how big the new string will be
	count = 0;
	for( i = 0; i < oldString.length(); i++ ) {
		if( !olastring::cmpn( &oldString[i], old, oldLen ) ) {
			count++;
			i += oldLen - 1;
		}
	}

	if( count ) {
		EnsureAlloced( len + ( ( newLen - oldLen ) * count ) + 2, false );

		// Replace the old data with the new data
		for( i = 0, j = 0; i < oldString.length(); i++ ) {
			if( !olastring::cmpn( &oldString[i], old, oldLen ) ) {
				memcpy( data + j, nw, newLen );
				i += oldLen - 1;
				j += newLen;
			} else {
				data[j] = oldString[i];
				j++;
			}
		}
		data[j] = 0;
		len = strlen( data );
	}
}

/*
============
olastring::Mid
============
*/
const char *olastring::mid( int start, int len, olastring &result ) const {
	int i;

	result.empty();

	i = length();
	if ( i == 0 || len <= 0 || start >= i ) {
		return 0;
	}

	if ( start + len >= i ) {
		len = i - start;
	}

	result.append( &data[ start ], len );
	return result;
}

/*
============
olastring::Mid
============
*/
olastring olastring::mid( int start, int len ) const {
	int i;
	olastring result;

	i = length();
	if ( i == 0 || len <= 0 || start >= i ) {
		return result;
	}

	if ( start + len >= i ) {
		len = i - start;
	}

	result.append( &data[ start ], len );
	return result;
}

/*
============
olastring::StripTrailingWhitespace
============
*/
void olastring::stripTrailingWhitespace( void ) {
	int i;
	
	// cast to unsigned char to prevent stripping off high-ASCII characters
	for( i = length(); i > 0 && (unsigned char)(data[ i - 1 ]) <= ' '; i-- ) {
		data[ i - 1 ] = '\0';
		len--;
	}
}

/*
============
olastring::StripQuotes

Removes the quotes from the beginning and end of the string
============
*/
olastring& olastring::stripQuotes ( void )
{
	if ( data[0] != '\"' )
	{
		return *this;
	}
	
	// Remove the trailing quote first
	if ( data[len-1] == '\"' )
	{
		data[len-1] = '\0';
		len--;
	}

	// Strip the leading quote now
	len--;	
	memmove( &data[ 0 ], &data[ 1 ], len );
	data[len] = '\0';
	
	return *this;
}

/*
=====================================================================

  filename methods

=====================================================================
*/

/*
============
olastring::FileNameHash
============
*/
int olastring::fileNameHash( void ) const {
	int		i;
	long	hash;
	char	letter;

	hash = 0;
	i = 0;
	while( data[i] != '\0' ) {
		letter = olastring::toLower( data[i] );
		if ( letter == '.' ) {
			break;				// don't include extension
		}
		if ( letter =='\\' ) {
			letter = '/';
		}
		hash += (long)(letter)*(i+119);
		i++;
	}
	hash &= (FILE_HASH_SIZE-1);
	return hash;
}

/*
============
olastring::BackSlashesToSlashes
============
*/
olastring &olastring::backSlashesToSlashes( void ) {
	int i;

	for ( i = 0; i < len; i++ ) {
		if ( data[ i ] == '\\' ) {
			data[ i ] = '/';
		}
	}
	return *this;
}

/*
============
olastring::SetFileExtension
============
*/
olastring &olastring::setFileExtension( const char *extension ) {
	stripFileExtension();
	if ( *extension != '.' ) {
		append( '.' );
	}
	append( extension );
	return *this;
}

/*
============
olastring::StripFileExtension
============
*/
olastring &olastring::stripFileExtension( void ) {
	int i;

	for ( i = len-1; i >= 0; i-- ) {
		if ( data[i] == '.' ) {
			data[i] = '\0';
			len = i;
			break;
		}
	}
	return *this;
}

/*
============
olastring::StripAbsoluteFileExtension
============
*/
olastring &olastring::stripAbsoluteFileExtension( void ) {
	int i;

	for ( i = 0; i < len; i++ ) {
		if ( data[i] == '.' ) {
			data[i] = '\0';
			len = i;
			break;
		}
	}

	return *this;
}

/*
==================
olastring::DefaultFileExtension
==================
*/
olastring &olastring::defaultFileExtension( const char *extension ) {
	int i;

	// do nothing if the string already has an extension
	for ( i = len-1; i >= 0; i-- ) {
		if ( data[i] == '.' ) {
			return *this;
		}
	}
	if ( *extension != '.' ) {
		append( '.' );
	}
	append( extension );
	return *this;
}

/*
==================
olastring::DefaultPath
==================
*/
olastring &olastring::defaultPath( const char *basepath ) {
	if ( ( ( *this )[ 0 ] == '/' ) || ( ( *this )[ 0 ] == '\\' ) ) {
		// absolute path location
		return *this;
	}

	*this = basepath + *this;
	return *this;
}

/*
====================
olastring::AppendPath
====================
*/
void olastring::appendPath( const char *text ) {
	int pos;
	int i = 0;

	if ( text && text[i] ) {
		pos = len;
		EnsureAlloced( len + strlen( text ) + 2 );

		if ( pos ) {
			if ( data[ pos-1 ] != '/' ) {
				data[ pos++ ] = '/';
			}
		}
		if ( text[i] == '/' ) {
			i++;
		}

		for ( ; text[ i ]; i++ ) {
			if ( text[ i ] == '\\' ) {
				data[ pos++ ] = '/';
			} else {
				data[ pos++ ] = text[ i ];
			}
		}
		len = pos;
		data[ pos ] = '\0';
	}
}

/*
==================
olastring::StripFilename
==================
*/
olastring &olastring::stripFilename( void ) {
	int pos;

	pos = length() - 1;
	while( ( pos > 0 ) && ( ( *this )[ pos ] != '/' ) && ( ( *this )[ pos ] != '\\' ) ) {
		pos--;
	}

	if ( pos < 0 ) {
		pos = 0;
	}

	capLength( pos );
	return *this;
}

/*
==================
olastring::StripPath
==================
*/
olastring &olastring::stripPath( void ) {
	int pos;

	pos = length();
	while( ( pos > 0 ) && ( ( *this )[ pos - 1 ] != '/' ) && ( ( *this )[ pos - 1 ] != '\\' ) ) {
		pos--;
	}

	*this = right( length() - pos );
	return *this;
}

/*
====================
olastring::ExtractFilePath
====================
*/
void olastring::extractFilePath( olastring &dest ) const {
	int pos;

	//
	// back up until a \ or the start
	//
	pos = length();
	while( ( pos > 0 ) && ( ( *this )[ pos - 1 ] != '/' ) && ( ( *this )[ pos - 1 ] != '\\' ) ) {
		pos--;
	}

	left( pos, dest );
}

/*
====================
olastring::ExtractFileName
====================
*/
void olastring::extractFileName( olastring &dest ) const {
	int pos;

	//
	// back up until a \ or the start
	//
	pos = length() - 1;
	while( ( pos > 0 ) && ( ( *this )[ pos - 1 ] != '/' ) && ( ( *this )[ pos - 1 ] != '\\' ) ) {
		pos--;
	}

	right( length() - pos, dest );
}

/*
====================
olastring::ExtractFileBase
====================
*/
void olastring::extractFileBase( olastring &dest ) const {
	int pos;
	int start;

	//
	// back up until a \ or the start
	//
	pos = length() - 1;
	while( ( pos > 0 ) && ( ( *this )[ pos - 1 ] != '/' ) && ( ( *this )[ pos - 1 ] != '\\' ) ) {
		pos--;
	}

	start = pos;
	while( ( pos < length() ) && ( ( *this )[ pos ] != '.' ) ) {
		pos++;
	}

	mid( start, pos - start, dest );
}

/*
====================
olastring::ExtractFileExtension
====================
*/
void olastring::extractFileExtension( olastring &dest ) const {
	int pos;

	//
	// back up until a . or the start
	//
	pos = length() - 1;
	while( ( pos > 0 ) && ( ( *this )[ pos - 1 ] != '.' ) ) {
		pos--;
	}

	if ( !pos ) {
		// no extension
		dest.empty();
	} else {
		right( length() - pos, dest );
	}
}


/*
=====================================================================

  char * methods to replace library functions

=====================================================================
*/

/*
============
olastring::IsNumeric

Checks a string to see if it contains only numerical values.
============
*/
bool olastring::isNumeric( const char *s ) {
	int		i;
	bool	dot;

	if ( *s == '-' ) {
		s++;
	}

	dot = false;
	for ( i = 0; s[i]; i++ ) {
		if ( !isdigit( s[i] ) ) {
			if ( ( s[ i ] == '.' ) && !dot ) {
				dot = true;
				continue;
			}
			return false;
		}
	}

	return true;
}

/*
============
olastring::HasLower

Checks if a string has any lowercase chars
============
*/
bool olastring::hasLower( const char *s ) {
	if ( !s ) {
		return false;
	}
	
	while ( *s ) {
		if ( charIsLower( *s ) ) {
			return true;
		}
		s++;
	}
	
	return false;
}

/*
============
olastring::HasUpper
	
Checks if a string has any uppercase chars
============
*/
bool olastring::hasUpper( const char *s ) {
	if ( !s ) {
		return false;
	}
	
	while ( *s ) {
		if ( charIsUpper( *s ) ) {
			return true;
		}
		s++;
	}
	
	return false;
}

/*
================
olastring::Cmp
================
*/
int olastring::cmp( const char *s1, const char *s2 ) {
	int c1, c2, d;

	do {
		c1 = *s1++;
		c2 = *s2++;

		d = c1 - c2;
		if ( d ) {
			return ( INTSIGNBITNOTSET( d ) << 1 ) - 1;
		}
	} while( c1 );

	return 0;		// strings are equal
}

/*
================
olastring::Cmpn
================
*/
int olastring::cmpn( const char *s1, const char *s2, int n ) {
	int c1, c2, d;

	assert( n >= 0 );

	do {
		c1 = *s1++;
		c2 = *s2++;

		if ( !n-- ) {
			return 0;		// strings are equal until end point
		}

		d = c1 - c2;
		if ( d ) {
			return ( INTSIGNBITNOTSET( d ) << 1 ) - 1;
		}
	} while( c1 );

	return 0;		// strings are equal
}

/*
================
olastring::Icmp
================
*/
int olastring::icmp( const char *s1, const char *s2 ) {
	int c1, c2, d;

	do {
		c1 = *s1++;
		c2 = *s2++;

		d = c1 - c2;
		while( d ) {
			if ( c1 <= 'Z' && c1 >= 'A' ) {
				d += ('a' - 'A');
				if ( !d ) {
					break;
				}
			}
			if ( c2 <= 'Z' && c2 >= 'A' ) {
				d -= ('a' - 'A');
				if ( !d ) {
					break;
				}
			}
			return ( INTSIGNBITNOTSET( d ) << 1 ) - 1;
		}
	} while( c1 );

	return 0;		// strings are equal
}

/*
================
olastring::Icmpn
================
*/
int olastring::icmpn( const char *s1, const char *s2, int n ) {
	int c1, c2, d;

	assert( n >= 0 );

	do {
		c1 = *s1++;
		c2 = *s2++;

		if ( !n-- ) {
			return 0;		// strings are equal until end point
		}

		d = c1 - c2;
		while( d ) {
			if ( c1 <= 'Z' && c1 >= 'A' ) {
				d += ('a' - 'A');
				if ( !d ) {
					break;
				}
			}
			if ( c2 <= 'Z' && c2 >= 'A' ) {
				d -= ('a' - 'A');
				if ( !d ) {
					break;
				}
			}
			return ( INTSIGNBITNOTSET( d ) << 1 ) - 1;
		}
	} while( c1 );

	return 0;		// strings are equal
}

/*
================
olastring::Icmp
================
*/
int olastring::icmpNoColor( const char *s1, const char *s2 ) {
	int c1, c2, d;

	do {
		while ( olastring::isColor( s1 ) ) {
			s1 += 2;
		}
		while ( olastring::isColor( s2 ) ) {
			s2 += 2;
		}
		c1 = *s1++;
		c2 = *s2++;

		d = c1 - c2;
		while( d ) {
			if ( c1 <= 'Z' && c1 >= 'A' ) {
				d += ('a' - 'A');
				if ( !d ) {
					break;
				}
			}
			if ( c2 <= 'Z' && c2 >= 'A' ) {
				d -= ('a' - 'A');
				if ( !d ) {
					break;
				}
			}
			return ( INTSIGNBITNOTSET( d ) << 1 ) - 1;
		}
	} while( c1 );

	return 0;		// strings are equal
}

/*
================
olastring::IcmpPath
================
*/
int olastring::icmpPath( const char *s1, const char *s2 ) {
	int c1, c2, d;

#if 0
//#if !defined( _WIN32 )
	//idLib::common->Printf( "WARNING: IcmpPath used on a case-sensitive filesystem?\n" );
#endif

	do {
		c1 = *s1++;
		c2 = *s2++;

		d = c1 - c2;
		while( d ) {
			if ( c1 <= 'Z' && c1 >= 'A' ) {
				d += ('a' - 'A');
				if ( !d ) {
					break;
				}
			}
			if ( c1 == '\\' ) {
				d += ('/' - '\\');
				if ( !d ) {
					break;
				}
			}
			if ( c2 <= 'Z' && c2 >= 'A' ) {
				d -= ('a' - 'A');
				if ( !d ) {
					break;
				}
			}
			if ( c2 == '\\' ) {
				d -= ('/' - '\\');
				if ( !d ) {
					break;
				}
			}
			// make sure folders come first
			while( c1 ) {
				if ( c1 == '/' || c1 == '\\' ) {
					break;
				}
				c1 = *s1++;
			}
			while( c2 ) {
				if ( c2 == '/' || c2 == '\\' ) {
					break;
				}
				c2 = *s2++;
			}
			if ( c1 && !c2 ) {
				return -1;
			} else if ( !c1 && c2 ) {
				return 1;
			}
			// same folder depth so use the regular compare
			return ( INTSIGNBITNOTSET( d ) << 1 ) - 1;
		}
	} while( c1 );

	return 0;
}

/*
================
olastring::IcmpnPath
================
*/
int olastring::icmpnPath( const char *s1, const char *s2, int n ) {
	int c1, c2, d;

#if 0
//#if !defined( _WIN32 )
	//idLib::common->Printf( "WARNING: IcmpPath used on a case-sensitive filesystem?\n" );
#endif

	assert( n >= 0 );

	do {
		c1 = *s1++;
		c2 = *s2++;

		if ( !n-- ) {
			return 0;		// strings are equal until end point
		}

		d = c1 - c2;
		while( d ) {
			if ( c1 <= 'Z' && c1 >= 'A' ) {
				d += ('a' - 'A');
				if ( !d ) {
					break;
				}
			}
			if ( c1 == '\\' ) {
				d += ('/' - '\\');
				if ( !d ) {
					break;
				}
			}
			if ( c2 <= 'Z' && c2 >= 'A' ) {
				d -= ('a' - 'A');
				if ( !d ) {
					break;
				}
			}
			if ( c2 == '\\' ) {
				d -= ('/' - '\\');
				if ( !d ) {
					break;
				}
			}
			// make sure folders come first
			while( c1 ) {
				if ( c1 == '/' || c1 == '\\' ) {
					break;
				}
				c1 = *s1++;
			}
			while( c2 ) {
				if ( c2 == '/' || c2 == '\\' ) {
					break;
				}
				c2 = *s2++;
			}
			if ( c1 && !c2 ) {
				return -1;
			} else if ( !c1 && c2 ) {
				return 1;
			}
			// same folder depth so use the regular compare
			return ( INTSIGNBITNOTSET( d ) << 1 ) - 1;
		}
	} while( c1 );

	return 0;
}

/*
=============
olastring::Copynz
 
Safe strncpy that ensures a trailing zero
=============
*/
void olastring::copynz( char *dest, const char *src, int destsize ) {
	if ( !src ) {
	//	idLib::common->Warning( "olastring::Copynz: 0 src" );
		return;
	}
	if ( destsize < 1 ) {
	//	idLib::common->Warning( "olastring::Copynz: destsize < 1" ); 
		return;
	}

	strncpy( dest, src, destsize-1 );
    dest[destsize-1] = 0;
}

/*
================
olastring::Append

  never goes past bounds or leaves without a terminating 0
================
*/
void olastring::append( char *dest, int size, const char *src ) {
	int		l1;

	l1 = strlen( dest );
	if ( l1 >= size ) {
//		idLib::common->Error( "olastring::Append: already overflowed" );
	}
	olastring::copynz( dest + l1, src, size - l1 );
}

/*
================
olastring::LengthWithoutColors
================
*/
int olastring::lengthWithoutColors( const char *s ) {
	int len;
	const char *p;

	if ( !s ) {
		return 0;
	}

	len = 0;
	p = s;
	while( *p ) {
		if ( olastring::isColor( p ) ) {
			p += 2;
			continue;
		}
		p++;
		len++;
	}

	return len;
}

/*
================
olastring::RemoveColors
================
*/
char *olastring::removeColors( char *string ) {
	char *d;
	char *s;
	int c;

	s = string;
	d = string;
	while( (c = *s) != 0 ) {
		if ( olastring::isColor( s ) ) {
			s++;
		}		
		else {
			*d++ = c;
		}
		s++;
	}
	*d = '\0';

	return string;
}

/*
================
olastring::snPrintf
================
*/
int olastring::snPrintf( char *dest, int size, const char *fmt, ...) {
	int len;
	va_list argptr;
	char buffer[32000];	// big, but small enough to fit in PPC stack

	va_start( argptr, fmt );
	len = vsprintf( buffer, fmt, argptr );
	va_end( argptr );
	if ( len >= sizeof( buffer ) ) {
	//	idLib::common->Error( "olastring::snPrintf: overflowed buffer" );
	}
	if ( len >= size ) {
	//	idLib::common->Warning( "olastring::snPrintf: overflow of %i in %i\n", len, size );
		len = size;
	}
	olastring::copynz( dest, buffer, size );
	return len;
}

/*
============
olastring::vsnPrintf

vsnprintf portability:

C99 standard: vsnprintf returns the number of characters (excluding the trailing
'\0') which would have been written to the final string if enough space had been available
snprintf and vsnprintf do not write more than size bytes (including the trailing '\0')

win32: _vsnprintf returns the number of characters written, not including the terminating 0 character,
or a negative value if an output error occurs. If the number of characters to write exceeds count, then count 
characters are written and -1 is returned and no trailing '\0' is added.

olastring::vsnPrintf: always appends a trailing '\0', returns number of characters written (not including terminal \0)
or returns -1 on failure or if the buffer would be overflowed.
============
*/
int olastring::vsnPrintf( char *dest, int size, const char *fmt, va_list argptr ) {
	int ret;

#ifdef _WIN32
#undef _vsnprintf
	ret = _vsnprintf( dest, size-1, fmt, argptr );
#define _vsnprintf	use_olastring_vsnPrintf
#else
#undef vsnprintf
	ret = vsnprintf( dest, size, fmt, argptr );
#define vsnprintf	use_olastring_vsnPrintf
#endif
	dest[size-1] = '\0';
	if ( ret < 0 || ret >= size ) {
		return -1;
	}
	return ret;
}

/*
============
sprintf

Sets the value of the string using a printf interface.
============
*/
int sprintf( olastring &string, const char *fmt, ... ) {
	int l;
	va_list argptr;
	char buffer[32000];
	
	va_start( argptr, fmt );
	l = olastring::vsnPrintf( buffer, sizeof(buffer)-1, fmt, argptr );
	va_end( argptr );
	buffer[sizeof(buffer)-1] = '\0';

	string = buffer;
	return l;
}

/*
============
vsprintf

Sets the value of the string using a vprintf interface.
============
*/
int vsprintf( olastring &string, const char *fmt, va_list argptr ) {
	int l;
	char buffer[32000];
	
	l = olastring::vsnPrintf( buffer, sizeof(buffer)-1, fmt, argptr );
	buffer[sizeof(buffer)-1] = '\0';
	
	string = buffer;
	return l;
}

/*
============
va

does a varargs printf into a temp buffer
NOTE: not thread safe
============
*/
char *va( const char *fmt, ... ) {
	va_list argptr;
	static int index = 0;
	static char string[4][16384];	// in case called by nested functions
	char *buf;

	buf = string[index];
	index = (index + 1) & 3;

	va_start( argptr, fmt );
	vsprintf( buf, fmt, argptr );
	va_end( argptr );

	return buf;
}



/*
============
olastring::BestUnit
============
*/
int olastring::bestUnit( const char *format, float value, Measure_t measure ) {
	int unit = 1;
	while ( unit <= 3 && ( 1 << ( unit * 10 ) < value ) ) {
		unit++;
	}
	unit--;
	value /= 1 << ( unit * 10 );
	sprintf( *this, format, value );
	*this += " ";
	*this += units[ measure ][ unit ];
	return unit;
}

/*
============
olastring::SetUnit
============
*/
void olastring::setUnit( const char *format, float value, int unit, Measure_t measure ) {
	value /= 1 << ( unit * 10 );
	sprintf( *this, format, value );
	*this += " ";
	*this += units[ measure ][ unit ];	
}

/*
================
olastring::InitMemory
================
*/
void olastring::initMemory( void ) {
#ifdef USE_STRING_DATA_ALLOCATOR
	stringDataAllocator.Init();
#endif
}

/*
================
olastring::ShutdownMemory
================
*/
void olastring::shutdownMemory( void ) {
#ifdef USE_STRING_DATA_ALLOCATOR
	stringDataAllocator.Shutdown();
#endif
}

/*
================
olastring::PurgeMemory
================
*/
void olastring::purgeMemory( void ) {
#ifdef USE_STRING_DATA_ALLOCATOR
	stringDataAllocator.FreeEmptyBaseBlocks();
#endif
}

/*
================
olastring::ShowMemoryUsage_f
================
*/
//void olastring::ShowMemoryUsage_f( const idCmdArgs &args ) {
//#ifdef USE_STRING_DATA_ALLOCATOR
////	idLib::common->Printf( "%6d KB string memory (%d KB free in %d blocks, %d empty base blocks)\n",
////		stringDataAllocator.GetBaseBlockMemory() >> 10, stringDataAllocator.GetFreeBlockMemory() >> 10,
////			stringDataAllocator.GetNumFreeBlocks(), stringDataAllocator.GetNumEmptyBaseBlocks() );
//#endif
//}

/*
================
olastring::FormatNumber
================
*/
struct formatList_t {
	int			gran;
	int			count;
};

// elements of list need to decend in size
formatList_t formatList[] = {
	{ 1000000000, 0 },
	{ 1000000, 0 },
	{ 1000, 0 }
};

int numFormatList = sizeof(formatList) / sizeof( formatList[0] );


olastring olastring::formatNumber( int number ) {
	olastring string;
	bool hit;

	// reset
	for ( int i = 0; i < numFormatList; i++ ) {
		formatList_t *li = formatList + i;
		li->count = 0;
	}

	// main loop
	do {
		hit = false;

		for ( int i = 0; i < numFormatList; i++ ) {
			formatList_t *li = formatList + i;

			if ( number >= li->gran ) {
				li->count++;
				number -= li->gran;
				hit = true;
				break;
			}
		}
	} while ( hit );

	// print out
	bool found = false;

	for ( int i = 0; i < numFormatList; i++ ) {
		formatList_t *li = formatList + i;

		if ( li->count ) {
			if ( !found ) {
				string += va( "%i,", li->count );
			} else {
				string += va( "%3.3i,", li->count );
			}
			found = true;
		}
		else if ( found ) {
			string += va( "%3.3i,", li->count );
		}
	}

	if ( found ) {
		string += va( "%3.3i", number );
	}
	else {
		string += va( "%i", number );
	}

	// pad to proper size
	int count = 11 - string.length();

	for ( int i = 0; i < count; i++ ) {
		string.insert( " ", 0 );
	}

	return string;
}

