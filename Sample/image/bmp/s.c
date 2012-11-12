#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BFTYPE 2
#define BFSIZE 4
#define BFRESERVED1 2
#define BFRESERVED2 2
#define BFOFFBITS 4

typedef struct tagBITMAPFILEHEADER {
  unsigned short bfType;
  unsigned long  bfSize;
  unsigned short bfReserved1;
  unsigned short bfReserved2;
  unsigned long  bfOffBits;
} BITMAPFILEHEADER;

typedef struct tagBITMAPCOREHEADER{
  unsigned long  bcSize;
  short          bcWidth;
  short          bcHeight;
  unsigned short bcPlanes;
  unsigned short bcBitCount;
} BITMAPCOREHEADER;

typedef struct tagBITMAPINFOHEADER{
  unsigned long  biSize;
  long           biWidth;
  long           biHeight;
  unsigned short biPlanes;
  unsigned short biBitCount;
  unsigned long  biCompression;
  unsigned long  biSizeImage;
  long           biXPixPerMeter;
  long           biYPixPerMeter;
  unsigned long  biClrUsed;
  unsigned long  biClrImporant;
} BITMAPINFOHEADER;

int dispFileHeader(BITMAPFILEHEADER *bfHeader)
{
  fprintf(stdout,
	  "[BITMAPFILEHEADER]\n"
	  "bfType      : %d\n"
	  "bfSize      : %ld\n"
	  "bfReserved1 : %d\n"
	  "bfReserved2 : %d\n"
	  "bfOffBits   : %ld\n"
	  "\n",
	  bfHeader->bfType,
	  bfHeader->bfSize,
	  bfHeader->bfReserved1,
	  bfHeader->bfReserved2,
	  bfHeader->bfOffBits);
  return 0;
}

int dispCoreHeader(BITMAPCOREHEADER *bcHeader)
{
  return 0;
}

int dispInfoHeader(BITMAPINFOHEADER *biHeader)
{
  fprintf(stdout,
	  "[BITMAPINFOHEADER]\n"
	  "biSize         : %ld\n"
	  "biWidth        : %ld\n"
	  "biHeight       : %ld\n"
	  "biPlanes       : %d\n"
	  "biBitCount     : %d\n"
	  "biCompression  : %ld\n"
	  "biSizeImage    : %ld\n"
	  "biXPixPerMeter : %ld\n"
	  "biYPixPerMeter : %ld\n"
	  "biClrUsed      : %ld\n"
	  "biClrImporant  : %ld\n"
	  "\n",
	  biHeader->biSize,
	  biHeader->biWidth,
	  biHeader->biHeight,
	  biHeader->biPlanes,
	  biHeader->biBitCount,
	  biHeader->biCompression,
	  biHeader->biSizeImage,
	  biHeader->biXPixPerMeter,
	  biHeader->biYPixPerMeter,
	  biHeader->biClrUsed,
	  biHeader->biClrImporant);
  return 0;
}

int main(int argc, char **argv)
{
  FILE *fp;
  long var;
  int i;
  BITMAPFILEHEADER bfHeader;
  BITMAPINFOHEADER biHeader;

  if(argc != 2)
    {
      fprintf(stdout, "owata\n");
      exit(EXIT_FAILURE);
    }

  if((fp = fopen(argv[1], "rb")) == NULL)
    {
      perror("fopen");
      exit(EXIT_FAILURE);
    }

  fread(&bfHeader.bfType, BFTYPE, 1, fp);
  fread(&bfHeader.bfSize, BFSIZE, 1, fp);
  fread(&bfHeader.bfReserved1, BFRESERVED1, 1, fp);
  fread(&bfHeader.bfReserved2, BFRESERVED2, 1, fp);
  fread(&bfHeader.bfOffBits, BFOFFBITS, 1, fp);
  dispFileHeader(&bfHeader);

  fread(&biHeader.biSize, 4, 1, fp);
  fread(&biHeader.biWidth, 4, 1, fp);
  fread(&biHeader.biHeight, 4, 1, fp);
  fread(&biHeader.biPlanes, 2, 1, fp);
  fread(&biHeader.biBitCount, 2, 1, fp);
  fread(&biHeader.biCompression, 4, 1, fp);
  fread(&biHeader.biSizeImage, 4, 1, fp);
  fread(&biHeader.biXPixPerMeter, 4, 1, fp);
  fread(&biHeader.biYPixPerMeter, 4, 1, fp);
  fread(&biHeader.biClrUsed, 4, 1, fp);
  fread(&biHeader.biClrImporant, 4, 1, fp);
  dispInfoHeader(&biHeader);

  fclose(fp);

  return 0;
}
