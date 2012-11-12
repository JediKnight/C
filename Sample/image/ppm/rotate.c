#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "image.h"
#include "matrix.h"

#define IMG_Y 640
#define IMG_X 640
#define COLOR 3
enum { R =0, G, B };
#define pos(y, x) (y * IMG_X) + x
#define en(pi) (pi % 360)

int initImageBlack(void *image, size_t size)
{
  if(memset(image, 0x01, size) == NULL)
    return -1;
  return 0;
}

int main(int argc, char **argv)
{
  CoordinateData cdata;
  unsigned char img[IMG_Y][IMG_X][COLOR], new[IMG_Y][IMG_X][COLOR];
  int x, y;
  float xy[4];
  FILE *fp;

  if(argc < 2)
    {
      perror("no argument");
      exit(EXIT_FAILURE);
    }

  cdata.px = 0.0f;
  cdata.py = 0.0f;
  cdata.cx = IMG_X / 2.0f;
  cdata.cy = IMG_Y / 2.0f;
  cdata.sx = 2.0f;
  cdata.sy = 2.0f;
  cdata.mx = 100.0f;
  cdata.my = 0.0f;
  cdata.angle = (float)toRadian(atoi(argv[1]));

  //  memset(img, 0x01, sizeof(img));
  //  memset(new, 0x01, sizeof(new));
  if(initImageBlack(img, IMG_X * IMG_Y * COLOR) == -1 || initImageBlack(new, IMG_X * IMG_Y * COLOR) == -1)
    {
      fprintf(stdout, "initImageBlack");
      exit(EXIT_FAILURE);
    }

  /* if((fp = fopen("sample.pnm", "rb")) == NULL) */
  /*   { perror("fopen"); return -1; } */
  /* fscanf(fp, "P6\n640 640\n255\n"); */
  /* fread(img, sizeof(char), IMG_Y * IMG_X * COLOR, fp); */
  /* fclose(fp); */

  iread(img, IMG_X * IMG_Y * COLOR);
  
  for(y = 0; y < IMG_Y; y++)
    {
      cdata.py = (float)y;
      for(x = 0; x < IMG_X; x++)
	{
	  cdata.px = (float)x;
	  rotate(&cdata, xy);
	  //scale(&cdata, xy);
	  //parallel(&cdata, xy);

	  if(xy[X] >= 0 && xy[Y] >= 0 && xy[X] < IMG_X && xy[Y] < IMG_Y)
	    {
	      new[(int)xy[Y]][(int)xy[X]][R] = img[y][x][R];
	      new[(int)xy[Y]][(int)xy[X]][G] = img[y][x][G];
	      new[(int)xy[Y]][(int)xy[X]][B] = img[y][x][B];
	    }
	}
    }

  if((fp = fopen("sample_rev.pnm", "wb")) == NULL)
    { perror("fopen"); return -1; }
  fprintf(fp, "P6\n%d %d\n%d\n", IMG_X, IMG_Y, 0xff);
  fwrite(new, sizeof(char), IMG_Y * IMG_X * COLOR, fp);
  fclose(fp);
  return 0;
}
