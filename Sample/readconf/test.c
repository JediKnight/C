#include <stdio.h>

#define testfunc(type) sizeof(type)

int main(int argc, char **argv)
{
  switch(testfunc(unsigned long long))
    {
    case 1:
      puts("char");
      break;

    case 2:
      puts("short");
      break;

    case 4:
      puts("int");
      break;

    case 8:
      puts("const char * or char * or double");
      break;

    default:
      printf("%ld", testfunc(unsigned long long));
      break;
    }

  return 0;
}
