#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <strings.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "slaveProcess.h"
#include "fileDescriptors.h"
#include "applicationProcess.h"

int main() {
	int bucle = 1;
	int applicationPID = getppid();
	int slavePID = getpid();

	char * applicationSlavePipeName;
	char * slaveApplicationPipeName;
	sprintf(applicationSlavePipeName, "/tmp/%d%d", applicationPID, slavePID);
	sprintf(slaveApplicationPipeName, "/tmp/%d%d", slavePID, applicationPID);
	int applicationSlaveFD;
	int slaveApplicationFD;
	mkfifo(applicationSlavePipeName, 0666);
	mkfifo(slaveApplicationPipeName, 0666);
	applicationSlaveFD = open(applicationSlavePipeName, O_RDONLY);
	slaveApplicationFD = open(slaveApplicationPipeName, O_WRONLY);
	char message [MAX_BUFFER_SIZE];
	bzero(message, MAX_BUFFER_SIZE);
	strncpy(message, WAITING_MESSAGE, MAX_NAMEPATH);
	write(slaveApplicationFD, message, strlen(message));
	int nbytes;
	char inputBuffer [MAX_NAMEPATH];
	char * fileAndHash;
	while(bucle) {
			bzero(inputBuffer, MAX_NAMEPATH);
			nbytes= read(applicationSlaveFD, inputBuffer, MAX_NAMEPATH);
			if (strncmp(KILL_MESSAGE, inputBuffer, nbytes))
			{
				bucle = 0;
			}
			else
			{
				fileAndHash = calculateFileMD5Hash(inputBuffer);
				write(slaveApplicationFD, fileAndHash, strlen(fileAndHash));
			}
	}
	return 0;
}

char * calculateFileMD5Hash(char * fileName)
{
	int status = 0;
	char * hashCalculated = malloc(sizeof(char) * 32);
	char * dump = malloc(sizeof(char) * 1000);
	char * fileWithHash = malloc(sizeof(char) * MAX_BUFFER_SIZE);
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

	sprintf(fileWithHash, "%s: %s", fileName, hashCalculated);
	return fileWithHash;
}

void executeMD5HashCommand(char * fileName)
{
	char * arguments[] = {"md5sum", fileName, NULL};
	execvp(arguments[0], arguments);
}
