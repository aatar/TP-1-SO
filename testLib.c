#include <stdio.h>
#include <string.h>
#include "testLib.h"

void ok()
{
	printf("Ok!\n");
}

void fail(char * message)
{
	fprintf(stderr, "%s\n", message);
}

int stringsAreEquals(char * aString, char * anotherString)
{
	return strcmp(aString, anotherString) == 0;
}

void checkIsNotNull(void * pointer)
{
	if (pointer == NULL)
	{
		fail("	Expected: a non null pointer, got: a null pointer");
	}
	else
	{
		ok();
	}
}
