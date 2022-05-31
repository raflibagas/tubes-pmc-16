#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct element
{
    int row;
    int col;
    int data;
    struct element *next;
}element;

typedef struct array
{
    element *head;
    int rows;
    int columns;
    element *tail;
    int nodecount;
}array;

typedef struct matrix
{
    element *head;
    int rows;
    int columns;
    element *tail;
    int nodecount;
}matrix;

typedef struct minterm
{
    int paired;
    int number_ones;
    int group;
    int number_pairs;
    array paired_minterms;
    array binary;
    struct minterm *next;
}minterm;

typedef struct table
{
    int number_prime_implicants;
    matrix arr;
    matrix brr;
    array minterm_counter;
}table;

static void activate(GtkApplication *app, gpointer user_data);
void simplify_btn_clicked(GtkWidget *widget, gpointer data);
void reset_btn_clicked(GtkWidget *widget, gpointer data);
void initialize_array(array *m, int r);
void initialize_matrix(matrix *m, int r, int c);
int find_data_array(array *m, int r);
int find_data_matrix(matrix *m, int r, int c);
void create_array(array *m, int d, int r);
void create_matrix(matrix *m, int d, int r, int c);
void calculating();
void add_minterms(int n);
minterm* create_minterm(int n);
void initialize_table();
void pairing();
void display_minterms();
int possible_pair(minterm *m1, minterm *m2);
void add_pair(minterm *m1, minterm *m2);
minterm* create_pair(minterm *m1, minterm *m2);
void add_to_table();
int check_dont_care(int n);
void display_table();
void filtering();
int number_implicants(int a, int *b);
int find_max(int *n);
void fill_binary(minterm *m1, minterm *m2, minterm *m3);
void convert_binary_to_minterm(char *str, int n);
void remove_minterm(int n);
int entry_err_handling(int n);
void reset_variables();

minterm *minterms_cache,*paired_minterms_cache;
array minterms_input,dont_cares_input;
table implicants_table;
int max_minterms,bits_size,max_group_1,max_group_2,number_minterms,number_dont_cares,reset;
char result[]="",epi_minterms[]="",table_minterms[]="";

GtkWidget *window,*grid,*variable_entry,*variable_label,*minterms_entry,*minterms_label,*dont_cares_entry,*dont_cares_label,*simplify_btn,*reset_btn,*result_entry,*result_label;