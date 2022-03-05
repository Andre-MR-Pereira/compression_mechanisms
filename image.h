#ifndef _IMAGE_H
#define _IMAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct tagBITMAPFILEHEADER
{
    unsigned short bfType;  //specifies the file type
    unsigned int bfSize;  //specifies the size in bytes of the bitmap file
    unsigned int bfReserved1;  //reserved; must be 0
    unsigned int bfReserved2;  //reserved; must be 0
    unsigned int bfOffBits;  //specifies the offset in bytes from the bitmapfileheader to the bitmap bits
}BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER
{
    unsigned int biSize;  //specifies the number of bytes required by the struct
    int biWidth;  //specifies width in pixels
    int biHeight;  //specifies height in pixels
    unsigned short biPlanes;  //specifies the number of color planes, must be 1
    unsigned short biBitCount;  //specifies the number of bits per pixel
    unsigned int biCompression;  //specifies the type of compression
    unsigned int biSizeImage;  //size of image in bytes
    int biXPelsPerMeter;  //number of pixels per meter in x axis
    int biYPelsPerMeter;  //number of pixels per meter in y axis
    unsigned int biClrUsed;  //number of colors used by the bitmap
    unsigned int biClrImportant;  //number of colors that are important
}BITMAPINFOHEADER;

struct bmp_header{
    unsigned short type;              // "BM" (0x42, 0x4D)
    unsigned int size;              // file size
    unsigned int reserved1;         // not used (0)
    unsigned int reserved2;         // not used (0)
    unsigned int offset;            // offset to image data (54B)
    unsigned int dib_size;          // DIB header size (40B)
    int width;             // width in pixels
    int height;            // height in pixels
    unsigned short planes;            // 1
    unsigned short bpp;               // bits per pixel (24)
    unsigned int compression;       // compression type (0/1/2) 0
    unsigned int image_size;        // size of picture in bytes, 0
    int x_ppm;             // X Pixels per meter (0)
    int y_ppm;             // X Pixels per meter (0)
    unsigned int num_colors;        // number of colors (0)
    unsigned int important_colors;  // important colors (0)
} __attribute__((__packed__));

struct pixel {
    int blue;
    int green;
    int red;
    //uint8_t alpha;
} __attribute__((__packed__));

void create_image(FILE*,int,int,int**,int**,int**,char*);
struct pixel* read_data(FILE* , const struct bmp_header* );
struct bmp_header* read_bmp_header(FILE* );
#endif