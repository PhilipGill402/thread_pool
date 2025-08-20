#include "job.h"

struct job* init_job(void (*function)(void*), void* argument){
    struct job* job = malloc(sizeof(struct job));
    job->fn = function;
    job->arg = argument;

    return job;
}
