/*
Meta Math
Elijah Delavar
main.c quiz.c quiz.h

TODO: Description
*/

#ifndef QUIZ
#define QUIZ

typedef struct ANSWERS {
    char *answer;
    int numAnswer;
    struct ANSWERS *next;
    struct ANSWERS *prev;
} Answers;

int beginGame();

#endif