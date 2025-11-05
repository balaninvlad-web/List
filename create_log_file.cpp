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
        }
        else if (list->prev[i] == -1)
        {
            fillcolor = "#445c00";  // Свободный узел
            color = "#fdfdfd";
        }
        else if (i == (unsigned)Get_head(list) && i == (unsigned)Get_tail(list))
        {
            fillcolor = "#edb1f1";  // Единственный элемент
            color = "#fdfdfd";
        }
        else if (i == (unsigned)Get_head(list))
        {
            fillcolor = "#5f3035";  // Голова
            color = "#fdfdfd";
        }
        else if (i == (unsigned)Get_tail(list))
        {
            fillcolor = "#305551";  // Хвост
            color = "#fdfdfd";
        }
        else
        {
            fillcolor = "#2799a0";  // Обычный узел
            color = "#fdfdfd";
        }
        if ( (unsigned)list->next[i] > list->capacity ||
             list->next[i] < 0 ||
             list->prev[i] > (int)list->capacity ||
             list->prev[i] < -1)
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

    fprintf(dot_file, "{rank = same; head; tail; free_ptr}\n");
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

void Create_dump_files(my_list_t* list, const char* func)
{
    assert (list);
    assert (func);
    char graph_filename[50];
    char dot_filename[50];
    char command[100];
    static int dump_counter = 1;
    int result = 0;

    printf("Create_dump_files STARTED for %s\n", func);

    if (dump_counter == 1)
    {
        system("mkdir -p imagesDump");
    }

    printf("=== Make_html_file called #%d ===\n", dump_counter);


    snprintf(graph_filename, sizeof(graph_filename), "graph_%d.png", dump_counter);

    snprintf(dot_filename, sizeof(dot_filename), "graph_%d.dot", dump_counter);

    Create_html_file (list, dump_counter, graph_filename, func);

    printf("Creating DOT file: %s\n", dot_filename);

    Create_log_file(list, dot_filename);

    printf("Converting to PNG...\n");

    snprintf(command, sizeof(command), "dot -Tpng %s -o imagesDump/%s", dot_filename, graph_filename);

    result = system(command);

    if (result != 0)
    {
            printf("WARNING: Graph generation failed for dump #%d\n", dump_counter);
    }

    remove(dot_filename);

    printf("Create_dump_files COMPLETED\n");

    dump_counter++;
}

void Create_html_file (my_list_t* list, int dump_counter, const char* graph_filename, const char* func)
{
    static FILE* html_file = NULL;

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
        dump_counter++;
        return;
    }

    Create_table (html_file, list);

    fprintf(html_file, "<h3>Graph Visualization</h3>\n");

    fprintf(html_file, "<img src='imagesDump/%s' alt='Graph %d' width='1500'>\n", graph_filename, dump_counter);

    Create_text_diagram (html_file, list);

    fprintf(html_file, "</pre>\n");
    fprintf(html_file, "</div>\n");

    fflush(html_file);
    printf("HTML dump #%d completed\n", dump_counter);
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
    }
    else
    {
        int count = 0;
        while (current != 0 && current <= actual_capacity && count < 20) //????
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
