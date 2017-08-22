#include "monitor.h"
#include <stdlib.h>

struct monitor*
monitorcreate()
{
  struct monitor* ptr = (struct monitor*)malloc(sizeof(struct monitor));
  struct monitor mon = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
  *ptr = mon;
  return ptr;
}
