#include "list_functions.h"
#include "create_log_file.h"

const int FREE_VALUE_PREV = -1;

ListErr_t Insert_after (my_list_t* list, int index, int value)
{
    size_t first_free = DUMMY_ELEMENT_POS;

    Check_and_find_first_before_insert (list, index, &first_free);

    size_t new_node = first_free;

    if (Set_first_free_pos(list, list->next[new_node]) != NOERORR)
    {
        return ERORRINDEX;
    }

    list->data[new_node] = value;

    list->next[new_node] = list->next[index];

    list->prev[new_node] = index;

    if (list->next[index] != DUMMY_ELEMENT_POS)
    {
        list->prev[list->next[index]] = new_node;
    }
    else
    {
        if (Set_tail(list, new_node) != NOERORR)
        {
            return ERORRINDEX;
        }
    }

    list->next[index] = new_node;

    if (index == DUMMY_ELEMENT_POS)
    {
        if (Set_head(list, new_node) != NOERORR)
        {
            return ERORRINDEX;
        }
    }

    list->size++;

    #ifdef DEBUG
        verificator(list, __FILE__, __func__, __LINE__);
    #endif

    return NOERORR;
}

ListErr_t Insert_before (my_list_t* list, int index, int value)
{
    size_t first_free = DUMMY_ELEMENT_POS;

    if (Is_free_node(list, index))
    {
        fprintf(stderr, "ERORR: cannot insert before free node\n");
        return ERORRINDEX;
    }

    Check_and_find_first_before_insert (list, index, &first_free);

    size_t new_node = first_free;

    if (Set_first_free_pos(list, list->next[new_node]) != NOERORR)
    {
        return ERORRINDEX;
    }

    list->data[new_node] = value;

    list->next[new_node] = index;

    list->prev[new_node] = list->prev[index];

    if(list->next[index] != DUMMY_ELEMENT_POS)
    {
        list->next[list->prev[index]] = new_node;
    }
    else
    {
        if (Set_tail(list, new_node) != NOERORR)
        {
            return ERORRINDEX;
        }
    }

    list->prev[index] = new_node;

    list->size++;

    #ifdef DEBUG
        verificator(list, __FILE__, __func__, __LINE__);
    #endif

    return NOERORR;
}

ListErr_t Delite_at(my_list_t* list, int index)
{
    #ifdef DEBUG
        verificator(list, __FILE__, __func__, __LINE__);
    #endif

    if (index < DUMMY_ELEMENT_POS || (unsigned)index > list->capacity)
    {
        fprintf(stderr, "ERORR: bad index\n");
        return ERORRINDEX;
    }

    if (index == DUMMY_ELEMENT_POS || Is_free_node(list, index))
    {
        fprintf(stderr, "ERORR: cannot delete free node or zero node\n");
        return ERORRINDEX;
    }

    int previous_index = list->prev[index];

    int next_index = list->next[index];

    list->next[previous_index] = next_index;

    list->prev[next_index] = previous_index;

    if (previous_index != DUMMY_ELEMENT_POS)
    {
        list->next[previous_index] = next_index;
    }
    else
    {
        Set_head(list, next_index);
    }

    if (next_index != DUMMY_ELEMENT_POS)
    {
        list->prev[next_index] = previous_index;
    }
    else
    {
        Set_tail(list, previous_index);
    }

    list->next[index] = Get_first_free_pos(list);

    list->prev[index] = FREE_VALUE_PREV;

    list->data[index] = DUMMY_ELEMENT_POS;

    Set_first_free_pos(list, index);

    list->size--;

    #ifdef DEBUG
        verificator(list, __FILE__, __func__, __LINE__);
    #endif

    return NOERORR;
}



