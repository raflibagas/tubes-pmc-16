#include "logicmin.h"

int main()
{
    matrix temp;
    initialize_matrix(&temp,2,4);
    create_matrix(&temp,1,1,1);
    create_matrix(&temp,1,1,2);
    create_matrix(&temp,1,1,3);
    create_matrix(&temp,0,1,4);
    create_matrix(&temp,1,2,1);
    create_matrix(&temp,1,2,2);
    create_matrix(&temp,0,2,3);
    create_matrix(&temp,1,2,4);

    printf("KARNAUGH MAP\n");
    print_kmap(&temp);
    return 0;
}