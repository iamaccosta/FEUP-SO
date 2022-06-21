#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_usage() {
    fprintf(stderr, "USAGE: phrases [-l] file\n");
    exit(1);
}

void parse_args(int argc, char* argv[], bool* listing, char** path) {
    if (argc != 2 && argc != 3) {
        print_usage();
    }

    if (strcmp(argv[1], "-l") == 0) {
        *listing = true;
    }
    if ((*listing && argc == 2) || (!*listing && argc == 3)) {
        print_usage();
    }

    if (argc == 2) {
        *path = argv[1];
    } else {
        *path = argv[2];
    }
}

#define END_OF_PHRASE_COUNT 3
const char END_OF_PHRASE[END_OF_PHRASE_COUNT] = {'.', '!', '?'};

bool is_end_of_phrase(char ch) {
    for (size_t i = 0; i < END_OF_PHRASE_COUNT; i++) {
        if (ch == END_OF_PHRASE[i]) {
            return true;
        }
    }
    return false;
}

#define ALLOC_SIZE 256

void put_character(char** string, size_t* capacity, size_t* length, char ch) {
    (*length)++;
    if (*length > *capacity) {
        *capacity += ALLOC_SIZE * sizeof(char);
        *string = realloc(*string, *capacity);
    }
    (*string)[*length - 1] = (char) ch;
}

int find_phrases(FILE* fd, bool listing) {
    size_t phrase_capacity = ALLOC_SIZE * sizeof(char);
    size_t phrase_length = 0;
    char* phrase = malloc(phrase_capacity);

    bool was_at_end_of_phrase = false;
    unsigned long count = 0;
    while (!feof(fd) && !ferror(fd)) {
        int ch = fgetc(fd);
        if (ch == EOF) {
            break;
        }

        bool is_at_end_of_phrase = is_end_of_phrase((char) ch);
        if (was_at_end_of_phrase && !is_at_end_of_phrase) {
            // end of phrase
            count++;

            if (listing) {
                put_character(&phrase, &phrase_capacity, &phrase_length, '\0');
                printf("[%lu] %s\n", count, phrase);
            }
            phrase_length = 0;
        }

        if (!(phrase_length == 0 && isspace(ch)) && ch != '\r') {
            if (ch == '\n') {
                ch = ' ';
            }
            put_character(&phrase, &phrase_capacity, &phrase_length, (char) ch);
        }

        was_at_end_of_phrase = is_at_end_of_phrase;
    }

    if (phrase_length != 0) {
        count++;

        if (listing) {
            put_character(&phrase, &phrase_capacity, &phrase_length, '\0');
            printf("[%lu] %s\n", count, phrase);
        }
    }

    free(phrase);

    if (!listing) {
        printf("%lu\n", count);
    }

    if (ferror(fd)) {
        perror("error reading from file");
        return 1;
    }
    return 0;
}

int main(int argc, char* argv[]) {
    bool listing = false;
    char* path;
    parse_args(argc, argv, &listing, &path);

    FILE* fd = fopen(path, "r");
    if (fd == NULL) {
        perror("error opening file");
        return 1;
    }

    int ret = find_phrases(fd, listing);
    fclose(fd);

    return ret;
}
