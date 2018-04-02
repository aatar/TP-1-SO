#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include "slaveProcess.h"
#include "fileDescriptors.h"
#include "queueOfFiles.h"

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
		// replace standard output with output part of pipe
		dup2(fileDescriptors[1], STDOUT);
		// close unused end of pipe
		close(fileDescriptors[0]);
		//execute md5 hash
		executeMD5HashCommand(fileName);
	}

	// replace standard input with output part of pipe
	dup2(fileDescriptors[0], STDIN);
	// close unused end of pipe
	close(fileDescriptors[1]);

	wait(&status);

	scanf("%s", hashCalculated);
	while(scanf("%s", dump) > 0)
	return hashCalculated;
}

void executeMD5HashCommand(char * fileName)
{
	char * arguments[] = {"md5sum", fileName, NULL};
	execvp(arguments[0], arguments);
}
