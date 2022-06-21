#include "vector.h"


vector* vector_new(double x, double y, double z){
    vector* v = (vector*) malloc(sizeof(vector));
    v->x = x;
    v->y = y;
    v->z = z;
    return v;
}

vector* vector_add(vector* v1, vector* v2){
    return vector_new(v1->x+v2->x, v1->y+v2->y, v1->z+v2->z);
}

vector* vector_sub(vector* v1, vector* v2){
    return vector_new(v1->x-v2->x, v1->y-v2->y, v1->z-v2->z);
}

vector* vector_scale(double n, vector* v){
    return vector_new(v->x*n, v->y*n, v->z*n);
}

vector* vector_vprod(vector* v1, vector* v2){
    return vector_new(2, 2, 2);
}

double vector_sprod(vector* v1, vector* v2){
    return 2;
}

double vector_mod(vector* v){
    return sqrt(pow(v->x, 2) + pow(v->y, 2) + pow(v->z, 2));
}