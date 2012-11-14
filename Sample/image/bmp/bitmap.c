#include <stdio.h>
#include "bitmap.h"

#define ErrorHandling(x)				\
  {							\
    fprintf(stderr, "%s\n", x);				\
    return -1;						\
  }

#define SetTheData(t1, v1, t2, v2)		\
  {						\
    t1 = v1;					\
    t2 += v2;					\
  }

int fileHeader(BITMAPFILEHEADER *bfHeader, FILE *fp)
{
  int count = 0;
  long var_long;
  short var_short;

  if(fread(&var_short, 2, 1, fp) != 1)
    ErrorHandling("fread bfType");

  SetTheData(bfHeader->bfType, var_short, count, 2);

  if(bfHeader->bfType != 0x4d42)
      ErrorHandling("not a bitmap file");

  if(fread(&var_long, 4, 1, fp) != 1)
      ErrorHandling("fread bfSize");

  SetTheData(bfHeader->bfSize, var_long, count, 4);

  if(fread(&var_short, 2, 1, fp) != 1)
      ErrorHandling("fread bfReserved1");

  SetTheData(bfHeader->bfReserved1, var_short, count, 2);

  if(fread(&var_short, 2, 1, fp) != 1)
      ErrorHandling("fread bfReserved2");

  SetTheData(bfHeader->bfReserved2, var_short, count, 2);

  if(fread(&var_long, 4, 1, fp) != 1)
      ErrorHandling("fread bfOffBits");

  SetTheData(bfHeader->bfOffBits, var_long, count, 4);

  return count;
}

int coreHeader(BITMAPCOREHEADER *bcHeader, FILE *fp)
{
  fread(&bcHeader->bcSize, 4, 1, fp);
  fread(&bcHeader->bcWidth, 2, 1, fp);
  fread(&bcHeader->bcHeight, 2, 1, fp);
  fread(&bcHeader->bcPlanes, 2, 1, fp);
  fread(&bcHeader->bcBitCount, 2, 1, fp);
  return 0;
}

int infoHeader(BITMAPINFOHEADER *biHeader, FILE *fp)
{
  int count = 0;
  long var_long;
  short var_short;

  if(fread(&var_long, 4, 1, fp) != 1)
    ErrorHandling("fread biSize");

  SetTheData(biHeader->biSize, var_long, count, 4);

  if(fread(&var_long, 4, 1, fp) != 1)
    ErrorHandling("fread biWidth");

  SetTheData(biHeader->biWidth, var_long, count, 4);

  if(fread(&var_long, 4, 1, fp) != 1)
    ErrorHandling("fread biHeight");

  SetTheData(biHeader->biHeight, var_long, count, 4);

  if(fread(&var_short, 2, 1, fp) != 1)
    ErrorHandling("fread biPlanes");

  SetTheData(biHeader->biPlanes, var_short, count, 2);

  if(fread(&var_short, 2, 1, fp) != 1)
    ErrorHandling("fread biBitCount");

  SetTheData(biHeader->biBitCount, var_short, count, 2);

  if(fread(&var_long, 4, 1, fp) != 1)
    ErrorHandling("fread bitCompression");

  SetTheData(biHeader->biCompression, var_long, count, 4);

  if(fread(&var_long, 4, 1, fp) != 1)
    ErrorHandling("fread biSizeImage");

  SetTheData(biHeader->biSizeImage, var_long, count, 4);
  count += 4;

  if(fread(&var_long, 4, 1, fp) != 1)
    ErrorHandling("fread biXPixPerMeter");

  SetTheData(biHeader->biXPixPerMeter, var_long, count, 4);

  if(fread(&var_long, 4, 1, fp) != 1)
    ErrorHandling("fread biYPixPerMeter");

  SetTheData(biHeader->biYPixPerMeter, var_long, count, 4);

  if(fread(&var_long, 4, 1, fp) != 1)
    ErrorHandling("fread biClrUsed");

  SetTheData(biHeader->biClrUsed, var_long, count, 4);

  if(fread(&var_long, 4, 1, fp) != 1)
    ErrorHandling("fread biClrImporant");

  SetTheData(biHeader->biClrImporant, var_long, count, 4);

  return count;
}

int dispFileHeader(BITMAPFILEHEADER *bfHeader)
{
  fprintf(stdout,
	  "[BITMAPFILEHEADER]\n"
	  "bfType          : %d\n"
	  "bfSize          : %ld [byte]\n"
	  "bfReserved1     : %d\n"
	  "bfReserved2     : %d\n"
	  "bfOffBits       : %ld [byte]\n"
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
	  "biSize          : %ld [byte]\n"
	  "biWidth         : %ld [pixel]\n"
	  "biHeight        : %ld [pixel]\n"
	  "biPlanes        : %d\n"
	  "biBitCount      : %d\n"
	  "biCompression   : %ld\n"
	  "biSizeImage     : %ld [byte]\n"
	  "biXPixPerMeter  : %ld [pixel/meter]\n"
	  "biYPixPerMeter  : %ld [pixel/meter]\n"
	  "biClrUsed       : %ld [color]\n"
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
