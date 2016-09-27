/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2015 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 */

#ifndef __REGISTRY_HPP
#define __REGISTRY_HPP

/*! \file registry.hpp

  \brief Registry related functions

  IDA uses the registry to store global configuration options that must
  persist after IDA has been closed.

  On Windows, IDA uses the Windows registry directly. On Unix systems, the registry
  is stored in a file (typically ~/.idapro/ida.reg).

  The root key for accessing IDA settings in the registry is defined by #ROOT_KEY_NAME.
*/

/// Key used to store IDA settings in registry (Windows version).
/// \note this name is automatically prepended to all
/// key names passed to functions in this file.
#define ROOT_KEY_NAME           "Software\\Hex-Rays\\IDA"

/// \cond
// Low level functions. DO NOT USE THEM. See the wrappers below.
// 'mode' is:
//  - 0: data is a raw buffer, and its datalen must be able to hold the entire binary contents
//  - 1: data is a raw buffer, and its datalen doesn't need to be able to hold the entire binary contents
//  - 2: data is a ::bytevec_t*, datalen is ignored.
idaman bool ida_export reg_bin_op(
        const char *name,
        bool save,
        void *data,
        size_t datalen,
        const char *subkey,
        int mode = 0);
idaman bool ida_export reg_str_op(
        const char *name,
        bool save,
        char *utf8,
        size_t utf8len,
        const char *subkey);
idaman int ida_export reg_int_op(
        const char *name,
        bool save,
        int value,
        const char *subkey = NULL);
/// \endcond

/// Types of values stored in the registry
enum regval_type_t
{
  reg_unknown = 0, ///< unknown
  reg_sz      = 1, ///< utf8 string
  reg_binary  = 3, ///< binary data
  reg_dword   = 4  ///< 32-bit number
};


/// Delete a key from the registry

idaman bool ida_export reg_delete_subkey(const char *name);


/// Delete a value from the registry.
/// \param name    value name
/// \param subkey  parent key
/// \return success

idaman bool ida_export reg_delete(const char *name, const char *subkey = NULL);


/// Is there already a key with the given name?

idaman bool ida_export reg_subkey_exists(const char *name);


/// Is there already a value with the given name?
/// \param name    value name
/// \param subkey  parent key

idaman bool ida_export reg_exists(const char *name, const char *subkey = NULL);


/// Retrieve the child names of the given key.
/// \param out      result
/// \param name     key name
/// \param subkeys  if true, collect subkey names. if false, collect value names.
/// \return false if the given key does not exist

idaman bool ida_export reg_subkey_children(qstrvec_t *out, const char *name, bool subkeys);


/// Get data type of a given value.
/// \param out     result
/// \param name    value name
/// \param subkey  key name
/// \return false if the [key+]value doesn't exist

idaman bool ida_export reg_data_type(regval_type_t *out, const char *name, const char *subkey = NULL);


/// Retrieve all string values associated with the given key.
/// Also see reg_update_strlist().

idaman void ida_export reg_read_strlist(const char *subkey, qstrvec_t *list);


/// Update list of strings associated with given key.
/// \param subkey      key name
/// \param add         string to be added to list, can be NULL
/// \param maxrecs     limit list to this size
/// \param rem         string to be removed from list, can be NULL
/// \param ignorecase  ignore case for 'add' and 'rem'

idaman void ida_export reg_update_strlist(
        const char *subkey,
        const char *add,
        size_t maxrecs,
        const char *rem = NULL,
        bool ignorecase = false);


/// Write binary data to the registry.
/// \param name     value name
/// \param data     input, must not be NULL
/// \param datalen  length of input in bytes
/// \param subkey   key name

inline void reg_write_binary(
        const char *name,
        const void *data,
        size_t datalen,
        const char *subkey = NULL)
{
  reg_bin_op(name, true, CONST_CAST(void *)(data), datalen, subkey);
}


/// Read binary data from the registry.
/// \param name       value name
/// \param[out] data  result, must not be NULL
/// \param datalen    length of out buffer in bytes
/// \param subkey     key name
/// \return false if 'data' is not large enough to hold all data present.
///         in this case 'data' is left untouched.

inline bool reg_read_binary(
        const char *name,
        void *data,
        size_t datalen,
        const char *subkey = NULL)
{
  return reg_bin_op(name, false, data, datalen, subkey);
}


/// Read a chunk of binary data from the registry.
/// This function succeeds even in the case of a partial read.
/// \param name       value name
/// \param[out] data  result, must not be NULL
/// \param datalen    length of output buffer in bytes
/// \param subkey     key name
/// \return success

inline bool reg_read_binary_part(
        const char *name,
        void *data,
        size_t datalen,
        const char *subkey = NULL)
{
  return reg_bin_op(name, false, data, datalen, subkey, 1);
}


/// Read binary data from the registry.
/// \param name       value name
/// \param[out] data  output buffer, must not be NULL
/// \param subkey     key name
/// \return success

inline bool reg_read_binary(
        const char *name,
        bytevec_t *data,
        const char *subkey = NULL)
{
  return reg_bin_op(name, false, data, 0, subkey, 2);
}


/// Write a string to the registry.
/// \param name    value name
/// \param utf8    utf8-encoded string
/// \param subkey  key name

inline void reg_write_string(
        const char *name,
        const char *utf8,
        const char *subkey = NULL)
{
  reg_str_op(name, true, CONST_CAST(char *)(utf8), 0, subkey);
}


/// Read a string from the registry.
/// \param name       value name
/// \param[out] utf8  output buffer
/// \param utf8len    length of output buffer
/// \param subkey     key name
/// \return success

inline bool reg_read_string(
        const char *name,
        char *utf8,
        size_t utf8len,
        const char *subkey = NULL)
{
  return reg_str_op(name, false, utf8, utf8len, subkey);
}


/// Read a string from the registry.
/// \param name       value name
/// \param utf8len    length of output buffer
/// \param[out] utf8  output buffer
/// \param def        default value, copied to 'utf8' if initial reg read fails.
/// \param subkey     key name

inline void reg_read_string(
        const char *name,
        size_t utf8len,
        char *utf8,
        const char *def,
        const char *subkey = NULL)
{
  if ( !reg_read_string(name, utf8, utf8len, subkey) )
      ::qstrncpy(utf8, def, utf8len);
}


/// Read integer value from the registry.
/// \param name    value name
/// \param defval  default value
/// \param subkey  key name
/// \return the value read from the registry, or 'defval' if the read failed

inline int reg_read_int(const char *name, int defval, const char *subkey = NULL)
{
  return reg_int_op(name, false, defval, subkey);
}


/// Write integer value to the registry.
/// \param name    value name
/// \param value   value to write
/// \param subkey  key name

inline void reg_write_int(const char *name, int value, const char *subkey = NULL)
{
  reg_int_op(name, true, value, subkey);
}


/// Read boolean value from the registry.
/// \param name    value name
/// \param defval  default value
/// \param subkey  key name
/// \return boolean read from registry, or 'defval' if the read failed

inline bool reg_read_bool(const char *name, bool defval, const char *subkey = NULL)
{
  return reg_int_op(name, false, int(defval), subkey) != 0;
}


/// Write boolean value to the registry.
/// \param name    value name
/// \param value   boolean to write (nonzero = true)
/// \param subkey  key name

inline void reg_write_bool(const char *name, int value, const char *subkey = NULL)
{
  reg_int_op(name, true, value != 0, subkey);
}


/// Get all subkey names of given key

inline bool reg_subkey_subkeys(qstrvec_t *out, const char *name)
{
  return reg_subkey_children(out, name, true);
}


/// Get all value names under given key

inline bool reg_subkey_values(qstrvec_t *out, const char *name)
{
  return reg_subkey_children(out, name, false);
}


/// Update registry with a file list.
/// Case sensitivity will vary depending on the target OS.
/// \note 'add' and 'rem' must be UTF-8, just like for regular string operations.

inline void reg_update_filestrlist(
        const char *subkey,
        const char *add,
        size_t maxrecs,
        const char *rem = NULL)
{
  reg_update_strlist(
    subkey, add, maxrecs, rem,
  #ifdef __NT__ // Ignore case in Windows
    true
  #else
    false
  #endif
  );
}

//-----------------------------------------------------------------------------
// INTERNALS

/// \cond
#define _RVN_(f) regname_ ## f

#ifndef __DEFINE_REG_NAMES__
#define REG_VAL_NAME(n,s)         \
    extern const char _RVN_(n)[]
#else
#define REG_VAL_NAME(n,s)         \
    extern const char _RVN_(n)[]; \
    const char _RVN_(n)[] = s
#endif

#define REG_BOOL_FUNC(func, valname)          \
REG_VAL_NAME(func, valname);                  \
inline void regset_ ## func (bool value)      \
  { reg_write_bool(_RVN_(func), value); }     \
inline bool regget_ ## func (bool def)        \
  { return reg_read_bool(_RVN_(func), def); }

#define REG_INT_FUNC(func, valname)           \
REG_VAL_NAME(func,valname);                   \
inline void regset_ ## func(int value)        \
{                                             \
  reg_int_op(_RVN_(func), true, value);       \
}                                             \
inline int regget_ ## func(int def=0)         \
{                                             \
  return reg_int_op(_RVN_(func), false, def); \
}

idaman void ida_export reg_load(void);
idaman void ida_export reg_flush(void);

// if using history functions below, you have to define the following two variables
extern const char regkey_history[];
extern int max_history_files;     // max number of files in the file menu
                                  // and in the welcome box
#define MAX_HISTORY_FILES_DEF 10  // default value

inline void regget_history(qstrvec_t *list)
{
#ifdef DEMO
  qnotused(list);
#else
  reg_read_strlist(regkey_history, list);
#endif
}

inline void reg_update_history(const char *addfile, const char *removefile = NULL)
{
#ifdef DEMO
  qnotused(addfile);
  qnotused(removefile);
#else
  // On Windows avoid duplicate upper/lower-case entries
  // by using reg_update_filestrlist() which takes care of case sensitivity
  reg_update_filestrlist(regkey_history, addfile, max_history_files, removefile);
#endif
}

inline void reg_history_size_truncate(void)
{
#ifndef DEMO
  reg_update_strlist(regkey_history, NULL, max_history_files, NULL);
#endif
}
/// \endcond

#endif // __REGISTRY_HPP
