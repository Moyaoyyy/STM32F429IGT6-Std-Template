//
// Created by anyMSG on 11/20/19.
//
#define G_PORT_CONFIG
#define __PFX32__ 1

#ifndef __PFX64__
#ifndef __PFX32__
#error Define Your platform WORD(__PFX32__/__PFX64__) first, I am exit now.
#endif
#endif
#ifndef __PFX32__
#ifndef __PFX64__
#error Define Your platform WORD(__PFX32__/__PFX64__) first, I am exit now.
#endif
#endif

#ifdef __PFX64__
typedef unsigned long int word_t;
// 用于检查一个字节是否为零的掩码
static const word_t BYTE_MASK = 0x7F7F7F7F7F7F7F7FULL;
#elif __PFX32__
typedef unsigned long int word_t;
// 用于检查一个字节是否为零的掩码
static const word_t BYTE_MASK = 0x7f7f7f7fU;
#endif

// 用于检查4个字节的任意一个字节是否为零
static inline word_t has_zero_byte(word_t x)
{
    return ((x - BYTE_MASK) & ~x & (BYTE_MASK << 1)) != 0;
}

// 计算字符串长度的函数
unsigned long int _port_strlen_(const char *s)
{
    const char *p = s;
    // 计算指针是否对齐
    while (((word_t)p & (sizeof(word_t) - 1)) != 0)
    {
        if (*p == '\0')
        {
            return p - s;
        }
        p++;
    }

    // 按字对齐的方式读取内存
    const word_t *wp = (const word_t *)p;
    while (1)
    {
        word_t w = *wp++;
        if (has_zero_byte(w))
        {
            // 如果找到了零字节，再逐个字符检查
            p = (const char *)(wp - 1);
            for (word_t i = 0; i < sizeof(word_t); i++)
            {
                if (p[i] == '\0')
                {
                    return p + i - s;
                }
            }
        }
    }
}

void *_port_memset_(void *dest, int val, unsigned long int len)
{
    unsigned char *ptr = (unsigned char *)dest;
    unsigned char value = (unsigned char)val;

    // 预先处理小部分字节以提高循环效率
    while (len > 0 && ((unsigned long int)ptr % sizeof(unsigned long int)) != 0)
    {
        *ptr++ = value;
        len--;
    }

    // 使用size_t类型进行批量设置
    unsigned long int wide_val = value;
    unsigned long int *wide_ptr;
    // 将value填充到整个size_t中
    for (unsigned long int i = 1; i < sizeof(unsigned long int); i++)
    {
        wide_val |= (wide_val << 8);
    }

    // 设置主要部分
    wide_ptr = (unsigned long int *)ptr;
    while (len >= sizeof(unsigned long int))
    {
        *wide_ptr++ = wide_val;
        len -= sizeof(unsigned long int);
    }

    // 处理剩余部分
    ptr = (unsigned char *)wide_ptr;
    while (len > 0)
    {
        *ptr++ = value;
        len--;
    }

    return dest;
}
