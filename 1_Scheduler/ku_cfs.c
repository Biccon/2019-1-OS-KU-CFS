#include "linkedlist.h"
int get_nice(int *nice_level, int index) {
    int temp = 0;
    for(int i=0;i<5;i++){
        temp += nice_level[i];
        if(temp > index) 
            return i;
    }

}

int main(int argc, char **argv)
{
    if(argc != 7){
        printf("argc not 7\n");
        exit(1);
    }
    
    float calc_nice[] = {0.64, 0.8, 1, 1.25, 1.5625}; // -2 -1 0 1 2
    int nice_level[5];
    int total_count = 0;
    for(int k=0;k<5;k++){
        int count = atoi(argv[k + 1]);
        nice_level[k] = count;
        total_count += count;
    }
    
    int time_slice = atoi(argv[6]);
    
    LinkedList* list = (LinkedList*)malloc(sizeof(LinkedList));
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
                if(WIFSTOPPED(status)) break;
            }
            insert_last(list, child, nice, 0); // pid, nice, exec=0으로 node 추가(nice 낮은 것 부터)
        }
        else if (child == 0)
        {
            execl("./ku_app", "./ku_app", args, NULL); // child가 ku_app으로 대체되어 실행됨. pid:child
        }
    }
    print_list(list);
    // 스케줄링 시작
    
    kill(get_first(list) -> pid, SIGCONT);

    // 스케줄링 끝
    free_list(list);
    return 0;
}