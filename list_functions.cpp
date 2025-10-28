#include "list_functions.h"

const int START_OF_DATA = 1;
const int CANAREICA1 = 217217217;

ListErr_t ListCtor (my_list_t* List, int capacity)
{
    assert(List);

    List->capacity = capacity;

    List->data = (data_t*)calloc(List->capacity + 2, sizeof(data_t));
    List->next = (next_t*)calloc(List->capacity + 2, sizeof(next_t));
    List->prev = (prev_t*)calloc(List->capacity + 2, sizeof(prev_t));

    if (!List->data || !List->next || !List->prev)
    {
        return ERORRDATANULL;
    }

    for(size_t i = START_OF_DATA; i < List->capacity; i++)
    {
        if(i == List->capacity)
            List->next[i] = 0;
        else
            List->next[i] = i + 1;
    }

    for(size_t i = 1; i <= List->capacity; i++)
    {
        List->prev[i] = -1;
    }

    List->data[0] = CANAREICA1;

    List->next[0] = CANAREICA1;

    List->prev[0] = CANAREICA1;

    List->first_free_pos = 1;
    List->Size = 0;
    List->head = 0;
    List->tail = 0;

    #ifdef DEBUG
        verificator (List, __FILE__, __func__ ,__LINE__);
    #endif

    return NOERORR;
}

ListErr_t ListDtor (my_list_t* List)
{
    //stk1 -> sizestk = 1000000;

    #ifdef DEBUG
        verificator (List, __FILE__, __func__ ,__LINE__);
    #endif

    free(List->data);
    free(List->next);
    free(List->prev);
    List->capacity = 0;

    return NOERORR;
}

ListErr_t verificator (my_list_t* List, const char* file, const char* func, int line)
{
    int counter_of_err = 0;

    if (List == NULL)
    {
        counter_of_err |=ERORRLISTNULL;
    }
    if (List->data == NULL)
    {
        counter_of_err |=ERORRDATANULL;
    }
    if (List->next == NULL)
    {
        counter_of_err |=ERORRNEXTNULL;
    }
    if (List->prev == NULL)
    {
        counter_of_err |=ERORRPREVNULL;
    }
    if (fabs(List->data[0] - CANAREICA1) > 1e-6)
    {
        counter_of_err |= ERORRCANAREICADATA;
    }
    if (List->next[0] != CANAREICA1)
    {
        counter_of_err |=ERORRCANAREICANEXT;
    }
    if (List->prev[0] != CANAREICA1)
    {
        counter_of_err |=ERORRCANAREICAPREV;
    }
    if (List->next[List->tail] != 0)
    {
        counter_of_err |=ERORRBADTAIL;
    }
    if (List->prev[List->head] != 0)
    {
        counter_of_err |=ERORRBADHEAD;
    }
    if (counter_of_err > 0)
        ListDump(List, counter_of_err, file, func, line);

    return counter_of_err;
}

ListErr_t ListDump (my_list_t* List, int counter_of_err, const char* file, const char* func, int line)
{
    fprintf(stderr, "======:LIST DUMP:======\n\n");

    fprintf(stderr, "FIRST FREE POS: %d\n", List->first_free_pos);

    fprintf(stderr, "Capacity: %lu\n", (unsigned long)List->capacity);
    fprintf(stderr, "Head: %d, Tail: %d\n", List->head, List->tail);
    fprintf(stderr, "First free: %d\n", List->first_free_pos);
    fprintf(stderr, "Size: %lu\n", (unsigned long)List->Size);

    printf("\n");

    fprintf(stderr, "LIST <int> in file: %s, function: %s, in line: %d", file, func, line);

    fprintf(stderr, "kod oshibki: %d\n", counter_of_err);

    fprintf(stderr, "dvoichnii kod ochibki: ");

    change_of_type(counter_of_err);

    fprintf(stderr, "\nSORTEDED DATA:");

    fprintf(stderr, "\n\nDATA:");
    for (size_t i = 0; i < List->capacity; i++)
    {
        fprintf(stderr,"[%d]%.2lf", i, List->data[i]);
    }
    fprintf(stderr, "\nNEXT:");
    for (size_t i = 0; i < List->capacity; i++)
    {
        fprintf(stderr,"[%d]%d   ", i, List->next[i]);
    }
    fprintf(stderr, "\nPREV:");
    for (size_t i = 0; i < List->capacity; i++)
    {
        fprintf(stderr,"[%d]%d   ", i, List->prev[i]);
    }

    printf("\nFree chain: ");

    int free_pos = List->first_free_pos;

    while (free_pos != 0 && free_pos <= (int)List->capacity)
    {
        fprintf(stderr, "%d -> ", free_pos);
        free_pos = List->next[free_pos];
        if (free_pos == List->first_free_pos) break;
    }

    fprintf(stderr, "\n\n======:END OF LIST:======\n\n");

    return 0;
}

void change_of_type(int i)
{
    for (int j = 31; j >= 0; j--)
        fprintf(stderr, "%d", (i >> j) & 1);
}

void Create_log_file (my_list_t* List)
{
    assert(List);

    FILE* dot_file = fopen("list_dump.dot", "w");

    assert(dot_file);

    printf("START Writing TO file.....\n");

    fprintf(dot_file,"digraph List{\n");

    fprintf(dot_file, "    node [shape= Mrecord, style=filled, fontname=\"Arial\"];\n");

    fprintf(dot_file, "    edge [fontname=\"Arial\"];\n\n");

    fprintf(dot_file, "    subgraph cluster_main {\n");
    fprintf(dot_file, "        label=\"Linearized List\";\n");
    fprintf(dot_file, "        style=filled;\n");
    fprintf(dot_file, "        fillcolor=lightgray;\n");
    fprintf(dot_file, "        margin=20;\n\n");

    for (size_t i = 0; i <= List->capacity; i++)
    {

        printf("START WRITE BLOCKS Writing TO file.....\n");

        const char* fillcolor = "";
        const char* color = "black";

        if (i == 0)
        {
            fillcolor = "#FFB6C1";  // Канарейка
            color = "darkred";
        } else if (List->prev[i] == -1)
        {
            fillcolor = "#98FB98";  // Свободный узел
            color = "darkgreen";
        } else if (i == (unsigned)List->head && i == List->tail)
        {
            fillcolor = "#DDA0DD";  // Единственный элемент
            color = "purple";
        } else if (i == (unsigned)List->head)
        {
            fillcolor = "#FFFACD";  // Голова
            color = "darkgoldenrod";
        } else if (i == (unsigned)List->tail)
        {
            fillcolor = "#FFDAB9";  // Хвост
            color = "darkorange";
        } else
        {
            fillcolor = "#E6E6FA";  // Обычный узел
            color = "navy";
        }

        fprintf(dot_file, "        node%d [label=\"{{Index: %zu}|{Data: %.2lf}|{Next: %d}|{Prev: %d}}\", "
                "fillcolor=\"%s\", color=\"%s\", fontcolor=\"%s\"];\n",
                (int)i, i, List->data[i], List->next[i], List->prev[i], fillcolor, color, color);

        printf("BLOCKS Writing TO file.....\n");
    }

    fprintf(dot_file, "\n\n");

    fprintf(dot_file, "    // Next connections (main chain)\n");

    for (size_t i = 0; i <= List->capacity; i++)
    {
        if (List->next[i] != 0 && List->next[i] != CANAREICA1 &&
            List->next[i] <= (int)List->capacity && List->prev[List->next[i]] != -1)
        {
            printf("NEXT Writing TO file.....\n");

            fprintf(dot_file, "    node%d -> node%d [color=\"#2E8B57\", penwidth=2, weight=2];\n",
                   (int)i, List->next[i]);
        }
    }

    fprintf(dot_file, "    // Free nodes connections\n");

    int free_pos = List->first_free_pos;

    while (free_pos != 0 && free_pos <= (int)List->capacity)
    {
        int next_free = List->next[free_pos];

        if (next_free != 0 && next_free <= (int)List->capacity)
        {
            printf("FREE Writing TO file.....\n");

            fprintf(dot_file, "    node%d -> node%d [color=\"#32CD32\", style=dashed];\n",
                   free_pos, next_free);
        }
        free_pos = next_free;
    }

    fprintf(dot_file, "    // Head and Tail pointers\n");
    fprintf(dot_file, "    head [shape=triangle, color=red, fillcolor=red, style=filled, label=\"HEAD\"];\n");
    fprintf(dot_file, "    tail [shape=triangle, color=blue, fillcolor=blue, style=filled, label=\"TAIL\"];\n");
    fprintf(dot_file, "    free_ptr [shape=triangle, color=green, fillcolor=green, style=filled, label=\"FREE\"];\n");

    if (List->head > 0 && List->head <= List->capacity)
    {
        printf("HEAD Writing TO file.....\n");

        fprintf(dot_file, "    head -> node%d:<f0> [color=red];\n", List->head);
    }

    if (List->tail > 0 && List->tail <= List->capacity)
    {
        printf("TAIL Writing TO file.....\n");

        fprintf(dot_file, "    tail -> node%d:<f0> [color=red];\n", List->tail);
    }

    if (List->first_free_pos > 0 && List->first_free_pos <= (int)List->capacity)
    {
        fprintf(dot_file, "    free_ptr -> node%d [color=green, penwidth=3, style=dashed];\n", List->first_free_pos);
    }

    fprintf(dot_file, "}\n");
    fclose(dot_file);

    printf("End Writing TO file.....\n");

    system("dot -T png list_dump.dot -o images/list_dump4.png");
}

/*void Create_log_file (my_list_t* List)
{
    FILE* log_file = fopen("logFile3.txt", "w");

    #ifdef DEBUG
        assert(List && log_file);
    #endif

    fprintf(log_file, "digraph list {\n");
    fprintf(log_file, "rankdir = LR;");

    for (size_t Index = 0; Index < List->capacity - 1; Index++)
    {
        if (List->prev[Index] == -1)
        {
            fprintf(log_file,
                    "node%d [shape=Mrecord; style = filled; fillcolor = \"%s\"; color = \"%s\"; label = \"{EMPTY | { data = %.0lf | next = %d | prev = %d } }\"]\n",
                    Index, "#e49217d0", "#5b919d78", List->data[Index], List->next[Index], List->prev[Index]);
        }
        else if (List->prev[Index] != CANAREICA1 && List->prev[Index] != -1)
        {
            fprintf(log_file,
                    "node%d [shape=Mrecord; style = filled; fillcolor = \"%s\"; color = \"%s\"; label = \"{USED | { data = %.0lf | next = %d | prev = %d } }\"]\n",
                    Index, "#77e417d0", "#942323d0", List->data[Index], List->next[Index], List->prev[Index]);
        }
        else
        {
            fprintf(log_file,
                    "node%d [shape=Mrecord; style = filled; fillcolor = \"%s\"; color = \"%s\"; label = \"{CANARY | { canary_data = %.0lf | canary_next = %d | canary_prev = %d } }\"]\n",
                    Index, "#aeaeaef4", "#892394d0", List->data[0], List->next[0], List->prev[0]);
        }
    }
    for (size_t Index = 0; Index < List->capacity - 1; Index++) // для ровной горизонтальной распечатки
    {
        fprintf(log_file, "node%d->node%d[color = \"transparent\"];\n", Index, Index + 1);
    }
    for (index_t Index = 1; Index != 0; Index = List->next[Index])
    {
        if (List->next[Index] != 0)
        {
            fprintf(log_file, "node%d->node%d[label = \"next\", color = \"green\"];\n",             Index, List->next[Index]);
            fprintf(log_file, "node%d->node%d[label = \"prev\", style=dotted, color = \"red\"];\n", List->next[Index], Index);
        }
    }
    for (index_t Index = List->first_free_pos; Index != 0; Index = List->next[Index])
    {
        if (List->next[Index] != 0)
        {
            fprintf(log_file, "node%d->node%d[label = \"free\", color = \"lightgray\"];\n", Index, List->next[Index]);
        }
    }

    fprintf(log_file, "}\n"); // end of graph

    #ifdef DEBUG
        assert(List);
    #endif

    fclose(log_file);

    system("dot -Tpng list_dump.dot -o list_dump2.png");

    return;
}*/
