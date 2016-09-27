/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2015 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 */

#ifndef _LOADER_HPP
#define _LOADER_HPP
#include <ida.hpp>
#pragma pack(push, 1)           // IDA uses 1 byte alignments!

/*! \file loader.hpp

  \brief Definitions of IDP, LDR, PLUGIN module interfaces

  This file also contains:
    - functions to load files into the database
    - functions to generate output files
    - high level functions to work with the database (open, save, close)

  The LDR interface consists of one structure: loader_t      \n
  The IDP interface consists of one structure: processor_t   \n
  The PLUGIN interface consists of one structure: plugin_t

  Modules can't use standard FILE* functions.
  They must use functions from <fpro.h>

  Modules can't use standard memory allocation functions.
  They must use functions from <pro.h>

  The exported entry #1 in the module should point to the
  the appropriate structure. (loader_t for LDR module, for example)
*/

//----------------------------------------------------------------------
//              DEFINITION OF LDR MODULES
//----------------------------------------------------------------------

class linput_t;         // loader input source. see diskio.hpp for the functions
struct extlang_t;       // check expr.hpp

/// Max number of chars to describe a file format
#define MAX_FILE_FORMAT_NAME    64

/// Loader description block - must be exported from the loader module
struct loader_t
{
  uint32 version;        ///< api version, should be #IDP_INTERFACE_VERSION
  uint32 flags;          ///< \ref LDRF_
/// \defgroup LDRF_ Loader flags
/// Used by loader_t::flags
//@{
#define LDRF_RELOAD  0x0001     ///< loader recognizes #NEF_RELOAD flag
//@}

  /// Check input file format.
  /// This function will be called many times till it returns !=0.
  /// \param li input file
  /// \param[out] fileformat name of file format
  /// \param n  initially == 0 for each loader, incremented after each call
  /// \return
  /// 1 if file format is recognized, and fills 'fileformatname', otherwise returns 0.
  /// This function may return a unique file format number instead of 1.
  /// To get this unique number, please contact the author.
  /// If the return value is ORed with #ACCEPT_FIRST, then this format
  /// should be placed first in the "load file" dialog box
  int (idaapi *accept_file)(linput_t *li,
                            char fileformatname[MAX_FILE_FORMAT_NAME],
                            int n);

/// Specify that a file format should be place first in "load file" dialog box.
/// See loader_t::accept_file
#define ACCEPT_FIRST    0x8000

  /// Load file into the database.
  /// \param fp              pointer to file positioned at the start of the file
  /// \param fileformatname  name of type of the file
  ///                        (it was returned by #accept_file)
  /// \param neflags         \ref NEF_
  ///
  /// If this function fails, loader_failure() should be called
  void (idaapi *load_file)(linput_t *li,
                           ushort neflags,
                           const char *fileformatname);
/// \defgroup NEF_ Load file flags
/// Passed as 'neflags' parameter to loader_t::load_file
//@{
#define NEF_SEGS        0x0001            ///< Create segments
#define NEF_RSCS        0x0002            ///< Load resources
#define NEF_NAME        0x0004            ///< Rename entries
#define NEF_MAN         0x0008            ///< Manual load
#define NEF_FILL        0x0010            ///< Fill segment gaps
#define NEF_IMPS        0x0020            ///< Create import segment
#ifndef NO_OBSOLETE_FUNCS
#define NEF_TIGHT       0x0040            ///< Don't align segments (OMF)
#endif
#define NEF_FIRST       0x0080            ///< This is the first file loaded
                                          ///< into the database.
#define NEF_CODE        0x0100            ///< for load_binary_file():
                                          ///<   load as a code segment
#define NEF_RELOAD      0x0200            ///< reload the file at the same place:
                                          ///<   - don't create segments
                                          ///<   - don't create fixup info
                                          ///<   - don't import segments
                                          ///<   - etc
                                          ///< load only the bytes into the base.
                                          ///< a loader should have #LDRF_RELOAD
                                          ///< bit set
#define NEF_FLAT        0x0400            ///< Autocreate FLAT group (PE)
#define NEF_MINI        0x0800            ///< Create mini database (do not copy
                                          ///< segment bytes from the input file;
                                          ///< use only the file header metadata)
#define NEF_LOPT        0x1000            ///< Display additional loader options dialog
#define NEF_LALL        0x2000            ///< Load all segments without questions
//@}

  /// Create output file from the database.
  /// This function may be absent.
  /// If fp == NULL, then this function returns:
  ///                 - 0: can't create file of this type
  ///                 - 1: ok, can create file of this type
  ///
  /// If fp != NULL, then this function should create the output file
  int (idaapi *save_file)(FILE *fp, const char *fileformatname);

  /// Take care of a moved segment (fix up relocations, for example).
  /// This function may be absent.
  /// A special calling method \code move_segm(BADADDR, delta, 0, formatname); \endcode
  /// means that the whole program has been moved in the memory (rebased) by delta bytes
  /// \param  from            previous linear address of the segment
  /// \param  to              current linear address of the segment
  /// \param  size            size of the moved segment
  /// \param  fileformatname  the file format
  /// \retval 1  ok
  /// \retval 0  failure
  int (idaapi *move_segm)(ea_t from,
                          ea_t to,
                          asize_t size,
                          const char *fileformatname);

  // This used to be "init_loader_options", but because no-one
  // calls that function, it is now marked as deprecated.
  void *_UNUSED1_was_init_loader_options;
};


/// See loader_failure()

idaman AS_PRINTF(1, 0) NORETURN void ida_export vloader_failure(const char *format, va_list va);


/// Display a message about a loader failure and stop the loading process.
/// The kernel will destroy the database.
/// If format == NULL, no message will be displayed
/// This function does not return (it longjumps)!
/// It may be called only from loader_t::load_file

AS_PRINTF(1, 2) NORETURN inline void loader_failure(const char *format=NULL, ...)
{
  va_list va;
  va_start(va, format);
  vloader_failure(format, va);
#ifdef __BORLANDC__
  // NOTREACHED
  va_end(va);
#endif
}

//----------------------------------------------------------------------
/// \def{LOADER_EXT, Loader module filename extension}
#ifdef __NT__
#ifdef __EA64__
#ifdef __X64__
#define LOADER_EXT "lx64"
#else
#define LOADER_EXT "l64"
#endif
#else
#define LOADER_EXT "ldw"
#endif
#endif

#ifdef __LINUX__
#ifdef __EA64__
#ifdef __X64__
#define LOADER_EXT "lx64"
#else
#define LOADER_EXT "llx64"
#endif
#else
#define LOADER_EXT "llx"
#endif
#endif

#ifdef __MAC__
#ifdef __EA64__
#ifdef __X64__
#define LOADER_EXT "lx64"
#else
#define LOADER_EXT "lmc64"
#endif
#else
#define LOADER_EXT "lmc"
#endif
#endif

#ifdef __BSD__
#ifdef __EA64__
#define LOADER_EXT "lbsd64"
#else
#define LOADER_EXT "lbsd"
#endif
#endif

/// \def{LOADER_DLL, Pattern to find loader files}
#ifdef __EA64__
#define LOADER_DLL "*64." LOADER_EXT
#else
#define LOADER_DLL "*." LOADER_EXT
#endif

//----------------------------------------------------------------------
//      Functions for the UI to load files
//----------------------------------------------------------------------
/// List of loaders
struct load_info_t
{
  load_info_t *next;
  char dllname[QMAXPATH];
  char ftypename[MAX_FILE_FORMAT_NAME];
  filetype_t ftype;
  int pri;              ///< 1-place first, 0-normal priority
};

/// Build list of potential loaders

idaman load_info_t *ida_export build_loaders_list(linput_t *li);


/// Free the list of loaders

idaman void ida_export free_loaders_list(load_info_t *list);


/// Get name of loader from its DLL file
/// (for example, for PE files we will get "PE").
/// This function modifies the original string and returns a pointer into it.
/// NB: if the file extension is a registered extlang extension (e.g. py or idc)
/// the extension is retained

idaman char *ida_export get_loader_name_from_dll(char *dllname);


/// Get name of loader used to load the input file into the database.
/// If no external loader was used, returns -1.
/// Otherwise copies the loader file name without the extension in the buf
/// and returns its length
/// (for example, for PE files we will get "PE").
/// For scripted loaders, the file extension is retained.

idaman ssize_t ida_export get_loader_name(char *buf, size_t bufsize);


/// Initialize user configurable options from the given loader
/// based on the input file.

idaman bool ida_export init_loader_options(linput_t *li, const load_info_t *loader);


/// Load a binary file into the database.
/// This function usually is called from ui.
/// \param filename   the name of input file as is
///                    (if the input file is from library, then
///                     this is the name from the library)
/// \param li        loader input source
/// \param _neflags  \ref NEF_. For the first file,
///                  the flag #NEF_FIRST must be set.
/// \param fileoff   Offset in the input file
/// \param basepara  Load address in paragraphs
/// \param binoff    Load offset (load_address=(basepara<<4)+binoff)
/// \param nbytes    Number of bytes to load from the file.
///                    - 0: up to the end of the file
///
///                  If nbytes is bigger than the number of
///                  bytes rest, the kernel will load as much
///                  as possible
/// \retval true   ok
/// \retval false  failed (couldn't open the file)

idaman bool ida_export load_binary_file(
                             const char *filename,
                             linput_t *li,
                             ushort _neflags,
                             int32 fileoff,
                             ea_t basepara,
                             ea_t binoff,
                             uint32 nbytes);


/// Load a non-binary file into the database.
/// This function usually is called from ui.
/// \param filename   the name of input file as is
///                   (if the input file is from library, then
///                    this is the name from the library)
/// \param li         loader input source
/// \param sysdlldir  a directory with system dlls. Pass "." if unknown.
/// \param _neflags   \ref NEF_. For the first file
///                   the flag #NEF_FIRST must be set.
/// \param loader     pointer to ::load_info_t structure.
///                   If the current IDP module has \ph{loader} != NULL
///                   then this argument is ignored.
/// \return success

idaman bool ida_export load_nonbinary_file(
                                const char *filename,
                                linput_t *li,
                                const char *sysdlldir,
                                ushort _neflags,
                                load_info_t *loader);


//--------------------------------------------------------------------------
/// Output file types
enum ofile_type_t
{
  OFILE_MAP  = 0,        ///< MAP file
  OFILE_EXE  = 1,        ///< Executable file
  OFILE_IDC  = 2,        ///< IDC file
  OFILE_LST  = 3,        ///< Disassembly listing
  OFILE_ASM  = 4,        ///< Assembly
  OFILE_DIF  = 5,        ///< Difference
};

/// \name Lines
/// Callback functions to output lines:
//@{
typedef int idaapi html_header_cb_t(FILE *fp);
typedef int idaapi html_footer_cb_t(FILE *fp);
typedef int idaapi html_line_cb_t(FILE *fp,
                                  const char *line,
                                  bgcolor_t prefix_color,
                                  bgcolor_t bg_color);
#define gen_outline_t html_line_cb_t
//@}

//------------------------------------------------------------------
/// Generate an output file.
/// \param otype  type of output file.
/// \param fp     the output file handle
/// \param ea1    start address. For some file types this argument is ignored
/// \param ea2    end address. For some file types this argument is ignored
///               as usual in ida, the end address of the range is not included
/// \param flags  \ref GENFLG_
///
/// For ::OFILE_EXE:
/// \retval 0  can't generate exe file
/// \retval 1  ok
///
/// For other file types:
/// \return number of the generated lines. -1 if an error occurred

idaman int ida_export gen_file(ofile_type_t otype, FILE *fp, ea_t ea1, ea_t ea2, int flags);

/// \defgroup GENFLG_ Generate file flags
/// Passed as 'flags' parameter to gen_file()
//@{
#define GENFLG_MAPSEG  0x0001          ///< ::OFILE_MAP: generate map of segments
#define GENFLG_MAPNAME 0x0002          ///< ::OFILE_MAP: include dummy names
#define GENFLG_MAPDMNG 0x0004          ///< ::OFILE_MAP: demangle names
#define GENFLG_MAPLOC  0x0008          ///< ::OFILE_MAP: include local names
#define GENFLG_IDCTYPE 0x0008          ///< ::OFILE_IDC: gen only information about types
#define GENFLG_ASMTYPE 0x0010          ///< ::OFILE_ASM,::OFILE_LST: gen information about types too
#define GENFLG_GENHTML 0x0020          ///< ::OFILE_ASM,::OFILE_LST: generate html (::ui_genfile_callback will be used)
#define GENFLG_ASMINC  0x0040          ///< ::OFILE_ASM,::OFILE_LST: gen information only about types
//@}

//----------------------------------------------------------------------
//      Helper functions for the loaders & ui
//----------------------------------------------------------------------

/// Load portion of file into the database.
/// This function will include (ea1..ea2) into the addressing space of the
/// program (make it enabled)
/// \param li         pointer of input source
/// \param pos        position in the file
/// \param ea1,ea2    range of destination linear addresses
/// \param patchable  should the kernel remember correspondence of
///                   file offsets to linear addresses.
/// \retval 1  ok
/// \retval 0  read error, a warning is displayed

idaman int ida_export file2base(linput_t *li,
                                int32 pos,
                                ea_t ea1,
                                ea_t ea2,
                                int patchable);

#define FILEREG_PATCHABLE       1       ///< means that the input file may be
                                        ///< patched (i.e. no compression,
                                        ///< no iterated data, etc)
#define FILEREG_NOTPATCHABLE    0       ///< the data is kept in some encoded
                                        ///< form in the file.


/// Load database from the memory.
/// This function works for wide byte processors too.
/// \param memptr   pointer to buffer with bytes
/// \param ea1,ea2  range of destination linear addresses
/// \param fpos     position in the input file the data is taken from.
///                 if == -1, then no file position correspond to the data.
/// \return 1 always

idaman int ida_export mem2base(const void *memptr,ea_t ea1,ea_t ea2,int32 fpos);


/// Unload database to a binary file.
/// This function works for wide byte processors too.
/// \param fp       pointer to file
/// \param pos      position in the file
/// \param ea1,ea2  range of source linear addresses
/// \return 1-ok(always), write error leads to immediate exit

idaman int ida_export base2file(FILE *fp,int32 pos,ea_t ea1,ea_t ea2);


/// Extract a module for an archive file.
/// Parse an archive file, show the list of modules to the user, allow him to
/// select a module, extract the selected module to a file (if the extract module
/// is an archive, repeat the process).
/// This function can handle ZIP, AR, AIXAR, OMFLIB files.
/// The temporary file will be automatically deleted by IDA at the end.
/// \param[in,out] filename    in: input file.
///                            out: name of the selected module.
/// \param bufsize             size of the buffer with 'filename'
/// \param is_remote           is the input file remote?
/// \param[out] temp_file_ptr  will point to the name of the file that
///                            contains the extracted module
/// \retval true   ok
/// \retval false  something bad happened (error message has been displayed to the user)

idaman bool ida_export extract_module_from_archive(
        char *filename,
        size_t bufsize,
        bool is_remote,
        char **temp_file_ptr);


/// Add long comment at \inf{minEA}.
///   - Input file:     ....
///   - File format:    ....
///
/// This function should be called only from the loader to describe the input file.

idaman void ida_export create_filename_cmt(void);


/// Get the input file type.
/// This function can recognize libraries and zip files.

idaman filetype_t ida_export get_basic_file_type(linput_t *li);


/// Get name of the current file type.
/// The current file type is kept in \inf{filetype}.
/// \param buf      buffer for the file type name
/// \param bufsize  its size
/// \return size of answer, this function always succeeds

idaman size_t ida_export get_file_type_name(char *buf, size_t bufsize);


//----------------------------------------------------------------------
//      Work with IDS files: read and use information from them
//

/// See ::importer_t
struct impinfo_t
{
  const char *dllname;
  void (idaapi*func)(uval_t num, const char *name, uval_t node);
  uval_t node;
};


/// Callback for checking dll module - passed to import_module().
/// \param li  pointer to input file
/// \param ii  import info.
///            If the function finds that ii.dllname does not match
///            the module name passed to import_module(), it returns 0. \n
///            Otherwise it calls ii.func for each exported entry.      \n
///            If ii.dllname==NULL then ii.func will be called
///            with num==0 and name==dllname.
/// \retval 0  dllname doesn't match, import_module() should continue
/// \retval 1  ok

typedef int (idaapi*importer_t)(linput_t *li,impinfo_t *ii);


/// Find and import a DLL module.
/// This function adds information to the database (renames functions, etc).
/// \param module    name of DLL
/// \param windir    system directory with dlls
/// \param modnode   node with information about imported entries.
///                  either altval or supval arrays may be absent.
///                  the node should never be deleted.
///                    - imports by ordinals:
///                        altval(ord) contains linear address
///                    - imports by name:
///                        supval(ea) contains the imported name
/// \param importer  callback function (may be NULL) to check dll module
/// \param ostype    type of operating system (subdir name).
///                  NULL means the IDS directory itself (not recommended)

idaman void ida_export import_module(const char *module,
                   const char *windir,
                   uval_t modnode,
                   importer_t importer,
                   const char *ostype);


/// Load and apply IDS file.
/// This function loads the specified IDS file and applies it to the database.
/// If the program imports functions from a module with the same name
/// as the name of the ids file being loaded, then only functions from this
/// module will be affected. Otherwise (i.e. when the program does not import
/// a module with this name) any function in the program may be affected.
/// \param fname  name of file to apply
/// \retval 1  ok
/// \retval 0  some error (a message is displayed).
///            if the ids file does not exist, no message is displayed

idaman int ida_export load_ids_module(char *fname);


//----------------------------------------------------------------------
//              DEFINITION OF PLUGIN MODULES
//----------------------------------------------------------------------
/// A plugin is a module in the plugins subdirectory that can perform
/// an action asked by the user. (usually via pressing a hotkey)
class plugin_t
{
public:
  int version;                  ///< Should be equal to #IDP_INTERFACE_VERSION
  int flags;                    ///< \ref PLUGIN_
/// \defgroup PLUGIN_ Plugin features
/// Used by plugin_t::flags
//@{
#define PLUGIN_MOD  0x0001      ///< Plugin changes the database.
                                ///< IDA won't call the plugin if
                                ///< the processor module prohibited any changes.
#define PLUGIN_DRAW 0x0002      ///< IDA should redraw everything after calling the plugin.
#define PLUGIN_SEG  0x0004      ///< Plugin may be applied only if the current address belongs to a segment
#define PLUGIN_UNL  0x0008      ///< Unload the plugin immediately after calling 'run'.
                                ///< This flag may be set anytime.
                                ///< The kernel checks it after each call to 'run'
                                ///< The main purpose of this flag is to ease
                                ///< the debugging of new plugins.
#define PLUGIN_HIDE 0x0010      ///< Plugin should not appear in the Edit, Plugins menu.
                                ///< This flag is checked at the start.
#define PLUGIN_DBG  0x0020      ///< A debugger plugin. init() should put
                                ///< the address of ::debugger_t to dbg.
#define PLUGIN_PROC 0x0040      ///< Load plugin when a processor module is loaded. (and keep it
                                ///< until the processor module is unloaded)
#define PLUGIN_FIX  0x0080      ///< Load plugin when IDA starts and keep it in the memory until IDA stops
#define PLUGIN_SCRIPTED 0x8000  ///< Scripted plugin. Should not be used by plugins,
                                ///< the kernel sets it automatically.
//@}

  int (idaapi *init)(void);     ///< Initialize plugin - returns \ref PLUGIN_INIT
/// \defgroup PLUGIN_INIT Plugin initialization codes
/// Return values for plugin_t::init()
//@{
#define PLUGIN_SKIP  0  ///< Plugin doesn't want to be loaded
#define PLUGIN_OK    1  ///< Plugin agrees to work with the current database.
                        ///< It will be loaded as soon as the user presses the hotkey
#define PLUGIN_KEEP  2  ///< Plugin agrees to work with the current database and wants to stay in the memory
//@}

  void (idaapi *term)(void);    ///< Terminate plugin. This function will be called
                                ///< when the plugin is unloaded. May be NULL.
  void (idaapi *run)(int arg);  ///< Invoke plugin
  const char *comment;          ///< Long comment about the plugin.
                                ///< it could appear in the status line
                                ///< or as a hint
  const char *help;             ///< Multiline help about the plugin
  const char *wanted_name;      ///< The preferred short name of the plugin
  const char *wanted_hotkey;    ///< The preferred hotkey to run the plugin
};

#if defined(__IDP__) && !defined(PLUGIN_SUBMODULE)
idaman ida_module_data plugin_t PLUGIN; // (declaration for plugins)
#endif

//--------------------------------------------------------------------------
/// Callback provided to hook_to_notification_point().
/// A plugin can hook to a notification point and receive notifications
/// of all major events in IDA. The callback function will be called
/// for each event.
/// \param user_data          data supplied in call to hook_to_notification_point()
/// \param notification_code  processor_t::idp_notify or ::ui_notification_t, depending on
///                           the hook type
/// \param va                 additional parameters supplied with the notification.
///                           see the event descriptions for information
/// \retval 0    ok, the event should be processed further
/// \retval !=0  the event is blocked and should be discarded.
///              in the case of processor modules, the returned value is used
///              as the return value of processor_t::notify()

typedef int idaapi hook_cb_t(void *user_data, int notification_code, va_list va);

/// Types of events that be hooked to with hook_to_notification_point()
enum hook_type_t
{
  HT_IDP,         ///< Hook to the processor module.
                  ///< The callback will receive all processor_t::idp_notify events.
  HT_UI,          ///< Hook to the user interface.
                  ///< The callback will receive all ::ui_notification_t events.
  HT_DBG,         ///< Hook to the debugger.
                  ///< The callback will receive all ::dbg_notification_t events.
  HT_IDB,         ///< Hook to the database events.
                  ///< These events are separated from the ::HT_IDP group
                  ///< to speed things up (there are too many plugins and
                  ///< modules hooking to the ::HT_IDP). Some essential events
                  ///< are still generated in th ::HT_IDP group:
                  ///< make_code, make_data, undefine, rename, add_func, del_func.
                  ///< This list is not exhaustive.
                  ///< A common trait of all events in this group: the kernel
                  ///< does not expect any reaction to the event and does not
                  ///< check the return code. For event names, see ::idb_event.
  HT_DEV,         ///< Internal debugger events.
                  ///< Not stable and undocumented for the moment
  HT_VIEW,        ///< Custom/IDA views notifications.
                  ///< Refer to ::view_notification_t
                  ///< for notification codes
  HT_OUTPUT,      ///< Output window notifications.
                  ///< Refer to ::msg_notification_t
                  ///< (::view_notification_t)
  HT_LAST
};


/// Register a callback for a class of events in IDA

idaman bool ida_export hook_to_notification_point(hook_type_t hook_type,
                                hook_cb_t *cb,
                                void *user_data);


/// Unregister a callback (also see hook_to_notification_point()).
/// A plugin should unhook before being unloaded
/// (preferably in its termination function).
/// If different callbacks have the same callback function pointer
/// and user_data is not NULL, only the callback whose associated
/// user defined data matches will be removed.
/// \return number of unhooked functions.

idaman int ida_export unhook_from_notification_point(hook_type_t hook_type,
                                    hook_cb_t *cb,
                                    void *user_data = NULL);


/// A well behaved processor module should call this function
/// in its processor_t::notify() function. If this function returns 0, then
/// the processor module should process the notification itself.
/// Otherwise the code should be returned to the caller, like this:
/// \code
///   int code = invoke_callbacks(HT_IDP, what, va);
///   if ( code ) return code;
///   ...
/// \endcode

idaman int ida_export invoke_callbacks(hook_type_t hook_type, int notification_code, va_list va);


/// Get plugin options from the command line.
/// If the user has specified the options in the -Oplugin_name:options
/// format, them this function will return the 'options' part of it
/// The 'plugin' parameter should denote the plugin name
/// Returns NULL if there we no options specified

idaman const char *ida_export get_plugin_options(const char *plugin);


//--------------------------------------------------------------------------
/// \def{PLUGIN_EXT, PLUGIN module file name extensions}
#ifdef __NT__
#ifdef __EA64__
#ifdef __X64__
#define PLUGIN_EXT  "px64"
#else
#define PLUGIN_EXT  "p64"
#endif
#else
#define PLUGIN_EXT  "plw"
#endif
#endif
#ifdef __LINUX__
#ifdef __EA64__
#ifdef __X64__
#define PLUGIN_EXT  "px64"
#else
#define PLUGIN_EXT  "plx64"
#endif
#else
#define PLUGIN_EXT  "plx"
#endif
#endif
#ifdef __MAC__
#ifdef __EA64__
#ifdef __X64__
#define PLUGIN_EXT  "px64"
#else
#define PLUGIN_EXT  "pmc64"
#endif
#else
#define PLUGIN_EXT  "pmc"
#endif
#endif
#ifdef __BSD__
#ifdef __EA64__
#define PLUGIN_EXT  "pbsd64"
#else
#define PLUGIN_EXT  "pbsd"
#endif
#endif

/// Pattern to find plugin files
#define PLUGIN_DLL "*." PLUGIN_EXT

// LOW LEVEL DLL LOADING FUNCTIONS
// Only the kernel should use these functions!
/// \cond
#define LNE_MAXSEG      10      // Max number of segments

#if 0
extern char dlldata[4096];      // Reserved place for DLL data
#define DLLDATASTART    0xA0    // Absolute offset of dlldata
extern char ldrdata[64];        // Reserved place for LOADER data
#define LDRDATASTART    (DLLDATASTART+sizeof(dlldata)) // Absolute offset of ldrdata
#endif

struct idadll_t
{
  void *dllinfo[LNE_MAXSEG];
  void *entry;                  // first entry point of DLL
  idadll_t(void) { dllinfo[0] = NULL; entry = NULL; }
  bool is_loaded(void) const { return dllinfo[0] != NULL; }
};

int load_dll(const char *file, idadll_t *dllmem);
                                // dllmem - allocated segments
                                //          dos: segment 1 (data) isn't allocated
                                // Returns 0 - ok, else:
#define RE_NOFILE       1       /* No such file */
#define RE_NOTIDP       2       /* Not IDP file */
#define RE_NOPAGE       3       /* Can't load: bad segments */
#define RE_NOLINK       4       /* No linkage info */
#define RE_BADRTP       5       /* Bad relocation type */
#define RE_BADORD       6       /* Bad imported ordinal */
#define RE_BADATP       7       /* Bad relocation atype */
#define RE_BADMAP       8       /* DLLDATA offset is invalid */

void                   load_dll_or_die(const char *file, idadll_t *dllmem);
idaman bool ida_export load_dll_or_say(const char *file, idadll_t *dllmem);

idaman void ida_export free_dll(idadll_t *dllmem);
/// \endcond

/// Processor name
struct idp_name_t
{
  qstring lname;        ///< long processor name
  qstring sname;        ///< short processor name
  bool    hidden;       ///< is hidden
  idp_name_t() : hidden(false) {}
};
DECLARE_TYPE_AS_MOVABLE(idp_name_t);
typedef qvector<idp_name_t> idp_names_t; ///< vector of processor names

/// Processor module description
struct idp_desc_t
{
  qstring   path;       ///< module file name
  time_t    mtime;      ///< time of last modification
  qstring   family;     ///< processor's family
  idp_names_t names;    ///< processor names
  bool      is_script;  ///< the processor module is a script
  bool      checked;    ///< internal, for cache management
  idp_desc_t(): mtime(time_t(-1)), is_script(false), checked(false) {}
};
DECLARE_TYPE_AS_MOVABLE(idp_desc_t);
typedef qvector<idp_desc_t> idp_descs_t; ///< vector of processor module descriptions


/// Get IDA processor modules descriptions

idaman const idp_descs_t *ida_export get_idp_descs(void);


/// Enumerate IDA plugins.
/// Returns when func() returns nonzero.
/// \param answer       buffer to contain the file name for which func()!=0
///                     (answer may be == NULL)
/// \param answer_size  size of 'answer'
/// \param func         callback function called for each file
///                       - file: full file name (with path)
///                       - ud: user data
///
///                     if 'func' returns non-zero value, the enumeration
///                     is stopped and full path of the current file
///                     is returned to the caller.
/// \param ud           user data. this pointer will be passed to
///                     the callback function
/// \param el           If the plugin is a scripted plugin, then 'el' will point to the
///                     associated extlang. This parameter may be NULL
/// \return zero or the code returned by func()

idaman int ida_export enum_plugins(
        int (idaapi *func)(const char *file, void *ud),
        void *ud,
        char *answer,
        size_t answer_size,
        const extlang_t **el = NULL);


//--------------------------------------------------------------------------
/// \def{IDP_EXT, IDP module file name extension}
#ifdef __NT__
#ifdef __EA64__
#ifdef __X64__
#define IDP_EXT  "ix64"
#else
#define IDP_EXT  "w64"
#endif
#else
#define IDP_EXT  "w32"
#endif
#endif
#ifdef __LINUX__
#ifdef __EA64__
#ifdef __X64__
#define IDP_EXT  "ix64"
#else
#define IDP_EXT  "ilx64"
#endif
#else
#define IDP_EXT  "ilx"
#endif
#endif
#ifdef __MAC__
#ifdef __EA64__
#ifdef __X64__
#define IDP_EXT  "ix64"
#else
#define IDP_EXT  "imc64"
#endif
#else
#define IDP_EXT  "imc"
#endif
#endif
#ifdef __BSD__
#ifdef __EA64__
#define IDP_EXT  "ibsd64"
#else
#define IDP_EXT  "ibsd"
#endif
#endif

/// \def{IDP_DLL, Pattern to find idp files}
#ifdef __EA64__
#define IDP_DLL "*64." IDP_EXT
#else
#define IDP_DLL "*." IDP_EXT
#endif


//--------------------------------------------------------------------------
/// Structure to store Plugin information
struct plugin_info_t
{
  plugin_info_t *next;  ///< next plugin information
  char *path;           ///< full path to the plugin
  char *org_name;       ///< original short name of the plugin
  char *name;           ///< short name of the plugin
                        ///< it will appear in the menu
  ushort org_hotkey;    ///< original hotkey to run the plugin
  ushort hotkey;        ///< current hotkey to run the plugin
  int arg;              ///< argument used to call the plugin
  plugin_t *entry;      ///< pointer to the plugin if it is already loaded
  idadll_t dllmem;
  int flags;            ///< a copy of plugin_t::flags
  char *comment;        ///< a copy of plugin_t::comment
};


/// Get pointer to the list of plugins. (some plugins might be listed several times
/// in the list - once for each configured argument)

idaman plugin_info_t *ida_export get_plugins(void);


/// Load a user-defined plugin.
/// \param name  short plugin name without path and extension,
///              or absolute path to the file name
/// \return pointer to plugin description block

idaman plugin_t *ida_export load_plugin(const char *name);


/// Run a loaded plugin with the specified argument.
/// \param ptr  pointer to plugin description block
/// \param arg  argument to run with

idaman bool ida_export run_plugin(const plugin_t *ptr, int arg);


/// Load & run a plugin

inline bool idaapi load_and_run_plugin(const char *name, int arg)
{
  return run_plugin(load_plugin(name), arg);
}


/// Run a plugin as configured.
/// \param ptr  pointer to plugin information block

idaman bool ida_export invoke_plugin(plugin_info_t *ptr);


/// Information for the user interface about available debuggers
struct dbg_info_t
{
  plugin_info_t *pi;
  struct debugger_t *dbg;
  dbg_info_t(plugin_info_t *_pi, struct debugger_t *_dbg) : pi(_pi), dbg(_dbg) {}
};
DECLARE_TYPE_AS_MOVABLE(dbg_info_t);


/// Get information about available debuggers

idaman size_t ida_export get_debugger_plugins(const dbg_info_t **array);


/// Initialize plugins with the specified flag

idaman void ida_export init_plugins(int flag);


/// Terminate plugins with the specified flag

idaman void ida_export term_plugins(int flag);


//------------------------------------------------------------------------
/// \name Regions
/// Work with file regions (for patching)
//@{
void init_fileregions(void);
void term_fileregions(void);
inline void save_fileregions(void) {}
void add_fileregion(ea_t ea1,ea_t ea2,int32 fpos);
void move_fileregions(ea_t from, ea_t to, asize_t size);
void del_fileregions(ea_t ea1, ea_t ea2);
//@}

/// Get offset in the input file which corresponds to the given ea.
/// If the specified ea can't be mapped into the input file offset,
/// return -1.

idaman int32 ida_export get_fileregion_offset(ea_t ea);


/// Get linear address which corresponds to the specified input file offset.
/// If can't be found, return #BADADDR

idaman ea_t ida_export get_fileregion_ea(int32 offset);


//------------------------------------------------------------------------
/// Generate an exe file (unload the database in binary form).
/// \return fp  the output file handle. if fp == NULL then return:
///               - 1: can generate an executable file
///               - 0: can't generate an executable file
/// \retval 1  ok
/// \retval 0  failed

idaman int ida_export gen_exe_file(FILE *fp);


//------------------------------------------------------------------------
/// Reload the input file.
/// This function reloads the byte values from the input file.
/// It doesn't modify the segmentation, names, comments, etc.
/// \param file       name of the input file. if file == NULL then returns:
///                     - 1: can reload the input file
///                     - 0: can't reload the input file
/// \param is_remote  is the file located on a remote computer with
///                   the debugger server?
/// \retval 1  ok
/// \retval 0  failed

idaman int ida_export reload_file(const char *file, bool is_remote);


//---------------------------------------------------------------------------
//       S N A P S H O T   F U N C T I O N S

/// Maximum database snapshot description length
#define MAX_DATABASE_DESCRIPTION 128

class snapshot_t;
typedef qvector<snapshot_t *> snapshots_t; ///< vector of database snapshots

/// Snapshot attributes
class snapshot_t
{
private:
  snapshot_t &operator=(const snapshot_t &);
  snapshot_t(const snapshot_t &);

  int compare(const snapshot_t &r) const
  {
    return id > r.id ? 1 : id < r.id ? -1 : 0;
  }

public:
  qtime64_t id;                          ///< snapshot ID. This value is computed using qgettimeofday()
  uint16 flags;                          ///< \ref SSF_
/// \defgroup SSF_ Snapshot flags
/// Used by snapshot_t::flags
//@{
#define SSF_AUTOMATIC         0x0001     ///< automatic snapshot
//@}
  char desc[MAX_DATABASE_DESCRIPTION];   ///< snapshot description
  char filename[QMAXPATH];               ///< snapshot file name
  snapshots_t children;                  ///< snapshot children
  DEFINE_MEMORY_ALLOCATION_FUNCS()
  DECLARE_COMPARISON_OPERATORS(snapshot_t)
  void clear()
  {
    for ( snapshots_t::iterator p=children.begin();
          p != children.end();
          ++p )
    {
      delete *p;
    }
    children.clear();
  }

  snapshot_t(): id(0), flags(0)
  {
    filename[0] = desc[0] = '\0';
  }

  ~snapshot_t()
  {
    clear();
  }
};
DECLARE_TYPE_AS_MOVABLE(snapshot_t);

//------------------------------------------------------------------------
/// Build the snapshot tree.
/// \param root  snapshot root that will contain the snapshot tree elements.
/// \return success

idaman bool ida_export build_snapshot_tree(snapshot_t *root);


//------------------------------------------------------------------------
/// Update the snapshot attributes.
/// \note only the snapshot description can be updated.
/// \param filename  snapshot file name or NULL for the current database
/// \param root      snapshot root (returned from build_snapshot_tree())
/// \param attr      snapshot instance containing the updated attributes
/// \param uf        \ref SSUF_
/// \return success
idaman bool ida_export update_snapshot_attributes(
        const char *filename,
        const snapshot_t *root,
        const snapshot_t *attr,
        int uf);

/// \defgroup SSUF_ Snapshot update flags
/// Passed as 'uf' parameter to update_snapshot_attributes()
//@{
#define SSUF_DESC        0x00000001             ///< Update the description
#define SSUF_PATH        0x00000002             ///< Update the path
#define SSUF_FLAGS       0x00000004             ///< Update the flags
//@}

//------------------------------------------------------------------------
/// Visit the snapshot tree.
/// \param root      snapshot root to start the enumeration from
/// \param callback  callback called for each child. return 0 to continue enumeration
///                  and non-zero to abort enumeration
/// \param ud        user data. will be passed back to the callback
/// \return true-ok, false-failed

idaman int ida_export visit_snapshot_tree(
        snapshot_t *root,
        int (idaapi *callback)(snapshot_t *ss, void *ud),
        void *ud);


/// Flush buffers to the disk

idaman int ida_export flush_buffers(void);


/// Is the database considered as trusted?

idaman bool ida_export is_trusted_idb(void);

//------------------------------------------------------------------------
/// Save current database using a new file name.
/// \param outfile  output database file name
/// \param flags    \ref DBFL_
/// \param root     optional: snapshot tree root.
/// \param attr     optional: snapshot attributes
/// \note when both root and attr are not NULL then the snapshot
///       attributes will be updated, otherwise the snapshot attributes
///       will be inherited from the current database.
/// \return success

idaman bool ida_export save_database_ex(
        const char *outfile,
        uint32 flags,
        const snapshot_t *root = NULL,
        const snapshot_t *attr = NULL);

/// \defgroup DBFL_ Database flags
/// Used to manage saving/closing of a database
//@{
#define DBFL_KILL       0x01            ///< delete unpacked database
#define DBFL_COMP       0x02            ///< collect garbage
#define DBFL_BAK        0x04            ///< create backup file (if !DBFL_KILL)
#define DBFL_TEMP       0x08            ///< temporary database
//@}

/// \cond
idaman ida_export_data char command_line_file[QMAXPATH];  // full path to the file specified in the command line
idaman ida_export_data char database_idb[QMAXPATH];       // full path of IDB file
idaman ida_export_data char database_id0[QMAXPATH];       // full path of ID0 file
idaman bool ida_export is_database_ext(const char *ext);  // check the file extension

idaman uint32 ida_export_data database_flags; // for close_database() - see 'Database flags'

inline bool is_temp_database(void) { return (database_flags & DBFL_TEMP) != 0; }

//--------------------------------------------------------------------------
#ifndef NO_OBSOLETE_FUNCS
idaman DEPRECATED bool ida_export save_database(const char *outfile, bool delete_unpacked);
idaman DEPRECATED char *ida_export get_idp_desc(const char *file, char *buf, size_t bufsize);
idaman DEPRECATED int ida_export enum_processor_modules(
        int (idaapi *func)(const char *file, void *ud),
        void *ud,
        char *answer,
        size_t answer_size,
        const extlang_t **el = NULL);
idaman int ida_export load_loader_module(linput_t *li, const char *lname, const char *fname, bool is_remote);
#endif

#pragma pack(pop)
#endif
