#include "list_on_signs_func.h"

int main ()
{
    LinkedList* List = ListCtor();

    Node* node1 = Insert_after_node(List, NULL, 10);

    Node* node2 = Insert_after_node(List, node1, 20);

    Node* node3 = Insert_after_node(List, NULL, 30);

    PrintList(List);

    DeleteNode (List, node1);

    PrintList(List);

    DeleteNode (List, GetHead(List));

    PrintList(List);

    DeleteNode (List, GetTail(List));

    PrintList(List);

    ListDtor(List);
}
