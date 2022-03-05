#ifndef _LZW_H
#define _LZW_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WORD_SIZE 80
#define SHORT_SIZE 65535
#define INT_SIZE 2147483647

void setup_table(char**,int);
void print_bits(int word_size);
void print_bits_LZW();
void decide_bit(int position);
void LZW(FILE*,FILE*);
void LZW16(FILE*,FILE*);
void LZW32(FILE*,FILE*);
void LZW64(FILE*,FILE*);
void iLZW(FILE*,FILE*);
void iLZW16(FILE*,FILE*);
void iLZW32(FILE*,FILE*);
void iLZW64(FILE*,FILE*);
void find_pattern(char*,char,int*,int);
int find_pattern4(char*);
void savein_table(char**,char* pattern,int size);
void LZW_savein_table(char** table,char* pattern,int size,int place);
int find_decoded(char*);

#endif