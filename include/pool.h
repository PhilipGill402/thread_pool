#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "job.h"
#include "queue.h"

typedef struct {
    pthread_t* workers;
    int num_workers;
    int active_workers;
    queue_t queue;
    pthread_mutex_t mutex;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
    pthread_cond_t finished;
    int accepting; //bool (1 or 0)
    int shutdown_requested; //bool (1 or 0)
    int graceful; //bool (1 or 0)

} pool_t;

pool_t* create_pool(int num_threads, int queue_capacity);
int submit_job(pool_t* tp, void (*fn)(void*), void* arg);
void shutdown_pool(pool_t* tp, int graceful);
void destroy_pool(pool_t* tp);
void wait_pool(pool_t* pool);
