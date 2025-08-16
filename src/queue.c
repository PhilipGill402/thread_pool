#include "queue.h"

struct job_queue{
    struct job* buffer;
    int capacity;
    int head;
    int tail;
    int num_jobs;
};
