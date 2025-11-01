#ifndef LIST_FUNCTIONS_H
#define LIST_FUNCTIONS_H
#include <TXLib.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>


typedef int ListErr_t;
typedef double data_t;
typedef int next_t;
typedef int prev_t;

typedef struct
{
    data_t* data;
    next_t* next;
    prev_t* prev;
    size_t  capacity;
    size_t  size;
} my_list_t;

enum nameOFerr {
    NOERORR                  = 0,
    ERORRINDEX               = 1 << 0,
    ERORRLISTNULL            = 1 << 1,
    ERORRDATANULL            = 1 << 2,
    ERORRNEXTNULL            = 1 << 3,
    ERORRPREVNULL            = 1 << 4,
    ERORRBADTAIL             = 1 << 5,
    ERORRBADHEAD             = 1 << 6,
    ERORRSIZE                = 1 << 7,
    ERORRSIZEZERO            = 1 << 8,
    ERORRREALOC              = 1 << 9,
    ERORRCYCLE               = 1 << 10,
    ERORRFIRSTFREEPOS        = 1 << 11,
    ERORRFREEPOS             = 1 << 12,
    ERORRHEADNOTFIRST        = 1 << 13,
    ERORRTAILNOTFIRST        = 1 << 14,
    ERROR_EMPTY_BUT_HAS_SIZE = 1 << 15,

};

extern const int START_OF_DATA;
extern const int BITS_IN_BYTE;
extern const int DUMMY_ELEMENT_POS;

ListErr_t ListCtor (my_list_t* List, int Capacity);
ListErr_t ListDtor (my_list_t* List);
ListErr_t Realocation_list (my_list_t* list);

ListErr_t verificator (my_list_t* List, const char* file, const char* func, int line);
ListErr_t ListDump (my_list_t* List, int i, const char* file, const char* func, int line);
void Make_html_file(my_list_t* list, const char* func);

void Change_of_type(int i);

ListErr_t Insert_after (my_list_t* List, int Index, int Value);
ListErr_t Insert_before (my_list_t* List, int Index, int Value);
ListErr_t Delite_at(my_list_t* List, int Index);

size_t Get_first_free_pos(my_list_t* List);
size_t Get_head(my_list_t* List);
size_t Get_tail(my_list_t* List);
size_t Get_list_size(my_list_t* List);

ListErr_t Set_first_free_pos(my_list_t* List, size_t value);
ListErr_t Set_head(my_list_t* List, size_t value);
ListErr_t Set_tail(my_list_t* List, size_t value);

bool Has_cycles (my_list_t* list);
bool Is_valid_node(my_list_t* List, size_t node);
bool Is_free_node(my_list_t* List, size_t node);
int Check_and_find_first_before_insert (my_list_t* List, int Index, size_t* First_free);

#endif

