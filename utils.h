#ifndef __utils_h__
#define __utils_h__

typedef unsigned int uint;
typedef unsigned short ushort;

void* ezaddress(unsigned int, unsigned short);
int   ezsocket(void);
int   ezbind(int, void*);
int   ezconnect(int, void*);
int   ezlisten(int);
int   ezaccept(int, char*);
int   ezserver(unsigned int, unsigned short);
int   ezlisten(int);
// int   ezerror(void);

#endif
