#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "BDI.h"
#include "LZW.h"
#include "BPC.h"
#include "image.h"
#include "cbmp.h"

int width=0;
int height=0;

int floats=1;

/*

// Reads a BMP image and returns a poiner to a BMP struct
BMP* bopen(char* file_path);

// Returns a deep copy of the BMP passed
BMP* b_deep_copy(BMP* to_copy);

// Takes a BMP pointer and returns the image width in pixels
int get_width(BMP* bmp);

// Takes a BMP pointer and returns the image height in pixels
int get_height(BMP* bmp);

// Takes a BMP pointer and returns the image depth in bytes
unsigned int get_depth(BMP* bmp);

// Takes a BMP pointer, (x, y), and sets the values of rgb pointers
void get_pixel_rgb(BMP* bmp, int x, int y, unsigned char* r, unsigned char* g, unsigned char* b);

// Takes a BMP pointer, (x, y), and rgb values and updates the pixel color
void set_pixel_rgb(BMP* bmp, int x, int y, unsigned char r, unsigned char g, unsigned char b);

// Writes the contents of the BMP pointer to the given file path
void bwrite(BMP* bmp, char* file_name);

// Frees all memory allocated for the BMP pointer
void bclose(BMP* bmp);

*/


FILE* open_file(FILE* file_pointer,int argc,char const *argv[]){ //abre o ficheiro da rede

    if (argc < 2) {
        printf("No file specified!\n");
        exit(0);
    }
    file_pointer = fopen(argv[1], "rb");
    if (file_pointer == NULL) {
        printf("No file found!\n");
        return NULL;
    }
    /*if (argv[2] == NULL) {
        printf("No width found!\n");
        return NULL;
    }
    width = atoi(argv[2]);
    if (argv[3] == NULL) {
        printf("No height found!\n");
        return NULL;
    }
    height = atoi(argv[3]);*/

    return file_pointer;
}

void close_file(FILE *file_pointer){
    fclose(file_pointer);
}

void run_LZW(int argc,char const *argv[]){
    FILE *fp = NULL,*compressed=NULL,*decompressed=NULL;
    int msec = 0;
    clock_t before = clock();
    clock_t difference;

    if((fp=open_file(fp,argc,argv))!=NULL){
        difference = clock() - before;
        msec = difference * 1000 / CLOCKS_PER_SEC;
        printf("PRE-LZW:Time taken %d seconds %d milliseconds\n",
        msec/1000, msec%1000);
        LZW(fp,compressed);
        difference = clock() - before;
        msec = difference * 1000 / CLOCKS_PER_SEC;
        printf("POS-LZW:Time taken %d seconds %d milliseconds\n",
        msec/1000, msec%1000);

        close_file(fp);

        difference = clock() - before;
        msec = difference * 1000 / CLOCKS_PER_SEC;
        printf("PRE-iLZW:Time taken %d seconds %d milliseconds\n",
        msec/1000, msec%1000);
        iLZW(compressed,decompressed);
        difference = clock() - before;
        msec = difference * 1000 / CLOCKS_PER_SEC;
        printf("POS-iLZW:Time taken %d seconds %d milliseconds\n",
        msec/1000, msec%1000);
        print_bits_LZW();
    }
}

void run_LZW2(int argc,char const *argv[]){
    FILE *fp = NULL,*compressed=NULL,*decompressed=NULL;
    int msec = 0;
    clock_t before = clock();
    clock_t difference;

    if((fp=open_file(fp,argc,argv))!=NULL){

        difference = clock() - before;
        msec = difference * 1000 / CLOCKS_PER_SEC;
        printf("PRE-LZW2:Time taken %d seconds %d milliseconds\n",
        msec/1000, msec%1000);
        LZW16(fp,compressed);
        difference = clock() - before;
        msec = difference * 1000 / CLOCKS_PER_SEC;
        printf("POS-LZW2:Time taken %d seconds %d milliseconds\n",
        msec/1000, msec%1000);

        close_file(fp);

        difference = clock() - before;
        msec = difference * 1000 / CLOCKS_PER_SEC;
        printf("PRE-iLZW2:Time taken %d seconds %d milliseconds\n",
        msec/1000, msec%1000);
        iLZW16(compressed,decompressed);
        difference = clock() - before;
        msec = difference * 1000 / CLOCKS_PER_SEC;
        printf("POS-iLZW2:Time taken %d seconds %d milliseconds\n",
        msec/1000, msec%1000);
    }
}

void run_LZW4(int argc,char const *argv[]){
    FILE *fp = NULL,*compressed=NULL,*decompressed=NULL;
    int msec = 0;
    clock_t before = clock();
    clock_t difference;

    if((fp=open_file(fp,argc,argv))!=NULL){

        difference = clock() - before;
        msec = difference * 1000 / CLOCKS_PER_SEC;
        printf("PRE-LZW4:Time taken %d seconds %d milliseconds\n",
        msec/1000, msec%1000);
        LZW32(fp,compressed);
        difference = clock() - before;
        msec = difference * 1000 / CLOCKS_PER_SEC;
        printf("POS-LZW4:Time taken %d seconds %d milliseconds\n",
        msec/1000, msec%1000);

        close_file(fp);

        difference = clock() - before;
        msec = difference * 1000 / CLOCKS_PER_SEC;
        printf("PRE-iLZW4:Time taken %d seconds %d milliseconds\n",
        msec/1000, msec%1000);
        iLZW32(compressed,decompressed);
        difference = clock() - before;
        msec = difference * 1000 / CLOCKS_PER_SEC;
        printf("POS-iLZW4:Time taken %d seconds %d milliseconds\n",
        msec/1000, msec%1000);
    }
}

void run_LZW8(int argc,char const *argv[]){
    FILE *fp = NULL,*compressed=NULL,*decompressed=NULL;
    int msec = 0;
    clock_t before = clock();
    clock_t difference;

    if((fp=open_file(fp,argc,argv))!=NULL){

        difference = clock() - before;
        msec = difference * 1000 / CLOCKS_PER_SEC;
        printf("PRE-LZW8:Time taken %d seconds %d milliseconds\n",
        msec/1000, msec%1000);
        LZW64(fp,compressed);
        difference = clock() - before;
        msec = difference * 1000 / CLOCKS_PER_SEC;
        printf("POS-LZW8:Time taken %d seconds %d milliseconds\n",
        msec/1000, msec%1000);

        close_file(fp);

        difference = clock() - before;
        msec = difference * 1000 / CLOCKS_PER_SEC;
        printf("PRE-iLZW8:Time taken %d seconds %d milliseconds\n",
        msec/1000, msec%1000);
        iLZW64(compressed,decompressed);
        difference = clock() - before;
        msec = difference * 1000 / CLOCKS_PER_SEC;
        printf("POS-iLZW8:Time taken %d seconds %d milliseconds\n",
        msec/1000, msec%1000);
    }
}

void run_BPC(int argc,char const *argv[]){
    int msec = 0,original_width=0,original_height=0;
    clock_t before = clock();
    clock_t difference;
    FILE *fp = NULL;
    unsigned char** red;
    unsigned char** blue;
    unsigned char** green;
    BMP *bmp,*compressed_photo,*newly_compressed;

    bmp=bopen(argv[1]);
    //printf("Width is %d, Height is %d\n", get_width(bmp),get_height(bmp));
    width=get_width(bmp);
    height=get_height(bmp);

    bwrite(bmp, "ORIGINAL.bmp");

    //ANCHOR alocação de matrizes
    red=(unsigned char**)malloc(height*sizeof(unsigned char*));
    for(int i=0;i<height;i++){
        red[i]=(unsigned char*)malloc(width*sizeof(unsigned char));
    }
    blue=(unsigned char**)malloc(height*sizeof(unsigned char*));
    for(int i=0;i<height;i++){
        blue[i]=(unsigned char*)malloc(width*sizeof(unsigned char));
    }
    green=(unsigned char**)malloc(height*sizeof(unsigned char*));
    for(int i=0;i<height;i++){
        green[i]=(unsigned char*)malloc(width*sizeof(unsigned char));
    }

    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            get_pixel_rgb(bmp, j,  i, &red[i][j], &green[i][j], &blue[i][j]);
        }
    }

    /*printf("===========PIXEIS================\n");
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            printf("%d|%d|%d\n",red[i][j],green[i][j],blue[i][j]);
        }
        printf("\n");
    }*/
    
    difference = clock() - before;
    msec = difference * 1000 / CLOCKS_PER_SEC;
    printf("RESET: Time taken %d seconds %d milliseconds\n",
    msec/1000, msec%1000);
    BPC(width,height,red,green,blue);
    difference = clock() - before;
    msec = difference * 1000 / CLOCKS_PER_SEC;
    printf("BPC: Time taken %d seconds %d milliseconds\n",
    msec/1000, msec%1000);
    //printf("BPC acaba com %d e %d\n",sent_attribute(1),sent_attribute(0));

    
    newly_compressed=b_deep_fake_copy(bmp,sent_attribute(1),sent_attribute(0),sent_matrix_full(0),sent_matrix_full(5),sent_matrix_full(1));
    bwrite(newly_compressed, "BPCcompressed.bmp");
    compressed_photo=bopen("BPCcompressed.bmp");

    //printf("BMP: %d||%d(%d)\n",get_width(bmp),get_height(bmp),get_depth(bmp));
    //printf("Newly: %d||%d(%d)\n",get_width(newly_compressed),get_height(newly_compressed),get_depth(newly_compressed));
    //printf("Compressed: %d||%d(%d)\n",get_width(compressed_photo),get_height(compressed_photo),get_depth(compressed_photo));

    //ANCHOR frees
    for(int i=0;i<height;i++){
        free(red[i]);
    }
    free(red);

    for(int i=0;i<height;i++){
        free(blue[i]);
    }
    free(blue);
    
    for(int i=0;i<height;i++){
        free(green[i]);
    }
    free(green);


    original_width=width;
    original_height=height;
    width=get_width(compressed_photo);
    height=get_height(compressed_photo);
    //printf("Original: %d e %d\n",original_width,original_height);
    //printf("Novo: %d e %d\n",width,height);
    
    //ANCHOR alocação de matrizes
    red=(unsigned char**)malloc(height*sizeof(unsigned char*));
    for(int i=0;i<height;i++){
        red[i]=(unsigned char*)malloc(width*sizeof(unsigned char));
    }
    blue=(unsigned char**)malloc(height*sizeof(unsigned char*));
    for(int i=0;i<height;i++){
        blue[i]=(unsigned char*)malloc(width*sizeof(unsigned char));
    }
    green=(unsigned char**)malloc(height*sizeof(unsigned char*));
    for(int i=0;i<height;i++){
        green[i]=(unsigned char*)malloc(width*sizeof(unsigned char));
    }

    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            get_pixel_rgb(compressed_photo, j,  i, &red[i][j], &green[i][j], &blue[i][j]);
        }
    }
    
    difference = clock() - before;
    msec = difference * 1000 / CLOCKS_PER_SEC;
    printf("RESET: Time taken %d seconds %d milliseconds\n",
    msec/1000, msec%1000);

    iBPC(original_width,original_height,width,height,red,green,blue);
    difference = clock() - before;
    msec = difference * 1000 / CLOCKS_PER_SEC;
    printf("Time taken %d seconds %d milliseconds\n",
    msec/1000, msec%1000);

    //printf("Limites: %d e %d\n",bmp->height,bmp->width);
    for(int i=0;i<bmp->height;i++){
        for(int j=0;j<bmp->width;j++){
            int line=i;
            int column=j;
            set_pixel_rgb(bmp, column,  line, sent_matrix(2,line,column), sent_matrix(4,line,column), sent_matrix(3,line,column));
            //printf("Linha %d, Coluna %d || %d e %d\n",line,column,i,j);
        }
    }
    bwrite(bmp, "BPCdecompressed.bmp");
    free_globals(original_height);
    
    //ANCHOR frees
    for(int i=0;i<height;i++){
        free(red[i]);
    }
    free(red);

    for(int i=0;i<height;i++){
        free(blue[i]);
    }
    free(blue);
    
    for(int i=0;i<height;i++){
        free(green[i]);
    }
    free(green);
}

void run_BDI(int argc,char const *argv[]){
    int msec = 0,original_width=0,original_height=0;
    clock_t before = clock();
    clock_t difference;
    FILE *fp = NULL;
    unsigned char** red;
    unsigned char** blue;
    unsigned char** green;
    unsigned char** red_floats;
    unsigned char** blue_floats;
    unsigned char** green_floats;
    unsigned int int_buffer;
    float float_buffer;
    int byte1,byte2,byte3,byte4;
    BMP *bmp,*compressed_photo,*newly_compressed;

    bmp=bopen(argv[1]);
    printf("Width is %d, Height is %d\n", get_width(bmp),get_height(bmp));
    width=get_width(bmp);
    height=get_height(bmp);

    bwrite(bmp, "ORIGINAL.bmp");


    //ANCHOR alocação de matrizes
    red=(unsigned char**)malloc(height*sizeof(unsigned char*));
    for(int i=0;i<height;i++){
        red[i]=(unsigned char*)malloc(width*sizeof(unsigned char));
    }
    blue=(unsigned char**)malloc(height*sizeof(unsigned char*));
    for(int i=0;i<height;i++){
        blue[i]=(unsigned char*)malloc(width*sizeof(unsigned char));
    }
    green=(unsigned char**)malloc(height*sizeof(unsigned char*));
    for(int i=0;i<height;i++){
        green[i]=(unsigned char*)malloc(width*sizeof(unsigned char));
    }

    if(floats==1){
        red_floats=(unsigned char**)malloc(height*sizeof(unsigned char*));
        for(int i=0;i<height;i++){
            red_floats[i]=(unsigned char*)malloc((4*width)*sizeof(unsigned char));
        }
        blue_floats=(unsigned char**)malloc(height*sizeof(unsigned char*));
        for(int i=0;i<height;i++){
            blue_floats[i]=(unsigned char*)malloc((4*width)*sizeof(unsigned char));
        }
        green_floats=(unsigned char**)malloc(height*sizeof(unsigned char*));
        for(int i=0;i<height;i++){
            green_floats[i]=(unsigned char*)malloc((4*width)*sizeof(unsigned char));
        }
    }

    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            get_pixel_rgb(bmp, j,  i, &red[i][j], &green[i][j], &blue[i][j]);
            if(floats==1){
                float_buffer=((float)red[i][j])/255;
                memcpy(&int_buffer, &float_buffer, sizeof(int_buffer));
                byte1 = (long)((int_buffer<<24)>>24);
                byte2 = (long)((int_buffer<<16)>>24);
                byte3 = (long)((int_buffer<<8)>>24);
                byte4 = (long)(int_buffer>>24);
                red_floats[i][(j*4)+0]=byte4;
                red_floats[i][(j*4)+1]=byte3;
                red_floats[i][(j*4)+2]=byte2;
                red_floats[i][(j*4)+3]=byte1;
                //printf("Number %d is float %f with bytes %x%x%x%x\n",red[i][j],float_buffer,byte4,byte3,byte2,byte1);
                float_buffer=((float)blue[i][j])/255;
                byte1 = (long)((int_buffer<<24)>>24);
                byte2 = (long)((int_buffer<<16)>>24);
                byte3 = (long)((int_buffer<<8)>>24);
                byte4 = (long)(int_buffer>>24);
                blue_floats[i][(j*4)+0]=byte4;
                blue_floats[i][(j*4)+1]=byte3;
                blue_floats[i][(j*4)+2]=byte2;
                blue_floats[i][(j*4)+3]=byte1;
                float_buffer=((float)green[i][j])/255;
                byte1 = (long)((int_buffer<<24)>>24);
                byte2 = (long)((int_buffer<<16)>>24);
                byte3 = (long)((int_buffer<<8)>>24);
                byte4 = (long)(int_buffer>>24);
                green_floats[i][(j*4)+0]=byte4;
                green_floats[i][(j*4)+1]=byte3;
                green_floats[i][(j*4)+2]=byte2;
                green_floats[i][(j*4)+3]=byte1;
            }
        }
    }

    /*printf("Line 0 is:\n");
    for(int i=0;i<32;i++){
        printf("%d_%x|",red[0][i],red[0][i]);
    }
    printf("\n");*/

    difference = clock() - before;
    msec = difference * 1000 / CLOCKS_PER_SEC;
    printf("RESET: Time taken %d seconds %d milliseconds\n",
    msec/1000, msec%1000);
    if(floats==1){
        BDI(4*width,height,red_floats,green_floats,blue_floats);
    }else{
        BDI(width,height,red,green,blue);
    }
    difference = clock() - before;
    msec = difference * 1000 / CLOCKS_PER_SEC;
    printf("BDI: Time taken %d seconds %d milliseconds\n",
    msec/1000, msec%1000);

    /* NOTE turn on
    newly_compressed=b_deep_fake_copy(bmp,sent_attribute(1),sent_attribute(0),sent_matrix_full(0),sent_matrix_full(5),sent_matrix_full(1));
    bwrite(newly_compressed, "FPCcompressed.bmp");
    compressed_photo=bopen("FPCcompressed.bmp");

    //ANCHOR frees
    for(int i=0;i<height;i++){
        free(red[i]);
    }
    free(red);

    for(int i=0;i<height;i++){
        free(blue[i]);
    }
    free(blue);
    
    for(int i=0;i<height;i++){
        free(green[i]);
    }
    free(green);

    /*NOTE turn on
    original_width=width;
    original_height=height;
    width=get_width(compressed_photo);
    height=get_height(compressed_photo);

    red=(unsigned char**)malloc(height*sizeof(unsigned char*));
    for(int i=0;i<height;i++){
        red[i]=(unsigned char*)malloc(width*sizeof(unsigned char));
    }
    blue=(unsigned char**)malloc(height*sizeof(unsigned char*));
    for(int i=0;i<height;i++){
        blue[i]=(unsigned char*)malloc(width*sizeof(unsigned char));
    }
    green=(unsigned char**)malloc(height*sizeof(unsigned char*));
    for(int i=0;i<height;i++){
        green[i]=(unsigned char*)malloc(width*sizeof(unsigned char));
    }

    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            get_pixel_rgb(compressed_photo, j,  i, &red[i][j], &green[i][j], &blue[i][j]);
        }
    }*/
    
    difference = clock() - before;
    msec = difference * 1000 / CLOCKS_PER_SEC;
    printf("RESET: Time taken %d seconds %d milliseconds\n",
    msec/1000, msec%1000);
    if(floats==1){
        iBDI(4*width,height,red_floats,green_floats,blue_floats);
    }else{
        iBDI(width,height,red,green,blue);
    }
    difference = clock() - before;
    msec = difference * 1000 / CLOCKS_PER_SEC;
    printf("iBDI: Time taken %d seconds %d milliseconds\n",
    msec/1000, msec%1000);

    /*for(int i=0;i<bmp->height;i++){
        for(int j=0;j<bmp->width;j++){
            int line=i;
            int column=j;
            set_pixel_rgb(bmp, column,  line, sent_matrix(2,line,column), sent_matrix(4,line,column), sent_matrix(3,line,column));
            //printf("Linha %d, Coluna %d || %d e %d\n",line,column,i,j);
        }
    }
    bwrite(bmp, "FPCdecompressed.bmp");*/
    free_globals_BDI(width*height);
    
    //ANCHOR frees
    for(int i=0;i<height;i++){
        free(red[i]);
    }
    free(red);

    for(int i=0;i<height;i++){
        free(blue[i]);
    }
    free(blue);
    
    for(int i=0;i<height;i++){
        free(green[i]);
    }
    free(green);

    if(floats==1){
        for(int i=0;i<height;i++){
            free(red_floats[i]);
        }
        free(red_floats);

        for(int i=0;i<height;i++){
            free(blue_floats[i]);
        }
        free(blue_floats);
        
        for(int i=0;i<height;i++){
            free(green_floats[i]);
        }
        free(green_floats);
    }
}

int main(int argc,char const *argv[]) {
    //run_LZW(argc,argv);
    //run_LZW2(argc,argv);
    //run_LZW4(argc,argv);
    //run_LZW8(argc,argv);
    run_BDI(argc,argv);
    //run_BPC(argc,argv);
}