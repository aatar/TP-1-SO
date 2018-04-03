#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_SIZE 1000
#define FILES_COUNT 5

char * testingFilesPaths[] = {"file1.txt", "file2.txt", "file3.txt", "file4.txt", "file5.txt"};

char * calculatedMD5Hashes[] = {"4d186321c1a7f0f354b297e8914ab240",
							 "cdcf8d0131420ccecc2e769f5db62e1b",
							 "61938292ca4cf3be12141ab6bc01c1d3",
							 "1683e3c47cf7a46dc308a70ca7d016c2",
							 "f130f9b486ae8f768161a710163ac80e"};

int main()
{
    /* Variable to store user content */
    char data[DATA_SIZE];
	
	/* File pointer to hold reference to our file */
    FILE * fPtr;


    /* 
     * Open file in w (write) mode. 
     * "data/file1.txt" is complete path to create file
     */
    fPtr = fopen("fileOutput.txt", "w");


    /* fopen() return NULL if last operation was unsuccessful */
    if(fPtr == NULL)
    {
        /* File not created hence exit */
        printf("Unable to create file.\n");
        exit(EXIT_FAILURE);
    }


    /* Input contents from user to store in file */
	char * s = "\n";
	strcpy(data,"");
    int i;
	for(i = 0; i < FILES_COUNT; i++) {
		strcat(data, testingFilesPaths[i]);
		strcat(data, " ");
		strcat(data, calculatedMD5Hashes[i]);
		strcat(data, "\n");
	}
	
	/* Write contents to file */
    fputs(data, fPtr);


    /* Close file to save contents */
    fclose(fPtr);


    /* Success message */
    printf("File created and saved successfully.  \n");


    return 0;
}