#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int main(int argc, char **argv)
{
    float calc_nice[] = {0.64, 0.8, 1, 1.25, 1.5625}; // -2 -1 0 1 2
    printf("first getpid : %d\n", getpid());
    for (int i = 0; i < 5; i++)
    {
        pid_t child = fork();
        if (child > 0)
        {
            printf("parent mypid : %d, my child %d\n", getpid(), child);
        }
        else if (child == 0)
        {
            printf("child %c %d\n", 'A' + i, getpid());
            break;
        }
    }

    printf("%f", calc_nice[0]);
}