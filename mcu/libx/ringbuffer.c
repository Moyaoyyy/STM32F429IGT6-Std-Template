#define G_RBUFFER

#include "stdlib.h"
#include "string.h"
// #include "api.h"
// #include "hal.h"
// #include "cmsis_os.h"
#include "ringbuffer.h"

int rbclear(rbptr_t rb, unsigned long int len)
{
    if (rb && rb->bf)
    {
        // 为什么又在最后将x赋给了rb->_RV_VALUE_
        unsigned long int x = rb->_RV_VALUE_;
        memset(rb->bf, 0, len);
        rb->count = 0;
        rb->wrp = rb->bf;
        rb->rdp = rb->bf;
        rb->capacity = len;
        rb->_RV_VALUE_ = x;
        return (1);
    }

    return (0);
}

void *rb_init(unsigned long int size)
{
    rbptr_t rbptr = NULL;
    rbptr = (rbptr_t)malloc(sizeof(struct ringbuffer));
    rbptr->bf = (unsigned char *)malloc(size);
    if (rbptr->bf)
    {
        rbclear(rbptr, size);
        // osSemaphoreCreate创建信号量,赋予初始令牌1
        // rbptr->evtrb = osSemaphoreCreate(NULL, 1);
        // rbptr->evtwb = osSemaphoreCreate(NULL, 1);
    }
    else
    {
        free(rbptr);
        rbptr = NULL;
    }

    return (rbptr);
}
// 缓冲区句柄，要读的数据区域，要读得大小
// rbread(msbp->flow, (unsigned char *)&sbp, 4)
int rbread(rbptr_t rb, unsigned char *buf, unsigned long int size)
{
    //	if(!__get_IPSR()) taskENTER_CRITICAL();
    if (rb && buf && size)
    {
        // rb->count当前已使用缓冲区大小
        if (rb->count >= size)
        {
            // in one direction, there is enough data for retrieving
            if (rb->wrp > rb->rdp)
            {
                memcpy(buf, (unsigned char *)rb->rdp, size);
                rb->rdp += size;
            }
            else if (rb->wrp < rb->rdp)
            {
                unsigned long int len1 = rb->bf + rb->capacity - 1 - rb->rdp + 1;
                if (len1 >= size)
                {
                    memcpy(buf, (unsigned char *)rb->rdp, size);
                    rb->rdp += size;
                }
                else
                {
                    unsigned long int len2 = size - len1;
                    memcpy(buf, (unsigned char *)rb->rdp, len1);
                    memcpy(buf + len1, rb->bf, len2);
                    rb->rdp = rb->bf + len2; // Wrap around
                }
            }
            rb->count -= size;
            // osSemaphoreRelease((osSemaphoreId)(rb->evtwb));
            //			if(!__get_IPSR()) taskEXIT_CRITICAL();
            return size;
        }
    }
    //	if(!__get_IPSR()) taskEXIT_CRITICAL();
    return (0);
}
// rbwrite(msbp->flow, (unsigned char *)&flow2put, 4)
int rbwrite(rbptr_t rb, unsigned char *buf, unsigned long int size)
{
    //	if(!__get_IPSR()) taskENTER_CRITICAL();
    if (rb && buf && size)
    {
        if (rb->capacity - rb->count < size)
        {
            ;
        }
        else
        {
            if (rb->wrp >= rb->rdp)
            {
                unsigned long int len1 = rb->bf + rb->capacity - rb->wrp;
                if (len1 >= size)
                {
                    memcpy((unsigned char *)rb->wrp, buf, size);
                    rb->wrp += size;
                }
                else
                {
                    unsigned long int len2 = size - len1;
                    memcpy((unsigned char *)rb->wrp, buf, len1);
                    memcpy(rb->bf, buf + len1, len2);
                    rb->wrp = rb->bf + len2; // Wrap around
                }
            }
            else
            {
                memcpy((unsigned char *)rb->wrp, buf, size);
                rb->wrp += size;
            }
            rb->count += size;
            // osSemaphoreRelease((osSemaphoreId)(rb->evtrb));
            //			if(!__get_IPSR()) taskEXIT_CRITICAL();
            return size;
        }
    }
    //	if(!__get_IPSR()) taskEXIT_CRITICAL();
    return (0);
}

int rbpeek_(rbptr_t rb, unsigned char *buf, unsigned long int size)
{
    //	if(!__get_IPSR()) taskENTER_CRITICAL();
    if (rb && buf && size)
    {
        if (rb->count >= size)
        {
            // in one direction, there is enough data for retrieving
            if (rb->wrp > rb->rdp)
            {
                memcpy(buf, (unsigned char *)rb->rdp, size);
            }
            else if (rb->wrp < rb->rdp)
            {
                // 不理解为什要-1+1
                // rb->bf-rb->rdp=偏移量，也就是读指针在该缓冲区的位置
                unsigned long int len1 = rb->bf + rb->capacity - 1 - rb->rdp + 1;
                if (len1 >= size)
                {
                    memcpy(buf, (unsigned char *)rb->rdp, size);
                }
                else
                {
                    unsigned long int len2 = size - len1;
                    memcpy(buf, (unsigned char *)rb->rdp, len1);
                    memcpy(buf + len1, rb->bf, len2);
                }
            }
            //			if(!__get_IPSR()) taskEXIT_CRITICAL();
            return size;
        }
    }
    //	if(!__get_IPSR()) taskEXIT_CRITICAL();
    return (0);
}

int rbput(rbptr_t rb, unsigned char value)
{
    if (rbfull(rb))
        return -1;
    return (rbwrite(rb, &value, 1));
}

int rbget(rbptr_t rb)
{
    if (rbempty(rb))
        return -1;
    unsigned char r;
    rbread(rb, (unsigned char *)&r, 1);
    return (r);
}

int rbpeek(rbptr_t rb)
{
    if (rbempty(rb))
        return -1;
    unsigned char r;
    rbpeek_(rb, (unsigned char *)&r, 1);
    return (r);
}
// 一次可读大量字节，超过count部分也没事
int rbreadblock(rbptr_t rb, unsigned char *buf, unsigned long int size)
{
    int x = 0;
LAB_REREAD:
    x = 0;
    if (rb && buf && size)
    {
        if (rb->count == 0)
        {
            // empty,then wait
            // 放开写的权限
            // osSemaphoreRelease((osSemaphoreId)(rb->evtwb));
            // 等待读信号量,
            // if (osOK != osSemaphoreWait((osSemaphoreId)(rb->evtrb), 20))
            // {
            // 		goto LAB_REREAD;
            // }
        }

        unsigned long int br = 0;
        if (rb->count >= size)
        {
            br = size;
        }
        else
        {
            br = rb->count;
            size -= br;
        }

        x = rbread(rb, buf, br);

        if (br < size || !x)
        {
            goto LAB_REREAD;
        }
    }

    return (x);
}

int rbwriteblock(rbptr_t rb, unsigned char *buf, unsigned long int size)
{
    int x = 0;
LAB_REWRITE:
    x = 0;
    if (rb && buf && size)
    {
        unsigned long int bw = 0;
        if (rb->capacity - rb->count < size)
        {
            bw = rb->capacity - rb->count;
            size -= bw;
        }
        else
        {
            bw = size;
        }

        x = rbwrite(rb, buf, bw);
        if (bw < size || !x)
        {
            // osSemaphoreWait((osSemaphoreId)(rb->evtwb), 20);
            goto LAB_REWRITE;
        }
    }
    return (x);
}

int rbputblock(rbptr_t rb, unsigned char value)
{
    return (rbwriteblock(rb, &value, 1));
}

int rbgetblock(rbptr_t rb)
{
    unsigned char r;
    rbreadblock(rb, (unsigned char *)&r, 1);
    return (r);
}
