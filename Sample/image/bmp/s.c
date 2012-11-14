#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bitmap.h"

int main(int argc, char **argv)
{
  FILE *fp;
  long var;
  int i;
  BITMAPFILEHEADER bfHeader;
  BITMAPINFOHEADER biHeader;
  RGBTRIPLE rgbTriple;

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

  if(fileHeader(&bfHeader, fp) == -1)
    return -1;

  dispFileHeader(&bfHeader);

  if(infoHeader(&biHeader, fp) == -1)
    return -1;

  dispInfoHeader(&biHeader);

  fclose(fp);

  return 0;
}
