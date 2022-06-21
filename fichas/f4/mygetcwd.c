#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
    char cwd[256];

    if(getcwd(cwd, sizeof(cwd)) == NULL)
        perror("getcwd error\n");
    else
        printf("Current working directory is: %s\n", cwd);

    return EXIT_SUCCESS;
}