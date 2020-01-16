#ifndef AuxFunctions_H
#define AuxFunctions_H

#define BITS_OF_BYTE 8
#define WIDTH_OF_ONE_MODULE 15
#define NUMBER_OF_CODE39_CHAR 44

int bin2dec(char *str);
int code39Index(char code39Signs[], char sign);
void loadCode39Data(FILE *code39Data, char code39Encoded[][WIDTH_OF_ONE_MODULE+1], char code39Signs[]);
void verifyInputData(int argc, char* argv[], char code39Signs[]);

#endif

