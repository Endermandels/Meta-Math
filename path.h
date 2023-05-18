/*
Meta Math
Elijah Delavar
main.c quiz.c quiz.h

TODO: Description
*/

#ifndef PATH
#define PATH

typedef struct STATE {
    char *title;
    char *path;
    char *val;      // value of state if path matches playerPath
    struct STATE *next;
    struct STATE *prev;
}State;

void freeSTDoublyLinkedList(State*);
int readPathCSV(char**, State**, State**);

#endif