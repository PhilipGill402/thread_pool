#include "pool.h"

struct thread_pool* init_tp(int num_threads, int queue_capacity){
    thread_pool* tp = malloc(sizeof(thread_pool*));
    tp->workers = malloc(sizeof(pthread_t*) * num_threads);
    tp->num_workers = 
    tp->queue = init_queue(queue_capacity);
    tp->m = 
    tp->not_empty = 
    tp->not_full = 
    tp->accepting = 1;
    tp->shutdown_requested = 0; 
    tp->graceful = 1;
}

int submit_job(struct thread_pool* tp, void (*fn)(void*), void* arg){
    struct job* job = init_job(fn, arg);
    
    if (!job){
        return -1;
    }

    return enqueue(&tp->queue, job);

}
