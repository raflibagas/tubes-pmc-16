#include "logicmin.h"

// Fungsi utama untuk inisiasi aplikasi logicmin
int main(int argc, char **argv)
{
    GtkApplication *app;
    int status;

    app=gtk_application_new("logic-minimization.com",G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app,"activate",G_CALLBACK(activate),NULL);
    status=g_application_run(G_APPLICATION(app),argc,argv);
    g_object_unref(app);

    return status;
}

// Fungsi front-end app yang menerima input dari user
static void activate(GtkApplication *app, gpointer user_data)
{
    window=gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window),"logicmin");
    gtk_window_set_default_size(GTK_WINDOW(window),250,280);
    gtk_container_set_border_width(GTK_CONTAINER(window),20);

    grid=gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window),grid);

    variable_label=gtk_label_new("Number Of\nVariables\t: ");
    variable_entry=gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(variable_entry),"Enter the number of variables");
    gtk_label_set_xalign(GTK_LABEL(variable_label),0);
    gtk_grid_attach(GTK_GRID(grid),variable_label,0,0,1,1);
    gtk_grid_attach(GTK_GRID(grid),variable_entry,1,0,2,1);

    minterms_label=gtk_label_new("Minterms\t\t: ");
    minterms_entry=gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(minterms_entry),"0,1,2,3,...");
    gtk_label_set_xalign(GTK_LABEL(minterms_label),0);
    gtk_grid_attach(GTK_GRID(grid),minterms_label,0,1,1,1);
    gtk_grid_attach(GTK_GRID(grid),minterms_entry,1,1,2,1);

    dont_cares_label=gtk_label_new("Dont Cares\t: ");
    dont_cares_entry=gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(dont_cares_entry),"0,1,2,3,...");
    gtk_label_set_xalign(GTK_LABEL(dont_cares_label),0);
    gtk_grid_attach(GTK_GRID(grid),dont_cares_label,0,2,1,1);
    gtk_grid_attach(GTK_GRID(grid),dont_cares_entry,1,2,2,1);

    simplify_btn=gtk_button_new_with_label("Simplify");
    gtk_grid_attach(GTK_GRID(grid),simplify_btn,1,3,1,1);
    g_signal_connect(simplify_btn,"clicked",G_CALLBACK(simplify_btn_clicked),NULL); // Tombol "Simplify" ditekan, fungsi simplify_btn_clicked berjalan

    reset_btn=gtk_button_new_with_label("Reset");
    gtk_grid_attach(GTK_GRID(grid),reset_btn,2,3,1,1);
    g_signal_connect(reset_btn,"clicked",G_CALLBACK(reset_btn_clicked),NULL); // Tombol "Reset" ditekan, fungsi reset_btn_clicked berjalan

    gtk_grid_set_row_spacing(GTK_GRID(grid),10);

    result_label=gtk_label_new("Result\t\t: ");
    result_entry=gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(result_entry),"Result");
    gtk_label_set_xalign(GTK_LABEL(result_label),0);
    gtk_grid_attach(GTK_GRID(grid),result_label,0,4,1,1);
    gtk_grid_attach(GTK_GRID(grid),result_entry,1,4,2,1);
    gtk_widget_set_sensitive(result_entry,FALSE);

    gtk_widget_show_all(window);
}

// Fungsi untuk mengolah input dan output
void simplify_btn_clicked(GtkWidget *widget, gpointer data)
{
    // Deklarasi variabel
    int i;
    const char s[2]=",";
    char *token;
    const gchar *minterms_entry_cache,*dont_cares_entry_cache;

    // Assign input dari user
    bits_size=atoi((char *)(gtk_entry_get_text(GTK_ENTRY(variable_entry))));
    minterms_entry_cache=gtk_entry_get_text(GTK_ENTRY(minterms_entry));
    dont_cares_entry_cache=gtk_entry_get_text(GTK_ENTRY(dont_cares_entry));
    max_minterms=pow(2,bits_size);
    
    // Mengidentifikasi input jumlah variabel
    if(bits_size<1 || bits_size>8)
    {
        gtk_entry_set_text(GTK_ENTRY(result_entry),"Err Entry!");
        return;
    }

    // Mengidentifikasi input minterm
    token=strtok(strdup(minterms_entry_cache),s);
    if(token==NULL)
    {
        // Input minterm tidak ada, output nol
        gtk_entry_set_text(GTK_ENTRY(result_entry),"0");
        return;
    }
    if(entry_err_handling(atoi(token)))
    {
        gtk_entry_set_text(GTK_ENTRY(result_entry),"Err Entry!");
        return;
    }

    initialize_array(&minterms_input,max_minterms);
    for(i=0;i<max_minterms;i++)
    {
        create_array(&minterms_input,-1,i);
    }

    i=0;
    create_array(&minterms_input,1,atoi(token));
    while(token!=NULL)
    {
        token=strtok(NULL,s);
        if(token!=NULL){
            create_array(&minterms_input,1,atoi(token));
            if(entry_err_handling(atoi(token)))
            {
                gtk_entry_set_text(GTK_ENTRY(result_entry),"Err Entry!");
                return;
            }
        }

        i++;
    }
    number_minterms=i;
    
    // Mengidentifikasi input dont care
    token=strtok(strdup(dont_cares_entry_cache),s);
    if(token==NULL)
    {
        number_dont_cares=0;
        calculating();
        gtk_entry_set_text(GTK_ENTRY(result_entry),result); // Menampilkan hasil pada aplikasi
        reset_variables();
        return;
    }
    if(entry_err_handling(atoi(token)))
    {
        gtk_entry_set_text(GTK_ENTRY(result_entry),"Err Entry!");
        return;
    }
    initialize_array(&dont_cares_input,max_minterms);
    for(i=0;i<max_minterms;i++)
    {
        create_array(&dont_cares_input,-1,i);
    }

    i=0;
    create_array(&dont_cares_input,1,atoi(token));
    while(token!=NULL)
    {
        token=strtok(NULL,s);
        if(token!=NULL){
            create_array(&dont_cares_input,1,atoi(token));
            if(entry_err_handling(atoi(token)))
            {
                gtk_entry_set_text(GTK_ENTRY(result_entry),"Err Entry!");
                return;
            }
        }

        i++;
    }
    number_dont_cares=i;

    calculating();

    if(number_minterms+number_dont_cares==max_minterms)
    {
        strcat(result,"1"); // Tautologi, output=1
    }

    // Menampilkan hasil pada layar
    gtk_entry_set_text(GTK_ENTRY(result_entry),result);

    reset_variables();
}

// Melakukan reset input dan output
void reset_btn_clicked(GtkWidget *widget, gpointer data)
{
    gtk_entry_set_text(GTK_ENTRY(variable_entry),"");
    gtk_entry_set_text(GTK_ENTRY(minterms_entry),"");
    gtk_entry_set_text(GTK_ENTRY(dont_cares_entry),"");
    gtk_entry_set_text(GTK_ENTRY(result_entry),"");

    minterms_cache=NULL;
    paired_minterms_cache=NULL;
    reset_variables();
}

// Fungsi untuk menginisiasi array
void initialize_array(array *m, int r)
{
    m->head=NULL;
    m->tail=NULL;
    m->rows=r;
}

// Fungsi untuk menginisiasi matriks
void initialize_matrix(matrix *m, int r, int c)
{
    m->head=NULL;
    m->tail=NULL;
    m->rows=r;
    m->columns=c;
}

// Output berupa data yang dicari pada array
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

// Output berupa data yang dicari pada matriks
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

// Untuk menyisipkan data pada array
void create_array(array *m, int d, int r)
{
    element *current=(element *)malloc(sizeof(element));
    current->row=r;
    current->data=d;
    current->next=m->head;
    m->head=current;
}

// Untuk menyisipkan data pada matriks
void create_matrix(matrix *m, int d, int r, int c)
{
    element *current=(element *)malloc(sizeof(element));
    current->row=r;
    current->col=c;
    current->data=d;
    current->next=m->head;
    m->head=current;
}

// Melakukan kalkulasi terhadap input yang diterima
void calculating()
{
    int i;
    reset=1;
    max_group_1=max_group_2=-1;

    if(number_dont_cares==0)
    {
        for(i=0;i<max_minterms;i++)
        {
            if(find_data_array(&minterms_input,i)==1)
            {
                add_minterms(i);
            }
        }
    }
    else
    {
        for(i=0;i<max_minterms;i++)
        {
            if(find_data_array(&minterms_input,i)==1 || find_data_array(&dont_cares_input,i)==1)
            {
                add_minterms(i);
            }
        }
    }
    (&implicants_table)->number_prime_implicants=0;
    initialize_table();
    pairing();
    display_table();
    // printf("\nSimplified Boolean Expression:\n");
    filtering();
    // printf("\n");
}

// Menambah minterm pada variabel
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

// Membuat/inisiasi minterm
minterm* create_minterm(int n)
{
    int b=bits_size-1;
    minterm *p;
    p=(minterm *)malloc(sizeof(minterm));
    if(p==NULL)
    {
        // printf("Insertion failed!\n");
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

// Inisiasi tabel implicant
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

// Memasangkan/membandingkan antar minterm
void pairing()
{
    minterm *p,*q;
    int match=0;
    static int iteration=1;
    p=minterms_cache;
    q=p;
    
    if(reset==1)
    {
        iteration=1;
        reset=0;
    }
    // printf("\nIteration %d\n",iteration);
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

// Menampilkan minterm
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
            // printf("%d,",find_data_array(&(p->paired_minterms),count));
            count++;
        }
        // printf("\b");
        count=0;
        // printf("   ");
        while(i<bits_size)
        {
            if(find_data_array(&(p->binary),i)==-1)
            {
                // printf("%c",'-');
            }
            else
            {
                // printf("%d",find_data_array(&(p->binary),i));
            }
            i++;
        }
        // printf("\n");
        n++;
        p=p->next;
    }
}

// Mengidentifikasi pasangan minterm
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

// Menambahkan pasangan minterm pada variabel
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

// Inisiasi pasangan minterm
minterm* create_pair(minterm *m1, minterm *m2)
{
    int i,j;
    minterm *p;
    p=(minterm *)malloc(sizeof(minterm));
    if(p==NULL)
    {
        // printf("Failed to create the pair");
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

// Menambahkan minterm ke tabel
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

// Mengecek ada/tidaknya dont care
int check_dont_care(int n)
{
    if(find_data_array(&dont_cares_input,n)==1)
    {
        return 1;
    }
    return 0;
}

// Menampilkan tabel implicant
void display_table()
{
    int i,j;
    // printf("\nPrime Implicants Table:\n");
    for(i=0;i<(&implicants_table)->number_prime_implicants;i++)
    {
        convert_binary_to_minterm(table_minterms,i);
        for(j=0;j<max_minterms;j++)
        {
            if(find_data_matrix(&((&implicants_table)->brr),i,j)==1)
            {
                // printf("   %d  ",j);;
            }
        }
        // printf("\n");
    }
}

// Melakukan penyederhanaan dengan mencari essential prime implicant
void filtering()
{
    int i,j,n,row,first=1;
    char *temp;
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
                // printf(" + ");
            }
            else
            {
                first=0;
            }
            convert_binary_to_minterm(epi_minterms,find_data_array(&essential_prime_implicant,i));
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
            // printf(" + ");
            char *plus=" + ";
            strcat(result,plus);
        }
        else
        {
            first=0;
        }
        convert_binary_to_minterm(result,row);
        remove_minterm(row);
    }
    // printf("\b");
}

// Menghitung banyak implicant
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

// Mencari nilai maks dari banyak minterm
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

// Mengisi nilai binary
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

// Melakukan konversi dari binary ke minterm
void convert_binary_to_minterm(char *str, int n)
{
    int i=0;
    char non_complement[10][10]={"a","b","c","d","e","f","g","h"};
    char complement[10][10]={"!a","!b","!c","!d","!e","!f","!g","!h"};
    while(i!=bits_size)
    {
        if(find_data_matrix(&((&implicants_table)->arr),n,i)!=-1)
        {
            if(find_data_matrix(&((&implicants_table)->arr),n,i)==1)
            {
                // printf("%s",non_complement[i]);
                strcat(str,non_complement[i]);
            }
            else
            {
                // printf("%s",complement[i]);
                strcat(str,complement[i]);
            }
        }
        i++;
    }
}

// Menghapus minterm
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

// Melakukan pengecekan terhadap input minterm atau dont care
int entry_err_handling(int n)
{
    if(n<0 || n>max_minterms-1)
    {
        return 1;
    }
    return 0;
}

// Melakukan reset pada variabel
void reset_variables()
{
    minterms_cache=NULL;
    paired_minterms_cache=NULL;
    result[0]='\0';
    epi_minterms[0]='\0';
    table_minterms[0]='\0';
}