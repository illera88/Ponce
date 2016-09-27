/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2015 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 *      Enums and bitfields
 *      Bitfields will be abbreviated as "bf".
 *
 */

#ifndef _ENUM_HPP
#define _ENUM_HPP

#include <stdio.h>
#include <nalt.hpp>

#pragma pack(push, 1)

/*! \file enum.hpp

   \brief Assembly level enum management

   Enums and bitfields are represented as ::enum_t.
*/

typedef tid_t enum_t;         ///< simple netnode that provides information about an enum.
                              ///< name(), cmt() and rptcmt() are kept in the usual form.
                              ///< memqty is kept in altval(-1).
                              ///<
                              ///< bitfields:
                              ///<   - have altval(-5) bit0 set to 1.
                              ///<   - altval('M') keeps ptrs to nodes by bitmasks
                              ///<
                              ///< enums:
                              ///<   - all members are kept in altval('E',member_value)-1

typedef uval_t bmask_t;       ///< unsigned value that describes a bitmask (also see ::enum_t).
                              ///< a bit mask is 32/64 bits.
                              ///< if a mask is one-bit, then ptr to ::const_t is kept immediately in altval('M').
                              ///< otherwise, masknode.altval('E') keeps ptrs to ::const_t by values
                              ///< (the same as with enums).

#define DEFMASK (bmask_t(-1)) ///< default bitmask

typedef uval_t const_t;       ///< members of enums are kept as netnodes.
                              ///< name(), cmt() and rptcmt() are kept in the usual form.
                              ///< altval(-2) is ::enum_t.

/// Max number of identical constants allowed for one enum type
const uchar MAX_ENUM_SERIAL = 255;


/// \cond
// you shouldn't use these definitions, they are private
#define ENUM_REVERSE    'Y'
#define ENUM_SELMEMS    'Z'           // temporary array for choose_enum_by_value
#define ENUM_QTY_IDX    uval_t(-1)
#define ENUM_FLG_IDX    uval_t(-3)
#define ENUM_FLAGS      uval_t(-5)
#define ENUM_ORDINAL    uval_t(-8)    // corresponding til type ordinal number
#define ENUM_FLAGS_IS_BF   0x00000001
#define ENUM_FLAGS_HIDDEN  0x00000002
#define ENUM_FLAGS_FROMTIL 0x00000004
#define ENUM_FLAGS_WIDTH   0x00000038
#define ENUM_FLAGS_GHOST   0x00000040 // ghost copy of a local type
bool set_enum_flag(enum_t id, uint32 bit, bool set);
#define ENUM_MASKS      'm'           // indexed by masks
#define ENUM_MEMBERS    'E'           // indexed by values
#define CONST_ENUM      uval_t(-2)
#define CONST_VALUE     uval_t(-3)
#define CONST_BMASK     uval_t(-6)
#define CONST_SERIAL    uval_t(-7)    // the serial number of the constant
#define CONST_SERIALS   's'           // indexed by serials (1..n)
                                      // the serials array is present only at the constant
                                      // with the minimal serial number (usually 0)

idaman netnode ida_export_data enums; // main netnode to access all enums.
                                      // should not be used directly!
                                      //   - altval(idx)-1 -> ::enum_t
                                      //   - altval('R',::enum_t)-1 -> idx
                                      //   - altval(-1) -> qty
/// \endcond


/// Get number of declared ::enum_t types

inline size_t   get_enum_qty(void)      { return size_t(enums.altval(ENUM_QTY_IDX)); }


/// Get enum by its ordinal number (0..n).
// nb: parenthesis around uval_t are required by gcc v3.2.2.

inline enum_t   getn_enum(size_t n)     { return ((uval_t)(int(n))==ENUM_QTY_IDX) ? BADNODE : enums.altval(n)-1; }


/// Get serial number of enum.
/// The serial number determines the place of the enum in the enum window.

inline uval_t   get_enum_idx(enum_t id) { return enums.altval(id,ENUM_REVERSE)-1; }


/// Get enum by name

idaman enum_t ida_export get_enum(const char *name);


/// Is enum a bitfield?
/// (otherwise - plain enum, no bitmasks except for #DEFMASK are allowed)

inline bool     is_bf(enum_t id)                        { return (netnode(id).altval(ENUM_FLAGS) & ENUM_FLAGS_IS_BF) != 0; }


/// Is enum collapsed?

inline bool     is_enum_hidden(enum_t id)               { return (netnode(id).altval(ENUM_FLAGS) & ENUM_FLAGS_HIDDEN) != 0; }

/// Collapse enum

inline bool     set_enum_hidden(enum_t id, bool hidden) { return set_enum_flag(id, ENUM_FLAGS_HIDDEN, hidden); }


/// Does enum come from type library?

inline bool     is_enum_fromtil(enum_t id)               { return (netnode(id).altval(ENUM_FLAGS) & ENUM_FLAGS_FROMTIL) != 0; }


/// Specify that enum comes from a type library

inline bool     set_enum_fromtil(enum_t id, bool fromtil) { return set_enum_flag(id, ENUM_FLAGS_FROMTIL, fromtil); }


/// Is a ghost copy of a local type?

inline bool     is_ghost_enum(enum_t id)                  { return (netnode(id).altval(ENUM_FLAGS) & ENUM_FLAGS_GHOST) != 0; }


/// Specify that enum is a ghost copy of a local type

inline bool     set_enum_ghost(enum_t id, bool ghost)     { return set_enum_flag(id, ENUM_FLAGS_GHOST, ghost); }


/// Get name of enum

inline ssize_t  get_enum_name(qstring *out, enum_t id) { return id > MAXADDR ? netnode(id).get_name(out) : -1;  }

inline qstring get_enum_name(tid_t id)
{
  qstring name;
  get_enum_name(&name, id);
  return name;
}

/// Get the width of a enum element.
/// 0 means unspecified, 1 means 1 byte, 2 means 2 bytes, 3 means 4 bytes, 4 means 8 bytes, etc.
/// max allowed value is 7.

inline size_t   get_enum_width(enum_t id)                { return id > MAXADDR ? size_t((netnode(id).altval(ENUM_FLAGS) & ENUM_FLAGS_WIDTH) >> 3) : 0; }

/// See comment for get_enum_width()

idaman bool ida_export set_enum_width(enum_t id, int width);


/// Get enum comment

inline ssize_t  get_enum_cmt(enum_t id, bool repeatable, char *buf, size_t bufsize) { return id > MAXADDR ? netnode(id).supstr(repeatable != 0, buf, bufsize) : -1; }


/// Get the number of the members of the enum

inline size_t   get_enum_size(enum_t id)                { return id > MAXADDR ? size_t(netnode(id).altval(ENUM_QTY_IDX)) : 0; }


/// Get flags determining the representation of the enum.
/// (currently they define the numeric base: octal, decimal, hex, bin) and signness.

inline flags_t  get_enum_flag(enum_t id)                { return id > MAXADDR ? flags_t(netnode(id).altval(ENUM_FLG_IDX)) : 0; }


/// Get a reference to an enum member by its name

idaman const_t ida_export get_enum_member_by_name(const char *name);


/// Get value of an enum member

inline uval_t   get_enum_member_value(const_t id)             { return id > MAXADDR ? netnode(id).altval(CONST_VALUE) : 0; }


/// Get the parent enum of an enum member

inline enum_t   get_enum_member_enum(const_t id)              { return id > MAXADDR ? netnode(id).altval(CONST_ENUM)-1 : BADNODE; }


/// Get bitmask of an enum member

inline bmask_t  get_enum_member_bmask(const_t id)             { return id > MAXADDR ? netnode(id).altval(CONST_BMASK)-1 : BADNODE; }


/// Find an enum member by enum, value and bitmask

idaman const_t ida_export get_enum_member(enum_t id, uval_t value, uchar serial, bmask_t mask);

/// \name Access to all used bitmasks in an enum
//@{
inline bmask_t  get_first_bmask(enum_t id)              { return netnode(id).alt1st(ENUM_MASKS); }
inline bmask_t  get_last_bmask(enum_t id)               { return netnode(id).altlast(ENUM_MASKS); }
inline bmask_t  get_next_bmask(enum_t id,bmask_t bmask) { return netnode(id).altnxt(bmask,ENUM_MASKS); }
inline bmask_t  get_prev_bmask(enum_t id,bmask_t bmask) { return netnode(id).altprev(bmask,ENUM_MASKS); }
//@}

/// \name Access to all enum members with specified bitmask
/// \note these functions return values, not ::const_t!
//@{
idaman uval_t ida_export get_first_enum_member(enum_t id, bmask_t bmask=DEFMASK);
idaman uval_t ida_export get_last_enum_member(enum_t id, bmask_t bmask=DEFMASK);
idaman uval_t ida_export get_next_enum_member(enum_t id, uval_t value, bmask_t bmask=DEFMASK);
idaman uval_t ida_export get_prev_enum_member(enum_t id, uval_t value, bmask_t bmask=DEFMASK);
//@}


/// Get name of an enum member by const_t

inline ssize_t  get_enum_member_name(qstring *out, const_t id) { return netnode(id).get_name(out); }


/// Get enum member's comment

inline ssize_t  get_enum_member_cmt(const_t id, bool repeatable, char *buf, size_t bufsize) { return netnode(id).supstr(repeatable != 0, buf, bufsize); }


/// \name Access to all enum members with specified value and mask
/// A sample loop looks like this:
/// \code
///   const_t main_cid;
///   uchar serial;
///   for ( const_t cid=main_cid=get_first_serial_enum_member(id, v, &serial, mask);
///         cid != BADNODE;
///         cid = get_next_serial_enum_member(main_cid, &serial) )
///   {
///     ...
///   }
/// \endcode
/// The 'out_serial' argument of get_first_serial_enum_member/get_last_serial_enum_member can be NULL.
/// The 'in_out_serial' is required for the other functions.
//@{
idaman const_t ida_export get_first_serial_enum_member(enum_t id, uval_t value, uchar *out_serial, bmask_t bmask);
idaman const_t ida_export get_last_serial_enum_member(enum_t id, uval_t value, uchar *out_serial, bmask_t bmask);
idaman const_t ida_export get_next_serial_enum_member(const_t first_cid, uchar *in_out_serial);
idaman const_t ida_export get_prev_serial_enum_member(const_t first_cid, uchar *in_out_serial);
//@}

/// Enum member visitor - see for_all_enum_members().
/// Derive your visitor from this class.
struct enum_member_visitor_t
{
  /// Implements action to take when enum member is visited.
  /// \return nonzero to stop the iteration
  virtual int idaapi visit_enum_member(const_t cid, uval_t value) = 0;
};

/// Visit all members of a given enum

idaman int ida_export for_all_enum_members(enum_t id, enum_member_visitor_t &cv);


/// Get serial number of an enum member

inline uchar get_enum_member_serial(const_t cid) { return uchar(netnode(cid).altval(CONST_SERIAL)); }

/// Get corresponding type ordinal number

inline int32 get_enum_type_ordinal(enum_t id) { return int32(netnode(id).altval(ENUM_ORDINAL)-1); }

/// Set corresponding type ordinal number

inline void set_enum_type_ordinal(enum_t id, int32 ord) { netnode(id).altset(ENUM_ORDINAL, ord+1); }

//--------------------------------------------------------------------------
// MANIPULATION

/// Add new enum type.
///   - if idx==#BADADDR then add as the last idx
///   - if name==NULL then generate a unique name "enum_%d"

idaman enum_t ida_export add_enum(size_t idx, const char *name, flags_t flag);


/// Delete an enum type

idaman void   ida_export del_enum(enum_t id);


/// Set serial number of enum.
/// Also see get_enum_idx().

idaman bool   ida_export set_enum_idx(enum_t id, size_t idx);


/// Set 'bitfield' bit of enum (i.e. convert it to a bitfield)

idaman bool ida_export set_enum_bf(enum_t id, bool bf);


/// Set name of enum type

idaman bool ida_export set_enum_name(enum_t id,const char *name);


/// Set comment for enum type

idaman bool ida_export set_enum_cmt(enum_t id,const char *cmt,bool repeatable);


/// Set data representation flags

inline bool set_enum_flag(enum_t id, flags_t flag) { return netnode(id).altset(ENUM_FLG_IDX,flag) != 0; }


/// Add member to enum type.
/// \return 0 if ok, otherwise one of \ref ENUM_MEMBER_

idaman int ida_export add_enum_member(enum_t id,const char *name, uval_t value, bmask_t bmask=DEFMASK);

/// \defgroup ENUM_MEMBER_ Add enum member result codes
/// Return values for add_enum_member()
//@{
#define ENUM_MEMBER_ERROR_NAME  1     ///< already have member with this name (bad name)
#define ENUM_MEMBER_ERROR_VALUE 2     ///< already have 256 members with this value
#define ENUM_MEMBER_ERROR_ENUM  3     ///< bad enum id
#define ENUM_MEMBER_ERROR_MASK  4     ///< bad bmask
#define ENUM_MEMBER_ERROR_ILLV  5     ///< bad bmask and value combination (~bmask & value != 0)
//@}

/// Delete member of enum type

idaman bool ida_export del_enum_member(enum_t id, uval_t value, uchar serial, bmask_t bmask);

/// Set name of enum member

idaman bool ida_export set_enum_member_name(const_t id, const char *name);

/// Set comment for enum member

inline bool set_enum_member_cmt(const_t id, const char *cmt, bool repeatable) { return set_enum_cmt(id, cmt, repeatable); }


///  Is bitmask one bit?

inline bool is_one_bit_mask(bmask_t mask)
{
  return (mask & (mask-1)) == 0;
}


/// Get bitmask node if bf-scheme is used,
/// otherwise return #BADNODE

inline netnode get_bmask_node(enum_t id, bmask_t bmask)
{
  if ( !is_bf(id) ) return BADNODE;
  return netnode(id).altval(bmask, ENUM_MASKS)-1;
}


/// \name Work with the bitmask name & comment
//@{
inline bool set_bmask_name(enum_t id, bmask_t bmask, const char *name)
  { return get_bmask_node(id, bmask).rename(name); }
inline ssize_t get_bmask_name(qstring *out, enum_t id, bmask_t bmask)
  { return get_bmask_node(id, bmask).get_name(out); }
inline bool set_bmask_cmt(enum_t id, bmask_t bmask, const char *cmt, bool repeatable)
  { return set_enum_cmt(get_bmask_node(id, bmask), cmt, repeatable); }
inline ssize_t get_bmask_cmt(enum_t id, bmask_t bmask, bool repeatable, char *buf, size_t bufsize)
  { return get_bmask_node(id, bmask).supstr(repeatable != 0, buf, bufsize); }
//@}

// kernel helper functions:
/// \cond
inline uval_t get_selected_enum(size_t n) { return enums.altval(n,ENUM_SELMEMS); }
inline void   add_selected_enum(size_t *idx, enum_t id) { enums.altset((*idx)++,id,ENUM_SELMEMS); }
inline void   unmark_selected_enums(void){ enums.altdel_all(ENUM_SELMEMS); }
inline bool set_enum_flag(enum_t id, uint32 bit, bool set)
{
  if ( id == BADNODE ) return false;
  netnode n(id);
  uint32 f = uint32(n.altval(ENUM_FLAGS));
  setflag(f, bit, set);
  return n.altset(ENUM_FLAGS, f) != 0;
}
inline void init_enums(void) { enums.create("$ enums"); }
inline void save_enums(void) {}
inline void term_enums(void) {}
/// \endcond

//------------------------------------------------------------------------

#ifndef NO_OBSOLETE_FUNCS
// old api names (removed because of name clash with MSVC10)
idaman DEPRECATED int ida_export add_const(enum_t id,const char *name, uval_t value, bmask_t bmask=DEFMASK);
#define CONST_ERROR_NAME  1
#define CONST_ERROR_VALUE 2
#define CONST_ERROR_ENUM  3
#define CONST_ERROR_MASK  4
#define CONST_ERROR_ILLV  5
idaman DEPRECATED bool ida_export del_const(enum_t id, uval_t value, uchar serial, bmask_t bmask);
idaman DEPRECATED bool ida_export set_const_name(const_t id, const char *name);
inline DEPRECATED bool set_const_cmt(const_t id, const char *cmt, bool repeatable) { return set_enum_cmt(id, cmt, repeatable); }
idaman DEPRECATED const_t ida_export get_const_by_name(const char *name);
inline DEPRECATED uval_t   get_const_value(const_t id)             { return id > MAXADDR ? netnode(id).altval(CONST_VALUE) : 0; }
inline DEPRECATED enum_t   get_const_enum(const_t id)              { return id > MAXADDR ? netnode(id).altval(CONST_ENUM)-1 : BADNODE; }
inline DEPRECATED bmask_t  get_const_bmask(const_t id)             { return id > MAXADDR ? netnode(id).altval(CONST_BMASK)-1 : BADNODE; }
idaman DEPRECATED const_t ida_export get_const(enum_t id, uval_t value, uchar serial, bmask_t mask);
idaman DEPRECATED uval_t ida_export get_first_const(enum_t id, bmask_t bmask=DEFMASK);
idaman DEPRECATED uval_t ida_export get_last_const(enum_t id, bmask_t bmask=DEFMASK);
idaman DEPRECATED uval_t ida_export get_next_const(enum_t id, uval_t value, bmask_t bmask=DEFMASK);
idaman DEPRECATED uval_t ida_export get_prev_const(enum_t id, uval_t value, bmask_t bmask=DEFMASK);
inline DEPRECATED ssize_t get_const_name(const_t id, char *buf, size_t bufsize) { return netnode_name(id, buf, bufsize); }
inline DEPRECATED ssize_t get_const_cmt(const_t id, bool repeatable, char *buf, size_t bufsize) { return netnode(id).supstr(repeatable != 0, buf, bufsize); }
idaman DEPRECATED const_t ida_export get_first_serial_const(enum_t id, uval_t value, uchar *serial, bmask_t bmask);
idaman DEPRECATED const_t ida_export get_last_serial_const(enum_t id, uval_t value, uchar *serial, bmask_t bmask);
idaman DEPRECATED const_t ida_export get_next_serial_const(const_t first_cid, uchar *serial);
idaman DEPRECATED const_t ida_export get_prev_serial_const(const_t first_cid, uchar *serial);
struct const_visitor_t { virtual int idaapi visit_const(const_t cid, uval_t value) = 0; };
idaman DEPRECATED int ida_export for_all_consts(enum_t id, const_visitor_t &cv);
inline DEPRECATED uchar get_const_serial(const_t cid) { return uchar(netnode(cid).altval(CONST_SERIAL)); }
#endif

#ifndef ENUM_SOURCE
#define enums   dont_use_enums_directly         ///< prohibit access to 'enums' netnode

#undef ENUM_REVERSE
#undef ENUM_SELMEMS
#undef ENUM_QTY_IDX
#undef ENUM_FLG_IDX
#undef ENUM_FLAGS
#undef ENUM_FLAGS_IS_BF
#undef ENUM_FLAGS_HIDDEN
#undef ENUM_MASKS
#undef ENUM_MEMBERS
#undef ENUM_ORDINAL
#undef CONST_ENUM
#undef CONST_VALUE
#undef CONST_BMASK
#undef CONST_SERIAL
#undef CONST_SERIALS

#endif

#pragma pack(pop)
#endif // _ENUM_HPP
