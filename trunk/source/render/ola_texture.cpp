#include <memory.h>

#include "ola_tga.h"

#include "ola_util.h"
#include "ola_texture.h"

#include "ola_assetmng.h"

OlaTexture::OlaTexture():
mWidth(0),
mHeight(0),
mBpp(0),
mRGBA(0),
mType(COLOR_UNKNOWN)
{

}

OlaTexture::~OlaTexture()
{

}

unsigned char testpixels[4 * 3] =
{
255, 0, 0, // Red
0, 255, 0, // Green
0, 0, 255, // Blue
255, 255, 255 // White
};

bool OlaTexture::filltest()
{
	return fill(testpixels,2,2,24);
}

typedef struct 
{
	unsigned char idLength;
	unsigned char colorMapType;

	// The image type.
#define TGA_TYPE_MAPPED 1
#define TGA_TYPE_COLOR 2
#define TGA_TYPE_GRAY 3
#define TGA_TYPE_MAPPED_RLE 9
#define TGA_TYPE_COLOR_RLE 10
#define TGA_TYPE_GRAY_RLE 11
	unsigned char imageType;

	// Color Map Specification.
	// We need to separately specify high and low bytes to avoid endianness
	// and alignment problems.
	unsigned char colorMapIndexLo, colorMapIndexHi;
	unsigned char colorMapLengthLo, colorMapLengthHi;
	unsigned char colorMapSize;

	// Image Specification.
	unsigned char xOriginLo, xOriginHi;
	unsigned char yOriginLo, yOriginHi;

	unsigned char widthLo, widthHi;
	unsigned char heightLo, heightHi;

	unsigned char bpp;

	/* Image descriptor.
	3-0: attribute bpp
	4:   left-to-right ordering
	5:   top-to-bottom ordering
	7-6: zero
	*/
#define TGA_DESC_ABITS 0x0f
#define TGA_DESC_HORIZONTAL 0x10
#define TGA_DESC_VERTICAL 0x20
	unsigned char descriptor;

} 
OlaTgaHeader;

typedef struct {
	unsigned int extensionAreaOffset;
	unsigned int developerDirectoryOffset;
#define TGA_SIGNATURE "TRUEVISION-XFILE"
	char signature[16];
	char dot;
	char null;
} 
OlaTgaFooter;

static char error[256];
static unsigned int verbose = 0;
static int totbytes = 0;

typedef struct {
	unsigned char *statebuf;
	int statelen;
	int laststate;
} 
OlaRLEstate;

int OlaTexture::readTga(OlaAsset* asset)
{
	unsigned char* byte = (unsigned char*)(asset->data);
	int len = asset->length;

	OlaTgaHeader tgaHeader;
	OlaTgaFooter tgaFooter;

	int foot_from = len - sizeof(OlaTgaFooter);
	memcpy(&tgaFooter,byte + foot_from,sizeof(OlaTgaFooter));

	if (memcmp(tgaFooter.signature, TGA_SIGNATURE,
		sizeof(tgaFooter.signature)) == 0) {
		lg("TGA: found New TGA\n");
	} else {
		lg("TGA: found Original TGA\n");
	}

	memcpy(&tgaHeader,byte,sizeof(OlaTgaHeader));

	int width = (tgaHeader.widthHi << 8) | tgaHeader.widthLo;
	int height = (tgaHeader.heightHi << 8) | tgaHeader.heightLo;
	int bpp = tgaHeader.bpp;
	lg("TGA: width=%d, height=%d, bpp=%d\n", width, height, bpp);

	int horzrev = tgaHeader.descriptor & TGA_DESC_HORIZONTAL;
	int vertrev = !(tgaHeader.descriptor & TGA_DESC_VERTICAL);

	int rle = 0;
	OlaRLEstate rleRec;
	OlaRLEstate *rleInfo;

	int format = COLOR_RGB;
	int components = 0;
	switch (tgaHeader.imageType) {
	  case TGA_TYPE_MAPPED_RLE:
		  rle = 1;
		  if (verbose) 
			  printf("TGA: run-length encoded\n");
	  case TGA_TYPE_MAPPED:
		  /* Test for alpha channel. */
		  format = COLOR_INDEX;
		  components = 1;
		  if (verbose) {
			  lg("TGA: %d bit indexed image (%d bit palette)\n",
				  tgaHeader.colorMapSize, bpp);
		  }
		  return 0;
		  break;

	  case TGA_TYPE_GRAY_RLE:
		  rle = 1;
		  if (verbose) 
			  lg("TGA: run-length encoded\n");
	  case TGA_TYPE_GRAY:
		  format = COLOR_GRAY;
		  components = 1;
		  if (verbose) lg("TGA: %d bit grayscale image\n", bpp);
		  break;

	  case TGA_TYPE_COLOR_RLE:
		  rle = 1;
		  if (verbose) 
			  lg("TGA: run-length encoded\n");
	  case TGA_TYPE_COLOR:
		  /* Test for alpha channel. */
		  if (bpp == 32) {
			  format = COLOR_RGBA; //;
			  components = 4;
			  if (verbose) {
				  lg("TGA: %d bit color image with alpha channel\n", bpp);
			  }
		  } else {
			  format = COLOR_RGB;
			  components = 3;
			  if (verbose) 
				  lg("TGA: %d bit color image\n", bpp);
		  }
		  break;

	  default:
		  if (verbose)
			  lg("TGA: unrecognized image type %d\n", tgaHeader.imageType);
		  return 0;
	}

	int index, colors, length;
	unsigned char *pixels,*cmap,*data;
	int i, j, k;
	int pelbytes, wbytes;
	if (tgaHeader.colorMapType == 1) {
		/* We need to read in the colormap. */
		index = (tgaHeader.colorMapIndexHi << 8) | tgaHeader.colorMapIndexLo;
		length = (tgaHeader.colorMapLengthHi << 8) | tgaHeader.colorMapLengthLo;

		if (verbose) {
			lg("TGA: reading color map (%d + %d) * (%d / 8)\n",
				index, length, tgaHeader.colorMapSize);
		}
		if (length == 0) {
			if (verbose) 
				lg("TGA: invalid color map length %d", length);
			return NULL;
		}
		if (tgaHeader.colorMapSize != 24) {
			/* We haven't implemented bit-packed fields yet. */			
			if (verbose) 
				lg("TGA: channel sizes other than 8 bits are unimplemented");
			return NULL;
		}

		pelbytes = tgaHeader.colorMapSize / 8;
		colors = length + index;
		cmap = (unsigned char*)malloc (colors * pelbytes);

		/* Zero the entries up to the beginning of the map. */
		memset(cmap, 0, index * pelbytes);

		/* Read in the rest of the colormap. */
		const unsigned char* cmap_start = byte + sizeof(OlaTgaHeader);
		memcpy(cmap,cmap_start,length);

		if (pelbytes >= 3) {
			/* Rearrange the colors from BGR to RGB. */
			int tmp;
			for (j = index; j < length * pelbytes; j += pelbytes) {
				tmp = cmap[j];
				cmap[j] = cmap[j + 2];
				cmap[j + 2] = tmp;
			}
		}
	} else {
		colors = 0;
		cmap = 0;
	}

	int start, end, dir;

	/* Allocate the data. */
	pelbytes = bpp / 8;
	pixels = (unsigned char *) malloc (width * height * pelbytes);

	if (rle) {
		rleRec.statebuf = 0;
		rleRec.statelen = 0;
		rleRec.laststate = 0;
		rleInfo = &rleRec;
		lg("can not read rle type tga \n");
		return 0;
	} else {
		rleInfo = 0;
	}

	wbytes = width * pelbytes;

	if (vertrev) {
		start = 0;
		end = height;
		dir = 1;
	} else {
		/* We need to reverse loading order of rows. */
		start = height-1;
		end = -1;
		dir = -1;
	}

	unsigned char* color_start = byte + sizeof(OlaTgaHeader);
	for (i = start; i != end; i += dir) {
		data = pixels + i*wbytes;
		unsigned char* idx = color_start + i*wbytes;

		memcpy(data,idx,wbytes);
 
		if (horzrev) {
			/* We need to mirror row horizontally. */
			for (j = 0; j < width/2; j++) {
				unsigned char tmp;

				for (k = 0; k < pelbytes; k++) {
					tmp = data[j*pelbytes+k];
					data[j*pelbytes+k] = data[(width-j-1)*pelbytes+k];
					data[(width-j-1)*pelbytes+k] = tmp;
				}
			}
		}
	}

	if (rle) {
		free(rleInfo->statebuf);
	}

	//bgra to rgab
	for (int h = 0 ; h < height ; h++)
	{
		for (int w = 0 ; w < width ; w++)
		{
			int idx = (h * height + w) * components;
			unsigned char c = pixels[idx + 0];
			pixels[idx + 0] = pixels[idx + 2];
			pixels[idx + 2] = c;
		}
	}

	bool b = fill(pixels,width,height,bpp);

	free(pixels);
	
	return b;
}