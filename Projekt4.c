#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "BMP.h"
#include "AuxFunctions.h"

int main(int argc, char* argv[]){
    //LOADING CODE39 DATA
	FILE *code39Data = fopen("Code39.txt", "r");
	if(!code39Data){
		fputs("ERROR: could not open the file\n", stderr);
		return 1;
	}
	char code39Encoded[NUMBER_OF_CODE39_CHAR][WIDTH_OF_ONE_MODULE+1] = {0};
	char code39Signs[NUMBER_OF_CODE39_CHAR];
	loadCode39Data(code39Data, code39Encoded, code39Signs);
	fclose(code39Data);

	//LOADING DATA
    verifyInputData(argc, argv, code39Signs);
	char *filename = argv[1];
	int codeBarHeight = strtol(argv[2], NULL, 0);
	int moduleWidth = strtol(argv[3], NULL, 0);
	char *textCode = argv[4];

	//LOADING TEXT STRING
	int codeBarWidth = ((strlen(textCode)+2) * (WIDTH_OF_ONE_MODULE+1) - 1) * moduleWidth;
	char *pictureRow = textToCode39(textCode, moduleWidth, codeBarHeight, codeBarWidth, code39Encoded, code39Signs);

	//BITMAP
	FILE *barCode = fopen(filename, "w");
	if(!barCode){
		fputs("ERROR: could not open the file\n", stderr);
		return 1;
	}
	struct BitmapComponents *bitmap = malloc(sizeof(struct BitmapComponents));
	if(bitmap == NULL){
		fputs("ERROR: run out of memory\n", stderr);
		return 1;
	}
	fillBitmapComponents(bitmap, barCode, codeBarHeight, codeBarWidth);

	//ENTER BAR CODE DATA INTO BMP
	fseek(barCode, bitmap->fileHeader.bfOffBits, 0);
	printImage(pictureRow, barCode, codeBarHeight, codeBarWidth);

	free(bitmap);
	fclose(barCode);
}
