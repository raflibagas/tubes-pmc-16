#include <stdlib.h>
#include <stdio.h>

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

void initialize_matrix(matrix *m, int r, int c);

void create_matrix(matrix *m, int d, int r, int c);

void print_matrix(matrix *m);

void print_kmap(matrix *m);

void initialize_kmap();

matrix read_kmap(matrix *m);

matrix* get_full_one(matrix *m);

matrix* get_four_one(matrix *m);

matrix* get_two_h_one(matrix *m);

matrix* get_two_v_one(matrix *m);

int find_data(matrix *m, int r, int c);