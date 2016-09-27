/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2015 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 */

#ifndef __FPRO_H
#define __FPRO_H

#include <pro.h>
#include <stdio.h>

#pragma pack(push, 1)

/*! \file fpro.h

  \brief System independent counterparts of FILE* related functions from Clib

  You should not use C standard I/O functions in your modules.
  The reason: Each module compiled with Borland
  (and statically linked to Borland's library) will host a copy of
  the FILE * information.

  So, if you open a file in the plugin and pass the handle to the
  kernel, the kernel will not be able to use it.

  If you really need to use the standard functions, define USE_STANDARD_FILE_FUNCTIONS.
  In this case do not mix them with q... functions.
*/

#if !defined(USE_STANDARD_FILE_FUNCTIONS) && !defined(_lint)
#undef stdin
#undef stdout
#undef stderr
#undef fgetc
#undef fputc
#define stdin      dont_use_stdin     ///< use gets()
#define stdout     dont_use_stdout    ///< use qprintf()
#define stderr     dont_use_stderr    ///< use qeprintf()
#define fopen      dont_use_fopen     ///< use qfopen()
#define fread      dont_use_fread     ///< use qfread()
#define fwrite     dont_use_fwrite    ///< use qfwrite()
#define ftell      dont_use_ftell     ///< use qftell()
#define fseek      dont_use_fseek     ///< use qfseek()
#define fclose     dont_use_fclose    ///< use qfclose()
#define fflush     dont_use_fflush    ///< use qflush()
#define fputc      dont_use_fputc     ///< use qfputc()
#define fgetc      dont_use_fgetc     ///< use qfgetc()
#define fgets      dont_use_fgets     ///< use qfgets()
#define fputs      dont_use_fputs     ///< use qfputs()
#define vfprintf   dont_use_vfprintf  ///< use qvfprintf()
#define vfscanf    dont_use_vfscanf   ///< use qvfscanf()
#define fprintf    dont_use_fprintf   ///< use qfprintf()
#define fscanf     dont_use_fscanf    ///< use qfscanf()
#endif


/// \name File I/O
/// The following functions work just like their counterparts from Clib,
/// only they are safer, system independent, and they set qerrno (see get_qerrno()).
//@{
idaman THREAD_SAFE FILE *ida_export qfopen(const char *file, const char *mode);
idaman THREAD_SAFE int   ida_export qfread(FILE *fp, void *buf, size_t n);
idaman THREAD_SAFE int   ida_export qfwrite(FILE *fp, const void *buf, size_t n);
idaman THREAD_SAFE int32 ida_export qftell(FILE *fp);
idaman THREAD_SAFE int   ida_export qfseek(FILE *fp, int32 offset, int whence);
idaman THREAD_SAFE int   ida_export qfclose(FILE *fp);
idaman THREAD_SAFE int   ida_export qflush(FILE *fp);
idaman THREAD_SAFE int   ida_export qfputc(int chr, FILE *fp);
idaman THREAD_SAFE int   ida_export qfgetc(FILE *fp);
idaman THREAD_SAFE char *ida_export qfgets(char *s, size_t len, FILE *fp);
idaman THREAD_SAFE int   ida_export qfputs(const char *s, FILE *fp);
idaman             FILE *ida_export qtmpfile(void);
idaman THREAD_SAFE int   ida_export qunlink(const char *fname);
idaman THREAD_SAFE char *ida_export qgets(char *line, size_t linesize);

idaman THREAD_SAFE AS_PRINTF(2, 0) int ida_export qvfprintf(FILE *fp, const char *format, va_list va);
idaman THREAD_SAFE AS_PRINTF(1, 0) int ida_export qvprintf(const char *format, va_list va);
idaman THREAD_SAFE AS_PRINTF(1, 0) int ida_export qveprintf(const char *format, va_list va);
idaman THREAD_SAFE AS_SCANF (2, 0) int ida_export qvfscanf(FILE *fp, const char *format, va_list va);

#ifdef __cplusplus
THREAD_SAFE AS_PRINTF(2, 3) inline int qfprintf(FILE *fp, const char *format, ...)
{
  va_list va;
  va_start(va, format);
  int code = qvfprintf(fp, format, va);
  va_end(va);
  return code;
}

THREAD_SAFE AS_PRINTF(1, 2) inline int qprintf(const char *format, ...)
{
  va_list va;
  va_start(va, format);
  int code = qvprintf(format, va);
  va_end(va);
  return code;
}

THREAD_SAFE AS_PRINTF(1, 2) inline int qeprintf(const char *format, ...)
{
  va_list va;
  va_start(va, format);
  int code = qveprintf(format, va);
  va_end(va);
  return code;
}

THREAD_SAFE AS_SCANF(2, 3) inline int qfscanf(FILE *fp, const char *format, ...)
{
  va_list va;
  va_start(va, format);
  int code = qvfscanf(fp, format, va);
  va_end(va);
  return code;
}
#endif
//@}


/// Rename a file: 'newname' may exist, and will be deleted

idaman THREAD_SAFE int ida_export qrename(const char *oldfname, const char *newfname);


/// Copy a file.
/// \param from       source file name
/// \param to         destination file name
/// \param overwrite  overwrite output if it exists?
/// \param cb         user callback. return false to abort the copy loop
/// \param ud         user data passed back to cb
/// \param flags      reserved (should be zero)
/// \retval -1  input file not found
/// \retval -2  output file not writable
/// \retval -3  output file already exists while overwrite is false
/// \retval -4  write failure
/// \retval -5  interrupted from the callback

idaman THREAD_SAFE int   ida_export qcopyfile(
        const char *from,
        const char *to,
        bool overwrite = true,
        bool (idaapi *cb)(size_t pos, size_t total, void *ud) = NULL,
        void *ud = NULL,
        int flags = 0);


/// Get temporary file name.
/// Returns absolute path, includes directory, and uses TEMP/TMP vars.

idaman char *ida_export qtmpnam(char *buf, size_t bufsize);


/// File janitor: will close a file upon deletion
typedef janitor_t<FILE*> file_janitor_t;
template <> inline file_janitor_t::~janitor_t()
{
  qfclose(resource);
}

/// \name readbytes/writebytes
/// Add-ins for 2..32 byte read/writes.
/// \param fp         pointer to file
/// \param res        value read from file
/// \param size       size of value in bytes (1..32)
/// \param mostfirst  is MSB first? (0/1)
/// \retval 0  All these functions return 0 on success
//@{
idaman THREAD_SAFE int ida_export freadbytes(FILE *fp,void *res,int size,int mostfirst);
idaman THREAD_SAFE int ida_export fwritebytes(FILE *fp,const void *l,int size,int mostfirst);

#ifdef __cplusplus
#define DEF_FREADBYTES(read, write, type, size)                         \
        inline int read(FILE *fp, type *res, bool mostfirst)            \
                { return freadbytes(fp, res, size, mostfirst); }        \
        inline int write(FILE *fp, const type *res, bool mostfirst)     \
                { return fwritebytes(fp, res, size, mostfirst); }
DEF_FREADBYTES(fread2bytes, fwrite2bytes, int16, 2)
DEF_FREADBYTES(fread2bytes, fwrite2bytes, uint16, 2)
DEF_FREADBYTES(fread4bytes, fwrite4bytes, int32, 4)
DEF_FREADBYTES(fread4bytes, fwrite4bytes, uint32, 4)
DEF_FREADBYTES(fread8bytes, fwrite8bytes, longlong, 8)
DEF_FREADBYTES(fread8bytes, fwrite8bytes, ulonglong, 8)
#else
#define fread2bytes(fp,v,mf)  freadbytes(fp,v,2,mf)
#define fwrite2bytes(fp,v,mf) fwritebytes(fp,v,2,mf)
#define fread4bytes(fp,v,mf)  freadbytes(fp,v,4,mf)
#define fwrite4bytes(fp,v,mf) fwritebytes(fp,v,4,mf)
#define fread8bytes(fp,v,mf)  freadbytes(fp,v,8,mf)
#define fwrite8bytes(fp,v,mf) fwritebytes(fp,v,8,mf)
#endif
//@}

/// \name Large file support
/// The following functions work just like their counterparts,
/// but support large files (>2Gb).
//@{
idaman THREAD_SAFE qoff64_t ida_export qftell64(FILE *fp);
idaman THREAD_SAFE int      ida_export qfseek64(FILE *fp, qoff64_t offset, int whence);
idaman THREAD_SAFE int      ida_export qcopyfile64(
  const char *from,
  const char *to,
  bool overwrite = true,
  bool (idaapi *cb)(uint64 pos, uint64 total, void *ud) = NULL,
  void *ud = NULL,
  int flags = 0);
//@}

#if !defined(feof) || !defined(ferror)
// If feof() and ferror() are not macros, we can not use them
// Fortunately, for borland and vc they are macros, so there is no problem
// GCC defines them as functions: I have no idea whether they will work or not
// Anyway we remove the error directive from this file
// so the plugins can be compiled with gcc
//#error  feof or ferror are not macros!
#endif



#pragma pack(pop)
#endif
