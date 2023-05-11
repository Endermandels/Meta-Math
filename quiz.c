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
#define DEBUG 0

// Prompt Doubly Linked List
Prompt *start = NULL;
Prompt *end = NULL;
int size = 0;

void dputs(const char*);
void clearStream(FILE*);
int loadPrompts();
int loadPrompt();
int beginGame();
char *getTitleDesc(const char*);
int quit();

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
    dputs("freeing rt...\n");
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
    Option introOpts[OPTIONCOUNT];
    introOpts[0].answer = "_";
    introOpts[0].goToTitle = "Q1";
    if (loadPrompt("Intro", "Welcome to Meta Math Quiz.  This quiz will help you in your journey to learn how to do basic mathematical operations such as addition, subtraction, multiplication, division, raising to an exponent, taking a natural logarithm, taking a derivative, taking your sanity, taking an integral, and taking a square-root.  Fully simplify each of your answers, otherwise I will not accept your answers.  This course will be divided into three sections: Basic Math, Slightly Less Basic Math, and Basic Math for an Intelligent Computer Program.  Your results will be recorded.  Do NOT tamper with them.\n\nLet us get started.", introOpts)) {
        return -1;
    }
    return 0;
}

int loadPrompt(char *title, char *description, Option options[OPTIONCOUNT]) {
    Prompt *intro = (Prompt*)malloc(sizeof(Prompt));
    if (!intro) {
        puts("!!! Failed to Allocate Memory !!!");
        return -1;
    }
    intro->description = NULL;
    intro->next = NULL;
    intro->prev = NULL;

    // Title
    strncpy(intro->title, title, strlen(title)+1);

    // Description
    intro->description = (char*)malloc(sizeof(char)*(strlen(description)+1));
    if (!intro->description) {
        puts("!!! Failed to Allocate Memory !!!");
        return -1;
    }
    strncpy(intro->description, description, strlen(description)+1);

    // Options
    for (int ii = 0; ii < OPTIONCOUNT; ii++){
        intro->options[ii] = options[ii];
    }

    start = intro;
    end = intro;
    return 0;
}

/*
First run of game.
Prompts are loaded in by this point.
*/
int beginGame() {
    char *desc = getTitleDesc("Intro");
    if (desc) puts(desc);
    return 0;
}

/*
Loop through prompt DLL to find the prompt with matching title.
Return title description (NULL if none found).
*/
char *getTitleDesc(const char *title) {
    char *description = NULL;
    Prompt *cur = start;
    while (cur) {
        if (!strcmp(cur->title, title)) {
            // found same title
            description = cur->description;
        }
        cur = cur->next;
    }
    return description;
}

/*
Ask for input.
Store valid input in Answers list.
*/
int addAnswer(short isNumerical, FILE *fp) {
    // read in string
    char buffer[100];
    int testEOF = scanf("%99[^\n\r]s", buffer);
    if (testEOF <= 0) {
        // Error
        clearStream(fp);
        return 1;
    }
    int lenBuffer = strlen(buffer);
    buffer[lenBuffer+1] = '\0';

    return 0;
}


int quit() {
    // free prompts
    freePTDoublyLinkedList();
    return 0;
}

