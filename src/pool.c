#include "pool.h"

int submit_job(struct thread_pool* tp, void (*fn)(void*), void* arg){
    struct job* job = init_job(fn, arg);
    enqueue(&tp->queue, job);

    return 0;
}
