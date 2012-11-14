#include <stdio.h>
#include "bitmap.h"

#define ErrorHandling(x)				\
  {							\
    fprintf(stderr, "%s\n", x);				\
    return -1;						\
  }

#define SetTheData(t1, v1, t2, v2) t1 = v1; t2 += v2
#define DispValLong(t, v, f) fprintf(stdout, "%-15s : %ld %s\n", t, v, f)
#define DispValShort(t, v, f) fprintf(stdout, "%-15s : %d %s\n", t, v, f)

int fileHeader(BITMAPFILEHEADER *bfHeader, FILE *fp)
{
  int count = 0;
  long var_long;
  short var_short;

  fprintf(stdout, "[BITMAPFILEHEADER]\n");

  if(fread(&var_short, 2, 1, fp) != 1)
    ErrorHandling("fread bfType");

  if(var_short != 0x4d42)
      ErrorHandling("not a bitmap file");

  SetTheData(bfHeader->bfType, var_short, count, 2);
  //  DispValShort("bfType", bfHeader->bfType, "");

  if(fread(&var_long, 4, 1, fp) != 1)
      ErrorHandling("fread bfSize");

  SetTheData(bfHeader->bfSize, var_long, count, 4);
  DispValLong("bfSize", bfHeader->bfSize, "[byte]");

  if(fread(&var_short, 2, 1, fp) != 1)
      ErrorHandling("fread bfReserved1");

  SetTheData(bfHeader->bfReserved1, var_short, count, 2);
  //  DispValShort("bfReserved1", bfHeader->bfReserved1, "");

  if(fread(&var_short, 2, 1, fp) != 1)
      ErrorHandling("fread bfReserved2");

  SetTheData(bfHeader->bfReserved2, var_short, count, 2);
  //  DispValShort("bfReserved2", bfHeader->bfReserved2, "");

  if(fread(&var_long, 4, 1, fp) != 1)
      ErrorHandling("fread bfOffBits");

  SetTheData(bfHeader->bfOffBits, var_long, count, 4);
  DispValLong("bfOffBits", bfHeader->bfOffBits, "[byte]");

  fprintf(stdout, "\n");

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

  fprintf(stdout, "[BITMAPINFOHEADER]\n");

  if(fread(&var_long, 4, 1, fp) != 1)
    ErrorHandling("fread biSize");

  SetTheData(biHeader->biSize, var_long, count, 4);
  DispValLong("biSize", biHeader->biSize, "[byte]");

  if(fread(&var_long, 4, 1, fp) != 1)
    ErrorHandling("fread biWidth");

  SetTheData(biHeader->biWidth, var_long, count, 4);
  DispValLong("biWith", biHeader->biWidth, "[pixel]");

  if(fread(&var_long, 4, 1, fp) != 1)
    ErrorHandling("fread biHeight");

  SetTheData(biHeader->biHeight, var_long, count, 4);
  DispValLong("biHeight", biHeader->biHeight, "[pixel]");

  if(fread(&var_short, 2, 1, fp) != 1)
    ErrorHandling("fread biPlanes");

  SetTheData(biHeader->biPlanes, var_short, count, 2);
  DispValShort("biPlanes", biHeader->biPlanes, "");

  if(fread(&var_short, 2, 1, fp) != 1)
    ErrorHandling("fread biBitCount");

  SetTheData(biHeader->biBitCount, var_short, count, 2);
  DispValShort("biBitCount", biHeader->biBitCount, "");

  if(fread(&var_long, 4, 1, fp) != 1)
    ErrorHandling("fread bitCompression");

  SetTheData(biHeader->biCompression, var_long, count, 4);
  DispValLong("biCompression", biHeader->biCompression, "");

  if(fread(&var_long, 4, 1, fp) != 1)
    ErrorHandling("fread biSizeImage");

  SetTheData(biHeader->biSizeImage, var_long, count, 4);
  DispValLong("biSizeImage", biHeader->biSizeImage, "[byte]");

  if(fread(&var_long, 4, 1, fp) != 1)
    ErrorHandling("fread biXPixPerMeter");

  SetTheData(biHeader->biXPixPerMeter, var_long, count, 4);
  DispValLong("biXPixPerMeter", biHeader->biXPixPerMeter, "[pixel/meter]");

  if(fread(&var_long, 4, 1, fp) != 1)
    ErrorHandling("fread biYPixPerMeter");

  SetTheData(biHeader->biYPixPerMeter, var_long, count, 4);
  DispValLong("biYPixPerMeter", biHeader->biYPixPerMeter, "[pixel/meter]");

  if(fread(&var_long, 4, 1, fp) != 1)
    ErrorHandling("fread biClrUsed");

  SetTheData(biHeader->biClrUsed, var_long, count, 4);
  DispValLong("biClrUsed", biHeader->biClrUsed, "");

  if(fread(&var_long, 4, 1, fp) != 1)
    ErrorHandling("fread biClrImporant");

  SetTheData(biHeader->biClrImporant, var_long, count, 4);
  DispValLong("biClrImporant", biHeader->biClrImporant, "");

  fprintf(stdout, "\n");

  return count;
}
