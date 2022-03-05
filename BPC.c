#include "BPC.h"

unsigned char** compression_matrix_red;
unsigned char** compression_matrix_green;
unsigned char** compression_matrix_blue;
unsigned char** uncompression_matrix_red;
unsigned char** uncompression_matrix_green;
unsigned char** uncompression_matrix_blue;
int compression_columns=8;
int compression_lines=0;

int itob(int number){
    int a[8],i,result=0,exponent=1;
    for(i=0;i<8;i++)    
    {    
        a[i]=number%2;
        result+=a[i]*exponent;
        exponent=exponent*10;
        number=number/2;    
    }
    return result;  
}

int btoi(int binary){
    int result=0,bit=0;
    int msb = 1 << 7;
    int b6 = 1 << 6;
    int b5 = 1 << 5;
    int b4 = 1 << 4;
    int b3 = 1 << 3;
    int b2 = 1 << 2;
    int b1 = 1 << 1;
    int lsb = 1 << 0;

    bit= (binary/10000000);
    result= bit*msb;
    binary-= bit*10000000;
    
    bit= (binary/1000000);
    result+= bit*b6;
    binary-= bit*1000000;


    bit= (binary/100000);
    result+= bit*b5;
    binary-= bit*100000;


    bit= (binary/10000);
    result+= bit*b4;
    binary-= bit*10000;

    bit= (binary/1000);
    result+= bit*b3;
    binary-= bit*1000;

    bit= (binary/100);
    result+= bit*b2;
    binary-= bit*100;

    bit= (binary/10);
    result+= bit*b1;
    binary-= bit*10;

    result+= binary*lsb;
    return result;
}

int MSB(int binary){
    int msb = 1 << 7;
    if(binary & msb)
        return 1;
    else
        return 0;
}

void BPC(int width,int height,unsigned char** red,unsigned char** green,unsigned char** blue){
    int counter=0;
    int line=0;
    int column=0;
    compression_matrix_red=allocate_compression_matrix(width,height);
    compression_matrix_green=allocate_compression_matrix(width,height);
    compression_matrix_blue=allocate_compression_matrix(width,height);
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            line=counter/8;
            column=counter%8;
            compression_matrix_red[line][column]=(unsigned char)MSB(itob((int)red[i][j]));
            compression_matrix_green[line][column]=(unsigned char)MSB(itob((int)green[i][j]));
            compression_matrix_blue[line][column]=(unsigned char)MSB(itob((int)blue[i][j]));
            counter++;
        }
    }
    //printf("--------------\n");
    for(int i=0;i<compression_lines;i++){
        compression_matrix_red[i][0] = (unsigned char)btoi(1*compression_matrix_red[i][7]+10*compression_matrix_red[i][6]+100*compression_matrix_red[i][5]+1000*compression_matrix_red[i][4]+10000*compression_matrix_red[i][3]+100000*compression_matrix_red[i][2]+1000000*compression_matrix_red[i][1]+10000000*compression_matrix_red[i][0]);
        compression_matrix_green[i][0] = (unsigned char)btoi(1*compression_matrix_green[i][7]+10*compression_matrix_green[i][6]+100*compression_matrix_green[i][5]+1000*compression_matrix_green[i][4]+10000*compression_matrix_green[i][3]+100000*compression_matrix_green[i][2]+1000000*compression_matrix_green[i][1]+10000000*compression_matrix_green[i][0]);
        compression_matrix_blue[i][0] = (unsigned char)btoi(1*compression_matrix_blue[i][7]+10*compression_matrix_blue[i][6]+100*compression_matrix_blue[i][5]+1000*compression_matrix_blue[i][4]+10000*compression_matrix_blue[i][3]+100000*compression_matrix_blue[i][2]+1000000*compression_matrix_blue[i][1]+10000000*compression_matrix_blue[i][0]);
        //printf("%u || %u || %u\n",compression_matrix_red[i][0],compression_matrix_green[i][0],compression_matrix_blue[i][0]);  
    }
    //printf("--------------\n");
}

void iBPC(int original_width,int original_height,int width,int height,unsigned char** red,unsigned char** green,unsigned char** blue){
    int bit_red=0,bit_green=0,bit_blue=0,binary_red=0,binary_green=0,binary_blue=0;
    int line=0,column=0;
    int pow10[8] = {
        1, 10, 100, 1000, 10000, 
        100000, 1000000, 10000000
    };

    uncompression_matrix_red=allocate_decompression_matrix(original_width,original_height);
    uncompression_matrix_green=allocate_decompression_matrix(original_width,original_height);
    uncompression_matrix_blue=allocate_decompression_matrix(original_width,original_height);

    for(int i=0;i<height;i++){
        for(int k=0;k<width;k++){
            binary_red=itob((int)red[i][k]);
            binary_green=itob((int)green[i][k]);
            binary_blue=itob((int)blue[i][k]);
            for(int j=0;j<8;j++){
                line=(((i*255+k)*8)+j)/original_width;
                column=(((i*255+k)*8)+j)%original_width;
                if(line < original_height && column<original_width){
                    bit_red=(binary_red/pow10[7-j]);
                    bit_green=(binary_green/pow10[7-j]);
                    bit_blue=(binary_blue/pow10[7-j]);
                    //printf("(%d||%d)Comprimido: %d e %d -> (%d||%d)Real: %d e %d\n",height,width,i,k,original_height,original_width,line,column);
                    uncompression_matrix_red[line][column]=btoi(bit_red*pow10[7]);
                    uncompression_matrix_green[line][column]=btoi(bit_green*pow10[7]);
                    uncompression_matrix_blue[line][column]=btoi(bit_blue*pow10[7]);

                    binary_red-= bit_red*pow10[7-j];
                    binary_green-= bit_green*pow10[7-j];
                    binary_blue-= bit_blue*pow10[7-j];
                }else{
                    return;
                }
            }
        }
    }
}

unsigned char** allocate_compression_matrix(int width,int height){
    int size=width*height;
    compression_lines=size/8;

    if(size%8!=0){
        compression_lines++;
    }

    unsigned char** matrix=(unsigned char**)malloc(compression_lines*sizeof(unsigned char*));
    for(int i=0;i<compression_lines;i++){
        matrix[i]=(unsigned char*)malloc(compression_columns*sizeof(unsigned char));
    }
    if(size%8!=0){
        for(int i=0;i<size%8;i++){
            matrix[compression_lines-1][8-i]=(unsigned char) 0;
        }
    }
    return matrix;
}

unsigned char** allocate_decompression_matrix(int width,int height){
    unsigned char** matrix=(unsigned char**)malloc(height*sizeof(unsigned char*));
    for(int i=0;i<height;i++){
        matrix[i]=(unsigned char*)malloc(width*sizeof(unsigned char));
    }

    return matrix;
}

unsigned char sent_matrix(int choice,int line,int column){
    if(choice==0){
        return compression_matrix_red[line][column];
    }else if(choice==1){
        return compression_matrix_green[line][column];
    }else if(choice==2){
        return uncompression_matrix_red[line][column];
    }else if(choice==3){
        return uncompression_matrix_green[line][column];
    }else if(choice==4){
        return uncompression_matrix_blue[line][column];
    }
    else{
        return compression_matrix_blue[line][column];
    }
}

unsigned char** sent_matrix_full(int choice){
    if(choice==0){
        return compression_matrix_red;
    }else if(choice==1){
        return compression_matrix_green;
    }else if(choice==2){
        return uncompression_matrix_red;
    }else if(choice==3){
        return uncompression_matrix_green;
    }else if(choice==4){
        return uncompression_matrix_blue;
    }
    else{
        return compression_matrix_blue;
    }
}

void free_globals(int original_height){
    //ANCHOR frees uncompressed
    for(int i=0;i<original_height;i++){
        free(uncompression_matrix_red[i]);
    }
    free(uncompression_matrix_red);

    for(int i=0;i<original_height;i++){
        free(uncompression_matrix_blue[i]);
    }
    free(uncompression_matrix_blue);
    
    for(int i=0;i<original_height;i++){
        free(uncompression_matrix_green[i]);
    }
    free(uncompression_matrix_green);

    //ANCHOR frees compressed
    for(int i=0;i<compression_lines;i++){
        free(compression_matrix_red[i]);
    }
    free(compression_matrix_red);

    for(int i=0;i<compression_lines;i++){
        free(compression_matrix_blue[i]);
    }
    free(compression_matrix_blue);
    
    for(int i=0;i<compression_lines;i++){
        free(compression_matrix_green[i]);
    }
    free(compression_matrix_green);
}

int sent_attribute(int choice){
    if(choice==0){
        return compression_lines;
    }else{
        return 1;
    }
}