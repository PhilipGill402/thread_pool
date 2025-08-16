#include "job.h"

struct job {
    void (*fn)(void*);
    void *arg;
};
