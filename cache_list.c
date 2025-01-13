#include "cache_list.h"

pthread_mutex_t cache_mutex = PTHREAD_MUTEX_INITIALIZER;

unsigned long hash(const char *str) {
    unsigned long hash = 0;
    while (*str != '\0') {
        hash = ((hash << 5) + hash) + (unsigned long) (*str);
        ++str;
    }
    return hash;
}

int init_cache_record(Cache *record) {
    record->request = 0;
    record->response_len = 0;
    record->buffer_size = 1024;  // Начальный размер буфера
    record->response = malloc(record->buffer_size);
    if (record->response == NULL) {
        return EXIT_FAILURE;
    }
    record->next = NULL;
    return EXIT_SUCCESS;
}

ssize_t find_in_cache(Cache *start, char *req, char *copy) {
    Cache *cur = start;
    pthread_mutex_lock(&cache_mutex);
    unsigned long req_hash = hash(req);
    while (cur != NULL) {
        if (cur->request == req_hash) {
            strncpy(copy, cur->response, cur->response_len);
            pthread_mutex_unlock(&cache_mutex);
            return cur->response_len;
        }
        cur = cur->next;
    }
    pthread_mutex_unlock(&cache_mutex);
    return -1;
}

void add_request(Cache *record, char *req) {
    unsigned long _hash = hash(req);
    printf("Current hash = %lu\n", _hash);
    record->request = _hash;
}

void add_response(Cache *record, char *resp, unsigned long cur_position, unsigned long resp_size) {
    // Проверяем, достаточно ли места в буфере
    if (cur_position + resp_size > record->buffer_size) {
        // Увеличиваем размер буфера
        ssize_t new_size = record->buffer_size * 2;
        while (cur_position + resp_size > new_size) {
            new_size *= 2;
        }
        char *new_buffer = realloc(record->response, new_size);
        if (new_buffer == NULL) {
            perror("Failed to realloc buffer");
            return;
        }
        record->response = new_buffer;
        record->buffer_size = new_size;
    }

    // Копируем данные в буфер
    memcpy(record->response + cur_position, resp, resp_size);
}

void add_size(Cache *record, ssize_t size) {
    record->response_len = size;
}

void delete_cache_record(Cache *record) {
    free(record->response);
}

void push_record(Cache *start, Cache *record) {
    Cache *cur = start;
    pthread_mutex_lock(&cache_mutex);
    printf("Starting caching\n");
    while (cur->next != NULL) {
        cur = cur->next;
        if(cur->request == record->request){
            printf("Find duplicate while pushing cache record\n");
            delete_cache_record(record);
            free(record);
            pthread_mutex_unlock(&cache_mutex);
            return;
        }
    }
    cur->next = record;
    printf("Cached the result, len = %ld\n\n\n", record->response_len);
    pthread_mutex_unlock(&cache_mutex);
}
