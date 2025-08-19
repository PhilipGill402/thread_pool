#pragma once
#include <stdlib.h>

typedef struct job{
    void (*fn)(void*);
    void *arg;
}job;
