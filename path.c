/*
Meta Math
Elijah Delavar
main.c quiz.c quiz.h

TODO: Description
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "path.h"

char *playerPath = NULL;

char *pathFN = "./GameFiles/Path.txt";
char *possiblePathsFN = "./GameFiles/PossiblePaths.csv";

State *stateStart = NULL;
State *stateEnd = NULL;

void savePlayerPath();      // TODO
State *freeST(State*);
void freeSTDoublyLinkedList();
State *getState(char*);
State *parseState(char*);
int readPathCSV();
int setState(Prompt*);      // TODO

/*
TODO
*/
void savePlayerPath() {
    if (playerPath) {
        free(playerPath);
    }
}

/*
Free state's character arrays.
Free state.

@param st: state to free
@return next state to free
*/
State* freeST(State *st) {
    dputs("Freeing State");
    if (st) {
        if (st->title) {
            dputs("freeing title...");
            free(st->title);
        }
        if (st->path) {
            dputs("freeing path...");
            free(st->path);
        }
        if (st->val) {
            dputs("freeing val...");
            free(st->val);
        }
        State *next = st->next;
        dputs("freeing st...\n");
        free(st);
        return next;
    }
    puts("!! Attempted to Free Null State !!");
    return NULL;
}

/*
Free all states in the doubly linked list.
*/
void freeSTDoublyLinkedList() {
    State *cur = stateStart;
    while (cur) {
        cur = freeST(cur);
    }
}

/*
Loop through state DLL to find the state with matching title.

@return matching state or NULL
*/
State *getState(char *title) {
    State *match = NULL;
    State *cur = stateStart;
    while (cur) {
        if (!strcmp(cur->title, title)) {
            // found same title
            match = cur;
        }
        cur = cur->next;
    }
    return match;
}

/*
Parse the given line into a State struct.
*/
State *parseState(char *line) {
    // Init State
    State *st = NULL;
    st = (State*)malloc(sizeof(State));
    if (!st) {
        puts("!!! Failed to Allocate Memory !!!");
        return NULL;
    }

    // Init State attributes
    st->title = NULL;
    st->path = NULL;
    st->val = NULL;
    st->next = NULL;
    st->prev = NULL;

    // Title
    char *delim = "|";
    char *token = NULL;
    token = strtok(line, delim);
    if (!token) {
        puts("! State Title Not Found !");
        free(st);
        return NULL;
    }
    int lentok = strlen(token) + 1;
    st->title = (char*)malloc(sizeof(char)*lentok);
    if (!st->title) {
        puts("!!! Failed to Allocate Memory !!!");
        free(st);
        return NULL;
    }
    strncpy(st->title, token, lentok);

    // Path
    token = strtok(NULL, delim);
    if (!token) {
        puts("! State Path Not Found !");
        freeST(st);
        return NULL;
    }
    lentok = strlen(token) + 1;
    st->path = (char*)malloc(sizeof(char)*lentok);
    if (!st->path) {
        puts("!!! Failed to Allocate Memory !!!");
        freeST(st);
        return NULL;
    }
    strncpy(st->path, token, lentok);

    // Val
    token = strtok(NULL, delim);
    if (!token) {
        puts("! State Val Not Found !");
        freeST(st);
        return NULL;
    }
    lentok = strlen(token) + 1;
    st->val = (char*)malloc(sizeof(char)*lentok);
    if (!st->val) {
        puts("!!! Failed to Allocate Memory !!!");
        freeST(st);
        return NULL;
    }
    strncpy(st->val, token, lentok);
    
    return st;
}

/*
Read the player's path and record it in the given character array.
Player's path is separated by newlines.
Read the possible paths and record each one in a State struct.
Possible paths are separated by commas and pipes.
Both character arrays need to be malloc'd.
*/
int readPathCSV() {
    // Player Path
    FILE *fp = NULL;
    fp = fopen(pathFN, "r");
    if (!fp) {
        puts("! File Not Found !");
        return -1;
    }

    // Read in player path and convert to comma separated values
    int MAXLEN = 10;
    char buffer[MAXLEN];

    while (fgets(buffer, MAXLEN, fp)) {
        int nl = strlen(buffer) - 1;

        if (buffer[nl] == '\r') {
            buffer[nl] = '\0';
        } else if (buffer[nl] == '\n') {
            // Add to player path
            if (nl-1 >= 0 && buffer[nl-1] == '\r') {
                buffer[nl-1] = ',';
                buffer[nl] = '\0';
            } else {
                buffer[nl] = ',';
                buffer[nl+1] = '\0';
            }

            nl += 2;

            // malloc player path
            if (!playerPath) {
                playerPath = (char*)malloc(sizeof(char)*nl);
                if (!playerPath) {
                    puts("!!! Failed to Allocate Memory !!!");
                    fclose(fp);
                    return -1;
                }
                playerPath[0] = '\0';
            } else {
                char *temp = NULL;
                temp = (char*)realloc(playerPath, sizeof(char)*(strlen(playerPath)+1+nl));
                if (!temp) {
                    puts("!!! Failed to Allocate Memory !!!");
                    fclose(fp);
                    return -1;
                }
                playerPath = temp;
            }

            // concatenate buffer to player path
            strcat(playerPath, buffer);
        } else {
            puts("!! Invalid Title on Player Path !!");
            fclose(fp);
            return -1;
        }
    }

    fclose(fp);

    // Possible Paths
    fp = NULL;
    fp = fopen(possiblePathsFN, "r");
    if (!fp) {
        puts("! File Not Found !");
        return -1;
    }

    // Read in saved state/value pairs
    MAXLEN = 512;
    char buffer2[MAXLEN];

    while (fgets(buffer2, MAXLEN, fp)) {
        int nl = strlen(buffer2) - 1;
        if (buffer2[nl] == '\r') {
            buffer2[nl] = '\0';
        } else if (buffer2[nl] == '\n') {
            // Add to State DLL
            buffer2[nl] = '\0';
            if (nl-1 >= 0 && buffer2[nl-1] == '\r') {
                buffer2[nl-1] = '\0';
            }

            nl += 2;

            // Create new State
            State *toLoad = NULL;
            toLoad = parseState(buffer2);
            if (!toLoad) {
                fclose(fp);
                return -1;
            }

            // Add State to DLL
            if (!stateStart) {
                stateStart = toLoad;
            } else {
                stateEnd->next = toLoad;
                toLoad->prev = stateEnd;
            }
            stateEnd = toLoad;
        }
    }

    fclose(fp);

    return 0;
}

/*
Check for State title in DLL that matches Prompt title.
If the State path matches the player's path,
    set the Prompt's description to the State's val.
*/
int setState(Prompt *pt) {
    if (!pt) {
        puts("! NULL Prompt !");
        return -1;
    }

    State *cur = stateStart;
    while (cur) {
        if (!strcmp(cur->title, pt->title)) {
            if (!strcmp(cur->path, playerPath)) {
                // player path equals state path
                int lenval = strlen(cur->val) + 1;
                // resize description if need be
                if (lenval > strlen(pt->description)+1) {
                    char *temp = (char*)realloc(pt->description, sizeof(char)*lenval);
                    if (!temp) {
                        puts("!!! Failed to Allocate Memory !!!");
                        return -1;
                    }
                    pt->description = temp;
                }
                strncpy(pt->description, cur->val, lenval);
                break;
            }
        }
        cur = cur->next;
    }
    return 0;
}