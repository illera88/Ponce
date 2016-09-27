/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2015 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 */

#ifndef _PRODIR_H
#define _PRODIR_H
#pragma pack(push, 1)

/*! \file prodir.h

  \brief Unified interface to qfindfirst64(),qfindnext64(),qfindclose64() functions.

  These are low level functions, it is better to use enumerate_files().
*/

/// \def{DIRCHAR,  Path separator}
/// \def{SDIRCHAR, Path separator as a string}
/// \def{DRVCHAR,  Windows drive separator}
#if defined(__MSDOS__) || defined(__OS2__) || defined(__NT__)
#define __FAT__
#define SDIRCHAR "\\"
#define DIRCHAR '\\'
#define DRVCHAR ':'
#else
#define SDIRCHAR "/"
#define DIRCHAR '/'
#endif

/// Extension character is '.' for all systems
#define EXTCHAR '.'

//----------------------------------------------------------------------------
/// \struct{qffblk_t,             Various file statistics returned by qfind()-like functions}
///    \var{qffblk_t::ff_name,    file path}
///    \var{qffblk_t::ff_fsize,   file size}
///    \var{qffblk_t::ff_attrib,  file attribute}
///    \var{qffblk_t::ff_ftime,   file time stamp (ms dos fat format)}
///    \var{qffblk_t::ff_fdate,   file date stamp (ms dos fat format)}
/// \def{FA_RDONLY, File cannot be opened for writing}
/// \def{FA_DIREC,  Directory}
/// \def{FA_ARCH,   File has not been backed up}
/// \def{MAXPATH,   Size limit of qffblk_t::ff_name}
#if defined(__UNIX__)
  #define MAXPATH   QMAXPATH
  struct qffblk_t                        // Unix
  {
    // user fields:
    int ff_attrib;
      #define FA_DIREC S_IFDIR
      #define FA_ARCH   0
      #define FA_RDONLY 0
    char ff_name[QMAXPATH];
    uint32 ff_fsize;
    uint16 ff_fdate;
    uint16 ff_ftime;
    // private fields:
    void *filelist;
    int fileidx, fileqty;
    char dirpath[QMAXPATH];
    char pattern[QMAXPATH];
    int attr;
    qffblk_t(void) : filelist(NULL), fileqty(0) {}
  };
#elif defined (__X64__)
  #define MAXPATH   _MAX_PATH
  struct qffblk_t : public __finddata64_t  // Win64 - use Visual Studio's ffblk
  {
    intptr_t handle;
    int attr;
      #define FA_RDONLY   0x01
      #define FA_DIREC    0x10
      #define FA_ARCH     0x20
    #define ff_name   name
    #define ff_attrib attrib
    #define ff_fsize  size
    unsigned short ff_ftime;
    unsigned short ff_fdate;
    qffblk_t(void) : handle(-1) {}
  };
#elif defined(UNDER_CE)
  #include <windows.h>
  #define MAXPATH   MAX_PATH
  struct qffblk_t : public WIN32_FIND_DATA // WinCE
  {
    HANDLE handle;
    char ff_name[QMAXPATH];
    #define ff_attrib           dwFileAttributes
      #define FA_RDONLY   0x01
      #define FA_DIREC    0x10
      #define FA_ARCH     0x20
    #define ff_fsize            nFileSizeLow
    unsigned short ff_ftime;
    unsigned short ff_fdate;
    int attr;
    qffblk_t(void) : handle(INVALID_HANDLE_VALUE) {}
  };
#else
  #define MAXPATH   260
  struct qffblk_t                       // Win32 - use Borland's ffblk
  {
    long           ff_reserved;
    long           ff_fsize;
    unsigned long  ff_attrib;
      #define FA_RDONLY   0x01
      #define FA_DIREC    0x10
      #define FA_ARCH     0x20
    unsigned short ff_ftime;
    unsigned short ff_fdate;
    char           ff_name[MAXPATH];
    qffblk_t(void) : ff_reserved(0) {}
  };
#endif

/// \def{MAXDRIVE, Max drive name size}
/// \def{MAXDIR,   Max directory name size}
/// \def{MAXFILE,  Max file name size}
/// \def{MAXEXT,   Max file extension size}
#if defined(__UNIX__)
  #define MAXDRIVE              QMAXPATH
  #define MAXDIR                QMAXPATH
  #define MAXFILE               QMAXPATH
  #define MAXEXT                QMAXPATH
#elif !defined(__BORLANDC__)
  #define MAXDRIVE              _MAX_DRIVE
  #define MAXDIR                _MAX_DIR
  #define MAXFILE               _MAX_FNAME
  #define MAXEXT                _MAX_EXT
#endif

/// Find first file that matches the pattern.
/// \param pattern  file name pattern, usually with * and ? wildcards
/// \param blk      structure that will hold the answer.
///                 blk->ff_name will hold the file name, for example.
/// \param attr     the desired file types (#FA_DIREC or 0)
/// \return 0 if found a file, other values mean error

idaman THREAD_SAFE int ida_export qfindfirst64(
        const char *pattern,
        struct qffblk64_t *blk,
        int attr);


/// Find next file that matches the pattern.
/// \param blk  structure that holds the current state.
///             blk->ff_name will hold the next file name upon return.
/// \return 0 if found the next file, other values mean error

idaman THREAD_SAFE int ida_export qfindnext64(struct qffblk64_t *blk);

/// Stop the file enumeration and free internal structures.
/// \note there is no need to call this function manually, it is called
///       from the ::qffblk64_t destructor.
/// \param blk  file enumeration structure

idaman THREAD_SAFE void ida_export qfindclose64(struct qffblk64_t *blk);


/// Common structure with 64-bit ff_fsize for 64-bit dir functions - see ::qffblk_t.
struct qffblk64_t
{
  int ff_attrib;
  char ff_name[QMAXPATH];
  uint64 ff_fsize;
  uint16 ff_fdate;
  uint16 ff_ftime;
  // private field
  struct qffblk_t base;

  qffblk64_t(void) {}
  ~qffblk64_t(void) { qfindclose64(this); }
};


#ifndef NO_OBSOLETE_FUNCS
#define ffblk                    qffblk_t
#define findfirst(file,blk,attr) qfindfirst(file,blk,attr)
#define findnext(blk)            qfindnext(blk)
#define findclose(blk)           qfindclose(blk)
idaman DEPRECATED THREAD_SAFE int ida_export qfindfirst(const char *pattern, qffblk_t *blk, int attr);
idaman DEPRECATED THREAD_SAFE int ida_export qfindnext(qffblk_t *blk);
idaman DEPRECATED THREAD_SAFE void ida_export qfindclose(qffblk_t *blk);
#endif

#pragma pack(pop)
#endif // _PRODIR_H
