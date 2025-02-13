#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>

typedef struct _Node Node;
typedef struct _LinkedList LinkedList;
struct _Node
{
    Node *prev_node;
    Node *next_node;
    pid_t pid;
    int nice_value;
    double vrumtime;
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

Node *insert_next(Node *node, pid_t pid, int nice_value, double vrumtime)
{
    Node *new_node = (Node *)malloc(sizeof(Node));

    new_node->prev_node = node;
    new_node->next_node = node->next_node;
    new_node->pid = pid;
    new_node->nice_value = nice_value;
    new_node->vrumtime = vrumtime;
    if ((node->next_node) != NULL)
        node->next_node->prev_node = new_node;
    node->next_node = new_node;

    return new_node;
}

Node *insert_last(LinkedList *list, pid_t pid, int nice_value, double vrumtime)
{
    return insert_next(list->tail->prev_node, pid, nice_value, vrumtime);
}

void change_node(Node *prev, Node *next)
{
    pid_t temp_pid = prev->pid;
    prev->pid = next->pid;
    next->pid = temp_pid;

    int temp_nice = prev->nice_value;
    prev->nice_value = next->nice_value;
    next->nice_value = temp_nice;

    double temp_vrumtime = prev->vrumtime;
    prev->vrumtime = next->vrumtime;
    next->vrumtime = temp_vrumtime;
}

void sort_by_vruntime(LinkedList *list)
{
    Node *temp = list->head->next_node;
    while (temp != (list->tail))
    {
        Node *temp2 = temp->next_node; // list->head->next_node;
        while (temp2 != (list->tail))
        {
            double time1 = temp->vrumtime;
            double time2 = temp2->vrumtime;
            pid_t pid1 = temp->pid;
            pid_t pid2 = temp2->pid;
            if (time1 > time2 || ((time1 == time2) && (pid1 > pid2)))
            {
                change_node(temp, temp2);
            }
            temp2 = temp2->next_node;
        }
        temp = temp->next_node;
    }
}

void kill_list(LinkedList* list){
    Node *temp = list->head->next_node;
    while (temp != (list->tail))
    {
        kill(temp -> pid, 9); // process kill
        temp = temp->next_node;
    }
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

int get_nice(int *nice_values, int index)
{
    int temp = 0;
    for (int i = 0; i < 5; i++)
    {
        temp += nice_values[i];
        if (temp >= index + 1)
            return i - 2;
    }
    return -2;
}