/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2015 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 */

/*! \file struct.hpp

  \brief Structure type management (assembly level types)

*/

#ifndef _STRUCT_HPP
#define _STRUCT_HPP
#include <bytes.hpp>
#pragma pack(push, 1)           // IDA uses 1 byte alignments!

#define STRUC_SEPARATOR '.'     ///< structname.fieldname

struct til_t;

/// Describes a member of an assembly level structure
class member_t
{
public:
  tid_t id;             ///< name(), cmt, rptcmt
  ea_t soff;            ///< start offset (for unions - number of the member 0..n)
  ea_t eoff;            ///< end offset
  flags_t flag;         ///< type+representation bits
//private:
  uint32 props;         ///< \ref MF_
/// \defgroup MF_ Struct member properties
/// Used by member_t::props
//@{
#define MF_OK     0x00000001    ///< is the member ok? (always yes)
#define MF_UNIMEM 0x00000002    ///< is a member of a union?
#define MF_HASUNI 0x00000004    ///< has members of type "union"?
#define MF_BYTIL  0x00000008    ///< the member was created due to the type system
#define MF_HASTI  0x00000010    ///< has type information?
//@}

  /// Is a member of a union?
  bool unimem(void)    const { return (props & MF_UNIMEM) != 0; }
  /// Has members of type "union"?
  bool has_union(void) const { return (props & MF_HASUNI) != 0; }
  /// Was the member created due to the type system?
  bool by_til(void)    const { return (props & MF_BYTIL)  != 0; }
  /// Has type information?
  bool has_ti(void)    const { return (props & MF_HASTI)  != 0; }
  /// Get start offset (for unions - returns 0)
  ea_t get_soff(void) const { return unimem() ? 0 : soff; }
};

/// Information about a structure type (assembly level)
class struc_t
{
protected:
  struc_t(void) {}      ///< plugins may not create struc_t instances. they should
                        ///< use add_struc() and get_struc()
public:
  tid_t id;             ///< struct id
  size_t memqty;        ///< number of members
  member_t *members;    ///< only defined members are stored here.
                        ///< there may be gaps between members.
  ushort age;           ///< not used
  uint32 props;         ///< \ref SF_
/// \defgroup SF_ Structure properties
/// Used by struc_t::props
//@{
#define SF_VAR    0x00000001    ///< is variable size structure (varstruct)?
                                ///< a variable size structure is one with
                                ///< the zero size last member.
                                ///< if the last member is a varstruct, then the current
                                ///< structure is a varstruct too.
#define SF_UNION  0x00000002    ///< is a union?
                                ///< varunions are prohibited!
#define SF_HASUNI 0x00000004    ///< has members of type "union"?
#define SF_NOLIST 0x00000008    ///< don't include in the chooser list
#define SF_TYPLIB 0x00000010    ///< the structure comes from type library
#define SF_HIDDEN 0x00000020    ///< the structure is collapsed
#define SF_FRAME  0x00000040    ///< the structure is a function frame
#define SF_ALIGN  0x00000F80    ///< alignment (shift amount: 0..31)
#define SF_GHOST  0x00001000    ///< ghost copy of a local type
//@}

  /// Is variable size structure?
  bool is_varstr(void)    const { return (props & SF_VAR)    != 0; }
  /// Is a union?
  bool is_union(void)     const { return (props & SF_UNION)  != 0; }
  /// Has members of type "union"?
  bool has_union(void)    const { return (props & SF_HASUNI) != 0; }
  /// Is included in chooser list?
  /// Use \ref set_struc_listed to change the listed status
  bool is_choosable(void) const { return (props & SF_NOLIST) == 0; }
  /// Does structure come from a type library?
  bool from_til(void)     const { return (props & SF_TYPLIB) != 0; }
  /// Is the structure collapsed?
  /// Use \ref set_struc_hidden to change the hidden status
  bool is_hidden(void)    const { return (props & SF_HIDDEN) != 0; }
  /// See #SF_ALIGN
  int get_alignment(void) const { return (props & SF_ALIGN) >> 7; }
  /// Is a ghost copy of a local type?
  bool is_ghost(void)  const    { return (props & SF_GHOST)  != 0; }

  /// Do not use; use set_struc_align()
  void set_alignment(int shift)
  {
    props &= ~SF_ALIGN;
    props |= (shift << 7) & SF_ALIGN;
  }

  void set_ghost(bool _is_ghost) { setflag(props, SF_GHOST, _is_ghost); }

  mutable int32 ordinal;        ///< corresponding local type ordinal number
};

/// \name Internal structures list
/// IDA maintains an internal vector of known structures.
/// Use these functions to work with this vector.
//@{

/// Get number of known structures

idaman size_t ida_export get_struc_qty(void);


/// Get index of first structure.
/// \return #BADADDR if no known structures, 0 otherwise

idaman uval_t ida_export get_first_struc_idx(void);


/// Get index of last structure.
/// \return #BADADDR if no known structures, get_struc_qty()-1 otherwise

idaman uval_t ida_export get_last_struc_idx(void);


/// Get previous struct index.
/// \return #BADADDR if resulting index is negative, otherwise idx - 1

inline uval_t get_prev_struc_idx(uval_t idx) { return (idx==BADNODE) ? idx : idx - 1; }


/// Get next struct index.
/// \return #BADADDR if resulting index is out of bounds, otherwise idx++

idaman uval_t ida_export get_next_struc_idx(uval_t idx);


/// Get internal number of the structure

idaman uval_t ida_export get_struc_idx(tid_t id);


/// Get struct id by struct number

idaman tid_t  ida_export get_struc_by_idx(uval_t idx);


/// Get pointer to struct type info

idaman struc_t *ida_export get_struc(tid_t id);

//@}


/// Get struct id by name

inline tid_t get_struc_id(const char *name)
{
  tid_t id = netnode(name);
  return get_struc(id) == NULL ? BADADDR : id;
}


/// Get struct name by id

inline ssize_t get_struc_name(qstring *out, tid_t id) { return netnode(id).get_name(out); }

inline qstring get_struc_name(tid_t id)
{
  qstring name;
  get_struc_name(&name, id);
  return name;
}

/// Get struct comment

inline ssize_t get_struc_cmt(tid_t id, bool repeatable, char *buf, size_t bufsize) { return netnode(id).supstr(repeatable != 0, buf, bufsize); }


/// Get struct size (also see get_struc_size(tid_t))

idaman asize_t ida_export get_struc_size(const struc_t *sptr);

/// Get struct size (also see get_struc_size(const struc_t *))

inline asize_t get_struc_size(tid_t id) { return get_struc_size(get_struc(id)); }


/// \name Struct offsets
/// \note for unions, soff == number of the current member
//@{

/// Get offset of member with largest offset less than 'offset'.
/// \return #BADADDR if no prev offset

idaman ea_t ida_export get_struc_prev_offset(const struc_t *sptr, ea_t offset);


/// Get offset of member with smallest offset larger than 'offset'.
/// \return #BADADDR if no next offset

idaman ea_t ida_export get_struc_next_offset(const struc_t *sptr, ea_t offset);


/// Get offset of last member.
/// \return #BADADDR if memqty == 0

idaman ea_t ida_export get_struc_last_offset(const struc_t *sptr);


/// Get offset of first member.
/// \return #BADADDR if memqty == 0

idaman ea_t ida_export get_struc_first_offset(const struc_t *sptr);


/// For unions: returns number of members, for structs: returns size of structure

inline ea_t get_max_offset(struc_t *sptr)
{
  if ( sptr == NULL ) return 0; // just to avoid GPF
  return sptr->is_union()
                ? sptr->memqty
                : get_struc_size(sptr);
}

//@}

/// Is variable size structure?

inline bool is_varstr(tid_t id)
{
  struc_t *sptr = get_struc(id);
  return sptr != NULL && sptr->is_varstr();
}

/// Is a union?

inline bool is_union(tid_t id)
{
  struc_t *sptr = get_struc(id);
  return sptr != NULL && sptr->is_union();
}


/// Get containing structure of member by its full name "struct.field"

idaman struc_t  *ida_export get_member_struc(const char *fullname);


/// Get child struct if member is a struct

idaman struc_t  *ida_export get_sptr(const member_t *mptr);


/// Get member at given offset

idaman member_t *ida_export get_member(const struc_t *sptr, asize_t offset);


/// Get a member by its name, like "field44"

idaman member_t *ida_export get_member_by_name(const struc_t *sptr,const char *membername);


/// Get a member by its fully qualified name, "struct.field"

idaman member_t *ida_export get_member_by_fullname(const char *fullname, struc_t **sptr_place);


/// Get a member's fully qualified name, "struct.field"

inline ssize_t idaapi get_member_fullname(qstring *out, tid_t mid) { return netnode(mid).get_name(out); }


/// Get name of structure member

idaman ssize_t ida_export get_member_name2(qstring *out, tid_t mid);

inline qstring get_member_name2(tid_t mid)
{
  qstring name;
  get_member_name2(&name, mid);
  return name;
}


/// Get comment of structure member

inline ssize_t idaapi get_member_cmt(tid_t mid, bool repeatable, char *buf, size_t bufsize) { return netnode(mid).supstr(repeatable != 0, buf, bufsize); }


/// Get size of structure member.
/// May return 0 for the last member of varstruct.
/// For union members, returns member_t::eoff.

inline asize_t get_member_size(const member_t *mptr)     { return mptr->unimem() ? mptr->eoff : (mptr->eoff - mptr->soff); }


/// Is variable size member?

idaman bool ida_export is_varmember(const member_t *mptr);


/// Get member that is most likely referenced by the specified offset.
/// Useful for offsets > sizeof(struct).

idaman member_t *ida_export get_best_fit_member(const struc_t *sptr, asize_t offset);


/// Get the next member idx, if it does not exist, return -1

idaman ssize_t ida_export get_next_member_idx(const struc_t *sptr, asize_t off);


/// Get the prev member idx, if it does not exist, return -1

idaman ssize_t ida_export get_prev_member_idx(const struc_t *sptr, asize_t off);

//--------------------------------------------------------------------------
// manipulation

/// Create a structure type.
/// if idx==#BADADDR then add as the last idx.
/// if name==NULL then a name will be generated "struct_%d".

idaman tid_t ida_export add_struc(uval_t idx, const char *name, bool is_union=false);


/// Delete a structure type

idaman bool ida_export del_struc(struc_t *sptr);


/// Set internal number of struct.
/// Also see get_struc_idx(), get_struc_by_idx().

idaman bool ida_export set_struc_idx(const struc_t *sptr, uval_t idx);


/// Set structure alignment (#SF_ALIGN)

idaman bool ida_export set_struc_align(struc_t *sptr, int shift);


/// Set structure name

idaman bool ida_export set_struc_name(tid_t id, const char *name);


/// Set structure comment

idaman bool ida_export set_struc_cmt(tid_t id, const char *cmt, bool repeatable);


/// Return values for add_struc_member()
enum struc_error_t
{
  STRUC_ERROR_MEMBER_OK      = 0,  ///< success
  STRUC_ERROR_MEMBER_NAME    = -1, ///< already has member with this name (bad name)
  STRUC_ERROR_MEMBER_OFFSET  = -2, ///< already has member at this offset
  STRUC_ERROR_MEMBER_SIZE    = -3, ///< bad number of bytes or bad sizeof(type)
  STRUC_ERROR_MEMBER_TINFO   = -4, ///< bad typeid parameter
  STRUC_ERROR_MEMBER_STRUCT  = -5, ///< bad struct id (the 1st argument)
  STRUC_ERROR_MEMBER_UNIVAR  = -6, ///< unions can't have variable sized members
  STRUC_ERROR_MEMBER_VARLAST = -7, ///< variable sized member should be the last member in the structure
  STRUC_ERROR_MEMBER_NESTED  = -8, ///< recursive structure nesting is forbidden
};


/// Add member to existing structure.
/// \param sptr       structure to modify
/// \param fieldname  if NULL, then "anonymous_#" name will be generated
/// \param offset     #BADADDR means add to the end of structure
/// \param flag       type + representation bits
/// \param mt         additional info about member type.
///                   must be present for
///                   structs, offsets, enums, strings,
///                   struct offsets.
/// \param nbytes     if == 0 then the structure
///                   will be a varstruct.
///                   in this case the member should be
///                   the last member in the structure

idaman struc_error_t ida_export add_struc_member(
                  struc_t *sptr,
                  const char *fieldname,
                  ea_t offset,
                  flags_t flag,
                  const opinfo_t *mt,
                  asize_t nbytes);


/// Delete member at given offset

idaman bool ida_export del_struc_member(struc_t *sptr, ea_t offset);


/// Delete members which occupy range of offsets (off1..off2).
/// \return number of deleted members or -1 on error

idaman int ida_export del_struc_members(struc_t *sptr, ea_t off1, ea_t off2);


/// Set name of member at given offset

idaman bool ida_export set_member_name(struc_t *sptr, ea_t offset,const char *name);


/// Set type of member at given offset (also see add_struc_member())

idaman bool ida_export set_member_type(struc_t *sptr, ea_t offset, flags_t flag,const opinfo_t *mt, asize_t nbytes);


/// Set member comment

idaman bool ida_export set_member_cmt(member_t *mptr,const char *cmt, bool repeatable);


/// Expand/Shrink structure type

idaman bool ida_export expand_struc(struc_t *sptr, ea_t offset, adiff_t delta, bool recalc=true);


/// Update struct information in the database (internal function)

idaman void ida_export save_struc2(struc_t *sptr, bool may_update_ltypes=true);


/// Hide/unhide a struct type
inline void idaapi set_struc_hidden(struc_t *sptr, bool is_hidden)
{
  setflag(sptr->props, SF_HIDDEN, is_hidden);
  save_struc2(sptr, false);
}

/// Add/remove a struct type from the struct list
inline void idaapi set_struc_listed(struc_t *sptr, bool is_listed)
{
  setflag(sptr->props, SF_NOLIST, !is_listed);
  save_struc2(sptr, false);
}

/// Member type information (return values for set_member_tinfo2())
enum smt_code_t
{
  SMT_BADARG = -6,   ///< bad parameters
  SMT_NOCOMPAT = -5, ///< the new type is not compatible with the old type
  SMT_WORSE = -4,    ///< the new type is worse than the old type
  SMT_SIZE = -3,     ///< the new type is incompatible with the member size
  SMT_ARRAY = -2,    ///< arrays are forbidden as function arguments
  SMT_OVERLAP = -1,  ///< member would overlap with members that can not be deleted
  SMT_FAILED = 0,    ///< failed to set new member type
  SMT_OK = 1,        ///< success: changed the member type
  SMT_KEEP = 2,      ///< no need to change the member type, the old type is better
};


/// Get tinfo for given member

idaman bool ida_export get_member_tinfo2(const member_t *mptr, tinfo_t *tif);


/// Delete tinfo for given member

idaman bool ida_export del_member_tinfo(struc_t *sptr, member_t *mptr);


/// Set tinfo for given member.
/// \param sptr    containing struct
/// \param mptr    target member
/// \param memoff  offset within member
/// \param tif     type info
/// \param flags   \ref SET_MEMTI_

idaman smt_code_t ida_export set_member_tinfo2(
        struc_t *sptr,
        member_t *mptr,
        uval_t memoff,
        const tinfo_t &tif,
        int flags);

/// \defgroup SET_MEMTI_ Set member tinfo flags
/// Passed as 'flags' parameter to set_member_tinfo2()
//@{
#define SET_MEMTI_MAY_DESTROY 0x0001 ///< may destroy other members
#define SET_MEMTI_COMPATIBLE  0x0002 ///< new type must be compatible with the old
#define SET_MEMTI_FUNCARG     0x0004 ///< mptr is function argument (can not create arrays)
#define SET_MEMTI_BYTIL       0x0008 ///< new type was created by the type subsystem
//@}


/// Try to get tinfo for given member - if failed, generate a tinfo using information about the
/// member id from the disassembly

idaman bool ida_export get_or_guess_member_tinfo2(const member_t *mptr, tinfo_t *tif);


/// Get operand type info for member

inline opinfo_t *retrieve_member_info(const member_t *mptr, opinfo_t *buf)
{
  if ( mptr == NULL )
    return NULL;
  return get_opinfo(mptr->id, 0, mptr->flag, buf);
}


/// Is member name prefixed with "anonymous"?

inline bool is_anonymous_member_name(const char *name)
{
  return name == NULL
      || strncmp(name, "anonymous", 9) == 0;
}


/// Is member name an auto-generated name?

inline bool is_dummy_member_name(const char *name)
{
  return name == NULL
      || strncmp(name, "arg_", 4) == 0
      || strncmp(name, "var_", 4) == 0
      || is_anonymous_member_name(name);
}


/// Check if the specified member id points to a struct member

inline member_t *idaapi get_member_by_id(
        qstring *out,
        tid_t mid,
        struc_t **sptr_place)
{
  if ( get_member_fullname(out, mid) > 0 )
    return get_member_by_fullname(out->begin(), sptr_place);
  return NULL;
}


/// Check if the specified member id points to a struct member. convenience function

inline member_t *idaapi get_member_by_id(tid_t mid, struc_t **sptr_place=NULL)
{
  qstring buf;
  return get_member_by_id(&buf, mid, sptr_place);
}


/// Is a member id?

inline bool is_member_id(tid_t mid)
{
  return get_member_by_id(mid) != NULL;
}


/// Is a special member with the name beginning with ' '?

idaman bool ida_export is_special_member(tid_t id);


/// Implements action to take when a field is visited with visit_stroff_fields()
struct ida_local struct_field_visitor_t
{
  virtual int idaapi visit_field(struc_t *sptr, member_t *mptr) = 0;
};


//--------------------------------------------------------------------------
/// Visit structure fields in a stroff expression or in a reference to a struct data variable.
/// This function can be used to enumerate all components of an expression like 'a.b.c'.
/// \param sfv           visitor object
/// \param path          struct path (path[0] contains the initial struct id)
/// \param plen          len
/// \param[in,out] disp  offset into structure
/// \param appzero       should visit field at offset zero?

idaman flags_t ida_export visit_stroff_fields(
        struct_field_visitor_t &sfv,
        const tid_t *path,
        int plen,
        adiff_t *disp,
        bool appzero);


//--------------------------------------------------------------------------
/// Should display a structure offset expression as the structure size?

inline bool stroff_as_size(int plen, const struc_t *sptr, asize_t value)
{
  return plen == 1
      && value > 0
      && sptr != NULL
      && !sptr->is_varstr()
      && value == get_struc_size(sptr);
}

//-------------------------------------------------------------------------
//      F U N C T I O N S   F O R   T H E   K E R N E L
//-------------------------------------------------------------------------

///-------------------------------------------------------------------\cond
inline void save_structs(void) {}
///----------------------------------------------------------------\endcond

//------------------------------------------------------------------------

#ifndef NO_OBSOLETE_FUNCS
idaman DEPRECATED void ida_export save_struc(struc_t *sptr); // update struct information in the database (internal function)
idaman DEPRECATED bool ida_export get_or_guess_member_type(const member_t *mptr, type_t *type, size_t tsize);
idaman DEPRECATED bool ida_export get_member_ti(const member_t *mptr, type_t *buf, size_t bufsize);
idaman DEPRECATED bool ida_export set_member_ti(struc_t *sptr, member_t *mptr, const type_t *type, int flags);
inline DEPRECATED bool del_ti(struc_t *sptr, member_t *mptr) { return del_member_tinfo(sptr, mptr); }
idaman DEPRECATED bool ida_export get_or_guess_member_tinfo(const member_t *mptr, qtype *type, qtype *fields);
idaman DEPRECATED bool ida_export get_member_tinfo(const member_t *mptr, qtype *buf, qtype *fields);
idaman DEPRECATED bool ida_export set_member_tinfo(const til_t *til, struc_t *sptr, member_t *mptr, uval_t memoff, const type_t *type, const p_list *fields, int flags);
idaman DEPRECATED ssize_t ida_export get_member_name(tid_t mid, char *buf, size_t bufsize);
#endif


#pragma pack(pop)
#endif // _STRUCT_HPP
