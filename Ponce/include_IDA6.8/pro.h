/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2015 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 */

#ifndef _PRO_H
#define _PRO_H

/*! \file pro.h

  \brief This is the first header included in the IDA project.

  It defines the most common types, functions and data.
  Also, it tries to make system dependent definitions.

  The following preprocessor macros are used in the project
  (the list may be incomplete)

  Platform must be specified as one of:

   __OS2__     - OS/2                                        \n
   __MSDOS__   - MS DOS 32-bit extender                      \n
   __NT__      - MS Windows (all platforms)                  \n
   __LINUX__   - Linux                                       \n
   __MAC__     - MAC OS X                                    \n
   __BSD__     - FreeBSD

   UNDER_CE    - Compiling for WindowsCE

   __EA64__    - 64-bit address size (sizeof(ea_t)==8)       \n
   __X64__     - 64-bit IDA itself (sizeof(void*)==8)

   __X86__     - Intel x86 processor (default)               \n
   __PPC__     - PowerPC                                     \n
   __ARM__     - ARM
*/

/// IDA SDK v6.8
#define IDA_SDK_VERSION      680

/// x86 processor by default
#ifndef __PPC__
#define __X86__
#endif

// Linux, Mac, or BSD imply Unix
#if defined(__LINUX__) || defined(__MAC__) || defined(__BSD__)
#define __UNIX__
#endif

// Only 64-bit IDA is available on 64-bit platforms
#ifdef __X64__
#undef __EA64__
#define __EA64__
#endif

/// \def{BADMEMSIZE, Invalid memory size}
#ifdef __X64__
#define BADMEMSIZE 0xFFFFFFFFFFFFFFFF
#else
#define BADMEMSIZE 0xFFFFFFFF
#endif

/// \def{ENUM_SIZE, Compiler-independent way to specify size of enum values}
#ifndef SWIG
#if defined(__VC__)
#define ENUM_SIZE(t) : t
#else
#define ENUM_SIZE(t)
#endif

#include <stdlib.h>     /* size_t, NULL, memory */
#include <stdarg.h>
#include <stddef.h>
#include <assert.h>
#include <limits.h>
#include <ctype.h>
#include <time.h>
#include <new>
#if defined(__NT__)
#  include <malloc.h>
#endif

/// \def{WIN32_LEAN_AND_MEAN, compile faster}
#if defined(__BORLANDC__)
#  define WIN32_LEAN_AND_MEAN
#  include <io.h>
#  include <dir.h>
#  include <mem.h>
#  include <alloc.h>
#elif defined(_MSC_VER)
#  define WIN32_LEAN_AND_MEAN
#  include <string.h>
#  ifndef UNDER_CE
#    include <io.h>
#    include <direct.h>
#  endif
#  include <map>
#else
#  include <algorithm>
#  include <wchar.h>
#  include <string.h>
#  include <unistd.h>
#  include <sys/stat.h>
#  include <errno.h>
#endif
#ifdef UNDER_CE         // Many files are missing in Windows CE
void abort(void);
typedef int off_t;
#else
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif

#pragma pack(push, 4)
#define STL_SUPPORT_PRESENT

//---------------------------------------------------------------------------
/// \def{EXTERNC,       specify C linkage}
/// \def{C_INCLUDE,     helper for 'extern "C" {}' statements}
/// \def{C_INCLUDE_END, \copydoc C_INCLUDE}
/// \def{INLINE,        inline keyword for c++}
#ifdef __cplusplus
#define EXTERNC         extern "C"
#define C_INCLUDE       EXTERNC {
#define C_INCLUDE_END   }
#define INLINE          inline
#else
#define EXTERNC
#define C_INCLUDE
#define C_INCLUDE_END
#define INLINE          __inline
#endif

//---------------------------------------------------------------------------
#if !defined(__OS2__) && !defined(__MSDOS__) && !defined(__NT__) \
 && !defined(__LINUX__) && !defined(__MAC__) && !defined(__BSD__)
#error "Please define one of: __NT__, __OS2__, __MSDOS__, __LINUX__,__MAC__,__BSD__"
#endif

#endif // SWIG
//---------------------------------------------------------------------------
#ifndef MAXSTR
#define MAXSTR 1024                ///< maximum string size
#endif

#define SMAXSTR QSTRINGIZE(MAXSTR) ///< get #MAXSTR as a string

/// \def{NT_CDECL, Some NT functions require __cdecl calling convention}
#ifdef __NT__
#define NT_CDECL __cdecl
#else
#define NT_CDECL
#endif

/// \def{DEPRECATED, identifies parts of the IDA API that are considered deprecated}
/// \def{NORETURN,   function does not return}
/// \def{PACKED,     type is packed}
/// \def{AS_PRINTF,  function accepts printf-style format and args}
/// \def{AS_SCANF,   function accepts scanf-style format and args}
#if defined(SWIG)
#define DEPRECATED
#define NORETURN
#define PACKED
#define AS_PRINTF(format_idx, varg_idx)
#define AS_SCANF(format_idx, varg_idx)
#elif defined(__GNUC__)
#define DEPRECATED __attribute__((deprecated))
#define NORETURN  __attribute__((noreturn))
#define PACKED __attribute__((__packed__))
#define AS_PRINTF(format_idx, varg_idx) __attribute__((format(printf, format_idx, varg_idx)))
#define AS_SCANF(format_idx, varg_idx)  __attribute__((format(scanf, format_idx, varg_idx)))
#else
#define DEPRECATED __declspec(deprecated)
#define NORETURN  __declspec(noreturn)
#define PACKED
#define AS_PRINTF(format_idx, varg_idx)
#define AS_SCANF(format_idx, varg_idx)
#endif

/// \def{GCC_DIAG_OFF, disable a specific GCC warning for the following code}
/// \def{GCC_DIAG_ON,  enable or restore a specific GCC warning for the following code}
#if defined(__GNUC__) && !defined(SWIG) && ((__GNUC__ * 100) + __GNUC_MINOR__) >= 402
#define GCC_DIAG_JOINSTR(x,y) _QSTRINGIZE(x ## y)
#  define GCC_DIAG_DO_PRAGMA(x) _Pragma (#x)
#  define GCC_DIAG_PRAGMA(x) GCC_DIAG_DO_PRAGMA(GCC diagnostic x)
#  if ((__GNUC__ * 100) + __GNUC_MINOR__) >= 406
#    define GCC_DIAG_OFF(x) GCC_DIAG_PRAGMA(push) \
                            GCC_DIAG_PRAGMA(ignored GCC_DIAG_JOINSTR(-W,x))
#    define GCC_DIAG_ON(x)  GCC_DIAG_PRAGMA(pop)
#  else
#    define GCC_DIAG_OFF(x) GCC_DIAG_PRAGMA(ignored GCC_DIAG_JOINSTR(-W,x))
#    define GCC_DIAG_ON(x)  GCC_DIAG_PRAGMA(warning GCC_DIAG_JOINSTR(-W,x))
#  endif
#else
#  define GCC_DIAG_OFF(x)
#  define GCC_DIAG_ON(x)
#endif

#if defined(DONT_DEPRECATE) || defined(__BORLANDC__)
#undef DEPRECATED
#define DEPRECATED
#endif

//---------------------------------------------------------------------------

#define __MF__  0             ///< byte sex of our platform (Most significant byte First).
                              ///<   0: little endian (Intel 80x86).
                              ///<   1: big endian (PowerPC).

//---------------------------------------------------------------------------
/// Macro to avoid of message 'Parameter x is never used'
#define qnotused(x)   (void)x

/// \def{va_argi, GNU C complains about some data types in va_arg because they are promoted to int and proposes to replace them by int}
#ifdef __GNUC__
#define va_argi(va, type)  ((type)va_arg(va, int))
#else
#define va_argi(va, type)  va_arg(va, type)
#endif

//---------------------------------------------------------------------------
#define CONST_CAST(x)   const_cast<x>   ///< cast a const to non-const
#define _QSTRINGIZE(x)  #x              ///< return x as a string. See #SMAXSTR for example
#define QSTRINGIZE(x)   _QSTRINGIZE(x)  ///< see #_QSTRINGIZE

//---------------------------------------------------------------------------

/// \def{idaapi,          specifies __stdcall calling convention}
/// \def{ida_export,      functions marked with this keyword are available as part of the IDA SDK}
/// \def{idaman,          specifies c linkage}
/// \def{ida_export_data, data items marked with this keyword are available as part of the IDA SDK}
/// \def{ida_module_data, identifies a data item that will be exported}
/// \def{ida_local,       identifies a non-public type definition}
#if defined(SWIG)                       // for SWIG
  #define idaapi
  #define idaman
  #define ida_export
  #define ida_export_data
  #define ida_module_data
  #define __fastcall
  #define ida_local
#elif defined(__NT__)                   // MS Windows
  #define idaapi            __stdcall
  #define ida_export        idaapi
  #ifdef _lint
    // tell lint that this function will be exported
    #define idaman EXTERNC __declspec(dllexport)
  #else
    #define idaman EXTERNC
  #endif
  #if defined(__IDP__)                  // modules
    #define ida_export_data __declspec(dllimport)
    #define ida_module_data __declspec(dllexport)
  #else                                 // kernel
    #define ida_export_data
    #define ida_module_data
  #endif
  #define ida_local
#elif defined(__UNIX__)                 // for unix
  #define idaapi
  #if defined(__MAC__)
    #define idaman            EXTERNC __attribute__((visibility("default")))
    #define ida_local         __attribute__((visibility("hidden")))
  #else
    #if __GNUC__ >= 4
      #define idaman          EXTERNC __attribute__ ((visibility("default")))
      #define ida_local       __attribute__((visibility("hidden")))
    #else
      #define idaman          EXTERNC
      #define ida_local
    #endif
  #endif
  #define ida_export
  #define ida_export_data
  #define ida_module_data
  #define __fastcall
#endif

/// Functions callable from any thread are marked with this keyword
#define THREAD_SAFE

//---------------------------------------------------------------------------
#ifndef __cplusplus
typedef int bool;
#define false 0
#define true 1
#endif

//---------------------------------------------------------------------------
// Linux C mode compiler already has these types defined
#if !defined(__LINUX__) || defined(__cplusplus)
typedef unsigned char  uchar;   ///< unsigned 8 bit value
typedef unsigned short ushort;  ///< unsigned 16 bit value
typedef unsigned int   uint;    ///< unsigned 32 bit value
#endif

typedef          char   int8;   ///< signed 8 bit value
typedef signed   char   sint8;  ///< signed 8 bit value
typedef unsigned char   uint8;  ///< unsigned 8 bit value
typedef          short  int16;  ///< signed 16 bit value
typedef unsigned short  uint16; ///< unsigned 16 bit value
typedef          int    int32;  ///< signed 32 bit value
typedef unsigned int    uint32; ///< unsigned 32 bit value

#include <llong.hpp>

typedef longlong        int64;  ///< signed 64 bit value
typedef ulonglong       uint64; ///< unsigned 64 bit value

/// \fn{int64 qatoll(const char *nptr), Convert string to 64 bit integer}
#if defined(__UNIX__)
inline int64 qatoll(const char *nptr) { return atoll(nptr); }
#elif defined(_MSC_VER)
inline int64 qatoll(const char *nptr) { return _atoi64(nptr); }
#else
inline int64 qatoll(const char *nptr) { return atol(nptr); }
#endif

/// \typedef{wchar16_t, 2-byte char}
/// \typedef{wchar32_t, 4-byte char}
#if defined(__BORLANDC__) || defined(_MSC_VER)
typedef wchar_t         wchar16_t;
typedef uint32          wchar32_t;
#elif defined(__GNUC__)
typedef uint16          wchar16_t;
typedef uint32          wchar32_t;
#endif

/// Signed size_t - used to check for size overflows when the counter becomes
/// negative. Also signed size_t allows us to signal an error condition using
/// a negative value, for example, as a function return value.
#if !defined(_SSIZE_T_DEFINED) && !defined(__ssize_t_defined) && !defined(__GNUC__)
typedef ptrdiff_t ssize_t;
#endif

/// \def{FMT_64, compiler-specific printf format specifier for 64-bit numbers}
/// \def{FMT_Z,  compiler-specific printf format specifier for size_t}
/// \def{FMT_ZS, compiler-specific printf format specifier for ssize_t}
#if defined(__GNUC__) && !defined(__MINGW32__)
  #define FMT_64 "ll"
  #define FMT_Z  "zu"
  #define FMT_ZS "zd"
#elif defined(_MSC_VER) || defined(__MINGW32__)
  #define FMT_64 "I64"
  #ifdef __X64__
    #define FMT_Z  "I64u"
    #define FMT_ZS "I64d"
  #else
    #define FMT_Z  "u"
    #define FMT_ZS "d"
  #endif
#elif defined(__BORLANDC__)
  #define FMT_64 "L"
  #define FMT_Z  "u"
  #define FMT_ZS "d"
#elif !defined(SWIG)
  #error "unknown compiler"
#endif

/// \typedef{ea_t,    effective address}
/// \typedef{sel_t,   segment selector}
/// \typedef{asize_t, memory chunk size}
/// \typedef{adiff_t, address difference}
/// \def{SVAL_MIN, minimum value for an object of type int}
/// \def{SVAL_MAX, maximum value for an object of type int}
/// \def{FMT_EA,   format specifier for ::ea_t values}
#ifdef __EA64__
  typedef uint64 ea_t;
  typedef uint64 sel_t;
  typedef uint64 asize_t;
  typedef int64 adiff_t;
  #define FMT_EA FMT_64
  #ifdef __GNUC__
    #define SVAL_MIN LLONG_MIN
    #define SVAL_MAX LLONG_MAX
  #else
    #define SVAL_MIN _I64_MIN
    #define SVAL_MAX _I64_MAX
  #endif
#else
  typedef uint32 ea_t;
  typedef uint32 sel_t;
  typedef uint32 asize_t;
  typedef int32 adiff_t;
  #define SVAL_MIN INT_MIN
  #define SVAL_MAX INT_MAX
  #define FMT_EA ""
#endif

typedef asize_t uval_t;   ///< unsigned value used by the processor.
                          ///<  - for 32-bit ::ea_t - ::uint32
                          ///<  - for 64-bit ::ea_t - ::uint64
typedef adiff_t sval_t;   ///< signed value used by the processor.
                          ///<  - for 32-bit ::ea_t - ::int32
                          ///<  - for 64-bit ::ea_t - ::int64
#ifndef SWIG
#define BADADDR ea_t(-1)  ///< this value is used for 'bad address'
#define BADSEL  sel_t(-1) ///< 'bad selector' value

//-------------------------------------------------------------------------
// Time related functions

typedef int32 qtime32_t;  ///< we use our own time type because time_t
                          ///< can be 32-bit or 64-bit depending on the compiler
typedef uint64 qtime64_t; ///< 64-bit time value expressed as seconds and
                          ///< microseconds since the Epoch

/// Get the 'seconds since the epoch' part of a qtime64_t

inline uint32 get_secs(qtime64_t t)
{
  return uint32(t>>32);
}


/// Get the microseconds part of a qtime64_t

inline uint32 get_usecs(qtime64_t t)
{
  return uint32(t);
}


/// Get a ::qtime64_t instance from a seconds value and microseconds value.
/// \param secs   seconds
/// \param usecs  microseconds

inline qtime64_t make_qtime64(uint32 secs, int32 usecs=0)
{
  return (qtime64_t(secs) << 32) | usecs;
}


/// Converts calendar time into a string.
/// Puts 'wrong timestamp\\n' into the buffer if failed
/// \param buf      output buffer
/// \param bufsize  size of the output buffer
/// \param t        calendar time
/// \return success

idaman THREAD_SAFE bool ida_export qctime(char *buf, size_t bufsize, qtime32_t t);


/// Converts calendar time into a string using Coordinated Universal Time (UTC).
/// Function is equivalent to asctime(gmtime(t)).
/// Puts 'wrong timestamp\\n' into the buffer if failed.
/// \param buf      output buffer
/// \param bufsize  of the output buffer
/// \param t        calendar time
/// \return success

idaman THREAD_SAFE bool ida_export qctime_utc(char *buf, size_t bufsize, qtime32_t t);


/// Converts a time value to a tm structure.
/// \param[out] _tm  result
/// \param t         local time
/// \returns success

idaman THREAD_SAFE bool ida_export qlocaltime(struct tm *_tm, qtime32_t t);


/// Same as qlocaltime(struct tm *, qtime32_t), but accepts a 64-bit time value

inline THREAD_SAFE bool qlocaltime64(struct tm *_tm, qtime64_t t)
{
  return qlocaltime(_tm, get_secs(t));
}


/// Get string representation of a qtime32_t.
/// Copies into 'buf' the content of 'format', expanding its format specifiers into the
/// corresponding values that represent the time described in 't', with a limit of 'bufsize' characters
/// see http://www.cplusplus.com/reference/ctime/strftime/ for more
/// \param buf      output buffer
/// \param bufsize  of the output buffer
/// \param format   format string
/// \param t        time value
/// \return length of the resulting string

idaman THREAD_SAFE size_t ida_export qstrftime(char *buf, size_t bufsize, const char *format, qtime32_t t);


/// Same as qstrftime(), but accepts a 64-bit time value

idaman THREAD_SAFE size_t ida_export qstrftime64(char *buf, size_t bufsize, const char *format, qtime64_t t);


/// Suspend execution for given number of milliseconds

idaman THREAD_SAFE void ida_export qsleep(int milliseconds);


/// High resolution timer.
/// On Unix systems, returns current time in nanoseconds.
/// On Windows, returns a high resolution counter (QueryPerformanceCounter)
/// \param[out] nsecs  result

idaman THREAD_SAFE void ida_export get_nsec_stamp(uint64 *nsecs);


/// Get the current time with microsecond resolution (in fact the resolution
/// is worse on windows)

idaman THREAD_SAFE qtime64_t ida_export qtime64(void);


/// Generate a random buffer.
/// \param[out] buffer  pointer to result
/// \param bufsz        size of buffer
/// \return success

idaman THREAD_SAFE bool ida_export gen_rand_buf(void *buffer, size_t bufsz);


#define qoff64_t int64        ///< file offset

/// Describes miscellaneous file attributes
struct qstatbuf64
{
  uint64    qst_dev;     ///< ID of device containing file
  uint32    qst_ino;     ///< inode number
  uint32    qst_mode;    ///< protection
  uint32    qst_nlink;   ///< number of hard links
  uint32    qst_uid;     ///< user ID of owner
  uint32    qst_gid;     ///< group ID of owner
  uint64    qst_rdev;    ///< device ID (if special file)
  qoff64_t  qst_size;    ///< total size, in bytes
  int32     qst_blksize; ///< blocksize for file system I/O
  int32     qst_blocks;  ///< number of 512B blocks allocated
  qtime64_t qst_atime;   ///< time of last access
  qtime64_t qst_mtime;   ///< time of last modification
  qtime64_t qst_ctime;   ///< time of last status change
};

// non standard functions are missing:
#ifdef _MSC_VER
#if _MSC_VER <= 1200
#define for if(0) ; else for    ///< MSVC <= 1200 is not compliant to the ANSI standard
#else
#pragma warning(disable : 4200) ///< zero-sized array in structure (non accept from cmdline)
#endif
/// \name VS posix names
/// Shut up Visual Studio (VS deprecated posix names but there seems to be no good reason for that)
//@{
#define chdir  _chdir
#define fileno _fileno
#define getcwd _getcwd
#define memicmp _memicmp
//@}
#endif

/// Is this IDA kernel? If not, we are executing a standalone application
idaman bool ida_export_data is_ida_kernel;

//---------------------------------------------------------------------------
/* error codes */
/*--------------------------------------------------*/

#define eOk           0    ///< no error
#define eOS           1    ///< os error, see errno
#define eDiskFull     2    ///< disk full
#define eReadError    3    ///< read error
#define eFileTooLarge 4    ///< file too large

/// Error code (errno)
typedef int error_t;


/// Set qerrno

idaman THREAD_SAFE error_t ida_export set_qerrno(error_t code);


/// Get qerrno

idaman THREAD_SAFE error_t ida_export get_qerrno(void);

//---------------------------------------------------------------------------
// debugging macros
/// \def{ZZZ, debug print}
/// \def{BPT, trigger a breakpoint from IDA. also see #INTERR}
#define ZZZ msg("%s:%d\n", __FILE__, __LINE__)
#if defined(__BORLANDC__)
#  define BPT __emit__(0xcc)
#  define __FUNCTION__ __FUNC__
#elif defined(__GNUC__)
#  if defined(__arm__) || defined(__aarch64__)
#    ifdef __LINUX__
#      define BPT __builtin_trap()
#    else
#      define BPT asm("trap")
#    endif
#  else
#    define BPT asm("int3")
#  endif
#elif defined(_MSC_VER) // Visual C++
#  define BPT __debugbreak()
#  ifdef _lint
     NORETURN void __debugbreak(void);
#  endif
#endif

/// \def{__CASSERT_N0__, compile time assertion}
/// \def{__CASSERT_N1__, compile time assertion}
/// \def{CASSERT,  results in a compile error if the cnd is not true}
/// \def{CASSERT0, returns 0 if cnd is true - otherwise results in a compile error}
#ifdef _lint
#define CASSERT(cnd) extern int pclint_cassert_dummy_var
#define CASSERT0(cnd) 0
#else
#define __CASSERT_N0__(l) COMPILE_TIME_ASSERT_ ## l
#define __CASSERT_N1__(l) __CASSERT_N0__(l)
#define CASSERT(cnd) typedef char __CASSERT_N1__(__LINE__) [(cnd) ? 1 : -1]
#define CASSERT0(cnd) (sizeof(char [1 - 2*!(cnd)]) - 1)
#endif

/// \def{INTERR, Show internal error message and terminate execution abnormally.
///              When IDA is being run under a debugger this will ensure that
///              the debugger will break immediately.}
#if defined(UNDER_CE) || defined(_lint)
#define INTERR(code) interr(code)
#else
#define INTERR(code) do { if ( under_debugger ) BPT; interr(code); } while(1)
#endif

#define QASSERT(code, cond) do if ( !(cond) ) INTERR(code); while (0)                 ///< run time assertion
#define QBUFCHECK(buf, size, src) ida_fill_buffer(buf, size, src, __FILE__, __LINE__) ///< run time assertion
idaman bool ida_export_data under_debugger;                                           ///< is IDA running under a debugger?
idaman THREAD_SAFE NORETURN void ida_export interr(int code);                         ///< Show internal error message and terminate execution

//---------------------------------------------------------------------------
idaman THREAD_SAFE void *ida_export qalloc(size_t size);                              ///< System independent malloc
idaman THREAD_SAFE void *ida_export qrealloc(void *alloc, size_t newsize);            ///< System independent realloc
idaman THREAD_SAFE void *ida_export qcalloc(size_t nitems, size_t itemsize);          ///< System independent calloc
idaman THREAD_SAFE void  ida_export qfree(void *alloc);                               ///< System independent free
idaman THREAD_SAFE char *ida_export qstrdup(const char *string);                      ///< System independent strdup
#define qnew(t)        ((t*)qalloc(sizeof(t)))  ///< create a new object in memory
/// \def{qnewarray, qalloc_array() is safer than qnewarray}
#ifdef NO_OBSOLETE_FUNCS
#define qnewarray(t,n)  use_qalloc_array
#else
#define qnewarray(t,n) ((t*)qcalloc((n),sizeof(t)))
#endif

/// Use this class to avoid integer overflows when allocating arrays
template <class T>
T *qalloc_array(size_t n)
{
  return (T *)qcalloc(n, sizeof(T));
}

/// Use this class to avoid integer overflows when allocating arrays
template <class T>
T *qrealloc_array(T *ptr, size_t n)
{
  size_t nbytes = n * sizeof(T);
  if ( nbytes < n )
    return NULL; // integer overflow
  return (T *)qrealloc(ptr, nbytes);
}

/// \def{qnumber, determine capacity of an array}
#if defined(__GNUC__) || defined(UNDER_CE)
#  define qnumber(arr) ( \
  0 * sizeof(reinterpret_cast<const ::qnumber_check_type *>(arr)) + \
  0 * sizeof(::qnumber_check_type::check_type((arr), &(arr))) + \
  sizeof(arr) / sizeof((arr)[0]) )
  struct qnumber_check_type
  {
      struct is_pointer;
      struct is_array {};
      template <typename T>
      static is_pointer check_type(const T *, const T * const *);
      static is_array check_type(const void *, const void *);
  };
#elif defined(_MSC_VER) && !defined(__LINT__)
#  define qnumber(array) _countof(array)
#else // poor man's implementation for other compilers and lint
#  define qnumber(array) (sizeof(array)/sizeof(array[0]))
#endif

/// \def{qoffsetof, gcc complains about offsetof() - we had to make our version}
#ifdef __GNUC__
#define qoffsetof(type, name) size_t(((char *)&((type *)1)->name)-(char*)1)
#else
#define qoffsetof offsetof
#endif

/// \def{set_vva, extracts a va_list passed as a variadic function argument}
/// \def{va_copy, copy a va_list}
#if defined(__GNUC__) && defined(__X64__)
  // gcc64 uses special array-type va_list, so we have to resort to tricks like these
  #define set_vva(va2, vp) va_copy(va2, *(va_list*)va_arg(vp, void*))
#else
  #ifndef va_copy
    #define va_copy(dst, src) dst = src
  #endif
  #if defined(__clang__)
    #define set_vva(va2, vp) va2 = va_arg(vp, va_list)
  #else
    #define set_vva(va2, vp) va_copy(va2, va_arg(vp, va_list))
  #endif
#endif


/// Reverse memory block.
/// Analog of strrev() function
/// \param buf   pointer to buffer to reverse
/// \param size  size of buffer
/// \return pointer to buffer

idaman THREAD_SAFE void *ida_export memrev(void *buf, ssize_t size);

#ifdef __GNUC__
idaman THREAD_SAFE int ida_export memicmp(const void *x, const void *y, size_t size);
#endif

//---------------------------------------------------------------------------
/* strings */
/// \def{strnicmp, see 'VS posix names'}
/// \def{stricmp,  see 'VS posix names'}
#ifdef __GNUC__
#define strnicmp strncasecmp
#define stricmp  strcasecmp
#elif defined(_MSC_VER) && !defined(_lint)
#define strnicmp _strnicmp
#define stricmp  _stricmp
#endif


/// Replace all occurrences of a character within a string.
/// \param str     to modify
/// \param char1   char to be replaced
/// \param char2   replacement char
/// \return pointer to resulting string

idaman THREAD_SAFE char *ida_export strrpl(char *str, int char1, int char2);


/// Get tail of a string
inline       char *tail(      char *str) { return strchr(str, '\0'); }
/// \copydoc tail(char *)
inline const char *tail(const char *str) { return strchr(str, '\0'); }


/// A safer strncpy - makes sure that there is a terminating zero.
/// nb: this function doesn't fill the whole buffer zeroes as strncpy does
/// nb: ssize_t(dstsize) must be > 0

idaman THREAD_SAFE char *ida_export qstrncpy(char *dst, const char *src, size_t dstsize);


/// A safer stpncpy - returns pointer to the end of the destination
/// nb: ssize_t(dstsize) must be > 0

idaman THREAD_SAFE char *ida_export qstpncpy(char *dst, const char *src, size_t dstsize);


/// A safer strncat - accepts the size of the 'dst' as 'dstsize' and returns dst
/// nb: ssize_t(dstsize) must be > 0

idaman THREAD_SAFE char *ida_export qstrncat(char *dst, const char *src, size_t dstsize);


/// Thread-safe version of strtok

idaman THREAD_SAFE char *ida_export qstrtok(char *s, const char *delim, char **save_ptr);


/// Convert the string to lowercase

idaman THREAD_SAFE char *ida_export qstrlwr(char *str);


/// Convert the string to uppercase

idaman THREAD_SAFE char *ida_export qstrupr(char *str);


/// Find one string in another (Case insensitive analog of strstr()).
/// \param s1  string to be searched
/// \param s2  string to search for
/// \return a pointer to the first occurrence of s2 within s1, NULL if none exists

idaman THREAD_SAFE const char *ida_export stristr(const char *s1, const char *s2);


/// Same as stristr(const char *, const char *) but returns a non-const result

inline char *idaapi stristr(char *s1, const char *s2) { return CONST_CAST(char *)(stristr((const char *)s1, s2)); }

/// is...() functions misbehave with 'char' argument. introduce more robust function
inline bool ida_local qisspace(char c) { return isspace(uchar(c)) != 0; }
inline bool ida_local qisalpha(char c) { return isalpha(uchar(c)) != 0; }   ///< see qisspace()
inline bool ida_local qisalnum(char c) { return isalnum(uchar(c)) != 0; }   ///< see qisspace()
inline bool ida_local qispunct(char c) { return ispunct(uchar(c)) != 0; }   ///< see qisspace()
inline bool ida_local qislower(char c) { return islower(uchar(c)) != 0; }   ///< see qisspace()
inline bool ida_local qisupper(char c) { return isupper(uchar(c)) != 0; }   ///< see qisspace()
inline bool ida_local qisprint(char c) { return isprint(uchar(c)) != 0; }   ///< see qisspace()
inline bool ida_local qisdigit(char c) { return isdigit(uchar(c)) != 0; }   ///< see qisspace()
inline bool ida_local qisxdigit(char c) { return isxdigit(uchar(c)) != 0; } ///< see qisspace()

/// Get lowercase equivalent of given char
inline char ida_local qtolower(char c) { return tolower(uchar(c)); }
/// Get uppercase equivalent of given char
inline char ida_local qtoupper(char c) { return toupper(uchar(c)); }

// We forbid using dangerous functions in IDA
#if !defined(USE_DANGEROUS_FUNCTIONS) && !defined(_lint)
#if defined(__BORLANDC__) && (__BORLANDC__ < 0x560  || __BORLANDC__ >= 0x580) // for BCB5 (YH)
#include <stdio.h>
#endif
#undef strcpy
#define strcpy          dont_use_strcpy            ///< use qstrncpy()
#define stpcpy          dont_use_stpcpy            ///< use qstpncpy()
#define strncpy         dont_use_strncpy           ///< use qstrncpy()
#define strcat          dont_use_strcat            ///< use qstrncat()
#define strncat         dont_use_strncat           ///< use qstrncat()
#define gets            dont_use_gets              ///< use qfgets()
#define sprintf         dont_use_sprintf           ///< use qsnprintf()
#define snprintf        dont_use_snprintf          ///< use qsnprintf()
#define wsprintfA       dont_use_wsprintf          ///< use qsnprintf()
#undef strcmpi
#undef strncmpi
#define strcmpi         dont_use_strcmpi           ///< use stricmp()
#define strncmpi        dont_use_strncmpi          ///< use strnicmp()
#define getenv          dont_use_getenv            ///< use qgetenv()
#define setenv          dont_use_setenv            ///< use qsetenv()
#define putenv          dont_use_putenv            ///< use qsetenv()
#define strtok          dont_use_strrok            ///< use qstrtok()
#undef strlwr
#undef strupr
#define strlwr          dont_use_strlwr            ///< use qstrlwr()
#define strupr          dont_use_strupr            ///< use qstrupr()
#define waitid          dont_use_waitid            ///< use qwait()
#define waitpid         dont_use_waitpid           ///< use qwait()
#define wait            dont_use_wait              ///< use qwait()
#endif

//---------------------------------------------------------------------------
#define streq(s1, s2)          (strcmp((s1), (s2))  == 0)           ///< convenient check for string equality
#define strieq(s1, s2)         (stricmp((s1), (s2)) == 0)           ///< see #streq
#define strneq(s1, s2, count)  (strncmp((s1), (s2), (count))  == 0) ///< see #streq
#define strnieq(s1, s2, count) (strnicmp((s1), (s2), (count)) == 0) ///< see #streq

//---------------------------------------------------------------------------
/// \defgroup qsnprintf qsnprintf/qsscanf
/// safer versions of sprintf/sscanf
///
/// Our definitions of sprintf-like functions support one additional format specifier
///
///      "%a"              which corresponds to ::ea_t
///
/// Usual optional fields like the width can be used too: %04a.
/// The width specifier will be doubled for 64-bit version.
/// These function return the number of characters _actually written_ to the output string.
/// excluding the terminating zero. (which is different from the snprintf).
/// They always terminate the output with a zero byte (if n > 0).
//@{
idaman AS_PRINTF(3, 4) THREAD_SAFE int ida_export qsnprintf(char *buffer, size_t n, const char *format, ...);           ///< A safer snprintf
idaman AS_SCANF (2, 3) THREAD_SAFE int ida_export qsscanf(const char *input, const char *format, ...);                  ///< A safer sscanf
idaman AS_PRINTF(3, 0) THREAD_SAFE int ida_export qvsnprintf(char *buffer, size_t n, const char *format, va_list va);   ///< See qsnprintf()
idaman AS_SCANF (2, 0) THREAD_SAFE int ida_export qvsscanf(const char *input, const char *format, va_list va);          ///< See qsscanf()
idaman AS_PRINTF(3, 4) THREAD_SAFE int ida_export append_snprintf(char *buf, const char *end, const char *format, ...); ///< Append result of sprintf to 'buf'
//@}

//---------------------------------------------------------------------------
/// qsnprintf that does not check its arguments.
/// Normally gcc complains about the non-literal formats. However, sometimes we
/// still need to call qsnprintf with a dynamically built format string.
/// OTOH, there are absolutely no checks of the input arguments, so be careful!
GCC_DIAG_OFF(format-nonliteral);
inline int nowarn_qsnprintf(char *buf, size_t size, const char *format, ...)
{
  va_list va;
  va_start(va, format);
  int code = ::qvsnprintf(buf, size, format, va);
  va_end(va);
  return code;
}
GCC_DIAG_ON(format-nonliteral);

//---------------------------------------------------------------------------
/// \def{QMAXPATH, maximum number of characters in a path specification}
/// \def{QMAXFILE, maximum number of characters in a filename specification}
#if defined(__NT__)
#define QMAXPATH        260
#define QMAXFILE        260
#else
#define QMAXPATH        PATH_MAX
#define QMAXFILE        PATH_MAX
#endif

idaman THREAD_SAFE char *ida_export vqmakepath(char *buf, size_t bufsize, const char *s1, va_list); ///< See qmakepath()


/// Construct a path from a null-terminated sequence of strings.
/// \param buf      output buffer. Can be == s1, but must not be NULL
/// \param bufsize  size of buffer
/// \return pointer to result

idaman THREAD_SAFE char *ida_export qmakepath(char *buf, size_t bufsize, const char *s1, ...);


/// Get the current working directory.
/// \param buf      output buffer
/// \param bufsize  size of buffer
/// This function calls error() if any problem occurs.

idaman void ida_export qgetcwd(char *buf, size_t bufsize);


/// Get the directory part of the path.
/// path and buf may point to the same buffer
/// \param[out] buf      buffer for the directory part. can be NULL.
/// \param[out] bufsize  size of this buffer
/// \param path          path to split
/// \retval true   ok
/// \retval false  input buffer did not have the directory part.
///                In this case the buffer is filled with "."

idaman THREAD_SAFE bool ida_export qdirname(char *buf, size_t bufsize, const char *path);


/// Construct filename from base name and extension.
/// \param buf      output buffer. Can be == base, but must not be NULL
/// \param bufsize  size of buffer
/// \param base     base name
/// \param ext      extension
/// \return pointer to result

idaman THREAD_SAFE char *ida_export qmakefile(
        char *buf,
        size_t bufsize,
        const char *base,
        const char *ext);


/// Split filename into base name and extension.
/// \param file  filename, may be changed
/// \param base  filled with base part, can be NULL
/// \param ext   filled with extension part, can be NULL
/// \return the base part

idaman THREAD_SAFE char *ida_export qsplitfile(char *file, char **base, char **ext);


/// Is the file name absolute (not relative to the current dir?)

idaman THREAD_SAFE bool ida_export qisabspath(const char *file);


/// Get the file name part of the given path.
/// \return NULL if path is NULL

idaman THREAD_SAFE const char *ida_export qbasename(const char *path);

#ifdef __cplusplus
/// Same as qbasename(const char *), but accepts and returns non-const char pointers
inline char *qbasename(char *path) { return CONST_CAST(char *)(qbasename((const char *)path)); }
#endif


/// Convert relative path to absolute path

idaman THREAD_SAFE char *ida_export qmake_full_path(char *dst, size_t dstsize, const char *src);


/// Search for a file in the PATH environment variable or the current directory.
/// \param file        the file name to look for. If the file is an absolute path
///                    then buf will return the file value.
/// \param buf         output buffer to hold the full file path
/// \param bufsize     output buffer size
/// \param search_cwd  search the current directory if file was not found in the PATH
/// \return true if the file was found and false otherwise

idaman THREAD_SAFE bool ida_export search_path(
        const char *file,
        char *buf,
        size_t bufsize,
        bool search_cwd);

/// Delimiter of directory lists
#if defined(__UNIX__)
#define DELIMITER       ":"     ///< for Unix - ';' for Windows
#else
#define DELIMITER       ";"     ///< for MS DOS, Windows, other systems - ':' for Unix
#endif


/// Set file name extension unconditionally.
/// \param outbuf   buffer to hold the answer. may be the same
///                 as the file name.
/// \param bufsize  output buffer size
/// \param file     the file name
/// \param ext      new extension (with or without '.')
/// \return pointer to the new file name

idaman THREAD_SAFE char *ida_export set_file_ext(
        char *outbuf,
        size_t bufsize,
        const char *file,
        const char *ext);


/// Get pointer to extension of file name.
/// \param file  filename
/// \return pointer to the file extension or NULL if extension doesn't exist

idaman THREAD_SAFE const char *ida_export get_file_ext(const char *file);

/// Does the given file name have an extension?
#ifdef __cplusplus
inline bool idaapi has_file_ext(const char *file)
  { return get_file_ext(file) != NULL; }
#endif


/// Set file name extension if none exists.
/// This function appends the extension to a file name.
/// It won't change file name if extension already exists
/// \param buf      output buffer
/// \param bufsize  size of the output buffer
/// \param file     file name
/// \param ext      extension (with or without '.')
/// \return pointer to the new file name

#ifdef __cplusplus
inline char *idaapi make_file_ext(
        char *buf,
        size_t bufsize,
        const char *file,
        const char *ext)
{
  if ( has_file_ext(file) )
    return ::qstrncpy(buf, file, bufsize);
  else
    return set_file_ext(buf, bufsize, file, ext);
}
#endif


/// Sanitize the file name.
/// Remove the directory path, and replace wildcards ? * and chars<' ' with _.
/// If the file name is empty, then:
///      - namesize != 0: generate a new temporary name, return true
///      - namesize == 0: return false

idaman THREAD_SAFE bool ida_export sanitize_file_name(char *name, size_t namesize);

//---------------------------------------------------------------------------
/* input/output */
/*--------------------------------------------------*/
#if !defined(__MSDOS__) && !defined(__OS2__) && !defined(__NT__) && !defined(_MSC_VER)
#define O_BINARY        0
#endif

#ifndef SEEK_SET
#define SEEK_SET        0   ///< beginning of file
#define SEEK_CUR        1   ///< current position of the file *
#define SEEK_END        2   ///< end of file *
#endif

/*--------------------------------------------------*/
/*   you should use these functions for file i/o    */


/// Works the same as it's counterpart from Clib.
/// The only difference is that it sets 'qerrno' variable too

idaman THREAD_SAFE int   ida_export qopen(const char *file, int mode);


/// Open file with given sharing_mode (use O_RDONLY, O_WRONLY, O_RDWR flags), sets qerrno

idaman THREAD_SAFE int   ida_export qopen_shared(const char *file, int mode, int share_mode);


/// Create new file with O_RDWR, sets qerrno

idaman THREAD_SAFE int   ida_export qcreate(const char *file, int stat);

idaman THREAD_SAFE int   ida_export qread(int h, void *buf, size_t n);                        ///< \copydoc qopen
idaman THREAD_SAFE int   ida_export qwrite(int h, const void *buf, size_t n);                 ///< \copydoc qopen
idaman THREAD_SAFE int32 ida_export qtell(int h);                                             ///< \copydoc qopen
idaman THREAD_SAFE int32 ida_export qseek(int h, int32 offset, int whence);                   ///< \copydoc qopen
idaman THREAD_SAFE int   ida_export qclose(int h);                                            ///< \copydoc qopen
idaman THREAD_SAFE int   ida_export qdup(int h);                                              ///< \copydoc qopen
idaman THREAD_SAFE int   ida_export qfsync(int h);                                            ///< \copydoc qopen


/// Get the file size.
/// This function may return 0 if the file is not found or if the file is too large (>4GB).
/// Call get_qerrno() and compare against eFileTooLarge to tell between the two cases.

idaman THREAD_SAFE uint32 ida_export qfilesize(const char *fname);

/// Get file length in bytes.
/// \param h  file descriptor
/// \return file length in bytes, -1 if error

idaman THREAD_SAFE uint32 ida_export qfilelength(int h);

/// Change file size.
/// \param h      file descriptor
/// \param fsize  desired size
/// \retval 0     on success
/// \retval -1    otherwise and qerrno is set

idaman THREAD_SAFE int   ida_export qchsize(int h, uint32 fsize);

/// Create an empty directory.
/// \param file  name (or full path) of directory to be created
/// \param mode  permissions (only used on unix systems)
/// \return 0    success
/// \return -1   otherwise and qerrno is set

idaman THREAD_SAFE int   ida_export qmkdir(const char *file, int mode);


/// Does the given file exist?

idaman THREAD_SAFE bool  ida_export qfileexist(const char *file);


/// Does the given path specify a directory?

idaman THREAD_SAFE bool  ida_export qisdir(const char *file);

/*--------------------------------------------------*/
idaman THREAD_SAFE qoff64_t ida_export qtell64(int h);                                ///< Same as qtell(), but with large file (>2Gb) support
idaman THREAD_SAFE qoff64_t ida_export qseek64(int h, qoff64_t offset, int whence);   ///< Same as qseek(), but with large file (>2Gb) support
idaman THREAD_SAFE uint64   ida_export qfilesize64(const char *fname);                ///< Same as qfilesize(), but with large file (>2Gb) support
idaman THREAD_SAFE uint64   ida_export qfilelength64(int h);                          ///< Same as qfilelength(), but with large file (>2Gb) support
idaman THREAD_SAFE int      ida_export qchsize64(int h, uint64 fsize);                ///< Same as qchsize(), but with large file (>2Gb) support
idaman THREAD_SAFE bool     ida_export qfileexist64(const char *file);                ///< Same as qfileexist(), but with large file (>2Gb) support
idaman THREAD_SAFE int      ida_export qstat64(const char *path, qstatbuf64 *buf);    ///< Same as qstat(), but with large file (>2Gb) support
idaman THREAD_SAFE int      ida_export qfstat64(int h, qstatbuf64 *buf);              ///< Same as qfstat(), but with large file (>2Gb) support

//---------------------------------------------------------------------------
/// Add a function to be called at exit time

idaman THREAD_SAFE void ida_export qatexit(void (idaapi *func)(void));


/// Remove a previously added exit-time function

idaman THREAD_SAFE void ida_export del_qatexit(void (idaapi*func)(void));

#endif // SWIG


/// Call qatexit functions, shut down UI and kernel, and exit.
/// \param code  exit code

idaman THREAD_SAFE NORETURN void ida_export qexit(int code);

//---------------------------------------------------------------------------
#define qmin(a,b) ((a) < (b)? (a): (b)) ///< universal min
#define qmax(a,b) ((a) > (b)? (a): (b)) ///< universal max
#if defined(__EA64__) && defined(__VC__) && defined(__cplusplus)
#if _MSC_VER < 1600
static inline int64 abs(int64 n) { return _abs64(n); }
#endif
static inline int32 abs(uint32 n) { return abs((int32)n); }
#endif

//----------------------------------------------------------------------
/// Test if 'bit' is set in 'bitmap'
inline bool idaapi test_bit(const uchar *bitmap, size_t bit)
{
  return (bitmap[bit/8] & (1<<(bit&7))) != 0;
}
/// Set 'bit' in 'bitmap'
inline void idaapi set_bit(uchar *bitmap, size_t bit)
{
  uchar *p = bitmap + bit/8;
  *p = uchar(*p | (1<<(bit&7)));
}
/// Clear 'bit' in 'bitmap'
inline void idaapi clear_bit(uchar *bitmap, size_t bit)
{
  uchar *p = bitmap + bit/8;
  *p = uchar(*p & ~(1<<(bit&7)));
}
/// Set first 'nbits' of 'bitmap'
inline void idaapi set_all_bits(uchar *bitmap, size_t nbits)
{
  memset(bitmap, 0xFF, (nbits+7)/8);
  if ( (nbits & 7) != 0 )
  {
    uchar *p = bitmap + nbits/8;
    *p = uchar(*p & ~((1 << (nbits&7))-1));
  }
}
/// Clear first 'nbits' of 'bitmap'
inline void idaapi clear_all_bits(uchar *bitmap, size_t nbits)
{
  memset(bitmap, 0, (nbits+7)/8);
}

//----------------------------------------------------------------------
/// Functions to work with intervals
namespace interval
{
  /// Do (off1,s1) and (off2,s2) overlap?
  inline bool overlap(uval_t off1, asize_t s1, uval_t off2, asize_t s2)
  {
    return off2 < off1+s1 && off1 < off2+s2;
  }
  /// Does (off1,s1) include (off2,s2)?
  inline bool includes(uval_t off1, asize_t s1, uval_t off2, asize_t s2)
  {
    return off2 >= off1 && off2+s2 <= off1+s1;
  }
  /// Does (off1,s1) contain off?
  inline bool contains(uval_t off1, asize_t s1, uval_t off)
  {
    return off >= off1 && off < off1+s1;
  }
}

//----------------------------------------------------------------------
#ifdef __cplusplus
/// Shift by the amount exceeding the operand size*8 is undefined by the standard.
/// Indeed, GNUC may decide not to rotate the operand in some cases.
/// We have to check this manually.
template <class T> T left_shift(const T &value, int shift)
{
  return shift >= sizeof(T)*8 ? 0 : (value << shift);
}
/// \copydoc left_shift
template <class T> T right_ushift(const T &value, int shift)
{
  return shift >= sizeof(T)*8 ? 0 : (value >> shift);
}
/// \copydoc left_shift
template <class T> T right_sshift(const T &value, int shift)
{
  return shift >= sizeof(T)*8 ? (value >= 0 ? 0 : -1) : (value >> shift);
}

/// Rotate left
template<class T> T qrotl(T value, size_t count)
{
  const size_t nbits = sizeof(T) * 8;
  count %= nbits;

  T high = value >> (nbits - count);
  value <<= count;
  value |= high;
  return value;
}

/// Rotate right
template<class T> T qrotr(T value, size_t count)
{
  const size_t nbits = sizeof(T) * 8;
  count %= nbits;

  T low = value << (nbits - count);
  value >>= count;
  value |= low;
  return value;
}

/// Make a mask of 'count' bits
template <class T> T make_mask(int count)
{
  return left_shift<T>(1, count) - 1;
}

/// Set a 'bit' in 'where' if 'value' if not zero
template<class T, class U> void idaapi setflag(T &where, U bit, bool cnd)
{
   if ( cnd )
     where = T(where | bit);
   else
     where = T(where & ~T(bit));
}

/// Check that unsigned multiplication does not overflow
template<class T> bool is_mul_ok(T count, T elsize)
{
  CASSERT((T)(-1) > 0); // make sure T is unsigned
  if ( elsize  == 0 || count == 0 )
    return true;
  return count <= ((T)(-1)) / elsize;
}

/// Check that unsigned addition does not overflow
template<class T> bool is_add_ok(T x, T y)
{
  CASSERT((T)(-1) > 0); // make sure T is unsigned
  return y <= ((T)(-1)) - x;
}

/// \def{OPERATOR_NEW, GCC does not check for an integer overflow in 'operator new[]'. We have to do it
///                    ourselves. Please note that 'char' arrays can still be allocated with
///                    plain 'operator new'.}
#ifdef __GNUC__
#  define OPERATOR_NEW(type, count) (is_mul_ok(size_t(count), sizeof(type)) \
                                     ? new type[count] \
                                     : (type *)qalloc_or_throw(BADMEMSIZE))
#else
#  define OPERATOR_NEW(type, count) new type[count]
#endif

#endif // __cplusplus

//-------------------------------------------------------------------------
/// Sign-, or zero-extend the value 'v' to occupy 64 bits.
/// The value 'v' is considered to be of size 'nbytes'.

idaman uint64 ida_export extend_sign(uint64 v, int nbytes, bool sign_extend);


/// We can not use multi-character constants because they are not portable - use this macro instead
#define MC2(c1, c2)          ushort(((c2)<<8)|c1)
#define MC3(c1, c2, c3)      uint32(((((c3)<<8)|(c2))<<8)|c1)              ///< \copydoc MC2
#define MC4(c1, c2, c3, c4)  uint32(((((((c4)<<8)|(c3))<<8)|(c2))<<8)|c1)  ///< \copydoc MC2


//---------------------------------------------------------------------------
/// Read at most 4 bytes from file.
/// \param h     file handle
/// \param res   value read from file
/// \param size  size of value in bytes (1,2,4)
/// \param mf    is MSB first?
/// \return 0 on success, nonzero otherwise

idaman THREAD_SAFE int ida_export readbytes(int h, uint32 *res, int size, bool mf);


/// Write at most 4 bytes to file.
/// \param h     file handle
/// \param l     value to write
/// \param size  size of value in bytes (1,2,4)
/// \param mf    is MSB first?
/// \return 0 on success, nonzero otherwise

idaman THREAD_SAFE int ida_export writebytes(int h, uint32 l, int size, bool mf);


/// Read a 2 byte entity from a file.
/// \param h    file handle
/// \param res  value read from file
/// \param mf   is MSB first?
/// \return 0 on success, nonzero otherwise

idaman THREAD_SAFE int ida_export read2bytes(int h, uint16 *res, bool mf);

#define read4bytes(h, res, mf)  readbytes(h, res, 4, mf) ///< see readbytes()
#define write2bytes(h, l, mf)   writebytes(h, l, 2, mf)  ///< see writebytes()
#define write4bytes(h, l, mf)   writebytes(h, l, 4, mf)  ///< see writebytes()

//---------------------------------------------------------------------------
/// \fn{uint32 swap32(uint32 x), Switch endianness of given value}
/// \fn{ushort swap16(ushort x), \copydoc swap32}
/// \def{swap32, Switch endianness of given value}
/// \def{swap16, \copydoc swap32}
#ifdef __cplusplus
#  ifndef swap32
inline uint32 swap32(uint32 x)
  { return (x>>24) | (x<<24) | ((x>>8) & 0x0000FF00L) | ((x<<8) & 0x00FF0000L); }
#  endif
#  ifndef swap16
inline ushort swap16(ushort x)
  { return ushort((x<<8) | (x>>8)); }
#  endif
#else
#  ifndef swap32
#    define swap32(x) uint32((x>>24) | (x<<24) | ((x>>8) & 0x0000FF00L) | ((x<<8) & 0x00FF0000L))
#  endif
#  ifndef swap16
#    define swap16(x) ushort((x<<8) | (x>>8))
#  endif
#endif

/// \def{swapea, Switch endianness of an ::ea_t value}
#ifdef __EA64__
#define swapea  swap64
#else
#define swapea  swap32
#endif

/// \def{qhtons, \copydoc swap32}
/// \def{qntohs, \copydoc swap32}
/// \def{qhtonl, \copydoc swap32}
/// \def{qntohl, \copydoc swap32}
#if __MF__
#define qhtonl(x) (x)
#define qntohl(x) (x)
#define qhtons(x) (x)
#define qntohs(x) (x)
#else
#define qhtons(x) swap16(x)
#define qntohs(x) swap16(x)
#define qhtonl(x) swap32(x)
#define qntohl(x) swap32(x)
#endif


/// Swap endianness of a given value in memory.
/// \param dst   result of swap
/// \param src   value to be swapped
/// \param size  size of value: can be 1, 2, 4, 8, or 16.
///              For any other values of size this function does nothing

idaman THREAD_SAFE void ida_export swap_value(void *dst, const void *src, int size);


idaman THREAD_SAFE void ida_export reloc_value(void *value, int size, adiff_t delta, bool mf);


/// Rotate left - can be used to rotate a value to the right if the count is negative.
/// \param x       value to rotate
/// \param count   shift amount
/// \param bits    number of bits to rotate (32 will rotate a dword)
/// \param offset  number of first bit to rotate.
///                (bits=8 offset=16 will rotate the third byte of the value)
/// \return the rotated value

idaman THREAD_SAFE uval_t ida_export rotate_left(uval_t x, int count, size_t bits, size_t offset);


#ifdef __cplusplus
/// Swap 2 objects of the same type using memory copies
template <class T> inline void qswap(T &a, T &b)
{
  char temp[sizeof(T)];
  memcpy(&temp, &a, sizeof(T));
  memcpy(&a, &b, sizeof(T));
  memcpy(&b, &temp, sizeof(T));
}
#endif

/// \name Safe buffer append
/// In the following macros, 'buf' must be always less than 'end'.
/// When we run up to the end, we put a 0 there and don't increase buf anymore
//@{
/// Append a character to the buffer checking the buffer size
#define APPCHAR(buf, end, chr)                    \
  do                                              \
  {                                               \
    char __chr = (chr);                           \
    QASSERT(518, (buf) < (end));                  \
    *(buf)++ = __chr;                             \
    if ( (buf) >= (end) )                         \
    {                                             \
        (buf) = (end)-1;                          \
        (buf)[0] = '\0';                          \
    }                                             \
  } while (0)

/// Put a zero byte at buffer.
/// NB: does not increase buf pointer!
#define APPZERO(buf, end)                         \
  do                                              \
  {                                               \
    QASSERT(519, (buf) < (end));                  \
    *(buf) = '\0';                                \
  } while (0)

/// Append a string to the buffer checking the buffer size
#define APPEND(buf, end, name)                    \
  do                                              \
  {                                               \
    QASSERT(520, (buf) < (end));                  \
    const char *__ida_in = (name);                \
    while ( true )                                \
    {                                             \
      if ( (buf) == (end)-1 )                     \
      {                                           \
        (buf)[0] = '\0';                          \
        break;                                    \
      }                                           \
      if (( *(buf) = *__ida_in++) == '\0' )       \
        break;                                    \
      (buf)++;                                    \
    }                                             \
  } while ( 0 )
//@}

/// qalloc() 'size' bytes, and throw a "not enough memory" error if failed

idaman THREAD_SAFE void *ida_export qalloc_or_throw(size_t size);


/// qrealloc() 'ptr' by 'size', and throw a "not enough memory" error if failed

idaman THREAD_SAFE void *ida_export qrealloc_or_throw(void *ptr, size_t size);


/// Change capacity of given qvector.
/// \param vec     a pointer to a qvector
/// \param old     a pointer to the qvector's array
/// \param cnt     number of elements to reserve
/// \param elsize  size of each element
/// \return a pointer to the newly allocated array

idaman THREAD_SAFE void *ida_export qvector_reserve(void *vec, void *old, size_t cnt, size_t elsize);

#if defined(__cplusplus)
  /// \def{PLACEMENT_DELETE, bcc complains about placement delete}
  /// \def{DEFINE_MEMORY_ALLOCATION_FUNCS,
  ///      Convenience macro to declare memory allocation functions.
  ///      It must be used for all classes that can be allocated/freed by the IDA kernel.}
  #if defined(SWIG)
    #define DEFINE_MEMORY_ALLOCATION_FUNCS()
  #else
    #ifndef __BORLANDC__
      #define PLACEMENT_DELETE void operator delete(void *, void *) {}
    #else
      #define PLACEMENT_DELETE
    #endif
    #define DEFINE_MEMORY_ALLOCATION_FUNCS()                          \
      void *operator new  (size_t _s) { return qalloc_or_throw(_s); } \
      void *operator new[](size_t _s) { return qalloc_or_throw(_s); } \
      void *operator new(size_t /*size*/, void *_v) { return _v; }    \
      void operator delete  (void *_blk) { qfree(_blk); }             \
      void operator delete[](void *_blk) { qfree(_blk); }             \
      PLACEMENT_DELETE
  #endif

/// Macro to declare standard inline comparison operators
#define DECLARE_COMPARISON_OPERATORS(type)                              \
  bool operator==(const type &r) const { return compare(r) == 0; }      \
  bool operator!=(const type &r) const { return compare(r) != 0; }      \
  bool operator< (const type &r) const { return compare(r) <  0; }      \
  bool operator> (const type &r) const { return compare(r) >  0; }      \
  bool operator<=(const type &r) const { return compare(r) <= 0; }      \
  bool operator>=(const type &r) const { return compare(r) >= 0; }

/// Macro to declare comparisons for our classes.
/// All comparison operators call the compare() function which returns -1/0/1
#define DECLARE_COMPARISONS(type)    \
  DECLARE_COMPARISON_OPERATORS(type) \
  friend int compare(const type &a, const type &b) { return a.compare(b); } \
  int compare(const type &r) const

// Internal declarations to detect pod-types
/// \cond
struct ida_true_type {};
struct ida_false_type {};
template <class T> struct ida_type_traits     { typedef ida_false_type is_pod_type; };
template <class T> struct ida_type_traits<T*> { typedef ida_true_type is_pod_type; };
template <> struct ida_type_traits< char>  { typedef ida_true_type is_pod_type; };
template <> struct ida_type_traits<uchar>  { typedef ida_true_type is_pod_type; };
template <> struct ida_type_traits<  int>  { typedef ida_true_type is_pod_type; };
template <> struct ida_type_traits< uint>  { typedef ida_true_type is_pod_type; };
template <> struct ida_type_traits<short>  { typedef ida_true_type is_pod_type; };
template <> struct ida_type_traits<ushort> { typedef ida_true_type is_pod_type; };
template <> struct ida_type_traits< long>  { typedef ida_true_type is_pod_type; };
template <> struct ida_type_traits<unsigned long> { typedef ida_true_type is_pod_type; };
template <> struct ida_type_traits< int64> { typedef ida_true_type is_pod_type; };
template <> struct ida_type_traits<uint64> { typedef ida_true_type is_pod_type; };
inline bool check_type_trait(ida_false_type) { return false; }
inline bool check_type_trait(ida_true_type) { return true; }
template <class T> inline bool is_pod_type(void)
{
  typedef typename ida_type_traits<T>::is_pod_type is_pod_type;
  return check_type_trait(is_pod_type());
}

// Can we move around objects of type T using simple memcpy/memmove?.
// This class can be specialized for any type T to improve qvector's behavior.
template <class T> struct ida_movable_type
{
  typedef typename ida_type_traits<T>::is_pod_type is_movable_type;
};
#define DECLARE_TYPE_AS_MOVABLE(T) template <> struct ida_movable_type<T> { typedef ida_true_type is_movable_type; }
template <class T> inline bool may_move_bytes(void)
{
  typedef typename ida_movable_type<T>::is_movable_type mmb_t;
  return check_type_trait(mmb_t());
}
/// \endcond

//---------------------------------------------------------------------------
/// Reimplementation of vector class from STL.
/// Only the most essential functions are implemented.                          \n
/// The vector container accepts objects agnostic to their positions
/// in the memory because it will move them arbitrarily (realloc and memmove).  \n
/// The reason why we have it is because it is not compiler dependent
/// (hopefully) and therefore can be used in IDA API.
template <class T> class qvector
{
  T *array;
  size_t n, alloc;
  friend void *ida_export qvector_reserve(void *vec, void *old, size_t cnt, size_t elsize);
  /// Copy contents of given qvector into this one
  qvector<T> &assign(const qvector<T> &x)
  {
    if ( x.n > 0 )
    {
      array = (T*)qalloc_or_throw(x.alloc * sizeof(T));
      alloc = x.alloc;
      while ( n < x.n )
      {
        new (array+n) T(x.array[n]);
        ++n;
      }
    }
    return *this;
  }
  /// Move data down in memory.
  /// \param dst  destination ptr
  /// \param src  source ptr
  /// \param cnt  number of elements to move
  void shift_down(T *dst, T *src, size_t cnt)
  {
    if ( may_move_bytes<T>() )
    {
      memmove(dst, src, cnt*sizeof(T));
    }
    else
    {
      ssize_t s = cnt;
      while( --s >= 0 )
      {
        new(dst) T(*src);
        src->~T();
        ++src;
        ++dst;
      }
    }
  }
  /// Move data up in memory.
  /// \param dst  destination ptr
  /// \param src  source ptr
  /// \param cnt  number of elements to move
  void shift_up(T *dst, T *src, size_t cnt)
  {
    if ( may_move_bytes<T>() )
    {
      memmove(dst, src, cnt*sizeof(T));
    }
    else
    {
      ssize_t s = cnt;
      dst += s;
      src += s;
      while( --s >= 0 )
      {
        --src;
        --dst;
        new(dst) T(*src);
        src->~T();
      }
    }
  }
public:
  typedef T value_type; ///< the type of objects contained in this qvector
  /// Constructor
  qvector(void) : array(NULL), n(0), alloc(0) {}
  /// Constructor - creates a new qvector identical to 'x'
  qvector(const qvector<T> &x) : array(NULL), n(0), alloc(0) { assign(x); }
  /// Destructor
  ~qvector(void) { clear(); }
  DEFINE_MEMORY_ALLOCATION_FUNCS()
  /// Append a new element to the end the qvector.
  void push_back(const T &x)
  {
    reserve(n+1);
    new (array+n) T(x);
    ++n;
  }
  /// Append a new empty element to the end of the qvector.
  /// \return a reference to this new element
  T &push_back(void)
  {
    reserve(n+1);
    T *ptr = array + n;
    new (ptr) T;
    ++n;
    return *ptr;
  }
  /// Remove the last element in the qvector
  void pop_back(void)
  {
    if ( n > 0 )
    {
#ifdef UNDER_CE         // clarm.exe is buggy
      --n;
      if ( !is_pod_type<T>() )
        array[n].~T();
#else
      array[--n].~T();
#endif
    }
  }
  size_t size(void) const { return n; }                           ///< Get the number of elements in the qvector
  bool empty(void) const { return n == 0; }                       ///< Does the qvector have 0 elements?
  const T &operator[](size_t _idx) const { return array[_idx]; }  ///< Allows use of typical c-style array indexing for qvectors
        T &operator[](size_t _idx)       { return array[_idx]; }  ///< Allows use of typical c-style array indexing for qvectors
  const T &at(size_t _idx) const { return array[_idx]; }          ///< Get element at index '_idx'
        T &at(size_t _idx)       { return array[_idx]; }          ///< Get element at index '_idx'
  const T &front(void) const { return array[0]; }                 ///< Get the first element in the qvector
        T &front(void)       { return array[0]; }                 ///< Get the first element in the qvector
  const T &back(void) const { return array[n-1]; }                ///< Get the last element in the qvector
        T &back(void)       { return array[n-1]; }                ///< Get the last element in the qvector
  /// Destroy all elements but do not free memory
  void qclear(void)
  {
    if ( is_pod_type<T>() )
    {
      n = 0;
    }
    else
    {
      while ( n > 0 )
      {
        array[n-1].~T();
        --n;
      }
    }
  }
  /// Destroy all elements and free memory
  void clear(void)
  {
    if ( array != NULL )
    {
      qclear();
      qfree(array);
      array = NULL;
      alloc = 0;
    }
  }
  /// Allow assignment of one qvector to another using '='
  qvector<T> &operator=(const qvector<T> &x)
  {
    size_t mn = qmin(n, x.n);
    for ( size_t i=0; i < mn; i++ )
      array[i] = x.array[i];
    if ( n > x.n )
    {
      if ( is_pod_type<T>() )
      {
        n = x.n;
      }
      else
      {
        while ( n > x.n )
        {
          array[n-1].~T();
          --n;
        }
      }
    }
    else
    {
      reserve(x.n);
      while ( n < x.n )
      {
        new(array+n) T(x.array[n]);
        ++n;
      }
    }
    return *this;
  }
  /// Resize to the given size.
  /// If the given size (_newsize) is less than the current size (n) of the qvector, then the last n - _newsize elements are simply deleted.                      \n
  /// If the given size is greater than the current size, the qvector is grown to _newsize, and the last _newsize - n elements will be filled with copies of 'x'. \n
  /// If the given size is equal to the current size, this function does nothing.
  void resize(size_t _newsize, const T &x)
  {
    if ( _newsize < n )
    {
      if ( !is_pod_type<T>() )
        for ( ssize_t i=ssize_t(n); --i >= ssize_t(_newsize); )
          array[i].~T();
      n = _newsize;
    }
    else
    {
      reserve(_newsize);
      while ( n < _newsize )
      {
        new(array+n) T(x);
        ++n;
      }
    }
  }
  /// Same as resize(size_t, const T &), but extra space is filled with empty elements
  void resize(size_t _newsize)
  {
    if ( is_pod_type<T>() )
    {
      if ( n < _newsize )
      {
        reserve(_newsize);
        memset(array+n, 0, (_newsize-n)*sizeof(T));
      }
      n = _newsize;
    }
    else
    {
      resize(_newsize, T());
    }
  }
  /// Add an element to the end of the qvector, which will be a new T() if x is not given
  void grow(const T &x=T())
  {
    reserve(n+1);
    new(array+n) T(x);
    ++n;
  }
  /// Get the number of elements that this qvector can contain - not the same
  /// as the number of elements currently in the qvector (size())
  size_t capacity(void) const { return alloc; }
  /// Increase the capacity of the qvector. If cnt is not greater than the current capacity
  /// this function does nothing.
  void reserve(size_t cnt)
  {
    if ( cnt > alloc )
    {
      if ( may_move_bytes<T>() )
      {
        array = (T *)qvector_reserve(this, array, cnt, sizeof(T));
      }
      else
      {
        size_t old_alloc = alloc;
        T *new_array = (T *)qvector_reserve(this, NULL, cnt, sizeof(T));
        size_t new_alloc = alloc;
        alloc = old_alloc;
        shift_down(new_array, array, n);
        qfree(array);
        array = new_array;
        alloc = new_alloc;
      }
    }
  }
  /// Shrink the capacity down to the current number of elements
  void truncate(void)
  {
    if ( alloc > n )
    {
      array = (T*)qrealloc(array, n*sizeof(T)); // should not fail
      alloc = n;
    }
  }
  /// Replace all attributes of this qvector with that of 'r', and vice versa.
  /// Effectively sets this = r and r = this without copying/allocating any memory.
  void swap(qvector<T> &r)
  {
    T *array2     = array;
    size_t n2     = n;
    size_t alloc2 = alloc;

    array = r.array;
    n     = r.n;
    alloc = r.alloc;

    r.array = array2;
    r.n     = n2;
    r.alloc = alloc2;
  }
  /// Empty the qvector and return a pointer to it's contents.
  /// The caller must free the result of this function
  T *extract(void)
  {
    truncate();
    alloc = 0;
    n = 0;
    T *res = array;
    array = NULL;
    return res;
  }
  /// Populate the qvector with dynamic memory.
  /// The qvector must be empty before calling this method!
  void inject(T *s, size_t len)
  {
    array = s;
    alloc = len;
    n = len;
  }
  /// Allow ability to test the equality of two qvectors using '=='.
  bool operator == (const qvector<T> &r) const
  {
    if ( n != r.n )
      return false;
    for ( size_t i=0; i < n; i++ )
      if ( array[i] != r[i] )
        return false;
    return true;
  }
  /// Allow ability to test equality of two qvectors using '!='
  bool operator != (const qvector<T> &r) const { return !(*this == r); }

  typedef T *iterator;
  typedef const T *const_iterator;

  iterator begin(void) { return array; }                ///< Get an iterator that points to the first element in the qvector
  iterator end(void) { return array + n; }              ///< Get an iterator that points to the end of the qvector (NOT the last element)
  const_iterator begin(void) const { return array; }    ///< Get a const iterator that points to the first element in the qvector
  const_iterator end(void) const { return array + n; }  ///< Get a const iterator that points to the end of the qvector (NOT the last element)
  /// Insert an element into the qvector at a specified position.
  /// \param it  an iterator that points to the desired position of the new element
  /// \param x  the element to insert
  /// \return an iterator that points to the newly inserted element
  iterator insert(iterator it, const T &x)
  {
    size_t idx = it - array;
    reserve(n+1);
    T *p = array + idx;
    size_t rest = end() - p;
    shift_up(p+1, p, rest);
    new(p) T(x);
    n++;
    return iterator(p);
  }
  /// Insert a several elements to the qvector at a specified position.
  /// \param it     position at which new elements will be inserted
  /// \param first  pointer to first element to be inserted
  /// \param last   pointer to end of elements to be inserted (the element pointed to by 'last' will not be included)
  /// \return an iterator that points to the first newly inserted element.
  template <class it2> iterator insert(iterator it, it2 first, it2 last)
  {
    size_t cnt = last - first;
    if ( cnt == 0 )
      return it;

    size_t idx = it - array;
    reserve(n+cnt);
    T *p = array + idx;
    size_t rest = end() - p;
    shift_up(p+cnt, p, rest);
    while ( first != last )
    {
      new(p) T(*first);
      ++p;
      ++first;
    }
    n += cnt;
    return iterator(array+idx);
  }
  /// Remove an element from the qvector.
  /// \param it  pointer to element to be removed
  /// \return pointer to the element that took its place
  iterator erase(iterator it)
  {
    it->~T();
    size_t rest = end() - it - 1;
    shift_down(it, it+1, rest);
    n--;
    return it;
  }
  /// Remove a subset of the qvector.
  /// \param first  pointer to head of subset to be removed
  /// \param last   pointer to end of subset to be removed (element pointed to by last will not be removed)
  /// \return a pointer to the element that took the place of 'first'
  iterator erase(iterator first, iterator last)
  {
    for ( T *p=first; p != last; ++p )
      p->~T();
    size_t rest = end() - last;
    shift_down(first, last, rest);
    n -= last - first;
    return first;
  }
  // non-standard extensions:
  /// Find an element in the qvector.
  /// \param x  element to find
  /// \return an iterator that points to the first occurrence of 'x'
  iterator find(const T &x)
  {
    iterator p;
    const_iterator e;
    for ( p=begin(), e=end(); p != e; ++p )
      if ( x == *p )
        break;
    return p;
  }
  /// \copydoc find
  const_iterator find(const T &x) const
  {
    const_iterator p, e;
    for ( p=begin(), e=end(); p != e; ++p )
      if ( x == *p )
        break;
    return p;
  }
  /// Does the qvector contain x?
  bool has(const T &x) const { return find(x) != end(); }
  /// Add an element to the end of the qvector - only if it isn't already present.
  /// \param x  the element to add
  /// \return false if 'x' is already in the qvector, true otherwise
  bool add_unique(const T &x)
  {
    if ( has(x) )
      return false;
    push_back(x);
    return true;
  }
  /// Find an element and remove it.
  /// \param x  the element to remove
  /// \return false if 'x' was not found, true otherwise
  bool del(const T &x)
  {
    iterator p = find(x);
    if ( p == end() )
      return false;
    erase(p);
    return true;
  }
};

/// Reimplementation of stack class from STL.
/// The reason why we have it is because it is not compiler dependent
/// (hopefully) and therefore can be used in IDA API
template<class T>
class qstack : public qvector<T>
{
  typedef qvector<T> inherited;
public:
  T pop(void)
  {
    T v = inherited::back();
    inherited::pop_back();
    return v;
  }
  const T &top(void) const
  {
    return inherited::back();
  }
  T &top(void) { return CONST_CAST(T&)(CONST_CAST(const qstack<T>*)(this)->top()); }
  void push(const T &v)
  {
    inherited::push_back(v);
  }
};

/// Standard lexical comparison.
/// \return -1 if a < b, 1 if a > b, and 0 if a == b
template <class T> int lexcompare(const T &a, const T &b)
{
  if ( a < b )
    return -1;
  if ( a > b )
    return 1;
  return 0;
}

/// Lexical comparison of two vectors. Also see lexcompare().
/// \return 0 if the two vectors are identical
///         1 if 'a' is larger than 'b'
///        -1 if 'a' is smaller than 'b'
///        otherwise return the first nonzero lexical comparison between each element in 'a' and 'b'
template <class T> int lexcompare_vectors(const T &a, const T &b)
{
  if ( a.size() != b.size() )
    return a.size() > b.size() ? 1 : -1;
  for ( int i=0; i < a.size(); i++ )
  {
    int code = lexcompare(a[i], b[i]);
    if ( code != 0 )
      return code;
  }
  return 0;
}

/// Smart pointer to objects derived from ::qrefcnt_obj_t
template <class T>
class qrefcnt_t
{
  T *ptr;
  void delref(void)
  {
    if ( ptr != NULL && --ptr->refcnt == 0 )
      ptr->release();
  }
public:
  explicit qrefcnt_t(T *p) : ptr(p) {}
  qrefcnt_t(const qrefcnt_t &r) : ptr(r.ptr)
  {
    if ( ptr != NULL )
      ptr->refcnt++;
  }
  qrefcnt_t &operator=(const qrefcnt_t &r)
  {
    delref();
    ptr = r.ptr;
    if ( ptr != NULL )
      ptr->refcnt++;
    return *this;
  }
  ~qrefcnt_t(void)
  {
    delref();
  }
  void reset(void)
  {
    delref();
    ptr = NULL;
  }
  operator T *() const
  {
    return ptr;
  }
  T *operator ->() const
  {
    return ptr;
  }
  T &operator *() const
  {
    return *ptr;
  }
};

/// Base class for reference count objects
class qrefcnt_obj_t
{
public:
  int refcnt; ///< counter
  /// Constructor
  qrefcnt_obj_t(void) : refcnt(1) {}
  /// Call destructor.
  /// We use release() instead of operator delete() to maintain binary
  /// compatibility with all compilers (vc and gcc use different vtable layouts
  /// for operator delete)
  virtual void idaapi release(void) = 0;
};

/// Interface class for iterator types.
template <class T>
class qiterator : public qrefcnt_obj_t
{
public:
  typedef T value_type;
  virtual bool idaapi first(void) = 0;
  virtual bool idaapi next(void) = 0;
  virtual T idaapi operator *(void) = 0;
  virtual T get(void) { return this->operator*(); }
};


/// \name strlen
/// Get the length of the given string
//@{
inline size_t idaapi qstrlen(const char *s) { return strlen(s); }
inline size_t idaapi qstrlen(const uchar *s) { return strlen((const char *)s); }
idaman THREAD_SAFE size_t ida_export qstrlen(const wchar16_t *s);
//@}

/// \name strcmp
/// Lexical comparison of strings.
/// \return 0 if two strings are identical
///         > 0 if 's1' is larger than 's2'
///         < 0 if 's2' is larger than 's1'
///         otherwise return first nonzero comparison between chars in 's1' and 's2'
//@{
inline int idaapi qstrcmp(const char *s1, const char *s2) { return strcmp(s1, s2); }
inline int idaapi qstrcmp(const uchar *s1, const uchar *s2) { return strcmp((const char *)s1, (const char *)s2); }
idaman THREAD_SAFE int ida_export qstrcmp(const wchar16_t *s1, const wchar16_t *s2);
//@}

/// \name strstr
/// Find a string within another string.
/// \return a pointer to the first occurrence of 's2' within 's1', NULL if s2 is not found in s1
//@{
inline const char *idaapi qstrstr(const char *s1, const char *s2) { return strstr(s1, s2); }
inline const uchar *idaapi qstrstr(const uchar *s1, const uchar *s2) { return (const uchar *)strstr((const char *)s1, (const char *)s2); }
//@}

/// \name strchr
/// Find a character within a string.
/// \return a pointer to the first occurrence of 'c' within 's1', NULL if c is not found in s1
//@{
inline       char *idaapi qstrchr(      char *s1, char c) { return strchr(s1, c); }
inline const char *idaapi qstrchr(const char *s1, char c) { return strchr(s1, c); }
inline       uchar *idaapi qstrchr(      uchar *s1, uchar c) { return (      uchar *)strchr((      char *)s1, c); }
inline const uchar *idaapi qstrchr(const uchar *s1, uchar c) { return (const uchar *)strchr((const char *)s1, c); }
idaman THREAD_SAFE const wchar16_t *ida_export qstrchr(const wchar16_t *s1, wchar16_t c);
inline THREAD_SAFE       wchar16_t *ida_export qstrchr(      wchar16_t *s1, wchar16_t c)
  { return (wchar16_t *)qstrchr((const wchar16_t *)s1, c); }
//@}

/// \name qstrrchr
/// Find a last occurrence of a character within a string.
/// \return a pointer to the last occurrence of 'c' within 's1', NULL if c is not found in s1
//@{
inline const char *idaapi qstrrchr(const char *s1, char c) { return strrchr(s1, c); }
inline       char *idaapi qstrrchr(      char *s1, char c) { return strrchr(s1, c); }
inline const uchar *idaapi qstrrchr(const uchar *s1, uchar c) { return (const uchar *)strrchr((const char *)s1, c); }
inline       uchar *idaapi qstrrchr(      uchar *s1, uchar c) { return (      uchar *)strrchr((const char *)s1, c); }
idaman THREAD_SAFE const wchar16_t *ida_export qstrrchr(const wchar16_t *s1, wchar16_t c);
inline THREAD_SAFE       wchar16_t *ida_export qstrrchr(      wchar16_t *s1, wchar16_t c)
  { return (wchar16_t *)qstrrchr((const wchar16_t *)s1, c); }
//@}


/// Reimplementation of the string class from STL.
/// Only the most essential functions are implemented.
/// The reason why we have this is because it is not compiler dependent
/// (hopefully) and therefore can be used in IDA API
template<class qchar>
class _qstring
{
  qvector<qchar> body;
public:
  /// Constructor
  _qstring(void) {}
  /// Constructor - creates a new qstring from an existing char *
  _qstring(const qchar *ptr)
  {
    if ( ptr != NULL )
    {
      size_t len = ::qstrlen(ptr) + 1;
      body.resize(len);
      memmove(body.begin(), ptr, len*sizeof(qchar));
    }
  }
  /// Constructor - creates a new qstring using first 'len' chars from 'ptr'
  _qstring(const qchar *ptr, size_t len)
  {
    if ( len > 0 )
    {
      body.resize(len+1);
      memmove(body.begin(), ptr, len*sizeof(qchar));
    }
  }
  void swap(_qstring<qchar> &r) { body.swap(r.body); }                        ///< Swap contents of two qstrings. see qvector::swap()
  size_t length(void) const { size_t l = body.size(); return l ? l - 1 : 0; } ///< Get number of chars in this qstring (not including terminating zero)
  size_t size(void) const { return body.size(); }                             ///< Get number of chars in this qstring (including terminating zero)
  /// Resize to the given size.
  /// The resulting qstring will have length() = s, and size() = s+1                   \n
  /// if 's' is greater than the current size then the extra space is filled with 'c'. \n
  /// if 's' is less than the current size then the trailing chars are removed
  void resize(size_t s, qchar c)
  {
    size_t oldsize = body.size();
    if ( oldsize != 0 && s >= oldsize )
      body[oldsize-1] = c; // erase the terminating zero
    body.resize(s+1, c);
    body[s] = 0; // ensure the terminating zero
  }
  /// Similar to resize(size_t, qchar) - but any extra space is filled with zeroes
  void resize(size_t s)
  {
    if ( s == 0 )
    {
      body.clear();
    }
    else
    {
      body.resize(s+1);
      body[s] = 0; // ensure the terminating zero
    }
  }
  void remove_last(int cnt=1)
  {
    ssize_t len = body.size() - cnt;
    if ( len <= 1 )
    {
      body.clear();
    }
    else
    {
      body.resize(len);
      body[len-1] = 0;
    }
  }
  void reserve(size_t cnt) { body.reserve(cnt); }     ///< Increase capacity the qstring. see qvector::reserve()
  void clear(void) { body.clear(); }                  ///< Clear qstring and free memory
  void qclear(void) { body.qclear(); }                ///< Clear qstring but do not free memory yet
  bool empty(void) const { return body.size() <= 1; } ///< Does the qstring have 0 non-null elements?
  /// Convert the qstring to a char *
  const qchar *c_str(void) const
  {
    static const qchar nullstr[] = { 0 };
    return body.empty() ? nullstr : &body[0];
  }
  typedef qchar *iterator;
  typedef const qchar *const_iterator;
        iterator begin(void)       { return body.begin(); } ///< Get a pointer to the beginning of the qstring
  const_iterator begin(void) const { return body.begin(); } ///< Get a const pointer to the beginning of the qstring
        iterator end(void)       { return body.end(); }     ///< Get a pointer to the end of the qstring (this is not the terminating zero)
  const_iterator end(void) const { return body.end(); }     ///< Get a const pointer to the end of the qstring (this is not the terminating zero)
  /// Allow assignment of qstrings using '='
  _qstring &operator=(const qchar *str)
  {
    size_t len = str == NULL ? 0 : ::qstrlen(str);
    if ( len > 0 )
    {
      body.resize(len+1);
      memmove(body.begin(), str, len*sizeof(qchar));
      body[len] = '\0';
    }
    else
    {
      qclear();
    }
    return *this;
  }
  /// Append a char using '+='
  _qstring &operator+=(qchar c)
  {
    return append(c);
  }
  /// Append another qstring using '+='
  _qstring &operator+=(const _qstring &r)
  {
    return append(r);
  }
  /// Get result of appending two qstrings using '+'
  _qstring operator+(const _qstring &r) const
  {
    _qstring s = *this;
    s += r;
    return s;
  }
  /// Test equality of two qstrings using '=='
  bool operator==(const _qstring &r) const
  {
    return ::qstrcmp(c_str(), r.c_str()) == 0;
  }
  /// Test equality of a qstring and a const char* using '=='
  bool operator==(const qchar *r) const
  {
    return ::qstrcmp(c_str(), r) == 0;
  }
  bool operator!=(const _qstring &r) const { return !(*this == r); }  ///< Test equality of two qstrings using '!='
  bool operator!=(const qchar *r) const { return !(*this == r); }     ///< Test equality of a qstring and a const char* with '!='
  /// Compare two qstrings using '<'. see qstrcmp()
  bool operator<(const _qstring &r) const
  {
    return ::qstrcmp(c_str(), r.c_str()) < 0;
  }
  /// Compare two qstrings using '<'. see qstrcmp()
  bool operator<(const qchar *r) const
  {
    return ::qstrcmp(c_str(), r) < 0;
  }
  /// Retrieve char at index 'idx' using '[]'
  const qchar &operator[](size_t idx) const
  {
    if ( !body.empty() || idx )
      return body[idx];
    static const qchar nullstr[] = { 0 };
    return nullstr[0];
  }
  /// Retrieve char at index 'idx' using '[]'
  qchar &operator[](size_t idx)
  {
    if ( !body.empty() || idx )
      return body[idx];
    static qchar nullstr[] = { 0 };
    return nullstr[0];
  }
  const qchar &at(size_t idx) const { return body.at(idx); } ///< Retrieve const char at index 'idx'
  qchar &at(size_t idx) { return body.at(idx); }             ///< Retrieve char at index 'idx'
  /// Extract C string from _qstring. Must qfree() it.
  qchar *extract(void) { return body.extract(); }
  /// Assign this qstring to an existing char *.
  /// See qvector::inject(T *, size_t)
  void inject(qchar *s, size_t len)
  {
    body.inject(s, len);
  }
  /// Same as to inject(qchar *, size_t), with len = strlen(s)
  void inject(qchar *s)
  {
    if ( s != NULL )
    {
      size_t len = ::qstrlen(s) + 1;
      body.inject(s, len);
    }
  }
  /// Get the last qchar in the string (for concatenation checks)
  qchar last(void) const
  {
    size_t len = length();
    return len == 0 ? '\0' : body[len-1];
  }
  /// Find a substring.
  /// \param str  the substring to look for
  /// \param pos  starting position
  /// \return the position of the beginning of the first occurrence of str, -1 of none exists
  size_t find(const qchar *str, size_t pos=0) const
  {
    if ( pos <= length() )
    {
      const qchar *beg = c_str();
      const qchar *ptr = ::qstrstr(beg+pos, str);
      if ( ptr != NULL )
        return ptr - beg;
    }
    return npos;
  }
  /// Replace all occurrences of 'what' with 'with'.
  /// \return false if 'what' is not found in the qstring, true otherwise
  bool replace(const qchar *what, const qchar *with)
  {
    _qstring result;
    size_t len_what = ::qstrlen(what);
    const qchar *_start = c_str();
    const qchar *last_pos = _start;
    while ( true )
    {
      const qchar *pos = ::qstrstr(last_pos, what);
      if ( pos == NULL )
        break;
      size_t n = pos - last_pos;
      if ( n > 0 )
        result.append(last_pos, n);
      result.append(with);
      last_pos = pos + len_what;
    }
    // no match at all?
    if ( last_pos == _start )
      return false;
    // any pending characters?
    if ( *last_pos )
      result.append(last_pos);
    swap(result);
    return true;
  }
  /// Same as find(const qchar *, size_t), but takes a qstring parameter
  size_t find(const _qstring &str, size_t pos=0) const { return find(str.c_str(), pos); }
  /// Find a character in the qstring.
  /// \param c    the character to look for
  /// \param pos  starting position
  /// \return index of first occurrence of 'c' if c is found, -1 otherwise
  size_t find(qchar c, size_t pos=0) const
  {
    if ( pos <= length() )
    {
      const qchar *beg = c_str();
      const qchar *ptr = qstrchr(beg+pos, c);
      if ( ptr != NULL )
        return ptr - beg;
    }
    return npos;
  }
  /// Search backwards for a character in the qstring.
  /// \param c    the char to look for
  /// \param pos  starting position
  /// \return index of first occurrence of 'c' if c is found, -1 otherwise
  size_t rfind(qchar c, size_t pos=0) const
  {
    if ( pos <= length() )
    {
      const qchar *beg = c_str();
      const qchar *ptr = qstrrchr(beg+pos, c);
      if ( ptr != NULL )
        return ptr - beg;
    }
    return npos;
  }
  /// Get a substring.
  /// \param pos   starting position
  /// \param n     ending position (non-inclusive)
  /// \return the resulting substring
  _qstring<qchar> substr(size_t pos=0, size_t n=npos) const
  {
    size_t endp = qmin(length(), n);
    if ( pos >= endp )
      pos = endp;
    return _qstring<qchar>(c_str()+pos, endp-pos);
  }
  /// Remove characters from the qstring.
  /// \param idx  starting position
  /// \param cnt  number of characters to remove
  _qstring& remove(size_t idx, size_t cnt)
  {
    size_t len = length();
    if ( idx < len && cnt != 0 )
    {
      cnt += idx;
      if ( cnt < len )
      {
        iterator p1 = body.begin() + cnt;
        iterator p2 = body.begin() + idx;
        memmove(p2, p1, (len-cnt)*sizeof(qchar));
        idx += len - cnt;
      }
      body.resize(idx+1);
      body[idx] = '\0';
    }
    return *this;
  }
  /// Insert a character into the qstring.
  /// \param idx  position of insertion (if idx >= length(), the effect is the same as append)
  /// \param c    char to insert
  _qstring& insert(size_t idx, qchar c)
  {
    size_t len = length();
    body.resize(len+2);
    body[len+1] = '\0';
    if ( idx < len )
    {
      iterator p1 = body.begin() + idx;
      memmove(p1+1, p1, (len-idx)*sizeof(qchar));
      len = idx;
    }
    body[len] = c;
    return *this;
  }
  /// Insert a string into the qstring.
  /// \param idx     position of insertion (if idx >= length(), the effect is the same as append)
  /// \param str     the string to insert
  /// \param addlen  number of chars from 'str' to insert
  _qstring& insert(size_t idx, const qchar *str, size_t addlen)
  {
    size_t len = length();
    body.resize(len+addlen+1);
    body[len+addlen] = '\0';
    if ( idx < len )
    {
      iterator p1 = body.begin() + idx;
      iterator p2 = p1 + addlen;
      memmove(p2, p1, (len-idx)*sizeof(qchar));
      len = idx;
    }
    memmove(body.begin()+len, str, addlen*sizeof(qchar));
    return *this;
  }
  /// Same as insert(size_t, const qchar *, size_t), but all chars in str are inserted
  _qstring& insert(size_t idx, const qchar *str)
  {
    if ( str != NULL )
    {
      size_t addlen = ::qstrlen(str);
      insert(idx, str, addlen);
    }
    return *this;
  }
  /// Same as insert(size_t, const qchar *), but takes a qstring parameter
  _qstring& insert(size_t idx, const _qstring &qstr)
  {
    size_t len = length();
    size_t add = qstr.length();
    body.resize(len+add+1);
    body[len+add] = '\0';
    if ( idx < len )
    {
      iterator p1 = body.begin() + idx;
      iterator p2 = p1 + add;
      memmove(p2, p1, (len-idx)*sizeof(qchar));
      len = idx;
    }
    memcpy(body.begin()+len, qstr.begin(), add*sizeof(qchar));
    return *this;
  }
  _qstring& insert(qchar c)               { return insert(0, c);    } ///< Prepend the qstring with 'c'
  _qstring& insert(const qchar *str)      { return insert(0, str);  } ///< Prepend the qstring with 'str'
  _qstring& insert(const _qstring &qstr)  { return insert(0, qstr); } ///< Prepend the qstring with 'qstr'
  /// Append c to the end of the qstring
  _qstring& append(qchar c)
  {
    size_t len = length();
    body.resize(len+2);
    body[len] = c;
    body[len+1] = '\0';
    return *this;
  }
  /// Append a string to the qstring.
  /// \param str     the string to append
  /// \param addlen  number of characters from 'str' to append
  _qstring& append(const qchar *str, size_t addlen)
  {
    size_t len = length();
    body.resize(len+addlen+1);
    body[len+addlen] = '\0';
    memmove(body.begin()+len, str, addlen*sizeof(qchar));
    return *this;
  }
  /// Same as append(const qchar *, size_t), but all chars in 'str' are appended
  _qstring& append(const qchar *str)
  {
    if ( str != NULL )
    {
      size_t addlen = ::qstrlen(str);
      append(str, addlen);
    }
    return *this;
  }
  /// Same as append(const qchar *), but takes a qstring argument
  _qstring& append(const _qstring &qstr)
  {
    size_t add = qstr.length();
    if ( add != 0 )
    {
      size_t len = length();
      body.resize(len+add+1);
      body[len+add] = '\0';
      memcpy(body.begin()+len, qstr.begin(), add*sizeof(qchar));
    }
    return *this;
  }
  /// Append result of qvsnprintf() to qstring
  AS_PRINTF(2, 0) _qstring& cat_vsprnt(const char *format, va_list va)
  { // since gcc64 forbids reuse of va_list, we make a copy for the second call:
    va_list copy;
    va_copy(copy, va);
    size_t add = ::qvsnprintf(NULL, 0, format, va);
    if ( add != 0 )
    {
      size_t len = length();
      body.resize(len+add+1);
      ::qvsnprintf(body.begin()+len, add+1, format, copy);
    }
    return *this;
  }
  /// Replace qstring with the result of qvsnprintf()
  AS_PRINTF(2, 0) _qstring& vsprnt(const char *format, va_list va)
  { // since gcc64 forbids reuse of va_list, we make a copy for the second call:
    va_list copy;
    va_copy(copy, va);
    body.clear();
    size_t add = ::qvsnprintf(NULL, 0, format, va);
    if ( add != 0 )
    {
      body.resize(add+1);
      ::qvsnprintf(body.begin(), add+1, format, copy);
    }
    return *this;
  }
  /// Append result of qsnprintf() to qstring
  AS_PRINTF(2, 3) _qstring& cat_sprnt(const char *format, ...)
  {
    va_list va;
    va_start(va, format);
    cat_vsprnt(format, va);
    va_end(va);
    return *this;
  }
  /// Replace qstring with the result of qsnprintf()
  AS_PRINTF(2, 3) _qstring& sprnt(const char *format, ...)
  {
    va_list va;
    va_start(va, format);
    vsprnt(format, va);
    va_end(va);
    return *this;
  }
  /// Fill qstring with a character.
  /// The qstring is resized if necessary until 'len' chars have been filled
  /// \param pos  starting position
  /// \param c    the character to fill
  /// \param len  number of positions to fill with 'c'
  _qstring& fill(size_t pos, qchar c, size_t len)
  {
    size_t endp = pos + len + 1;
    if ( body.size() < endp )
    {
      body.resize(endp);
      body[endp-1] = '\0';
    }
    memset(body.begin()+pos, c, len);
    return *this;
  }
  /// Clear contents of qstring and fill with 'c'
  _qstring& fill(qchar c, size_t len)
  {
    body.qclear();
    if ( len > 0 )
      resize(len, c);
    return *this;
  }
  /// Remove all instances of the specified char from the beginning of the qstring
  _qstring& ltrim(qchar blank = ' ')
  {
    if ( !empty() )
    {
      iterator b = body.begin();
      iterator e = body.end()-1;
      while ( b < e && *b == blank )
        b++;
      if ( b > body.begin() )
      {
        memmove(body.begin(), b, sizeof(qchar)*(e-b+1));
        resize(e-b);
      }
    }
    return *this;
  }
  /// Remove all instances of the specified char from the end of the qstring
  _qstring& rtrim(qchar blank = ' ')
  {
    if ( !empty() )
    {
      iterator b = body.begin();
      iterator e = body.end()-1;
      while ( e > b && *(e-1) == blank )
        e--;
      resize(e-b);
    }
    return *this;
  }
  /// Remove all instances of the specified char from both ends of the qstring
  _qstring& trim2(qchar blank = ' ')
  {
    rtrim(blank);
    ltrim(blank);
    return *this;
  }
// embedded visual studio and visual C/C++ 6.0 are not aware of
//   static const varname = init_value;
/// Invalid position
#if defined(UNDER_CE) || defined(_MSC_VER) && _MSC_VER <= 1200
  enum { npos = -1 };
#else
  static const size_t npos = (size_t) -1;
#endif
};
typedef _qstring<char> qstring;       ///< regular string
typedef _qstring<uchar> qtype;        ///< type string
typedef _qstring<wchar16_t> qwstring; ///< unicode string

/// Vector of bytes (use for dynamic memory)
class bytevec_t: public qvector<uchar>
{
public:
  /// Constructor
  bytevec_t() {}
  /// Constructor - fill bytevec with 'sz' bytes from 'buf'
  bytevec_t(const void *buf, size_t sz) { append(buf, sz); }
  /// Append bytes to the bytevec
  /// \param buf   pointer to buffer that will be appended
  /// \param sz    size of buffer
  bytevec_t &append(const void *buf, size_t sz)
  {
    if ( sz > 0 )
    {
      size_t cur_sz = size();
      size_t new_sz = cur_sz + sz;
      if ( new_sz < cur_sz )
        new_sz = BADMEMSIZE; // integer overflow, ask too much and it will throw
      resize(new_sz);
      memcpy(begin() + cur_sz, buf, sz);
    }
    return *this;
  }
  /// Grow the bytevec and fill with a value
  /// \param sz      number of bytes to add to bytevec
  /// \param filler  filler value
  bytevec_t &growfill(size_t sz, uchar filler=0)
  {
    if ( sz > 0 )
    {
      size_t cur_sz = size();
      size_t new_sz = cur_sz + sz;
      if ( new_sz < cur_sz )
        new_sz = BADMEMSIZE; // integer overflow, ask too much and it will throw
      resize(new_sz, filler);
    }
    return *this;
  }
  /// See qvector::inject(T *, size_t)
  void inject(void *buf, size_t len)
  {
    qvector<uchar>::inject((uchar *)buf, len);
  }

  /// Is the specified bit set in the bytevec?
  bool test_bit(size_t bit) const   { return ::test_bit(begin(), bit); }
  /// Set the specified bit
  void set_bit(size_t bit)          { ::set_bit(begin(), bit); }
  /// Clear the specified bit
  void clear_bit(size_t bit)        { ::clear_bit(begin(), bit); }
  /// See set_all_bits(uchar *, size_t)
  void set_all_bits(size_t nbits)   { resize((nbits+7)/8); ::set_all_bits(begin(), nbits); }
  /// See clear_all_bits(uchar *, size_t)
  void clear_all_bits(size_t nbits) { ::clear_all_bits(begin(), nbits); }
  /// For each bit that is set in 'b', set the corresponding bit in this bytevec
  void set_bits(const bytevec_t &b)
  {
    size_t nbytes = b.size();
    if ( size() < nbytes )
      resize(nbytes);
    for ( size_t i=0; i < nbytes; i++ )
      at(i) |= b[i];
  }
  /// For each bit that is set in 'b', the clear the corresponding bit in this bytevec
  void clear_bits(const bytevec_t &b)
  {
    size_t nbytes = qmin(size(), b.size());
    iterator p = begin();
    for ( size_t i=0; i < nbytes; i++, ++p )
      *p = uchar(*p & ~b[i]);
  }
};

/// Relocation information (relocatable objects - see ::relobj_t)
struct reloc_info_t : public bytevec_t
{
/// \defgroup RELOBJ_ Relocatable object info flags
/// used by relobj_t::ri
//@{
#define RELOBJ_MASK 0xF    ///< the first byte describes the relocation entry types
#define   RELSIZE_1     0  ///< 8-bit relocations
#define   RELSIZE_2     1  ///< 16-bit relocations
#define   RELSIZE_4     2  ///< 32-bit relocations
#define   RELSIZE_8     3  ///< 64-bit relocations
#define   RELSIZE_CUST 15  ///< custom relocations, should be handled internally
#define RELOBJ_CNT 0x80    ///< counter present (not used yet)
//@}
};

idaman THREAD_SAFE bool ida_export relocate_relobj(struct relobj_t *_relobj, ea_t ea, bool mf);

/// Relocatable object
struct relobj_t : public bytevec_t
{
  ea_t base;                            ///< current base
  reloc_info_t ri;                      ///< relocation info

  relobj_t(void) : base(0) {}
  bool relocate(ea_t ea, bool mf) { return relocate_relobj(this, ea, mf); } ///< mf=1:big endian
};

#define QLIST_DEFINED ///< signal that the qlist class has been defined
/// Linked list
template <class T> class qlist
{
  struct listnode_t
  {
    listnode_t *next;
    listnode_t *prev;
  };

  struct datanode_t : public listnode_t
  {
    T data;
  };

  listnode_t node;
  size_t length;

  void init(void)
  {
    node.next = &node;
    node.prev = &node;
    length = 0;
  }

public:
  typedef T value_type;
  class const_iterator;
/// Used for defining the 'iterator' and 'const_iterator' classes for qlist
#define DEFINE_LIST_ITERATOR(iter, constness, cstr)                     \
  class iter                                                            \
  {                                                                     \
    friend class qlist<T>;                                              \
    constness listnode_t *cur;                                          \
    iter(constness listnode_t *x) : cur(x) {}                           \
  public:                                                               \
    typedef constness T value_type;                                     \
    iter(void) {}                                                       \
    iter(const iter &x) : cur(x.cur) {}                                 \
    cstr                                                                \
    iter &operator=(const iter &x) { cur = x.cur; return *this; }       \
    bool operator==(const iter &x) const { return cur == x.cur; }       \
    bool operator!=(const iter &x) const { return cur != x.cur; }       \
    constness T &operator*(void) const { return ((datanode_t*)cur)->data; }  \
    constness T *operator->(void) const { return &(operator*()); } \
    iter& operator++(void)       /* prefix ++  */                       \
    {                                                                   \
      cur = cur->next;                                                  \
      return *this;                                                     \
    }                                                                   \
    iter operator++(int)         /* postfix ++ */                       \
    {                                                                   \
      iter tmp = *this;                                                 \
      ++(*this);                                                        \
      return tmp;                                                       \
    }                                                                   \
    iter& operator--(void)       /* prefix --  */                       \
    {                                                                   \
      cur = cur->prev;                                                  \
      return *this;                                                     \
    }                                                                   \
    iter operator--(int)         /* postfix -- */                       \
    {                                                                   \
      iter tmp = *this;                                                 \
      --(*this);                                                        \
      return tmp;                                                       \
    }                                                                   \
  };
  DEFINE_LIST_ITERATOR(iterator, , friend class const_iterator;)
  DEFINE_LIST_ITERATOR(const_iterator, const, const_iterator(const iterator &x) : cur(x.cur) {})

/// Used to define qlist::reverse_iterator and qlist::const_reverse_iterator
#define DEFINE_REVERSE_ITERATOR(riter, iter)                            \
  class riter                                                           \
  {                                                                     \
    iter p;                                                             \
  public:                                                               \
    riter(void) {}                                                      \
    riter(const iter &x) : p(x) {}                                      \
    typename iter::value_type &operator*(void) const { iter q=p; return *--q; }  \
    typename iter::value_type *operator->(void) const { return &(operator*()); } \
    riter &operator++(void) { --p; return *this; }                      \
    riter  operator++(int) { iter q=p; --p; return q; }                 \
    riter &operator--(void) { ++p; return *this; }                      \
    riter  operator--(int) { iter q=p; ++p; return q; }                 \
    bool operator==(const riter& x) const { return p == x.p; }          \
    bool operator!=(const riter& x) const { return p != x.p; }          \
  };
  DEFINE_REVERSE_ITERATOR(reverse_iterator, iterator)
  DEFINE_REVERSE_ITERATOR(const_reverse_iterator, const_iterator)
#undef DEFINE_LIST_ITERATOR
#undef DEFINE_REVERSE_ITERATOR
  /// Constructor
  qlist(void) { init(); }
  /// Constructor - creates a qlist identical to 'x'
  qlist(const qlist<T> &x)
  {
    init();
    insert(begin(), x.begin(), x.end());
  }
  /// Destructor
  ~qlist(void)
  {
    clear();
  }
  DEFINE_MEMORY_ALLOCATION_FUNCS()

  /// Construct a new qlist using '='
  qlist<T> &operator=(const qlist<T> &x)
  {
    if ( this != &x )
    {
      iterator first1 = begin();
      iterator last1 = end();
      const_iterator first2 = x.begin();
      const_iterator last2 = x.end();
      while ( first1 != last1 && first2 != last2 )
        *first1++ = *first2++;
      if ( first2 == last2 )
        erase(first1, last1);
      else
        insert(last1, first2, last2);
    }
    return *this;
  }
  /// Set this = x and x = this, without copying any memory
  void swap(qlist<T> &x)
  {
    std::swap(node, x.node);
    std::swap(length, x.length);
  }

  iterator begin(void) { return node.next; }      ///< Get a pointer to the head of the list
  iterator end(void) { return &node; }            ///< Get a pointer to the end of the list
  bool empty(void) const { return length == 0; }  ///< Get true if the list has 0 elements
  size_t size(void) const { return length; }      ///< Get the number of elements in the list
  T &front(void) { return *begin(); }             ///< Get the first element in the list
  T &back(void) { return *(--end()); }            ///< Get the last element in the list

  const_iterator begin(void) const { return node.next; } ///< \copydoc begin
  const_iterator end(void) const { return &node; }       ///< \copydoc end
  const T&front(void) const { return *begin(); }         ///< \copydoc front
  const T&back(void) const { return *(--end()); }        ///< \copydoc end

  reverse_iterator rbegin() { return reverse_iterator(end()); }                   ///< Get a reverse iterator that points to end of list. See DEFINE_REVERSE_ITERATOR
  reverse_iterator rend() { return reverse_iterator(begin()); }                   ///< Get a reverse iterator that points to beginning of list. See DEFINE_REVERSE_ITERATOR
  const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); } ///< See rbegin()
  const_reverse_iterator rend() const { return const_reverse_iterator(begin()); } ///< See rend()

  /// Insert an element into the qlist.
  /// \param p  the position to insert the element
  /// \param x  the element to be inserted
  /// \return position of newly inserted element
  iterator insert(iterator p, const T& x)
  {
    datanode_t *tmp = (datanode_t*)qalloc_or_throw(sizeof(datanode_t));
    new (&(tmp->data)) T(x);
    linkin(p, tmp);
    return tmp;
  }
  /// Insert an empty element into the qlist.
  /// \param p  position to insert the element
  /// \return reference to this new element
  T &insert(iterator p)
  {
    datanode_t *tmp = (datanode_t*)qalloc_or_throw(sizeof(datanode_t));
    new (&(tmp->data)) T();
    linkin(p, tmp);
    return tmp->data;
  }
  /// Insert all elements between 'first' and 'last' (non-inclusive)
  /// at position pointed to by 'p'
  template <class it2> void insert(iterator p, it2 first, it2 last)
  {
    while ( first != last )
      insert(p, *first++);
  }
  /// Insert at beginning of list
  void push_front(const T &x) { insert(begin(), x); }
  /// Insert at end of list
  void push_back(const T &x) { insert(end(), x); }
  /// Insert empty element at end of list
  T &push_back(void) { return insert(end()); }
  /// Erase element at position pointed to by 'p'
  void erase(iterator p)
  {
    p.cur->prev->next = p.cur->next;
    p.cur->next->prev = p.cur->prev;
    ((datanode_t*)p.cur)->data.~T();
    qfree(p.cur);
    --length;
  }
  /// Erase all elements between 'p1' and 'p2'
  void erase(iterator p1, iterator p2)
  {
    while ( p1 != p2 )
      erase(p1++);
  }
  /// Erase all elements in the qlist
  void clear(void) { erase(begin(), end()); }
  /// Erase first element of the qlist
  void pop_front(void) { erase(begin()); }
  /// Erase last element of the qlist
  void pop_back(void) { iterator tmp = end(); erase(--tmp); }
  /// Compare two qlists with '=='
  bool operator==(const qlist<T> &x) const
  {
    if ( length != x.length )
      return false;
    const_iterator q=x.begin();
    for ( const_iterator p=begin(), e=end(); p != e; ++p,++q )
      if ( *p != *q )
        return false;
    return true;
  }
  /// Compare two qlists with !=
  bool operator!=(const qlist<T> &x) const { return !(*this == x); }
private:
  void linkin(iterator p, listnode_t *tmp)
  {
    tmp->next = p.cur;
    tmp->prev = p.cur->prev;
    p.cur->prev->next = tmp;
    p.cur->prev = tmp;
    ++length;
  }
};

typedef qvector<uval_t> uvalvec_t;    ///< vector of unsigned values
typedef qvector<sval_t> svalvec_t;    ///< vector of signed values
typedef qvector<ea_t> eavec_t;        ///< vector of addresses
typedef qvector<int> intvec_t;        ///< vector of integers
typedef qvector<qstring> qstrvec_t;   ///< vector of strings
typedef qvector<qwstring> qwstrvec_t; ///< vector of unicode strings
typedef qvector<bool> boolvec_t;      ///< vector of bools

// Our containers do not care about their addresses. They can be moved around with simple memcpy
/// \cond
template <class T> struct ida_movable_type<qvector<T> >   { typedef ida_true_type is_movable_type; };
template <class T> struct ida_movable_type<_qstring<T> >  { typedef ida_true_type is_movable_type; };
template <class T> struct ida_movable_type<qlist<T> >     { typedef ida_true_type is_movable_type; };
template <class T> struct ida_movable_type<qiterator<T> > { typedef ida_true_type is_movable_type; };
/// \endcond

//-------------------------------------------------------------------------
/// Resource janitor to facilitate use of the RAII idiom
template <typename T>
struct janitor_t
{
  janitor_t(T &r) : resource(r) {} ///< Constructor
  ~janitor_t(); ///< We provide no implementation for this function, you should
                ///< provide specialized implementation yourself
protected:
  T &resource;
};

//-------------------------------------------------------------------------
/// Align element up to nearest boundary
template <class T> T align_up(T val, int elsize)
{
  int mask = elsize - 1;
  val += mask;
  val &= ~mask;
  return val;
}

//-------------------------------------------------------------------------
/// Align element down to nearest boundary
template <class T> T align_down(T val, int elsize)
{
  int mask = elsize - 1;
  val &= ~mask;
  return val;
}

//-------------------------------------------------------------------------
/// \def{DEFINE_VIRTUAL_DTOR,
///      GCC generates multiple destructors and they occupy multiple slots of the
///      virtual function table. Since it makes the vft incompatible with other
///      compilers - we simply never generate virtual destructors for gcc. This is not an
///      ideal solution but it works.
///      We have this problem only under MS Windows. On other platforms everything is
///      compiled with GCC so the vft layout is the same for the kernel and plugins.}
/// \def{DECLARE_VIRTUAL_DTOR, see #DEFINE_VIRTUAL_DTOR}
#if defined(SWIG)
  #define DEFINE_VIRTUAL_DTOR(name)
  #define DECLARE_VIRTUAL_DTOR(name)
#elif defined(__GNUC__) && defined(__NT__)
  #define DEFINE_VIRTUAL_DTOR(name) virtual void idaapi dummy_dtor_for_gcc(void) {}
  #define DECLARE_VIRTUAL_DTOR(name) virtual void idaapi dummy_dtor_for_gcc(void)
#else
  #define DEFINE_VIRTUAL_DTOR(name) virtual idaapi ~name(void) {}
  #define DECLARE_VIRTUAL_DTOR(name) virtual idaapi ~name(void)
#endif

/// Declare class as uncopyable.
/// (copy assignment and copy ctr are undefined, so if anyone calls them,
///  there will be a compilation or link error)
#define DECLARE_UNCOPYABLE(T) T &operator=(const T &); T(const T &);

#endif // __cplusplus

struct hit_counter_t;
idaman void ida_export reg_hit_counter(hit_counter_t *, bool do_reg);


/// Create new ::hit_counter_t with given name

idaman THREAD_SAFE hit_counter_t *ida_export create_hit_counter(const char *name);

idaman THREAD_SAFE void ida_export hit_counter_timer(hit_counter_t *, bool enable);

/// Statistical counter for profiling
struct hit_counter_t
{
  const char *name;     ///< name is owned by hit_counter_t
                        ///< reg_hit_counter() allocates it
  int total, misses;
  uint64 elapsed;       ///< number of elapsed counts
  uint64 stamp;         ///< start time
  hit_counter_t(const char *_name)
    : name(_name), total(0), misses(0), elapsed(0)
    { reg_hit_counter(this, true); }
  virtual ~hit_counter_t(void) { reg_hit_counter(this, false); }
  /// Prints the counter to the message window and resets it
  virtual void print(void);
  // time functions
  void start(void) { hit_counter_timer(this, true); }
  void stop(void) { hit_counter_timer(this, false); }
};

/// Formalized counter increment - see ::hit_counter_t
class incrementer_t
{
  hit_counter_t &ctr;
public:
  incrementer_t(hit_counter_t &_ctr) : ctr(_ctr) { ctr.total++; ctr.start(); }
  ~incrementer_t(void) { ctr.stop(); }
  DEFINE_MEMORY_ALLOCATION_FUNCS()
  void failed(void) { ctr.misses++; }
};

#ifndef UNICODE
#define cwstr(dst, src, dstsize) ::qstrncpy(dst, src, dstsize)
#define wcstr(dst, src, dstsize) ::qstrncpy(dst, src, dstsize)
#endif


/// Encode base64

idaman THREAD_SAFE bool ida_export base64_encode(qstring *output, const void *input, size_t size);


/// Decode base64

idaman THREAD_SAFE bool ida_export base64_decode(bytevec_t *output, const char *input, size_t size); ///< Decode base64


/// Convert tabulations to spaces
/// \param out      output buffer to append to
/// \param str      input string. can not be equal to out->c_str()
/// \param tabsize  tabulation size
/// \returns true-replaced some tabs

idaman THREAD_SAFE bool ida_export replace_tabs(qstring *out, const char *str, int tabsize);


/// Unicode -> char

idaman THREAD_SAFE bool ida_export u2cstr(const wchar16_t *in, qstring *out, int nsyms=-1);


///< Char -> unicode

idaman THREAD_SAFE bool ida_export c2ustr(const char *in, qwstring *out, int nsyms=-1);


/// utf-8 -> 16bit unicode

idaman THREAD_SAFE int ida_export utf8_unicode(const char *in, wchar16_t *out, size_t outsize);


/// 16bit unicode -> utf8

idaman THREAD_SAFE bool ida_export unicode_utf8(qstring *res, const wchar16_t *in, int nsyms);


/// Windows utf-8 (<0xFFFF) -> idb representation (oem)

idaman THREAD_SAFE bool ida_export win_utf2idb(char *buf);


/// Read one utf-8 character from string. if error, return -1

idaman THREAD_SAFE wchar32_t ida_export get_utf8_char(const char **pptr);

#if defined(__NT__) && !defined(UNDER_CE)
/// Do not use windows CharToOem/OemToChar functions - ida can replace CodePage
idaman void ida_export char2oem(char *inout);
idaman void ida_export oem2char(char *inout); ///< \copydoc char2oem
#else
inline void idaapi char2oem(char* /*inout*/) { }
inline void idaapi oem2char(char* /*inout*/) { }
#endif


/// Set codepages to be used for char2oem/oem2char (Windows only).
/// if parameter == -1, use CP_ACP and CP_OEMCP
/// \return false if codepage unsupported

idaman bool ida_export set_codepages(int acp/* = CP_ACP*/, int oemcp/* = CP_OEMCP*/);


/// Get codepages used for char2oem/oem2char (Windows only).
/// \param[out] oemcp  pointer to oem codepage
/// \return current codepage

idaman int ida_export get_codepages(int* oemcp);

#ifdef __NT__
/// Convert data from codepage incp to outcp.
/// Either codepage can be CP_UTF16 for Unicode text (buffer sizes are still in bytes!)
/// flags: 1: convert control characters (0x01-0x1F) to glyphs
/// \param insize  insize == -1: input is null-terminated
/// \return number of bytes after conversion (not counting termination zero)

idaman int ida_export convert_codepage(const void* in, int insize, void* out, size_t outsize, int incp, int outcp, int flags = 0);

#else
inline int idaapi convert_codepage(const void* /*in*/, int /*insize*/, void* /*out*/, size_t /*outsize*/, int /*incp*/, int /*outcp*/, int /*flags*/ = 0) { return 0; }
#endif


/// Convert data from encoding fromcode into tocode.
/// \return number of input bytes converted (can be less than actual size if there was an invalid character)
/// -1 if source or target encoding is not supported
/// possible encoding names: windows codepages ("CP1251" etc), charset names ("Shift-JIS"), and many encodings supported by iconv

idaman int ida_export convert_encoding(const char *fromcode, const char *tocode, const bytevec_t *indata, bytevec_t *out, int flags = 0);

#ifndef CP_UTF8
#define CP_UTF8 65001 ///< utf-8 codepage
#endif

#ifndef CP_UTF16
#define CP_UTF16 1200 ///< utf-16 codepage
#endif

#ifdef __NT__
#ifndef INVALID_FILE_ATTRIBUTES
#define INVALID_FILE_ATTRIBUTES ((DWORD)-1) ///< old Visual C++ compilers were not defining this
#endif
#ifndef BELOW_NORMAL_PRIORITY_CLASS
#define BELOW_NORMAL_PRIORITY_CLASS       0x00004000 ///< \copydoc INVALID_FILE_ATTRIBUTES
#endif
#endif

idaman ida_export_data char SubstChar; ///< default char, used if a char cannot be represented in a codepage

typedef uint32 flags_t;   ///< 32-bit flags for each address
typedef ea_t tid_t;       ///< type id (for enums, structs, etc)

typedef uint32 bgcolor_t;       ///< background color in RGB
#define DEFCOLOR bgcolor_t(-1)  ///< default color (used in function, segment definitions)

//-------------------------------------------------------------------------
// Command line
//-------------------------------------------------------------------------

/// Tools for command line parsing
struct channel_redir_t
{
  int fd;                     ///< channel number
  qstring file;               ///< file name to redirect to/from.
                              ///< if empty, the channel must be closed.
  int flags;                  ///< \ref IOREDIR_
/// \defgroup IOREDIR_ i/o redirection flags
/// used by channel_redir_t::flags
//@{
#define IOREDIR_INPUT  0x01   ///< input redirection
#define IOREDIR_OUTPUT 0x02   ///< output redirection
#define IOREDIR_APPEND 0x04   ///< append, do not overwrite the output file
#define IOREDIR_QUOTED 0x08   ///< the file name was quoted
//@}
  bool is_input(void) const { return (flags & IOREDIR_INPUT) != 0; }
  bool is_output(void) const { return (flags & IOREDIR_OUTPUT) != 0; }
  bool is_append(void) const { return (flags & IOREDIR_APPEND) != 0; }
  bool is_quoted(void) const { return (flags & IOREDIR_QUOTED) != 0; }
  int start;                   ///< begin of the redirection string in the command line
  int length;                  ///< length of the redirection string in the command line
};
typedef qvector<channel_redir_t> channel_redirs_t; ///< vector of channel_redir_t objects


/// Parse a space separated string (escaping with backslash is supported).
/// \param cmdline      the string to be parsed
/// \param[out] args    a string vector to hold the results
/// \param[out] redirs  map of channel redirections found in cmdline
///                        - if NULL, redirections won't be parsed
///                        - if there are syntax errors in redirections, consider them as arguments
/// \param flags        #LP_PATH_WITH_ARGS or 0
/// \return the number of parsed arguments

idaman THREAD_SAFE size_t ida_export parse_command_line3(
        const char *cmdline,
        qstrvec_t *args,
        channel_redirs_t *redirs,
        int flags);


/// Copy and expand command line arguments.
/// For '@filename' arguments the file contents are inserted into the resulting argv.
/// Format of the file: one switch per line, ';' for comment lines
/// \param[out] p_argc  size of the returned argv array
/// \param argc         number of entries in argv array
/// \param argv         array of strings
/// \return new argv (terminated by NULL).
///          It must be freed with free_argv()

idaman char **ida_export expand_argv(int *p_argc, int argc, const char *const argv[]);


/// Free 'argc' elements of 'argv'

inline void free_argv(int argc, char **argv)
{
  if ( argv != NULL )
  {
    for ( int i = 0; i < argc; i++ )
      qfree(argv[i]);
    qfree(argv);
  }
}


/// Quote a command line argument if it contains escape characters.
/// For example, *.c will be converted into "*.c" because * may be inadvertently
/// expanded by the shell
/// \return true: modified 'arg'

idaman bool ida_export quote_cmdline_arg(qstring *arg);


/// Parse the -r command line switch (for instant debugging).
/// r_switch points to the value of the -r switch. Example: win32@localhost+
/// \return true-ok, false-parse error

idaman bool ida_export parse_dbgopts(struct instant_dbgopts_t *ido, const char *r_switch);

/// Options for instant debugging
struct instant_dbgopts_t
{
  qstring debmod;       ///< name of debugger module
  qstring env;          ///< config variables for debmod. example: DEFAULT_CPU=13;MAXPACKETSIZE=-1
  qstring host;         ///< remote hostname (if remote debugging)
  qstring pass;         ///< password for the remote debugger server
  int port;             ///< port number for the remote debugger server
  int pid;              ///< process to attach to (-1: ask the user)
  int event_id;         ///< event to trigger upon attaching
  bool attach;          ///< should attach to a process?
};


//-------------------------------------------------------------------------
// PROCESSES
//-------------------------------------------------------------------------

/// Information for launching a process with IDA API
struct launch_process_params_t
{
  size_t cb;                     ///< size of this structure
  int flags;                     ///< \ref LP_
/// \defgroup LP_ Launch process flags
/// used by launch_process_params_t::flags
//@{
#define LP_NEW_CONSOLE    0x0001 ///< create new console (only ms windows)
#define LP_TRACE          0x0002 ///< debug: unix: ptrace(TRACEME), windows: DEBUG_PROCESS
#define LP_PATH_WITH_ARGS 0x0004 ///< 'args' contains executable path too
#define LP_USE_SHELL      0x0008 ///< use shell to launch the command.
                                 ///< 'path' is ignored in this case.
#define LP_LAUNCH_32_BIT  0x0010 ///< prefer to launch 32-bit part of file (only mac)
#define LP_LAUNCH_64_BIT  0x0020 ///< prefer to launch 64-bit part of file (only mac)
                                 ///< only one of LP_LAUNCH_*_BIT bits can be specified
#define LP_NO_ASLR        0x0040 ///< disable ASLR (only mac)
#define LP_DETACH_TTY     0x0080 ///< detach the current tty (unix)
//@}
  const char *path;              ///< file to run
  const char *args;              ///< command line arguments
  ssize_t in_handle;             ///< handle for stdin or -1
  ssize_t out_handle;            ///< handle for stdout or -1
  ssize_t err_handle;            ///< handle for stderr or -1
  char *env;                     ///< zero separated environment variables that will be appended
                                 ///< to the existing environment block (existing variables will be updated).
                                 ///< each variable has the following form: var=value\0
                                 ///< must be terminated with two zero bytes!
  const char *startdir;          ///< current directory for the new process
  void *info;                    ///< os specific info (on windows it points to PROCESS_INFORMATION)
                                 ///< on unix, not used
  launch_process_params_t(void)  ///< constructor
    : cb(sizeof(*this)), flags(0), path(NULL), args(NULL),
      in_handle(-1), out_handle(-1),  err_handle(-1),
      env(NULL), startdir(NULL), info(NULL) {}
};

/// Launch the specified process in parallel.
/// \return handle (unix: child pid), NULL - error

idaman THREAD_SAFE void *ida_export launch_process(
        const launch_process_params_t &lpp,
        qstring *errbuf);


/// Forcibly terminate a running process.
/// \returns 0-ok, otherwise an error code that can be passed to winerr()

idaman THREAD_SAFE int ida_export term_process(void *handle);


/// Wait for state changes in a child process (UNIX only).
/// Here: child, status, flags - the same as in system call waitpid()
/// \param timeout_ms  timeout in milliseconds

idaman THREAD_SAFE int ida_export qwait_timed(int child, int *status, int flags, int timeout_ms);

#if defined(__UNIX__)
#  ifdef WCONTINUED
#    define QWCONTINUED WCONTINUED
#  else
#    define QWCONTINUED 8
#  endif
#  ifdef WNOHANG
#    define QWNOHANG WNOHANG
#  else
#    define QWNOHANG 1
#  endif
inline int qwait(int child, int *status, int flags)
{
  return qwait_timed(child, status, flags, (flags & QWNOHANG) != 0 ? 0 : -1);
}
#endif


/// Check whether process has terminated or not.
/// \param handle          process handle to wait for
/// \param[out] exit_code  pointer to the buffer for the exit code
/// \param msecs           how long to wait. special values:
///                          - 0: do not wait
///                          - 1 or -1: wait infinitely
///                          - other values: timeout in milliseconds
/// \retval 0   process has exited, and the exit code is available.
///             if *exit_code < 0: the process was killed with a signal -*exit_code
/// \retval 1   process has not exited yet
/// \retval -1  error happened, see error code for winerr() in *exit_code

idaman THREAD_SAFE int ida_export check_process_exit(
        void *handle,
        int *exit_code,
        int msecs=-1);

/// Teletype control
enum tty_control_t
{
  TCT_UNKNOWN = 0,
  TCT_OWNER,
  TCT_NOT_OWNER
};


/// Check if the current process is the owner of the TTY specified
/// by 'fd' (typically an opened descriptor to /dev/tty).

idaman THREAD_SAFE tty_control_t ida_export is_control_tty(int fd);


/// If the current terminal is the controlling terminal of the calling
/// process, give up this controlling terminal.
/// \note The current terminal is supposed to be /dev/tty

idaman THREAD_SAFE void ida_export qdetach_tty(void);


/// Make the current terminal the controlling terminal of the calling
/// process.
/// \note The current terminal is supposed to be /dev/tty

idaman THREAD_SAFE void ida_export qcontrol_tty(void);

//-------------------------------------------------------------------------
/// THREADS
//-------------------------------------------------------------------------

/// Thread callback function
typedef int (idaapi *qthread_cb_t)(void *ud);

/// Thread opaque handle
typedef struct __qthread_t {} *qthread_t;


/// Create a thread and return a thread handle

idaman THREAD_SAFE qthread_t ida_export qthread_create(qthread_cb_t thread_cb, void *ud);


/// Free a thread resource (does not kill the thread)

idaman THREAD_SAFE void ida_export qthread_free(qthread_t q);


/// Wait a thread until it terminates

idaman THREAD_SAFE bool ida_export qthread_join(qthread_t q);


/// Forcefully kill a thread (calls pthread_cancel under unix)

idaman THREAD_SAFE bool ida_export qthread_kill(qthread_t q);


/// Get current thread. Must call qthread_free() to free it!

idaman THREAD_SAFE qthread_t ida_export qthread_self(void);


/// Is the current thread the same as 'q'?

idaman THREAD_SAFE bool ida_export qthread_same(qthread_t q);


/// Are we running in the main thread?

idaman THREAD_SAFE bool ida_export is_main_thread(void);


/// Thread safe function to work with the environment

idaman THREAD_SAFE bool ida_export qsetenv(const char *varname, const char *value);
idaman THREAD_SAFE bool ida_export qgetenv(const char *varname, qstring *buf=NULL); ///< \copydoc qsetenv


//-------------------------------------------------------------------------
/// Semaphore.
/// Named semaphores are public, nameless ones are local to the process
//-------------------------------------------------------------------------
typedef struct __qsemaphore_t {} *qsemaphore_t;
idaman THREAD_SAFE qsemaphore_t ida_export qsem_create(const char *name, int init_count);   ///< Create a new semaphore
idaman THREAD_SAFE bool ida_export qsem_free(qsemaphore_t sem);                             ///< Free a semaphore
idaman THREAD_SAFE bool ida_export qsem_post(qsemaphore_t sem);                             ///< Unlock a semaphore
idaman THREAD_SAFE bool ida_export qsem_wait(qsemaphore_t sem, int timeout_ms);             ///< Lock and decrement a semaphore. timeout = -1 means block indefinitely

//-------------------------------------------------------------------------
/// Mutex
//-------------------------------------------------------------------------
typedef struct __qmutex_t {} *qmutex_t;
idaman THREAD_SAFE bool ida_export qmutex_free(qmutex_t m);      ///< Free a mutex
idaman THREAD_SAFE qmutex_t ida_export qmutex_create();          ///< Create a new mutex
idaman THREAD_SAFE bool ida_export qmutex_lock(qmutex_t m);      ///< Lock a mutex
idaman THREAD_SAFE bool ida_export qmutex_unlock(qmutex_t m);    ///< Unlock a mutex

/// Mutex locker object. Will lock a given mutex upon creation and unlock it when the object is destroyed
class qmutex_locker_t
{
  qmutex_t lock;
public:
  qmutex_locker_t(qmutex_t _lock) : lock(_lock) { qmutex_lock(lock); }
  ~qmutex_locker_t(void) { qmutex_unlock(lock); }
};

//-------------------------------------------------------------------------
//  PIPES
//-------------------------------------------------------------------------
#ifdef __NT__
typedef void *qhandle_t;        ///< MS Windows HANDLE
#else
typedef int qhandle_t;          ///< file handle in Unix
#endif


/// Create a pipe.
/// \param[out] handles
///               - handles[0] : read handle
///               - handles[1] : write handle
/// \return error code (0-ok)

idaman THREAD_SAFE int ida_export qpipe_create(qhandle_t handles[2]);


/// Read from a pipe. \return number of read bytes. -1-error

idaman THREAD_SAFE ssize_t ida_export qpipe_read(qhandle_t handle, void *buf, size_t size);


/// Write to a pipe. \return number of written bytes. -1-error

idaman THREAD_SAFE ssize_t ida_export qpipe_write(qhandle_t handle, const void *buf, size_t size);


/// Close a pipe. \return error code (0-ok)

idaman THREAD_SAFE int ida_export qpipe_close(qhandle_t handle);


/// Wait for file/socket/pipe handles.
/// \param handles        handles to wait for
/// \param n              number of handles
/// \param write_bitmask  bitmask of indexes of handles opened for writing
/// \param idx            handle index
/// \param timeout_ms     timeout value in milliseconds
/// \return error code. on timeout, returns 0 and sets idx to -1

idaman THREAD_SAFE int ida_export qwait_for_handles(
        const qhandle_t *handles,
        int n,
        uint32 write_bitmask,
        int *idx,
        int timeout_ms);



#ifndef NO_OBSOLETE_FUNCS
idaman DEPRECATED THREAD_SAFE size_t ida_export parse_command_line(const char *cmdline, qstrvec_t *args);
idaman DEPRECATED THREAD_SAFE size_t ida_export parse_command_line2(const char *cmdline, qstrvec_t *args, channel_redirs_t *redirs);
idaman DEPRECATED char *ida_export qsplitpath(char *path, char **dir, char **file);
idaman DEPRECATED void *ida_export init_process(const launch_process_params_t &lpi, qstring *errbuf);
idaman DEPRECATED THREAD_SAFE int ida_export get_process_exit_code(void *handle, int *exit_code);
idaman DEPRECATED NORETURN void ida_export vinterr(const char *file, int line, const char *format, va_list va);
idaman DEPRECATED error_t ida_export_data qerrno;
#if !defined(__BORLANDC__) && !defined(_MSC_VER)
idaman DEPRECATED THREAD_SAFE char *ida_export strlwr(char *s);
idaman DEPRECATED THREAD_SAFE char *ida_export strupr(char *s);
#endif
#define launch_process_t launch_process_params_t ///< for convenience
#ifndef __GNUC__
typedef uint32 ulong; ///< unsigned 32 bit value
#endif
#if defined(__VC__) && defined(__X64__)
#define __VC64__
#define qstat _stat64                   // use qstat64()
#define qfstat _fstat64                 // use qfstat64()
#define qstatbuf struct __stat64
#else
#define qstat stat
#define qfstat fstat
#define qstatbuf struct stat
#endif
#endif

#pragma pack(pop)
#endif /* _PRO_H */
