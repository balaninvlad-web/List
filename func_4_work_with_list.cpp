#include "list_functions.h"

ListErr_t Insert_at (my_list_t* List, int Index, int Value)
{
    if (Index < 0 || (unsigned)Index > List->capacity)
    {
        fprintf(stderr,"ERORR: bad index\n");
        return ERORRINDEX;
    }

    if (List->first_free_pos == 0)
    {
        fprintf(stderr, "ERORR: no free space. first_free_pos = 0\n");
        return ERORRINDEX;
    }

    #ifdef DEBUG
        verificator(List, __FILE__, __func__, __LINE__);
    #endif

    index_t New_node = List->first_free_pos;

    if (New_node == 0)
    {
        fprintf(stderr, "ERORR: no free space\n");
        return ERORRINDEX;
    }

    List->first_free_pos = List->next[New_node];

    if (Index == 0)
    {
        List->next[New_node] = List->head;

        List->prev[New_node] = 0;

        if (List->head != 0)
        {
            List->prev[List->head] = New_node;
        }
        else
        {
            List->tail = New_node;
        }

        List->head = New_node;
    }
    else
    {
        List->next[New_node] = List->next[Index];

        List->prev[New_node] = Index;

        if (List->next[Index] != 0)
        {
            List->prev[List->next[Index]] = New_node;
        }
        else
        {
            List->tail = New_node;
        }

        List->next[Index] = New_node;
    }

    List->data[New_node] = Value;

    List->prev[New_node] = (List->prev[New_node] == -1) ? Index : List->prev[New_node];

    List->Size++;

    #ifdef DEBUG
        verificator(List, __FILE__, __func__, __LINE__);
    #endif

    return NOERORR;
}

ListErr_t Delite_at(my_list_t* List, int Index)
{
    #ifdef DEBUG
        verificator(List, __FILE__, __func__, __LINE__);
    #endif

    if (Index < 0 || (unsigned)Index > List->capacity)
    {
        fprintf(stderr, "ERORR: bad index\n");
        return ERORRINDEX;
    }

    int Previous_Index = List->prev[Index];

    int Next_Index = List->next[Index];

    List->next[Previous_Index] = Next_Index;

    List->prev[Next_Index] = Previous_Index;

    List->next[Index] = List->first_free_pos;

    List->prev[Index] = -1;

    List->data[Index] = 0;

    #ifdef DEBUG
        verificator(List, __FILE__, __func__, __LINE__);
    #endif

    return NOERORR;
}

