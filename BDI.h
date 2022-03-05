#ifndef _BDI_H
#define _BDI_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

int zeros_unit(unsigned char* line);
int rep_values_unit(unsigned char* line);
int b8_d1_unit(unsigned char* line);
int b8_d2_unit(unsigned char* line);
int b8_d4_unit(unsigned char* line);
int b4_d1_unit(unsigned char* line);
int b4_d2_unit(unsigned char* line);
int b2_d1_unit(unsigned char* line);

unsigned char* extract_line(int width,int height,int starting_position,unsigned char** matrix);
unsigned char** BDI_allocate_compression_matrix(int matrix_size,int width,int height,unsigned char** matrix);
long long intob(int n);
int btoint(long long binary);
void BDI(int width,int height,unsigned char** red,unsigned char** green,unsigned char** blue);
void iBDI(int width,int height,unsigned char** red,unsigned char** green,unsigned char** blue);
void free_globals_BDI(int matrix_size);
long* assembly_cache_line(unsigned char* line);
long** BDI_allocate_decompression_matrix(int width,int height);
int** BDI_allocate_bits_matrix(int compression_lines,int line_size);
unsigned char* decompression_unit(long* uncompression_matrix,int* bits_matrix);

#endif