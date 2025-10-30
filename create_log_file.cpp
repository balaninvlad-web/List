#include "create_log_file.h"

const int MAX_COMMAND_LENGTH = 200;

void Create_log_file (my_list_t* list)
{
    assert(list);

    FILE* dot_file = fopen("list_dump.dot", "w");

    Create_head_log_file (list, dot_file);

    Create_graph_node (list, dot_file);

    Make_ranking (list, dot_file);

    Make_arrow (list, dot_file);

    Make_service_signs(list, dot_file);

    Writing_log_file ();

    fclose(dot_file);
}

void Create_head_log_file (my_list_t* list, FILE* dot_file)
{
    assert(list);

    assert(dot_file);

    printf("START Writing TO file.....\n");

    fprintf(dot_file,"digraph list{\n");

    fprintf(dot_file, "    rankdir = HR\n");

    fprintf(dot_file, "    node [shape=plaintext, style=filled, fontname=\"Arial\"];\n");

    fprintf(dot_file, "    edge [fontname=\"Arial\"];\n\n");

    fprintf(dot_file, "    subgraph cluster_main {\n");
    fprintf(dot_file, "        label=\"Linearized list\";\n");
    fprintf(dot_file, "        style=filled;\n");
    fprintf(dot_file, "        fillcolor=lightgray;\n");
    fprintf(dot_file, "        margin=20;\n\n");
}

void Create_graph_node (my_list_t* list, FILE* dot_file)
{
    assert(list);

    assert(dot_file);

    for (size_t i = 0; i <= list->capacity; i++)
    {

        printf("START WRITE BLOCKS Writing TO file.....\n");

        const char* fillcolor = "";
        const char* color = "black";

        if (i == 0)
        {
            fillcolor = "#FFB6C1";  // Канарейка
            color = "darkred";
        } else if (list->prev[i] == -1)
        {
            fillcolor = "#98FB98";  // Свободный узел
            color = "darkgreen";
        } else if (i == (unsigned)Get_head(list) && i == (unsigned)Get_tail(list))
        {
            fillcolor = "#DDA0DD";  // Единственный элемент
            color = "purple";
        } else if (i == (unsigned)Get_head(list))
        {
            fillcolor = "#FFFACD";  // Голова
            color = "darkgoldenrod";
        } else if (i == (unsigned)Get_tail(list))
        {
            fillcolor = "#FFDAB9";  // Хвост
            color = "darkorange";
        } else
        {
            fillcolor = "#E6E6FA";  // Обычный узел
            color = "navy";
        }
        fprintf(dot_file, "        node%d [label=<<TABLE BORDER='0' CELLBORDER='1' CELLSPACING='0'><TR><TD>Index: %d</TD></TR><TR><TD>Data: %.2lf</TD></TR><TR><TD>Next: %d</TD></TR><TR><TD>Prev: %d</TD></TR></TABLE>>, "
                          "fillcolor=\"%s\", color=\"%s\", fontcolor=\"%s\"];\n",
               (int)i, (int)i, list->data[i], list->next[i], list->prev[i], fillcolor, color, color);

        printf("BLOCKS Writing TO file.....\n");
    }

    fprintf(dot_file,"}\n");
}

void Make_ranking (my_list_t* list, FILE* dot_file)
{
    assert(list);

    assert(dot_file);

    fprintf(dot_file,"{ rank = same; ");
    for (size_t i = 0; i <= list->capacity; i++)
    {
        fprintf(dot_file,"node%d; ", (int)i);
    }

    fprintf(dot_file,"}\n");

    fprintf(dot_file, "{rank = same; head; tail; free}\n");
}

void Make_arrow (my_list_t* list, FILE* dot_file)
{
    assert(list);

    assert(dot_file);

    for (size_t i = 0; i < list->capacity; i++)
    {
        fprintf(dot_file, "node%d -> node%d [color = \"transparent\"; weight=100];\n", (int)i, (int)(i + 1));
    }

    for (size_t i = 0; i <= list->capacity; i++)
    {
        int next = list->next[i];
        int prev = list->prev[i];

        if (next != 0 && next <= (int)list->capacity && list->prev[next] == (int)i)
        {
        //черна
            fprintf(dot_file, "    node%d -> node%d [color=\"black\", dir=both, "
                    "arrowhead=\"normal\", arrowsize=1.1, penwidth=2, weight=0];\n",
                    (int)i, next);
        }
        else if (next != 0 && next <= (int)list->capacity)
        {
        //синя
            fprintf(dot_file, "    node%d -> node%d [color=\"blue\", "
                    "arrowhead=\"normal\", arrowsize=2.2, penwidth=2, weight=0];\n",
                    (int)i, next);
        }
        if (prev > 0 && prev <= (int)list->capacity && list->next[prev] != (int)i)
        {
        // красна
            fprintf(dot_file, "    node%d -> node%d [color=\"red\", style=dashed, "
                    "arrowhead=\"normal\", arrowsize=1.2, penwidth=2, constraint=false];\n",
                    (int)i, prev);
        }
    }

    int free_pos = Get_first_free_pos(list);

    while (free_pos != 0 && free_pos <= (int)list->capacity)
    {
        int next_free = list->next[free_pos];

        if (next_free != 0 && next_free <= (int)list->capacity)
        {
            printf("FREE Writing TO file.....\n");

            fprintf(dot_file, "    node%d -> node%d [color=\"#32CD32\", style=dashed];\n",
                   free_pos, next_free);
        }
        free_pos = next_free;
    }
}

void Make_service_signs(my_list_t* list, FILE* dot_file)
{
    assert(list);

    assert(dot_file);

    fprintf(dot_file, "    head [shape=Mrecord, color=red, fillcolor=red, style=filled, label=\"HEAD\"];\n");
    fprintf(dot_file, "    tail [shape=Mrecord, color=blue, fillcolor=blue, style=filled, label=\"TAIL\"];\n");
    fprintf(dot_file, "    free_ptr [shape=Mrecord, color=green, fillcolor=green, style=filled, label=\"FREE\"];\n");


    if ((unsigned)Get_head(list) <= list->capacity)
    {
        printf("HEAD Writing TO file.....\n");

        fprintf(dot_file, "    head -> node%d [color=red, weight=100];\n", Get_head(list));
    }

    if ((unsigned)Get_tail(list) <= list->capacity)
    {
        printf("TAIL Writing TO file.....\n");

        fprintf(dot_file, "    tail -> node%d [color=red, weight=100];\n", Get_tail(list));
    }

    if (Get_first_free_pos(list) <= list->capacity)
    {
        fprintf(dot_file, "    free_ptr -> node%d [color=green, penwidth=3, style=dashed, weight=100];\n", Get_first_free_pos(list));
    }

    fprintf(dot_file, "}\n");
    fclose(dot_file);

    printf("End Writing TO file.....\n");
}

void Writing_log_file (void)
{
    static int image_counter = 1;

    char command[MAX_COMMAND_LENGTH] = "";

    snprintf(command, sizeof(command), "dot -Tpng list_dump.dot -o imagesDump/list_dump%d.png", image_counter);

    system(command);

    printf("dot -Tpng list_dump.dot -o imagesDump/list_dump%d.png", image_counter);

    printf("Graph generated: list_dump%d.png\n", image_counter);

    image_counter++;
}


