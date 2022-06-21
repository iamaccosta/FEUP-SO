#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc, char *argv[]){
    if(argc != 2)
        return EXIT_FAILURE;

    FILE *f = fopen(argv[1], "w");
    fclose(f);

    mode_t newperms = (mode_t) 0;

    newperms |= S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

    if (chmod(argv[1], newperms) == -1) {
        (void)fprintf(stderr, "%s: cannot chmod %s\n", argv[0], argv[2]);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}