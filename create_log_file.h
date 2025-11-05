#ifndef CREATE_LOG_FILE_H
#define CREATE_LOG_FILE_H
#include <TXLib.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "list_functions.h"

extern const int MAX_COMMAND_LENGTH;
void Create_dump_files(my_list_t* list, const char* func);

void Create_html_file (my_list_t* list, int dump_counter, const char* graph_filename, const char* func);

void Create_head_html (FILE** html_file);
void Create_table (FILE* html_file, my_list_t* list);
void Create_text_diagram (FILE* html_file, my_list_t* list);

void Create_log_file (my_list_t* list, const char* filename);

void Create_head_log_file (my_list_t* list, FILE* dot_file);
void Create_graph_node (my_list_t* list, FILE* dot_file);
void Make_ranking (my_list_t* list, FILE* dot_file);
void Make_arrow (my_list_t* list, FILE* dot_file);
void Make_service_signs(my_list_t* list, FILE* dot_file);
void Writing_log_file (void);

#endif
