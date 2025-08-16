#pragma once
#include <stdlib.h>
#include "job.h"

typedef struct job_queue{
    struct job* buffer;
    int capacity;
    int head;
    int tail;
    int num_jobs;
}job_queue; 
