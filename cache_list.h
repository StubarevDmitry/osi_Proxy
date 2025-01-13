#ifndef LAB3_PROXY_CACHE_LIST_H
#define LAB3_PROXY_CACHE_LIST_H

#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define CACHE_BUFFER_SIZE (1024 * 1024 * 64 * 4) // 64 MB

typedef struct cache {
    unsigned long request;
    char *response;
    ssize_t response_len;
    ssize_t buffer_size;  // Текущий размер буфера
    struct cache *next;
} Cache;

extern pthread_mutex_t cache_mutex;  // Объявление переменной

unsigned long hash(const char *str);
int init_cache_record(Cache *record);
ssize_t find_in_cache(Cache *start, char *req, char *copy);
void add_request(Cache *record, char *req);
void add_response(Cache *record, char *resp, unsigned long cur_position, unsigned long resp_size);
void add_size(Cache *record, ssize_t size);
void delete_cache_record(Cache *record);
void push_record(Cache *start, Cache *record);

#endif //LAB3_PROXY_CACHE_LIST_H
