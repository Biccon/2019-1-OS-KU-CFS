#include "linkedlist.h"

void main()
{
    LinkedList *list = (LinkedList *)malloc(sizeof(LinkedList));
    init_list(list);

    insert_next(list->head, getpid(), 2, 1.133);
    insert_first(list, 123, -1, 1.2);
    insert_last(list, 1324124123, 123, 1.14123);
    insert_first(list, 123, -21, 1.242);
    insert_first(list, 123, -13, 1.2345);
    insert_first(list, 123, -14, 3.2123);

    print_node(list);
    free_list(list);
}