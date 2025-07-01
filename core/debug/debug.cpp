#include "debug.hpp"

#include <sys/types.h>

#include <cstdint>

#include "core/stream/ostream.hpp"

using namespace ymd;

namespace ymd {
OutputStreamByRoute DEBUGGER;
}

int _write(int fd, char* buf, int len) {
    DEBUGGER.write(buf, len);
    return len;
}

void* _sbrk(ptrdiff_t incr) {
    extern char _end[];
    extern char _heap_end[];
    static char* curbrk = _end;
    if ((curbrk + incr < _end)) {
        return (void*)UINT32_MAX;
    }
    curbrk += incr;
    return curbrk - incr;
}

void _exit(int status) { while (true); }

ssize_t _read(int fd, void* buf, size_t count) { return 0; }

extern "C" {
void _init(void) {}
void _fint(void) {}
void _kill(int pid, int sig) {}
pid_t _getpid() { return 0; }
int raise(int sig) { return 0; }

void abort(void) { while (1); }

void* __dso_handle = nullptr;

void __cxa_atexit(void (*func)(void*), void* objptr, void* dso_handle) {}
}

// https://github.com/esp8266/Arduino/blob/master/cores/esp8266/libc_replacements.cpp
extern "C" {

int _open_r(struct _reent* unused, const char* ptr, int mode) {
    (void)unused;
    (void)ptr;
    (void)mode;
    return 0;
}

int _close_r(struct _reent* unused, int file) {
    (void)unused;
    (void)file;
    return 0;
}

// int _fstat_r(struct _reent* unused, int file, struct stat *st) {
//     (void)unused;
//     (void)file;
//     st->st_mode = S_IFCHR;
//     return 0;
// }

int _lseek_r(struct _reent* unused, int file, int ptr, int dir) {
    (void)unused;
    (void)file;
    (void)ptr;
    (void)dir;
    return 0;
}

int _read_r(struct _reent* unused, int file, char* ptr, int len) {
    (void)unused;
    (void)file;
    (void)ptr;
    (void)len;
    return 0;
}

// int _write_r(struct _reent* r, int file, char *ptr, int len) {
//     (void) r;
//     int pos = len;
//     if (file == STDOUT_FILENO) {
//         while(pos--) {
//             ets_putc(*ptr);
//             ++ptr;
//         }
//     }
//     return len;
// }

int _putc_r(struct _reent* r, int c, FILE* file) __attribute__((weak));

// int _putc_r(struct _reent* r, int c, FILE* file) {
//     (void) r;
//     if (file->_file == STDOUT_FILENO) {
//       ets_putc(c);
//       return c;
//     }
//     return EOF;
// }

// int puts(const char * str) {
//     char c;
//     while((c = *str) != 0) {
//         ets_putc(c);
//         ++str;
//     }
//     ets_putc('\n');
//     return true;
// }

// #undef putchar
// int putchar(int c) {
//     ets_putc(c);
//     return c;
// }

int atexit(void (*func)()) __attribute__((weak));
int atexit(void (*func)()) {
    (void)func;
    return 0;
}
};