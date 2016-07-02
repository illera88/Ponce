/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2015 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 */

#ifndef _XREF_HPP
#define _XREF_HPP
#pragma pack(push, 1)           // IDA uses 1 byte alignments!

/*! \file xref.hpp

  \brief Functions that deal with cross-references

  There are 2 types of xrefs: CODE and DATA references.
  All xrefs are kept in the bTree except ordinary execution flow
  to the next instruction. Ordinary execution flow to
  the next instruction is kept in flags (see bytes.hpp).

  Cross-references are automatically sorted.
*/

class func_t;   // #include <funcs.hpp>
class member_t; // #include <struct.hpp>

//---------------------------------------------------------------------------
//      C R O S S - R E F E R E N C E   T Y P E S
//---------------------------------------------------------------------------

/// \defgroup xref_type Cross-Reference types
/// Describe categories of code/data xrefs.
/// \note IDA handles the xrefs automatically and may delete an xref
/// added by the user if it does not contain the #XREF_USER bit.
//@{

/// CODE xref types
enum cref_t
{
  fl_U,                         ///< unknown -- for compatibility with old
                                ///< versions. Should not be used anymore.
  fl_CF = 16,                   ///< Call Far
                                ///< This xref creates a function at the
                                ///< referenced location
  fl_CN,                        ///< Call Near
                                ///< This xref creates a function at the
                                ///< referenced location
  fl_JF,                        ///< Jump Far
  fl_JN,                        ///< Jump Near
  fl_USobsolete,                ///< User specified (obsolete)
  fl_F,                         ///< Ordinary flow: used to specify execution
                                ///< flow to the next instruction.
};

/// DATA xref types
enum dref_t
{
  dr_U,                         ///< Unknown -- for compatibility with old
                                ///< versions. Should not be used anymore.
  dr_O,                         ///< Offset
                                ///< The reference uses 'offset' of data
                                ///< rather than its value
                                ///<    OR
                                ///< The reference appeared because the "OFFSET"
                                ///< flag of instruction is set.
                                ///< The meaning of this type is IDP dependent.
  dr_W,                         ///< Write access
  dr_R,                         ///< Read access
  dr_T,                         ///< Text (for forced operands only)
                                ///< Name of data is used in manual operand
  dr_I,                         ///< Informational
                                ///< (a derived java class references its base
                                ///<  class informationally)
};


/// \defgroup XREF_T Cross-Reference type flags
/// These flags are combined with a ::cref_t or ::dref_t value to describe the
/// type of a cross reference.
/// These bits are accessible to the kernel.
//@{
#define XREF_USER       0x20    ///< User specified xref.
                                ///< This xref will not be deleted by IDA.
                                ///< This bit should be combined with
                                ///< the existing xref types (::cref_t & ::dref_t)
#define XREF_TAIL       0x40    ///< Reference to tail byte in extrn symbols
#define XREF_BASE       0x80    ///< Reference to the base part of an offset
#define XREF_MASK       0x1F    ///< Mask to get xref type

#define XREF_PASTEND    0x100   ///< Reference is past item. This bit may be passed to
                                ///< add_dref() functions but it won't be saved in
                                ///< the database. It will prevent the destruction
                                ///< of eventual alignment directives.
//@}

/// Get character describing the xref type.
/// \param xrtype  combination of \ref XREF_T and a ::cref_t of ::dref_t value
idaman char ida_export xrefchar(char xrtype);

//@} xref_type

//---------------------------------------------------------------------------
//      A D D / D E L E T E   C R O S S - R E F E R E N C E S
//---------------------------------------------------------------------------

/// Create a code cross-reference.
/// \param from  linear address of referencing instruction
/// \param to    linear address of referenced  instruction
/// \param type  cross-reference type
/// \return success

idaman bool ida_export add_cref(ea_t from, ea_t to, cref_t type);


/// Delete a code cross-reference.
/// \param from    linear address of referencing instruction
/// \param to      linear address of referenced  instruction
/// \param expand  policy to delete the referenced instruction
///                  - 1: plan to delete the referenced instruction if it has
///                       no more references.
///                  - 0: don't delete the referenced instruction even if no
///                       more cross-references point to it
/// \retval 1  the referenced instruction will     be deleted
/// \retval 0  the referenced instruction will not be deleted

idaman int ida_export del_cref(ea_t from, ea_t to, bool expand);


/// Create a data cross-reference.
/// \param from  linear address of referencing instruction or data
/// \param to    linear address of referenced  data
/// \param type  cross-reference type
/// \return success (may fail if user-defined xref exists from->to)

idaman bool ida_export add_dref(ea_t from, ea_t to, dref_t type);


/// Delete a data cross-reference.
/// \param from  linear address of referencing instruction or data
/// \param to    linear address of referenced  data

idaman void ida_export del_dref(ea_t from, ea_t to);


//-------------------------------------------------------------------------
//      E N U M E R A T E   ( G E T )   C R O S S - R E F E R E N C E S
//-------------------------------------------------------------------------

struct xrefblk_t;
/// \name Helper functions
/// Should not be called directly!
//@{
idaman bool ida_export xrefblk_t_first_from(xrefblk_t *,ea_t from,int flags);
idaman bool ida_export xrefblk_t_next_from(xrefblk_t *);
idaman bool ida_export xrefblk_t_first_to(xrefblk_t *,ea_t to,int flags);
idaman bool ida_export xrefblk_t_next_to(xrefblk_t *);
//@}

/// Structure to enumerate all xrefs.
/// This structure provides a way to access cross-references from a given address.
/// For example:
///
/// \code
///      xrefblk_t xb;
///      for ( bool ok=xb.first_from(ea, XREF_ALL); ok; ok=xb.next_from() )
///      {
///         // xb.to - contains the referenced address
///      }
/// \endcode
/// or:
/// \code
///      xrefblk_t xb;
///      for ( bool ok=xb.first_to(ea, XREF_ALL); ok; ok=xb.next_to() )
///      {
///         // xb.from - contains the referencing address
///      }
/// \endcode
///
/// First, all code references will be returned, then all data references.
/// If you need only code references, stop calling next() as soon as you get a dref.
///
/// If you need only data references, pass #XREF_DATA flag to first().
/// You may not modify the contents of a xrefblk_t structure! It is read only.
struct xrefblk_t
{
  ea_t from;            ///< the referencing address - filled by first_to(),next_to()
  ea_t to;              ///< the referenced address - filled by first_from(), next_from()
  uchar iscode;         ///< 1-is code reference; 0-is data reference
  uchar type;           ///< type of the last returned reference (::cref_t & ::dref_t)
  uchar user;           ///< 1-is user defined xref, 0-defined by ida

/// \defgroup XREF_ Xref enumeration flags
/// Passed as 'flags' parameter to functions in ::xrefblk_t
//@{
#define XREF_ALL        0x00            ///< return all references
#define XREF_FAR        0x01            ///< don't return ordinary flow xrefs
#define XREF_DATA       0x02            ///< return data references only
//@}

  /// \name Get first/next
  /// The following functions first return code references, then data references.
  /// If you need only code references, you need to check 'iscode' after each call.
  /// If you need only data references, use #XREF_DATA bit.
  /// \param flags  \ref XREF_
  /// \retval 1  ok
  /// \retval 0  no more xrefs
  //@{

  /// Get first xref from the given address (store in #to)
  bool first_from(ea_t _from, int flags)
    { return xrefblk_t_first_from(this, _from, flags); }

  /// Get next xref from address provided to first_from()
  bool next_from(void)
    { return xrefblk_t_next_from(this); }

  /// Get xref to given address (store in #from)
  bool first_to(ea_t _to, int flags)
    { return xrefblk_t_first_to(this, _to, flags); }

  /// Get next xref to address provided to first_to()
  bool next_to(void)
    { return xrefblk_t_next_to(this); }

  /// Get xref from '_from' that comes after '_to'
  bool next_from(ea_t _from, ea_t _to, int flags)
  {
    if ( first_from(_from, flags) )
    {
      to = _to;
      return next_from();
    }
    return false;
  }

  /// Get xref to '_to' that comes after '_from'
  bool next_to(ea_t _from, ea_t _to, int flags)
  {
    if ( first_to(_to, flags) )
    {
      from = _from;
      return next_to();
    }
    return false;
  }

  //@}
};

//-------------------------------------------------------------------------

/// This variable will contains type of the last xref returned
/// by the following functions. It is not exported, so if you need to know
/// the cross reference type, please use the ::xrefblk_t structure to enumerate
/// the cross references.

extern char lastXR;


/// Get first data referenced from the specified address.
/// \param from  linear address of referencing instruction or data
/// \return linear address of first (lowest) data referenced from the specified address.
///         The ::lastXR variable contains type of the reference.
///         Return #BADADDR if the specified instruction/data doesn't reference
///         to anything.

idaman ea_t ida_export get_first_dref_from(ea_t from);


/// Get next data referenced from the specified address.
/// \param from     linear address of referencing instruction or data
/// \param current  linear address of current referenced data.
///                 This value is returned by get_first_dref_from() or
///                 previous call to get_next_dref_from() functions.
/// \return linear address of next data or #BADADDR.
///         The ::lastXR variable contains type of the reference

idaman ea_t ida_export get_next_dref_from(ea_t from,ea_t current);


/// Get address of instruction/data referencing to the specified data.
/// \param to linear address of referencing instruction or data
/// \return #BADADDR if nobody refers to the specified data.
///         The ::lastXR variable contains type of the reference.

idaman ea_t ida_export get_first_dref_to(ea_t to);


/// Get address of instruction/data referencing to the specified data
/// \param to       linear address of referencing instruction or data
/// \param current  current linear address.
///                 This value is returned by get_first_dref_to() or
///                 previous call to get_next_dref_to() functions.
/// \return #BADADDR if nobody refers to the specified data.
///         The #lastXR variable contains type of the reference.

idaman ea_t ida_export get_next_dref_to(ea_t to,ea_t current);


/// Get first instruction referenced from the specified instruction.
/// If the specified instruction passes execution to the next instruction
/// then the next instruction is returned. Otherwise the lowest referenced
/// address is returned (remember that xrefs are kept sorted!).
/// \param from  linear address of referencing instruction
/// \return first referenced address.
///         The #lastXR variable contains type of the reference.
///         If the specified instruction doesn't reference to other instructions
///         then returns #BADADDR.

idaman ea_t ida_export get_first_cref_from(ea_t from);


/// Get next instruction referenced from the specified instruction.
/// \param from     linear address of referencing instruction
/// \param current  linear address of current referenced instruction
///                 This value is returned by get_first_cref_from() or
///                 previous call to get_next_cref_from() functions.
/// \return next referenced address or #BADADDR.
///         The #lastXR variable contains type of the reference.

idaman ea_t ida_export get_next_cref_from(ea_t from,ea_t current);


/// Get first instruction referencing to the specified instruction.
/// If the specified instruction may be executed immediately after its
/// previous instruction then the previous instruction is returned.
/// Otherwise the lowest referencing address is returned.
/// (remember that xrefs are kept sorted!).
/// \param to  linear address of referenced instruction
/// \return linear address of the first referencing instruction or #BADADDR.
///         The #lastXR variable contains type of the reference.

idaman ea_t ida_export get_first_cref_to(ea_t to);


/// Get next instruction referencing to the specified instruction.
/// \param to       linear address of referenced instruction
/// \param current  linear address of current referenced instruction
///                 This value is returned by get_first_cref_to() or
///                 previous call to get_next_cref_to() functions.
/// \return linear address of the next referencing instruction or #BADADDR.
///         The #lastXR variable contains type of the reference.

idaman ea_t ida_export get_next_cref_to(ea_t to,ea_t current);


/// \name Far code references
/// The following functions are similar to get_{first|next}_cref_{from|to}
/// functions. The only difference is that they don't take into account
/// ordinary flow of execution. Only jump and call xrefs are returned.
/// (fcref means "far code reference")
//@{
idaman ea_t ida_export get_first_fcref_from(ea_t from);
idaman ea_t ida_export get_next_fcref_from (ea_t from,ea_t current);
idaman ea_t ida_export get_first_fcref_to  (ea_t to);
idaman ea_t ida_export get_next_fcref_to   (ea_t to,ea_t current);
//@}


/// Has a location external to the function references?

idaman bool ida_export has_external_refs(func_t *pfn, ea_t ea);


//-------------------------------------------------------------------------
//      S W I T C H  T A B L E  F U N C T I O N S
//-------------------------------------------------------------------------
#ifdef NALT_HPP

/// Create switch table from the switch information.
/// Usually there is no need to call this function directly because the kernel
/// will call it for the result of \ph{is_switch()}.
/// \param insn_ea  address of the 'indirect jump' instruction
/// \param si       switch information

idaman void ida_export create_switch_table(
        ea_t insn_ea,
        const switch_info_ex_t *si);


/// Create code xrefs for the switch table.
/// This function creates xrefs from the indirect jump.
/// Usually there is no need to call this function directly because the kernel
/// will call it for switch tables
/// \param insn_ea  address of the 'indirect jump' instruction
/// \param si       switch information

idaman void ida_export create_switch_xrefs(
        ea_t insn_ea,
        const switch_info_ex_t *si);


/// Vector of case values - see calc_switch_cases()
typedef qvector<svalvec_t> casevec_t;


/// Get detailed information about the switch table cases.
/// \param insn_ea  address of the 'indirect jump' instruction
/// \param si       switch information
/// \param casevec  vector of case values...
/// \param targets  ...and corresponding target addresses
/// \return success

idaman bool ida_export calc_switch_cases(
        ea_t insn_ea,
        const switch_info_ex_t *si,
        casevec_t *casevec,
        eavec_t *targets);

#endif

//-------------------------------------------------------------------------
//      F U N C T I O N S   F O R   T H E   K E R N E L
//-------------------------------------------------------------------------

/// \cond
idaman int ida_export create_xrefs_from(ea_t ea);  // returns 0: no item at ea
idaman void ida_export delete_all_xrefs_from(ea_t ea, bool expand);
/// \endcond

//------------------------------------------------------------------------

#pragma pack(pop)
#endif // _XREF_HPP
