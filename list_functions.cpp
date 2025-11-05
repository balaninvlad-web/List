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

    list->first_free_pos = 1;

    list->data[0] = 0;
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

    FILE* html = fopen("all_dumps.html", "a");
    if (html)
    {
        fprintf(html, "</body></html>\n");
        fclose(html);
    }

    return NOERORR;
}

ListErr_t verificator (my_list_t* list, const char* file, const char* func, int line)
{
    int counter_of_err = 0;

    list->next[5] = 3000;

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
    if (list->size != Get_list_size(list))
    {
        counter_of_err |= ERORRSIZE; // 7 ошибка
    }
    if (Get_head(list) == Get_tail(list))
    {
        counter_of_err |= ERORRSIZEZERO; // 8 ошибка
    }
    if (Has_cycles(list))
    {
        counter_of_err |= ERORRCYCLE;   // 9 ошибка
    }
    if (list->first_free_pos == 0)
    {
        counter_of_err |= ERORRFIRSTFREEPOS;    // 10 ошибка
    }
    if (list->first_free_pos != 0 && !Is_free_node(list, Get_first_free_pos(list)))
    {
        counter_of_err |= ERORRFREEPOS; // 11 ошибка
    }
    if (Get_head(list) != 0 && !Is_valid_node(list, Get_head(list)))
    {
        counter_of_err |= ERORRBADHEAD; // 12 ошибка
    }
    if (Get_tail(list) != 0 && !Is_valid_node(list, Get_tail(list)))
    {
        counter_of_err |= ERORRBADTAIL; // 13 ошибка
    }
    if (Get_head(list) != 0 && list->prev[Get_head(list)] != 0)
    {
        counter_of_err |= ERORRHEADNOTFIRST;    // 14 ошибка
    }
    if (Get_tail(list) != 0 && list->next[Get_tail(list)] != 0)
    {
        counter_of_err |= ERORRTAILNOTFIRST;    // 15 ошибка
    }
    if (list->size > 0 && (Get_head(list) == 0 || Get_tail(list) == 0))
    {
        counter_of_err |= ERROR_EMPTY_BUT_HAS_SIZE; // 16 ошибка
    }
    if (counter_of_err > 0)
        ListDump(list, counter_of_err, file, func, line); // TODO: dont call dump here

    return counter_of_err;
}

ListErr_t ListDump (my_list_t* list, int counter_of_err, const char* file, const char* func, int line)
{
    assert (list);

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

    fprintf(stderr, "\nDATA:");
    for (size_t i = 0; i <= list->capacity; i++)
    {
        fprintf(stderr,"[%d]%.2lf", i, list->data[i]);
    }

    fprintf(stderr, "\nNEXT:");
    for (size_t i = 0; i <= list->capacity; i++)
    {
        fprintf(stderr,"[%d]%d   ", i, list->next[i]);
    }

    fprintf(stderr, "\nPREV:");
    for (size_t i = 0; i <= list->capacity; i++)
    {
        fprintf(stderr,"[%d]%d   ", i, list->prev[i]);
    }

    printf("\nFree chain: ");

    int free_pos = Get_first_free_pos(list);

    while (free_pos != DUMMY_ELEMENT_POS && free_pos <= (int)list->capacity)
    {
        fprintf(stderr, "%d -> ", free_pos);
        free_pos = list->next[free_pos];
        if ((unsigned)free_pos == Get_first_free_pos(list)) break;
    }

    fprintf(stderr, "\n======:END OF list:======\n\n");

    Create_dump_files(list, func);

    Create_log_file (list, "list_dump.dot");

    return 0;
}

void Change_of_type(int i)
{
    for (int j = (sizeof(int)*BITS_IN_BYTE - 1); j >= 0; j--)
        fprintf(stderr, "%d", (i >> j) & 1);
}

ListErr_t Realocation_list (my_list_t* list)
{
    assert(list);

    data_t* old_data = list->data;
    next_t* old_next = list->next;
    prev_t* old_prev = list->prev;

    size_t new_capacity = 2 * list->capacity;

    data_t* new_data = (data_t*)realloc(list->data, (new_capacity) * sizeof(data_t));

    next_t* new_next = (next_t*)realloc(list->next, (new_capacity) * sizeof(next_t));

    prev_t* new_prev = (prev_t*)realloc(list->prev, (new_capacity) * sizeof(prev_t));

    if (!new_data || !new_next || !new_prev)
    {
        fprintf(stderr, "ERORR: invalid realocatoin\n");

        if (new_data) free(new_data);
        if (new_next) free(new_next);
        if (new_prev) free(new_prev);

        list->data = old_data;
        list->next = old_next;
        list->prev = old_prev;

        return ERORRREALOC;
    }

    list->data = new_data;
    list->next = new_next;
    list->prev = new_prev;

    for(size_t i = list->capacity; i <= new_capacity; i++)
    {
        list->data[i] = DUMMY_ELEMENT_POS;

        if (i == new_capacity)
        {
            list->next[i] = DUMMY_ELEMENT_POS;
        }
        else
        {
            list->next[i] = i + 1;
        }

        list->prev[i] = -1;
    }

    if (list->first_free_pos == DUMMY_ELEMENT_POS)
    {
        list->first_free_pos = list->capacity;
    }
    else
    {
        int current = Get_first_free_pos(list);

        while (list->next[current] != DUMMY_ELEMENT_POS)
        {
            current = list->next[current];
        }

        list->next[current] = list->capacity;
    }

    list->next[new_capacity] = 0;

    list->capacity = new_capacity;

    return NOERORR;
}


// Геттеры
size_t Get_first_free_pos(my_list_t* list)
{
    assert(list);

    return list->first_free_pos;
}

size_t Get_head(my_list_t* list)
{
    assert(list);
    assert(list->next);

    if (list->next[DUMMY_ELEMENT_POS] < 0 || (size_t)list->next[DUMMY_ELEMENT_POS] > list->capacity)
    {
        fprintf(stderr, "ERROR: invalid head value: %d\n", list->next[DUMMY_ELEMENT_POS]);
        return 0;
    }

    return list->next[DUMMY_ELEMENT_POS];
}

size_t Get_tail(my_list_t* list)
{
    assert(list);
    assert(list->prev);

    if (list->prev[DUMMY_ELEMENT_POS] < 0 || (size_t)list->prev[DUMMY_ELEMENT_POS] > list->capacity)
    {
        fprintf(stderr, "ERROR: invalid tail value: %d\n", list->prev[DUMMY_ELEMENT_POS]);
        return 0;
    }

    return list->prev[DUMMY_ELEMENT_POS];
}

size_t Get_list_size(my_list_t* list)
{
    size_t count_elem = 0;
    size_t current = Get_head(list);
    while (current != DUMMY_ELEMENT_POS && Is_valid_node(list, current))
    {
        count_elem++;
        current = list->next[current];
        if (current == Get_head(list)) break;
    }
    return count_elem;
}

// Сеттеры
ListErr_t Set_first_free_pos(my_list_t* list, size_t value)
{
    assert(list);
    assert(list->data);

    if (value == DUMMY_ELEMENT_POS || (size_t)value > list->capacity)
    {
        fprintf(stderr, "ERROR: trying to set invalid first_free_pos: %d\n", value);
        return ERORRINDEX;
    }

    list->first_free_pos = value;

    return NOERORR;
}

ListErr_t Set_head(my_list_t* list, size_t value)
{
    assert(list);
    assert(list->next);

    if (value != DUMMY_ELEMENT_POS && ((size_t)value > list->capacity || list->prev[value] == -1))
    {
        fprintf(stderr, "ERROR: trying to set invalid head: %d\n", value);
        return ERORRINDEX;
    }

    list->next[DUMMY_ELEMENT_POS] = value;
    return NOERORR;
}

ListErr_t Set_tail(my_list_t* list, size_t value)
{
    assert(list);
    assert(list->prev);

    if (value != DUMMY_ELEMENT_POS && ((size_t)value > list->capacity || list->prev[value] == -1))
    {
        fprintf(stderr, "ERROR: trying to set invalid tail: %d\n", value);
        return ERORRINDEX;
    }

    list->prev[DUMMY_ELEMENT_POS] = value;

    return NOERORR;
}

// чеки
bool Has_cycles (my_list_t* list)
{
    if (Get_head(list) == DUMMY_ELEMENT_POS)
    {
        return false;
    }

    bool* visited = (bool*)calloc(list->capacity + 1, sizeof(bool));

    int actual_index = Get_head(list);

    while (actual_index != DUMMY_ELEMENT_POS)
    {
        if (visited[actual_index])
        {
            free(visited);
            return true;
        }

        visited[actual_index] = true;
        actual_index = list->next[actual_index];
    }

    free (visited);
    return false;
}

bool Is_valid_node (my_list_t* list, size_t node)
{
    return (node > 0 && (unsigned)node <= list->capacity && list->prev[node] != -1);
}

bool Is_free_node (my_list_t* list, size_t node)
{
    return (node > 0 && (unsigned)node <= list->capacity && list->prev[node] == -1);
}

int Check_and_find_first_before_insert (my_list_t* list, int Index, size_t* First_free)
{
    #ifdef DEBUG
        verificator(list, __FILE__, __func__, __LINE__);
    #endif

    if (Index < DUMMY_ELEMENT_POS || (unsigned)Index > list->capacity)
    {
        fprintf(stderr,"ERORR: bad index\n");
        return ERORRINDEX;
    }

    if (Index != DUMMY_ELEMENT_POS && Is_free_node(list, Index))
    {
        fprintf(stderr, "ERORR: cannot insert after free node\n");
        return ERORRINDEX;
    }

    *First_free = Get_first_free_pos(list);

    if (*First_free == DUMMY_ELEMENT_POS)
    {
        fprintf(stderr, "ERORR: no free space. first_free_pos = 0\n");
        return ERORRINDEX;
    }

    return NOERORR;
}


