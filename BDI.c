#include "BDI.h"

const long ZEROS[] = {0,0,0,0};
const long REPEATED_VALUES[] = {0,0,0,1};
const long BASE8_D1[] = {0,0,1,0};
const long BASE8_D2[] = {0,0,1,1};
const long BASE8_D4[] = {0,1,0,0};
const long BASE4_D1[] = {0,1,0,1};
const long BASE4_D2[] = {0,1,1,0};
const long BASE2_D1[] = {0,1,1,1};
const long UNCOMPRESSED[] = {1,1,1,1};

int zeros_counter =0;
int rep_val_counter =0;
int b8_d1_counter =0;
int b8_d2_counter =0;
int b8_d4_counter =0;
int b4_d1_counter =0;
int b4_d2_counter =0;
int b2_d1_counter =0;
int uncomp_counter =0;

char example_cache_line[]={0,0,0,0,9,164,1,120,0,0,0,11,0,0,0,1,9,164,168,56,0,0,0,10,0,0,0,11,9,164,194,240};

long zero_base_number = ((long int)0<<56)|((long int)0<<48)|((long int)0<<40)|((long int)0<<32)|((long int)0<<24)|((long int)0<<16)|((long int)0<<8)| (long int)0;
int zero_base_number_int=(0<<24) |(0<<16) |(0<<8) | 0;

char zero_line_32[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
char zero_line_8[]={0,0,0,0,0,0,0,0};

unsigned char** compression_matrix_red;
unsigned char** compression_matrix_green;
unsigned char** compression_matrix_blue;
long** uncompression_matrix_red;
long** uncompression_matrix_green;
long** uncompression_matrix_blue;
int** bits_matrix_red;
int** bits_matrix_green;
int** bits_matrix_blue;

int uncompressed_line_size=32;
int BASE=8;

long compressible_line[32]; //NOTE uncompressed_line_size
int bit_line[32];

int zeros_unit(unsigned char* line){
    int base_size=1,output=1;

    for(int i=0;i<uncompressed_line_size;i++){
        if(line[i]!=0){
            output=0;
            break;
        }
    }
    //printf("Result: %d\n",output);
    return output;
}

int rep_values_unit(unsigned char* line){
    int base_size=8,output=1;
    unsigned char base[] = {line[0],line[1],line[2],line[3],line[4],line[5],line[6],line[7]};
    unsigned char value[base_size];

    //printf("Base is: %d %d %d %d %d %d %d %d | %x %x %x %x %x %x %x %x\n",base[0],base[1],base[2],base[3],base[4],base[5],base[6],base[7],base[0],base[1],base[2],base[3],base[4],base[5],base[6],base[7]);
    
    for(int i=base_size;i<uncompressed_line_size;i=i+base_size){
        value[0] = line[i];
        value[1] = line[i+1];
        value[2] = line[i+2];
        value[3] = line[i+3];
        value[4] = line[i+4];
        value[5] = line[i+5];
        value[6] = line[i+6];
        value[7] = line[i+7];
        //printf("Value is: %d %d %d %d %d %d %d %d | %x %x %x %x %x %x %x %x\n",value[0],value[1],value[2],value[3],value[4],value[5],value[6],value[7],value[0],value[1],value[2],value[3],value[4],value[5],value[6],value[7]);
        if(value[0]!=base[0] || value[1]!=base[1] ||value[2]!=base[2] ||value[3]!=base[3] ||value[4]!=base[4] ||value[5]!=base[5] ||value[6]!=base[6] ||value[7]!=base[7]){
            output=0;
            break;
        }
    }
    if(output==1){
        //printf("Base is: %d %d %d %d %d %d %d %d | %x %x %x %x %x %x %x %x\n",base[0],base[1],base[2],base[3],base[4],base[5],base[6],base[7],base[0],base[1],base[2],base[3],base[4],base[5],base[6],base[7]);
        compressible_line[0]= (long) base[0];
        compressible_line[1] = (long) base[1];
        compressible_line[2] = (long) base[2];
        compressible_line[3] = (long) base[3];
        compressible_line[4] = (long) base[4];
        compressible_line[5] = (long) base[5];
        compressible_line[6] = (long) base[6];
        compressible_line[7] = (long) base[7];
        //printf("Pattern is %ld %ld %ld %ld %ld %ld %ld %ld\n",compressible_line[0],compressible_line[1],
        //compressible_line[2],compressible_line[3],compressible_line[4],compressible_line[5],compressible_line[6],
        //compressible_line[7]);
    }
    //printf("Result: %d\n",output);
    return output;
}

int b8_d1_unit(unsigned char* line){
    int base_size=8,output=1;
    unsigned char base[base_size];
    unsigned char value[base_size];
    long diff=0,zero_diff=0;
    long base_number,value_number;

    for(int i=0;i<uncompressed_line_size;i=i+base_size){
        if(line[i+0]!=0 || line[i+1]!=0 || line[i+2]!=0 || line[i+3]!=0|| line[i+4]!=0|| line[i+5]!=0|| line[i+6]!=0|| line[i+7]!=0){
            base[0]=line[i+0];
            base[1]=line[i+1];
            base[2]=line[i+2];
            base[3]=line[i+3];
            base[4]=line[i+4];
            base[5]=line[i+5];
            base[6]=line[i+6];
            base[7]=line[i+7];
            break;
        }
    }

    //printf("Base is: %d %d %d %d %d %d %d %d | %x %x %x %x %x %x %x %x\n",base[0],base[1],base[2],base[3],base[4],base[5],base[6],base[7],base[0],base[1],base[2],base[3],base[4],base[5],base[6],base[7]);
    base_number = ((long int)base[0]<<56)|((long int)base[1]<<48)|((long int)base[2]<<40)|((long int)base[3]<<32)|((long int)base[4]<<24)|((long int)base[5]<<16)|((long int)base[6]<<8)| (long int)base[7];
    //printf("Base is %lu | %lx\n",base_number,base_number);
    //printf("Zero is %lu | %lx\n",zero_base_number,zero_base_number);
    compressible_line[0] = (long) base_number;
    for(int i=0;i<uncompressed_line_size;i=i+base_size){
        //printf("$$$$$$$$$$$$$$$$$$$$$$$\tNovaLinha\t$$$$$$$$$$$$$$$$\n");
        value[0] = line[i];
        value[1] = line[i+1];
        value[2] = line[i+2];
        value[3] = line[i+3];
        value[4] = line[i+4];
        value[5] = line[i+5];
        value[6] = line[i+6];
        value[7] = line[i+7];
        //printf("Value is: %d %d %d %d %d %d %d %d | %x %x %x %x %x %x %x %x\n",value[0],value[1],value[2],value[3],value[4],value[5],value[6],value[7],value[0],value[1],value[2],value[3],value[4],value[5],value[6],value[7]);
        value_number =  ((long int)value[0]<<56)|((long int)value[1]<<48)|((long int)value[2]<<40)|((long int)value[3]<<32)|((long int)value[4]<<24)|((long int)value[5]<<16)|((long int)value[6]<<8)| (long int)value[7];
        //printf("Value is %lu | %lx\n",value_number,value_number);
        zero_diff=value_number-zero_base_number;
	    diff=value_number-base_number;
        //printf("\tDiff is: %ld | %lx\n",diff,diff);
        //printf("\tZero Diff is: %ld | %lx\n",zero_diff,zero_diff);
        if(labs(zero_diff) <= 255){
            //printf("\t\tZero base on it\n");
            compressible_line[(i/base_size)+1] = (long) zero_diff;
            if(zero_diff<0){
                bit_line[(i/base_size)]=0;
            }else{
                bit_line[(i/base_size)]=1;
            }
        }else if(labs(diff) <= 255){
            //printf("\t\tBase on it\n");
            compressible_line[(i/base_size)+1] = (long) diff;
            if(diff<0){
                bit_line[(i/base_size)]=2;
            }else{
                bit_line[(i/base_size)]=3;
            }
        }else{
            output=0;
            break;
        }
    }
    //printf("Result: %d\n",output);
    return output;
}

int b8_d2_unit(unsigned char* line){
    int base_size=8,output=1;
    unsigned char base[base_size];
    unsigned char value[base_size];
    long diff=0,zero_diff=0;
    long base_number,value_number;

    for(int i=0;i<uncompressed_line_size;i=i+base_size){
        if(line[i+0]!=0 || line[i+1]!=0 || line[i+2]!=0 || line[i+3]!=0|| line[i+4]!=0|| line[i+5]!=0|| line[i+6]!=0|| line[i+7]!=0){
            base[0]=line[i+0];
            base[1]=line[i+1];
            base[2]=line[i+2];
            base[3]=line[i+3];
            base[4]=line[i+4];
            base[5]=line[i+5];
            base[6]=line[i+6];
            base[7]=line[i+7];
            break;
        }
    }

    //printf("Base is: %d %d %d %d %d %d %d %d | %x %x %x %x %x %x %x %x\n",base[0],base[1],base[2],base[3],base[4],base[5],base[6],base[7],base[0],base[1],base[2],base[3],base[4],base[5],base[6],base[7]);
    base_number = ((long int)base[0]<<56)|((long int)base[1]<<48)|((long int)base[2]<<40)|((long int)base[3]<<32)|((long int)base[4]<<24)|((long int)base[5]<<16)|((long int)base[6]<<8)| (long int)base[7];
    //printf("Base is %lu | %lx\n",base_number,base_number);
    compressible_line[0] = (long) base_number;
    for(int i=0;i<uncompressed_line_size;i=i+base_size){
        value[0] = line[i];
        value[1] = line[i+1];
        value[2] = line[i+2];
        value[3] = line[i+3];
        value[4] = line[i+4];
        value[5] = line[i+5];
        value[6] = line[i+6];
        value[7] = line[i+7];
        //printf("Value is: %d %d %d %d %d %d %d %d | %x %x %x %x %x %x %x %x\n",value[0],value[1],value[2],value[3],value[4],value[5],value[6],value[7],value[0],value[1],value[2],value[3],value[4],value[5],value[6],value[7]);
        value_number =  ((long int)value[0]<<56)|((long int)value[1]<<48)|((long int)value[2]<<40)|((long int)value[3]<<32)|((long int)value[4]<<24)|((long int)value[5]<<16)|((long int)value[6]<<8)| (long int)value[7];
        //printf("Value is %lu | %lx\n",value_number,value_number);
        zero_diff=value_number-zero_base_number;
        diff=value_number-base_number;
        //printf("Diff is: %ld | %lx\n",diff,diff);   //ANCHOR B8D2
        //printf("\tZero Diff is: %ld | %lx\n",zero_diff,zero_diff);
        if(labs(zero_diff) <= 65535){
            //printf("\t\t[Zero base] GUARDA %ld\n",(long) zero_diff);
            compressible_line[(i/base_size)+1] = (long) zero_diff;
            //printf("[%d] : %ld\n",i/base_size,compressible_line[i/base_size]);
            if(zero_diff<0){
                bit_line[(i/base_size)]=0;
            }else{
                bit_line[(i/base_size)]=1;
            }
        }else if(labs(diff) <= 65535){
            //printf("\t\t[Base] GUARDA %ld\n",(long) diff);
            compressible_line[(i/base_size)+1] = (long) diff;
            //printf("[%d] : %ld\n",i/base_size,compressible_line[i/base_size]);
            if(diff<0){
                bit_line[(i/base_size)]=2;
            }else{
                bit_line[(i/base_size)]=3;
            }
        }else{
            output=0;
            break;
        }
    }
    //printf("Result: %d\n",output);
    return output;
}

int b8_d4_unit(unsigned char* line){
    int base_size=8,output=1;
    unsigned char base[base_size];
    unsigned char value[base_size];
    long diff=0,zero_diff=0;
    long base_number,value_number;

    for(int i=0;i<uncompressed_line_size;i=i+base_size){
        if(line[i+0]!=0 || line[i+1]!=0 || line[i+2]!=0 || line[i+3]!=0|| line[i+4]!=0|| line[i+5]!=0|| line[i+6]!=0|| line[i+7]!=0){
            base[0]=line[i+0];
            base[1]=line[i+1];
            base[2]=line[i+2];
            base[3]=line[i+3];
            base[4]=line[i+4];
            base[5]=line[i+5];
            base[6]=line[i+6];
            base[7]=line[i+7];
            break;
        }
    }

    //printf("Base is: %d %d %d %d %d %d %d %d | %x %x %x %x %x %x %x %x\n",base[0],base[1],base[2],base[3],base[4],base[5],base[6],base[7],base[0],base[1],base[2],base[3],base[4],base[5],base[6],base[7]);
    base_number = ((long int)base[0]<<56)|((long int)base[1]<<48)|((long int)base[2]<<40)|((long int)base[3]<<32)|((long int)base[4]<<24)|((long int)base[5]<<16)|((long int)base[6]<<8)| (long int)base[7];
    //printf("Base is %lu | %lx\n",base_number,base_number);
    compressible_line[0] = (long) base_number;
    for(int i=0;i<uncompressed_line_size;i=i+base_size){
        value[0] = line[i];
        value[1] = line[i+1];
        value[2] = line[i+2];
        value[3] = line[i+3];
        value[4] = line[i+4];
        value[5] = line[i+5];
        value[6] = line[i+6];
        value[7] = line[i+7];
        //printf("Value is: %d %d %d %d %d %d %d %d | %x %x %x %x %x %x %x %x\n",value[0],value[1],value[2],value[3],value[4],value[5],value[6],value[7],value[0],value[1],value[2],value[3],value[4],value[5],value[6],value[7]);
        value_number =  ((long int)value[0]<<56)|((long int)value[1]<<48)|((long int)value[2]<<40)|((long int)value[3]<<32)|((long int)value[4]<<24)|((long int)value[5]<<16)|((long int)value[6]<<8)| (long int)value[7];
        //printf("Value is %lu | %lx\n",value_number,value_number);
        zero_diff=value_number-zero_base_number;
        diff=value_number-base_number;
        //printf("Diff is: %ld | %lx\n",diff,diff);
        //printf("\tZero Diff is: %ld | %lx\n",zero_diff,zero_diff);
        if(labs(zero_diff) <= 4294967295){
            //printf("\t\tZero base on it\n");
            compressible_line[(i/base_size)+1] = (long) zero_diff;
            //printf("[%d] : %ld\n",i/base_size,compressible_line[i/base_size]);
            if(zero_diff<0){
                bit_line[(i/base_size)]=0;
            }else{
                bit_line[(i/base_size)]=1;
            }
        }else if(labs(diff) <= 4294967295){
            //printf("\t\tBase on it\n");
            compressible_line[(i/base_size)+1] = (long) diff;
            if(diff<0){
                bit_line[(i/base_size)]=2;
            }else{
                bit_line[(i/base_size)]=3;
            }
        }else{
            output=0;
            break;
        }
    }
    //printf("Result: %d\n",output);
    return output;
}

int b4_d1_unit(unsigned char* line){
    int base_size=4,output=1;
    unsigned char base[base_size];
    unsigned char value[base_size];
    int diff=0,zero_diff=0;
    unsigned int base_number,value_number;
    
    for(int i=0;i<uncompressed_line_size;i=i+base_size){
        if(line[i+0]!=0 || line[i+1]!=0 || line[i+2]!=0 || line[i+3]!=0){
            base[0]=line[i+0];
            base[1]=line[i+1];
            base[2]=line[i+2];
            base[3]=line[i+3];
            break;
        }
    }

    //printf("Base is: %d %d %d %d | %x %x %x %x\n",base[0],base[1],base[2],base[3],base[0],base[1],base[2],base[3]);
    base_number =  (base[0]<<24) |(base[1]<<16) |(base[2]<<8) | base[3];
    //printf("Base is %d | %x\n",base_number,base_number);
    compressible_line[0] = (long) base_number;
    for(int i=0;i<uncompressed_line_size;i=i+base_size){
        value[0] = line[i];
        value[1] = line[i+1];
        value[2] = line[i+2];
        value[3] = line[i+3];
        //printf("Value is: %d %d %d %d | %x %x %x %x\n",value[0],value[1],value[2],value[3],value[0],value[1],value[2],value[3]);
        value_number =  (value[0]<<24) |(value[1]<<16) |(value[2]<<8) | value[3];
        //printf("Value is %d | %x\n",value_number,value_number);
        zero_diff=value_number-zero_base_number_int;
        diff=value_number-base_number;
        //printf("\tDiff is: %d | %x\n",diff,diff);
        //printf("\tZero Diff is: %d | %x\n",zero_diff,zero_diff);
        if(abs(zero_diff) <= 255){
            //printf("\t\tZero base on it\n");
            compressible_line[(i/base_size)+1] = (long) zero_diff;
            if(zero_diff<0){
                bit_line[(i/base_size)]=0;
            }else{
                bit_line[(i/base_size)]=1;
            }
        }else if(abs(diff) <= 255){
            //printf("\t\tBase on it\n");
            compressible_line[(i/base_size)+1] = (long) diff;
            if(diff<0){
                bit_line[(i/base_size)]=2;
            }else{
                bit_line[(i/base_size)]=3;
            }
        }else{
            output=0;
            break;
        }
    }
    //printf("Result: %d\n",output);
    return output;
}

int b4_d2_unit(unsigned char* line){
    int base_size=4,output=1;
    unsigned char base[base_size];
    unsigned char value[base_size];
    int diff=0,zero_diff=0;
    unsigned int base_number,value_number;

    for(int i=0;i<uncompressed_line_size;i=i+base_size){
        if(line[i+0]!=0 || line[i+1]!=0 || line[i+2]!=0 || line[i+3]!=0){
            base[0]=line[i+0];
            base[1]=line[i+1];
            base[2]=line[i+2];
            base[3]=line[i+3];
            break;
        }
    }

    //printf("Base is: %d %d %d %d | %x %x %x %x\n",base[0],base[1],base[2],base[3],base[0],base[1],base[2],base[3]);
    base_number =  (base[0]<<24) |(base[1]<<16) |(base[2]<<8) | base[3];
    //printf("Base is %d | %x\n",base_number,base_number);
    compressible_line[0] = (long) base_number;
    for(int i=0;i<uncompressed_line_size;i=i+base_size){
        value[0] = line[i];
        value[1] = line[i+1];
        value[2] = line[i+2];
        value[3] = line[i+3];
        //printf("Value is: %d %d %d %d | %x %x %x %x\n",value[0],value[1],value[2],value[3],value[0],value[1],value[2],value[3]);
        value_number =  (value[0]<<24) |(value[1]<<16) |(value[2]<<8) | value[3];
        //printf("Value is %d | %x\n",value_number,value_number);
        zero_diff=value_number-zero_base_number_int;
        diff=value_number-base_number;
        //printf("\tDiff is: %d | %x\n",diff,diff);
        //printf("\tZero Diff is: %d | %x\n",zero_diff,zero_diff);
        if(abs(zero_diff) <= 65535){
            //printf("\t\tZero base on it\n");
            compressible_line[(i/base_size)+1] = (long) zero_diff;
            if(zero_diff<0){
                bit_line[(i/base_size)]=0;
                //printf("Coloca 0\n");
            }else{
                bit_line[(i/base_size)]=1;
                //printf("Coloca 1\n");
            }
        }else if(abs(diff) <= 65535){
            //printf("\t\tBase on it\n");
            compressible_line[(i/base_size)+1] = (long) diff;
            //printf("[%d]Guarda %ld\n",(i/base_size)+1,compressible_line[(i/base_size)+1]);
            if(diff<0){
                bit_line[(i/base_size)]=2;
                //printf("Coloca 2\n");
            }else{
                bit_line[(i/base_size)]=3;
                //printf("Coloca 3\n");
            }
        }else{
            output=0;
            break;
        }
    }
    //printf("Result: %d\n",output);
    return output;
}

int b2_d1_unit(unsigned char* line){
    int base_size=2,output=1;
    unsigned char base[base_size];
    unsigned char value[base_size];
    int diff=0,zero_diff=0;
    unsigned int base_number,value_number;

    for(int i=0;i<uncompressed_line_size;i=i+base_size){
        if(line[i+0]!=0 || line[i+1]!=0){
            base[0]=line[i+0];
            base[1]=line[i+1];
            break;
        }
    }

    //unsigned int result = (a<<24) | (b<<16)| (c<<8) | d;
    base_number =  (base[0]<<8) | base[1];
    //printf("Base is %d | %x\n",base_number,base_number);
    compressible_line[0] = (long) base_number;
    for(int i=0;i<uncompressed_line_size;i=i+base_size){
        value[0] = line[i];
        value[1] = line[i+1];
        value_number =  (value[0]<<8) | value[1];
        //printf("Value is %d | %x\n",value_number,value_number);
        zero_diff=value_number-zero_base_number_int;
        diff=value_number-base_number;
        //printf("Diff is: %d | %x\n",diff,diff);
        if(abs(zero_diff) <= 255){
            //printf("\t\tZero base on it\n");
            compressible_line[(i/base_size)+1] = (long) zero_diff;
            if(zero_diff<0){
                bit_line[(i/base_size)]=0;
            }else{
                bit_line[(i/base_size)]=1;
            }
        }else if(abs(diff) <= 255){
            //printf("\t\tBase on it\n");
            compressible_line[(i/base_size)+1] = (long) diff;
            if(diff<0){
                bit_line[(i/base_size)]=2;
            }else{
                bit_line[(i/base_size)]=3;
            }
        }else{
            output=0;
            break;
        }
    }
    //printf("Result: %d\n",output);
    return output;
}


unsigned char* extract_line(int width,int height,int starting_position,unsigned char** matrix){
    unsigned char* line;
    int row,row_step=0,column,column_step=0;

    starting_position*=uncompressed_line_size;

    row=starting_position/width;
    column=starting_position%width;

    line=(unsigned char*)malloc(uncompressed_line_size*sizeof(unsigned char));
    for(int i=0;i<uncompressed_line_size;i++){
        if(row+row_step<height){
            line[i]=matrix[row+row_step][column+column_step];
        }else{
            line[i]='0';
        }
        column_step++;
        if(column_step+column > width){
            row_step++;
            column=0;
            column_step=0;
        }
        //printf("line[%d] = %u : %d | %d\n",i,line[i],row+row_step,column_step+column);
    }
    return line;
}

long* assembly_cache_line(unsigned char* line){
    long* compressed_line;
    //linha com 32 entradas: fazer as diferencas
    //printf("\nStart\n");
    //printf("\t\tUnit tested: Zeros\n");
    if(zeros_unit(line)==1){
        //printf("Unit chosen: Zeros\n");
        compressed_line=(long*)malloc((4+uncompressed_line_size)*sizeof(long));
        for(int i=0;i<4;i++){
            compressed_line[i]=ZEROS[i];
        }
        zeros_counter++;
        return compressed_line;
    }
    //printf("\n======================\n");
    //printf("\t\tUnit tested: Rep.Values\n");
    if(rep_values_unit(line)==1){
        //printf("Unit chosen: Rep.Values\n");
        compressed_line=(long*)malloc((4+uncompressed_line_size)*sizeof(long));
        for(int i=0;i<4;i++){
            compressed_line[i]=REPEATED_VALUES[i];
        }
        for(int i=0;i<4+8;i++){
            compressed_line[i+4]=compressible_line[i];
        }
        rep_val_counter++;
        return compressed_line;
    }
    //printf("\n======================\n");
    //printf("\t\tUnit tested: b8_d1\n");
    if(b8_d1_unit(line)==1){
        //printf("Unit chosen: b8_d1\n");
        compressed_line=(long*)malloc((4+uncompressed_line_size)*sizeof(long));
        for(int i=0;i<4;i++){
            compressed_line[i]=BASE8_D1[i];
        }
        for(int i=0;i<(uncompressed_line_size/8)+1;i++){
            compressed_line[i+4]=compressible_line[i];
        }
        b8_d1_counter++;
        return compressed_line;
    }
    //printf("\n======================\n");
    //printf("\t\tUnit tested: b4_d1\n");
    if(b4_d1_unit(line)==1){
        //printf("Unit chosen: b4_d1\n");
        compressed_line=(long*)malloc((4+uncompressed_line_size)*sizeof(long));
        for(int i=0;i<4;i++){
            compressed_line[i]=BASE4_D1[i];
            //printf("%ld|\n",compressed_line[i]);
        }
        for(int i=0;i<(uncompressed_line_size/4)+1;i++){
            compressed_line[i+4]=compressible_line[i];
            //printf("%ld|\n",compressed_line[i]);
        }
        b4_d1_counter++;
        return compressed_line;
    }
    //printf("\n======================\n");
    //printf("\t\tUnit tested: b8_d2\n");
    if(b8_d2_unit(line)==1){
        //printf("\n\nUnit chosen: b8_d2\n\n"); //ANCHOR unidade
        compressed_line=(long*)malloc((4+uncompressed_line_size)*sizeof(long));
        //printf("\nCode:");
        for(int i=0;i<4;i++){
            compressed_line[i]=BASE8_D2[i];
            //printf("%ld|",compressed_line[i]);
        }
        //printf("\nValues:");
        for(int i=0;i<(uncompressed_line_size/8)+1;i++){
            compressed_line[i+4]=compressible_line[i];
            //printf("%ld|",compressed_line[i+4]);
        }
        //printf("\n");
        b8_d2_counter++;
        return compressed_line;
    }
    //printf("\n======================\n");
    //printf("\t\tUnit tested: b2_d1\n");
    if(b2_d1_unit(line)==1){
        //printf("Unit chosen: b2_d1\n");
        compressed_line=(long*)malloc((4+uncompressed_line_size)*sizeof(long));
        for(int i=0;i<4;i++){
            compressed_line[i]=BASE2_D1[i];
        }
        for(int i=0;i<(uncompressed_line_size/2)+1;i++){
            compressed_line[i+4]=compressible_line[i];
        }
        b2_d1_counter++;
        return compressed_line;
    }
    //printf("\n======================\n");
    //printf("\t\tUnit tested: b4_d2\n");
    if(b4_d2_unit(line)==1){
        //printf("Unit chosen: b4_d2\n");
        compressed_line=(long*)malloc((4+uncompressed_line_size)*sizeof(long));
        for(int i=0;i<4;i++){
            compressed_line[i]=BASE4_D2[i];
            //printf("%ld|",compressed_line[i]);
        }
        for(int i=0;i<(uncompressed_line_size/4)+1;i++){
            compressed_line[i+4]=compressible_line[i];
            //printf("%ld|",compressed_line[i+4]);
        }
        b4_d2_counter++;
        return compressed_line;
    }
    //printf("\n======================\n");
    //printf("\t\tUnit tested: b8_d4\n");
    if(b8_d4_unit(line)==1){
        //printf("Unit chosen: b8_d4\n");
        compressed_line=(long*)malloc((4+uncompressed_line_size)*sizeof(long));
        for(int i=0;i<4;i++){
            compressed_line[i]=BASE8_D4[i];
        }
        for(int i=0;i<(uncompressed_line_size/8)+1;i++){
            compressed_line[i+4]=compressible_line[i];
        }
        b8_d4_counter++;
        return compressed_line;
    }else{
        //printf("\n======================\n");
        //printf("Unit chosen: No compress\n");
        compressed_line=(long*)malloc((4+uncompressed_line_size)*sizeof(long));
        for(int i=0;i<4;i++){
            //printf("Passa %ld para [%d]\n",UNCOMPRESSED[i],i);
            compressed_line[i]=UNCOMPRESSED[i];
        }
        for(int i=0;i<uncompressed_line_size;i++){
            compressed_line[i+4]=(long) line[i];
        }
        uncomp_counter++;
        return  compressed_line;
    }
}

void BDI(int width,int height,unsigned char** red,unsigned char** green,unsigned char** blue){
    int matrix_size = width*height;
    long* compressed_line_red,*compressed_line_green,*compressed_line_blue;
    int contador=1;
    int compression_lines=matrix_size/uncompressed_line_size;
    
    if(matrix_size%uncompressed_line_size!=0){
        compression_lines++;
    }

    compression_matrix_red=BDI_allocate_compression_matrix(matrix_size,width,height,red);
    compression_matrix_green=BDI_allocate_compression_matrix(matrix_size,width,height,green);
    compression_matrix_blue=BDI_allocate_compression_matrix(matrix_size,width,height,blue);

    uncompression_matrix_red=BDI_allocate_decompression_matrix(compression_lines,(4+uncompressed_line_size));
    uncompression_matrix_green=BDI_allocate_decompression_matrix(compression_lines,(4+uncompressed_line_size));
    uncompression_matrix_blue=BDI_allocate_decompression_matrix(compression_lines,(4+uncompressed_line_size));

    bits_matrix_red=BDI_allocate_bits_matrix(compression_lines,uncompressed_line_size);
    bits_matrix_green=BDI_allocate_bits_matrix(compression_lines,uncompressed_line_size);
    bits_matrix_blue=BDI_allocate_bits_matrix(compression_lines,uncompressed_line_size);

    //printf("0: Unsigned char:%u\tBinary:%lld\tDecimal:%d\n",compression_matrix_red[0][0],intob(compression_matrix_red[0][0]),btoint(intob(compression_matrix_red[0][0])));
    //printf("1: Unsigned char:%u\tBinary:%lld\tDecimal:%d\n",compression_matrix_red[0][1],intob(compression_matrix_red[0][1]),btoint(intob(compression_matrix_red[0][1])));
    //printf("2: Unsigned char:%u\tBinary:%lld\tDecimal:%d\n",compression_matrix_red[0][2],intob(compression_matrix_red[0][2]),btoint(intob(compression_matrix_red[0][2])));
    
    for(int line=0;line<matrix_size/uncompressed_line_size;line++){
        /*printf("Cache line is: [");
        for(int j=0;j<uncompressed_line_size;j++){
            printf("%d|",compression_matrix_red[line][j]);
        }
        printf("]\n");*/
        compressed_line_red=assembly_cache_line(compression_matrix_red[line]);
        uncompression_matrix_red[line]=compressed_line_red;
        for(int i=0;i<uncompressed_line_size;i++){
            bits_matrix_red[line][i]=bit_line[i];
        }
        //printf("[%d]Real First line: %ld %ld %ld %ld -> %ld %ld %ld %ld\n",line,compressed_line_red[0],compressed_line_red[1],
        //compressed_line_red[2],compressed_line_red[3],compressed_line_red[4],compressed_line_red[5],
        //compressed_line_red[6],compressed_line_red[7]);
        
        compressed_line_green=assembly_cache_line(compression_matrix_green[line]);
        uncompression_matrix_green[line]=compressed_line_green;
        for(int i=0;i<uncompressed_line_size;i++){
            bits_matrix_green[line][i]=bit_line[i];
        }


        compressed_line_blue=assembly_cache_line(compression_matrix_blue[line]);
        uncompression_matrix_blue[line]=compressed_line_blue;
        for(int i=0;i<uncompressed_line_size;i++){
            bits_matrix_blue[line][i]=bit_line[i];
        }

        //printf("Teste 1: %d %d %d\n",bits_matrix_red[line][0],bits_matrix_green[line][0],bits_matrix_blue[line][0]);
        //printf("Teste 2: %d %d %d\n",bits_matrix_red[line][1],bits_matrix_green[line][1],bits_matrix_blue[line][1]);
        //printf("Teste 3: %d %d %d\n",bits_matrix_red[line][2],bits_matrix_green[line][2],bits_matrix_blue[line][2]);

        /*if(compressed_line[0]!=1 || compressed_line[1]!=1 || compressed_line[2]!=1 || compressed_line[3]!=1){
            //printf("\t[%d]Code: ",contador);
            for(int i=0;i<4;i++){
                //printf("%ld",compressed_line[i]);
            }
            //printf("\n");
            contador++;
        }*/
        //printf("\t[%d]Code: ",contador);
    }
    
    /*compressed_line_red=assembly_cache_line(example_cache_line);
    printf("[%d]Real First line: %ld %ld %ld %ld -> %ld %ld %ld %ld %ld %ld %ld\n",69,compressed_line_red[0],compressed_line_red[1],
        compressed_line_red[2],compressed_line_red[3],compressed_line_red[4],compressed_line_red[5],
        compressed_line_red[6],compressed_line_red[7],compressed_line_red[8],compressed_line_red[9],compressed_line_red[10]);
    uncompression_matrix_red[0]=compressed_line_red;
    for(int i=0;i<uncompressed_line_size;i++){
        bits_matrix_red[0][i]=bit_line[i];
    }*/

    printf("\t#####  Results #####\n");
    printf("Zeros: %d | %d\n",zeros_counter,zeros_counter);
    printf("Repeated Values: %d | %d\n",rep_val_counter,rep_val_counter*8);
    printf("B8_D1: %d | %d\n",b8_d1_counter,b8_d1_counter*12);
    printf("B8_D2: %d | %d\n",b8_d2_counter,b8_d2_counter*16);
    printf("B8_D4: %d | %d\n",b8_d4_counter,b8_d4_counter*24);
    printf("B4_D1: %d | %d\n",b4_d1_counter,b4_d1_counter*12);
    printf("B4_D2: %d | %d\n",b4_d2_counter,b4_d2_counter*20);
    printf("B2_D1: %d | %d\n",b2_d1_counter,b2_d1_counter*18);
    printf("Uncompressed: %d | %d\n",uncomp_counter,uncomp_counter*32);
    printf("\t Original byte size: %d | %d\n",3*compression_lines,3*matrix_size);
    int final_byte=zeros_counter+rep_val_counter*8+b8_d1_counter*12+
    b8_d2_counter*16+b8_d4_counter*24+b4_d1_counter*12+b4_d2_counter*20+
    b4_d2_counter*20+b2_d1_counter*18+uncomp_counter*32;
    printf("\t Final byte size: %d | %d\n",3*(compression_lines/2),final_byte);

    //free_globals_BDI(matrix_size);
}

void iBDI(int width,int height,unsigned char** red,unsigned char** green,unsigned char** blue){
    int matrix_size = width*height;
    int compression_lines=matrix_size/uncompressed_line_size;

    if(matrix_size%uncompressed_line_size!=0){
        compression_lines++;
    }

    for(int line=0;line<compression_lines;line++){
        decompression_unit(uncompression_matrix_red[line],bits_matrix_red[line]);
        decompression_unit(uncompression_matrix_green[line],bits_matrix_green[line]);
        decompression_unit(uncompression_matrix_blue[line],bits_matrix_blue[line]);
    }
    //decompression_unit(uncompression_matrix_red[0],bits_matrix_red[0]);
}

unsigned char* decompression_unit(long* uncompression_matrix,int* bits_matrix){
    char* line;
    int first=1,code=1000*(int)uncompression_matrix[0]+100*(int)uncompression_matrix[1]+10*(int)uncompression_matrix[2]+(int)uncompression_matrix[3];
    long adder_result=0;
    int base0,base1,base2,base3,base4,base5,base6,base7,value0,value1,value2,value3,value4,value5,value6,value7;

    line=(unsigned char*)malloc(uncompressed_line_size*sizeof(unsigned char));
    //printf("Code is %d\n",code);
    switch (code)
    {
        case 0:
            if(uncompressed_line_size==8){
                memcpy(line,zero_line_8,8);
            }else{
                memcpy(line,zero_line_32,32);
            }
            break;
        case 1:
            for(int i=0;i<uncompressed_line_size/8;i=i+8){
                line[i]=uncompression_matrix[4+0];
                line[i+1]=uncompression_matrix[4+1];
                line[i+2]=uncompression_matrix[4+2];
                line[i+3]=uncompression_matrix[4+3];
                line[i+4]=uncompression_matrix[4+4];
                line[i+5]=uncompression_matrix[4+5];
                line[i+6]=uncompression_matrix[4+6];
                line[i+7]=uncompression_matrix[4+7];
            }
            break;
        case 10:
            adder_result=uncompression_matrix[4];
            //printf("Base is %lu | %lx\n",adder_result,adder_result);
            base0=adder_result>>56 & 0xFF;
            base1=adder_result>>48 & 0xFF;
            base2=adder_result>>40 & 0xFF;
            base3=adder_result>>32 & 0xFF;
            base4=adder_result>>24 & 0xFF;
            base5=adder_result>>16 & 0xFF;
            base6=adder_result>>8 & 0xFF;
            base7=adder_result & 0xFF;
            //printf("Base is %d %d %d %d\n",base0,base1,base2,base3);
            for(int i=0;i<uncompressed_line_size/4;i++){
                if(bits_matrix[i]==0){
                    adder_result=zero_base_number-uncompression_matrix[5+i];
                }else if(bits_matrix[i]==1){
                    adder_result=zero_base_number+uncompression_matrix[5+i];
                }else if(bits_matrix[i]==2){
                    adder_result=uncompression_matrix[4]-uncompression_matrix[5+i];
                }else{
                    adder_result=uncompression_matrix[4]+uncompression_matrix[5+i];
                }
                value0=adder_result>>56 & 0xFF;
                value1=adder_result>>48 & 0xFF;
                value2=adder_result>>40 & 0xFF;
                value3=adder_result>>32 & 0xFF;
                value4=adder_result>>24 & 0xFF;
                value5=adder_result>>16 & 0xFF;
                value6=adder_result>>8 & 0xFF;
                value7=adder_result & 0xFF;
                //printf("[%d]:Bit is %d\n",i,bits_matrix_red[0][i]);
                //printf("[%d]:Value is %d %d %d %d\n",i,value0,value1,value2,value3);
            }
            break;
        case 11:
            adder_result=uncompression_matrix[4];
            //printf("Base is %lu | %lx\n",adder_result,adder_result);
            base0=adder_result>>56 & 0xFF;
            base1=adder_result>>48 & 0xFF;
            base2=adder_result>>40 & 0xFF;
            base3=adder_result>>32 & 0xFF;
            base4=adder_result>>24 & 0xFF;
            base5=adder_result>>16 & 0xFF;
            base6=adder_result>>8 & 0xFF;
            base7=adder_result & 0xFF;
            //printf("Base is %d %d %d %d\n",base0,base1,base2,base3);
            for(int i=0;i<uncompressed_line_size/4;i++){
                if(bits_matrix[i]==0){
                    adder_result=zero_base_number-uncompression_matrix[5+i];
                }else if(bits_matrix[i]==1){
                    adder_result=zero_base_number+uncompression_matrix[5+i];
                }else if(bits_matrix[i]==2){
                    adder_result=uncompression_matrix[4]-uncompression_matrix[5+i];
                }else{
                    adder_result=uncompression_matrix[4]+uncompression_matrix[5+i];
                }
                value0=adder_result>>56 & 0xFF;
                value1=adder_result>>48 & 0xFF;
                value2=adder_result>>40 & 0xFF;
                value3=adder_result>>32 & 0xFF;
                value4=adder_result>>24 & 0xFF;
                value5=adder_result>>16 & 0xFF;
                value6=adder_result>>8 & 0xFF;
                value7=adder_result & 0xFF;
                //printf("[%d]:Bit is %d\n",i,bits_matrix_red[0][i]);
                //printf("[%d]:Value is %d %d %d %d\n",i,value0,value1,value2,value3);
            }
            break;
        case 100:
            adder_result=uncompression_matrix[4];
            //printf("Base is %lu | %lx\n",adder_result,adder_result);
            base0=adder_result>>56 & 0xFF;
            base1=adder_result>>48 & 0xFF;
            base2=adder_result>>40 & 0xFF;
            base3=adder_result>>32 & 0xFF;
            base4=adder_result>>24 & 0xFF;
            base5=adder_result>>16 & 0xFF;
            base6=adder_result>>8 & 0xFF;
            base7=adder_result & 0xFF;
            //printf("Base is %d %d %d %d\n",base0,base1,base2,base3);
            for(int i=0;i<uncompressed_line_size/4;i++){
                if(bits_matrix[i]==0){
                    adder_result=zero_base_number-uncompression_matrix[5+i];
                }else if(bits_matrix[i]==1){
                    adder_result=zero_base_number+uncompression_matrix[5+i];
                }else if(bits_matrix[i]==2){
                    adder_result=uncompression_matrix[4]-uncompression_matrix[5+i];
                }else{
                    adder_result=uncompression_matrix[4]+uncompression_matrix[5+i];
                }
                value0=adder_result>>56 & 0xFF;
                value1=adder_result>>48 & 0xFF;
                value2=adder_result>>40 & 0xFF;
                value3=adder_result>>32 & 0xFF;
                value4=adder_result>>24 & 0xFF;
                value5=adder_result>>16 & 0xFF;
                value6=adder_result>>8 & 0xFF;
                value7=adder_result & 0xFF;
                //printf("[%d]:Bit is %d\n",i,bits_matrix_red[0][i]);
                //printf("[%d]:Value is %d %d %d %d\n",i,value0,value1,value2,value3);
            }
            break;
        case 101:
            adder_result=uncompression_matrix[4];
            //printf("Base is %lu | %lx\n",adder_result,adder_result);
            base0=adder_result>>24 & 0xFF;
            base1=adder_result>>16 & 0xFF;
            base2=adder_result>>8 & 0xFF;
            base3=adder_result & 0xFF;
            //printf("Base is %d %d %d %d\n",base0,base1,base2,base3);
            for(int i=0;i<uncompressed_line_size/4;i++){
                if(bits_matrix[i]==0){
                    adder_result=zero_base_number-uncompression_matrix[5+i];
                }else if(bits_matrix[i]==1){
                    adder_result=zero_base_number+uncompression_matrix[5+i];
                }else if(bits_matrix[i]==2){
                    adder_result=uncompression_matrix[4]-uncompression_matrix[5+i];
                }else{
                    adder_result=uncompression_matrix[4]+uncompression_matrix[5+i];
                }
                value0=adder_result>>24 & 0xFF;
                value1=adder_result>>16 & 0xFF;
                value2=adder_result>>8 & 0xFF;
                value3=adder_result & 0xFF;
                //printf("[%d]:Bit is %d\n",i,bits_matrix_red[0][i]);
                //printf("[%d]:Value is %d %d %d %d\n",i,value0,value1,value2,value3);
            }
            break;
        case 110:
            adder_result=uncompression_matrix[4];
            //printf("Base is %lu | %lx\n",adder_result,adder_result);
            base0=adder_result>>24 & 0xFF;
            base1=adder_result>>16 & 0xFF;
            base2=adder_result>>8 & 0xFF;
            base3=adder_result & 0xFF;
            //printf("Base is %d %d %d %d\n",base0,base1,base2,base3);
            for(int i=0;i<uncompressed_line_size/4;i++){
                if(bits_matrix[i]==0){
                    adder_result=zero_base_number-uncompression_matrix[5+i];
                }else if(bits_matrix[i]==1){
                    adder_result=zero_base_number+uncompression_matrix[5+i];
                }else if(bits_matrix[i]==2){
                    adder_result=uncompression_matrix[4]-uncompression_matrix[5+i];
                }else{
                    adder_result=uncompression_matrix[4]+uncompression_matrix[5+i];
                }
                value0=adder_result>>24 & 0xFF;
                value1=adder_result>>16 & 0xFF;
                value2=adder_result>>8 & 0xFF;
                value3=adder_result & 0xFF;
                //printf("[%d]:Bit is %d\n",i,bits_matrix_red[0][i]);
                //printf("[%d]:Value is %d %d %d %d\n",i,value0,value1,value2,value3);
            }
            break;
        case 111:
            adder_result=uncompression_matrix[4];
            //printf("Base is %lu | %lx\n",adder_result,adder_result);
            base2=adder_result>>8 & 0xFF;
            base3=adder_result & 0xFF;
            //printf("Base is %d %d %d %d\n",base0,base1,base2,base3);
            for(int i=0;i<uncompressed_line_size/2;i++){
                if(bits_matrix[i]==0){
                    adder_result=zero_base_number-uncompression_matrix[5+i];
                }else if(bits_matrix[i]==1){
                    adder_result=zero_base_number+uncompression_matrix[5+i];
                }else if(bits_matrix[i]==2){
                    adder_result=uncompression_matrix[4]-uncompression_matrix[5+i];
                }else{
                    adder_result=uncompression_matrix[4]+uncompression_matrix[5+i];
                }
                value2=adder_result>>8 & 0xFF;
                value3=adder_result & 0xFF;
                //printf("[%d]:Bit is %d\n",i,bits_matrix_red[0][i]);
                //printf("[%d]:Value is %d %d %d %d\n",i,value0,value1,value2,value3);
            }
            break;
        case 1111:
            
            break;
        default:
            printf("Wrong Code %d\n",code);
            break;
    }
}

void free_globals_BDI(int matrix_size){
    int compression_lines=matrix_size/uncompressed_line_size;

    if(matrix_size%uncompressed_line_size!=0){
        compression_lines++;
    }

    //ANCHOR frees uncompressed
    for(int i=0;i<compression_lines;i++){
        free(uncompression_matrix_red[i]);
        free(bits_matrix_red[i]);
    }
    free(uncompression_matrix_red);
    free(bits_matrix_red);

    for(int i=0;i<compression_lines;i++){
        free(uncompression_matrix_blue[i]);
        free(bits_matrix_blue[i]);
    }
    free(uncompression_matrix_blue);
    free(bits_matrix_blue);
    
    for(int i=0;i<compression_lines;i++){
        free(uncompression_matrix_green[i]);
        free(bits_matrix_green[i]);
    }
    free(uncompression_matrix_green);
    free(bits_matrix_green);

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

long** BDI_allocate_decompression_matrix(int compression_lines,int line_size){
    long** matrix=(long**)malloc(compression_lines*sizeof(long*));
    for(int i=0;i<compression_lines;i++){
        matrix[i]=(long*)malloc(line_size*sizeof(long));
    }

    return matrix;
}

int** BDI_allocate_bits_matrix(int compression_lines,int line_size){
    int** matrix=(int**)malloc(compression_lines*sizeof(int*));
    for(int i=0;i<compression_lines;i++){
        matrix[i]=(int*)malloc(line_size*sizeof(int));
    }

    return matrix;
}

unsigned char** BDI_allocate_compression_matrix(int matrix_size,int width,int height,unsigned char** matrix){
    int compression_lines=matrix_size/uncompressed_line_size;

    if(matrix_size%uncompressed_line_size!=0){
        compression_lines++;
    }

    unsigned char** compressed_matrix=(unsigned char**)malloc(compression_lines*sizeof(unsigned char*));
    
    for(int i=0;i<compression_lines;i++){
        compressed_matrix[i]=(unsigned char*)malloc(uncompressed_line_size*sizeof(unsigned char));
        compressed_matrix[i]=extract_line(width,height,i,matrix);
    }
    return compressed_matrix;
}

long long intob(int n){
    long long bin = 0;
    int rem, i = 1;

    while (n!=0) {
        rem = n % 2;
        n /= 2;
        bin += rem * i;
        i *= 10;
    }

    return bin;
}

int btoint(long long binary){
    int dec = 0, i = 0, rem;

    while (binary!=0) {
        rem = binary % 10;
        binary /= 10;
        dec += rem * pow(2, i);
        ++i;
    }

    return dec;
}