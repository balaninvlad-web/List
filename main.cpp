#include "list_functions.h"
#include "create_log_file.h"

int main ()//int argc, char* argv[]
{
    my_list_t List ={};

    ListCtor (&List, 10);

    #ifdef DEBUG
        verificator (&List, __FILE__, __func__ ,__LINE__);
    #endif

    Insert_after (&List, 0, 1);

    ListDump(&List, 0, __FILE__, __func__ ,__LINE__);

    Insert_after (&List, 1, 2);

    ListDump(&List, 0, __FILE__, __func__ ,__LINE__);

    Insert_after (&List, 1, 3);

    ListDump(&List, 0, __FILE__, __func__ ,__LINE__);

    Insert_before (&List, 1, 4);

    ListDump(&List, 0, __FILE__, __func__ ,__LINE__);

    Delite_at (&List, 2);

    ListDump(&List, 0, __FILE__, __func__ ,__LINE__);

    Insert_after (&List, 1, 5);

    ListDump(&List, 0, __FILE__, __func__ ,__LINE__);

    Insert_after (&List, 2, 6);

    ListDump(&List, 0, __FILE__, __func__ ,__LINE__);

    Delite_at (&List, 2);

    ListDump(&List, 0, __FILE__, __func__ ,__LINE__);

    Insert_after (&List, 0, 1);

    ListDump(&List, 0, __FILE__, __func__ ,__LINE__);

    Insert_after (&List, 1, 2);

    ListDump(&List, 0, __FILE__, __func__ ,__LINE__);

    Insert_after (&List, 1, 3);

    ListDump(&List, 0, __FILE__, __func__ ,__LINE__);

    Insert_before (&List, 1, 4);

    ListDump(&List, 0, __FILE__, __func__ ,__LINE__);

    #ifdef DEBUG
        verificator (&List, __FILE__, __func__ ,__LINE__);

        ListDump(&List, 0, __FILE__, __func__ ,__LINE__);
    #endif

    Create_log_file (&List);

    ListDtor (&List);
}
