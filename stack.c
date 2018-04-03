// typedef struct
// {
//     char * path;
//     struct stackNodeCDT *next;
// } stackNodeCDT;
//
// void push(char * item)
// {
//     stackNodeCDT * nptr = malloc(sizeof(stackNodeCDT));
//     nptr->path = item;
//     nptr->next = top;
//     top = nptr;
// }
//
// int isEmpty()
// {
// 	return top == NULL;
// }
//
// char * pop()
// {
//     stackNodeCDT * temp;
//     temp = top;
//     top = top->next;
//     char * resp = temp->path;
//     free(temp);
//     return resp;
// }
