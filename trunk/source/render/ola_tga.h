/*********************************************************************NVMH1****
File:
nv_tga.h

Copyright (C) 1999, 2000 NVIDIA Corporation
This file is provided without support, instruction, or implied warranty of any
kind.  NVIDIA makes no guarantee of its fitness for a particular purpose and is
not liable under any circumstances for any damages or loss whatsoever arising
from the use or inability to use this file or items derived from it.

Comments:


******************************************************************************/

#ifndef __OLA_TGA_H__
#define __OLA_TGA_H__

namespace OlaTGA
{
	enum tgaCOLORFORMAT
	{
		COLOR_UNKNOWN = 0,	// unknown
		COLOR_INDEX,		// palette
		COLOR_GRAY,
		COLOR_RGB,
		COLOR_RGBA,
	};

	typedef struct 
	{
		int  width;
		int  height;
		int  components;
		unsigned int format;

		int  cmapEntries;
		unsigned int   cmapFormat;
		unsigned char *cmap;

		unsigned char *pixels;
	} 
	tgaImage;

	extern tgaImage * read(const char *filename);
} // namespace tga

#endif /* __nv_tga_h__ */
