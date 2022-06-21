#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

int read_matrix(char* path, unsigned long** matrix, size_t* lines, size_t* columns) {
    FILE* fd = fopen(path, "r");
    if (fd == NULL) {
        perror("error opening file");
        return 1;
    }

    char* line = NULL;
    size_t len = 0;
    if (getline(&line, &len, fd) == -1) {
        perror("error reading file");
        return 1;
    };

    if (sscanf(line, "%lux%lu\n", lines, columns) != 2) {
        fprintf(stderr, "Invalid matrix size specifier\n");
        return 1;
    }

    *matrix = malloc((*lines) * (*columns) * sizeof(unsigned long));
    size_t index = 0;

    while (getline(&line, &len, fd) != -1) {
        char* number_str = strtok(line, " ");
        while (number_str != NULL) {
            unsigned long number = strtoul(number_str, NULL, 0);
            if (number == ULONG_MAX) {
                fprintf(stderr, "Number overflows unsigned long");
            }

            (*matrix)[index] = number;
            index++;

            number_str = strtok(NULL, " \n");
        }
    }

    free(line);
    return 0;
}

void add_matrices(const unsigned long* matrix1, const unsigned long* matrix2, unsigned long** result, size_t lines, size_t cols) {
    *result = mmap(NULL, lines*cols*sizeof(unsigned long), PROT_READ|PROT_WRITE,
        MAP_SHARED|MAP_ANONYMOUS, 0, 0);

    for (size_t col = 0; col < cols; col++) {
        int res = fork();
        if (res == -1) {
            perror("Error executing fork");
            exit(1);
        } else if (res == 0) {
            for (size_t line = 0; line < lines; line++) {
                (*result)[line * cols + col] = matrix1[line * cols + col] + matrix2[line * cols + col];
            }
            exit(0);
        }
    }

    int status = 0;
    while (wait(&status) > 0) {
        if (!WIFEXITED(status) || (WIFEXITED(status) && WEXITSTATUS(status) != 0)) {
            fprintf(stderr, "Child process did not terminate cleanly.\n");
            exit(1);
        }
    }

    if (errno != ECHILD) {
        perror("Error waiting for child processes");
        exit(1);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "USAGE: addmx file1 file2\n");
        exit(1);
    }

    unsigned long* matrix1;
    size_t matrix1_lines;
    size_t matrix1_cols;
    read_matrix(argv[1], &matrix1, &matrix1_lines, &matrix1_cols);

    unsigned long* matrix2;
    size_t matrix2_lines;
    size_t matrix2_cols;
    read_matrix(argv[2], &matrix2, &matrix2_lines, &matrix2_cols);

    if (matrix1_cols != matrix2_cols || matrix1_lines != matrix2_lines) {
        fprintf(stderr, "Error! Different matrix sizes");
    }

    unsigned long* result;
    add_matrices(matrix1, matrix2, &result, matrix1_lines, matrix1_cols);

    printf("%lux%lu\n", matrix1_lines, matrix1_cols);
    for (size_t i = 0; i < matrix1_lines; i++) {
        for (size_t j = 0; j < matrix1_cols; j++) {
            printf("%lu ", result[i * matrix1_cols + j]);
        }
        printf("\n");
    }

    free(matrix1);
    free(matrix2);

    return 0;
}
