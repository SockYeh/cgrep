#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <unistd.h> 
#define MAX_LINE_LENGTH 256

int main(int argc, char *argv[])
{
    int option;

    int caseInsensitve = 0;
    int invertMatch = 0;
    int countMatch = 0;
    int showLineNum = 0;

    int count;
    int lineNum;


    while ((option = getopt(argc, argv, "icvn")) != -1) {
        switch (option) {
            case 'i': caseInsensitve = 1; break;
            case 'v': invertMatch = 1; break;
            case 'c': countMatch = 1; break;
            case 'n': showLineNum = 1; break;
            default:
                fprintf(stderr, "Usage: ./cgrep \"pattern\" [-icvn] file\n");
                return 1;
        }
    }


    
    FILE *filePointer;
    if ((argc - optind) < 2) {
        filePointer = stdin;
    } else {
        filePointer = fopen(argv[optind + 1], "r");
        
        if (filePointer == NULL) {
            fprintf(stderr, "Error: Could not open file.\n");
            return 1;
        }
    }

    regex_t reegex;
    int regFlags = REG_EXTENDED | REG_NOSUB;

    if (caseInsensitve) {
        regFlags |= REG_ICASE;
    }

    int reti = regcomp(&reegex,argv[optind], regFlags);

    char lineBuffer[MAX_LINE_LENGTH];
    while (fgets(lineBuffer, MAX_LINE_LENGTH, filePointer) != NULL) {
        int match = 0;
        lineNum++;
       
        size_t len = strlen(lineBuffer);
        if (len > 0 && lineBuffer[len - 1] == '\n') {
            lineBuffer[--len] = '\0'; 
        }
        if (len > 0 && lineBuffer[len - 1] == '\r') {
            lineBuffer[len - 1] = '\0';
        }

        if (!reti) {
            if (!regexec(&reegex, lineBuffer, 0, NULL, 0)) {
                match = 1;
            }
        } else if (caseInsensitve) {
            if (strcasestr(lineBuffer, argv[optind]) != NULL) {
                match = 1;
            }
        } else {
            if (strstr(lineBuffer, argv[optind]) != NULL) {
                match = 1;
            }
        }

        if (invertMatch != match) {
            if (showLineNum) {
                printf("%d:", lineNum);
            }
            printf("%s\n",lineBuffer);
            count++;
        }
        
    }

    if (countMatch) {
        printf("Total Matches: %d\n", count);
    }
    
    fclose(filePointer);
    regfree(&reegex);

    return 0;
}