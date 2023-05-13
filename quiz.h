/*
Meta Math
Elijah Delavar
main.c quiz.c quiz.h

TODO: Description
*/

#ifndef QUIZ
#define QUIZ

#define OPTIONCOUNT 4

typedef struct OPTION {
    char answer[50];
    char goToTitle[80];
}Option;

typedef struct PROMPT {
    char title[80];
    char *description;
    int optionsUsed;
    struct OPTION options[OPTIONCOUNT];
    struct PROMPT *next;
    struct PROMPT *prev;
}Prompt;

int readCSV();
int beginGame();
int quit(int);

#endif