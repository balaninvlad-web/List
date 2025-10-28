#include "list_functions.h"

int main ()
{
    my_list_t List ={};

    ListCtor (&List, 5);

    #ifdef DEBUG
        verificator (&List, __FILE__, __func__ ,__LINE__);
    #endif

    Insert_at (&List, 1, 1);

    Insert_at (&List, 1, 2);

    Insert_at (&List, 2, 3);

    Delite_at (&List, 2);

    #ifdef DEBUG
        verificator (&List, __FILE__, __func__ ,__LINE__);

        ListDump(&List, 1, __FILE__, __func__ ,__LINE__);
    #endif

    Create_log_file (&List);

    ListDtor (&List);
}
