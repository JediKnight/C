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
