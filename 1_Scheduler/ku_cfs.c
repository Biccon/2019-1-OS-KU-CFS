#include "ku_cfs.h"

LinkedList *list;
int time_slice = 0;

void scheduler_handler(int signum)
{
    double calc_nice[] = {0.64, 0.8, 1.0, 1.25, 1.5625};
 
    Node *current_process = get_first(list);
    int nice_level = current_process->nice_level;
    current_process->exec_time += 1 * calc_nice[nice_level + 2];
    kill(current_process->pid, SIGSTOP);
    sort_by_exec(list);

    if (--time_slice == 0)
    {
        print_list(list);
        free_list(list);
        exit(1);
    }

    Node *next_process = get_first(list);
    kill(next_process->pid, SIGCONT);
}

void set_timer(){
    struct sigaction signal_action;
    struct itimerval scheduler;

    memset(&signal_action, 0, sizeof(signal_action));
    signal_action.sa_handler = &scheduler_handler;
    sigaction(SIGALRM, &signal_action, NULL);

    scheduler.it_interval.tv_sec = 1;
    scheduler.it_interval.tv_usec = 0;
    scheduler.it_value.tv_sec = 1;
    scheduler.it_value.tv_usec = 0;

    kill(get_first(list)->pid, SIGCONT);
    setitimer(ITIMER_REAL, &scheduler, NULL);
    while (1);
}

void make_children(int total_count, int* nice_level){
    for (int i = 0; i < total_count; i++)
    {
        char args[2];
        args[0] = 'A' + i;
        args[1] = '\0';

        pid_t child = fork();
        if (child > 0)
        {
            while (1)
            {
                int status;
                waitpid(child, &status, WUNTRACED | WNOHANG);
                if (WIFSTOPPED(status))
                {
                    insert_last(list, child, get_nice(nice_level, i), 0, args[0]);
                    break;
                }
            }
        }
        else if (child == 0)
        {
            execl("./ku_app", "./ku_app", args, NULL);
        }
    }
}
int main(int argc, char **argv)
{
    int nice_level[5];
    int total_count = 0;
    for (int k = 0; k < 5; k++)
    {
        nice_level[k] = atoi(argv[k + 1]);
        total_count += nice_level[k];
    }

    time_slice = atoi(argv[6]);

    list = (LinkedList *)malloc(sizeof(LinkedList));
    init_list(list);
    
    make_children(total_count, nice_level);

    set_timer();
}
