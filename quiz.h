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
    char *answer;
    char *goToTitle;
}Option;

typedef struct PROMPT {
    char title[80];
    char *description;
    struct OPTION options[OPTIONCOUNT];
    struct PROMPT *next;
    struct PROMPT *prev;
}Prompt;

int loadPrompts();
int beginGame();
int quit();

#endif