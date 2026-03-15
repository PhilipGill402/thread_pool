#include "pool.h"
#include "unistd.h"

void fn(void* arg) {
    int num = *(int*)arg;
    printf("start %d\n", num);
    fflush(stdout);

    sleep(2);

    printf("end %d\n", num);
    fflush(stdout);
}

int main() {
    pool_t* pool = create_pool(2, 20);

    int jobs[10];
    for (int i = 0; i < 10; i++) {
        jobs[i] = i;
        submit_job(pool, fn, &jobs[i]);
    }

    usleep(100000); // 0.1 sec
    shutdown_pool(pool, 0); // non-graceful

    destroy_pool(pool);
    return 0;
}
