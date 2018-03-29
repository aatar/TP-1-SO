//ApplicationTest
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h> 
#include <stdio.h> 
#include <string.h>

#define NULL ((void *)0)

void testSearchFileAndList();


void givenAFile();
void whenFileFound();
void thenAddToList();

char * examplePath=NULL;
DIR * file=NULL;

struct 
{
	/* data */
};


int main(int args, char *argv[])
{
	printf("Test Application: ");
	testTakeADirectory();


// ImpletarQueue
	testAddAFileToFIFO();

	testWriteOnFds();

	testSaveOnDisk();

	testManageState();

	testKillChilds();
}


void testTakeADirectory()
{
	givenAPath();

	whenFileInPathFound();

	thenVerifyIsADirectory();
}


void givenAPath()
{
	examplePath=".";
}

void whenFileInPathFound()
{
	file=opendir(examplePath);

}

void thenVerifyIsADirectory (){
	if (file==NULL){
		perror("Zaraza");
	}

	struct dirent *dir;
	dir=readdir(file);
	

	if (dir!=NULL){
		if (isDirectory(dir)){
			perror("ZARAZA");
		}
	}

}


int isDirectory(dirent *dir){
	return (dir->d_type==DT_DIR)
}