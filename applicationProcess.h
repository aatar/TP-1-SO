#ifndef APPLICATION_PROCESS_H
#define APPLICATION_PROCESS_H

#define MAX_NAMEPATH 255
#define MAX_AMOUNT_OF_SLAVES 5;

void push(char * path);
char * pop();
int isEmpty();
void makeStack(char * path);

void pathIsCorrect(char const * argv[]);
void createPipes();
void createProcesses();

typedef struct
{
    char * path;
    struct stackNodeCDT *next;
} stackNodeCDT;


#endif
