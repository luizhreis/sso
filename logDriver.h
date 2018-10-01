#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "logDriver.c"

int createLogFile(char*);
int logProcessStarted(int, int, int);
int logProcessCreation(int, int, int, int);
int logProcessTerminated(int, int);
int logProcessPreempted(int, int);
