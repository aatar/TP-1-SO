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
    if(pidSlaves[x] > 0)
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

}

void createFile()
{
    /* Variable to store user content */
    char data[DATA_SIZE];
	
	/* File pointer to hold reference to our file */
    FILE * fPtr;


    /* 
     * Open file in w (write) mode. 
     * "data/file1.txt" is complete path to create file
     */
    fPtr = fopen("fileOutput.txt", "w");


    /* fopen() return NULL if last operation was unsuccessful */
    if(fPtr == NULL)
    {
        /* File not created hence exit */
        printf("Unable to create file.\n");
        exit(EXIT_FAILURE);
    }


    /* Input contents from user to store in file */
	strcpy(data,"");
    int i;
	for(i = 0; i < FILES_COUNT; i++) {
		strcat(data, testingFilesPaths[i]);
		strcat(data, " ");
		strcat(data, calculatedMD5Hashes[i]);
		strcat(data, "\t");
	}
	
	/* Write contents to file */
    fputs(data, fPtr);


    /* Close file to save contents */
    fclose(fPtr);
}
