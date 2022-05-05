#include "logicmin.h"

void initialize_matrix(matrix *m, int r, int c)
{
    m->head=NULL;
    m->tail=NULL;
    m->rows=r;
    m->columns=c;
    m->nodecount=r*c;
}

void create_matrix(matrix *m, int d, int r, int c)
{
    element *current=(element *)malloc(sizeof(element));
    current->row=r;
    current->col=c;
    current->data=d;
    current->next=m->head;
    m->head=current;
}

void print_matrix(matrix *m)
{
    if(m->head==NULL)
    {
        printf("Matrix is empty!\n");
        return;
    }

    int i,j;
    for(i=1;i<=m->rows;i++)
    {
        for(j=1;j<=m->columns;j++)
        {
            printf("%d\t",find_data(m,i,j));
        }
        printf("\n");
    }
}

void print_kmap(matrix *m)
{
    if(m->head==NULL)
    {
        printf("Matrix is empty!\n");
        return;
    }

    int i,j;
    printf("x/yz\ty'z'\ty'z\tyz\tyz'\n");
    for(i=1;i<=m->rows;i++)
    {
        if(i==1)
        {
            printf("x'\t");
        }
        else
        {
            printf("x\t");
        }
        for(j=1;j<=m->columns;j++)
        {
            printf("%d\t",find_data(m,i,j));
        }
        printf("\n");
    }
}

void initialize_kmap()
{

}

matrix read_kmap(matrix *m)
{
    int i,j;
    matrix res;
    i=1;
    j=1;
    
}

matrix* get_full_one(matrix *m, int r)
{
    if(!check_full_one(m, r)){
        return NULL;
    }
    
}

matrix* get_four_one(matrix *m);

matrix* get_two_h_one(matrix *m);

matrix* get_two_v_one(matrix *m);

int find_data(matrix *m, int r, int c)
{
    element *current=m->head;
    while(current!=NULL)
    {
        if(current->row==r && current->col==c)
            return current->data;
        current=current->next;
    }
}