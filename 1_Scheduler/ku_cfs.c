#include "linkedlist.h"

int get_nice(int *nice_level, int index) {
    int temp = 0;
    for(int i=0;i<5;i++){
        temp += nice_level[i];
        if(temp >= index + 1)
            return i - 2;
    }
}

LinkedList* list;
int time_slice = 0;
void scheduler_handler(int signum)
{
    float calc_nice[] = {0.64, 0.8, 1, 1.25, 1.5625}; // -2 -1 0 1 2

    Node* current_process = get_first(list);//list->head->next_node;
    printf("cur pid %d, %c\n", current_process -> pid, current_process -> temp);
    int nice_level = current_process -> nice_level;
    current_process -> exec_time += 1 * calc_nice[nice_level + 2]; // sec * calc[nice_value + 2]
    kill(current_process -> pid, SIGSTOP);
    while(1){
        int status, t;
        t = waitpid(current_process -> pid, &status, WUNTRACED | WNOHANG);
        if(WIFSTOPPED(status)) break;
    }
    
    if(--time_slice == 0) exit(1);

    sort_by_exec(list);
    Node* next_process = get_first(list);
    printf("nxt pid %d, %c\n", next_process -> pid, next_process -> temp);
    print_list(list);
    kill(next_process -> pid, SIGCONT);
}

int main(int argc, char **argv)
{
    if(argc != 7){
        printf("argc not 7\n");
        exit(1);
    }
    
    int nice_level[5];
    int total_count = 0;
    for(int k=0;k<5;k++){
        nice_level[k] = atoi(argv[k + 1]);
        total_count += nice_level[k];
    }
    
    time_slice = atoi(argv[6]);
    
    list = (LinkedList*)malloc(sizeof(LinkedList));
    init_list(list);
    for (int i = 0; i < total_count; i++)
    {
        int nice = get_nice(nice_level, i);
        char args[2];
        args[0] = 'A' + i;
        args[1] = '\0';

        pid_t child = fork();
        if (child > 0)
        {
            while(1){
                int status, t;
                t = waitpid(child, &status, WUNTRACED | WNOHANG);
                if(WIFSTOPPED(status)){
			break;
		}
            }
            insert_last(list, child, nice, 0, args[0]); // pid, nice, exec=0으로 node 추가(nice 낮은 것 부터)
	}
        else if (child == 0)
        {
            execl("./ku_app", "./ku_app", args, NULL); // child가 ku_app으로 대체되어 실행됨. pid:child
        }
    }
    sleep(3);
    
    struct sigaction signal_action;
    struct itimerval scheduler;
 
    memset(&signal_action, 0, sizeof(signal_action));
    signal_action.sa_handler = &scheduler_handler;
    sigaction(SIGALRM, &signal_action, NULL);
    
    scheduler.it_interval.tv_sec = 1;
    scheduler.it_interval.tv_usec = 0;
    scheduler.it_value.tv_sec = 1;
    scheduler.it_value.tv_usec = 0;
    
    kill(get_first(list) -> pid, SIGCONT);
    setitimer(ITIMER_REAL, &scheduler, NULL);
    while(1); 

    free_list(list);
}
