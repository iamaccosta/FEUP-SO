#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
    FILE *f1 = fopen(argv[1], "r");

    FILE *f2 = fopen(argv[2], "w");

    char *str = (char *) malloc(sizeof(char) * 100);
    while(fgets(str, 100, f1) != NULL){
        fprintf(f2, "%s", str);
    }

    fclose(f1);
    fclose(f2);
    free(str);
}