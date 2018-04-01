//ApplicationTest
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>

#define MAX_NAMEPATH 255

void givenAPath();
void whenPathIsGiven();
void thenVerifyPathIsCorrect();
void correctFileTest();
void thenVerifyPathIsNotNull();
void thenCreateStack();

void checkIsNotNull(void * pointer);
void ok();
void fail(char * failError);
void takeAFileTest();


void push(char * path);
char * pop();

struct stackNodeCDT
{
    char * path;
    struct stackNodeCDT *next;
};

struct stackNodeCDT * top = NULL;


char * examplePath = NULL;
DIR * file = NULL;
struct dirent * dir;

int main()
{
	printf("Testeo de archivos dados: ");
	correctFileTest();

	printf("Testeo de toma de un archivo: ");
	takeAFileTest();

	// printf("Testeo de Envio:\n");

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
          printf("%s\n",d_path);
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
    struct stackNodeCDT * nptr = malloc(sizeof(struct stackNodeCDT));
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
    struct stackNodeCDT * temp;
    temp = top;
    top = top->next;
    char * resp = temp->path;
    free(temp);
    return resp;
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
  ok();
}
