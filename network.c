#include "options.h"
#include "monitor.h"
#include "manager.h"
#include "director.h"
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <pthread.h>
#include <sys/select.h>

volatile int running;

void
sighandle(int sig) {
  running = 0;
}

int
main(int argc, char** argv)
{
  /* handle options */
  struct node* opts = (struct node*)malloc(sizeof(struct node));
  int ret = handleopts(argc, argv, opts);
  running = 1;
  if (ret != 0) {
    free(opts);
    return (ret == -1) ? EXIT_FAILURE : 0;
  }

  /* construct monitor */
  struct monitor* monitor = monitorcreate();
  monitor->host = opts;
  fd_set sockets;
  FD_ZERO(&sockets);
  monitor->sockets = &sockets;

  /* start manager */
  pthread_t manager;
  if (pthread_create(&manager, 0, &manage, monitor) != 0) {
    fprintf(stderr, "failed to create manager thread\n");
    free(monitor); free(opts);
    return EXIT_FAILURE;
  }

  /* start director */
  pthread_t director;
  if (pthread_create(&director, 0, &direct, monitor) != 0) {
    fprintf(stderr, "failed to create director thread\n");
    free(monitor); free(opts);
    return EXIT_FAILURE;
  }

  /* start server */
  signal(SIGINT, sighandle);
  running = 1;
  while (running);

  /* stop server */
  printf("kill signal recieved\n");
  monitor->running = 0;
  pthread_join(manager, 0);
  pthread_join(director, 0);
  printf("cleaning up\n");
  free(monitor); free(opts);

  return 0;
}
