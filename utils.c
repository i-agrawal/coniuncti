#include "utils.h"
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// int uerror;
//
// int
// ezerror(void)
// {
//   return uerror;
// }

void*
ezaddress(unsigned int ip, unsigned short port)
{
  struct sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_port = htons(port);
  address.sin_addr.s_addr = ip;
  int* iptr = (int*)address.sin_zero;
  iptr[0] = 0x00;
  iptr[1] = 0x00;

  struct sockaddr_in* sptr = (void*)malloc(sizeof(struct sockaddr_in));
  *sptr = address;
  return (void*)sptr;
}

int
ezsocket(void)
{
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    // error: socket creation failed
    // uerror = 1
    return -1;
  }
  fcntl(sockfd, F_SETFL, O_NONBLOCK);

  return sockfd;
}

int
ezbind(int sockfd, void* address)
{
  if (bind(sockfd, address, sizeof(struct sockaddr_in)) == -1) {
    // error: socket binding to address failed
    // uerror = 2
    close(sockfd);
    return -1;
  }

  return sockfd;
}

int
ezconnect(int sockfd, void* address)
{
  if (connect(sockfd, address, sizeof(struct sockaddr_in)) == -1) {
    // error: address connection failed
    // uerror = 3
    close(sockfd);
    return -1;
  }

  return sockfd;
}

int
ezlisten(int sockfd)
{
  if (listen(sockfd, 10) == -1) {
    // error: listening on socket failed
    // uerror = 4
    close(sockfd);
    return -1;
  }

  return sockfd;
}

int
ezaccept(int sockfd, char* addr)//, unsigned int* len)
{
  unsigned int length = sizeof(struct sockaddr_in);
  struct sockaddr_in address;
  int cfd = accept(sockfd, (struct sockaddr*)&address, (void*)&length);
  if (cfd == -1) {
    // error: accepting client failed
    // uerror = 5;
    strcpy(addr, "accepting client failed");
    return -1;
  }
  if (address.sin_family != AF_INET) {
    strcpy(addr, "non ipv4 address");
  }
  else {
    unsigned char* ip = (unsigned char *)&(address.sin_addr.s_addr);
    sprintf(addr, "%hhu.%hhu.%hhu.%hhu:%hu", ip[0], ip[1], ip[2], ip[3], address.sin_port);
  }

  return cfd;
}

int
ezserver(unsigned int ip, unsigned short port)
{
  int sockfd = ezsocket();
  if (sockfd == -1) {
    return -1;
  }
  void* addr = ezaddress(ip, port);
  sockfd = ezbind(sockfd, addr);
  free(addr);
  if (sockfd == -1) {
    return -1;
  }
  sockfd = ezlisten(sockfd);
  if (sockfd == -1) {
    return -1;
  }
  return sockfd;
}
