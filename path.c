/*
Meta Math
Elijah Delavar
main.c quiz.c quiz.h

TODO: Description
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *pathFN = "./GameFiles/Path.txt";
char *possiblePathsFN = "./GameFiles/PossiblePaths.csv";

int readPathCSV(char**, char**);
short checkPathMatch(char*);    // TODO

/*
Read the player's path and record it in the given character array.
Player's path is separated by newlines.
Read the possible paths and record it in the given character array.
Possible paths are separated by commas and pipes.
Both character arrays need to be malloc'd.
*/
int readPathCSV(char **playerPath, char **possiblePaths) {
    // Player Path
    FILE *fp = NULL;
    fp = fopen(pathFN, "r");
    if (!fp) {
        puts("! File Not Found !");
        return -1;
    }

    // Read in player path and convert to comma separated values
    int MAXLEN = 10;
    char buffer[MAXLEN];

    while (fgets(buffer, MAXLEN, fp)) {
        int nl = strlen(buffer) - 1;

        // Add to player path
        if (buffer[nl] == '\n') {
            if (buffer[nl-1] == '\r') {
                buffer[nl-1] = ',';
                buffer[nl] = '\0';
            } else {
                buffer[nl] = ',';
                buffer[nl+1] = '\0';
            }

            nl += 2;

            // malloc player path
            if (!(*playerPath)) {
                *playerPath = (char*)malloc(sizeof(char)*nl);
                if (!(*playerPath)) {
                    puts("!!! Failed to Allocate Memory !!!");
                    fclose(fp);
                    return -1;
                }
                *playerPath[0] = '\0';
            } else {
                char *temp = NULL;
                temp = (char*)realloc(*playerPath, sizeof(char)*(strlen(*playerPath)+1+nl));
                if (!temp) {
                    puts("!!! Failed to Allocate Memory !!!");
                    fclose(fp);
                    return -1;
                }
                *playerPath = temp;
            }

            // concatenate buffer to player path
            strcat(*playerPath, buffer);
        } else {
            puts("!! Invalid Title on Player Path !!");
            fclose(fp);
            return -1;
        }
    }

    fclose(fp);

    // Possible Paths
    fp = NULL;
    fp = fopen(possiblePathsFN, "r");
    if (!fp) {
        puts("! File Not Found !");
        return -1;
    }

    fclose(fp);

    return 0;
}