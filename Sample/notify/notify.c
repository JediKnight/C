#include <stdio.h>		/* perror(), printf() */
#include <stdlib.h>		/* exit() */
#include <sys/wait.h>		/* wait() */
#include <netinet/in.h>		/* IPPROTO_TCP */
#include <sys/socket.h>		/* socket(), bind(), listen(), accept() */
#include <string.h>		/* bzero() */
#include <signal.h>		/* kill() */
#include <unistd.h>		/* close() */
#include <ncurses.h>		/* initscr(), printw(), enwin() */
#include "sc_socket.h"

#undef IPADDR
#define IPADDR "127.0.0.1"
#undef PORTNM
#define PORTNM 19860
#define WNDWIDTH 35

int initCurses()
{
  return 0;
}

int resizeWnd()
{
  return 0;
}

#define LINE 2
void mesgWnd(int sockfd)
{
  WINDOW *wnd, *pad;
  int max_y, max_x, pminrow, pmincol, sminrow, smincol, smaxrow, smaxcol;

  initscr();
  noecho();
  nocbreak();
  start_color();
  use_default_colors();
  init_pair(1, COLOR_WHITE, COLOR_BLUE);
  init_pair(2, COLOR_BLACK, COLOR_WHITE);
  getmaxyx(stdscr, max_y, max_x);

  wnd = newwin(0, 0, max_y, max_x - WNDWIDTH);
  pad = newpad(max_y, WNDWIDTH);

  wbkgd(wnd, COLOR_PAIR(1));
  wbkgd(pad, COLOR_PAIR(2));

  pminrow = pmincol = 0;
  sminrow = 0;
  smincol = max_x;
  smaxrow = max_y;
  smaxcol = max_x + WNDWIDTH;

  wprintw(wnd, "mainwindow> ");
  wrefresh(wnd);
  prefresh(pad, pminrow, pmincol, sminrow, smincol, smaxrow, smaxcol);

  for(;;)
    {
      char buf[BUFSIZ], *p;
      recvdata(sockfd, buf, BUFSIZ);
      if(strcmp(buf, "exit") == 0) break;
      wprintw(pad, "%s\n", buf);
      wrefresh(wnd);
      prefresh(pad, pminrow, pmincol, sminrow, smincol - WNDWIDTH - 1, smaxrow, smaxcol - WNDWIDTH - 1);
      sleep(3);
      prefresh(pad, pminrow, pmincol, sminrow, smincol, smaxrow, smaxcol);
    }
  endwin();
  delwin(wnd);
  delwin(pad);
}

void child()
{
  int server_sockfd, client_sockfd;

  if((server_sockfd = serverSocket(IPADDR, PORTNM)) == -1)
    { fprintf(stderr, "serverSocket"); exit(EXIT_FAILURE); }

  if(listen(server_sockfd, SOMAXCONN) == -1)
    { perror("listen"); closeSocket(server_sockfd); exit(EXIT_FAILURE); }

  if((client_sockfd = clientSocket(server_sockfd)) == -1)
    { fprintf(stderr, "clientSocket"); exit(EXIT_FAILURE); }

  mesgWnd(client_sockfd);

  closeSocket(server_sockfd);
  closeSocket(client_sockfd);

  exit(EXIT_SUCCESS);
}

void parent()
{
  wait(NULL);
  puts("process end");
}

int main()
{
  pid_t pid;
  switch(pid = fork())
    {
    case -1:
      perror("fork");
      exit(EXIT_FAILURE);

    case 0:
      child();
      break;

    default:
      parent();
      break;
    }

  return 0;
}
