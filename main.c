/*
Meta Math
Elijah Delavar
main.c quiz.c quiz.h

TODO: Compilation

TODO: Description
*/

#include <stdio.h>
#include <stdlib.h>
#include "quiz.h"

int main(int argc, char const *argv[])
{
    if (loadPrompts()) {
        puts("Error occurred!");
        exit(1);
    }
    if (beginGame()) {
        puts("Error occurred!");
        exit(1);
    }
    return quit();
}