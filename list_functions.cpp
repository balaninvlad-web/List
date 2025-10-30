#include "list_functions.h"
#include "create_log_file.h"

const int START_OF_DATA = 1;
const int BITS_IN_BYTE = 8;

ListErr_t ListCtor (my_list_t* list, int capacity)
{
    assert(list);

    list->capacity = capacity;

    list->data = (data_t*)calloc(list->capacity + 2, sizeof(data_t));
    list->next = (next_t*)calloc(list->capacity + 2, sizeof(next_t));
    list->prev = (prev_t*)calloc(list->capacity + 2, sizeof(prev_t));

    if (!list->data || !list->next || !list->prev)
    {
        return ERORRDATANULL;
    }
    // TODO сделать в функцию когда напишу реалокацию
    for (size_t i = START_OF_DATA; i < list->capacity; i++)
    {
        if(i == list->capacity)
            list->next[i] = 0;
        else
            list->next[i] = i + 1;
    }

    for (size_t i = 1; i <= list->capacity; i++)
    {
        list->prev[i] = -1;
    }

    list->data[0] = 1;

    list->next[0] = 0;

    list->prev[0] = 0;

    Set_first_free_pos(list, 1);
    list->size = 0;

    #ifdef DEBUG
        verificator (list, __FILE__, __func__ ,__LINE__);
    #endif

    return NOERORR;
}

ListErr_t ListDtor (my_list_t* list)
{
    if (list == NULL)
    {
        fprintf(stderr, "ERORR: list is NULL before Destructor\n");
        return ERORRLISTNULL;
    }

    if (list->data == NULL)
    {
        fprintf(stderr, "ERORR: data is NULL before Destructor\n");
        return ERORRDATANULL;
    }

    if (list->next == NULL)
    {
        fprintf(stderr, "ERORR: next is NULL before Destructor\n");
        return ERORRNEXTNULL;
    }

    if (list->prev == NULL)
    {
        fprintf(stderr, "ERORR: prev is NULL before Destructor\n");
        return ERORRPREVNULL;
    }

    #ifdef DEBUG
        verificator (list, __FILE__, __func__, __LINE__);
    #endif

    free (list->data);
    free (list->next);
    free (list->prev);
    list->capacity = 0;

    return NOERORR;
}

ListErr_t Realocation_list (my_list_t* list)
{
    assert(list);

    data_t* old_data = list->data;
    next_t* old_next = list->next;
    prev_t* old_prev = list->prev;

    size_t new_capacity = 2 * capacity;

    data_t* new_data = (data_t*)realloc(list->data, new_capacity * sizeof(data_t));

    next_t* new_next = (next_t*)realloc(list->next, new_capacity * sizeof(next_t));

    prev_t* new_prev = (prev_t*)realloc(list->prev, new_capacity * sizeof(prev_t));

    if (!new_data || !new_next || !new_prev)
    {
        fprintf("hrjdosk");
        if (new_data) free(new_data);
        if (new_next) free(new_next);
        if (new_prev) free(new_prev);

        list->data = old_data;
        list->next = old_next;
        list->prev = old_prev;

        return -1;
    }

    list->data = new_data;
    list->next = new_next;
    list->prev = new_prev;

    for(size_t i = capacity; i < new_capacity; i++)
    {
        list->data[i] = 0;

        if (i == new_capacity - 1)
        {
            list->next[i] = 0;
        }
        else
        {
            list->next[i] = i + 1;
        }

        list->prev[i] = -1;
    }



}

ListErr_t verificator (my_list_t* list, const char* file, const char* func, int line)
{
    int counter_of_err = 0;

    if (list == NULL) // 1 ошибка
    {
        counter_of_err |= ERORRLISTNULL;
    }
    if (list->data == NULL) // 2 ошибка
    {
        counter_of_err |= ERORRDATANULL;
    }
    if (list->next == NULL) // 3 ошибка
    {
        counter_of_err |= ERORRNEXTNULL;
    }
    if (list->prev == NULL) // 4 ошибка
    {
        counter_of_err |= ERORRPREVNULL;
    }
    /*if (list->next[list->prev[0]] != 0) // 5 ошибка
    {
       counter_of_err |= ERORRBADTAIL;
    }
    if (list->prev[list->next[0]] != 0) // 6 ошибка
    {
        counter_of_err |= ERORRBADHEAD;
    }*/
    if (list->size != Get_list_size(list))
    {
        counter_of_err |= ERORRSIZE; // 7 ошибка
    }
    if (Get_head(list) == Get_tail(list))
    {
        counter_of_err |= ERORRSIZEZERO; // 8 ошибка
    }
    if (counter_of_err > 0)
        ListDump(list, counter_of_err, file, func, line); // dont call dump here

    return counter_of_err;
}

ListErr_t ListDump (my_list_t* list, int counter_of_err, const char* file, const char* func, int line)
{
    fprintf(stderr, "\n======:list DUMP:======\n\n");

    fprintf(stderr, "Capacity: %lu\n", (unsigned long)list->capacity);
    fprintf(stderr, "Head: %d \n Tail: %d\n", Get_head(list), Get_tail(list));
    fprintf(stderr, "First free: %d\n", Get_first_free_pos(list));
    fprintf(stderr, "size: %lu\n", (unsigned long)list->size);

    printf("\n");

    fprintf(stderr, "list <int> in file: %s, function: %s, in line: %d\n", file, func, line);

    fprintf(stderr, "kod oshibki: %d\n", counter_of_err);

    fprintf(stderr, "dvoichnii kod ochibki: ");

    Change_of_type(counter_of_err);

    //fprintf(stderr, "\nSORTEDED DATA:");

    fprintf(stderr, "\nDATA:");
    for (size_t i = 0; i < list->capacity; i++)
    {
        fprintf(stderr,"[%d]%.2lf", i, list->data[i]);
    }

    fprintf(stderr, "\nNEXT:");
    for (size_t i = 0; i < list->capacity; i++)
    {
        fprintf(stderr,"[%d]%d   ", i, list->next[i]);
    }

    fprintf(stderr, "\nPREV:");
    for (size_t i = 0; i < list->capacity; i++)
    {
        fprintf(stderr,"[%d]%d   ", i, list->prev[i]);
    }

    printf("\nFree chain: ");

    int free_pos = Get_first_free_pos(list);

    while (free_pos != 0 && free_pos <= (int)list->capacity)
    {
        fprintf(stderr, "%d -> ", free_pos);
        free_pos = list->next[free_pos];
        if ((unsigned)free_pos == Get_first_free_pos(list)) break;
    }

    fprintf(stderr, "\n======:END OF list:======\n\n");

    Create_log_file (list);

    return 0;
}

void Change_of_type(int i)
{
    for (int j = (sizeof(int)*BITS_IN_BYTE - 1); j >= 0; j--)
        fprintf(stderr, "%d", (i >> j) & 1);
}

// Геттеры
size_t Get_first_free_pos(my_list_t* List)
{
    assert(List);
    assert(List->data);

    if (List->data[0] < 0 || (size_t)List->data[0] > List->capacity)
    {
        fprintf(stderr, "ERROR: invalid first_free_pos value: %lf\n", List->data[0]);
        return 0;
    }
    return (size_t)List->data[0];
}

size_t Get_head(my_list_t* List)
{
    assert(List);
    assert(List->next);

    if (List->next[0] < 0 || (size_t)List->next[0] > List->capacity)
    {
        fprintf(stderr, "ERROR: invalid head value: %d\n", List->next[0]);
        return 0;
    }

    return List->next[0];
}

size_t Get_tail(my_list_t* List)
{
    assert(List);
    assert(List->prev);

    if (List->prev[0] < 0 || (size_t)List->prev[0] > List->capacity)
    {
        fprintf(stderr, "ERROR: invalid tail value: %d\n", List->prev[0]);
        return 0;
    }

    return List->prev[0];
}

size_t Get_list_size(my_list_t* List)
{
    size_t count_elem = 0;
    size_t current = Get_head(List);
    while (current != 0 && Is_valid_node(List, current))
    {
        count_elem++;
        current = List->next[current];
        if (current == Get_head(List)) break;
    }
    return count_elem;
}

// Сеттеры
ListErr_t Set_first_free_pos(my_list_t* List, size_t value)
{
    assert(List);
    assert(List->data);

    if (value == 0 || (size_t)value > List->capacity)
    {
        fprintf(stderr, "ERROR: trying to set invalid first_free_pos: %d\n", value);
        return ERORRINDEX;
    }

    List->data[0] = value;

    return NOERORR;
}

ListErr_t Set_head(my_list_t* List, size_t value)
{
    assert(List);
    assert(List->next);

    if (value != 0 && ((size_t)value > List->capacity || List->prev[value] == -1))
    {
        fprintf(stderr, "ERROR: trying to set invalid head: %d\n", value);
        return ERORRINDEX;
    }

    List->next[0] = value;
    return NOERORR;
}

ListErr_t Set_tail(my_list_t* List, size_t value)
{
    assert(List);
    assert(List->prev);

    if (value != 0 && ((size_t)value > List->capacity || List->prev[value] == -1))
    {
        fprintf(stderr, "ERROR: trying to set invalid tail: %d\n", value);
        return ERORRINDEX;
    }

    List->prev[0] = value;

    return NOERORR;
}

// чеки

bool Is_valid_node(my_list_t* List, size_t node)
{
    return (node > 0 && (unsigned)node <= List->capacity && List->prev[node] != -1);
}

bool Is_free_node(my_list_t* List, size_t node)
{
    return (node > 0 && (unsigned)node <= List->capacity && List->prev[node] == -1);
}

int Check_and_find_first_before_insert (my_list_t* List, int Index, size_t* First_free)
{
    #ifdef DEBUG
        verificator(List, __FILE__, __func__, __LINE__);
    #endif

    if (Index < 0 || (unsigned)Index > List->capacity)
    {
        fprintf(stderr,"ERORR: bad index\n");
        return ERORRINDEX;
    }

    if (Index != 0 && Is_free_node(List, Index))
    {
        fprintf(stderr, "ERORR: cannot insert after free node\n");
        return ERORRINDEX;
    }

    *First_free = Get_first_free_pos(List);

    if (*First_free == 0)
    {
        fprintf(stderr, "ERORR: no free space. first_free_pos = 0\n");
        return ERORRINDEX;
    }

    return NOERORR;
}
