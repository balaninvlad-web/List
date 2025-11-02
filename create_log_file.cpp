#include "create_log_file.h"

const int MAX_COMMAND_LENGTH = 200;

void Create_log_file (my_list_t* list, const char* filename)
{
    assert(list);

    FILE* dot_file = fopen(filename, "w");

    Create_head_log_file (list, dot_file);

    Create_graph_node (list, dot_file);

    Make_ranking (list, dot_file);

    Make_arrow (list, dot_file);

    Make_service_signs(list, dot_file);

    //Writing_log_file ();

    fclose(dot_file);
}

void Create_head_log_file (my_list_t* list, FILE* dot_file)
{
    assert(list);

    assert(dot_file);

    printf("START Writing TO file.....\n");

    fprintf(dot_file,"digraph list{\n");

    fprintf(dot_file, "    bgcolor=\"#001f29\"\n");

    fprintf(dot_file, "    rankdir = HR\n");
    fprintf(dot_file, "    nodesep = 0.5;\n");
    fprintf(dot_file, "    ranksep = 0.7;\n");

    fprintf(dot_file, "    node [shape=plaintext, style=filled, fontname=\"Arial\"];\n");

    fprintf(dot_file, "    edge [fontname=\"Arial\"];\n\n");

    //fprintf(dot_file, "    subgraph cluster_main {\n");
    //fprintf(dot_file, "        label=\"Linearized list\";\n");
    //fprintf(dot_file, "        style=filled;\n");
    //fprintf(dot_file, "        fillcolor=lightgray;\n");
    //fprintf(dot_file, "        margin=20;\n\n");

    fprintf(dot_file, "    {\n");
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
            fillcolor = "#691236";  // фиктивная ячейка
            color = "#fdfdfd";
        } else if (list->prev[i] == -1)
        {
            fillcolor = "#445c00";  // Свободный узел
            color = "#fdfdfd";
        } else if (i == (unsigned)Get_head(list) && i == (unsigned)Get_tail(list))
        {
            fillcolor = "#edb1f1";  // Единственный элемент
            color = "#fdfdfd";
        } else if (i == (unsigned)Get_head(list))
        {
            fillcolor = "#5f3035";  // Голова
            color = "#fdfdfd";
        } else if (i == (unsigned)Get_tail(list))
        {
            fillcolor = "#305551";  // Хвост
            color = "#fdfdfd";
        } else
        {
            fillcolor = "#2799a0";  // Обычный узел
            color = "#fdfdfd";
        }
        if ( (unsigned)list->next[i] > list->capacity || list->next[i] < 0 || list->prev[i] > (int)list->capacity || list->prev[i] < -1)
        {
            fillcolor = "red";

            fprintf(dot_file, "     node%d [shape=tripleoctagon, label = \"ERORR NEXT %d\", fillcolor=red, color= white, fontcolor= white];\n", (int)list->next[i], (int)list->next[i]);
        }
        fprintf(dot_file, "        node%d [label=<<TABLE BORDER='1' CELLBORDER='1' CELLSPACING='0'><TR><TD COLSPAN = '2'>Index: %d</TD></TR><TR><TD COLSPAN = '2'>Data: %.2lf</TD></TR><TR><TD>Next: %d</TD><TD>Prev: %d</TD></TR></TABLE>>, "
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

    for (size_t i = 1; i <= list->capacity; i++)
    {
        int next = list->next[i];
        int prev = list->prev[i];
        // ошибки
        if ((unsigned)next > list->capacity || next < 0 || prev > (int)list->capacity || prev < -1)
        {
            fprintf(dot_file, "    node%d -> node%d [color = red, arrowsize=3, penwidth=1, weight=0];\n", (int)i, next);
        }

        if (next != 0 && next <= (int)list->capacity && list->prev[next] == (int)i)
        {
        // двустороня
            fprintf(dot_file, "    node%d -> node%d [color=\"#508191\", dir=both, "
                    "arrowhead=\"normal\", arrowsize=0.9, penwidth=1, weight=0];\n",
                    (int)i, next);
        }
        else if (next != 0 && next <= (int)list->capacity)
        {
        //синя или зелена
            if (list->prev[next] == -1)
            {
                fprintf(dot_file, "    node%d -> node%d [color=\"#465c00\", "
                    "arrowhead=\"normal\", label=\"next\", fontcolor=\"#465c00\", fontsize=13, arrowsize=0.8, penwidth=1, weight=0];\n",
                    (int)i, next);
            }
            else
            {
            fprintf(dot_file, "    node%d -> node%d [color=\"#adebff\", "
                    "arrowhead=\"normal\", label=\"next\", fontcolor=\"#adebff\", fontsize=13, arrowsize=0.8, penwidth=1, weight=0];\n",
                    (int)i, next);
            }
        }
        if (prev > 0 && prev <= (int)list->capacity && list->next[prev] != (int)i)
        {
        // розова обратна пунктирна
            fprintf(dot_file, "    node%d -> node%d [color=\"#ffadb1\", style=dashed, "
                    "arrowhead=\"normal\",label=\"prev\", fontcolor=\"#ffadb1\", fontsize=13, arrowsize=0.7, penwidth=2, constraint=false];\n",
                    (int)i, prev);
        }
    }
    //int free_pos = Get_first_free_pos(list);
}

void Make_service_signs(my_list_t* list, FILE* dot_file)
{
    assert(list);

    assert(dot_file);

    fprintf(dot_file, "    head [shape=Mrecord, color=\"#fdfdfd\", fillcolor=\"#5f3035\", fontcolor=white, style=filled, label=\"HEAD\"];\n");
    fprintf(dot_file, "    tail [shape=Mrecord, color=\"#fdfdfd\", fillcolor=\"#305551\", fontcolor=white, style=filled, label=\"TAIL\"];\n");
    fprintf(dot_file, "    free_ptr [shape=Mrecord, color=\"#fdfdfd\", fillcolor=\"#465c00\", fontcolor=white, style=filled, label=\"FREE\"];\n");


    if ((unsigned)Get_head(list) <= list->capacity)
    {
        printf("HEAD Writing TO file.....\n");

        fprintf(dot_file, "    head -> node%d [color=\"#5f3035\", weight=100];\n", Get_head(list));
    }

    if ((unsigned)Get_tail(list) <= list->capacity)
    {
        printf("TAIL Writing TO file.....\n");

        fprintf(dot_file, "    tail -> node%d [color=\"#305551\", weight=100];\n", Get_tail(list));
    }

    if (Get_first_free_pos(list) <= list->capacity)
    {
        fprintf(dot_file, "    free_ptr -> node%d [color=\"#465c00\", penwidth=3, style=dashed, weight=100];\n", Get_first_free_pos(list));
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


