/*
Meta Math
Elijah Delavar
main.c quiz.c quiz.h

TODO: Description
*/

#include <stdio.h>
#include <string.h>
#include "quiz.h"

char *pathFN = "./GameFiles/Path.txt";
char *possiblePathsFN = "./GameFiles/PossiblePaths.csv";

int readPathCSV(char*, char*);
short checkPathMatch(char*);    // TODO

/*
Read the player's path and record it in the given character array.
Read the possible paths and record it in the given character array.
Both character arrays need to be malloc'd.
*/
int readPathCSV(char *playerPath, char *possiblePaths) {
    FILE *fp = NULL;
    fp = fopen(pathFN, "r");
    if (!fp) {
        puts("! File Not Found !");
        return -1;
    }



    fclose(fp);

    return 0;
}