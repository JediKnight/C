#ifndef _SC_SOCKET_H_
#define _SC_SOCKET_H_

int serverSocket(char *host, int port);
int clientSocket(int ssockfd);
int closeSocket(int sockfd);
int recvdata(int sockfd, char *buf, int bufsiz);

#endif	/* _SC_SOCKET_H_ */
