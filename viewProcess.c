#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <signal.h>


int main(int argc, char *argv[]) {
  int bucle = 1;
  sem_t *s = sem_open("/sem",O_RDWR);

  if (s == NULL)
  {
    perror("sem_open failed");

  }

  while(bucle)
  {
    sem_wait(s);
    //read
    for (s = p; *s != '\0'; s++)
    {
      putchar(*s);

    }
    putchar('\n');

    sem_post(s);

  }

  //free like

  shmdt(p);
  return 0;
}
