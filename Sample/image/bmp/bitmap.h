#ifndef _BITMAP_H
#define _BITMAP_H

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

typedef struct tagRGBTRIPLE{
  unsigned char rgbBlue;
  unsigned char rgbGreen;
  unsigned char rgbRed;
} RGBTRIPLE;

enum {
  BI_RGB = 0,
  BI_RLE8,
  BI_RLE4,
  BI_BITFIELDS,
  BI_JPEG,
  BI_PNG
};

int fileHeader(BITMAPFILEHEADER *, FILE *);
int coreHeader(BITMAPCOREHEADER *, FILE *);
int infoHeader(BITMAPINFOHEADER *, FILE *);

#endif	/* _BITMAP_H */
