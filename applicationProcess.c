#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "applicationProcess.h"
#include "queue.h"

#define DATA_SIZE 1000
#define FILES_COUNT 5

queueADT filesQueue;
queueADT hashedFilesQueue;
int applicationPipes[MAX_AMOUNT_OF_PIPES];
int slavesPipes[MAX_AMOUNT_OF_PIPES];
int pidSlaves[MAX_AMOUNT_OF_SLAVES];
char buffer[256];
char * testingFilesPaths[] = {"file1.txt", "file2.txt", "file3.txt", "file4.txt", "file5.txt"};

char * calculatedMD5Hashes[] = {"4d186321c1a7f0f354b297e8914ab240",
							 "cdcf8d0131420ccecc2e769f5db62e1b",
							 "61938292ca4cf3be12141ab6bc01c1d3",
							 "1683e3c47cf7a46dc308a70ca7d016c2",
							 "f130f9b486ae8f768161a710163ac80e"};
int main(int argc, char const *argv[])
{
  if(argc < 2)
  {
    printf("You haven't passed a valid pattern\n");
    return 1;
  }

  filesQueue = createQueue();
  hashedFilesQueue = createQueue();
  //vector de pipelines
  // Forks ()
  // while y switch
  /*Tareas del Padre: Lectura de pipes
  *Si le mandaron un archivo:
  * 1- Mandar a shared memory con vista
  * 2- Mandarlo a Archivo
  * 3- Le mando un archivo al esclavo mientras tenga archivos a procesar.
  */
  makeFileQueue(argv[1], filesQueue);

  // creating pipes
  for (int pipeIndex = 0; pipeIndex < MAX_AMOUNT_OF_SLAVES; pipeIndex++)
  {
    pipe(applicationPipes + (pipeIndex * 2));
    pipe(slavesPipes + (pipeIndex * 2));
  }


  int applicationPID = getpid();
  createProcesses(applicationPID);

  return 0;
}


void createProcesses(int applicationPID)
{
  int pidChild;
  for (int index = 0; index < MAX_AMOUNT_OF_SLAVES; index++) {
    switch (pidChild = fork()) {
      case -1:
        perror("Fork failed\n");
        abort();
        break;
      case 0:
        int myValue = index;
        int bucle = 1;
        int nbytes;
        char * outputBuffer[MAX_NAMEPATH];
        close(slavesPipes[index * 2]);
        close(applicationPipes[index * 2 + 1]);
        while(bucle)
        {
          bzero(buffer, MAX_NAMEPATH);
          nbytes = read(applicationPipes(index * 2), buffer, MAX_NAMEPATH);

          if (strncmp("0", buffer, 1))
          {
              bucle = 0;
          }
          else if (nbytes == 0)
          {
            // probarlo
            sprintf(buffer, "0");
            write(slavesPipes[index * 2 + 1], buffer, MAX_NAMEPATH);
          }
          else
          {
            bzero(outputBuffer, MAX_NAMEPATH);
            strcpy(outputBufferbuffer, calculateFileMD5Hash(buffer));
            write(slavesPipes[index * 2 + 1], outputBuffer, strlen(outputBuffer));
          }
        }
        return;
      default:
        close(slavesPipes[index * 2 + 1]);
        close(applicationPipes[index * 2]);
        pidSlaves[index] = pidChild;
        break;
    }
  }
  int processView = fork();
  if (processView == -1)
  {

  }
  else if(processView == 0)
  {

  }
  else if (pidChild != 0)
  {
    int bucle;
    int nbytes;
    while(bucle)
    {
      for (int slaveIndex = 0; slaveIndex < MAX_AMOUNT_OF_SLAVES; slaveIndex++)
      {
          if (!pidSlaves[slaveIndex] || nbytes = read(slaveIndex[slaveIndex * 2], buffer, MAX_NAMEPATH))
          {
            if (strncmp("0", buffer, nbytes))
            {
                answerSlaveRequest(slaveIndex);
            }
            else
            {
              // Mandar a vista resultado mediante shm_memory;
              // Mandar a Archivo;
              // Mandar nuevo archivo a ese esclavo;
            }
          }
        }
      }
    }
}

void answerSlaveRequest(int slaveIndex)
{
  char * fatherOutputBuffer;
  if (isEmpty())
  {
    strcpy(fatherOutputBuffer, "0");
    write(applicationPipes(slaveIndex * 2 + 1), fatherOutputBuffer, strlen(fatherOutputBuffer));
    pidSlaves[slaveIndex] = -1;
  }
  else
  {
    strcpy(fatherOutputBuffer, pop());
    write(applicationPipes(slaveIndex * 2 + 1), fatherOutputBuffer, strlen(fatherOutputBuffer));
  }
}

void makeFileQueue(char * path, queueADT queue)
{
	DIR * d = opendir(path);
	struct dirent * dir;
 	while ((dir = readdir(d)) != NULL)
  {
    	if(dir-> d_type != DT_DIR)
      {
          char d_path[MAX_NAMEPATH];
        	sprintf(d_path, "%s/%s", path, dir->d_name);
        	enqueue(d_path, queue);
      }
      else
      {
      	if(strcmp(dir->d_name,".") != 0 && strcmp(dir->d_name,"..") != 0 )
      	{
        	char d_path[MAX_NAMEPATH]; // here I am using sprintf which is safer than strcat
        	sprintf(d_path, "%s/%s", path, dir->d_name);
        	makeFileQueue(d_path, queue); // recall with the new path
      	}
      }
  }
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
