#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "job.h"
#include "queue.h"

typedef struct thread_pool{
    pthread_t* workers;
    int num_workers;
    struct job_queue queue;
    pthread_mutex_t m;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
    int accepting; //bool (1 or 0)
    int shutdown_requested; //bool (1 or 0)
    int graceful; //bool (1 or 0)

}thread_pool;

