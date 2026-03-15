#include "pool.h"

void* worker(void* arg) {
    pool_t* pool = arg;

    while (1) {
        // 1. lock mutex
        // 2. wait for a job if queue is empty
        // 3. if shutting down, exit
        // 4. pop a job from queue
        // 5. unlock mutex
        // 6. run the job function
        // 7. loop again
    }

    return NULL;
}

pool_t create_pool(int num_threads, int queue_capacity) {
    pool_t pool = {
        .workers = malloc(sizeof(pthread_t)*num_threads); 
        .num_workers = num_threads,
        .queue = create_queue(sizeof(job_t), NULL),
        .mutex = PTHREAD_MUTEX_INITIALIZER,
        .not_empty = PTHREAD_COND_INITIALIZER,
        .not_full = PTHREAD_COND_INITIALIZER,
        .accepting = 1,
        .shutdown_requested = 0,
        .graceful = 1
    };
    
    for (int i = 0; i < num_threads; i++) {
        pthread_create(&pool.workers[i], NULL, worker, &pool);
    }

    resize_queue(&queue, queue_capacity);  

    return pool;
}

int submit_job(pool_t* tp, void (*fn)(void*), void* arg) {
    job_t job = create_job(fn, arg);
    enqueue(&tp->queue, &job);

    return 1;
}

void shutdown_pool(pool_t* tp, int graceful) {
    tp->shutdown_requested = 1;
    tp->graceful = graceful;
}

void destroy_pool(pool_t* tp) {
    if (tp->shutdown_requested && tp->graceful) {
        pthread_cont_wait(&tp->not_empty, &tp->mutex);
    } 

    free(tp->workers);
    tp->workers = NULL;
    queue_release(&tp->queue);
    tp->num_workers = 0;
    pthread_mutex_destroy(&tp->mutex);
    pthread_cond_destroy(&tp->not_empty);
    pthread_cond_destroy(&tp->not_full);
    tp->accepting = 0;
    tp->shutdown_requested = 0;
    tp->graceful = 0;
}
