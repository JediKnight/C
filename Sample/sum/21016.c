#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int hex(char *ret, int decimal, int num)
{
  int x, y, i;
  x = num;
  for(i = 0; x > 0; i++)
    {
      if((y = x % decimal) >= 10)
	sprintf(ret, "%c%s", ('a' + (y - 10)), ret);
      else
	sprintf(ret, "%d%s", y, ret);
      x /= decimal;
      printf("%d %s\n", y, ret);
    }

  return 0;
}

int main(int argc, char **argv)
{
  char ret[100];
  if(argc != 3)
    {
      fprintf(stdout, "%s [decimal] [value]\n", argv[0]);
      return -1;
    }

  if(atoi(argv[1]) <= 1 || atoi(argv[1]) > 36)
    {
      fprintf(stderr, "hanigai\n");
      return -1;
    }

  memset(ret, '\0', sizeof(argv) / sizeof(argv[0]));
  hex(ret, atoi(argv[1]), atoi(argv[2]));
  printf("%s\n", ret);

  return 0;
}
