#include <ctype.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUFFER_SIZE 256
#define CYPHER_PATH "cypher.txt"

#define READ_END 0
#define WRITE_END 1

typedef struct {
    char* first;
    char* second;
} cypher_pair;

void freeCypherPair(cypher_pair* p) {
    free(p->first);
    free(p->second);
}

bool isAlphaNumOrHiphen(char ch) {
    return isalnum(ch) || ch == '-';
}

void writeToPipe(int fd, char* buf, size_t size) {
    size_t bytes_written = 0;
    while (bytes_written != size) {
        ssize_t bytes_written_this_cycle = write(fd, buf + bytes_written, size - bytes_written);
        if (bytes_written_this_cycle == -1) {
            perror("error writing into pipe");
            exit(1);
        }
        bytes_written += (size_t) bytes_written_this_cycle;
    }
}

void ioProcess(int input_fd) {
    size_t read_offset = 0;
    char input_buffer[BUFFER_SIZE];

    while (!feof(stdin) && !ferror(stdin)) {
        size_t bytes_read = fread(input_buffer + read_offset, sizeof(char), BUFFER_SIZE - read_offset, stdin) + read_offset;
        read_offset = 0;
        size_t bytes_to_use = bytes_read;
        if (bytes_read == BUFFER_SIZE && isAlphaNumOrHiphen(input_buffer[bytes_read - 1])) {
            for (size_t i = bytes_read - 2; i > 0; i--) {
                if (!isAlphaNumOrHiphen(input_buffer[i])) {
                    bytes_to_use = i + 1;
                    break;
                }
            }
        }

        writeToPipe(input_fd, input_buffer, bytes_to_use);

        if (bytes_read != bytes_to_use) {
            read_offset = bytes_read - bytes_to_use;
            for (size_t i = 0; i + bytes_to_use < bytes_read; i++) {
                input_buffer[i] = input_buffer[bytes_to_use + i];
            }
        }
    }
}

char* translateWord(char* input, cypher_pair* cyphers, size_t cypher_count) {
    for (size_t i = 0; i < cypher_count; i++) {
        if (strcmp(input, cyphers[i].first) == 0) {
            return cyphers[i].second;
        } else if (strcmp(input, cyphers[i].second) == 0) {
            return cyphers[i].first;
        }
    }
    return input;
}

void cypherProcess(int input_fd, int output_fd, cypher_pair* cyphers, size_t cypher_count) {
    signal(SIGPIPE, SIG_IGN);

    char input_buffer[BUFFER_SIZE];
    char word_buffer[BUFFER_SIZE];
    size_t word_size = 0;
    ssize_t bytes_read;

    while ((bytes_read = read(input_fd, input_buffer, BUFFER_SIZE)) > 0) {
        if (bytes_read < 0) {
            perror("error reading from pipe");
            exit(1);
        }

        for (size_t i = 0; i < (size_t) bytes_read; i++) {
            char ch = input_buffer[i];
            if (isAlphaNumOrHiphen(ch)) {
                word_buffer[word_size] = ch;
                word_size++;
            } else {
                if (word_size > 0) {
                    word_buffer[word_size] = '\0';
                    char* word = translateWord(word_buffer, cyphers, cypher_count);
                    if (dprintf(output_fd, "%s", word) < 0) {
                        perror("error writing to pipe");
                        exit(1);
                    }

                    word_size = 0;
                }
                if (write(output_fd, &ch, 1) < 0) {
                    perror("error writing to pipe");
                    exit(1);
                }
            }
        }
    }

    if (word_size > 0) {
        word_buffer[word_size] = '\0';
        char* word = translateWord(word_buffer, cyphers, cypher_count);
        dprintf(output_fd, "%s", word);
    }
}

cypher_pair* readCyphers(char* path, size_t* cypher_count) {
    FILE* file = fopen(path, "r");
    if (file == NULL) {
        perror("error opening file");
        exit(1);
    }

    *cypher_count = 0;
    cypher_pair* vec = NULL;

    char first[BUFFER_SIZE];
    char second[BUFFER_SIZE];
    while (fscanf(file, "%s %s\n", first, second) == 2) {
        (*cypher_count)++;
        vec = realloc(vec, *cypher_count * sizeof(cypher_pair));

        size_t first_len = strlen(first);
        vec[(*cypher_count)-1].first = malloc(first_len + 1);
        strcpy(vec[(*cypher_count)-1].first, first);

        size_t second_len = strlen(second);
        vec[(*cypher_count)-1].second = malloc(second_len + 1);
        strcpy(vec[(*cypher_count)-1].second, second);
    }

    return vec;
}

typedef struct {
    int fd;
    int child_pid;
} print_thread_args;

void* readPipeToStdout(void* args_) {
    print_thread_args* args = args_;

    int res, status;
    char buf[BUFFER_SIZE+1];
    while ((res = waitpid(args->child_pid, &status, WNOHANG)) == 0) {
        ssize_t bytes_read = read(args->fd, buf, BUFFER_SIZE);
        if (bytes_read < 0) {
            perror("error reading from pipe");
            exit(1);
        }

        buf[bytes_read] = '\0';
        printf("%s", buf);
    }

    if (res == -1) {
        perror("waitpid error");
        exit(1);
    }
    if (!(WIFEXITED(status) && WEXITSTATUS(status) == 0)) {
        fprintf(stderr, "child process didn't exit successfully\n");
        exit(1);
    }

    return NULL;
}

int main(int argc, __attribute__((unused)) char* argv[]) {
    if (argc > 1) {
        fprintf(stderr, "USAGE: cypher\n");
        return 1;
    }

    size_t cypher_count;
    cypher_pair* cyphers = readCyphers(CYPHER_PATH, &cypher_count);
    if (cyphers == NULL) {
        fprintf(stderr, "Failed to load cypher\n");
        return 1;
    }

    int input_pipe_fd[2];
    int output_pipe_fd[2];
    if (pipe(input_pipe_fd) != 0 || pipe(output_pipe_fd) != 0) {
        perror("error creating pipe");
        free(cyphers);
        return 1;
    }

    pid_t res = fork();
    if (res == -1) {
        perror("error during fork");
        free(cyphers);
        return 1;
    } else if (res == 0) {
        close(input_pipe_fd[WRITE_END]);
        close(output_pipe_fd[READ_END]);

        cypherProcess(input_pipe_fd[READ_END], output_pipe_fd[WRITE_END], cyphers, cypher_count);

        close(input_pipe_fd[READ_END]);
        close(output_pipe_fd[WRITE_END]);
    } else {
        close(input_pipe_fd[READ_END]);
        close(output_pipe_fd[WRITE_END]);

        pthread_t print_thread;
        print_thread_args args = {
            .fd = output_pipe_fd[READ_END],
            .child_pid = res,
        };
        pthread_create(&print_thread, NULL, readPipeToStdout, &args);

        ioProcess(input_pipe_fd[WRITE_END]);
        close(input_pipe_fd[WRITE_END]);

        pthread_join(print_thread, NULL);
        close(output_pipe_fd[READ_END]);
    }

    for (size_t i = 0; i < cypher_count; i++)  {
        freeCypherPair(&cyphers[i]);
    }
    free(cyphers);
    return 0;
}
