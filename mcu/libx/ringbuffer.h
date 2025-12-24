#ifndef ringbuffer_h
#define ringbuffer_h
#ifndef G_RBUFFER
#define G_RBUFFER extern
#endif

#include <stdint.h>

typedef struct ringbuffer
{
    unsigned char *bf;                // 缓冲区
    unsigned long int capacity;       // 容量
    unsigned long int _RV_VALUE_;     // 缓冲区名关键字
    volatile unsigned long int count; // 当前已使用缓冲区大小
    volatile unsigned char *wrp;      // 可写缓冲区
    volatile unsigned char *rdp;      // 可读缓冲区
    void *evtrb;                      // 写缓冲区信号量
    void *evtwb;                      // 读缓冲区信号量
} rb_t, *rbptr_t;

#define rbempty(prb) ((prb) ? (!((prb)->count)) : 0)
#define rbfull(prb) ((prb) ? (((prb)->capacity == (prb)->count)) : 0)

G_RBUFFER int rbclear(rbptr_t rb, unsigned long int len);

G_RBUFFER int rbput(rbptr_t rb, unsigned char value);

G_RBUFFER int rbget(rbptr_t rb);
G_RBUFFER int rbpeek(rbptr_t rb);
G_RBUFFER int rbpeek_(rbptr_t rb, unsigned char *buf, unsigned long int size);
G_RBUFFER int rbread(rbptr_t rb, unsigned char *buf, unsigned long int size);
G_RBUFFER int rbwrite(rbptr_t rb, unsigned char *buf, unsigned long int size);
G_RBUFFER int rbreadblock(rbptr_t rb, unsigned char *buf,
                          unsigned long int size);
G_RBUFFER int rbwriteblock(rbptr_t rb, unsigned char *buf,
                           unsigned long int size);
G_RBUFFER int rbputblock(rbptr_t rb, unsigned char value);
G_RBUFFER int rbgetblock(rbptr_t rb);
G_RBUFFER void *rb_init(unsigned long int size);

#endif