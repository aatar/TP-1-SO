#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>
#include "applicationProcess.h"
#include "queue.h"

queueADT filesQueue;
queueADT hashedFilesQueue;
queueADT finalQueue;
int pidSlaves[MAX_AMOUNT_OF_SLAVES];
int applicationSlaveFD[MAX_AMOUNT_OF_SLAVES];
int slaveApplicationFD[MAX_AMOUNT_OF_SLAVES];
char buffer[256];

int main(int argc, char const *argv[])
{
  char * applicationSlavePipeName;
  char * slaveApplicationPipeName;

  if(argc < 2)
  {
    printf("You haven't passed a valid pattern\n");
    return 1;
  }

  filesQueue = createQueue();
  hashedFilesQueue = createQueue();
  finalQueue = createQueue();

  for (int argIndex = 1; argIndex < argc; argIndex++)
  {
    makeFileQueue(argv[argIndex], filesQueue);
  }


  int applicationPID = getpid();

  int slavePID;
  for (int index = 0; index < MAX_AMOUNT_OF_SLAVES; index++) {
    switch (slavePID = fork()) {
      case -1:
        perror("Fork failed\n");
        exit(1);
        break;
      case 0:

        //exec slave;
        execlp("./slaveProcess.o", "slaveProcess.o", NULL);
        perror("Slave process exec() failed. Exitting.");
        exit(1);
        break;
      default:

        pidSlaves[index] = slavePID;
        sprintf(applicationSlavePipeName, "/tmp/%d%d", applicationPID, slavePID);
        sprintf(slaveApplicationPipeName, "/tmp/%d%d", slavePID, applicationPID);
        mkfifo(applicationSlavePipeName, 0666);
        mkfifo(slaveApplicationPipeName, 0666);
        applicationSlaveFD[index] = open(applicationSlavePipeName, O_WRONLY);
        slaveApplicationFD[index] = open(slaveApplicationPipeName, O_RDONLY);
        break;
    }
  }
  /*int viewPID;
  *switch de mierda
  *if(viewPID = fork())
  *{
  * Soy VIsta, WIndowsVista
  *}
  *else
  *{
  * Soy el fucking application
  * while (thereAreSlavesAlive){
    applicationProcess();
  *  }
  * }
  */


  return 0;
}

void applicationProcess()
{
  fd_set readset;
  FD_ZERO(&readset);
  int maxfd = 0;
  for (int index; index < MAX_AMOUNT_OF_SLAVES; index++)
  {
    FD_SET(slaveApplicationFD[index], &readset);
    maxfd = (maxfd > slaveApplicationFD[index]) ? maxfd : slaveApplicationFD[index];
  }

  int result;
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
  for(int x=0; x < MAX_AMOUNT_OF_SLAVES; x++)
  {
    if(slavePID[x] > 0)
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
  nbytes = read( slaveApplicationFD[index] , buffer, MAX_BUFFER_SIZE);
  if (!strncmp(WAITING_MESSAGE, buffer, nbytes))
  {
    enqueue(buffer, hashedFilesQueue);
    enqueue(buffer, finalQueue);
    //Buffer contiene Resultado archivo;
    //Agregar a la queue;
    //Agregar a Archivo.
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
  write(applicationSlaveFD[Index] , applicationOutputBuffer, strlen(applicationOutputBuffer));
}

void makeFileQueue(char * path, queueADT queue)
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

}
