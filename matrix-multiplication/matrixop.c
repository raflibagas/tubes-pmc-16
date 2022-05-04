#include "matrixop.h"

matrix adjust_matrix(matrix *m)
{
    int i,j,n;
    matrix temp;
    n = adjust_size(m);
    initialize_matrix(&temp,n,n);
    for(i=1;i<=n;i++)
    {
        for(j=1;j<=n;j++)
        {
            if(i<=m->rows && j<=m->columns)
            {
                create_matrix(&temp, find_data(m,i,j),i,j);
            }
            else
            {
                create_matrix(&temp,0,i,j);
            }
        }
    }
    return temp;
}

matrix add_matrix(matrix *m1, matrix *m2)
{
    int i,j;
    matrix temp;
    initialize_matrix(&temp,m1->rows,m1->columns);
    for(i=1;i<=m1->rows;i++)
    {
        for(j=1;j<=m1->columns;j++)
        {
            create_matrix(&temp,find_data(m1,i,j)+find_data(m2,i,j),i,j);
        }
    }
    return temp;
}

matrix substract_matrix(matrix *m1, matrix *m2)
{
    int i,j;
    matrix temp;
    initialize_matrix(&temp,m1->rows,m1->columns);
    for(i=1;i<=m1->rows;i++)
    {
        for(j=1;j<=m1->columns;j++)
        {
            create_matrix(&temp,find_data(m1,i,j)-find_data(m2,i,j),i,j);
        }
    }
    return temp;
}

matrix naive(matrix *m1, matrix *m2)
{
    matrix temp_matrix;
    initialize_matrix(&temp_matrix, m1->rows, m2->columns);
    if(m1->columns != m2->rows)
    {
        return temp_matrix;
    }
    int i,j;
    for(i=1;i<=m1->rows;i++)
    {
        for(j=1;j<=m2->columns;j++)
        {
            create_matrix(&temp_matrix, multiply_matrix(m1,m2,i,j),i,j);
        }
    }
    return temp_matrix;
}

matrix initialize_strassen(matrix *m1, matrix *m2)
{
    matrix adj_m1,adj_m2,adj_res;
    adj_m1=adjust_matrix(m1);
    adj_m2=adjust_matrix(m2);
    initialize_matrix(&adj_res,m1->columns,m2->rows);
    adj_res=strassen(&adj_m1,&adj_m2);
    
    if(m1->rows!=adjust_size(m1))
    {
        int i,j;
        matrix res;
        initialize_matrix(&res,m1->columns,m2->rows);
        for(i=1;i<=m1->columns;i++)
        {
            for(j=1;j<=m2->rows;j++)
            {
                create_matrix(&res,find_data(&adj_res,i,j),i,j);
            }
        }
        return res;
    }
    return adj_res;
}

matrix strassen(matrix *m1, matrix *m2)
{
    matrix temp_matrix;
    initialize_matrix(&temp_matrix,m1->columns,m2->rows);
    if(m1->rows==1)
    {
        temp_matrix = naive(m1,m2);
        return temp_matrix;
    }
    matrix a,b,c,d,e,f,g,h;
    initialize_matrix(&a,(m1->rows)/2,(m1->columns)/2);
    initialize_matrix(&b,(m1->rows)/2,(m1->columns)/2);
    initialize_matrix(&c,(m1->rows)/2,(m1->columns)/2);
    initialize_matrix(&d,(m1->rows)/2,(m1->columns)/2);
    initialize_matrix(&e,(m2->rows)/2,(m2->columns)/2);
    initialize_matrix(&f,(m2->rows)/2,(m2->columns)/2);
    initialize_matrix(&g,(m2->rows)/2,(m2->columns)/2);
    initialize_matrix(&h,(m2->rows)/2,(m2->columns)/2);

    split_matrix(m1,&a,&b,&c,&d);
    split_matrix(m2,&e,&f,&g,&h);

    matrix p1,p2,p3,p4,p5,p6,p7,par1,par2;
    initialize_matrix(&p1,(&a)->rows,(&b)->columns);
    initialize_matrix(&p2,(&a)->rows,(&b)->columns);
    initialize_matrix(&p3,(&a)->rows,(&b)->columns);
    initialize_matrix(&p4,(&a)->rows,(&b)->columns);
    initialize_matrix(&p5,(&a)->rows,(&b)->columns);
    initialize_matrix(&p6,(&a)->rows,(&b)->columns);
    initialize_matrix(&p7,(&a)->rows,(&b)->columns);

    par1=a;
    par2=substract_matrix(&f,&h);
    p1=strassen(&par1,&par2);

    par1=add_matrix(&a,&b);
    par2=h;
    p2=strassen(&par1,&par2);

    par1=add_matrix(&c,&d);
    par2=e;
    p3=strassen(&par1,&par2);

    par1=d;
    par2=substract_matrix(&g,&e);
    p4=strassen(&par1,&par2);

    par1=add_matrix(&a,&d);
    par2=add_matrix(&e,&h);
    p5=strassen(&par1,&par2);

    par1=substract_matrix(&b,&d);
    par2=add_matrix(&g,&h);
    p6=strassen(&par1,&par2);

    par1=substract_matrix(&a,&c);
    par2=add_matrix(&e,&f);
    p7=strassen(&par1,&par2);

    matrix c1,c2,c3,c4;
    initialize_matrix(&c1,(&p1)->rows,(&p1)->columns);
    initialize_matrix(&c2,(&p1)->rows,(&p1)->columns);
    initialize_matrix(&c3,(&p1)->rows,(&p1)->columns);
    initialize_matrix(&c4,(&p1)->rows,(&p1)->columns);

    par1=add_matrix(&p5,&p4);
    par2=substract_matrix(&p2,&p6);
    c1=substract_matrix(&par1,&par2);

    c2=add_matrix(&p1,&p2);

    c3=add_matrix(&p3,&p4);

    par1=add_matrix(&p1,&p5);
    par2=add_matrix(&p3,&p7);
    c4=substract_matrix(&par1,&par2);

    unite_matrix(&temp_matrix,&c1,&c2,&c3,&c4);
    return temp_matrix;
}

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

void input_random_data(matrix *m)
{
    int i,j;
    for(i=1;i<=m->rows;i++)
    {
        for(j=1;j<=m->columns;j++)
        {
            create_matrix(m,random_data(),i,j);
        }
    }
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

void split_matrix(matrix *m, matrix *c1, matrix *c2, matrix *c3, matrix *c4)
{
    int i,j;
    for(i=1;i<=m->rows;i++)
    {
        for(j=1;j<=m->columns;j++)
        {
            if(i<=(m->rows)/2 && j<=(m->columns)/2)
            {
                create_matrix(c1,find_data(m,i,j),i,j);
            }
            else if(i<=(m->rows)/2 && j>(m->columns)/2)
            {
                create_matrix(c2,find_data(m,i,j),i,j-((m->columns)/2));
            }
            else if(i>(m->rows)/2 && j<=(m->columns)/2)
            {
                create_matrix(c3,find_data(m,i,j),i-(m->rows)/2,j);
            }
            else
            {
                create_matrix(c4,find_data(m,i,j),i-(m->rows)/2,j-(m->columns)/2);
            }
        }
    }
}

void unite_matrix(matrix *m, matrix *c1, matrix *c2, matrix *c3, matrix *c4)
{
    int i,j;
    for(i=1;i<=m->rows;i++)
    {
        for(j=1;j<=m->columns;j++)
        {
            if(i<=(m->rows)/2 && j<=(m->columns)/2)
            {
                create_matrix(m,find_data(c1,i,j),i,j);
            }
            else if(i<=(m->rows)/2 && j>(m->columns)/2)
            {
                create_matrix(m,find_data(c2,i,j-(m->columns)/2),i,j);
            }
            else if(i>(m->rows)/2 && j<=(m->columns)/2)
            {
                create_matrix(m,find_data(c3,i-(m->rows)/2,j),i,j);
            }
            else
            {
                create_matrix(m,find_data(c4,i-(m->rows)/2,j-(m->columns)/2),i,j);
            }
        }
    }  
}

int adjust_size(matrix *m)
{
    int n=1;
    while(n*2<m->rows)
    {
        n=n*2;
    }
    return n = n*2;
}

int random_data()
{
    int lower=0, upper=100;
    return (rand() % (upper-lower+1)) + lower;
}

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

int multiply_matrix(matrix *m1, matrix *m2, int r, int c)
{
    int i,sum=0;
    for(i=1;i<=m1->columns;i++)
    {
        sum = sum + find_data(m1,r,i) * find_data(m2,i,c);
    }
    return sum;
}