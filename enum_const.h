#ifndef ENUM_CONST_H
#define ENUM_CONST_H

#include <string.h>

#define HTTP "http"
#define HOST "Host:"
#define END_STR '\0'

enum {
    MAX_BUFFER_SIZE = 4096,
    PORT = 8080,
    HOST_SIZE = 50,
    MAX_USERS_COUNT = 10
};

enum error {
    SOCKET_ERROR = -1,
    SEND_ERROR = -1,
    LISTEN_ERROR = -1,
    BIND_ERROR = -1,
    WRITE_ERROR = -1,
    NOT_FOUND_CACHE = -1,
    ADD_INFO_STATUS_ERROR = 0,
    PTHREAD_ERROR = -1
};

#endif // ENUM_CONST_H
