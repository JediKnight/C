#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
  unsigned short biWidth;
  unsigned short biHeight;
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
	  "bfType          : %d\n"
	  "bfSize          : %ld\n"
	  "bfReserved1     : %d\n"
	  "bfReserved2     : %d\n"
	  "bfOffBits       : %ld\n"
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
  fprintf(stdout,
	  "[BITMAPCOREHEADER]\n"
	  "bcSize          : %ld\n"
	  "bcWidth         : %d\n"
	  "bcHeight        : %d\n"
	  "bcPlanes        : %d\n"
	  "bcBitCount      : %d\n"
	  "\n",
	  bcHeader->bcSize,
	  bcHeader->bcWidth,
	  bcHeader->bcHeight,
	  bcHeader->bcPlanes,
	  bcHeader->bcBitCount);
  return 0;
}

int dispInfoHeader(BITMAPINFOHEADER *biHeader)
{
  fprintf(stdout,
	  "[BITMAPINFOHEADER]\n"
	  "biSize          : %ld\n"
	  "biWidth         : %ld\n"
	  "biHeight        : %ld\n"
	  "biPlanes        : %d\n"
	  "biBitCount      : %d\n"
	  "biCompression   : %ld\n"
	  "biSizeImage     : %ld\n"
	  "biXPixPerMeter  : %ld\n"
	  "biYPixPerMeter  : %ld\n"
	  "biClrUsed       : %ld\n"
	  "biClrImporant   : %ld\n"
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
  BITMAPCOREHEADER bcHeader;
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

  fread(&bfHeader.bfType, 2, 1, fp);
  fread(&bfHeader.bfSize, 4, 1, fp);
  fread(&bfHeader.bfReserved1, 2, 1, fp);
  fread(&bfHeader.bfReserved2, 2, 1, fp);
  fread(&bfHeader.bfOffBits, 4, 1, fp);
  dispFileHeader(&bfHeader);

  /* fread(&bcHeader.bcSize, 4, 1, fp); */
  /* fread(&bcHeader.bcWidth, 4, 1, fp); */
  /* fread(&bcHeader.bcHeight, 4, 1, fp); */
  /* fread(&bcHeader.bcPlanes, 2, 1, fp); */
  /* fread(&bcHeader.bcBitCount, 2, 1, fp); */
  /* dispCoreHeader(&bcHeader); */

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
