#ifndef _SC_SOCKET_H_
#define _SC_SOCKET_H_

#undef IPADDR
#define IPADDR "127.0.0.1"
#undef PORTNM
#define PORTNM 19860

int serverSocket(char *host, int port);
int clientSocket(int ssockfd);
int closeSocket(int sockfd);
int recvdata(int sockfd, char *buf, int bufsiz);

#endif	/* _SC_SOCKET_H_ */
