#include <stdio.h>
#include "readconf.h"

_config_t = config_t;

int main()
{
  struct config_t {
    struct setting {
      int a;
      int b;
      int c;
    } settings;
  } config_ts;

  char *path = "./test.conf";
  readconf(path);
  return 0;
}
