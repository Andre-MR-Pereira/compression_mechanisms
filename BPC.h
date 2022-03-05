#ifndef _BPC_H
#define _BPC_H

#include <stdio.h>
#include <stdlib.h>

int itob(int);
int MSB(int);
int btoi(int);
void BPC(int,int,unsigned char**,unsigned char**,unsigned char**);
void iBPC(int,int,int,int,unsigned char**,unsigned char**,unsigned char**);
unsigned char** allocate_compression_matrix(int width,int height);
unsigned char** allocate_decompression_matrix(int width,int height);
unsigned char sent_matrix(int choice,int line,int column);
unsigned char** sent_matrix_full(int choice);
int sent_attribute(int choice);
void free_globals(int height);

#endif