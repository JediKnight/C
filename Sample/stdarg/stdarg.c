#include <stdio.h>
#include <stdarg.h>
#include <string.h>

void print(const char *format, ...)
{
  const char *p, *tmp = format;
  va_list args;

  va_start(args, format);

  while((p = strchr(tmp, '%')) != NULL)
    {
      ++p;
      switch(*p)
	{
	case 'd':
	  printf("%d", va_arg(args, int));
	  break;

	case 's':
	  printf("%s", va_arg(args, const char *));
	  break;

	default:
	  printf("%s", tmp);
	  break;
	}
      tmp = p;
    }

  va_end(args);
}

int main(void)
{
  print("%dprint%s", 10, "hoge");

  return 0;
}
