#ifndef PROXY_H
#define PROXY_H

#include "enum_const.h"
#include "cache_list.h"
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

// Определение структуры context
typedef struct {
    int client_socket;
    char *request;
} context;

// Объявление переменных
extern int server_is_on;
extern Cache *cache;
extern sem_t thread_semaphore;

// Объявление функций
int create_server_socket();
int connect_to_remote(char *host);
int read_request(int client_socket, char *request);
int init_cache();
void destroy_cache();
int send_from_cache(char *request, int client_socket);
void* execute_client_request(void *arg);
void accept_new_client(int server_socket);
void run_proxy();

#endif // PROXY_H
