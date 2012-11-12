#include <stdio.h>
#include <string.h>
#include "image.h"

int clearImagefd(void *imgfd, size_t size)
{
  if(memset(imgfd, '\0', size) == NULL)
    return -1;
  return 0;
}

int iread(void *imgfd, size_t size)
{
  FILE *fp;
  if((fp = fopen("sample.pnm", "rb")) == NULL)
    {
      perror("fopen");
      return -1;
    }

  fscanf(fp, "P6\n640 640\n255\n");
  fread(imgfd, sizeof(char), size, fp);
  fclose(fp);

  return 0;
}

int iwrite(void *imgfd)
{
}
