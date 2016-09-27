/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2015 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 */

#ifndef _ENTRY_HPP
#define _ENTRY_HPP
#pragma pack(push, 1)

/*! \file entry.hpp

  \brief Functions that deal with entry points

  Exported functions are considered as entry points as well.

  IDA maintains list of entry points to the program.
  Each entry point:
    - has an address
    - has a name
    - may have an ordinal number
*/

/// Get number of entry points

idaman size_t ida_export get_entry_qty(void);


/// Add an entry point to the list of entry points.
/// \param ord       ordinal number
///                  if ordinal number is equal to 'ea' then ordinal is not used
/// \param ea        linear address
/// \param name      name of entry point. If the specified location already
///                  has a name, the old name will be appended to the regular
///                  comment. If name == NULL, then the old name will be retained.
/// \param makecode  should the kernel convert bytes at the entry point
///                  to instruction(s)
/// \return success (currently always true)

idaman bool ida_export add_entry(uval_t ord, ea_t ea,const char *name, bool makecode);


/// Get ordinal number of an entry point.
/// \param idx  internal number of entry point. Should be
///             in the range 0..get_entry_qty()-1
/// \return ordinal number or 0.

idaman uval_t ida_export get_entry_ordinal(size_t idx);


/// Get entry point address by its ordinal
/// \param ord  ordinal number of entry point
/// \return address or #BADADDR

idaman ea_t ida_export get_entry(uval_t ord);


/// Get name of the entry point by its ordinal.
/// \param ord      ordinal number of entry point
/// \param buf      output buffer, may be NULL
/// \param bufsize  output buffer size
/// \return size of entry name or -1

idaman ssize_t ida_export get_entry_name(uval_t ord, char *buf, size_t bufsize);


/// Rename entry point.
/// \param ord      ordinal number of the entry point
/// \param name     name of entry point. If the specified location already
///                 has a name, the old name will be appended to a repeatable
///                 comment.
/// \return success

idaman bool ida_export rename_entry(uval_t ord, const char *name);




#pragma pack(pop)
#endif // _ENTRY_HPP
