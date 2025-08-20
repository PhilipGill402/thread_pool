#include "queue.h"

struct job_queue* init_queue(int capacity){
    struct job_queue* queue = malloc(sizeof(struct job_queue));

    queue->buffer = malloc(sizeof(struct job*) * capacity);
    queue->capacity = capacity;
    queue->head = 0;
    queue->tail = 0;
    queue->num_jobs = 0;

    return queue;
} 

void destroy_queue(struct job_queue* queue){
    free(queue->buffer);
    free(queue);
}

void enqueue(struct job_queue* queue, struct job* job){
    if (queue->num_jobs == queue->capacity){
        return; 
    }

    queue->buffer[queue->tail] = job;
    queue->tail = (queue->tail + 1) % queue->capacity;
    queue->num_jobs++;
    return; 
}

struct job* dequeue(struct job_queue* queue){
    if (queue->num_jobs == 0){
        return NULL;
    }

    struct job* job = queue->buffer[queue->head];
    queue->head = (queue->head + 1) % queue->capacity;
    queue->num_jobs--;

    return job;
}
