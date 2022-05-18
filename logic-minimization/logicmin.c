#include "logicmin.h"

void initialize_array(array *m, int r)
{
    m->head=NULL;
    m->tail=NULL;
    m->rows=r;
}

void initialize_matrix(matrix *m, int r, int c)
{
    m->head=NULL;
    m->tail=NULL;
    m->rows=r;
    m->columns=c;
}

int find_data_array(array *m, int r)
{
    element *current=m->head;
    while(current!=NULL)
    {
        if(current->row==r)
            return current->data;
        current=current->next;
    }
}

int find_data_matrix(matrix *m, int r, int c)
{
    element *current=m->head;
    while(current!=NULL)
    {
        if(current->row==r && current->col==c)
            return current->data;
        current=current->next;
    }
}

void create_array(array *m, int d, int r)
{
    element *current=(element *)malloc(sizeof(element));
    current->row=r;
    current->data=d;
    current->next=m->head;
    m->head=current;
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

void add_minterms(int n)
{
    minterm *p,*q,*temp;
    p=create_minterm(n);
    if(p!=NULL)
    {
        if(minterms_cache==NULL)
        {
            minterms_cache=p;
            minterms_cache->next=NULL;
            return;
        }
        else
        {
            q=minterms_cache;
            if(p->group<minterms_cache->group)
            {
                p->next=minterms_cache;
                minterms_cache=p;
                return;
            }
            while(q->next!=NULL && ((q->next->group)<=(p->group)))
            {
                q=q->next;
            }
            if(q->next==NULL)
            {
                q->next=p;
                p->next=NULL;
            }
            else
            {
                temp=q->next;
                q->next=p;
                p->next=temp;
            }
        }
    }
}

minterm* create_minterm(int n)
{
    int b=bits_size-1;
    minterm *p;
    p=(minterm *)malloc(sizeof(minterm));
    if(p==NULL)
    {
        printf("Insertion failed!\n");
    }
    else
    {
        p->number_ones=0;
        initialize_array(&(p->paired_minterms),max_minterms);
        create_array(&(p->paired_minterms),n,0);
        p->number_pairs=1;
        initialize_array(&(p->binary),max_minterms);
        while(n!=0)
        {
            create_array(&(p->binary),n%2,b);
            if(find_data_array(&(p->binary),b)==1)
            {
                (p->number_ones)++;
            }
            b--;
            n=n/2;
        }
        while(b!=-1)
        {
            create_array(&(p->binary),0,b);
            b--;
        }
        p->paired=0;
    }
    p->group=p->number_ones;
    if(p->group>max_group_1)
        {
            max_group_1=p->group;
        }
    return p;
}

void initialize_table()
{
    int i,j;
    initialize_matrix(&((&implicants_table)->brr),max_minterms,max_minterms);
    for(i=0;i<max_minterms;i++)
    {
        for(j=0;j<max_minterms;j++)
        {
            create_matrix(&((&implicants_table)->brr),-1,i,j);
        }
    }
}

void pairing()
{
    minterm *p,*q;
    int match=0;
    static int iteration=1;
    p=minterms_cache;
    q=p;
    printf("Iteration %d\n",iteration);
    iteration++;
    display_minterms();
    max_group_2=-1;
    while(p->group!=max_group_1)
    {
        q=q->next;
        while(q!=NULL && (p->group==q->group))
        {
            q=q->next;
        }
        if(q==NULL)
        {
            p=p->next;
            q=p;
            continue;
        }
        else
        {
            if(q->group!=(p->group+1))
            {
                p=p->next;
                q=p;
                continue;
            }
            if(possible_pair(p,q))
            {
                match=1;
                p->paired=1;
                q->paired=1;
                add_pair(p,q);
                if((p->group)>max_group_2)
                {
                    max_group_2=p->group;
                }
            }
        }
    }
    add_to_table();
    if(match)
    {
        minterms_cache=paired_minterms_cache;
        paired_minterms_cache=NULL;
        max_group_1=max_group_2;
        pairing();
    }
}

void display_minterms()
{
    int count=0,i=0,n=1;
    minterm *p;
    p=minterms_cache;
    while(p!=NULL)
    {
        i=0;
        while(count<(p->number_pairs))
        {
            printf("%d,",find_data_array(&(p->paired_minterms),count));
            count++;
        }
        printf("\b");
        count=0;
        printf("   ");
        while(i<bits_size)
        {
            if(find_data_array(&(p->binary),i)==-1)
            {
                printf("%c",'-');
            }
            else
            {
                printf("%d",find_data_array(&(p->binary),i));
            }
            i++;
        }
        printf("\n");
        n++;
        p=p->next;
    }
}

int possible_pair(minterm *m1, minterm *m2)
{
    int n=bits_size-1,different=0;
    while(n!=-1)
    {
        if(find_data_array(&(m1->binary),n)!=find_data_array(&(m2->binary),n))
        {
            if(different)
            {
                return 0;
            }
            else
            {
                different=1;
            }
        }
        n--;
    }
    return 1;
}

void add_pair(minterm *m1, minterm *m2)
{
    minterm *p,*temp;
    p=create_pair(m1,m2);
    if(paired_minterms_cache==NULL)
    {
        paired_minterms_cache=p;
    }
    else
    {
        temp=paired_minterms_cache;
        while(temp->next!=NULL)
        {
            temp=temp->next;
        }
        temp->next=p;
    }
}

minterm* create_pair(minterm *m1, minterm *m2)
{
    int i,j;
    minterm *p;
    p=(minterm *)malloc(sizeof(minterm));
    if(p==NULL)
    {
        printf("Failed to create the pair");
    }
    else
    {
        for(i=0;i<m1->number_pairs;i++)
        {
            create_array(&(p->paired_minterms),find_data_array(&(m1->paired_minterms),i),i);
        }
        p->number_pairs=m1->number_pairs*2;
        for(j=0;j<m2->number_pairs;j++)
        {
            create_array(&(p->paired_minterms),find_data_array(&(m2->paired_minterms),j),i++);
        }
        p->paired=0;
        p->next=NULL;
        p->group=m1->group;
        fill_binary(m1,m2,p);
    }
    return p;
}

void add_to_table()
{
    int i,j,k,match;
    minterm *p;
    p=minterms_cache;
    while(p!=NULL)
    {
        if(!(p->paired))
        {
            if((&implicants_table)->number_prime_implicants!=0)
            {
                for(i=0;i<(&implicants_table)->number_prime_implicants;i++)
                {
                    match=1;
                    for(j=0;j<p->number_pairs;j++)
                    {
                        if(find_data_matrix(&((&implicants_table)->brr),i,find_data_array(&(p->paired_minterms),j))==1)
                        {
                            printf("case 0\n");
                            continue;
                        }
                        else
                        {
                            match=0;
                            break;
                        }
                    }
                    if(match==1)
                    {
                        break;
                    }
                }
                if(match==1)
                {
                    p=p->next;
                    continue;
                }
            }
            k=bits_size-1;
            while(k!=-1)
            {
                create_matrix(&((&implicants_table)->arr),find_data_array(&(p->binary),k),(&implicants_table)->number_prime_implicants,k);
                k--;
            }
            for(k=0;k<p->number_pairs;k++)
            {
                if(check_dont_care(find_data_array(&(p->paired_minterms),k))==1)
                {
                    create_matrix(&((&implicants_table)->brr),-1,(&implicants_table)->number_prime_implicants,find_data_array(&(p->paired_minterms),k));
                    continue;
                }
                create_array(&((&implicants_table)->minterm_counter),find_data_array(&((&implicants_table)->minterm_counter),(&implicants_table)->number_prime_implicants)+1,(&implicants_table)->number_prime_implicants);
                create_matrix(&((&implicants_table)->brr),1,(&implicants_table)->number_prime_implicants,find_data_array(&(p->paired_minterms),k));
            }
            ((&implicants_table)->number_prime_implicants)++;
        }
        p=p->next;
    }
}

int check_dont_care(int n)
{
    if(find_data_array(&dont_cares_input,n)==1)
    {
        return 1;
    }
    return 0;
}

void display_table()
{
    int i,j;
    printf("Prime Implicants Table:\n");
    for(i=0;i<(&implicants_table)->number_prime_implicants;i++)
    {
        convert_binary_to_minterm(i);
        for(j=0;j<max_minterms;j++)
        {
            if(find_data_matrix(&((&implicants_table)->brr),i,j)==1)
            {
                printf("   %d  ",j);;
            }
        }
        printf("\n");
    }
}

void filtering()
{
    int i,j,n,row,first=1;
    array essential_prime_implicant;
    initialize_array(&essential_prime_implicant,max_minterms);
    for(i=0;i<max_minterms;i++)
    {
        if(find_data_array(&minterms_input,i)==1)
        {
            if(number_implicants(i,&n)==1)
            {
                create_array(&essential_prime_implicant,n,i);
            }
        }
    }
    for(i=0;i<max_minterms;i++)
    {
        if(find_data_array(&essential_prime_implicant,i)==-1)
        {
            if(first!=1)
            {
                printf(" + ");
            }
            else
            {
                first=0;
            }
            convert_binary_to_minterm(find_data_array(&essential_prime_implicant,i));
            remove_minterm(find_data_array(&essential_prime_implicant,i));
            for(j=i+1;j<max_minterms;j++)
            {
                if(find_data_array(&essential_prime_implicant,j)==find_data_array(&essential_prime_implicant,i))
                {
                    create_array(&essential_prime_implicant,-1,j);
                }
            }
            create_array(&essential_prime_implicant,-1,i);
        }
    }
    while(find_max(&row)!=0)
    {
        if(first!=1)
        {
            printf(" + ");
        }
        else
        {
            first=0;
        }
        convert_binary_to_minterm(row);
        remove_minterm(row);
    }
    printf("\b");
}

int number_implicants(int a, int *b)
{
    int i,j,count=0;
    for(i=0;i<(&implicants_table)->number_prime_implicants;i++)
    {
        if(find_data_matrix(&((&implicants_table)->brr),i,a))
        {
            j=i;
            count++;
        }
    }
    *b=j;
    return count;
}

int find_max(int *n)
{
    int i,greatest=-1;
    for(i=0;i<(&implicants_table)->number_prime_implicants;i++)
    {
        if(find_data_array(&((&implicants_table)->minterm_counter),i)>greatest)
        {
            *n=i;
            greatest=find_data_array(&((&implicants_table)->minterm_counter),i);
        }
    }
    return greatest;
}

void fill_binary(minterm *m1, minterm *m2, minterm *m3)
{
    int n=bits_size-1;
    while(n!=-1)
    {
        if(find_data_array(&(m1->binary),n)==find_data_array(&(m2->binary),n))
        {
            create_array(&(m3->binary),find_data_array(&(m1->binary),n),n);
        }
        else
        {
            create_array(&(m3->binary),-1,n);
        }
        n--;
    }
}

void convert_binary_to_minterm(int n)
{
    int i=0;
    char non_complement[]={'a','b','c','d','e','f','g','h'};
    char complement[10][10]={"!a","!b","!c","!d","!e","!f","!g","!h"};
    while(i!=bits_size)
    {
        if(find_data_matrix(&((&implicants_table)->arr),n,i)!=-1)
        {
            if(find_data_matrix(&((&implicants_table)->arr),n,i)==1)
            {
                printf("%c",non_complement[i]);
            }
            else
            {
                printf("%s",complement[i]);
            }
        }
        i++;
    }
}

void remove_minterm(int n)
{
    int i,j;
    for(i=0;i<max_minterms;i++)
    {
        if(find_data_matrix(&((&implicants_table)->brr),n,i)==1)
        {
            create_array(&minterms_input,-1,i);

            for(j=0;j<(&implicants_table)->number_prime_implicants;j++)
            {
                if(find_data_matrix(&((&implicants_table)->brr),j,i)==1)
                {
                    create_matrix(&((&implicants_table)->brr),-1,j,i);
                    create_array(&((&implicants_table)->minterm_counter),find_data_array(&((&implicants_table)->minterm_counter),j)-1,j);
                }
            }
        }
    }
}