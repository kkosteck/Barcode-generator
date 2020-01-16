#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "BMP.h"
#include "AuxFunctions.h"

void fillBitmapComponents( struct BitmapComponents *bitmap, FILE *barCode, int codeBarHeight, int codeBarWidth){
	int imageSize = (codeBarHeight * codeBarWidth) / BITS_OF_BYTE;
	bitmap->fileHeader.bfType = 19778; // = 'BM'
	bitmap->fileHeader.bfSize = imageSize + sizeof(struct BitmapFileHeader) + sizeof(struct BitmapCoreHeader) + 2*sizeof(struct RgbQuad);
	bitmap->fileHeader.bfReserved = 0;
	bitmap->fileHeader.bfOffBits = sizeof(struct BitmapFileHeader) + sizeof(struct BitmapCoreHeader) + 2*sizeof(struct RgbQuad);

	bitmap->coreHeader.bcSize = sizeof(struct BitmapCoreHeader);
	bitmap->coreHeader.bcWidth = codeBarWidth;
	bitmap->coreHeader.bcHeight = codeBarHeight;
	bitmap->coreHeader.bcPlanes = 1;
	bitmap->coreHeader.bcBpp = 1;

	bitmap->colour1.rgbBlue = 255;
	bitmap->colour1.rgbGreen = 255;
	bitmap->colour1.rgbRed = 255;
	bitmap->colour1.rgbReserved = 0;

	bitmap->colour2.rgbBlue = 0;
	bitmap->colour2.rgbGreen = 0;
	bitmap->colour2.rgbRed = 0;
	bitmap->colour2.rgbReserved = 0;

	fwrite(&bitmap->fileHeader.bfType, sizeof(word), 1, barCode);
	fwrite(&bitmap->fileHeader.bfSize, sizeof(dword), 1, barCode);
	fwrite(&bitmap->fileHeader.bfReserved, sizeof(dword), 1, barCode);
	fwrite(&bitmap->fileHeader.bfOffBits, sizeof(dword), 1, barCode);

	fwrite(&bitmap->coreHeader.bcSize, sizeof(dword), 1, barCode);
	fwrite(&bitmap->coreHeader.bcWidth, sizeof(word), 1, barCode);
	fwrite(&bitmap->coreHeader.bcHeight, sizeof(word), 1, barCode);
	fwrite(&bitmap->coreHeader.bcPlanes, sizeof(word), 1, barCode);
	fwrite(&bitmap->coreHeader.bcBpp, sizeof(word), 1, barCode);

	fwrite(&bitmap->colour1.rgbBlue, sizeof(Byte), 1, barCode);
	fwrite(&bitmap->colour1.rgbGreen, sizeof(Byte), 1, barCode);
	fwrite(&bitmap->colour1.rgbRed, sizeof(Byte), 1, barCode);
	fwrite(&bitmap->colour1.rgbReserved, sizeof(Byte), 1, barCode);

	fwrite(&bitmap->colour2.rgbBlue, sizeof(Byte), 1, barCode);
	fwrite(&bitmap->colour2.rgbGreen, sizeof(Byte), 1, barCode);
	fwrite(&bitmap->colour2.rgbRed, sizeof(Byte), 1, barCode);
	fwrite(&bitmap->colour2.rgbReserved, sizeof(Byte), 1, barCode);
}

char *textToCode39(char *textCode,int moduleWidth, int codeBarHeight, int codeBarWidth, char code39Encoded[][WIDTH_OF_ONE_MODULE+1], char code39Signs[]){
    //Bar code line with 1 module width
	char *basicRow = malloc((strlen(textCode)+2) * (WIDTH_OF_ONE_MODULE+1) * sizeof(char));
	if(basicRow == NULL){
		fputs("ERROR: run out of memory\n", stderr);
		exit(1);
	}
	char space[2] = "0";
	const int asteriskId = code39Index(code39Signs, '*');
	strcpy(basicRow, code39Encoded[asteriskId]);
	strcat(basicRow, space);
	for(int i = 0; i < strlen(textCode); ++i){
        int index = code39Index(code39Signs, textCode[i]);
		strcat(basicRow, code39Encoded[index]);
		strcat(basicRow, space);
	}
	strcat(basicRow, code39Encoded[asteriskId]);
	int codeWidthPadding = codeBarWidth;
	if(codeBarWidth%32 != 0)
		codeWidthPadding = codeBarWidth + 32 - codeBarWidth%32;
    //Applying module width
	char *pictureRow = malloc((codeWidthPadding + 1) * sizeof(char));
	if(pictureRow == NULL){
		fputs("ERROR: run out of memory\n", stderr);
		exit(1);
	}
	for(int i = 0; i < strlen(basicRow); ++i){
		for(int j = 0; j < moduleWidth; ++j)
			if(basicRow[i] == '1')
				pictureRow[i*moduleWidth + j] = '1';
			else
				pictureRow[i*moduleWidth + j] = '0';
	}
	//Padding to 32bit per row
	for(int i = codeBarWidth; i < codeWidthPadding; ++i)
		pictureRow[i] = '0';
	pictureRow[codeWidthPadding] = '\0';

	free(basicRow);
	return pictureRow;
}

void printImage(char *pictureRow, FILE *barCode, int codeBarHeight, int codeBarWidth){
	for(int i = 0; i < codeBarHeight; ++i){
        for(int j = 0; j < strlen(pictureRow); j+=8){
            char subText[9];
            for(int k = 0; k < 8; ++k)
                subText[k] = pictureRow[j+k];
            subText[8] = '\0';
            int temporary = bin2dec(subText);
            fwrite(&temporary, sizeof(Byte), 1, barCode);
        }
	}
}
