#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <signal.h>

#define SHARED_MEMORY_PATH "/shm"
#define MAXIMUM_DATA_LENGTH 290

void signal_callback_handler(int signum)
{
        if ( shm_unlink("/mysem") < 0 )
        {
                perror("shm_unlink");
        }
   exit(signum);
}

int main(int argc, char *argv[]) {
    int shmfd;
    int shared_seg_size = MAXIMUM_DATA_LENGTH;
    char * data;


    signal(SIGINT, signal_callback_handler);

    /* creating the shared memory object    --  shm_open()  */
    shmfd = shm_open(SHMOBJ_PATH, O_CREAT | O_RDWR, S_IRWXU | S_IRWXG);
    if (shmfd < 0)
    {
        perror("In shm_open()");
        exit(1);
    }

    fprintf(stderr, "Created shared memory object %s\n", SHMOBJ_PATH);

    ftruncate(shmfd, shared_seg_size);

    /* requesting the shared segment    --  mmap() */
    data = (char *)mmap(NULL, shared_seg_size, PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);
    if (data == NULL)
    {
        perror("In mmap()");
        exit(1);
    }

    while(1)
    {
      printf("%s\n",data);
      sleep(2);
    }

    if (shm_unlink(SHARED_MEMORY_PATH) != 0)
    {
        perror("In shm_unlink()");
        exit(1);
    }

    return 0;
}
