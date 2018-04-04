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
#include "applicationProcess.h"
#include "queue.h"

#define DATA_SIZE 1000
#define FILES_COUNT 5

queueADT filesQueue;
queueADT hashedFilesQueue;
queueADT finalQueue;
int pidSlaves[MAX_AMOUNT_OF_SLAVES];
int applicationSlaveFD[MAX_AMOUNT_OF_SLAVES];
int slaveApplicationFD[MAX_AMOUNT_OF_SLAVES];
char buffer[256];
char * testingFilesPaths[] = {"file1.txt", "file2.txt", "file3.txt", "file4.txt", "file5.txt"};

char * calculatedMD5Hashes[] = {"4d186321c1a7f0f354b297e8914ab240",
							 "cdcf8d0131420ccecc2e769f5db62e1b",
							 "61938292ca4cf3be12141ab6bc01c1d3",
							 "1683e3c47cf7a46dc308a70ca7d016c2",
							 "f130f9b486ae8f768161a710163ac80e"};


int main(int argc, char const *argv[])
{

  char applicationSlavePipeName[MAX_PIPENAME_LENGTH];
  char slaveApplicationPipeName[MAX_PIPENAME_LENGTH];

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
  for (int index = 0; index < 5; index++) {
    switch (slavePID = fork()) {
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
        sprintf(applicationSlavePipeName, "./%d%d", applicationPID, slavePID);
        sprintf(slaveApplicationPipeName, "./%d%d", slavePID, applicationPID);
        sleep(1);
        mkfifo(applicationSlavePipeName, 0666);
        mkfifo(slaveApplicationPipeName, 0666);
        applicationSlaveFD[index] = open(applicationSlavePipeName, O_WRONLY);
        slaveApplicationFD[index] = open(slaveApplicationPipeName, O_RDONLY);
        break;
    }
		while(thereAreSlavesAlive())
		{
				applicationProcess();
		}
  }

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
  printf("%s\n", buffer);
  if (!strncmp(WAITING_MESSAGE, buffer, nbytes))
  {
    enqueue(buffer, hashedFilesQueue);
    enqueue(buffer, finalQueue);
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
  // close shm and fd
}

// void createFile()
// {
//   char userContent[FINAL_FILE_SIZE];
//   FILE * fPtr;
//   fPtr = fopen("fileOutput.txt", "w");
//
//   if(fPtr == NULL)
//   {
//     printf("Unable to create file.\n");
//     exit(10);
//   }
//
// 	strcpy(userContent, "");
// 	for(int i = 0; i < FILES_COUNT; i++) {
// 		strcat(userContent, testingFilesPaths[i]);
// 		strcat(userContent, ": ");
// 		strcat(userContent, calculatedMD5Hashes[i]);
// 		strcat(userContent, "\t");
// 	}
//   fputs(userContent, fPtr);
//   fclose(fPtr);
// }
