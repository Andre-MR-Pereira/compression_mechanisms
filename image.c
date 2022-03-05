#include "image.h"

void create_image(FILE* f,int w,int h,int** red,int** blue,int** green,char* name){
    unsigned char *img = NULL;
    int filesize = 54 + 3*w*h;  //w is your image width, h is image height, both int
    int x,y,r,g,b;

    img = (unsigned char *)malloc(3*w*h);
    memset(img,0,3*w*h);

    for(int i=0; i<w; i++)
    {
        for(int j=0; j<h; j++)
        {
            x=i; y=(h-1)-j;
            r = red[i][j]*255;
            g = green[i][j]*255;
            b = blue[i][j]*255;
            //printf("%d & %d & %d\n",r,g,b);
            if (r > 255) r=255;
            if (g > 255) g=255;
            if (b > 255) b=255;
            img[(x+y*w)*3+2] = (unsigned char)(r);
            img[(x+y*w)*3+1] = (unsigned char)(g);
            img[(x+y*w)*3+0] = (unsigned char)(b);
        }
    }

    unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
    unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0};
    unsigned char bmppad[3] = {0,0,0};

    bmpfileheader[ 2] = (unsigned char)(filesize    );
    bmpfileheader[ 3] = (unsigned char)(filesize>> 8);
    bmpfileheader[ 4] = (unsigned char)(filesize>>16);
    bmpfileheader[ 5] = (unsigned char)(filesize>>24);

    bmpinfoheader[ 4] = (unsigned char)(       w    );
    bmpinfoheader[ 5] = (unsigned char)(       w>> 8);
    bmpinfoheader[ 6] = (unsigned char)(       w>>16);
    bmpinfoheader[ 7] = (unsigned char)(       w>>24);
    bmpinfoheader[ 8] = (unsigned char)(       h    );
    bmpinfoheader[ 9] = (unsigned char)(       h>> 8);
    bmpinfoheader[10] = (unsigned char)(       h>>16);
    bmpinfoheader[11] = (unsigned char)(       h>>24);

    f = fopen(name,"wb");
    fwrite(bmpfileheader,1,14,f);
    fwrite(bmpinfoheader,1,40,f);
    for(int i=0; i<h; i++)
    {
        fwrite(img+(w*(h-i-1)*3),3,w,f);
        fwrite(bmppad,1,(4-(w*3)%4)%4,f);
    }

    free(img);
    fclose(f);
}

struct bmp_header* read_bmp_header(FILE* fp){
    BITMAPINFOHEADER bitmapInfoHeader;
    BITMAPFILEHEADER bitmapFileHeader;  //our bitmap file header
    struct bmp_header* header;
    header=(struct bmp_header*)malloc(sizeof(struct bmp_header));

    //read the bitmap file header
    fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER),1,fp);

    header->type=bitmapFileHeader.bfType;
    header->size=bitmapFileHeader.bfSize;
    header->reserved1=bitmapFileHeader.bfReserved1;
    header->reserved2=bitmapFileHeader.bfReserved2;
    header->offset=bitmapFileHeader.bfOffBits;

    //read the bitmap info header
    fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER),1,fp);

    header->dib_size=bitmapInfoHeader.biSize;
    header->width=bitmapInfoHeader.biWidth;
    header->height=bitmapInfoHeader.biHeight;
    header->planes=bitmapInfoHeader.biPlanes;
    header->bpp=bitmapInfoHeader.biBitCount;
    header->compression=bitmapInfoHeader.biCompression;
    header->image_size=bitmapInfoHeader.biSizeImage;
    header->x_ppm=bitmapInfoHeader.biXPelsPerMeter;
    header->y_ppm=bitmapInfoHeader.biYPelsPerMeter;
    header->num_colors=bitmapInfoHeader.biClrUsed; 
    header->important_colors=bitmapInfoHeader.biClrImportant; 

    return header;
}

struct pixel* read_data(FILE* stream, const struct bmp_header* header){
  if(stream == NULL || header == NULL){
    return 0;
  }
  printf("Sizes: %d %d\n",header->width,header->height);
  // w == 1 && p == 1;   w == 2 && p == 2;   w == 3 && p == 3;   w == 4 && p == 0  
  int padding = header->width % (4-((3*header->width)%4))%4;
  printf("Padding %d\n",padding);
  int num_of_pixels = header->width * header->height;
  printf("Number pixels %d\n",num_of_pixels);
  struct pixel* Pixel[num_of_pixels];

  fseek(stream, header->offset, SEEK_SET); //move 54B (header size)
  printf("INITS\n");
  int index_p = 0;
  for(int i = 0; i < header->height; i++){
    for(int j = 0; j < header->width; j++){
      Pixel[index_p] = malloc(sizeof(struct pixel));
      fread(&(Pixel[index_p]->blue), 1, 1, stream); 
      fread(&(Pixel[index_p]->green), 1, 1, stream); 
      fread(&(Pixel[index_p]->red), 1, 1, stream);
      printf("Pixel is %d %d %d\n",Pixel[index_p]->blue,Pixel[index_p]->green,Pixel[index_p]->red); 
      index_p++;
    }
    fseek(stream, padding, SEEK_CUR);  //padding at the end of row
  }
  return *Pixel;
}