#include <stdio.h>

int main()
{
  int a = 10;
  int b = 5;
  int c = 1;
  puts("a = 10, b = 5, c = 1");
  printf("NOT          : ~a     = %d\n", ~a);
  printf("OR           : a | b  = %d\n", a | b);
  printf("AND          : a & b  = %d\n", a & b);
  printf("XOR          : a ^ b  = %d\n", a ^ b);
  printf("bit shift << : a << 1 = %d\n", a << 1);
  printf("bit shift >> : a >> 1 = %d\n", a >> 1);
  return 0;
}
