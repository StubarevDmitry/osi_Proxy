#include "proxy.h"

int main() {
    printf("SERVER START\n");

    sem_init(&thread_semaphore, 0, MAX_USERS_COUNT);
    run_proxy();
    sem_destroy(&thread_semaphore);
    exit(EXIT_SUCCESS);
}
