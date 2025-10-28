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
typedef int index_t;


typedef struct
{
    data_t* data;
    next_t* next;
    prev_t* prev;
    index_t first_free_pos;
    index_t head;
    index_t tail;
    size_t  capacity;
    size_t  Size;
} my_list_t;

enum nameOFerr {
    NOERORR           = 0,
    ERORRINDEX        = 1 << 0,
    ERORRLISTNULL     = 1 << 1,
    ERORRDATANULL     = 1 << 2,
    ERORRNEXTNULL     = 1 << 3,
    ERORRPREVNULL     = 1 << 4,
    ERORRCANAREICADATA = 1 << 5,
    ERORRCANAREICANEXT = 1 << 6,
    ERORRCANAREICAPREV = 1 << 7,
    ERORRBADTAIL      = 1 << 8,
    ERORRBADHEAD      = 1 << 9
};

extern const int START_OF_DATA;
extern const int CANAREICA1;

ListErr_t ListCtor (my_list_t* List, int capacity);
ListErr_t ListDtor (my_list_t* List);

ListErr_t verificator (my_list_t* List, const char* file, const char* func, int line);
ListErr_t ListDump (my_list_t* List, int i, const char* file, const char* func, int line);

void change_of_type(int i);

ListErr_t Insert_at (my_list_t* List, int Index, int Value);
ListErr_t Delite_at(my_list_t* List, int Index);

void Create_log_file (my_list_t* List);

#endif

