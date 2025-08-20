#pragma once
#include <stdlib.h>

typedef struct job{
    void (*fn)(void*);
    void* arg;
}job;

struct job* init_job(void (*function)(void*), void* argument);
