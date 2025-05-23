#pragma once

#if MG_ARCH == MG_ARCH_WIN32

// Avoid name clashing; (macro expansion producing 'defined' has undefined
// behaviour). See config.h for user options
#ifndef MG_ENABLE_WINSOCK
#if (!defined(MG_ENABLE_TCPIP) || !MG_ENABLE_TCPIP) && \
    (!defined(MG_ENABLE_LWIP) || !MG_ENABLE_LWIP) &&   \
    (!defined(MG_ENABLE_FREERTOS_TCP) || !MG_ENABLE_FREERTOS_TCP)
#define MG_ENABLE_WINSOCK 1
#else
#define MG_ENABLE_WINSOCK 0
#endif
#endif

#ifndef _CRT_RAND_S
#define _CRT_RAND_S
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef _WINSOCK_DEPRECATED_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#endif

#include <ctype.h>
#include <direct.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#if defined(_MSC_VER) && _MSC_VER < 1700
#define __func__ ""
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;
typedef unsigned char uint8_t;
typedef char int8_t;
typedef unsigned short uint16_t;
typedef short int16_t;
typedef unsigned int uint32_t;
typedef int int32_t;
typedef enum { false = 0, true = 1 } bool;
#else
#include <stdbool.h>
#include <stdint.h>
#if MG_ENABLE_WINSOCK
#include <ws2tcpip.h>
#endif
#endif

#include <process.h>
#include <winerror.h>
#include <winsock2.h>       // fix missing macros and types

// For mg_random()
#if defined(_MSC_VER) && _MSC_VER < 1700
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x400  // Let vc98 pick up wincrypt.h
#endif
#include <wincrypt.h>
#pragma comment(lib, "advapi32.lib")
#endif

#if defined(_MSC_VER) && _MSC_VER <= 1200
  #ifndef IPPROTO_IP
    #define IPPROTO_IP 0
  #endif

  #ifndef IP_ADD_MEMBERSHIP
    struct ip_mreq {
        struct in_addr imr_multiaddr;
        struct in_addr imr_interface;
    };
    #define IP_ADD_MEMBERSHIP  12
  #endif
#endif

// Protect from calls like std::snprintf in app code
// See https://github.com/cesanta/mongoose/issues/1047
#ifndef __cplusplus
#define snprintf _snprintf
#define vsnprintf _vsnprintf
#ifndef strdup  // For MSVC with _DEBUG, see #1359
#define strdup(x) _strdup(x)
#endif
#endif

typedef unsigned long nfds_t;
#if defined(_MSC_VER)
#if MG_ENABLE_WINSOCK
#pragma comment(lib, "ws2_32.lib")
#endif
#ifndef alloca
#define alloca(a) _alloca(a)
#endif
#endif

#define MG_DIRSEP '\\'

#ifndef MG_PATH_MAX
#define MG_PATH_MAX FILENAME_MAX
#endif

#if MG_ENABLE_WINSOCK

#define MG_INVALID_SOCKET INVALID_SOCKET
#define MG_SOCKET_TYPE SOCKET
#define poll(a, b, c) WSAPoll((a), (b), (c))
#define closesocket(x) closesocket(x)
typedef int socklen_t;

#ifndef SO_EXCLUSIVEADDRUSE
#define SO_EXCLUSIVEADDRUSE ((int) (~SO_REUSEADDR))
#endif

#define MG_SOCK_ERR(errcode) ((errcode) < 0 ? WSAGetLastError() : 0)

#define MG_SOCK_PENDING(errcode)                                            \
  (((errcode) < 0) &&                                                       \
   (WSAGetLastError() == WSAEINTR || WSAGetLastError() == WSAEINPROGRESS || \
    WSAGetLastError() == WSAEWOULDBLOCK))

#define MG_SOCK_RESET(errcode) \
  (((errcode) < 0) && (WSAGetLastError() == WSAECONNRESET))

#endif  // MG_ENABLE_WINSOCK

#define realpath(a, b) _fullpath((b), (a), MG_PATH_MAX)
#define sleep(x) Sleep((x) *1000)
#define mkdir(a, b) _mkdir(a)
#define timegm(x) _mkgmtime(x)

#ifndef S_ISDIR
#define S_ISDIR(x) (((x) &_S_IFMT) == _S_IFDIR)
#endif

#ifndef MG_ENABLE_DIRLIST
#define MG_ENABLE_DIRLIST 1
#endif

#ifndef SIGPIPE
#define SIGPIPE 0
#endif

#ifndef MG_ENABLE_POSIX_FS
#define MG_ENABLE_POSIX_FS 1
#endif

#ifndef MG_IO_SIZE
#define MG_IO_SIZE 16384
#endif

#endif
