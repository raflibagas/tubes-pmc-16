#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef struct element
{
    int row;
    int col;
    int data;
    struct element *next;
}element;

typedef struct matrix
{
    element *head;
    int rows;
    int columns;
    element *tail;
    int nodecount;
}matrix;

matrix adjust_matrix(matrix *m);

matrix add_matrix(matrix *m1, matrix *m2);

matrix substract_matrix(matrix *m1, matrix *m2);

matrix naive(matrix *m1, matrix *m2);

matrix initialize_strassen(matrix *m1, matrix *m2);

matrix strassen(matrix *m1, matrix *m2);

void initialize_matrix(matrix *m, int r, int c);

void create_matrix(matrix *m, int d, int r, int c);

void input_random_data(matrix *m);

void print_matrix(matrix *m);

void split_matrix(matrix *m, matrix *c1, matrix *c2, matrix *c3, matrix *c4);

void unite_matrix(matrix *m, matrix *c1, matrix *c2, matrix *c3, matrix *c4);

int adjust_size(matrix *m);

int random_data();

int find_data(matrix *m, int r, int c);

int multiply_matrix(matrix *m1, matrix *m2, int r, int c);