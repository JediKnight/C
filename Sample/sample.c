#include <stdio.h>
#include <errno.h>
#include "network.h"

int main()
{
  int server_sockfd, client_sockfd;
  if((server_sockfd = serverSocket("127.0.0.1", 19860)) != -1)
    {
      perror("serverSocket");
      return -1;
    }

  return 0;
}
