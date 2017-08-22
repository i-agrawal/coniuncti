#include "director.h"
#include "monitor.h"
#include "utils.h"
#include <sys/select.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void*
direct(void* monptr)
{
  struct monitor* monitor = monptr;
  printf("director started\n");

  // create servers
  struct node host = *(monitor->host);
  unsigned char* ip = (unsigned char*)&(host.ip);
  fd_set* sockets = monitor->sockets;
  int plistenfd = ezserver(host.ip, host.pport);
  if (plistenfd == -1) {
    fprintf(stderr, "peer server creation failed\n");
    return 0;
  }
  printf("listening for peer connections on %hhu.%hhu.%hhu.%hhu:%hu\n", ip[0], ip[1], ip[2], ip[3], host.pport);
  int clistenfd = ezserver(host.ip, host.cport);
  if (clistenfd == -1) {
    fprintf(stderr, "client server creation failed\n");
    close(plistenfd);
    return 0;
  }
  printf("listening for client connections on %hhu.%hhu.%hhu.%hhu:%hu\n", ip[0], ip[1], ip[2], ip[3], host.cport);

  // add servers
  while(monitor->fdlock);
  monitor->fdlock = 1;
  FD_SET(plistenfd, sockets);
  FD_SET(clistenfd, sockets);
  monitor->fdlock = 0;

  int ret, tmp;
  struct timeval tv;
  tv.tv_sec = 0;
  tv.tv_usec = 1000;
  fd_set reads;
  char* address = (char*)malloc(25);
  while (monitor->running) {
    while(monitor->fdlock);
    monitor->fdlock = 1;
    reads = *sockets;
    ret = select(FD_SETSIZE, &reads, 0, 0, &tv);
    if (ret == -1) {
      fprintf(stderr, "socket status management failed\n");
      goto dirclean;
    }
    else if (ret != 0) {
      if (FD_ISSET(plistenfd, &reads)) {
        tmp = ezaccept(plistenfd, address);
        if (tmp != -1) {
          printf("new peer connection %s\n", address);
          close(tmp);
        }
      }
    }
    monitor->fdlock = 0;
  }

  dirclean:
  printf("director stopping\n");
  FD_CLR(plistenfd, sockets);
  FD_CLR(clistenfd, sockets);
  close(plistenfd);
  close(clistenfd);
  return 0;
}
