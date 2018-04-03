#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include "applicationProcess.h"
#include "queue.h"

#define MAX_NAMEPATH 255
#define MAX_AMOUNT_OF_SLAVES 5;
#define MAX_AMOUNT_OF_PIPES 10;

queueADT filesQueue;
queueADT hashedFilesQueue;
int pidSlaves[MAX_AMOUNT_OF_SLAVES];
char buffer[256];

int main(int argc, char const *argv[])
{
  if(argc < 2)
  {
    printf("You haven't passed a valid pattern\n");
    return 1;
  }

  filesQueue = createQueue();
  hashedFilesQueue = createQueue();

  for (int argIndex = 1; argIndex < argc; argIndex++)
  {
    enqueueFile(argv[argIndex], filesQueue);
  }


  int applicationPID = getpid();

  int pidChild;
  for (int index = 0; index < MAX_AMOUNT_OF_SLAVES; index++) {
    switch (pidChild = fork()) {
      case -1:
        perror("Fork failed\n");
        abort();
        break;
      case 0:
        //exev slave;
        break;
      default:

        pidSlaves[index] = pidChild;
        int fd;
        char * parentSlavePipe = sprintf( "/tmp/%d%d" , applicationPID , pidChild);
        char * slaveParentPipe = sprintf( "/tmp/%d%d" , pidChild, applicationPID);

        break;
    }
  }

  return 0;
}

void makeFileQueue(char * path, queueADT queue)
{
	DIR * d = opendir(path);
	struct dirent * dir = readdir(d);
  if(dir != NULL) {
    if(dir-> d_type != DT_DIR)
    {
        char d_path[MAX_NAMEPATH];
        sprintf(d_path, "%s/%s", path, dir->d_name);
        enqueue(d_path, queue);
    }
  }
}
