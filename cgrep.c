#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 256

int main(int argc, char *argv[])
{
    if (argc < 3) {
        printf("Usage: ./cgrep file pattern\n");
        return 1;
    }
    
    FILE *filePointer;
    filePointer = fopen(argv[1],"r");
    
    if (filePointer == NULL) {
        printf("Error: Could not open file.\n");
        return 1;
    }
    
    char line_buffer[MAX_LINE_LENGTH];
    while (fgets(line_buffer, MAX_LINE_LENGTH, filePointer) != NULL) {
        if (strstr(line_buffer, argv[2]) == NULL) {
            continue;
        }
        printf("%s",line_buffer);
    }



    fclose(filePointer);

    return 0;
}