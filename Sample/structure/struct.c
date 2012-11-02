#include <stdio.h>

struct _test {
  int integer;
  char character;
  char *pchar;
};

int testFunc(struct _test t)
{
  printf("integer:%d, character:%c, pchar:%s\n", t.integer, t.character, t.pchar);
  return 0;
}

int main()
{
  struct _test test;
  test.integer = 10;
  test.character = 'a';
  test.pchar = "test";
  testFunc(test);
  return 0;
}
