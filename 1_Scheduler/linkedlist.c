#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
typedef struct _Node Node;
typedef struct _LinkedList LinkedList;
struct _Node {
    Node *prev_node;
    Node *next_node;

    pid_t pid;
    int nice_level;
    float exec_time;
};

struct _LinkedList {
    Node *head;
    Node *tail;
};

void init_list(LinkedList* list) {
    list -> head = (Node*)malloc(sizeof(Node));
    list -> tail = (Node*)malloc(sizeof(Node));

    list -> head -> prev_node = NULL;
    list -> head -> next_node = list -> tail;
    list -> tail -> prev_node = list -> head;
    list -> tail -> next_node = NULL;
}

Node* insert_next(Node* node, pid_t pid, int nice_level, float exec_time) {
    Node* new_node = (Node*)malloc(sizeof(Node));

    // 새로운 노드의 Link설정
    new_node -> prev_node = node;
    new_node -> next_node = node -> next_node;
    new_node -> pid = pid;
    new_node -> nice_level = nice_level;
    new_node -> exec_time = exec_time;
    
    // 기존 노드들 (새로운 노드의 왼쪽 오른쪽이 될) 링크 재설정
    if((node -> next_node) != NULL) node -> next_node -> prev_node = new_node;
    node -> next_node = new_node;

    return new_node;
}

Node* insert_first(LinkedList* list, pid_t pid, int nice_level, float exec_time) {
    insert_next(list -> head, pid, nice_level, exec_time);
}

Node* insert_last(LinkedList* list, pid_t pid, int nice_level, float exec_time) {
    insert_next(list -> tail -> prev_node, pid, nice_level, exec_time);
}

void print_node(LinkedList* list) {
    Node* temp = list -> head -> next_node;
    while(temp != (list -> tail)){
        printf("pid : %d, nice_level : %d, exec_time : %f\n", temp -> pid, temp -> nice_level, temp -> exec_time);
        temp = temp -> next_node;
    }
}

Node* get_first(LinkedList* list) {
    return list -> head -> next_node;
}
Node* get_last(LinkedList* list){
    return list -> tail -> prev_node;
}

int get_size(LinkedList* list) {
    int size = 0;
    Node* temp = list -> head -> next_node;
    while(temp != (list -> tail)){
        size++;
        temp = temp -> next_node;
    }
    return size;
}

void change_node(Node* prev, Node* next) {
    
}
void sort_exec(LinkedList *list){

}

void free_list(LinkedList *list) {
    Node* temp = list -> head -> next_node;
    while(temp != (list -> tail)) {
        free(temp);
        temp = temp -> next_node;
    }
    free(list -> head);
    free(list -> tail);
    free(list);
} 
void main(){
    LinkedList* list = (LinkedList*)malloc(sizeof(LinkedList));
    init_list(list);

    insert_next(list -> head, getpid(), 2, 1.133);
    insert_first(list, 123,-1, 1.2);
    insert_last(list, 1324124123,123, 1.14123);
    insert_first(list, 123,-21, 1.242);
    insert_first(list, 123,-13, 1.2345);
    insert_first(list, 123,-14, 3.2123);
    
    print_node(list);
    free_list(list);
    printf("프리완료\n");
}