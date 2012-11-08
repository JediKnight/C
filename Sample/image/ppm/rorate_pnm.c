#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "matrix.h"

#define IMG_Y 640
#define IMG_X 640
#define COLOR 3
enum { R =0, G, B };
#define pos(y, x) (y * IMG_X) + x

int flip()
{
  return 0;
}

int main(int argc, char **argv)
{
  unsigned char img[IMG_Y][IMG_X][COLOR], new[IMG_Y][IMG_X][COLOR];
  int x, y;
  FILE *fp;

  printf("%d", argc);
  if(argc < 1) exit;

  memset(img, 0x01, sizeof(img));
  memset(new, 0x01, sizeof(new));

  if((fp = fopen("sample.pnm", "rb")) == NULL)
    { perror("fopen"); return -1; }
  fscanf(fp, "P6\n640 640\n255\n");
  fread(img, sizeof(char), IMG_Y * IMG_X * COLOR, fp);
  fclose(fp);

  for(y = 0; y < IMG_Y; y++)
    {
      for(x = 0; x < IMG_X; x++)
	{
	  position pos = { .x = (double)x, .y = (double)y, .cx = (double)(IMG_X / 2), .cy = (double)(IMG_Y / 2), .angle = (double)torad(atoi(argv[1])) };
	  rotate(&pos);
	  if(pos.x > 0 && pos.y > 0)
	    {
	      new[(int)pos.y][(int)pos.x][R] = img[y][x][R];
	      new[(int)pos.y][(int)pos.x][G] = img[y][x][G];
	      new[(int)pos.y][(int)pos.x][B] = img[y][x][B];
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
