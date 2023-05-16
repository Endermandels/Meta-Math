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
#include <ctype.h>
#include "quiz.h"
#define DEBUG 0

// Quiz File Pointer
FILE *q1 = NULL;
char *q1filename = "./GameFiles/Quiz1.csv";

// Prompt Doubly Linked List
Prompt *start = NULL;
Prompt *end = NULL;

// Function Prototypes

void dputs(const char*);
void clearStream(FILE*);
int lowerAnswer(char*);
int userInput(char*, FILE*);
Prompt *freePT(Prompt*);
void freePTDoublyLinkedList();
Prompt *getPrompt(char*);
int loadPrompt(char*, char*, Option*);
int parseOptions(char*, Option*);
int parseLine(char*);
int readCSV();
int beginGame();
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
Change the user's input answer to all lowercase.
Answer must be null terminated.
*/
int lowerAnswer(char *answer) {
    int ii = 0;
    while (answer[ii]) {
        answer[ii] = tolower(answer[ii]);
        ii++;
    }
    return 0;
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
    lowerAnswer(buffer);
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
        if (pt->title) {
            dputs("freeing title...");
            free(pt->title);
        }
        if (pt->description) {
            dputs("freeing description...");
            free(pt->description);
        }
        if (pt->options) {
            dputs("freeing options...");
            for (int ii = 0; ii < pt->optionsUsed; ii++) {
                if (pt->options[ii].answer) {
                    free(pt->options[ii].answer);
                }
                if (pt->options[ii].goToTitle) {
                    free(pt->options[ii].goToTitle);
                }
            }
        }
        Prompt *next = pt->next;
        dputs("freeing pt...\n");
        free(pt);
        return next;
    }
    puts("!! Attempt to Free Null Prompt !!");
    return NULL;
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
Prompt *getPrompt(char *title) {
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
    toLoad->title = NULL;
    toLoad->description = NULL;
    toLoad->next = NULL;
    toLoad->prev = NULL;

    // Title
    int lenTitle = strlen(title) + 1;
    toLoad->title = (char*)malloc(sizeof(char)*lenTitle);
    if (!toLoad->title) {
        puts("!!! Failed to Allocate Memory !!!");
        return -1;
    }
    strncpy(toLoad->title, title, strlen(title)+1);

    // Description
    int lenDesc = strlen(description) + 1;
    toLoad->description = (char*)malloc(sizeof(char)*lenDesc);
    if (!toLoad->description) {
        puts("!!! Failed to Allocate Memory !!!");
        return -1;
    }
    strncpy(toLoad->description, description, lenDesc);

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

void freeOptions(Option *options) {
    for (int ii = 0; ii < OPTIONCOUNT; ii++) {
        if (options[ii].answer) {
            free(options[ii].answer);
        }
        if (options[ii].goToTitle) {
            free(options[ii].goToTitle);
        }
    }
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

        options[ii].answer = NULL;
        options[ii].goToTitle = NULL;

        // Answer
        int lenAnswer = strlen(token)+1;
        options[ii].answer = (char*)malloc(sizeof(char)*lenAnswer);
        if (!options[ii].answer) {
            puts("!!! Failed to Allocate Memory !!!");
            freeOptions(options);
            return -1;
        }
        strncpy(options[ii].answer, token, lenAnswer);
        token = strtok(NULL, delim);

        // GoToTitle
        int lenGoTo = strlen(token)+1;
        options[ii].goToTitle = (char*)malloc(sizeof(char)*lenGoTo);
        if (!options[ii].goToTitle) {
            puts("!!! Failed to Allocate Memory !!!");
            freeOptions(options);
            return -1;
        }
        strncpy(options[ii].goToTitle, token, lenGoTo);
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
            strncpy(title, token, 79);
            title[79] = '\0';
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
Read a CSV formatted for this game.
Feed in appropriate pipe separated values into the parseLine function.
*/
int readCSV() {
    // Open file
    FILE *fp = fopen(q1filename, "r");
    if (!fp) {
        puts("! File Not Found !");
        return -1;
    }
    q1 = fp;

    // Read in line by line
    // Store each buffer in a dynamically allocated string until three "|"s have been recorded.
    int MAX_LEN = 512;
    int lenSoFar = 0;
    int currentLineSize = MAX_LEN;

    char *line = NULL;
    line = (char*)malloc(MAX_LEN*sizeof(char));
    if (!line) {
        puts("!!! Failed to Allocate Memory !!!");
        return -1;
    }
    line[0] = '\0';
    
    char buffer[MAX_LEN];
    short pipeCount = 0;

    while (fgets(buffer, MAX_LEN, q1)) {
        // Count pipes
        char *pipes = strstr(buffer, "|");
        while (pipes) {
            pipeCount++;
            pipes = strstr(pipes+1, "|");
        }

        // Double line's size
        int nl = strlen(buffer) - 1;
        lenSoFar += nl+2;
        if (lenSoFar > currentLineSize){
            char *tempLine = (char*)realloc(line, currentLineSize*2);
            if (!tempLine) {
                free(line);
                puts("!!! Failed to Allocate Memory !!!");
                return -1;
            }
            line = tempLine;
            currentLineSize *= 2;
        }

        // Check for sufficient pipe count
        if (pipeCount == 2){
            if (buffer[nl] == '\n') {
                buffer[nl] = '\0';
                if (buffer[nl-1] == '\r') {
                    buffer[nl-1] = '\0';
                }
                dputs(line);

                strcat(line, buffer);
                // Parse line
                if (parseLine(line)) {
                    free(line);
                    return -1;
                }
                line[0] = '\0';
                pipeCount = 0;
                lenSoFar = 0;
            } else {
                strcat(line, buffer);
            }
        } else {
            strcat(line, buffer);
        }
    }

    free(line);
    fclose(q1);
    q1 = NULL;
    return 0;
}

/*
First run of game.
Prompts are loaded in by this point.
Loop through prompts starting at Intro.
*/
int beginGame() {
    char goToTitle[80];
    strncpy(goToTitle, "INT", 6);
    Prompt *cur = getPrompt(goToTitle);

    while (cur) {
        // State check
        if (cur->title[0] == '?') {
            for (int ii = 0; ii < cur->optionsUsed; ii++) {
                if (!strcmp(cur->options[ii].answer, "_") || !strcmp(cur->description, cur->options[ii].answer)) {
                    // either error or state match
                    strncpy(goToTitle, cur->options[ii].goToTitle, strlen(cur->options[ii].goToTitle)+1);
                    break;
                }
            }
        } else {
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
                    if (!strcmp(cur->options[ii].answer, "_") || !strcmp(cur->options[ii].answer, answer)) {
                        // either end of options or user answer matches
                        strncpy(goToTitle, cur->options[ii].goToTitle, strlen(cur->options[ii].goToTitle)+1);
                        break;
                    }
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
Free DLL.
Close any possibly open files.
Handle error status.
*/
int quit(int status) {
    // free prompts
    freePTDoublyLinkedList();
    if (q1) {
        fclose(q1);
    }
    if (status) {
        exit(status);
    }
    return status;
}
