/*
 *      The Interactive Disassembler (IDA).
 *      Copyright (c) 1990-2015 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 */

#ifndef _STRLIST_HPP
#define _STRLIST_HPP
#pragma pack(push, 1)           // IDA uses 1 byte alignments!

/*! \file strlist.hpp

  \brief Functions that deal with the string list

  While the kernel keeps the strings list, it does not update it.
  The string list is not used by the kernel because
  keeping it up-to-date would slow down IDA without any benefit.

  The users of this list should refresh_strlist() before accessing it.
*/

/// Structure to keep string list parameters
struct strwinsetup_t
{
  strwinsetup_t()
    : strtypes(0), minlen(-1), display_only_existing_strings(0),
      only_7bit(1), ignore_heads(0), ea1(BADADDR), ea2(BADADDR) {}
  uint32 strtypes; // bitmask of allowed string types
  sval_t minlen;
  uchar display_only_existing_strings;
  uchar only_7bit;
  uchar ignore_heads;
  ea_t ea1, ea2;

  inline bool is_initialized() const { return minlen > -1; }

  // private functions:
  bool setup_strings_window(void);
  void save_config(void);
  void restore_config(void);
};

/// Information about one string from the string list
struct string_info_t
{
  ea_t ea;
  int length;
  int type;
  string_info_t() : ea(BADADDR), length(0), type(0) {}
  string_info_t(ea_t _ea) : ea(_ea), length(0), type(0) {}
  bool operator<(const string_info_t &string_info) const { return ea < string_info.ea; }
};
DECLARE_TYPE_AS_MOVABLE(string_info_t);


/// Set string list options
/// \return success

idaman bool ida_export set_strlist_options(const strwinsetup_t *options);


/// Refresh the string list

idaman void ida_export refresh_strlist(ea_t ea1, ea_t ea2);


/// Get number of elements in the string list

idaman size_t ida_export get_strlist_qty(void);


/// Get nth element of the string list (n=0,,get_strlist_qty()-1)

idaman bool ida_export get_strlist_item(int n, string_info_t *si);




#pragma pack(pop)
#endif // _STRLIST_HPP
