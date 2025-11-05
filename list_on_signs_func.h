#ifndef LIST_ON_SIGNS_FUNC_H
#define LIST_ON_SIGNS_FUNC_H
#include <TXLib.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

typedef int ListErr_t;

typedef struct Node
{
    int data;
    struct Node* next;
    struct Node* prev;
} Node;

typedef struct
{
    Node* dummy;
    size_t size;
} LinkedList;

LinkedList* ListCtor();
void ListDtor (LinkedList* list);

Node* Insert_after_node(LinkedList* list, Node* after_node, int value);
void PrintList (LinkedList* list);
int DeleteNode (LinkedList* list, Node* node);

Node* GetHead(LinkedList* list);
Node* GetTail(LinkedList* list);
void VerifyList(LinkedList* list);

void Create_log_file(LinkedList* list, const char* filename);
void Create_head_log_file(LinkedList* list, FILE* dot_file);
void Create_graph_node(LinkedList* list, FILE* dot_file);
void Make_ranking(LinkedList* list, FILE* dot_file);
void Make_arrow(LinkedList* list, FILE* dot_file);
void Make_service_signs(LinkedList* list, FILE* dot_file);
void Create_picture(void);

#endif
