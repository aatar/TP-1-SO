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

#define MAX_NAMEPATH 255
#define MAX_AMOUNT_OF_SLAVES 5;

queueADT filesQueue;
queueADT hashedFilesQueue;
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

  for (int argIndex = 1; argIndex < argc; argIndex++)
  {
    enqueueFile(argv[argIndex], filesQueue);
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
        int applicationSlaveFD;
        int slaveApplicationFD;
        sprintf(applicationSlavePipeName, "/tmp/%d%d", applicationPID, slavePID);
        sprintf(slaveApplicationPipeName, "/tmp/%d%d", slavePID, applicationPID);
        mkfifo(applicationSlavePipeName, 0666);
        mkfifo(slaveApplicationPipeName, 0666);
        applicationSlaveFD[index] = open(applicationSlavePipeName, O_WRONLY);
        slaveApplicationFD[index] = open(slaveApplicationPipeName, O_RDONLY);
        break;
    }
  }

  // bucle



  return 0;
}

void applicationProcess()
{
  fd_set readset;
  FD_ZERO( &readset );
  int maxfd = 0;
  for (int index ; index < MAX_AMOUNT_OF_SLAVES; index++)
  {
    FD_SET( slaveApplicationFD[inde] , &readset );
    maxfd = ( maxfd > slaveApplicationFD[inde])? maxfd : slaveApplicationFD[index];
  }

  int result;
  result = select(maxfd+1, &readset ,NULL, NULL, NULL );
  if (result == -1)
  {
    //ERROR;
  }
  else
  {
    for (int index = 0; index < MAX_AMOUNT_OF_SLAVES; index++)
    {
      if (FD_ISSET(slaveApplicationFD[index], &readset))
      {
        //Read FD from slave Index
      }
    }
  }

}

void makeFileQueue(char * path, queueADT queue)
{
	DIR * d = opendir(path);
	struct dirent * dir = readdir(d);
  if(dir != NULL) {
    if(dir-> d_type != DT_DIR)
    {
        char d_path[MAX_NAMEPATH];
        sprintf(d_path, "%s/%s", path, dir->d_name);
        enqueue(d_path, queue);
    }
  }
}
