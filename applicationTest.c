//ApplicationTest
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include "applicationProcess.h"
#include "testLib.h"
#include "queue.h"

#define MAX_NAMEPATH 255

void givenAPath();
void givenAQueue();
void givenTheFDS();

void whenPathIsGiven();
void whenPipesAreCreated();

void thenVerifyPathIsCorrect();
void thenVerifyPathIsNotNull();
void thenCreateFileQueue();
void thenVerifyPipeIsCreated();
void thenVerifyMultiplePipesAreCreated();

void correctFileTest();
void takeAFileTest();
void pipeCreationTest();


char * examplePath = NULL;
DIR * file = NULL;
struct dirent * dir;
int communicationPipes[4];
int * communicationMultiplePipes;
int pipeCreationFather;
int pipeCreationSon;
int multiplePipes[4];
int sonsAmount = 2;
queueADT queue;

int main()
{
	printf("Testeo de archivos dados: ");
	correctFileTest();

	printf("Testeo de toma de un archivo: ");
	takeAFileTest();

	// printf("Testeo de Envio:\n");
  printf("Testeo de creacion de canales de comunicacion con Hijos:");
  pipeCreationTest();

  return 0;
}


void correctFileTest()
{
	givenAPath();
	whenPathIsGiven();
	thenVerifyPathIsNotNull();
}

void givenAPath()
{
	examplePath = ".";
}

void givenAQueue()
{
	queue = createQueue();
}

void whenPathIsGiven()
{
	file = opendir(examplePath);
}

void thenVerifyPathIsNotNull()
{
	checkIsNotNull((void *)file);
}

void takeAFileTest()
{
  givenAPath();
	givenAQueue();
  whenPathIsGiven();
  thenCreateFileQueue();
}

void thenCreateFileQueue()
{
  makeFileQueue(examplePath, queue);
}

void pipeCreationTest()
{
  givenTheFDS();
  whenPipesAreCreated();
  thenVerifyPipeIsCreated();
  thenVerifyMultiplePipesAreCreated();
}

void givenTheFDS()
{
  /*
  * communicationPipes 0,1 belongs Father to Son 1
  * communicationPipes 2,3 belongs Son 1 to Father
  */
  communicationMultiplePipes = malloc(sizeof(int) * sonsAmount * 4);
}

void whenPipesAreCreated()
{
  // pipeCreationFather = pipe(communicationPipes);
  // pipeCreationSon = pipe(communicationPipes + 2);
  for(int i = 0; i < sonsAmount * 2; i++)
  {
    multiplePipes[i] = pipe(communicationMultiplePipes + (i * 2));
  }
}

void thenVerifyPipeIsCreated()
{
  if(pipeCreationFather == -1)
  {
    fail("Father pipe not created");
  }
  else {
    ok();
  }
  if (pipeCreationSon == -1)
  {
    fail("Son pipe not created");
  }
  else
  {
    ok();
  }
}

void thenVerifyMultiplePipesAreCreated()
{
  // j is the number of pipes. To a communication between father and son belogns two pipes. To make full duplex connection.
  for(int j = 0; j < sonsAmount * 2; j++)
  {
    if(multiplePipes[j] == -1)
    {
      fail("Father pipe not created");
      putchar('\n');
    }
    else {
      ok();
    }
    if (pipeCreationSon == -1)
    {
      fail("Son pipe not created");
      putchar('\n');
    }
    else
    {
      ok();
    }
  }
}
