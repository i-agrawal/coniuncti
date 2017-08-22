#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>

#define handleipv4(addr)\
    addr4 = addr;\
    if (addr4 != 0) {\
      address = (unsigned char*)(&addr4->sin_addr);\
      printf("%hhu.%hhu.%hhu.%hhu", address[0], address[1], address[2], address[3]);\
    }

#define handleipv6(addr)\
    addr6 = addr;\
    if (addr6 != 0) {\
      address = (unsigned char*)(&addr6->sin6_addr.s6_addr);\
      for (i = 0; i != 7; i++) {\
        printf("%x%x:", address[2*i], address[2*i+1]);\
      }\
      printf("%x%x", address[14], address[15]);\
    }

int
main(int argc, char** argv)
{
  struct ifaddrs* ifs;
  if (getifaddrs(&ifs) == -1) {
    fprintf(stderr, "getting interfaces failed\n");
  }
  struct ifaddrs* ptr;
  int length, i;
  char* name = "\0";
  struct sockaddr_in* addr4;
  struct sockaddr_in6* addr6;
  unsigned char* address;
  unsigned short* address6;
  for (ptr = ifs; ptr->ifa_next != 0; ptr = ptr->ifa_next) {
    if (strcmp(name, ptr->ifa_name) != 0) {
      name = ptr->ifa_name;
      length = strlen(name);
      printf("%s: flags=%u\n", name, ptr->ifa_flags);
    }
    if (ptr->ifa_addr->sa_family == AF_INET) {
      printf("%*c  inet ", length, ' ');
      handleipv4((void*)(ptr->ifa_addr));
      printf(" netmask ");
      handleipv4((void*)(ptr->ifa_netmask));
      printf(" broadcast ");
      handleipv4((void*)(ptr->ifa_broadaddr));
      printf("\n");
    }
    else if (ptr->ifa_addr->sa_family == AF_INET6) {
      printf("%*c  inet6 ", length, ' ');
      handleipv6((void*)(ptr->ifa_addr));
      printf(" netmask ");
      handleipv6((void*)(ptr->ifa_netmask));
      printf(" broadcast ");
      handleipv6((void*)(ptr->ifa_broadaddr));
      printf("\n");
    }
  }
  freeifaddrs(ifs);
  return 0;
}
