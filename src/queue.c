#include "queue.h"

struct job_queue{
    struct job* buffer;
    int capacity;
    int head;
    int tail;
    int num_jobs;
};

struct job_queue* init_queue(job_queue* queue){
    queue->buffer = malloc(sizeof(struct job) * 10);
    queue->capacity = 10;
    queue->head = 0;
    queue->tail = 0;
    queue->num_jobs = 0;
}



void append_queue(struct job_queue* queue, struct job* job){
    if (num_jobs > capacity){
        queue->buffer = realloc(sizeof(struct job) * capacity * 2);
    }


}

int main(){
    struct job_queue q;
    struct job_queue* queue = init_queue(&q);
}



