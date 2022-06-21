#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char * argv[]){
    FILE *f = fopen(argv[1], "r");

    int count = 0;
    if(strcmp(argv[2], "c") == 0){
        char c;
        while(fscanf(f, "%c", &c) == 1){
            if(c != ' ' && c != '\n')
                count ++;
        }

        printf("%d characters\n", count);
    }
    else if(strcmp(argv[2], "w") == 0){
        char c;
        while(fscanf(f, "%c", &c) != EOF){
            if(c == ' ') count ++;
        }
        printf("%d words\n", count);
    }
    else if(strcmp(argv[2], "l") == 0){
        char *str = (char *) malloc(sizeof(char) * 100);
        while(fgets(str, 1000, f) != NULL){
            count++;
        }
        printf("%d lines\n", count);
    }
    else printf("none\n");
}