#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <sys/select.h>
#include "applicationProcess.h"
#include "queue.h"
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/select.h>

queueADT filesQueue;
char finalValues[MAX_FINAL_FILE_SIZE];
int pidSlaves[MAX_AMOUNT_OF_SLAVES];
int applicationSlaveFD[MAX_AMOUNT_OF_SLAVES];
int slaveApplicationFD[MAX_AMOUNT_OF_SLAVES];
char buffer[256];
//Inicializacion shm y sem
/*
int shmsegid;
char *p, *s;
char c;
sem_t * sema;
*/


int main(int argc, char const *argv[])
{
  /* para inicializacion shm y semaforos
  sem_unlink("/sem");
  sema = sem_open("/sem",O_CREAT|O_EXCL,0777,1);

  //Crear memoria
  shmsegid = shmget(1237, 1024, IPC_CREAT | 0666);

  if (shmsegid < 0)
  {
    perror("shmget failed: ");
  }
  // Attach memoria a nuestro address space
  p = shmat(shmsegid, 0 , 0);

  if (p == (char *) -1)
  {
      perror("shmat failed: ");
  }
  */
  char applicationSlavePipeName[MAX_PIPENAME_LENGTH];
  char slaveApplicationPipeName[MAX_PIPENAME_LENGTH];

  if(argc < 2)
  {
    printf("You haven't passed any pattern\n");
    return 1;
  }

  filesQueue = createQueue();
  strcpy(finalValues, "");

  for (int argIndex = 1; argIndex < argc; argIndex++)
  {
    makeFileQueue(argv[argIndex], filesQueue);
  }


  int applicationPID = getpid();

  int slavePID;
  for (int index = 0; index < MAX_AMOUNT_OF_SLAVES; index++) 
  {
    switch (slavePID = fork()) 
    {
      case -1:
        perror("Fork failed\n");
        exit(1);
        break;
      case 0:
        execlp("./slaveProcess", "slaveProcess", NULL);
        perror("Slave process exec() failed. Exitting.");
        exit(1);
        break;
      default:
        pidSlaves[index] = slavePID;
        sprintf(applicationSlavePipeName, "/tmp/%d%d", applicationPID, slavePID);
        sprintf(slaveApplicationPipeName, "/tmp/%d%d", slavePID, applicationPID);
        sleep(1);
        mkfifo(applicationSlavePipeName, 0666);
        mkfifo(slaveApplicationPipeName, 0666);
        applicationSlaveFD[index] = open(applicationSlavePipeName, O_WRONLY);
        slaveApplicationFD[index] = open(slaveApplicationPipeName, O_RDONLY);
        answerSlaveRequest(index);
        break;
    }
  }
/* Exec viewProcess  
int pidView;
  if (pidView = fork())
  {
    execlp("./viewProcess", "viewProcess", NULL);
    perror("Vista process exec() failed. Exitting.");
    exit(1);
  }
*/	
  createFile();
  while(thereAreSlavesAlive())
  {
      		printf("", thereAreSlavesAlive());
		applicationProcess();
  }
  createFile();
  closeAll();
  return 0;
  }

void applicationProcess()
{
  fd_set readset;
  FD_ZERO(&readset);
  int maxfd = 0;
  int result;
  for (int index; index < MAX_AMOUNT_OF_SLAVES; index++)
  {
    FD_SET(slaveApplicationFD[index], &readset);
    maxfd = (maxfd > slaveApplicationFD[index]) ? maxfd : slaveApplicationFD[index];
  }

  result = select(maxfd + 1, &readset ,NULL, NULL, NULL );
  if (result == -1)
  {
    perror("Select failed\n");
    exit(1);
  }
  else
  {
    for (int index = 0; index < MAX_AMOUNT_OF_SLAVES; index++)
    {
      if (FD_ISSET(slaveApplicationFD[index], &readset))
      {
        readSlavePipe(index);
      }
    }
  }

}

int thereAreSlavesAlive()
{
  for(int slaveIndex = 0; slaveIndex < MAX_AMOUNT_OF_SLAVES; slaveIndex++)
  {
    if(pidSlaves[slaveIndex] > 0)
    {
      return 1;
    }
  }
  return 0;
}

void readSlavePipe(int index)
{
  int nbytes;
  char buffer[MAX_BUFFER_SIZE];
  bzero(buffer, MAX_BUFFER_SIZE);
  nbytes = read(slaveApplicationFD[index] , buffer, MAX_BUFFER_SIZE);
  if (strncmp(WAITING_MESSAGE, buffer, nbytes))
  {
    strcat(finalValues, buffer);
    strcat(finalValues, "\r\n");

    	/* Para enviar mensaje por shm
    	sem_wait(sema);
	//Write message
	s = p;
  	for (c = 0; c < strlen(buffer); c++)
	{
		*s++ = buffer[c];
	}
  	*s = '\0';
	sem_post(sema);
   	*/
  }
   answerSlaveRequest(index);;
   return;
}

void answerSlaveRequest(int index)
{
  char applicationOutputBuffer[MAX_NAMEPATH];
  bzero(applicationOutputBuffer, MAX_NAMEPATH);
  if(isEmpty(filesQueue))
  {
    strcpy(applicationOutputBuffer, KILL_MESSAGE);
    pidSlaves[index] = -1;
  }
  else
  {
    strcpy(applicationOutputBuffer, dequeue(filesQueue));
  }
  write(applicationSlaveFD[index] , applicationOutputBuffer, strlen(applicationOutputBuffer));
}

void makeFileQueue(const char * path, queueADT queue)
{
	DIR * d = opendir(path);
	if(errno == ENOTDIR)
  {
    enqueue(path, queue);
  }
	closedir(d);
  return;
}

void closeAll()
{
  char applicationSlavePipeName[MAX_PIPENAME_LENGTH];
  char slaveApplicationPipeName[MAX_PIPENAME_LENGTH];
  int applicationPID = getpid();
  printf("Bye\n");
  for (int i = 0; i < MAX_AMOUNT_OF_SLAVES; i++) {
    close(applicationSlaveFD[i]);
    close(slaveApplicationFD[i]);
    sprintf(applicationSlavePipeName, "/tmp/%d%d", applicationPID, pidSlaves[i]);
    sprintf(slaveApplicationPipeName, "/tmp/%d%d", pidSlaves[i], applicationPID);
    unlink(applicationSlavePipeName);
    unlink(slaveApplicationPipeName);
  }
  // close shm and fd
}

void createFile()
{
  FILE * fPtr;
  fPtr = fopen("fileOutput.txt", "w");
  if(fPtr == NULL)
  {
    printf("Unable to create file.\n");
    exit(10);
  }
  fputs(finalValues, fPtr);
  fclose(fPtr);
}
