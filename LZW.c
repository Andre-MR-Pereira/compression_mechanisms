#include "LZW.h"

//NOTE short represent max: 32767
char* encoding_table[SHORT_SIZE*1000];
char* decoding_table[SHORT_SIZE*1000];
int last_placed=0;
int LZW_last_placed=0;
int iLZW_last_placed=0;
int Bit1 = 0;
int Bit2 = 0;
int Bit3 = 0;
int Bit4 = 0;
int Bit5 = 0;
int Bit6 = 0;
int Bit7 = 0;
int Bit8 = 0;
int Bit9 = 0;
int Bit10 = 0;
int Bit11 = 0;
int Bit12 = 0;
int Bit13 = 0;
int Bit14 = 0;
int Bit15 = 0;
int Bit16 = 0;
int Bit17 = 0;
int Bit18 = 0;
int Bit19 = 0;
int Bit20 = 0;
int LZW_size = 0;

void find_pattern(char* word,char c,int* positons,int place){
    char buffer[strlen(word)+2];
    int iterator=0;
    strcpy(buffer,word);
    buffer[strlen(word)]=c;
    buffer[strlen(word)+1]='\0';
    positons[0]=-1;
    positons[1]=-1;
    if(place==0){
        iterator = LZW_last_placed;
    }else{
        iterator = iLZW_last_placed;
    }
    for(int i=0;i<iterator;i++){
        if(strcmp(encoding_table[i],word)==0){
            positons[0]=i;
        }
        if(strcmp(encoding_table[i],buffer)==0){
            positons[1]=i;
            return;
        }
    }
}

void print_bits(int word_size){
    printf("There are %d dictionary entries, each with size %d\n",last_placed,word_size);
    int data = last_placed * word_size;
    int metadata = ((Bit20*20)+(Bit19*19)+(Bit18*18)+(Bit17*17)+(Bit16*16)+(Bit15*15)+(Bit14*14)+(Bit13*13)+(Bit12*12)+(Bit11*11)+(Bit10*10)+(Bit9*9)+(Bit8*8)+(Bit7*7)+(Bit6*6)+(Bit5*5)+(Bit4*4)+(Bit3*3)+(Bit2*2)+(Bit1*1))/8;
    printf("[%d|%d]Size is %d Bytes\n",data,metadata,data + metadata);
}

void print_bits_LZW(){
    printf("There are [%d|%d] dictionary entries\n",LZW_last_placed,iLZW_last_placed);
    int data = 0;
    for(int i=0;i<LZW_last_placed;i++){
        data += (int) strlen(encoding_table[i]);
    }
    int metadata = ((Bit20*20)+(Bit19*19)+(Bit18*18)+(Bit17*17)+(Bit16*16)+(Bit15*15)+(Bit14*14)+(Bit13*13)+(Bit12*12)+(Bit11*11)+(Bit10*10)+(Bit9*9)+(Bit8*8)+(Bit7*7)+(Bit6*6)+(Bit5*5)+(Bit4*4)+(Bit3*3)+(Bit2*2)+(Bit1*1))/8;
    printf("[%d|%d]Size is %d Bytes\n",data,metadata,data + metadata);
}

void decide_bit(int position){
    if(position<2){
        Bit1++;
    }else if(position<4){
        Bit2++;
    }else if(position<8){
        Bit3++;
    }else if(position<16){
        Bit4++;
    }else if(position<32){
        Bit5++;
    }else if(position<64){
        Bit6++;
    }else if(position<128){
        Bit7++;
    }else if(position<256){
        Bit8++;
    }else if(position<512){
        Bit9++;
    }else if(position<1024){
        Bit10++;
    }else if(position<2048){
        Bit11++;
    }else if(position<4096){
        Bit12++;
    }else if(position<8192){
        Bit13++;
    }else if(position<16384){
        Bit14++;
    }else if(position<32768){
        Bit15++;
    }else if(position<65536){
        Bit16++;
    }else if(position<131072){
        Bit17++;
    }else if(position<262144){
        Bit18++;
    }else if(position<524288){
        Bit19++;
    }else{
        Bit20++;
    }
}

int find_pattern4(char* word){
    int positons=-1;
    for(int i=0;i<last_placed;i++){
        if(strcmp(encoding_table[i],word)==0){
            //printf("[%d]Igual: %s -> %s\n",i,encoding_table[i],word);
            positons=i;
            return positons;
        }
    }
    return positons;
}

int find_decoded(char* word){
    for(int i=0;i<LZW_last_placed;i++){
        if(strcmp(encoding_table[i],word)==0){
            return i;
        }
    }
    return -1;
}

void savein_table(char** table,char* pattern,int size){
    if(last_placed<INT_SIZE){
        table[last_placed]=(char*)malloc(sizeof(char)*(size+1));
        strcpy(table[last_placed],pattern);
        table[last_placed][size]='\0';
        last_placed++;
    }
}

void LZW_savein_table(char** table,char* pattern,int size,int place){
    if(place==0){
        if(LZW_last_placed<INT_SIZE){
            table[LZW_last_placed]=(char*)malloc(sizeof(char)*(size+1));
            strcpy(table[LZW_last_placed],pattern);
            table[LZW_last_placed][size]='\0';
            LZW_last_placed++;
        }
    }else{
        if(iLZW_last_placed<INT_SIZE){
            table[iLZW_last_placed]=(char*)malloc(sizeof(char)*(size+1));
            strcpy(table[iLZW_last_placed],pattern);
            table[iLZW_last_placed][size]='\0';
            iLZW_last_placed++;
        }
    }
    
}

void setup_table(char** table,int place){
    for(int i=0;i<256;i++){
        char buffer[1];
        char holder=(char) i;
        buffer[0]=holder;
        LZW_savein_table(table,buffer,1,place);
    }
}

void LZW(FILE* fp,FILE* compressed){
    char word[WORD_SIZE],c;
    int size=1,position[2];
    setup_table(encoding_table,0);
    if((word[0] = fgetc(fp)) == EOF){
        printf("Empty file\n");
        return;
    }
    word[1]='\0';
    compressed=fopen("LZWcompressed.txt","w");
    Bit1++;
    while ((c = fgetc(fp)) != EOF)
    {
        find_pattern(word,c,position,0);
        if(position[1]!=-1 && size<WORD_SIZE-1){
            word[size]=c;
            size++;
            word[size]='\0';
        }else{
            fprintf(compressed,"%d ",(int) position[0]);
            decide_bit(position[0]);
            if(last_placed<INT_SIZE){   
                word[size]=c;
                size++;
                word[size]='\0';
                LZW_savein_table(encoding_table,word,strlen(word),0);
            }
            word[0]=c;
            size=1;
            word[size]='\0';
        }
    }
    find_pattern(word,c,position,0);
    fprintf(compressed,"%d ",(int) position[0]);
    decide_bit(position[0]);
    fclose(compressed);
}

void LZW16(FILE* fp,FILE* compressed){
    char word[3],c;
    int size=3,position;
    //setup_table(encoding_table);
    if((fgets(word,3,fp)) == NULL){
        printf("Empty file\n");
        return;
    }
    compressed=fopen("LZW2compressed.txt","w");
    position=find_pattern4(word);
    savein_table(encoding_table,word,strlen(word));
    fprintf(compressed,"%hu ",(unsigned short) 0);
    Bit1++;
    while ((fgets(word,3,fp)) != NULL)
    {
        position=find_pattern4(word);
        if(position==-1 && last_placed<SHORT_SIZE){
            savein_table(encoding_table,word,strlen(word));
            position=last_placed-1;
        }
        decide_bit(position);
        fprintf(compressed,"%hu ",(unsigned short) position);
    }
    fclose(compressed);
}

void LZW32(FILE* fp,FILE* compressed){
    char word[5],c;
    int size=5,position;
    //setup_table(encoding_table);
    if((fgets(word,5,fp)) == NULL){
        printf("Empty file\n");
        return;
    }
    compressed=fopen("LZW4compressed.txt","w");
    position=find_pattern4(word);
    savein_table(encoding_table,word,strlen(word));
    fprintf(compressed,"%u ",(unsigned int) 0);
    Bit1++;
    while ((fgets(word,5,fp)) != NULL)
    {
        position=find_pattern4(word);
        if(position==-1 && last_placed<INT_SIZE){
            savein_table(encoding_table,word,strlen(word));
            position=last_placed-1;
        }
        decide_bit(position);
        fprintf(compressed,"%u ",(unsigned int) position);
    }
    fclose(compressed);
}

void LZW64(FILE* fp,FILE* compressed){
    char word[9],c;
    int size=9,position;
    //setup_table(encoding_table);
    if((fgets(word,9,fp)) == NULL){
        printf("Empty file\n");
        return;
    }
    compressed=fopen("LZW8compressed.txt","w");
    position=find_pattern4(word);
    savein_table(encoding_table,word,strlen(word));
    fprintf(compressed,"%d ",(int) 0);
    Bit1++;
    while ((fgets(word,9,fp)) != NULL)
    {
        position=find_pattern4(word);
        if(position==-1 && last_placed<INT_SIZE){
            savein_table(encoding_table,word,strlen(word));
            position=last_placed-1;
        }
        decide_bit(position);
        fprintf(compressed,"%d ",(int) position);
    }
    fclose(compressed);
}

void iLZW(FILE* compressed,FILE* decompressed){
    char word[WORD_SIZE],output[WORD_SIZE],c;
    int old,new;
    int s_old,s_new;
    int position[2];
    setup_table(decoding_table,1);
    compressed=fopen("LZWcompressed.txt","r");
    decompressed=fopen("LZWdecompressed.txt","w");
    rewind(compressed);
    fscanf(compressed,"%d ",&s_old);
    old=(int) s_old;
    fprintf(decompressed,"%s",encoding_table[old]);
    while ((fscanf(compressed,"%d ",&s_new)) != EOF){
        new=(int) s_new;
        if((find_decoded(encoding_table[new]))==-1){
            strcpy(output,encoding_table[old]);
            strncat(output, &c, 1);
        }else{
            strcpy(output,encoding_table[new]);
        }
        fprintf(decompressed,"%s",output);
        c = output[0];
        strcpy(word,encoding_table[old]);
        strncat(word, &c, 1);
        LZW_savein_table(decoding_table,word,strlen(word),1);
        old=new;
    }
    fclose(compressed);
    fclose(decompressed);
}

void iLZW16(FILE* compressed,FILE* decompressed){
    char word[WORD_SIZE],output[WORD_SIZE],c;
    int old,new;
    unsigned short s_old,s_new;
    int position[2];
    compressed=fopen("LZW2compressed.txt","r");
    decompressed=fopen("LZW2decompressed.txt","w");
    rewind(compressed);
    fscanf(compressed,"%hu ",&s_old);
    old=(int) s_old;
    fprintf(decompressed,"%s",encoding_table[old]);
    while ((fscanf(compressed,"%hu ",&s_new)) != EOF){
        //printf("S[%hu]\n",s_new);
        new=(int) s_new;
        //printf("New[%d]\n",new);
        //find_decoded(encoding_table[new]);
        strcpy(output,encoding_table[new]);
        fprintf(decompressed,"%s",output);
        //strcpy(word,encoding_table[old]);
        //savein_table(decoding_table,word,strlen(word));
        old=new;
    }
    fclose(compressed);
    fclose(decompressed);
    print_bits(2);
}

void iLZW32(FILE* compressed,FILE* decompressed){
    char word[WORD_SIZE],output[WORD_SIZE],c;
    int old,new;
    int s_old,s_new;
    int position[2];
    compressed=fopen("LZW4compressed.txt","r");
    decompressed=fopen("LZW4decompressed.txt","w");
    rewind(compressed);
    fscanf(compressed,"%u ",&s_old);
    old=(int) s_old;
    fprintf(decompressed,"%s",encoding_table[old]);
    while ((fscanf(compressed,"%u ",&s_new)) != EOF){
        //printf("S[%hu]\n",s_new);
        new=(int) s_new;
        //printf("New[%d]\n",new);
        //find_decoded(encoding_table[new]);
        strcpy(output,encoding_table[new]);
        fprintf(decompressed,"%s",output);
        //strcpy(word,encoding_table[old]);
        //savein_table(decoding_table,word,strlen(word));
        old=new;
    }
    fclose(compressed);
    fclose(decompressed);
    print_bits(4);
}

void iLZW64(FILE* compressed,FILE* decompressed){
    char word[WORD_SIZE],output[WORD_SIZE],c;
    int old,new;
    int s_old,s_new;
    int position[2];
    compressed=fopen("LZW8compressed.txt","r");
    decompressed=fopen("LZW8decompressed.txt","w");
    rewind(compressed);
    fscanf(compressed,"%d ",&s_old);
    old=(int) s_old;
    fprintf(decompressed,"%s",encoding_table[old]);
    while ((fscanf(compressed,"%d ",&s_new)) != EOF){
        //printf("S[%hu]\n",s_new);
        new=(int) s_new;
        //printf("New[%d]\n",new);
        //find_decoded(encoding_table[new]);
        strcpy(output,encoding_table[new]);
        fprintf(decompressed,"%s",output);
        //strcpy(word,encoding_table[old]);
        //savein_table(decoding_table,word,strlen(word));
        old=new;
    }
    fclose(compressed);
    fclose(decompressed);
    print_bits(8);
}