#include <curses.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>

int main(void)
{
  int fifodes;
  char buffer[256];

  mkfifo("my_fifo",0777);
  fifodes = open("my_fifo",O_RDONLY);
  printf("file open");
  read(fifodes,buffer,256);
  /* if(initscr() == NULL) */
  /*   { */
  /*     perror("initscr()"); */
  /*     exit(EXIT_FAILURE); */
  /*   } */
  /* printw("Message: %s",buffer); */
  /* getch(); */
  /* endwin(); */

  close(fifodes);

  return(0);
}
