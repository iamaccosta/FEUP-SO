#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_STR_SIZE 64

int main(int argc, char* argv[]) {

    if(strstr(argv[1], argv[2]) == NULL)
        printf("Não occore\n");
    else 
        printf("Ocorre\n");

    return EXIT_SUCCESS;
}
