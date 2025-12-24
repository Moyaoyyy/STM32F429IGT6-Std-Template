#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/stat.h>

#include "bsp_usart.h"

#ifndef STDIN_FILENO
#define STDIN_FILENO 0
#endif
#ifndef STDOUT_FILENO
#define STDOUT_FILENO 1
#endif
#ifndef STDERR_FILENO
#define STDERR_FILENO 2
#endif

extern uint8_t _end;
extern uint8_t _estack;

static uint8_t *s_brk = NULL;

void *_sbrk(ptrdiff_t incr)
{
    uint8_t *prev_brk;

    if (s_brk == NULL)
    {
        s_brk = &_end;
    }

    if ((s_brk + incr) > &_estack)
    {
        errno = ENOMEM;
        return (void *)-1;
    }

    prev_brk = s_brk;
    s_brk += incr;
    return (void *)prev_brk;
}

int _write(int file, const char *ptr, int len)
{
    if ((file == STDOUT_FILENO) || (file == STDERR_FILENO))
    {
        for (int i = 0; i < len; ++i)
        {
            __io_putchar(ptr[i]);
        }
        return len;
    }

    errno = EBADF;
    return -1;
}

int _read(int file, char *ptr, int len)
{
    (void)file;
    (void)ptr;
    (void)len;

    errno = ENOSYS;
    return -1;
}

int _close(int file)
{
    (void)file;
    return 0;
}

int _fstat(int file, struct stat *st)
{
    (void)file;

    if (st == NULL)
    {
        errno = EINVAL;
        return -1;
    }

    st->st_mode = S_IFCHR;
    return 0;
}

int _isatty(int file)
{
    (void)file;
    return 1;
}

int _lseek(int file, int ptr, int dir)
{
    (void)file;
    (void)ptr;
    (void)dir;
    return 0;
}

int _kill(int pid, int sig)
{
    (void)pid;
    (void)sig;
    errno = EINVAL;
    return -1;
}

int _getpid(void)
{
    return 1;
}

void _exit(int status)
{
    (void)status;
    while (1)
    {
        /* hang */
    }
}
