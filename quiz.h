/*
Meta Math
Elijah Delavar
main.c quiz.c quiz.h

TODO: Description
*/

#ifndef QUIZ
#define QUIZ

#define OPTIONCOUNT 8

typedef struct OPTION {
    char *answer;    // used for answers and stateIDs
    char *goToTitle;
}Option;

typedef struct PROMPT {
    char *title;        // if '?' is first character, this is a state check
    char *description;  // if state check, this is the value
    int optionsUsed;
    struct OPTION options[OPTIONCOUNT];
    struct PROMPT *next;
    struct PROMPT *prev;
}Prompt;

int readQuizCSV();
int beginGame();
int quit(int);

#endif