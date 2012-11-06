#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "network.h"

int serverSocket(char *host, int port)
{
  struct sockaddr_in address;
  int sockfd, opt, len;

  if((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
    {
      perror("socket");
      return -1;
    }

  bzero((char *)&address, sizeof(address));
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = inet_addr(host);
  address.sin_port = htons(port);
  len = sizeof(address);

  opt = 1;
  if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, (socklen_t)sizeof(opt)) == -1)
    {
      perror("setsockopt");
      return -1;
    }

  if(bind(sockfd, (const struct sockaddr *)&address, (socklen_t)len) == -1)
    {
      perror("bind");
      return -1;
    }

  return sockfd;
}

int clientSocket(int ssockfd)
{
  struct sockaddr_in address;
  int sockfd, len;

  if((sockfd = accept(ssockfd, (struct sockaddr *)&address, (socklen_t *)&len)) == -1)
    {
      perror("accept");
      return -1;
    }

  return sockfd;
}

int closeSocket(int sockfd)
{
  if(close(sockfd) == -1)
    {
      return -1;
    }

  return 0;
}

int recvdata(int sockfd, char *buf, int bufsiz)
{
  char *p;

  memset(buf, '\0', bufsiz);

  if(recv(sockfd, buf, bufsiz, 0) == -1)
    {
      perror("recv");
      return -1;
    }

  if((p = strchr(buf, '\n') -1) != NULL)
    {
      *p = '\0';
    }

  return 0;
}
