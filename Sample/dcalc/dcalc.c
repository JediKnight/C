#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

int usage(char *progname)
{
  return fprintf(stdout, "%s [decimal] [number]\n", progname);
}

int numberOfDigits(int num)
{
  int cnt;
  for(cnt = 0; num > 0; cnt++, num /= 10);
  return cnt;
}

int digitDecimalNumber(int dec, int len)
{
  return (int)pow(dec, len) - 1;
}

int strSearch(char *argv)
{
  int i;
  for(i = 0; argv[i] != '\0'; i++)
    if(isdigit(argv[i]) == 0)
      return -1;
  return 0;
}

char itoa(int num)
{
  char p[2];
  sprintf(p, "%d", num);
  return p[0];
}

int toDecimal(int dec, int num, char *res, int len)
{
  int tmp;
  for(; num > 0; --len)
    {
      tmp = (num % dec);
      if(dec > 10 && tmp >= 10)
	*(res + len - 1) = 'a' + (tmp - 10);
      else
	*(res + len - 1) = itoa(tmp);
      num /= dec;
    }
  return 0;
}

int main(int argc, char **argv)
{
  int decimal, number, len;
  char *result;

  if(argc != 3)
    {
      usage(argv[0]);
      return -1;
    }

  if(strSearch(argv[1]) == -1)
    {
      usage(argv[0]);
      return -1;
    }

  if(strSearch(argv[2]) == -1)
    {
      usage(argv[0]);
      return -1;
    }

  if(atoi(argv[1]) <= 1)
    {
      usage(argv[0]);
      return -1;
    }

  if(atoi(argv[1]) < 2 || atoi(argv[1]) > 36)
    {
      usage(argv[0]);
      return -1;
    }
     
  decimal = atoi(argv[1]);
  number = atoi(argv[2]);
  len = (int)(log(number) / log(decimal)) + 1;
  result = (char *)malloc(sizeof(char) * len);
  memset(result, '\0', len + 1);
  toDecimal(decimal, number, result, len);
  printf("%s\n", result);
  free(result);
  return 0;
}
