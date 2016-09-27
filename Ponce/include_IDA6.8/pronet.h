// socket API wrappers

#ifndef __PRONET_H__
#define __PRONET_H__

#ifdef __NT__
#  pragma pack(push)
#  include <winsock2.h>   // may change structure packing?!
#  pragma pack(pop)
#else    // __NT__
#  include <errno.h>
#  include <sys/socket.h>
#endif

#ifdef __NT__
#ifdef UNDER_CE
#pragma comment(lib, "ws2")
#else
#pragma comment(lib, "WS2_32.lib")
#endif
#endif

/*! \file pronet.h

  \brief Network related functions

  Each of the following functions work just like their C standard equivalent, only
  they are safer and system independent.
*/

//---------------------------------------------------------------------------
#ifdef __NT__
#  define SIG_SAFE_CALL(expr) return expr
#  define SOCKLEN_T  int
#  define SOCKBUF_T  char *
#else
#  define SIG_SAFE_CALL(expr)            \
     do                                  \
     {                                   \
       long rc = expr;                   \
       if ( rc != -1 || errno != EINTR ) \
         return rc;                      \
     }                                   \
     while (true)
#  define SOCKLEN_T  socklen_t
#  define SOCKBUF_T  void *
#endif

//---------------------------------------------------------------------------
inline ssize_t qsendto(int socket, const SOCKBUF_T buf, size_t size, int flags, const struct sockaddr *dest_addr, SOCKLEN_T addrlen)
{
  SIG_SAFE_CALL(::sendto(socket, buf, size, flags, dest_addr, addrlen));
}

//---------------------------------------------------------------------------
inline ssize_t qrecvfrom(int socket, SOCKBUF_T buf, size_t size, int flags, struct sockaddr *src_addr, SOCKLEN_T *addrlen)
{
  SIG_SAFE_CALL(::recvfrom(socket, buf, size, flags, src_addr, addrlen));
}

//---------------------------------------------------------------------------
inline ssize_t qsend(int socket, const void *buf, size_t size)
{
#ifdef __NT__
  return qsendto(socket, (SOCKBUF_T)buf, size, 0, NULL, 0);
#else
  SIG_SAFE_CALL(::send(socket, buf, size, 0));
#endif
}

//---------------------------------------------------------------------------
inline ssize_t qrecv(int socket, void *buf, size_t size)
{
#ifdef __NT__
  return qrecvfrom(socket, (SOCKBUF_T)buf, size, 0, NULL, NULL);
#else
  SIG_SAFE_CALL(::recv(socket, buf, size, 0));
#endif
}

//---------------------------------------------------------------------------
inline int qselect(int nflds, fd_set *rds, fd_set *wds, fd_set *eds, struct timeval *timeout)
{
  SIG_SAFE_CALL(::select(nflds, rds, wds, eds, timeout));
}

//---------------------------------------------------------------------------
// Prevent using of the socket functions directly
// (compiler diagnostics: call of overloaded ... is ambiguous)
namespace DONT_USE_FUNCS
{
  inline ssize_t sendto(int, const SOCKBUF_T, size_t, int, const struct sockaddr *, SOCKLEN_T) { return 0; }
  inline ssize_t recvfrom(int, SOCKBUF_T, size_t, int, struct sockaddr *, SOCKLEN_T *) { return 0; }
  inline ssize_t send(int, const SOCKBUF_T, size_t, int) { return 0; }
  inline ssize_t recv(int, SOCKBUF_T, size_t, int)       { return 0; }
  inline int select(int, fd_set *, fd_set *, fd_set *, struct timeval *) { return 0; }
}
using namespace DONT_USE_FUNCS;

#undef SIG_SAFE_CALL
#undef SOCKLEN_T
#undef SOCKBUF_T

#endif // __PRONET_H__
