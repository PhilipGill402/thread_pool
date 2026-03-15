#include "job.h"

job_t init_job(void (*function)(void*), void* argument) {
    job_t job = {
        .fn = function,
        .arg = argument
    };

    return job;
}
