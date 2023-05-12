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

void dputs(const char*);
void clearStream(FILE*);
int loadPrompts();
int loadPrompt(char*, char*, Option*);
int parseLine(char*);
int parseOptions(char*, Option*);
int parseOptionEquation(char*, Option*, int);
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
Receive user's input and copy it into dest.
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

/*
Loop through prompt DLL to find the prompt with matching title.

@return matching prompt or NULL
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
Append a prompt to the DLL given the title, description and options.
*/
int loadPrompt(char *title, char *description, Option *options) {
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
        if (!strcmp(options[ii].answer, "_")) {
            // stop loading options
            toLoad->optionsUsed = ii+1;
            break;
        }
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
Parse the third section of the CSV into answer and goToTitle pairs.

@param  line Third section of the CSV.
@param  options Structure to store answer and goToTitle pairs.
*/
int parseOptions(char *line, Option *options) {
    // delimits set to ';' and '='
    const char *delim = ";=";
    char *token = strtok(line, delim);
    int ii = 0;
    
    while (token) {
        if (ii >= OPTIONCOUNT) {
            puts("!! Too Many Options !!");
            return -1;
        }

        // Answer
        strncpy(options[ii].answer, token, strlen(token)+1);
        token = strtok(NULL, delim);

        // GoToTitle
        strncpy(options[ii].goToTitle, token, strlen(token)+1);
        token = strtok(NULL, delim);
        ii++;
    }
    return 0;
}

/*
Take in a line (no newline at the end, NULL terminated).
A line is defined as all three pipe separated strings.
Parse the line into title, description and options.
Checks that each line has the proper formatting.
Load prompt.
*/
int parseLine(char *line) {
    char title[80];
    char *description;
    Option options[OPTIONCOUNT];

    const char *delim = "|";
    char *token = strtok(line, delim);
    int ii = 0;

    while (token && ii < 3) {
        if (ii == 0) {
            // Title
            int lenTok = strlen(token);
            if (lenTok > 79) {
                strncpy(title, token, 79);
                title[79] = '\0';
            } else {
                strncpy(title, token, strlen(token)+1);
            }
        } else if (ii == 1) {
            // Description
            description = token;
        } else if (ii == 2) {
            // Options
            // Parse token
            if (parseOptions(token, options)) {
                return -1;
            }
        }
        // Retrieve next token
        token = strtok(NULL, delim);
        ii++;
    }

    if (loadPrompt(title, description, options)) {
        return -1;
    }
    return 0;
}

/*
TODO: Description
*/
int loadPrompts() {
    // TODO: Implement File Reading

    char *str = "Intro|Welcome to Meta Math Quiz.  This quiz will help you in your journey to learn how to do basic mathematical operations such as addition, subtraction, multiplication, division, raising to an exponent, taking a natural logarithm, taking a derivative, taking your sanity, taking an integral, and taking a square-root.  Fully simplify each of your answers, otherwise I will not accept your answers.  This course will be divided into three sections: Basic Math, Slightly Less Basic Math, and Basic Math for an Intelligent Computer Program.  Your results will be recorded.  Do NOT tamper with them.\n\nLet us begin.|_=Q1";
    char *malStr = NULL;
    malStr = (char*)malloc(sizeof(char)*(strlen(str)+1));
    if (!malStr) {
        puts("!!! Failed to Allocate Memory !!!");
        return -1;
    }
    strncpy(malStr, str, strlen(str)+1);
    if (parseLine(malStr)) {
        free(malStr);
        return -1;
    }
    free(malStr);

    str = "Q1|What is 1+1?|2=Q2;_=TryAgain";
    malStr = NULL;
    malStr = (char*)malloc(sizeof(char)*(strlen(str)+1));
    if (!malStr) {
        puts("!!! Failed to Allocate Memory !!!");
        return -1;
    }
    strncpy(malStr, str, strlen(str)+1);
    if (parseLine(malStr)) {
        free(malStr);
        return -1;
    }
    free(malStr);

    str = "Q2|What is 3+26?|29=Q3;_=TryAgain";
    malStr = NULL;
    malStr = (char*)malloc(sizeof(char)*(strlen(str)+1));
    if (!malStr) {
        puts("!!! Failed to Allocate Memory !!!");
        return -1;
    }
    strncpy(malStr, str, strlen(str)+1);
    if (parseLine(malStr)) {
        free(malStr);
        return -1;
    }
    free(malStr);
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
            for (int ii = 0; ii < cur->optionsUsed; ii++) {
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
Quit the game.
Handle error status.
*/
int quit(int status) {
    // free prompts
    freePTDoublyLinkedList();
    if (status) {
        exit(status);
    }
    return status;
}
