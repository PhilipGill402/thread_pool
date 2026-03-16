#include "pool.h"

void* worker(void* arg) {
    thread_pool_t* pool = arg;

    while (1) {
        pthread_mutex_lock(&pool->mutex);

        while (is_empty(&pool->queue) && !pool->shutdown_requested) {
            pthread_cond_wait(&pool->not_empty, &pool->mutex);
        } 
    
        if (pool->shutdown_requested) {
            if (!pool->graceful) {
                pthread_mutex_unlock(&pool->mutex);
                return NULL;
            } else if (is_empty(&pool->queue)) {
                pthread_mutex_unlock(&pool->mutex);
                return NULL;
            }
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

thread_pool_t* create_pool(int num_threads, int queue_capacity) {
    thread_pool_t* pool = malloc(sizeof(thread_pool_t));
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

int submit_job(thread_pool_t* tp, void (*fn)(void*), void* arg) {
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

void shutdown_pool(thread_pool_t* tp, int graceful) {
    pthread_mutex_lock(&tp->mutex);
    
    tp->shutdown_requested = 1;
    tp->graceful = graceful;
    
    pthread_mutex_unlock(&tp->mutex);
}

void destroy_pool(thread_pool_t* pool) {
    pthread_mutex_lock(&pool->mutex);
    pool->shutdown_requested = 1;

    pthread_cond_broadcast(&pool->not_empty);
    pthread_cond_broadcast(&pool->not_full);
    pthread_cond_broadcast(&pool->finished);
    pthread_mutex_unlock(&pool->mutex);

    for (int i = 0; i < pool->num_workers; i++) {
        pthread_join(pool->workers[i], NULL);
    }

    pthread_mutex_destroy(&pool->mutex);
    pthread_cond_destroy(&pool->not_empty);
    pthread_cond_destroy(&pool->not_full);
    pthread_cond_destroy(&pool->finished);

    free(pool->workers);
    pool->workers = NULL;
    queue_release(&pool->queue);
    free(pool);
    pool = NULL;
}

void wait_pool(thread_pool_t* pool) {
    pthread_mutex_lock(&pool->mutex);
    
    while (!is_empty(&pool->queue) || pool->active_workers > 0) {
        pthread_cond_wait(&pool->finished, &pool->mutex);
    }

    pthread_mutex_unlock(&pool->mutex);
}



