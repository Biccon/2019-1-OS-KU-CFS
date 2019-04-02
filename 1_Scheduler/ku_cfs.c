#include "ku_cfs.h"

LinkedList *list;
int time_slice = 0;
void scheduler_handler(int signum)
{
    double calc_nice[] = {0.64, 0.8, 1.0, 1.25, 1.5625};
 
    Node *current_process = get_first(list);
    kill(current_process->pid, SIGSTOP); // 먼저 작동중인 프로세스에 SIGSTOP을 보낸다.

    int nice_level = current_process->nice_level;
    current_process->vrumtime += 1 * calc_nice[nice_level + 2]; // 방금 멈춘 프로세스의 vrumtime을 계산하여 업데이트 한다.

    sort_by_exec(list); // vrumtime을 기준으로 list를 정렬한다.

    if (--time_slice <= 0) // time_slice가 0보다 같거나 작으면 time_slice가 끝난 것.
    {
        //print_list(list);
        free_list(list); // 종료 이전에 list를 free시켜준다
        exit(1); // 따라서 종료
    }

    Node *next_process = get_first(list); // 정렬이 끝난 후이므로 list의 맨 앞 노드가 vruntime이 가장 작은 것
    kill(next_process->pid, SIGCONT); // next_process에 SIGCONT를 보내서 시작.
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
