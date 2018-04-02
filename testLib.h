#ifndef TEST_LIB_H
#define TEST_LIB_H

void ok();
void fail(char* message);
int stringsAreEquals(char * aString, char * anotherString);
void checkIsNotNull(void * pointer);

#endif
