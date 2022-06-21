#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int main(int argc, char *argv[]){
    FILE *f = fopen(argv[1], "r");
    
    char *str = (char *) malloc(sizeof(char) * 100);

    if(strcmp(argv[2], "u") == 0){
        printf("upper\n");
        while(fgets(str, 100, f) != NULL){
            for(int i = 0; i <= strlen(str); i++){
                str[i] = toupper(str[i]);
            }
            printf("%s", str);
        }
    }
    else if(strcmp(argv[2], "l") == 0){
        printf("lower\n");
        while(fgets(str, 100, f) != NULL){
            for(int i = 0; i <= strlen(str); i++){
                str[i] = tolower(str[i]);
            }
            printf("%s", str);
        }
    }
    else{
        printf("none\n");
        while(fgets(str, 100, f) != NULL){
            printf("%s", str);
        }
    }

    fclose(f);
    free(str);
}