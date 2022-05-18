#include "logicmin.h"

int main()
{
    int i,n,number_variables,number_minterms,number_dont_care;
    char any_dont_care;

    max_group_1=max_group_2=-1;
    printf("Number of variables\t\t\t\t: ");
    scanf("%d",&number_variables);
    bits_size=number_variables;
    max_minterms=(int)pow(2,bits_size);
    initialize_array(&minterms_input,max_minterms);
    initialize_array(&dont_cares_input,max_minterms);
    printf("Number of minterms (Excluding DON'T CARE)\t: ");
    scanf("%d",&number_minterms);

    if(number_minterms==0) return 1;
    for(i=0;i<max_minterms;i++)
    {
        create_array(&minterms_input,-1,i);
        create_array(&dont_cares_input,-1,i);
    }
    printf("Enter the minterms (Not DON'T CARE): \n");
    for(i=0;i<number_minterms;i++)
    {
        scanf("%d",&n);
        create_array(&minterms_input,1,n);
        add_minterms(n);
    }
    printf("Any DON'T CARE? (Y/N): ");
    scanf("%s",&any_dont_care);
    if(any_dont_care=='Y' || any_dont_care=='y')
    {
        printf("Enter the number of DON'T CARE: ");
        scanf("%d",&number_dont_care);
        printf("Enter the DON'T CARE MINTERMS: \n");
        for(i=0;i<number_dont_care;i++)
        {
            scanf("%d",&n);
            create_array(&dont_cares_input,1,n);
            add_minterms(n);
        }
    }
    (&implicants_table)->number_prime_implicants=0;
    initialize_table();
    pairing();
    display_table();
    printf("Simplified Boolean Expression:\n");
    filtering();
    printf("\n");
    return 0;
}