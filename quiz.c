/*
Meta Math
Elijah Delavar
main.c quiz.c quiz.h

TODO: Description

TODOS:
    Read startup state from file
    Save game state (automatically)

*/

#include <stdio.h>
#include <string.h>
#include "quiz.h"
#define DEBUG 1


int size = 0;       // size of Answers list
Answers *start;     // start of Answers list
Answers *end;       // end of Answers list

int beginGame();
int addAnswer(const char*, FILE*);
int removeAnswer(int);
void clearStream(FILE*);
void dputs(const char*);

/*
Debug puts.
*/
void dputs(const char *string) {
    #if DEBUG
        puts(string);
    #endif
}

/*
Clear file stream until newline or return character.
*/
void clearStream(FILE *fp) {
    dputs("Clearing stream...");
    char ch = fgetc(fp);
    while (ch != '\n' && ch != '\r') {
        dputs("Clearing stream...");
        ch = fgetc(fp);
    }
}

/*
First run of game.
*/
int beginGame() {
    const char *intro = "Welcome to Meta Math Quiz.\n\nQuestion 1:  What is 12*5?";
    puts(intro);

    int error = addAnswer("int", stdin);
    printf("%d\n", error);

    printf("Enter a test string: ");
    error = addAnswer("string", stdin);
    printf("%d\n", error);

    return 0;
}

/*
Ask for input.
Store valid input in Answers list.
*/
int addAnswer(const char *mode, FILE *fp) {
    // char *answer = NULL;
    int numAnswer = 0;

    // strstr checks if left arg contains right arg
    if (strstr(mode, "int")) {
        // read in int
        int testEOF = scanf("%d", &numAnswer);
        if (testEOF <= 0) {
            // Error
            return 1;
        }
        // clear stream
        clearStream(fp);
    }
    if (strstr(mode, "string")) {
        // read in string
        char buffer[100];
        int testEOF = scanf("%99[^\n\r]s", buffer);
        if (testEOF <= 0) {
            // Error
            return 1;
        }
        int lenBuffer = strlen(buffer);
        buffer[lenBuffer+1] = '\0';
    }

    // Add results to Answers list

    return 0;
}

int removeAnswer(int index) {
    return 0;
}

