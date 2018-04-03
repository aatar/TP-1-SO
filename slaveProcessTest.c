#include <stdio.h>
#include "testLib.h"
#include "slaveProcess.h"
#include "queue.h"

void testHashMD5OneFilePath();
void testHashMD5MultipleFilesPaths();

void givenAFilePath();
void givenMultipleFilesPaths();
void givenAQueue();

void whenHashMD5OfTheFileIsCalculated();
void whenMultipleFilesHashesAreCalculated();

void thenTheHashMD5IsReturned();
void thenTheHashesMD5AreReturned();

queueADT queue;
char * testingFilesPaths[] = {"file1.txt", "file2.txt", "file3.txt", "file4.txt", "file5.txt"};
char * fileName;

char * calculatedMD5Hash;
char * calculatedMD5Hashes[5];
char * expectedMD5Hashes[] = {"4d186321c1a7f0f354b297e8914ab240",
							 "cdcf8d0131420ccecc2e769f5db62e1b",
							 "61938292ca4cf3be12141ab6bc01c1d3",
							 "1683e3c47cf7a46dc308a70ca7d016c2",
							 "f130f9b486ae8f768161a710163ac80e"};

int main()
{
	printf("Test MD5 Hash for one file path: \n");
	testHashMD5OneFilePath();

	printf("Test MD5 Hash for multiple files paths: \n");
	testHashMD5MultipleFilesPaths();
}


void testHashMD5OneFilePath()
{
	givenAFilePath();
	whenHashMD5OfTheFileIsCalculated();
	thenTheHashMD5IsReturned();
}

void testHashMD5MultipleFilesPaths()
{
	givenAQueue();
	givenMultipleFilesPaths();
	whenMultipleFilesHashesAreCalculated();
	thenTheHashesMD5AreReturned();
}


void givenAFilePath()
{
	fileName = testingFilesPaths[0];
}

void givenAQueue()
{
	queue = createQueue();
}

void givenMultipleFilesPaths()
{
	for(int filePathIndex = 0; filePathIndex < 5; filePathIndex++)
	{
		enqueue(testingFilesPaths[filePathIndex], queue);
	}
}


void whenHashMD5OfTheFileIsCalculated()
{
	calculatedMD5Hash = calculateFileMD5Hash(fileName);
}

void whenMultipleFilesHashesAreCalculated()
{
	char * file;
	char * hashMD5Calculated;
	int i = 0;
	while(!isEmpty(queue))
	{
		file = dequeue(queue);
		hashMD5Calculated = calculateFileMD5Hash(file);
		calculatedMD5Hashes[i] = hashMD5Calculated;
		i++;
	}
}


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
	for(int hashIndex = 0; hashIndex < 5; hashIndex++)
	{
		if(!stringsAreEquals(expectedMD5Hashes[hashIndex], calculatedMD5Hashes[hashIndex]))
		{
			fail("One of the expected MD5 Hashes is different from the calculated one \n");
			return;
		}
	}
	ok();
}
