#include "job.h"

struct job* init_job(void (*function)(void*), void* argument){
    struct job* job = malloc(sizeof(struct job));
    if (!job){
        return NULL;
    } 

    job->fn = function;
    job->arg = argument;

    return job;
}
