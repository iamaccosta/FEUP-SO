#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

int main(int argc, char** argv) {
    
    matrix* u1 = matrix_new_random(4, 4, 1, 9);
    matrix* u2 = matrix_new(4, 4);

    matrix_print(u1);

    matrix_set(3, 3, 3, u2);
    matrix_print(u2);

    matrix* u3 = matrix_sub(u1, u2);
    matrix_print(u3);
    
    return 0;
}