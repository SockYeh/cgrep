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
    int reti = regcomp(&reegex,argv[optind], REG_EXTENDED);

    char lineBuffer[MAX_LINE_LENGTH];
    while (fgets(lineBuffer, MAX_LINE_LENGTH, filePointer) != NULL) {
        
        lineBuffer[strcspn(lineBuffer, "\n")] = 0;

        if (!reti) {
            if (!regexec(&reegex, lineBuffer, 0, NULL, 0)) {
                printf("%s",lineBuffer);
            }
        } else if (strstr(lineBuffer, argv[1]) != NULL) {
            printf("%s",lineBuffer);
        }
        
    }

    fclose(filePointer);
    regfree(&reegex);

    return 0;
}