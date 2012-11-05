#include <arpa/inet.h>		/* inet_addr() */
#include <errno.h>		/* perror() */
#include <netinet/in.h>		/* htons() */
#include <pthread.h>            /* pthread_create(), pthread_detach() */
#include <stdio.h>		/* fprintf() */
#include <stdlib.h>		/* exit() */
#include <string.h>		/* memset() */
#include <sys/fcntl.h>		/* open() */
#include <sys/select.h>		/* select() */
#include <sys/sem.h>		/* semget() */
#include <sys/socket.h>		/* socket(), bind(), listen(), recv() */
#include <sys/stat.h>		/* mkfifo() */
#include <sys/types.h>		/*  */
#include <time.h>		/* struct timeval */
#include <unistd.h>		/* read(), write(), pipe() */
#include "network.h"


#undef BUFSIZ
#define BUFSIZ 255 * 2

#undef IPADDR
#define IPADDR "127.0.0.1"
#undef PORTNM
#define PORTNM 19860

#define maxfds(a, b) (a > b) ? a : b

enum { R = 0, W };

int fildes[2];
fd_set fds;

void reply(void *fd)
{
  int conn_fd = *((int *)fd), rsize;
  char buf[BUFSIZ];
  struct timeval timeout;

  timeout.tv_sec = 0;
  timeout.tv_usec = 0;

  while(1)
    {
      if(select(maxfds(fildes[R], fildes[W]) +1, NULL, &fds, NULL, &timeout) == -1)
	{ perror("select"); exit(EXIT_FAILURE); };

      if(FD_ISSET(fildes[W], &fds))
	{
	  puts("parent get");
	  read(fildes[R], buf, BUFSIZ);
	  write(conn_fd, buf, BUFSIZ);
	}

      if((rsize = recv(conn_fd, buf, BUFSIZ, 0)) < 0)
	{} //	{ perror("recv()"); exit(EXIT_FAILURE); }
      
      else if(rsize == 0)
	{ break; }
      
      else
	{
	  //	  goodbyeReturn(buf);
	  if(strcmp(buf, "exit") == 0)
	    { break; }
	  printf("sockfd:%d> %s\n", conn_fd, buf);
	  //	  write(fildes[W], buf, rsize);
	  write(conn_fd +1, buf, rsize);
	}
    }
  
  if(close(conn_fd) < 0)
    { perror("close()"); exit(EXIT_FAILURE); }
  
  printf("close %d\n", conn_fd);
}

int serverSocket()
{
  struct sockaddr_in address;
  int sockfd, opt, len;

  if((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    { perror("socket()"); return -1; }

  bzero((char *)&address, sizeof(address));
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = inet_addr(IPADDR);
  address.sin_port = htons(PORTNM);
  len = sizeof(address);

  opt = 1;
  if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, (socklen_t)sizeof(opt)) == -1)
    { perror("setsockopt()"); return -1; }

  if(bind(sockfd, (const struct sockaddr *)&address, (socklen_t)len) == -1)
    { perror("bind()"); return -1; }

  return sockfd;
}

int setBlocking(int *sockfd)
{
  int flags;
  if((flags = fcntl(*sockfd, F_GETFL, 0)) < 0)
    { perror("fcntl() F_GETFL"); return -1; };
  flags = flags & ~(flags & O_NONBLOCK);
  if(fcntl(*sockfd, F_SETFL, flags) < 0)
    { perror("fcntl() F_SETFL"); return -1; };
  return 0;

}

int setNonBlocking(int *sockfd)
{
  int flags;
  if((flags = fcntl(*sockfd, F_GETFL, 0)) < 0)
    { perror("fcntl() F_GETFL"); return -1; };
  if(fcntl(*sockfd, F_SETFL, O_NONBLOCK|flags) < 0)
    { perror("fcntl() F_SETFL"); return -1; };
  return 0;
}

int main(int argc, char **argv)
{
  struct sockaddr_in addr;
  struct timeval timeout;
  pthread_t worker;
  int listen_sockfd, child_sockfd, len;

  FD_ZERO(&fds);
  FD_SET(fildes[R], &fds);
  FD_SET(fildes[W], &fds);

  if (pipe(fildes) < 0) 
    { perror("pipe()"); exit(EXIT_FAILURE); }

  timeout.tv_sec = 0;
  timeout.tv_usec = 0;

/*   if(setBlocking(&listen_sockfd) < 0) */
/*     { fprintf(stderr, "setBlocking()\n"); exit(EXIT_FAILURE); } */

  if((listen_sockfd = serverSocket()) < 0)
    { fprintf(stderr, "serverSocket()\n"); exit(EXIT_FAILURE); }

  if(setNonBlocking(&listen_sockfd) < 0)
    { fprintf(stderr, "setNonBlocking()\n"); exit(EXIT_FAILURE); }

  if(listen(listen_sockfd, SOMAXCONN) < 0)
    { perror("listen()"); exit(EXIT_FAILURE); }

  while(1)
    {
      int n;
      char buf[BUFSIZ];

      if((n = select(maxfds(fildes[R], fildes[W]) +1, NULL, &fds, NULL, &timeout)) < 0)
      	{ perror("select()"); exit(EXIT_FAILURE); }

      if(FD_ISSET(fildes[W], &fds))
      	{
      	  puts("child sync");
      	  memset(buf, '\0', BUFSIZ);
      	  read(fildes[R], buf, BUFSIZ);
      	  write(fildes[W], buf, BUFSIZ);
      	}

      if((child_sockfd = accept(listen_sockfd, (struct sockaddr *)&addr, (socklen_t *)&len)) < 0)
	{} //	{ perror("accept()"); exit(EXIT_FAILURE); }
      
      else
	{
	  printf("accept:%d\n", child_sockfd);
	  if(pthread_create(&worker, NULL, (void *)reply, (void *)&child_sockfd) != 0)
	    { perror("pthread_create()"); exit(EXIT_FAILURE); }
	  
	  if(pthread_detach(worker) < 0)
	    { perror("pthread_detach()"); exit(EXIT_FAILURE); }
	}
      
      usleep(1);
    }

  if(close(listen_sockfd) < 0)
    { perror("close()"); exit(EXIT_FAILURE); }

  return 0;
}
