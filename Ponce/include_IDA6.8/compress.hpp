
#ifndef COMPRESS_HPP
#define COMPRESS_HPP

#include <diskio.hpp>

#pragma pack(push, 1)

/*! \file compress.hpp

  \brief Data compression functions

*/

/// Compress data.
/// This function depends on the value of legacy_idb, so it is not completely
/// thread safe. However, legacy_idb does not change its value.

idaman THREAD_SAFE int ida_export zip_deflate(
        void *ud,
        ssize_t (idaapi *file_reader)(void *ud, void *buf, size_t size),
        ssize_t (idaapi *file_writer)(void *ud, const void *buf, size_t size));


/// Uncompress data.
/// This function depends on the value of legacy_idb, so it is not completely
/// thread safe. However, legacy_idb does not change its value.

idaman THREAD_SAFE int ida_export zip_inflate(
        void *ud,
        ssize_t (idaapi *file_reader)(void *ud, void *buf, size_t size),
        ssize_t (idaapi *file_writer)(void *ud, const void *buf, size_t size));


/// Process zip file and enumerate all files stored in it
/// \param zipfile    name of zip file
/// \param _callback  callback for each file. params:
///                     - ud:          user data
///                     - offset:      offset in the zip file
///                     - method:      compression method (\ref compression_methods)
///                     - csize:       compressed size
///                     - ucsize:      uncompressed size
///                     - attributes:  file attributes
/// \param ud         user data
/// \return \ref PKZ_

idaman THREAD_SAFE int ida_export process_zipfile(
        const char *zipfile,
        int (idaapi *_callback)(
                         void *ud,
                         int32 offset,
                         int method,
                         uint32 csize,
                         uint32 ucsize,
                         uint32 attributes,
                         const char *filename),
        void *ud);


/// Search for specified entry in zip file, and calls the
/// callback with it, if found.
/// \param zipfile    name of zip file
/// \param entry      entry in zip file. E.g., "path/to/entry.dat"
/// \param _callback  callback for each file. params:
///                     - ud:          user data
///                     - offset:      offset in the zip file
///                     - method:      compression method (\ref compression_methods)
///                     - csize:       compressed size
///                     - ucsize:      uncompressed size
///                     - attributes:  file attributes
/// \param ud         user data
/// \param case_sensitive  should the search be case sensitive?
/// \return \ref PKZ_

idaman THREAD_SAFE int ida_export process_zipfile_entry(
        const char *zipfile,
        const char *entry,
        int (idaapi *_callback)(
                         void *ud,
                         int32 offset,
                         int method,
                         uint32 csize,
                         uint32 ucsize,
                         uint32 attributes,
                         const char *filename),
        void *ud,
        bool case_sensitive = true);


/// See process_zipfile()

idaman THREAD_SAFE int ida_export process_zipfile64(
        const char *zipfile,
        int (idaapi *callback)(
                         void *ud,
                         qoff64_t offset,
                         int method,
                         uint64 csize,
                         uint64 ucsize,
                         uint32 attributes,
                         const char *filename),
        void *ud);


/// See process_zipfile_entry()

idaman THREAD_SAFE int ida_export process_zipfile_entry64(
        const char *zipfile,
        const char *entry,
        int (idaapi *callback)(
                         void *ud,
                         qoff64_t offset,
                         int method,
                         uint64 csize,
                         uint64 ucsize,
                         uint32 attributes,
                         const char *filename),
        void *ud,
        bool case_sensitive = true);

/// \defgroup PKZ_ Compression error codes
/// Returned by functions in compress.hpp
//@{
#define PKZ_OK            0
#define PKZ_ERRNO         1
#define PKZ_STREAM_ERROR  2
#define PKZ_DATA_ERROR    3
#define PKZ_MEM_ERROR     4
#define PKZ_BUF_ERROR     5
#define PKZ_VERSION_ERROR 6
#define PKZ_RERR          777   // read error
#define PKZ_WERR          778   // write error
//@}

/// \defgroup compression_methods Compression methods
/// passed as 'method' parameter to callback functions in compress.hpp
//@{
#define STORED            0
#define SHRUNK            1
#define REDUCED1          2
#define REDUCED2          3
#define REDUCED3          4
#define REDUCED4          5
#define IMPLODED          6
#define TOKENIZED         7
#define DEFLATED          8
#define NUM_METHODS       9    /* index of last method + 1 */
//@}

extern bool legacy_idb;         ///< for old idb files

/// Upon closing outer linput, perform one of these actions
enum linput_close_code_t
{
  LOC_CLOSE,    ///< close the inner linput
  LOC_UNMAKE,   ///< unmake the inner linput
  LOC_KEEP,     ///< do nothing
};


/// Create a linput to read a compressed input stream
/// \param in      linput with compressed data, seeked to the stream beginning
/// \param insize  size of compressed data. -1 - unknown
/// \param loc     what to do upon closing the resulting linput
/// \return linput that can be used to read uncompressed data.
///          NULL if any error (no more linput descriptors).

idaman THREAD_SAFE linput_t *ida_export create_zip_linput(
        linput_t *in,
        ssize_t insize=-1,
        linput_close_code_t loc=LOC_CLOSE);

#pragma pack(pop)
#endif
