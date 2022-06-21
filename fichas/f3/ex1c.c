#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_STR_SIZE 64

int main(int argc, char* argv[]) {
    int occurs = 0;
    for(int i = 0; i < strlen(argv[1]); i++){
        if(argv[1][i] == argv[2][0]){
            int j = 0;
            for(int k = 0; k < strlen(argv[2]); k++){
                if(argv[1][i] == argv[2][k]){
                    i++;
                    j++;
                    if(j == strlen(argv[2])) occurs++;
                }
            }
        }
    }

    printf("%d Ocurrs\n", occurs);

    return EXIT_SUCCESS;
}
