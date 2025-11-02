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
    if (Get_first_free_pos == 0)
    {
        counter_of_err |= ERORRFIRSTFREEPOS;    // 10 ошибка
    }
    if (Get_first_free_pos(list) != 0 && !Is_free_node(list, Get_first_free_pos(list)))
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

    while (free_pos != 0 && free_pos <= (int)list->capacity)
    {
        fprintf(stderr, "%d -> ", free_pos);
        free_pos = list->next[free_pos];
        if ((unsigned)free_pos == Get_first_free_pos(list)) break;
    }

    fprintf(stderr, "\n======:END OF list:======\n\n");

    Make_html_file(list, func);

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
        list->data[i] = 0;

        if (i == new_capacity)
        {
            list->next[i] = 0;
        }
        else
        {
            list->next[i] = i + 1;
        }

        list->prev[i] = -1;
    }

    if (Get_first_free_pos(list) == 0)
    {
        Set_first_free_pos(list, list->capacity);
    }
    else
    {
        int current = Get_first_free_pos(list);

        while (list->next[current] != 0)
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
bool Has_cycles (my_list_t* list)
{
    if (Get_head(list) == 0)
    {
        return false;
    }

    bool* visited = (bool*)calloc(list->capacity + 1, sizeof(bool));

    int actual_index = Get_head(list);

    while (actual_index != 0)
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

bool Is_valid_node (my_list_t* List, size_t node)
{
    return (node > 0 && (unsigned)node <= List->capacity && List->prev[node] != -1);
}

bool Is_free_node (my_list_t* List, size_t node)
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

void Make_html_file(my_list_t* list, const char* func)
{
    static int dump_counter = 1;

    int result = 0;

    if (dump_counter == 1)
    {
        system("mkdir -p imagesDump");
    }

    static FILE* html_file = NULL;

    char graph_filename[50];
    char dot_filename[50];
    char command[100];

    printf("=== Make_html_file called #%d ===\n", dump_counter);

    if (html_file == NULL)
    {
        Create_head_html (&html_file);
    }

    fprintf(html_file, "<div class='dump'>\n");
    fprintf(html_file, "<h2>Dump %d Called from: <font color=white>%s</font></h2>\n", dump_counter, func);

    if (list == NULL)
    {
        printf("ERROR: list is NULL\n");
        fprintf(html_file, "<p><b>ERROR: List is NULL</b></p>\n");
        fprintf(html_file, "</div>\n");
        fflush(html_file);
        dump_counter++;
        return;
    }

    snprintf(graph_filename, sizeof(graph_filename), "graph_%d.png", dump_counter);

    snprintf(dot_filename, sizeof(dot_filename), "graph_%d.dot", dump_counter);

    Create_log_file(list, dot_filename);

    snprintf(command, sizeof(command), "dot -Tpng %s -o imagesDump/%s", dot_filename, graph_filename);

    result = system(command);

    if (result != 0)
    {
            printf("WARNING: Graph generation failed for dump #%d\n", dump_counter);
    }

    remove(dot_filename);

    Create_table (html_file, list);

    fprintf(html_file, "<h3>Graph Visualization</h3>\n");

    fprintf(html_file, "<img src='imagesDump/%s' alt='Graph %d' width='1500'>\n", graph_filename, dump_counter);

    Create_text_diagram (html_file, list);

    fprintf(html_file, "</pre>\n");
    fprintf(html_file, "</div>\n");

    fflush(html_file);
    printf("HTML dump #%d completed\n", dump_counter);

    dump_counter++;
}

void Create_head_html (FILE** html_file)
{
    printf("Creating HTML file...\n");
    *html_file = fopen("All_dumps.html", "w");

    //html_file = *html_file;

    if (html_file == NULL)
    {
        printf("ERROR: Cannot create HTML file\n");
        return;
    }

    fprintf(*html_file, "<!DOCTYPE html>\n");
    fprintf(*html_file, "<html lang='ru'>\n");
    fprintf(*html_file, "<head>\n");
    fprintf(*html_file, "<meta charset='UTF-8'>\n");
    fprintf(*html_file, "<title>List Dumps</title>\n");
    fprintf(*html_file, "<style>\n");
    /* Основные цвета */
    fprintf(*html_file, "body { background-color: #001f29; color: #ffffff; }\n");
    /* Таблица */
    fprintf(*html_file, "table { border-collapse: collapse; margin: 10px; }\n");
    fprintf(*html_file, "th, td { border: 1px solid #0077a3; padding: 5px; }\n");
    fprintf(*html_file, "th { background-color: #00415a; }\n");
    /* Картинки */
    fprintf(*html_file, "img { max-width: 100%%; height: auto; margin: 10px; }\n");
    /* Блоки дампов */
    fprintf(*html_file, ".dump { border: 2px solid #0099cc; padding: 15px; margin: 10px; }\n");
    /* Заголовки */
    fprintf(*html_file, "h1 { color: #00ccff; }\n");
    fprintf(*html_file, "h2 { color: #00b8e6; }\n");
    fprintf(*html_file, "h3 { color: #66d9ff; }\n");
    /* Текстовая диаграмма */
    fprintf(*html_file, "pre { color: #b3ecff; background-color: #001a21; padding: 10px; }\n");
    fprintf(*html_file, "</style>\n");
    fprintf(*html_file, "</head>\n");
    fprintf(*html_file, "<body>\n");
    fprintf(*html_file, "<h1 style='color: red;'>ALL LIST DUMPS</h1>\n");
    printf("HTML file created successfully\n");
}

void Create_table (FILE* html_file, my_list_t* list)
{
    fprintf(html_file, "<p><b>Head:</b> %d, <b>Tail:</b> %d, <b>Free:</b> %d, <b>Size:</b> %d, <b>Capacity:</b> %d</p>\n",
            Get_head(list), Get_tail(list), Get_first_free_pos(list), list->size, list->capacity);

    fprintf(html_file, "<table border='1' style='border-collapse: collapse;'>\n");

    fprintf(html_file, "<tr><th>Index</th><th>Data</th><th>Next</th><th>Prev</th><th>Status</th></tr>\n");

    for (size_t i = 0; i <= list->capacity; i++)
    {
        const char* status = "USED";
        if (i == 0) status = "SERVICE";
        else if (list->prev[i] == -1) status = "FREE";

        fprintf(html_file, "<tr><td>%u</td><td>%.2lf</td><td>%d</td><td>%d</td><td>%s</td></tr>\n",
                (unsigned)i, list->data[i], list->next[i], list->prev[i], status);
    }

    fprintf(html_file, "</table>\n");
}

void Create_text_diagram (FILE* html_file, my_list_t* list)
{
    int actual_capacity = list->capacity;

    fprintf(html_file, "<h3>Text Diagram:</h3>\n");
    fprintf(html_file, "<pre>\n");

    int current = Get_head(list);
    fprintf(html_file, "HEAD → ");

    if (current == 0)
    {
        fprintf(html_file, " → TAIL\n");
    } else
    {
        int count = 0;
        while (current != 0 && current <= actual_capacity && count < 20)
        {
            fprintf(html_file, "[%d]", current);
            current = list->next[current];
            if (current != 0 && current <= actual_capacity)
            {
                fprintf(html_file, " → ");
            }
            count++;
        }
        fprintf(html_file, " → TAIL\n");
    }
}
