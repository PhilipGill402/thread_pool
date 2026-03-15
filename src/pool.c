#include "pool.h"

void* worker(void* arg) {
    pool_t* pool = arg;

    while (1) {
        pthread_mutex_lock(&pool->mutex);

        while (is_empty(&pool->queue) && !pool->shutdown_requested) {
            pthread_cond_wait(&pool->not_empty, &pool->mutex);
        } 

        if (pool->shutdown_requested && !is_empty(&pool->queue)) {
            pthread_mutex_unlock(&pool->mutex); 
            return NULL;
        }

        job_t job = *(job_t*)dequeue(&pool->queue);
        pool->active_workers++;
        
        pthread_cond_signal(&pool->not_full);
        
        pthread_mutex_unlock(&pool->mutex);

        job.fn(job.arg);

        pthread_mutex_lock(&pool->mutex);
        pool->active_workers--;

        if (is_empty(&pool->queue) && pool->active_workers == 0) {
            pthread_cond_signal(&pool->finished);
        }

        pthread_mutex_unlock(&pool->mutex);
    }

    return NULL;
}

pool_t* create_pool(int num_threads, int queue_capacity) {
    pool_t* pool = malloc(sizeof(pool_t));
    pool->workers = malloc(sizeof(pthread_t)*num_threads);
    pool->num_workers = num_threads;
    pool->queue = create_queue(sizeof(job_t), NULL);
    pool->accepting = 1;
    pool->shutdown_requested = 0;
    pool->graceful = 1;
    pool->active_workers = 0;

    pthread_mutex_init(&pool->mutex, NULL);
    pthread_cond_init(&pool->not_empty, NULL);
    pthread_cond_init(&pool->not_full, NULL);
    pthread_cond_init(&pool->finished, NULL);
    
    for (int i = 0; i < num_threads; i++) {
        pthread_create(&pool->workers[i], NULL, worker, pool);
    }

    resize_queue(&pool->queue, queue_capacity);

    return pool;
}

int submit_job(pool_t* tp, void (*fn)(void*), void* arg) {
    job_t job = init_job(fn, arg);
    
    pthread_mutex_lock(&tp->mutex);
    
    while (queue_size(&tp->queue) == tp->queue.capacity && !tp->shutdown_requested) {
        pthread_cond_wait(&tp->not_full, &tp->mutex);
    }

    if (tp->shutdown_requested) {
        pthread_mutex_unlock(&tp->mutex);
        return 0;
    }

    enqueue(&tp->queue, &job);
    
    
    pthread_cond_signal(&tp->not_empty);
    pthread_mutex_unlock(&tp->mutex);

    return 1;
}

void shutdown_pool(pool_t* tp, int graceful) {
    tp->shutdown_requested = 1;
    tp->graceful = graceful;
}

void destroy_pool(pool_t* tp) {
    if (tp->shutdown_requested && tp->graceful) {
        pthread_cond_wait(&tp->not_empty, &tp->mutex);
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
    free(tp);
    tp = NULL;
}

void wait_pool(pool_t* pool) {
    pthread_mutex_lock(&pool->mutex);
    
    while (!is_empty(&pool->queue) || pool->active_workers > 0) {
        pthread_cond_wait(&pool->finished, &pool->mutex);
    }

    pthread_mutex_unlock(&pool->mutex);
}



