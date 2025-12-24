#ifndef __PORT_TYPE__H__
#define __PORT_TYPE__H__
#define __PFX32__
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

#if defined(__GNUC__) || defined(__clang__)
#define forceinline __attribute__((always_inline))
#elif (defined(__GNUC__) && !defined(__clang__))
#define inline inline
#elif defined(_MSC_VER)
#define inline __inline
#define forceinline __forceinline
#endif

#if defined(__GNUC__) && !defined(__CC_ARM)

#ifndef __weak
#define __weak __attribute__((weak))
#endif

#ifndef __packed
#define __packed __attribute__((__packed__))
#endif

#endif

/* xDEPRECATED(version)
 * Declare a variable, type, or function deprecated.
 * The macro must be placed before the declaration.
 * Usage:
 *    xDEPRECATED(3.3) extern int old_var;
 *    xDEPRECATED(3.4) typedef int T1;
 */
#if defined(__GNUC__) && \
    ((__GNUC__ >= 4) || (__GNUC__ == 3) && (__GNUC_MINOR__ >= 1))
#define xDEPRECATED(VERSION_UNUSED) __attribute__((__deprecated__))
#define __FILENAME__                                                         \
    (__builtin_strrchr(__FILE__, '/') ? __builtin_strrchr(__FILE__, '/') + 1 \
                                      : __FILE__)
#elif defined(_MSC_VER)
#define xDEPRECATED(VERSION) __declspec(deprecated("deprecated in " #VERSION))
#else
#define xDEPRECATED(VERSION_UNUSED)
#endif

#ifndef UNUSED
#define UNUSED(x) ((void)(x))
#endif

#define cTRACE_
#define _VERSION_INFO_()                                                       \
    do                                                                         \
    {                                                                          \
        printf("\r\n\t%s %s %s<chinmel@163.com>\r\n\r\n", _THIS_PROJECT_NAME_, \
               _THIS_MODULE_NAME_, _VERSION_);                                 \
    } while (0)

#define DBG_INFO(x) printf("[%s-%s]%s\r\n", __DATE__, __TIME__, (x))

#define DBG_ERROR(x)                                                      \
    printf("[%s-%s] %s [%s]:[%d]\r\n", __DATE__, __TIME__, (x), __FILE__, \
           __LINE__)

#define DBG_LINE()                                                            \
    printf("[%s-%s]DBG %s() [%s]:[%d]\r\n", __DATE__, __TIME__, __FUNCTION__, \
           __FILE__, __LINE__)

#define DBG_EXIT(x) \
    do              \
    {               \
        DBG_LINE(); \
    } while (1);

#define IS_KEY_FOUND_STRING_SEARCH(instring, keystring, firsthandler) \
    ((instring) && (keystring) &&                                     \
     NULL != ((firsthandler) = strstr((const char *)(instring), (keystring))))
#define IS_KEY_FOUND_STRING_CMP(instring, keystring) \
    ((instring) && (keystring) &&                    \
     0 == strcmp((const char *)(instring), (keystring)))
#define IS_KEY_FOUND_STRING_CMPX(instring, keystring, length) \
    ((instring) && (keystring) &&                             \
     0 == strncmp((const char *)(instring), (keystring), length))
#define KEY_FETCH_INDEX(in, key) ((in)[key])

#ifdef _DEBUG_
#define IF_FAILED_RETURN_EXP(v, x, e, r) \
    if ((((v)) == ((x))))                \
    {                                    \
        e;                               \
        return ((r));                    \
    }
#define IF_FAILED_RETURN(v, x, r)      \
    if ((((v)) == ((x))))              \
    {                                  \
        DBG_ERROR("IF_FAILED_RETURN"); \
        return ((r));                  \
    }
#define IF_FAILED(v, x, e) \
    if ((((v)) == ((x))))  \
    e

#define IFERR_EXIT(x) IF_FAILED(0, (x), { XAPP_ON_SIGINT_ROUTINE(__LINE__); })
#define IFERR_NONE_FAILED_EXIT(x) \
    IF_FAILED(MODULE_ERR_NONE, !(x), { XAPP_ON_SIGINT_ROUTINE(__LINE__); })
#else
#define IF_FAILED_RETURN_EXP(v, x, e, r) ((x))
#define IF_FAILED_RETURN(v, x, r) ((x))
#define IF_FAILED(v, x, e) ((x))
#endif

#ifndef NULL
#define NULL ((void *)(0))
#endif
#define _CNOP_() ((void)NULL)

#if defined(__PFX64__)
#define PRIL "l"

typedef char _x8_;
typedef unsigned char u8;
typedef signed char s8;

typedef short int _x16_;
typedef unsigned short int u16;
typedef signed short int s16;

typedef int _x32_;
typedef unsigned int u32;
typedef signed int s32;

typedef long _x64_;
typedef long long int __x64__;
typedef unsigned long __u64__;
typedef unsigned long long int u64;
typedef signed long long int s64;

typedef const char cx8;
typedef const unsigned char cu8;
typedef const signed char cs8;

typedef const short int cx16;
typedef const unsigned short int cu16;
typedef const signed short int cs16;

typedef const int cx32;
typedef const signed int cs32;
typedef const unsigned int cu32;

typedef const long long int cx64;
typedef const signed long long int cs64;
typedef const unsigned long long int cu64;

typedef struct xuint128
{
    u64 first;
    u64 second;
} u128;

/*
 * ====msg_strcut_complie_check()====
 *
 * ==OS's complier struct size==
 * sizeof( s8): 1	sizeof( float): 4
 * sizeof( short int): 2	sizeof( int): 4
 * sizeof( long): 8
 * sizeof( long int): 8	sizeof( long long int): 8
 * sizeof( size_t): 8	sizeof( void*): 8
 *
 *
 * =============================
 * sizeof(msg_length)=2
 * sizeof(retries)=1
 * sizeof(_reserved_)=1
 * sizeof(_src_handler_)=4
 * sizeof(type)=4
 * sizeof(destination_cid)=8
 * sizeof(source_cid)=8
 * sizeof(verify_string[16])=16
 * sizeof(content_length)=2
 * sizeof(content)=8
 * sizeof(msg_struct)=64,sum =130
 *
 * ==msg_strcut_complie_check() end==
 */

typedef double osfixednumber_t;
typedef s64 mof_t;
typedef _x32_ retvalue_t;
typedef u64 nsize_t;

typedef _x32_ osfd_t;
typedef _x8_ osstring_t;
typedef u32 ostime_t;
typedef u16 osport_t;
typedef u64 osword_t;
typedef u32 oshalfword_t;
typedef s64 osretmax_t;
typedef nsize_t osvaluemax_t;
typedef u64 osvoid2word_t;
typedef s64 osindex_t;
typedef s64 ostimeinterval_t;
typedef _x64_ ostimevalue_t;

typedef u32 idl_t;
typedef u16 ids_t;
typedef u16 msg_width_t;
typedef u32 version_t;

typedef nsize_t oprCode_t;
typedef osstring_t *oprDesp_t;
typedef idl_t msg_srcID_t;
typedef idl_t msg_desID_t;
typedef osstring_t *msg_content_t;
typedef ostime_t msg_time_t;
typedef osstring_t *msg_misc_t;
typedef osword_t device_id_t;

#elif defined(__PFX32__)
#define PRIL ""
typedef char _x8_;
typedef signed char s8;
typedef unsigned char u8;
typedef short int _x16_;
typedef unsigned short int u16;
typedef signed short int s16;
typedef int _x32_;
typedef long _xl32_;
typedef signed long int s32;
typedef unsigned long int u32;

typedef long long _x64_;
typedef long long int __x64__;
typedef unsigned long long __u64__;
typedef unsigned long long int u64;
typedef signed long long int s64;

typedef const char cx8;
typedef const unsigned char cu8;
typedef const signed char cs8;

typedef const short int cx16;
typedef const unsigned short int cu16;
typedef const signed short int cs16;

typedef const int cx32;
typedef const signed int cs32;
typedef const unsigned int cu32;

typedef const long long int cx64;
typedef const signed long long int cs64;
typedef const unsigned long long int cu64;

/*
 * ====msg_strcut_complie_check()====
 *
 * ==OS's complier struct size==
 * sizeof( s8): 1	sizeof( float): 4
 * sizeof( short int): 2	sizeof( int): 4
 * sizeof( long int): 4	sizeof( long long int): 8
 * sizeof( size_t): 4	sizeof( void*): 4
 *
 *
 * =============================
 * sizeof(msg_length)=2
 * sizeof(retries)=1
 * sizeof(_reserved_)=1
 * sizeof(_src_handler_)=4
 * sizeof(type)=4
 * sizeof(destination_cid)=4
 * sizeof(source_cid)=4
 * sizeof(verify_string[16])=16
 * sizeof(content_length)=2
 * sizeof(content)=4
 * sizeof(msg_struct)=40,sum =98
 *
 * ==msg_strcut_complie_check() end==
 * */
typedef double osfixednumber_t;
typedef s32 mof_t;
typedef _x32_ retvalue_t;
typedef u32 nsize_t;

typedef _xl32_ oslong_t;
typedef _x32_ osfd_t;
typedef _x8_ osstring_t;
typedef u32 ostime_t;
typedef u16 osport_t;
typedef u32 osword_t;
typedef u16 oshalfword_t;
typedef s32 osretmax_t;
typedef nsize_t osvaluemax_t;
typedef u32 osvoid2word_t;
typedef s32 osindex_t;
typedef s32 ostimeinterval_t;
typedef _x32_ ostimevalue_t;

typedef u32 idl_t;
typedef u16 ids_t;
typedef u16 msg_width_t;
typedef u32 version_t;

typedef nsize_t oprCode_t;
typedef osstring_t *oprDesp_t;
typedef idl_t msg_srcID_t;
typedef idl_t msg_desID_t;
typedef osstring_t *msg_content_t;
typedef ostime_t msg_time_t;
typedef osstring_t *msg_misc_t;
typedef osword_t device_id_t;

#endif

typedef enum
{
    OBOOT_DEVICE_RVD_NONE = 0,
    OBOOT_DEVICE_RVD_TTY_IN,
    OBOOT_DEVICE_RVD_TTY_OUT,
    OBOOT_DEVICE_RVD_TTY_ERROR,
    __OBOOT_DEVICE_RVD_TTY__,
    OBOOT_DEVICE_RVD_SYS_CRM,
    OBOOT_DEVICE_RVD_SYS_MEMPOOL,
    OBOOT_DEVICE_RVD_SYS_LIBX,
    OBOOT_DEVICE_RVD_SYS_OTHER,
    __OBOOT_DEVICE_RVD_SYS__,
    OBOOT_DEVICE_RVD_OTHER,
    __OBOOT_HAL_DEVICE_RVD__
} device_id_reserved_t;
#define device_id_check(devid) \
    ((__OBOOT_HAL_DEVICE_RVD__ < (devid)) ? (devid) : OBOOT_DEVICE_RVD_NONE)

#if __STDC_VERSION__ >= 199901L

#include "stdint.h"

#elif !defined(SIZE_MAX)
#define SIZE_MAX (sizeof(nsize_t) - 1)
#endif

#ifndef _ASM_GENERIC_ERRNO_BASE_H
#define EPERM 1            /* Operation not permitted */
#define ENOENT 2           /* No such file or directory */
#define ESRCH 3            /* No such process */
#define EINTR 4            /* Interrupted system call */
#define EIO 5              /* I/O error */
#define ENXIO 6            /* No such device or address */
#define E2BIG 7            /* Argument list too long */
#define ENOEXEC 8          /* Exec format error */
#define EBADF 9            /* Bad file number */
#define ECHILD 10          /* No child processes */
#define EAGAIN 11          /* Try again */
#define ENOMEM 12          /* Out of memory */
#define EACCES 13          /* Permission denied */
#define EFAULT 14          /* Bad address */
#define ENOTBLK 15         /* Block device required */
#define EBUSY 16           /* Device or resource busy */
#define EEXIST 17          /* File exists */
#define EXDEV 18           /* Cross-device link */
#define ENODEV 19          /* No such device */
#define ENOTDIR 20         /* Not a directory */
#define EISDIR 21          /* Is a directory */
#define EINVAL 22          /* Invalid argument */
#define ENFILE 23          /* File table overflow */
#define EMFILE 24          /* Too many open files */
#define ENOTTY 25          /* Not a typewriter */
#define ETXTBSY 26         /* Text file busy */
#define EFBIG 27           /* File too large */
#define ENOSPC 28          /* No space left on device */
#define ESPIPE 29          /* Illegal seek */
#define EROFS 30           /* Read-only file system */
#define EMLINK 31          /* Too many links */
#define EPIPE 32           /* Broken pipe */
#define EDOM 33            /* Math argument out of domain of func */
#define ERANGE 34          /* Math result not representable */
#define EDEADLK 35         /* Resource deadlock would occur */
#define ENAMETOOLONG 36    /* File name too long */
#define ENOLCK 37          /* No record locks available */
#define ENOSYS 38          /* Function not implemented */
#define ENOTEMPTY 39       /* Directory not empty */
#define ELOOP 40           /* Too many symbolic links encountered */
#define EWOULDBLOCK EAGAIN /* Operation would block */
#define ENOMSG 42          /* No message of desired type */
#define EIDRM 43           /* Identifier removed */
#define ECHRNG 44          /* Channel number out of range */
#define EL2NSYNC 45        /* Level 2 not synchronized */
#define EL3HLT 46          /* Level 3 halted */
#define EL3RST 47          /* Level 3 reset */
#define ELNRNG 48          /* Link number out of range */
#define EUNATCH 49         /* Protocol driver not attached */
#define ENOCSI 50          /* No CSI structure available */
#define EL2HLT 51          /* Level 2 halted */
#define EBADE 52           /* Invalid exchange */
#define EBADR 53           /* Invalid request descriptor */
#define EXFULL 54          /* Exchange full */
#define ENOANO 55          /* No anode */
#define EBADRQC 56         /* Invalid request code */
#define EBADSLT 57         /* Invalid slot */
#define EDEADLOCK EDEADLK
#define EBFONT 59          /* Bad font file format */
#define ENOSTR 60          /* Device not a stream */
#define ENODATA 61         /* No data available */
#define ETIME 62           /* Timer expired */
#define ENOSR 63           /* Out of streams resources */
#define ENONET 64          /* Machine is not on the network */
#define ENOPKG 65          /* Package not installed */
#define EREMOTE 66         /* Object is remote */
#define ENOLINK 67         /* Link has been severed */
#define EADV 68            /* Advertise error */
#define ESRMNT 69          /* Srmount error */
#define ECOMM 70           /* Communication error on send */
#define EPROTO 71          /* Protocol error */
#define EMULTIHOP 72       /* Multihop attempted */
#define EDOTDOT 73         /* RFS specific error */
#define EBADMSG 74         /* Not a data message */
#define EOVERFLOW 75       /* Value too large for defined data type */
#define ENOTUNIQ 76        /* Name not unique on network */
#define EBADFD 77          /* File descriptor in bad state */
#define EREMCHG 78         /* Remote address changed */
#define ELIBACC 79         /* Can not access a needed shared library */
#define ELIBBAD 80         /* Accessing a corrupted shared library */
#define ELIBSCN 81         /* .lib section in a.out corrupted */
#define ELIBMAX 82         /* Attempting to link in too many shared libraries */
#define ELIBEXEC 83        /* Cannot exec a shared library directly */
#define EILSEQ 84          /* Illegal byte sequence */
#define ERESTART 85        /* Interrupted system call should be restarted */
#define ESTRPIPE 86        /* Streams pipe error */
#define EUSERS 87          /* Too many users */
#define ENOTSOCK 88        /* Socket operation on non-socket */
#define EDESTADDRREQ 89    /* Destination address required */
#define EMSGSIZE 90        /* Message too long */
#define EPROTOTYPE 91      /* Protocol wrong type for socket */
#define ENOPROTOOPT 92     /* Protocol not available */
#define EPROTONOSUPPORT 93 /* Protocol not supported */
#define ESOCKTNOSUPPORT 94 /* Socket type not supported */
#define EOPNOTSUPP 95      /* Operation not supported on transport endpoint */
#define EPFNOSUPPORT 96    /* Protocol family not supported */
#define EAFNOSUPPORT 97    /* Address family not supported by protocol */
#define EADDRINUSE 98      /* Address already in use */
#define EADDRNOTAVAIL 99   /* Cannot assign requested address */
#define ENETDOWN 100       /* Network is down */
#define ENETUNREACH 101    /* Network is unreachable */
#define ENETRESET 102      /* Network dropped connection because of reset */
#define ECONNABORTED 103   /* Software caused connection abort */
#define ECONNRESET 104     /* Connection reset by peer */
#define ENOBUFS 105        /* No buffer space available */
#define EISCONN 106        /* Transport endpoint is already connected */
#define ENOTCONN 107       /* Transport endpoint is not connected */
#define ESHUTDOWN 108      /* Cannot send after transport endpoint shutdown */
#define ETOOMANYREFS 109   /* Too many references: cannot splice */
#define ETIMEDOUT 110      /* Connection timed out */
#define ECONNREFUSED 111   /* Connection refused */
#define EHOSTDOWN 112      /* Host is down */
#define EHOSTUNREACH 113   /* No route to host */
#define EALREADY 114       /* Operation already in progress */
#define EINPROGRESS 115    /* Operation now in progress */
#define ESTALE 116         /* Stale NFS file handle */
#define EUCLEAN 117        /* Structure needs cleaning */
#define ENOTNAM 118        /* Not a XENIX named type file */
#define ENAVAIL 119        /* No XENIX semaphores available */
#define EISNAM 120         /* Is a named type file */
#define EREMOTEIO 121      /* Remote I/O error */
#define EDQUOT 122         /* Quota exceeded */
#define ENOMEDIUM 123      /* No medium found */
#define EMEDIUMTYPE 124    /* Wrong medium type */
#define ECANCELED 125      /* Operation Canceled */
#define ENOKEY 126         /* Required key not available */
#define EKEYEXPIRED 127    /* Key has expired */
#define EKEYREVOKED 128    /* Key has been revoked */
#define EKEYREJECTED 129   /* Key was rejected by service */
/* for robust mutexes */
#define EOWNERDEAD 130      /* Owner died */
#define ENOTRECOVERABLE 131 /* State not recoverable */
#define ERFKILL 132         /* Operation not possible due to RF-kill */
#define EHWPOISON 133       /* Memory page has hardware error */
#endif

#define _EOK (0)
#define _EMEM (-EPERM)
#define _ENOENT (-ENOENT)             /* No such file or directory */
#define _ESRCH (-ESRCH)               /* No such process */
#define _EINTR (-EINTR)               /* Interrupted system call */
#define _EIO (-EIO)                   /* I/O error */
#define _ENXIO (-ENXIO)               /* No such device or address */
#define _E2BIG (-E2BIG)               /* Argument list too long */
#define _ENOEXEC (-ENOEXEC)           /* Exec format error */
#define _EBADF (-EBADF)               /* Bad file number */
#define _ECHILD (-ECHILD)             /* No child processes */
#define _EAGAIN (-EAGAIN)             /* Try again */
#define _ENOMEM (-ENOMEM)             /* Out of memory */
#define _EACCES (-EACCES)             /* Permission denied */
#define _EFAULT (-EFAULT)             /* Bad address */
#define _ENOTBLK (-ENOTBLK)           /* Block device required */
#define _EBUSY (-EBUSY)               /* Device or resource busy */
#define _EEXIST (-EEXIST)             /* File exists */
#define _EXDEV (-EXDEV)               /* Cross-device link */
#define _ENODEV (-ENODEV)             /* No such device */
#define _ENOTDIR (-ENOTDIR)           /* Not a directory */
#define _EISDIR (-EISDIR)             /* Is a directory */
#define _EINVAL (-EINVAL)             /* Invalid argument */
#define _ENFILE (-ENFILE)             /* File table overflow */
#define _EMFILE (-EMFILE)             /* Too many open files */
#define _ENOTTY (-ENOTTY)             /* Not a typewriter */
#define _ETXTBSY (-ETXTBSY)           /* Text file busy */
#define _EFBIG (-EFBIG)               /* File too large */
#define _ENOSPC (-ENOSPC)             /* No space left on device */
#define _ESPIPE (-ESPIPE)             /* Illegal seek */
#define _EROFS (-EROFS)               /* Read-only file system */
#define _EMLINK (-EMLINK)             /* Too many links */
#define _EPIPE (-EPIPE)               /* Broken pipe */
#define _EDOM (-EDOM)                 /* Math argument out of domain of func */
#define _ERANGE (-ERANGE)             /* Math result not representable */
#define _EDEADLK (-EDEADLK)           /* Resource deadlock would occur */
#define _ENAMETOOLONG (-ENAMETOOLONG) /* File name too long */
#define _ENOLCK (-ENOLCK)             /* No record locks available */
#define _ENOSYS (-ENOSYS)             /* Function not implemented */
#define _ENOTEMPTY (-ENOTEMPTY)       /* Directory not empty */
#define _ELOOP (-ELOOP)               /* Too many symbolic links encountered */
#define _EWOULDBLOCK (-EWOULDBLOCK)   /* Operation would block */
#define _ENOMSG (-ENOMSG)             /* No message of desired type */
#define _EIDRM (-EIDRM)               /* Identifier removed */
#define _ECHRNG (-ECHRNG)             /* Channel number out of range */
#define _EL2NSYNC (-EL2NSYNC)         /* Level 2 not synchronized */
#define _EL3HLT (-EL3HLT)             /* Level 3 halted */
#define _EL3RST (-EL3RST)             /* Level 3 reset */
#define _ELNRNG (-ELNRNG)             /* Link number out of range */
#define _EUNATCH (-EUNATCH)           /* Protocol driver not attached */
#define _ENOCSI (-ENOCSI)             /* No CSI structure available */
#define _EL2HLT (-EL2HLT)             /* Level 2 halted */
#define _EBADE (-EBADE)               /* Invalid exchange */
#define _EBADR (-EBADR)               /* Invalid request descriptor */
#define _EXFULL (-EXFULL)             /* Exchange full */
#define _ENOANO (-ENOANO)             /* No anode */
#define _EBADRQC (-EBADRQC)           /* Invalid request code */
#define _EBADSLT (-EBADSLT)           /* Invalid slot */
#define _EDEADLOCK (-EDEADLOCK)
#define _EBFONT (-EBFONT)       /* Bad font file format */
#define _ENOSTR (-ENOSTR)       /* Device not a stream */
#define _ENODATA (-ENODATA)     /* No data available */
#define _ETIME (-ETIME)         /* Timer expired */
#define _ENOSR (-ENOSR)         /* Out of streams resources */
#define _ENONET (-ENONET)       /* Machine is not on the network */
#define _ENOPKG (-ENOPKG)       /* Package not installed */
#define _EREMOTE (-EREMOTE)     /* Object is remote */
#define _ENOLINK (-ENOLINK)     /* Link has been severed */
#define _EADV (-EADV)           /* Advertise error */
#define _ESRMNT (-ESRMNT)       /* Srmount error */
#define _ECOMM (-ECOMM)         /* Communication error on send */
#define _EPROTO (-EPROTO)       /* Protocol error */
#define _EMULTIHOP (-EMULTIHOP) /* Multihop attempted */
#define _EDOTDOT (-EDOTDOT)     /* RFS specific error */
#define _EBADMSG (-EBADMSG)     /* Not a data message */
#define _EOVERFLOW (-EOVERFLOW) /* Value too large for defined data type */
#define _ENOTUNIQ (-ENOTUNIQ)   /* Name not unique on network */
#define _EBADFD (-EBADFD)       /* File descriptor in bad state */
#define _EREMCHG (-EREMCHG)     /* Remote address changed */
#define _ELIBACC (-ELIBACC)     /* Can not access a needed shared library */
#define _ELIBBAD (-ELIBBAD)     /* Accessing a corrupted shared library */
#define _ELIBSCN (-ELIBSCN)     /* .lib section in a.out corrupted */
#define _ELIBMAX \
    (-ELIBMAX)                /* Attempting to link in too many shared libraries */
#define _ELIBEXEC (-ELIBEXEC) /* Cannot exec a shared library directly */
#define _EILSEQ (-EILSEQ)     /* Illegal byte sequence */
#define _ERESTART                                                                          \
    (-ERESTART)                             /* Interrupted system call should be restarted \
                                             */
#define _ESTRPIPE (-ESTRPIPE)               /* Streams pipe error */
#define _EUSERS (-EUSERS)                   /* Too many users */
#define _ENOTSOCK (-ENOTSOCK)               /* Socket operation on non-socket */
#define _ENETERROR (-ENOTSOCK)              /* Socket operation on non-socket */
#define _EDESTADDRREQ (-EDESTADDRREQ)       /* Destination address required */
#define _EMSGSIZE (-EMSGSIZE)               /* Message too long */
#define _EPROTOTYPE (-EPROTOTYPE)           /* Protocol wrong type for socket */
#define _ENOPROTOOPT (-ENOPROTOOPT)         /* Protocol not available */
#define _EPROTONOSUPPORT (-EPROTONOSUPPORT) /* Protocol not supported */
#define _ESOCKTNOSUPPORT (-ESOCKTNOSUPPORT) /* Socket type not supported */
#define _EOPNOTSUPP \
    (-EOPNOTSUPP)                     /* Operation not supported on transport endpoint */
#define _EPFNOSUPPORT (-EPFNOSUPPORT) /* Protocol family not supported */
#define _EAFNOSUPPORT \
    (-EAFNOSUPPORT)                     /* Address family not supported by protocol */
#define _EADDRINUSE (-EADDRINUSE)       /* Address already in use */
#define _EADDRNOTAVAIL (-EADDRNOTAVAIL) /* Cannot assign requested address */
#define _ENETDOWN (-ENETDOWN)           /* Network is down */
#define _ENETUNREACH (-ENETUNREACH)     /* Network is unreachable */
#define _ENETRESET \
    (-ENETRESET)                      /* Network dropped connection because of reset */
#define _ECONNABORTED (-ECONNABORTED) /* Software caused connection abort */
#define _ECONNRESET (-ECONNRESET)     /* Connection reset by peer */
#define _ENOBUFS (-ENOBUFS)           /* No buffer space available */
#define _EISCONN (-EISCONN)           /* Transport endpoint is already connected */
#define _ENOTCONN (-ENOTCONN)         /* Transport endpoint is not connected */
#define _ESHUTDOWN \
    (-ESHUTDOWN)                      /* Cannot send after transport endpoint shutdown */
#define _ETOOMANYREFS (-ETOOMANYREFS) /* Too many references: cannot splice */
#define _ETIMEDOUT (-ETIMEDOUT)       /* Connection timed out */
#define _ECONNREFUSED (-ECONNREFUSED) /* Connection refused */
#define _EHOSTDOWN (-EHOSTDOWN)       /* Host is down */
#define _EHOSTUNREACH (-EHOSTUNREACH) /* No route to host */
#define _EALREADY (-EALREADY)         /* Operation already in progress */
#define _EINPROGRESS (-EINPROGRESS)   /* Operation now in progress */
#define _ESTALE (-ESTALE)             /* Stale NFS file handle */
#define _EUCLEAN (-EUCLEAN)           /* Structure needs cleaning */
#define _ENOTNAM (-ENOTNAM)           /* Not a XENIX named type file */
#define _ENAVAIL (-ENAVAIL)           /* No XENIX semaphores available */
#define _EISNAM (-EISNAM)             /* Is a named type file */
#define _EREMOTEIO (-EREMOTEIO)       /* Remote I/O error */
#define _EDQUOT (-EDQUOT)             /* Quota exceeded */
#define _ENOMEDIUM (-ENOMEDIUM)       /* No medium found */
#define _EMEDIUMTYPE (-EMEDIUMTYPE)   /* Wrong medium type */
#define _ECANCELED (-ECANCELED)       /* Operation Canceled */
#define _ENOKEY (-ENOKEY)             /* Required key not available */
#define _EKEYEXPIRED (-EKEYEXPIRED)   /* Key has expired */
#define _EKEYREVOKED (-EKEYREVOKED)   /* Key has been revoked */
#define _EKEYREJECTED (-EKEYREJECTED) /* Key was rejected by service */
/* for robust mutexes */
#define _EOWNERDEAD (-EOWNERDEAD)           /* Owner died */
#define _ENOTRECOVERABLE (-ENOTRECOVERABLE) /* State not recoverable */
#define _ERFKILL (-ERFKILL)                 /* Operation not possible due to RF-kill */
#define _EHWPOISON (-EHWPOISON)             /* Memory page has hardware error */

#define DEFAULT_ERRORCODE_FAILED (_EMEM)

typedef osword_t (*__subclass_callback__)(void *, void *, osword_t, osword_t *);
/*link是成员地址，type是结构体类型,member是成员名称*/
#define list_entry(link, type, member) \
    ((type *)((_x8_ *)(link) - (osword_t)(&((type *)0)->member)))

// 求成员element相对与结构体stru_name的偏移量（在这个结构体当中的位置），(struct
// stru_name*)0)将结构体的地址变为从零开始，这样就可以求出一个成员的偏移量
#define offsetof(type, member) ((osword_t)(&((type *)0)->member))
#define has_member(type, member) \
    ((offsetof(type, member) != (osword_t)(-1)) ? 1 : 0)

typedef enum
{
    bFALSE = 0,
    bTRUE = -1
} oboolean_t;

#define __STRINGIFY__(x) #x

#define HS_PORT_TYPE
#define HS_PORT_TYPE_INTERNAL_ONLY
#ifndef HS__RVD__HEAD__
// #include "__port_config__.h"
#endif
#endif
