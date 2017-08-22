#include <stdio.h>

#include "manager.h"
#include "monitor.h"

void*
manage(void* monptr)
{
  struct monitor* monitor = monptr;
  printf("manager started\n");

  while (monitor->running) {

  }

  printf("manager stopping\n");
  return 0;
}
