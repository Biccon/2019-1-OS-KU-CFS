#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/wait.h>

#define ITIMER_REAL 0
#define ITIMER_VIRTUAL 1
#define ITIMER_PROF 2
/*
int
getitimer(int which, struct itimerval *value);

int
setitimer(int which, const struct itimerval *restrict value,
    struct itimerval *restrict ovalue);
*/