#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
    FILE *f = fopen(argv[1], "r");

    char *str = (char *) malloc(sizeof(char) * 100);
    while(fgets(str, 100, f) != NULL){
        printf("%s", str);
    }

    fclose(f);
    free(str);
}