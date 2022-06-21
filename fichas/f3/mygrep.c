#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
    FILE *f = fopen(argv[2], "r");

    char c;
    int col = 1, row = 1, i = 0;

    while(fscanf(f, "%c", &c) != EOF){
        if(c == '\n'){
            row++;
            col = 1;
        }
        if(c == argv[1][i]){
            i++;
            if(i == strlen(argv[1]))
                printf("[%d, %ld]\n", row, col-strlen(argv[1])+1);
        }
        else 
            i=0;

        col++;
    }
    return 0;
}
