#include "options.h"
#include "monitor.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void
usage(void* fd, char* name)
{
  fprintf(fd, "usage: %s\n", name);
  fprintf(fd, "   -h, --help         print this message\n");
  fprintf(fd, "   -s, --subnet       !mandatory! the subnet cidr [ex. 127.0.0.1/24]\n");
  fprintf(fd, "       --connect      comma seperated list of cluster connections\n");
  fprintf(fd, "       --server-ip    ip to server on [default: 0.0.0.0]\n");
  fprintf(fd, "       --peer-port    port for peers to connect [default: 5535]\n");
  fprintf(fd, "       --client-port  port for clients to connect [default: 5536]\n");
}

int
handleopts(int argc, char** argv, void* dest)
{
  struct node opts;
  opts.pport = 5535;
  opts.cport = 5556;
  opts.ip = 0;
  opts.subnet = 0;
  opts.mask = 0;
  unsigned char* ipptr = (unsigned char*)&(opts.ip);
  unsigned char* subptr = (unsigned char*)&(opts.subnet);
  unsigned char* maskptr = (unsigned char*)&(opts.mask);

  int conns;
  unsigned char* connects;
  unsigned short* conports;

  char* arg;
  unsigned int i, j, k;
  unsigned char* kptr = (unsigned char*)&k;
  for (i = 1; i != argc; i++) {
    if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
      usage(stdout, argv[0]);
      return 1;
    }
    else if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--subnet") == 0) {
      if (i+1 == argc) {
        fprintf(stderr, "%s: no value provided\n", argv[i]);
        goto error;
      }
      arg = argv[i+1];
      unsigned char masksz;
      j = sscanf(arg, "%hhu.%hhu.%hhu.%hhu/%hhu", subptr, subptr+1, subptr+2, subptr+3, &masksz);
      if (j != 5) {
        fprintf(stderr, "improper subnet specification \"%s\"\n", arg);
        goto error;
      }
      if (masksz > 32) {
        fprintf(stderr, "improper masking: %hhu bits\n", masksz);
        goto error;
      }
      k = 0;
      for (j = 0; j != 31; j++) {
        k |= (j < masksz);
        k <<= 1;
      }
      k |= (j < masksz);
      maskptr[0] = kptr[3];
      maskptr[1] = kptr[2];
      maskptr[2] = kptr[1];
      maskptr[3] = kptr[0];
      i++;
    }
    else if (strcmp(argv[i], "--connect") == 0){
      if (i+1 == argc) {
        fprintf(stderr, "--connect: no value provided\n");
        goto error;
      }
      arg = argv[i+1];
      conns = 1;
      for (j = 0; arg[j] != 0; j++) {
        conns += (arg[j] == ',');
      }
      connects = (unsigned char*)malloc(4*conns);
      conports = (unsigned short*)malloc(2*conns);
      char* addr = strtok(arg, ",");
      for (j = 0; addr != 0; j++) {
        k = sscanf(addr, "%hhu.%hhu.%hhu.%hhu:%hu", connects+4*j, connects+4*j+1, connects+4*j+2, connects+4*j+3, conports+j);
        if (k != 5) {
          fprintf(stderr, "improper connection specification \"%s\"\n", addr);
          goto error;
        }
        addr = strtok(0, ",");
      }
      if (j != conns) {
        fprintf(stderr, "improper delimiting\n");
        goto error;
      }
      i++;
    }
    else if (strcmp(argv[i], "--server-ip") == 0){
      if (i+1 == argc) {
        fprintf(stderr, "--server-ip: no value provided\n");
        goto error;
      }
      arg = argv[i+1];
      j = sscanf(arg, "%hhu.%hhu.%hhu.%hhu", ipptr, ipptr+1, ipptr+2, ipptr+3);
      if (j != 4) {
        fprintf(stderr, "improper server ip specification \"%s\"\n", arg);
        goto error;
      }
      i++;
    }
    else if (strcmp(argv[i], "--client-port") == 0){
      if (i+1 == argc) {
        fprintf(stderr, "--client-port: no value provided\n");
        goto error;
      }
      arg = argv[i+1];
      j = sscanf(arg, "%hu", &(opts.cport));
      if (j != 1) {
        fprintf(stderr, "improper client port specification \"%s\"\n", arg);
        goto error;
      }
      i++;
    }
    else if (strcmp(argv[i], "--peer-port") == 0){
      if (i+1 == argc) {
        fprintf(stderr, "--peer-port: no value provided\n");
        goto error;
      }
      arg = argv[i+1];
      j = sscanf(arg, "%hu", &(opts.pport));
      if (j != 1) {
        fprintf(stderr, "improper peer port specification \"%s\"\n", arg);
        goto error;
      }
      i++;
    }
    else {
      fprintf(stderr, "unknown option \"%s\"\n", argv[i]);
      goto error;
    }
  }
  if (opts.subnet == 0 || opts.mask == 0) {
    fprintf(stderr, "please specify a proper subnet\n");
    goto error;
  }
  struct node* dst = dest;
  *dst = opts;
  return 0;

  error:
  usage(stderr, argv[0]);
  return -1;
}
