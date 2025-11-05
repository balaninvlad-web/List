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

} my_list_t;

enum nameOFerrWORK
{
    NOERORR,
    ERORRLISTNULL,
    ERORRDATANULL,
    ERORRNEXTNULL,
    ERORRPREVNULL,
    ERORRCANAREICADATA,
    ERORRCANAREICANEXT,
    ERORRCANAREICAPREV,
    ERORRBADTAIL,
    ERORRBADHEAD,
    ERORRINDEX,
    // TODO дописать енамы эрорров
};

enum nameOFerr
{
    ERORRLISTNULL       = 1 << 1,
    ERORRDATANULL       = 1 << 2,
    ERORRNEXTNULL       = 1 << 3,
    ERORRPREVNULL       = 1 << 4,
    ERORRCANAREICADATA  = 1 << 5,
    ERORRCANAREICANEXT  = 1 << 6,
    ERORRCANAREICAPREV  = 1 << 7,
    ERORRBADTAIL        = 1 << 8,
    ERORRBADHEAD        = 1 << 9,
    ERORRINDEX          = 1 << 10,
};

extern const int START_OF_DATA;
extern const int CANAREICA1;

ListErr_t StackCtor (my_list_t* stk1);
ListErr_t StackDtor (my_list_t* stk1);
ListErr_t verificator (my_list_t* stk1, const char* file, const char* func, int line);
ListErr_t StackDump (my_list_t* stk1, int i, const char* file, const char* func, int line);

#endif

