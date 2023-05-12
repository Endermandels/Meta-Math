/*
Meta Math
Elijah Delavar
main.c quiz.c quiz.h

TODO: Description

NOTES:
    Positive errors are intentional
    Negative errors are unintentional
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "quiz.h"
#define DEBUG 1

// Prompt Doubly Linked List
Prompt *start = NULL;
Prompt *end = NULL;
int size = 0;

void dputs(const char*);
void clearStream(FILE*);
int loadPrompts();
int loadPrompt();
int beginGame();
Prompt *getPrompt(const char*);
int userInput(char*, FILE*);
int quit(int);

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
Free prompt's character arrays.
Free prompt.

@param pt: prompt to free
@return next prompt to free
*/
Prompt* freePT(Prompt *pt) {
    dputs("Freeing Prompt");
    if (pt) {
        if (pt->description) {
            dputs("freeing description...");
            free(pt->description);
        }
    }
    Prompt *next = pt->next;
    dputs("freeing pt...\n");
    free(pt);
    return next;
}

/*
Free all prompts in the doubly linked list.
*/
void freePTDoublyLinkedList() {
    Prompt *cur = start;
    while (cur) {
        cur = freePT(cur);
    }
}

int loadPrompts() {
    // TODO: Implement File Reading
    // TODO: Implement Prompt String Parsing
    Option introOpts[OPTIONCOUNT];
    introOpts[0].answer = "_";
    introOpts[0].goToTitle = "Q1";
    char *title = "Intro";
    char *desc = "Welcome to Meta Math Quiz.  This quiz will help you in your journey to learn how to do basic mathematical operations such as addition, subtraction, multiplication, division, raising to an exponent, taking a natural logarithm, taking a derivative, taking your sanity, taking an integral, and taking a square-root.  Fully simplify each of your answers, otherwise I will not accept your answers.  This course will be divided into three sections: Basic Math, Slightly Less Basic Math, and Basic Math for an Intelligent Computer Program.  Your results will be recorded.  Do NOT tamper with them.\n\nLet us get started.";
    if (loadPrompt(title, desc, introOpts)) {
        return -1;
    }
    Option q1opts[OPTIONCOUNT];
    q1opts[0].answer = "2";
    q1opts[0].goToTitle = "Q2";
    q1opts[1].answer = "_";
    q1opts[1].goToTitle = "TryAgain";
    char *title2 = "Q1";
    char *desc2 = "\nWhat is 1+1?";
    if (loadPrompt(title2, desc2, q1opts)) {
        return -1;
    }
    return 0;
}

int loadPrompt(char *title, char *description, Option options[OPTIONCOUNT]) {
    Prompt *toLoad = (Prompt*)malloc(sizeof(Prompt));
    if (!toLoad) {
        puts("!!! Failed to Allocate Memory !!!");
        return -1;
    }
    toLoad->description = NULL;
    toLoad->next = NULL;
    toLoad->prev = NULL;

    // Title
    strncpy(toLoad->title, title, strlen(title)+1);

    // Description
    toLoad->description = (char*)malloc(sizeof(char)*(strlen(description)+1));
    if (!toLoad->description) {
        puts("!!! Failed to Allocate Memory !!!");
        return -1;
    }
    strncpy(toLoad->description, description, strlen(description)+1);

    // Options
    for (int ii = 0; ii < OPTIONCOUNT; ii++){
        toLoad->options[ii] = options[ii];
    }

    // Link to list
    if (!start) {
        start = toLoad;
    } else {
        end->next = toLoad;
        toLoad->prev = end;
    }
    end = toLoad;
    return 0;
}

/*
First run of game.
Prompts are loaded in by this point.
Loop through prompts starting at Intro.
*/
int beginGame() {
    char goToTitle[80];
    strncpy(goToTitle, "Intro", 6);
    Prompt *cur = getPrompt(goToTitle);

    while (cur) {
        // Description
        puts(cur->description);
        // Answer
        if (!strcmp(cur->options[0].answer, "_")) {
            // No answer required
            strncpy(goToTitle, cur->options[0].goToTitle, strlen(cur->options[0].goToTitle)+1);
            clearStream(stdin);
        } else {
            char answer[1000];
            int testEOF = userInput(answer, stdin);
            while (testEOF) {
                puts("! Invalid Answer !");
                testEOF = userInput(answer, stdin);
            }
            // Options
            for (int ii = 0; ii < OPTIONCOUNT; ii++) {
                if (!strcmp(cur->options[ii].answer, "_")) {
                    // End of options
                    strncpy(goToTitle, cur->options[ii].goToTitle, strlen(cur->options[ii].goToTitle)+1);
                    break;
                }
                if (!strcmp(cur->options[ii].answer, answer)) {
                    // User answer matches one of the options
                    strncpy(goToTitle, cur->options[ii].goToTitle, strlen(cur->options[ii].goToTitle)+1);
                    break;
                }
            }
        }
        // Repeat
        cur = getPrompt(goToTitle);
    }
    return 0;
}

/*
Loop through prompt DLL to find the prompt with matching title.
Return prompt (NULL if none found).
*/
Prompt *getPrompt(const char *title) {
    Prompt *match = NULL;
    Prompt *cur = start;
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
Receive user's input and copy it into dest.
Return status.
*/
int userInput(char *dest, FILE *fp) {
    // read in string
    char buffer[1000];
    int testEOF = scanf("%999[^\n\r]s", buffer);
    clearStream(fp);
    if (testEOF <= 0) {
        return -1;
    }
    int lenBuffer = strlen(buffer);
    buffer[lenBuffer+1] = '\0';
    strncpy(dest, buffer, lenBuffer+1);
    return 0;
}


int quit(int status) {
    // free prompts
    freePTDoublyLinkedList();
    if (status) {
        exit(status);
    }
    return status;
}

