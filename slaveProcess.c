#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include "slaveProcess.h"
#include "fileDescriptors.h"

int main() {
	int bucle = 1;
	while(bucle) {



	}
	return 0;
}

char * calculateFileMD5Hash(char * fileName)
{
	int status = 0;
	char * hashCalculated = malloc(sizeof(char)*32);
	char * dump = malloc(sizeof(char)*1000);
	int fileDescriptors[2];
	pid_t pid;

	if(pipe(fileDescriptors) < 0)
	{
		fprintf(stderr, "Pipe Failed\n");
		exit(1);
	}

	pid = fork();
	if(pid < 0)
	{
		fprintf(stderr, "Fork Failed\n");
		exit(1);
	}
	else if (pid == 0)
	{
		dup2(fileDescriptors[1], STDOUT);
		close(fileDescriptors[0]);
		executeMD5HashCommand(fileName);
	}

	dup2(fileDescriptors[0], STDIN);
	close(fileDescriptors[1]);
	wait(&status);

	scanf("%s", hashCalculated);
	while(scanf("%s", dump) > 0)

	char * fileWithHash;
	sprintf(fileWithHash, "%s: %s", fileName, hashCalculated);
	return fileWithHash;
}

void executeMD5HashCommand(char * fileName)
{
	char * arguments[] = {"md5sum", fileName, NULL};
	execvp(arguments[0], arguments);
}
