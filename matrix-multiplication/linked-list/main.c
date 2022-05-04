#include "matrixop.h"

int main()
{
    // deklarasi variabel
    int r1,c1,r2,c2,i,j;
    matrix m1,m2,m3,m4;

    // input
    printf("Enter number of rows of Matrix A\t: ");
    scanf("%d", &r1);
    printf("Enter number of columns of Matrix A\t: ");
    scanf("%d", &c1);
    printf("Enter number of rows of Matrix B\t: ");
    scanf("%d", &r2);
    printf("Enter number of columns of Matrix B\t: ");
    scanf("%d", &c2);
    initialize_matrix(&m1, r1, c1);
    input_random_data(&m1);
    initialize_matrix(&m2, r2, c2);
    input_random_data(&m2);

    // output
    printf("\nMatrix A\n");
    print_matrix(&m1);
    printf("\n");
    printf("Matrix B\n");
    print_matrix(&m2);
    printf("\n");

    printf("Hasil Naive Algorithm\n");
    m3 = naive(&m1,&m2);
    print_matrix(&m3);
    printf("\n");

    printf("Hasil Strassen Algortihm\n");
    m4 = initialize_strassen(&m1,&m2);
    print_matrix(&m4);
    printf("\n");

    return 0;
}