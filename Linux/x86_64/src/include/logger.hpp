#ifndef LOGGER_HPP
#define LOGGER_HPP

// 3rd Party Libraries

// Platform Libraries

// Project Libraries

// Standard Libraries
#include <stdio.h>
#include <stdarg.h>

#ifdef LOG

typedef enum error_level {
    INFO = 0,
    OKAY,
    WARN,
    ERRO
}error_level_t;

static inline void write(error_level_t error_level,
                         FILE * stream,
                         const char * file_name,
                         const int line_number,
                         const char * fmt, ...) {
    char level = 'i';
    va_list args = {};

    switch (error_level) {
        case (INFO): break;
        case (OKAY): {level = '+'; break;}
        case (WARN): {level = '!'; break;}
        case (ERRO): {level = 'x'; break;}
    }

    fprintf(stream, "[%c]:[%s:%d]: ", level, file_name, line_number);
    va_start(args, fmt);
    vfprintf(stream, fmt, args);
    va_end(args);
    fprintf(stream, "\n");
    fflush(stream);
}
#define INFO(stream, ...) write(INFO, stream, __FILE_NAME__, __LINE__, __VA_ARGS__)
#define OKAY(stream, ...) write(OKAY, stream, __FILE_NAME__, __LINE__, __VA_ARGS__)
#define WARN(stream, ...) write(WARN, stream, __FILE_NAME__, __LINE__, __VA_ARGS__)
#define ERRO(stream, ...) write(ERRO, stream, __FILE_NAME__, __LINE__, __VA_ARGS__)

#else

#define INFO(stream, ...)
#define OKAY(stream, ...)
#define WARN(stream, ...)
#define ERRO(stream, ...)

#endif // LOG
#endif // LOGGER_HPP
