#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <unistd.h> 
#define MAX_LINE_LENGTH 256

int main(int argc, char *argv[])
{
    int caseInsensitve = 0;
    int invertMatch = 0;
    int option;

    while ((option = getopt(argc, argv, "iv")) != -1) {
        switch (option) {
            case 'i': caseInsensitve = 1; break;
            case 'v': invertMatch = 1; break;
            default:
                fprintf(stderr, "Usage: ./cgrep \"pattern\" [-iv] file\n");
                return 1;
        }
    }


    if ((argc - optind) < 2) {
        fprintf(stderr, "Usage: ./cgrep \"pattern\" [-iv] file\n");
        return 1;
    }
    
    FILE *filePointer;
    filePointer = fopen(argv[optind + 1],"r");
    
    if (filePointer == NULL) {
        fprintf(stderr, "Error: Could not open file.\n");
        return 1;
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
            printf("%s\n",lineBuffer);
        }
        
    }

    fclose(filePointer);
    regfree(&reegex);

    return 0;
}