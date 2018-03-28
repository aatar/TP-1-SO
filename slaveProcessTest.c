#include <stdio.h>
#include "testLib.h"
#include "slaveProcess.h"
#include "queueOfFiles.h"

void testHashMD5OneFile();
void testHashMD5MultipleFiles();

void givenAFile();
void givenMultipleFiles();
void givenAQueue();

void whenHashMD5OfTheFileIsCalculated();
void whenMultipleFilesHashesAreCalculated();

void thenTheHashMD5IsReturned();
void thenTheHashesMD5AreReturned();

#define SIZE 5
#define MAXSIZE 10

queueADT queue;
char* testingFiles[] = {"file1.txt", "file2.txt", "file3.txt", "file4.txt", "file5.txt"};
char* fileName;

char* calculatedMD5Hash;
char* calculatedMD5Hashes[SIZE];
char* expectedMD5Hashes[] = {"4d186321c1a7f0f354b297e8914ab240", 
							 "cdcf8d0131420ccecc2e769f5db62e1b", 
							 "61938292ca4cf3be12141ab6bc01c1d3",
							 "1683e3c47cf7a46dc308a70ca7d016c2", 
							 "f130f9b486ae8f768161a710163ac80e"};

int main()
{
	printf("Test MD5 Hash for one file: \n");
	testHashMD5OneFile();

	printf("Test MD5 Hash for multiple files: \n");
	testHashMD5MultipleFiles();
}


///TEST//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void testHashMD5OneFile()
{
	givenAFile();

	whenHashMD5OfTheFileIsCalculated();

	thenTheHashMD5IsReturned();
}

void testHashMD5MultipleFiles()
{	
	givenAQueue();
	givenMultipleFiles();

	whenMultipleFilesHashesAreCalculated();

	thenTheHashesMD5AreReturned();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///GIVEN/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void givenAFile()
{
	fileName = testingFiles[0];
}

void givenAQueue()
{
	queue = createQueue(MAXSIZE);
}

void givenMultipleFiles()
{
	int i;
	for(i = 0; i < SIZE; i++)
	{
		enqueueFile(testingFiles[i], queue);
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///WHEN//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void whenHashMD5OfTheFileIsCalculated()
{
	calculatedMD5Hash = calculateFileMD5Hash(fileName);
}

void whenMultipleFilesHashesAreCalculated()
{
	char* file;
	char* hashMD5Calculated;
	int i = 0;

	while(!isEmpty(queue))
	{
		file = dequeueFile(queue);
		hashMD5Calculated = calculateFileMD5Hash(file);
		calculatedMD5Hashes[i] = hashMD5Calculated;
		i++;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///THEN//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void thenTheHashMD5IsReturned()
{
	if(stringsAreEquals(expectedMD5Hashes[0], calculatedMD5Hash))
	{
		ok();
	}
	else
	{
		fail("Expected MD5 Hash is different from the calculated one \n");
	}
}

void thenTheHashesMD5AreReturned()
{
	int i;

	for(i = 0; i < SIZE; i++)
	{
		if(!stringsAreEquals(expectedMD5Hashes[i], calculatedMD5Hashes[i]))
		{
			fail("One of the expected MD5 Hashes is different from the calculated one \n");
			return;
		}
	}
	ok();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////