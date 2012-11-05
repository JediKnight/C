#include <stdio.h>
#include <errno.h>

#define IMG_Y 640
#define IMG_X 640
#define COLOR 3
enum { R =0, G, B };
#define pos(y, x) (y * IMG_X) + x

int flip()
{
  return 0;
}

int main()
{
#ifndef _1ARRAY
  unsigned char image[IMG_Y][IMG_X][COLOR];
#else
  unsigned char imgae[IMG_Y * IMG_X * COLOR];
#endif

  int x, y, mx;
  FILE *fp;

  if((fp = fopen("sample.pnm", "rb")) == NULL)
    { perror("fopen"); return -1; }
  fscanf(fp, "P6\n640 640\n255\n");
  fread(image, sizeof(char), IMG_Y * IMG_X * COLOR, fp);
  fclose(fp);

  for(y = 0; y < IMG_Y; y++)
    {
#ifndef _1ARRAY
      for(x = 0, mx = IMG_X; x < IMG_X / 2; x++, mx--)
	{
	  char tmpR, tmpG, tmpB;
	  tmpR = image[y][x][R];
	  tmpG = image[y][x][G];
	  tmpB = image[y][x][B];

	  image[y][x][R] = image[y][mx][R];
	  image[y][x][G] = image[y][mx][G];
	  image[y][x][B] = image[y][mx][B];

	  image[y][mx][R] = tmpR;
	  image[y][mx][G] = tmpG;
	  image[y][mx][B] = tmpB;
	}
#else
      for(x = 0, mx = IMG_X; x < (IMG_X * COLOR) / 2; x++, mx--)
	{
	  char tmpR, tmpG, tmpB;
	  tmpR = image[(y * IMG_X) + x + R];
	  tmpG = image[(y * IMG_X) + x + G];
	  tmpB = image[(y * IMG_X) + x + B];

	  image[(y * IMG_X) + x + R] = image[(y * IMG_X) + mx + R];
	  image[(y * IMG_X) + x + G] = image[(y * IMG_X) + mx + G];
	  image[(y * IMG_X) + x + B] = image[(y * IMG_X) + mx + B];

	  image[(y * IMG_X) + mx + R] = tmpR;
	  image[(y * IMG_X) + mx + G] = tmpG;
	  image[(y * IMG_X) + mc + B] = tmpB;
	}
#endif
    }

  if((fp = fopen("sample_rev.pnm", "wb")) == NULL)
    { perror("fopen"); return -1; }
  fprintf(fp, "P6\n%d %d\n%d\n", IMG_X, IMG_Y, 0xff);
  fwrite(image, sizeof(char), IMG_Y * IMG_X * COLOR, fp);
  fclose(fp);
  return 0;
}
