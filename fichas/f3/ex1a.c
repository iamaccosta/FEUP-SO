#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define MAX_STR_SIZE 64

int main(int argc, char* argv[]) {
    printf("%s\n", argv[1]);

    for(int i = 0; i < strlen(argv[1]); i++){
        argv[1][i] = tolower(argv[1][i]);
    }

    printf("%s\n", argv[1]);

    return EXIT_SUCCESS;
}
