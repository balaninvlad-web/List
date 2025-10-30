#ifndef CREATE_LOG_FILE_H
#define CREATE_LOG_FILE_H
#include <TXLib.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "list_functions.h"

extern const int MAX_COMMAND_LENGTH;

void Create_log_file (my_list_t* list);

void Create_head_log_file (my_list_t* list, FILE* dot_file);
void Create_graph_node (my_list_t* list, FILE* dot_file);
void Make_ranking (my_list_t* list, FILE* dot_file);
void Make_arrow (my_list_t* list, FILE* dot_file);
void Make_service_signs(my_list_t* list, FILE* dot_file);
void Writing_log_file (void);

#endif
