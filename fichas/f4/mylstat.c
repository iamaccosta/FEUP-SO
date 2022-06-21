#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    int total_size = 0, total_blocks = 0;
    struct stat info;
    
    if (argc < 2) {
        fprintf(stderr, "usage: %s file\n", argv[0]);
        return EXIT_FAILURE;
    }

    for(int i = 1; i < argc; i++){
        if (lstat(argv[1], &info) == -1) {
            fprintf(stderr, "fsize: Can’t stat %s\n", argv[1]);
            return EXIT_FAILURE;
        }
        
        printf("%s size: %d bytes, disk_blocks: %d, last modification: %ld, owner ID: %d\n", argv[1], (int)info.st_size, (int)info.st_blocks, info.st_mtime, info.st_uid);
        total_size += (int)info.st_size;
        total_blocks += (int)info.st_blocks;
    }

    printf("total_size: %d bytes, total_disk_blocks: %d\n", total_size, total_blocks);
    return EXIT_SUCCESS;
}