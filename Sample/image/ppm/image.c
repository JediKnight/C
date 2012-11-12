#include <stdio.h>
#include <string.h>
#include "image.h"
#include "matrix.h"

int clearImagefd(void *imgfd, size_t size)
{
  if(memset(imgfd, '\0', size) == NULL)
    return -1;
  return 0;
}

int iread(CoordinateData *cdata, void *imgfd, size_t size)
{
  FILE *fp;
  int size_x, size_y, color;
  if((fp = fopen("sample.pnm", "rb")) == NULL)
    {
      perror("fopen");
      return -1;
    }

  fscanf(fp, "P6\n%f %f\n%d\n", &cdata->width, &cdata->height, &cdata->color);
  fread(imgfd, sizeof(char), size, fp);
  fclose(fp);

  return 0;
}

int iwrite(void *imgfd)
{
}
