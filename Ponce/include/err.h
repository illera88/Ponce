/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2015 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 */

#ifndef _ERR_H
#define _ERR_H

#ifndef UNDER_CE
#include <errno.h>
#endif

#pragma pack(push, 1)

/*! \file err.h

  \brief Thread safe functions that deal with error codes

*/

/// Print error message to stderr (analog of perror)

idaman THREAD_SAFE AS_PRINTF(1, 0) void ida_export vqperror(const char *format, va_list va);


/// Get error description string.
/// if _qerrno=-1, get_qerrno() will be used

idaman THREAD_SAFE char *ida_export qstrerror(error_t _qerrno, char *buf, size_t bufsize);


/// A convenience function to generate error messages (returns "header: error message")

idaman THREAD_SAFE char *ida_export get_errdesc(const char *header, error_t _qerrno=-1);


/// Get error message for MS Windows error codes

idaman THREAD_SAFE char *ida_export winerr(int code);


/// errno or GetLastError() depending on the system.
/// on Windows CE there is no 'errno', so we use GetLastError()

idaman THREAD_SAFE int ida_export qerrcode(int new_code=-1);


/// Get error string corresponding to qerrcode().
/// if code == -1, then qerrcode() will be called.

idaman const char *ida_export qerrstr(int code=-1);


#ifdef __cplusplus

/// See vqperror()

THREAD_SAFE AS_PRINTF(1, 2) inline void qperror(const char *format, ...)
{
  va_list va;
  va_start(va, format);
  vqperror(format, va);
  va_end(va);
}


/// See set_qerrno()

THREAD_SAFE inline void set_errno(int code)
{
#ifdef UNDER_CE
  set_qerrno(code);
#else
  errno = code;
  set_qerrno(eOS);
#endif
}
#endif

#if defined(UNDER_CE) && (!defined(EBADF) || !defined(ENOMEM))
const int EBADF = 1513;
const int ENOMEM = 1514;
#endif

// Internal functions
/// \cond

// n=0..3
idaman THREAD_SAFE void ida_export set_error_data(int n, size_t data);
idaman THREAD_SAFE void ida_export set_error_string(int n, const char *str);
idaman THREAD_SAFE size_t ida_export get_error_data(int n);
idaman THREAD_SAFE const char *ida_export get_error_string(int n);

#define QPRM_TYPE(t,n,x)        set_error_data(n-1, t(x))
#define QPRM_CHAR(n,x)          QPRM_TYPE(char,n,x)
#define QPRM_SHORT(n,x)         QPRM_TYPE(short,n,x)
#define QPRM_INT(n,x)           QPRM_TYPE(int,n,x)
#define QPRM_INT32(n,x)         QPRM_TYPE(int32,n,x)
#define QPRM_UCHAR(n,x)         QPRM_TYPE(uchar,n,x)
#define QPRM_USHORT(n,x)        QPRM_TYPE(ushort,n,x)
#define QPRM_UINT(n,x)          QPRM_TYPE(uint,n,x)
#define QPRM_UINT32(n,x)        QPRM_TYPE(uint32,n,x)
#define QPRM(n,x)               set_error_string(n-1, x)

/// \endcond

#pragma pack(pop)
#endif

