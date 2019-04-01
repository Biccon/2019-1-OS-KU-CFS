#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct _Node Node;
typedef struct _LinkedList LinkedList;
struct _Node
{
    Node *prev_node;
    Node *next_node;

    pid_t pid;
    int nice_level;
    float exec_time;
};
struct _LinkedList
{
    Node *head;
    Node *tail;
};

void init_list(LinkedList *list)
{
    list->head = (Node *)malloc(sizeof(Node));
    list->tail = (Node *)malloc(sizeof(Node));

    list->head->prev_node = NULL;
    list->head->next_node = list->tail;
    list->tail->prev_node = list->head;
    list->tail->next_node = NULL;
}

Node *get_first(LinkedList *list)
{
    return list->head->next_node;
}

Node *get_last(LinkedList *list)
{
    return list->tail->prev_node;
}

Node *insert_next(Node *node, pid_t pid, int nice_level, float exec_time)
{
    Node *new_node = (Node *)malloc(sizeof(Node));

    new_node->prev_node = node;
    new_node->next_node = node->next_node;
    new_node->pid = pid;
    new_node->nice_level = nice_level;
    new_node->exec_time = exec_time;

    if ((node->next_node) != NULL)
        node->next_node->prev_node = new_node;
    node->next_node = new_node;

    return new_node;
}

Node *insert_first(LinkedList *list, pid_t pid, int nice_level, float exec_time)
{
    return insert_next(list->head, pid, nice_level, exec_time);
}

Node *insert_last(LinkedList *list, pid_t pid, int nice_level, float exec_time)
{
    return insert_next(list->tail->prev_node, pid, nice_level, exec_time);
}

void print_list(LinkedList *list)
{
    Node *temp = list->head->next_node;
    while (temp != (list->tail))
    {
        printf("pid : %d, nice_level : %d, exec_time : %f\n", temp->pid, temp->nice_level, temp->exec_time);
        temp = temp->next_node;
    }
}

int get_size(LinkedList *list)
{
    int size = 0;
    Node *temp = list->head->next_node;
    while (temp != (list->tail))
    {
        size++;
        temp = temp->next_node;
    }
    return size;
}

void change_node(Node *prev, Node *next)
{
    Node *temp_prev = prev->prev_node;
    Node *temp_next = prev->next_node;

    prev->prev_node = next->prev_node;
    prev->next_node = next->next_node;
    next->prev_node = temp_prev;
    next->next_node = temp_next;
}

void sort_by_exec(LinkedList *list)
{
    Node *temp = list->head->next_node;
    Node *temp2 = temp->next_node;
}

void free_list(LinkedList *list)
{
    Node *temp = list->head->next_node;
    while (temp != (list->tail))
    {
        Node *temp2 = temp;
        temp = temp->next_node;
        free(temp2);
    }
    free(list->head);
    free(list->tail);
    free(list);
}
