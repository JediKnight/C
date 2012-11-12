#include <stdio.h>
#include <errno.h>

#define IMG_Y 640
#define IMG_X 640
#define COLOR 3
enum { R =0, G, B };

int main()
{
  unsigned char image[IMG_Y][IMG_X][COLOR];
  int x, y, c = 0xff;
  FILE *fp;

  if((fp = fopen("sample.pnm", "wb")) == NULL)
    {
      perror("fopen");
      return -1;
    }

  fprintf(fp, "P6\n%d %d\n%d\n", IMG_X, IMG_Y, 0xff);

  /* 右上 */
  for(y = 0; y < IMG_Y / 2; y++)
    {
      for(x = 0; x < IMG_X / 2; x++)
	{
	  image[y][x][R] = 0xff;
	  image[y][x][G] = 0;
	  image[y][x][B] = 0;
	}
    }

  /* 右下 */
  for(y = IMG_Y / 2; y < IMG_Y; y++)
    {
      for(x = 0; x < IMG_X; x++)
	{
	  image[y][x][R] = 0;
	  image[y][x][G] = 0;
	  image[y][x][B] = 0xff;
	}
    }

  for(y = 0; y < IMG_Y / 2; y++)
    {
      for(x = IMG_X / 2; x < IMG_X; x++)
	{
	  image[y][x][R] = 0xff;
	  image[y][x][G] = 0xff;
	  image[y][x][B] = 0xff;
	}
    }

	  
  /* 左下 */
  for(y = IMG_Y / 2; y < IMG_Y; y++)
    {
      for(x = IMG_X / 2; x < IMG_X; x++)
	{
	  image[y][x][R] = 0;
	  image[y][x][G] = 0xff;
	  image[y][x][B] = 0;
	}
    }

  fwrite(image, sizeof(char), IMG_Y * IMG_X * COLOR, fp);
  fclose(fp);
  return 0;
}
