#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(int argc, char *argv[]) {
  int shmsegid;
  char *p, *s;
  char c;

  //Crear memoria
  shmsegid = shmget(1237, 1024, IPC_CREAT | 0666);
  if (shmsegid < 0)
  {
    perror("shmget failed: ");
  }
  //Attach memoria a nuestro address space
  p = shmat(shmsegid, 0 , 0);
  if (p == (char *) -1)
  {
    perror("shmat failed: ");
  }
  int bucle = 1;
  sem_t * sema = sem_open("/sem",O_RDWR);

  if (sema == NULL)
  {
    perror("sem_open failed");

  }

  while(bucle)
  {
    sem_wait(sema);
    //read
    for (s = p; *s != '\0'; s++)
    {
      putchar(*s);

    }
    putchar('\n');

    sem_post(sema);

  }

  //free like

  shmdt(p);
  
  return 0;
}
