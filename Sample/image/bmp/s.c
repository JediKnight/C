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

  fpos_t pos;
  
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

  /* fgetpos(fp, &pos); */
  /* printf("%d\n", (int)pos); */
  if(fileHeader(&bfHeader, fp) == -1)
    {
      return -1;
    }

  /* fgetpos(fp, &pos); */
  /* printf("%d\n", (int)pos); */
  fseek(fp, 14, SEEK_SET);
  if(infoHeader(&biHeader, fp) == -1)
    {
      return -1;
    }

  fclose(fp);

  return 0;
}
