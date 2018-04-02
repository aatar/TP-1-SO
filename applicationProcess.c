#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include "applicationProcess.h"

stackNodeCDT * top = NULL;
int slavesAmount = MAX_AMOUNT_OF_SLAVES;
int applicationPipes[MAX_AMOUNT_OF_SLAVES * 2];
int slavesPipes[MAX_AMOUNT_OF_SLAVES * 2];
int pidSlaves[MAX_AMOUNT_OF_SLAVES];
char buffer[256];

int main(int argc, char const *argv[])
{
  if(!pathIsCorrect(argc, argv))
  {
    return 1;
  }
  //vector de pipelines
  // Forks ()
  // while y switch
  /*Tareas del Padre: Lectura de pipes
  *Si le mandaron un archivo:
  * 1- Mandar a shared memory con vista
  * 2- Mandarlo a Archivo
  * 3- Le mando un archivo al esclavo mientras tenga archivos a procesar.
  */
  makeStack(argv[1]);
  createPipes();
  int applicationPID = getpid();
  createProcesses();

  return 0;
}

int pathIsCorrect(int argc, char const * argv[])
{
  if(argc < 2)
  {
    printf("No se ha ingresado ningun argumento valido\n");
    return 1;
  }
  DIR * file = opendir(argv[1]);

  if (file == NULL)
  {
    printf("Path ingresado no corresponde a un path existente\n", );
    return 1;
  }
  return 0;

}

void createPipes()
{
  for (int pipeIndex = 0; pipeIndex < MAX_AMOUNT_OF_SLAVES; pipeIndex++)
  {
    pipe(applicationPipes + (pipeIndex * 2));
    pipe(slavesPipes + (pipeIndex * 2));
  }
}

void createProcesses()
{
  int pidChild;
  for (int index = 0; index < MAX_AMOUNT_OF_SLAVES; index++) {
    switch (pidChild = fork()) {
      case -1:
        // mensaje de error
        exit(1);
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

void makeStack(char * path)
{
	DIR * d = opendir(path);
	struct dirent * dir;
 	while ((dir = readdir(d)) != NULL)
  {
    	if(dir-> d_type != DT_DIR)
      {
          char d_path[MAX_NAMEPATH];
        	sprintf(d_path, "%s/%s", path, dir->d_name);
        	push(d_path);
      }
      else
      {
      	if(strcmp(dir->d_name,".") != 0 && strcmp(dir->d_name,"..") != 0 )
      	{
        	char d_path[MAX_NAMEPATH]; // here I am using sprintf which is safer than strcat
        	sprintf(d_path, "%s/%s", path, dir->d_name);
        	makeStack(d_path); // recall with the new path
      	}
      }
  }
}



void push(char * item)
{
    stackNodeCDT * nptr = malloc(sizeof(stackNodeCDT));
    nptr->path = item;
    nptr->next = top;
    top = nptr;
}

int isEmpty()
{
	return top == NULL;
}

char * pop()
{
    stackNodeCDT * temp;
    temp = top;
    top = top->next;
    char * resp = temp->path;
    free(temp);
    return resp;
}
