#include <stdio.h>
#include <sys/time.h>
#include <signal.h>

#define ITIMER_REAL 0
#define ITIMER_VIRTUAL 1
#define ITIMER_PROF 2

void timer_handler(int signum)
{
    static int count = 0;
    printf("expired %d\n", ++count);
}

int main(int argc){
    struct sigaction sa;
    struct itimerval timer;
 
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &timer_handler;
    sigaction(SIGVTALRM, &sa, NULL);
 
    timer.it_interval.tv_sec = 1;
    timer.it_interval.tv_usec = 0;
    timer.it_value.tv_sec = 1;
    timer.it_value.tv_usec = 0;

    setitimer(ITIMER_VIRTUAL, &timer, NULL);
    
    while(1);
}