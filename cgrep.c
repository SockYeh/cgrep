#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define MAX_LINE_LENGTH 256

int main(int argc, char *argv[])
{
    if (argc < 3) {
        printf("Usage: ./cgrep \"pattern\" file\n");
        return 1;
    }
    
    FILE *filePointer;
    filePointer = fopen(argv[2],"r");
    
    if (filePointer == NULL) {
        printf("Error: Could not open file.\n");
        return 1;
    }

    regex_t reegex;
    int reti = regcomp(&reegex,argv[1], REG_EXTENDED);

    char line_buffer[MAX_LINE_LENGTH];
    while (fgets(line_buffer, MAX_LINE_LENGTH, filePointer) != NULL) {
        
        if (!reti) {
            if (!regexec(&reegex, line_buffer, 0, NULL, 0)) {
                printf("%s",line_buffer);
            }
        } else if (strstr(line_buffer, argv[1]) != NULL) {
            printf("%s",line_buffer);
        }
        
    }



    fclose(filePointer);
    regfree(&reegex);

    return 0;
}