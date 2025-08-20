#pragma once
#include <stdlib.h>
#include <stdio.h>
#include "job.h"

typedef struct job_queue{
    struct job** buffer;
    int capacity;
    int head;
    int tail;
    int num_jobs;
}job_queue;

struct job_queue* init_queue(int capacity);
void destroy_queue(struct job_queue* queue);
void enqueue(struct job_queue* queue, struct job* job);
struct job* dequeue(struct job_queue* queue);
