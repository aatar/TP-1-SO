//ApplicationTest
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include "applicationProcess.h"

#define MAX_NAMEPATH 255

void givenAPath();
void givenTheFDS();

void whenPathIsGiven();
void whenPipesAreCreated();

void thenVerifyPathIsCorrect();
void thenVerifyPathIsNotNull();
void thenCreateStack();
void thenVerifyPipeIsCreated();
void thenVerifyMultiplePipesAreCreated();

void correctFileTest();
void takeAFileTest();
void pipeCreationTest();

void checkIsNotNull(void * pointer);
void ok();
void fail(char * failError);


char * examplePath = NULL;
DIR * file = NULL;
struct dirent * dir;
int communicationPipes[4];
int * communicationMultiplePipes;
int pipeCreationFather;
int pipeCreationSon;
int multiplePipes[4];
int sonsAmount = 2;

stackNodeCDT * top = NULL;

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

void whenPathIsGiven()
{
	file = opendir(examplePath);
}

void thenVerifyPathIsNotNull()
{
	checkIsNotNull((void *)file);
}


void checkIsNotNull(void * pointer)
{
	if (pointer == NULL)
	{
		fail("	Expected: a non null pointer, got: a null pointer");
	}
	else
	{
		ok();
	}
}


void ok()
{
	printf("	Ok!\n");
}

void fail(char * withError)
{
	putchar('\n');
	fprintf(stderr, "%s", withError);
}


void takeAFileTest()
{
  givenAPath();
  whenPathIsGiven();
  thenCreateStack();
}

void thenCreateStack()
{
  makeStack(examplePath);
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
