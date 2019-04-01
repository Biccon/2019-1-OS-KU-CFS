#include "ku_cfs.h"

int main(int argc, char **argv){
    pid_t pids[3];
    int tmp_sum = 0;
	int tmp_arr[] = {0,1,2,0,0};

	char start_char[2];
	
	for(int i=0;i<5;i++){
		tmp_sum += tmp_arr[i];
	}

	for(int i=0; i<tmp_sum; i++){
		start_char[0] = 'A'+i;
		start_char[1] = '\0';
		pids[i] = fork();
        if(pids[i] == 0){
            printf("%s\n", start_char);
			execl("./ku_app", "./ku_app", start_char, NULL);
		}
	}
    sleep(2);
    printf("??\n");
    kill(pids[3], SIGCONT);
    printf("//\n");
}