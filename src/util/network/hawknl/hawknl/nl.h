/*
  HawkNL cross platform network library
  Copyright (C) 2000-2004 Phil Frisbie, Jr. (phil@hawksoft.com)

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public
  License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place - Suite 330,
  Boston, MA  02111-1307, USA.

  Or go to http://www.gnu.org/copyleft/lgpl.html
*/

#ifndef NL_H
#define NL_H

#include <string.h> /* for strcpy, strlen, and memcpy in macros */
#include "hawklib.h"

#ifdef __cplusplus
extern "C" {
#endif
    
#define NL_MAJOR_VERSION 1
#define NL_MINOR_VERSION 7
#define NL_VERSION_STRING "HawkNL 1.7 beta 1"
    
/* define NL_SAFE_COPY for Sparc and other processors that do not allow non-aligned
   memory access. Needed for read* and write* macros */
/*#define NL_SAFE_COPY */

#ifdef HL_WINDOWS_APP
/* undefine this to remove IPX code, Windows only  */
// #define NL_INCLUDE_IPX
#endif

/* undefine this to remove loopback code */
#define NL_INCLUDE_LOOPBACK
    
/* undefine this to remove serial code */
#define NL_INCLUDE_SERIAL
    
/* undefine this to remove modem code */
#define NL_INCLUDE_MODEM
    
/* undefine this to remove parallel code */
#define NL_INCLUDE_PARALLEL
    

/* many CE devices will not allow non-aligned memory access */
#if defined (_WIN32_WCE)
#define NL_SAFE_COPY
#undef NL_INCLUDE_IPX
#endif
    
    
/* Any more needed here? */
#if defined WIN32 || defined WIN64 || defined __i386__ || defined __alpha__ || defined __mips__
#define NL_LITTLE_ENDIAN
#else
#define NL_BIG_ENDIAN
#endif
    
/* How do we detect Solaris 64 and Linux 64 bit? */
#if defined WIN64
#define IS_64_BIT
#endif
    
/* 8 bit */
typedef char NLbyte;
typedef unsigned char NLubyte;
typedef unsigned char NLboolean;
/* 16 bit */
typedef short NLshort;
typedef unsigned short NLushort;
/* 32 bit */
typedef float NLfloat;
#ifdef IS_64_BIT
typedef int NLlong;             /* Longs are 64 bit on a 64 bit CPU, but integers are still 32 bit. */
typedef unsigned int NLulong;   /* This is, of course, not true on Windows (yet another exception), */
                                /* but it does not hurt. */
#else
typedef long NLlong;
typedef unsigned long NLulong;
#endif
/* 64 bit */
typedef double NLdouble;
/* misc. */
typedef int NLint;
typedef unsigned int NLuint;
typedef unsigned int NLenum;
typedef void NLvoid;
typedef NLlong NLsocket;
/* NOTE: NLchar is only to be used for external strings that might be unicode */
typedef char NLchar;
    
typedef struct _NLaddress
{
    NLubyte    addr[32];       /* large enough to hold IPv6 address */
    NLenum     driver;         /* driver type */
    NLboolean  valid;          /* set to NL_TRUE when address is valid */
} NLaddress;
    
/* for backwards compatability */
#if !defined(address_t)
    typedef struct _NLaddress address_t;
#endif
    
    typedef struct _NLtime
    {
        NLlong seconds;     /* seconds since 12:00AM, 1 January, 1970 */
        NLlong mseconds;    /* milliseconds added to the seconds */
        NLlong useconds;    /* microseconds added to the seconds */
    } NLtime;
    
/* max string size limited to 256 (255 plus NULL termination) for MacOS */
#define NL_MAX_STRING_LENGTH   256
    
/* max packet size for NL_UDP* and NL_TCP_PACKETS */
#define NL_MAX_PACKET_LENGTH   16384
    
/* max number groups and sockets per group */
#define NL_MAX_GROUPS           128

#if defined (macintosh)
/* WARNING: Macs only allow up to 32K of local data, don't exceed 4096 */
/* Does NOT apply to Mac OSX */
#define NL_MAX_GROUP_SOCKETS      4096
#else
/* max number of sockets per group NL will handle */
#define NL_MAX_GROUP_SOCKETS      8192
#endif
    
#define NL_INVALID              (-1)
    
/* Boolean values */
#define NL_FALSE                ((NLboolean)(0))
#define NL_TRUE                 ((NLboolean)(1))
    
/* Network types */
/* Only one can be selected at a time */
#define NL_IP                   0x0003  /* all platforms */
#define NL_IPV6                 0x0004  /* not yet implemented, IPv6 address family */
#define NL_LOOP_BACK            0x0005  /* all platforms, for single player client/server emulation with no network */
#define NL_IPX                  0x0006  /* Windows only */
#define NL_SERIAL               0x0007  /* not yet implemented, Windows and Linux only? */
#define NL_MODEM                0x0008  /* not yet implemented, Windows and Linux only? */
#define NL_PARALLEL             0x0009  /* not yet implemented, Windows and Linux only? */
    
/* Socket types */
#define NL_TCP                  0x0010  /* NL_IP (TCP), NL_IPX (SPX), NL_LOOP_BACK */
#define NL_TCP_PACKETS          0x0011  /* NL_IP (TCP), NL_IPX (SPX), NL_LOOP_BACK */
#define NL_UDP                  0x0012  /* NL_IP (UDP), NL_IPX, NL_LOOP_BACK */
#define NL_UDP_BROADCAST        0x0013  /* NL_IP (UDP), NL_IPX, NL_LOOP_BACK */
#define NL_UDP_MULTICAST        0x0014  /* NL_IP (UDP) */
#define NL_RAW                  0x0015  /* NL_SERIAL or NL_PARALLEL */
#define NL_UDP_ENHANCED         0x0016  /* NL_IP (UDP), NL_LOOP_BACK */
    
/* for backwards compatability */
#define NL_RELIABLE             NL_TCP
#define NL_UNRELIABLE           NL_UDP
#define NL_RELIABLE_PACKETS     NL_TCP_PACKETS
#define NL_BROADCAST            NL_UDP_BROADCAST
#define NL_MULTICAST            NL_UDP_MULTICAST
    
/* nlGetString */
#define NL_VERSION              0x0020  /* the version string */
#define NL_NETWORK_TYPES        0x0021  /* space delimited list of available network types */
#define NL_SOCKET_TYPES         0x0022  /* space delimited list of available socket types */
                                        /* only valid AFTER nlSelectNetwork */
/* for backwards compatability */
#define NL_CONNECTION_TYPES     NL_SOCKET_TYPES
    
/* nlGetInteger, nlGetSocketStat, nlClear */
#define NL_PACKETS_SENT         0x0030  /* total packets sent since last nlClear */
#define NL_BYTES_SENT           0x0031  /* total bytes sent since last nlClear */
#define NL_AVE_BYTES_SENT       0x0032  /* average bytes sent per second for the last 8 seconds */
#define NL_HIGH_BYTES_SENT      0x0033  /* highest bytes per second ever sent */
#define NL_PACKETS_RECEIVED     0x0034  /* total packets received since last nlClear */
#define NL_BYTES_RECEIVED       0x0035  /* total bytes received since last nlClear */
#define NL_AVE_BYTES_RECEIVED   0x0036  /* average bytes received per second for the last 8 seconds */
#define NL_HIGH_BYTES_RECEIVED  0x0037  /* highest bytes per second ever received */
#define NL_ALL_STATS            0x0038  /* nlClear only, clears out all counters */
#define NL_OPEN_SOCKETS         0x0039  /* number of open sockets */
    
/* nlEnable, nlDisable, nlSetSocketOpt, nlGetSocketOpt */
#define NL_BLOCKING_IO          0x0040  /* set IO to blocking, default is NL_FALSE for non-blocking IO */

/* nlEnable, nlDisable */
#define NL_SOCKET_STATS         0x0041  /* enable collection of socket read/write statistics, default disabled */
#define NL_BIG_ENDIAN_DATA      0x0042  /* enable big endian data for nlSwap* and read/write macros, default enabled */
#define NL_LITTLE_ENDIAN_DATA   0x0043  /* enable little endian data for nlSwap* and read/write macros, default disabled */
#define NL_MULTIPLE_DRIVERS     0x0044  /* enable multiple drivers to be selected */
    
/* nlPollGroup */
#define NL_READ_STATUS          0x0050  /* poll the read status for all sockets in the group */
#define NL_WRITE_STATUS         0x0051  /* poll the write status for all sockets in the group */
#define NL_ERROR_STATUS         0x0052  /* poll the error status for all sockets in the group */
    
/* nlHint, advanced network settings for experienced developers */
#define NL_LISTEN_BACKLOG       0x0060  /* TCP, SPX: the backlog of connections for listen */

/* nlHint, nlSetSocketOpt, nlGetSocketOpt, advanced network settings for experienced developers */
#define NL_MULTICAST_TTL        0x0061  /* UDP : The multicast TTL value. Default : 1 */
#define NL_REUSE_ADDRESS        0x0062  /* TCP, UDP : Allow IP address to be reused. Default : NL_FALSE */
#define NL_TCP_NO_DELAY         0x0063  /* TCP : disable Nagle algorithm, arg != 0 to disable, 0 to enable */
    
/* errors */
#define NL_NO_ERROR             0x0000  /* no error is stored */
#define NL_NO_NETWORK           0x0100  /* no network was found on init or no network is selected */
#define NL_OUT_OF_MEMORY        0x0101  /* out of memory */
#define NL_INVALID_ENUM         0x0102  /* function called with an invalid NLenum */
#define NL_INVALID_SOCKET       0x0103  /* socket is not valid, or has been terminated */
#define NL_INVALID_PORT         0x0104  /* the port could not be opened */
#define NL_INVALID_TYPE         0x0105  /* the network type is not available */
#define NL_SYSTEM_ERROR         0x0106  /* a system error occurred, call nlGetSystemError */
#define NL_SOCK_DISCONNECT      0x0107  /* the socket should be closed because of a connection loss or error */
#define NL_NOT_LISTEN           0x0108  /* the socket has not been set to listen */
#define NL_CON_REFUSED          0x0109  /* connection refused, or socket already connected */
#define NL_NO_PENDING           0x010a  /* there are no pending connections to accept */
#define NL_BAD_ADDR             0x010b  /* the address or port are not valid */
#define NL_MESSAGE_END          0x010c  /* the end of a reliable stream (TCP) message has been reached */
#define NL_NULL_POINTER         0x010d  /* a NULL pointer was passed to a function */
#define NL_INVALID_GROUP        0x010e  /* the group is not valid, or has been destroyed */
#define NL_OUT_OF_GROUPS        0x010f  /* out of internal group objects */
#define NL_OUT_OF_GROUP_SOCKETS 0x0110  /* the group has no more room for sockets */
#define NL_BUFFER_SIZE          0x0111  /* the buffer was too small to store the data, retry with a larger buffer */
#define NL_PACKET_SIZE          0x0112  /* the size of the packet exceeds NL_MAX_PACKET_LENGTH or the protocol max */
#define NL_WRONG_TYPE           0x0113  /* the function does not support the socket type */
#define NL_CON_PENDING          0x0114  /* a non-blocking connection is still pending */
#define NL_SELECT_NET_ERROR     0x0115  /* a network is already selected, and NL_MULTIPLE_DRIVERS is not enabled, call nlShutDown and nlInit first */
#define NL_PACKET_SYNC          0x0116  /* the NL_RELIABLE_PACKET stream is out of sync */
#define NL_TLS_ERROR            0x0117  /* thread local storage could not be created */
#define NL_TIMED_OUT            0x0118  /* the function timed out */
#define NL_SOCKET_NOT_FOUND     0x0119  /* the socket was not found in the group */
#define NL_STRING_OVER_RUN      0x011a  /* the string is not null terminated, or is longer than NL_MAX_STRING_LENGTH */
#define NL_CONNECTED            0x011b  /* the socket is already connected */
/* for backwards compatability */
#define NL_SOCKET_ERROR         NL_SYSTEM_ERROR
#define NL_CON_TERM             NL_SOCK_DISCONNECT

/* standard multicast TTL settings as recommended by the */
/* white paper at http://www.ipmulticast.com/community/whitepapers/howipmcworks.html */
#define NL_TTL_LOCAL                1   /* local LAN only */
#define NL_TTL_SITE                 15  /* this site */
#define NL_TTL_REGION               63  /* this region */
#define NL_TTL_WORLD                127 /* the world */

/*

  Low level API, a thin layer over Sockets or other network provider.
  
*/

HL_EXP NLboolean HL_APIENTRY nlListen(NLsocket socket);

HL_EXP NLsocket  HL_APIENTRY nlAcceptConnection(NLsocket socket);

HL_EXP NLsocket  HL_APIENTRY nlOpen(NLushort port, NLenum type);

HL_EXP NLboolean HL_APIENTRY nlConnect(NLsocket socket, const NLaddress *address);

HL_EXP NLboolean HL_APIENTRY nlClose(NLsocket socket);

HL_EXP NLint     HL_APIENTRY nlRead(NLsocket socket, /*@out@*/ NLvoid *buffer, NLint nbytes);

HL_EXP NLint     HL_APIENTRY nlWrite(NLsocket socket, const NLvoid *buffer, NLint nbytes);

HL_EXP NLlong    HL_APIENTRY nlGetSocketStat(NLsocket socket, NLenum name);

HL_EXP NLboolean HL_APIENTRY nlClearSocketStat(NLsocket socket, NLenum name);

HL_EXP NLint     HL_APIENTRY nlPollGroup(NLint group, NLenum name, /*@out@*/ NLsocket *sockets, NLint number, NLint timeout);

HL_EXP NLboolean HL_APIENTRY nlPollSocket(NLsocket socket, NLenum name, NLint timeout);

HL_EXP NLboolean HL_APIENTRY nlHint(NLenum name, NLint arg);

HL_EXP NLboolean HL_APIENTRY nlSetSocketOpt(NLsocket socket, NLenum name, NLint arg);

HL_EXP NLint     HL_APIENTRY nlGetSocketOpt(NLsocket socket, NLenum name);

/*

  Address management API
  
*/

HL_EXP /*@null@*/ NLchar*   HL_APIENTRY nlAddrToString(const NLaddress *address, /*@returned@*/ /*@out@*/ NLchar *string);

HL_EXP NLboolean HL_APIENTRY nlStringToAddr(const NLchar *string, /*@out@*/ NLaddress *address);

HL_EXP NLboolean HL_APIENTRY nlGetRemoteAddr(NLsocket socket, /*@out@*/ NLaddress *address);

HL_EXP NLboolean HL_APIENTRY nlSetRemoteAddr(NLsocket socket, const NLaddress *address);

HL_EXP NLboolean HL_APIENTRY nlGetLocalAddr(NLsocket socket, /*@out@*/ NLaddress *address);

HL_EXP NLaddress* HL_APIENTRY nlGetAllLocalAddr(/*@out@*/ NLint *count);

HL_EXP NLboolean HL_APIENTRY nlSetLocalAddr(const NLaddress *address);

HL_EXP /*@null@*/ NLchar* HL_APIENTRY nlGetNameFromAddr(const NLaddress *address, /*@returned@*/ /*@out@*/ NLchar *name);

HL_EXP NLboolean HL_APIENTRY nlGetNameFromAddrAsync(const NLaddress *address, /*@out@*/ NLchar *name);

HL_EXP NLboolean HL_APIENTRY nlGetAddrFromName(const NLchar *name, /*@out@*/ NLaddress *address);

HL_EXP NLboolean HL_APIENTRY nlGetAddrFromNameAsync(const NLchar *name, /*@out@*/ NLaddress *address);

HL_EXP NLboolean HL_APIENTRY nlAddrCompare(const NLaddress *address1, const NLaddress *address2);

HL_EXP NLushort  HL_APIENTRY nlGetPortFromAddr(const NLaddress *address);

HL_EXP NLboolean HL_APIENTRY nlSetAddrPort(NLaddress *address, NLushort port);


/*

  Group management API
  
*/

HL_EXP NLint     HL_APIENTRY nlGroupCreate(void);

HL_EXP NLboolean HL_APIENTRY nlGroupDestroy(NLint group);

HL_EXP NLboolean HL_APIENTRY nlGroupAddSocket(NLint group, NLsocket socket);

HL_EXP NLboolean HL_APIENTRY nlGroupGetSockets(NLint group, /*@out@*/ NLsocket *sockets, /*@in@*/ NLint *number);

HL_EXP NLboolean HL_APIENTRY nlGroupDeleteSocket(NLint group, NLsocket socket);

/*

  Time API
  
*/

HL_EXP NLboolean HL_APIENTRY nlTime(/*@out@*/ NLtime *ts);

/*

  Misc. API
  
*/

HL_EXP NLboolean HL_APIENTRY nlInit(void);

HL_EXP void      HL_APIENTRY nlShutdown(void);

HL_EXP NLboolean HL_APIENTRY nlSelectNetwork(NLenum network);

HL_EXP const /*@observer@*//*@null@*/ NLchar* HL_APIENTRY nlGetString(NLenum name);

HL_EXP NLlong    HL_APIENTRY nlGetInteger(NLenum name);

HL_EXP NLboolean HL_APIENTRY nlGetBoolean(NLenum name);

HL_EXP NLboolean HL_APIENTRY nlClear(NLenum name);

HL_EXP NLenum    HL_APIENTRY nlGetError(void);

HL_EXP const /*@observer@*/ NLchar* HL_APIENTRY nlGetErrorStr(NLenum err);

HL_EXP NLint     HL_APIENTRY nlGetSystemError(void);

HL_EXP const /*@observer@*/ NLchar* HL_APIENTRY nlGetSystemErrorStr(NLint err);

HL_EXP NLboolean HL_APIENTRY nlEnable(NLenum name);

HL_EXP NLboolean HL_APIENTRY nlDisable(NLenum name);

HL_EXP NLushort  HL_APIENTRY nlGetCRC16(NLubyte *data, NLint len);

HL_EXP NLulong   HL_APIENTRY nlGetCRC32(NLubyte *data, NLint len);

HL_EXP NLushort  HL_APIENTRY nlSwaps(NLushort x);

HL_EXP NLulong   HL_APIENTRY nlSwapl(NLulong x);

HL_EXP NLfloat   HL_APIENTRY nlSwapf(NLfloat f);

HL_EXP NLdouble  HL_APIENTRY nlSwapd(NLdouble d);


/* macros for writing/reading packet buffers */
/* NOTE: these also endian swap the data as needed */
/* write* or read* (buffer *, count, data [, length]) */

#ifdef NL_SAFE_COPY
#define writeShort(x, y, z)     {NLushort nl_temps = nlSwaps(z); memcpy((char *)&x[y], (char *)&nl_temps, 2); y += 2;}
#define writeLong(x, y, z)      {NLulong  nl_templ = nlSwapl(z); memcpy((char *)&x[y], (char *)&nl_templ, 4); y += 4;}
#define writeFloat(x, y, z)     {NLfloat  nl_tempf = nlSwapf(z); memcpy((char *)&x[y], (char *)&nl_tempf, 4); y += 4;}
#define writeDouble(x, y, z)    {NLdouble nl_tempd = nlSwapd(z); memcpy((char *)&x[y], (char *)&nl_tempd, 8); y += 8;}
#define readShort(x, y, z)      {memcpy((char *)&z, (char *)&x[y], 2); z = nlSwaps(z); y += 2;}
#define readLong(x, y, z)       {memcpy((char *)&z, (char *)&x[y], 4); z = nlSwapl(z); y += 4;}
#define readFloat(x, y, z)      {memcpy((char *)&z, (char *)&x[y], 4); z = nlSwapf(z); y += 4;}
#define readDouble(x, y, z)     {memcpy((char *)&z, (char *)&x[y], 8); z = nlSwapd(z); y += 8;}

#else /* !NL_SAFE_COPY */
#define writeShort(x, y, z)     {*((NLushort *)((NLbyte *)&x[y])) = nlSwaps(z); y += 2;}
#define writeLong(x, y, z)      {*((NLulong  *)((NLbyte *)&x[y])) = nlSwapl(z); y += 4;}
#define writeFloat(x, y, z)     {*((NLfloat  *)((NLbyte *)&x[y])) = nlSwapf(z); y += 4;}
#define writeDouble(x, y, z)    {*((NLdouble *)((NLbyte *)&x[y])) = nlSwapd(z); y += 8;}
#define readShort(x, y, z)      {z = nlSwaps(*(NLushort *)((NLbyte *)&x[y])); y += 2;}
#define readLong(x, y, z)       {z = nlSwapl(*(NLulong  *)((NLbyte *)&x[y])); y += 4;}
#define readFloat(x, y, z)      {z = nlSwapf(*(NLfloat  *)((NLbyte *)&x[y])); y += 4;}
#define readDouble(x, y, z)     {z = nlSwapd(*(NLdouble *)((NLbyte *)&x[y])); y += 8;}
#endif /* !NL_SAFE_COPY */

#define writeByte(x, y, z)      (*(NLbyte *)&x[y++] = (NLbyte)z)
#define writeBlock(x, y, z, a)  {memcpy((char *)&x[y], (char *)z, a);y += a;}
#define readByte(x, y, z)       (z = *(NLbyte *)&x[y++])
#define readBlock(x, y, z, a)   {memcpy((char *)z, (char *)&x[y], a);y += a;}

#define writeString(x, y, z)    {strcpy((char *)&x[y], (char *)z); y += (strlen((char *)z) + 1);}
#define readString(x, y, z)     {strcpy((char *)z, (char *)&x[y]); y += (strlen((char *)z) + 1);}

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif /* NL_H */

