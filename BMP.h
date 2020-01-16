#ifndef BMP_H
#define BMP_H

#define WIDTH_OF_ONE_MODULE 15

typedef uint16_t word;
typedef uint32_t dword;
typedef unsigned char Byte;

struct BitmapFileHeader{
	word bfType;
	dword bfSize;
	dword bfReserved;
	dword bfOffBits;
};

struct BitmapCoreHeader{
	dword bcSize;
	word bcWidth;
	word bcHeight;
	word bcPlanes;
	word bcBpp;
};

struct RgbQuad{
	Byte rgbBlue;
	Byte rgbGreen;
	Byte rgbRed;
	Byte rgbReserved;
};

struct BitmapComponents{
	struct BitmapFileHeader fileHeader;
	struct BitmapCoreHeader coreHeader;
	struct RgbQuad colour1;
	struct RgbQuad colour2;
};

void fillBitmapComponents( struct BitmapComponents *bitmap, FILE *barCode, int codeBarHeight, int codeBarWidth);
char *textToCode39(char *textCode, int moduleWidth, int codeBarHeight, int codeBarWidth, char code39Encoded[][WIDTH_OF_ONE_MODULE+1], char code39Signs[]);
void printImage(char *pictureRow, FILE *barCode, int codeBarHeight, int codeBarWidth);

#endif

