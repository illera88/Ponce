/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2015 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 */

#ifndef _HELP_H
#define _HELP_H
#pragma pack(push, 1)

typedef int help_t;     /* Help messages are referred by ints         */

// Get pointer to message text by its message id
// The message texts are read from ida.hlp at the beginning
// Returns: pointer to message text (NULL is never returned by IDA)

idaman THREAD_SAFE const char *ida_export itext(help_t msg_id);

//------------------------------------------------------------------------
#ifdef __KERNWIN_HPP
GCC_DIAG_OFF(format-nonliteral);
NORETURN inline void Err(help_t format, ...)
{
  va_list va;
  va_start(va, format);
  verror(itext(format), va);
  // NOTREACHED
}

inline void Warn(help_t format, ...)
{
  va_list va;
  va_start(va, format);
  vwarning(itext(format), va);
  va_end(va);
}

inline void Info(help_t format, ...)
{
  va_list va;
  va_start(va, format);
  vinfo(itext(format), va);
  va_end(va);
}

inline int Message(help_t format,...)
{
  va_list va;
  va_start(va, format);
  int nbytes = vmsg(itext(format), va);
  va_end(va);
  return nbytes;
}

inline int askyn_v(int deflt, help_t format, va_list va)
{
  return askyn_cv(deflt, itext(format), va);
}

inline int askyn(int deflt, help_t format, ...)
{
  va_list va;
  va_start(va, format);
  int code = askyn_cv(deflt, itext(format), va);
  va_end(va);
  return code;
}
GCC_DIAG_ON(format-nonliteral);
#endif

#ifndef NO_OBSOLETE_FUNCS
idaman DEPRECATED THREAD_SAFE char *ida_export ivalue(help_t mes, char *buf, size_t bufsize);
idaman DEPRECATED THREAD_SAFE char *ida_export qivalue(help_t mes); // Return answer in dynamic memory

#ifdef __cplusplus
/*      This class is used for temporary message:               */
/*              the message is kept until it goes out of scope. */
/*      simply use: old_itext(n)                                */

DEPRECATED class old_itext
{
  char *ptr;
public:
  old_itext(help_t mes) { ptr = qivalue(mes); }
  ~old_itext(void)      { qfree(ptr); }
  operator char*()  { return ptr; }
};
#endif /* __cplusplus */
#endif

#pragma pack(pop)
#endif /* _HELP_H */
