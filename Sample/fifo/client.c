#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
  int fifodes ;
  char buffer[256];

  /*  FIFO ファイルをオープンにする */
  fifodes = open("my_fifo",O_WRONLY);
  printf("file open");
  fgets(buffer,256,stdin);

  /* データを送信する */
  write(fifodes,buffer,strlen(buffer)+1);

  close(fifodes);

  return(0);
}
