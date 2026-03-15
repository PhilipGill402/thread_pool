#include "pool.h"
#include "unistd.h"

void fn(void* arg) {
    int num = *(int*)arg;
    printf("start %d\n", num);
    fflush(stdout);

    sleep(1);

    printf("end %d\n", num);
    fflush(stdout);
}

int main() {
    pool_t* pool = create_pool(4, 10);

    int a = 1, b = 2, c = 3, d = 4;

    submit_job(pool, fn, &a);
    submit_job(pool, fn, &b);
    submit_job(pool, fn, &c);
    submit_job(pool, fn, &d);

    wait_pool(pool);
}
