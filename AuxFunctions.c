#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdbool.h>
#include "AuxFunctions.h"

int bin2dec(char *str){
	int temp = 0;
	for(int i = 0; i < strlen(str); ++i)
		temp += (str[i] - '0')*pow(2,strlen(str)-i-1);
	return temp;
}

bool isNumber(char *text){
    for(int i = 0; i < strlen(text); ++i){
        if(!isdigit(text[i]))
            return false;
    }
    return true;
}

int code39Index(char code39Signs[], char sign){
    for(int i = 0; i < NUMBER_OF_CODE39_CHAR; ++i)
        if(code39Signs[i] == sign)
            return i;

    printf("ERROR: code39 invalid character\n");
    exit(EXIT_FAILURE);
}

void loadCode39Data(FILE *code39Data, char code39Encoded[][WIDTH_OF_ONE_MODULE+1],char code39Signs[]){
	int sign = getc(code39Data);
	int i = 0;
	while(sign != EOF){
        code39Signs[i] = sign;
		fscanf(code39Data, " %s%*c", &code39Encoded[i][0]);
		sign = getc(code39Data);
        ++i;
	}
}

void verifyInputData(int argc, char* argv[], char code39Signs[]){
	if(argc != 5){
		fputs("ERROR: Incomplete data provided\n", stderr);
		exit(EXIT_FAILURE);
	}

	char substr[5];
	strncpy(substr, argv[1] + (strlen(argv[1]) - 4), 4);
    if(strcmp(substr, ".bmp") != 0){
        fputs("ERROR: Bad file extension (should be .bmp)\n", stderr);
        exit(EXIT_FAILURE);
    }

    if(!isNumber(argv[2])){
        fputs("ERROR: Invalid image height specified\n", stderr);
        exit(EXIT_FAILURE);
    }
    if(!isNumber(argv[3])){
        fputs("ERROR: Invalid module width specified\n", stderr);
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i <strlen(argv[4]); ++i){
        bool check = false;
        for(int j = 0; j < NUMBER_OF_CODE39_CHAR - 1; ++j)
            if(argv[4][i] == code39Signs[j])
                check = true;
        if(!check){
            fputs("ERROR: code39 invalid character\n", stderr);
            exit(EXIT_FAILURE);
        }
    }
}
