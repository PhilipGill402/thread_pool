#pragma once
#include <stdlib.h>

typedef struct {
    void (*fn)(void*);
    void* arg;
}job_t;

job_t init_job(void (*function)(void*), void* argument);
