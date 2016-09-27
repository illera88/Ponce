/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2015 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 */

#ifndef FIXUP_HPP
#define FIXUP_HPP

#include <nalt.hpp>
#include <segment.hpp>

/*! \file fixup.hpp

  \brief Functions that deal with fixup information

  A loader should setup fixup information using set_fixup().
*/

#pragma pack(push, 1)           // IDA uses 1 byte alignments!

/// Fixup information structure
struct fixup_data_t
{
  uchar type;                   ///< \ref FIXUP_
/// \defgroup FIXUP_ Fixup flags
/// Used by fixup_data_t::type
//@{
#define FIXUP_MASK      0xF     ///< type mask
#define FIXUP_OFF8      0       ///< 8-bit offset
#define FIXUP_OFF16     1       ///< 16-bit offset
#define FIXUP_SEG16     2       ///< 16-bit base--logical segment base (selector)
#define FIXUP_PTR16     3       ///< 32-bit long pointer (16-bit base:16-bit
                                ///< offset)
#define FIXUP_OFF32     4       ///< 32-bit offset
#define FIXUP_PTR32     5       ///< 48-bit pointer (16-bit base:32-bit offset)
#define FIXUP_HI8       6       ///< high  8 bits of 16bit offset
#define FIXUP_HI16      7       ///< high 16 bits of 32bit offset
#define FIXUP_LOW8      8       ///< low   8 bits of 16bit offset
#define FIXUP_LOW16     9       ///< low  16 bits of 32bit offset
#define FIXUP_VHIGH     0xA     ///< high \ph{high_fixup_bits} of 32bit offset
#define FIXUP_VLOW      0xB     ///< low  \ph{high_fixup_bits} of 32bit offset
#define FIXUP_OFF64     0xC     ///< 64-bit offset
//#define FIXUP_          0xD
//#define FIXUP_          0xE
#define FIXUP_CUSTOM    0xF     ///< fixup is processed by processor module
//
#define FIXUP_REL       0x10    ///< fixup is relative to the linear address specified in the 3d parameter to set_fixup()
#define FIXUP_SELFREL   0x0     ///< self-relative?
                                ///<   - disallows the kernel to convert operands
                                ///<      in the first pass
                                ///<   - this fixup is used during output
                                ///<
                                ///< This type of fixup is not used anymore.
                                ///< Anyway you can use it for commenting purposes
                                ///< in the loader modules.
#define FIXUP_EXTDEF    0x20    ///< target is a location (otherwise - segment).
                                ///< Use this bit if the target is a symbol
                                ///< rather than an offset from the beginning of a segment.
#define FIXUP_UNUSED    0x40    ///< fixup is ignored by IDA
                                ///<   - disallows the kernel to convert operands
                                ///<   - this fixup is not used during output
#define FIXUP_CREATED   0x80    ///< fixup was not present in the input file
//@}
#ifdef __EA64__
  sel_t sel;                    ///< target selector
#else
  ushort sel;                   ///< target selector
#endif
  ea_t off;                     ///< target offset
  adiff_t displacement;         ///< target displacement

  /// Is fixup processed by processor module?
  bool is_custom(void) const { return get_type() == FIXUP_CUSTOM; }

  /// Fixup type is identified by the low 8 bits of #type (see #FIXUP_MASK)
  uchar get_type(void) const { return type & FIXUP_MASK; }
};


/// Main netnode to access fixup info

idaman netnode ida_export_data fixup_node;


/// Set fixup information. You should fill ::fixup_data_t and call this
/// function and the kernel will remember information in the database.
/// Use this function if #FIXUP_REL bit is clear.
/// For custom fixups use set_custom_fixup().
/// \param source the fixup source address, i.e. the address modified by the fixup

idaman void ida_export set_fixup(ea_t source, const fixup_data_t *fp);


/// Set fixup information. You should fill ::fixup_data_t and call this
/// function and the kernel will remember information in the database.
/// Use this function if #FIXUP_REL bit is set.
/// For custom fixups use set_custom_fixup_ex().

idaman void ida_export set_fixup_ex(ea_t source, const fixup_data_t *fd, ea_t offset_base);


/// Delete fixup information

idaman void ida_export del_fixup(ea_t source);


/// Get fixup information

idaman bool ida_export get_fixup(ea_t source, fixup_data_t *fd);


/// \name Enumerate addresses with fixup information:
//@{
idaman ea_t ida_export get_first_fixup_ea(void);
idaman ea_t ida_export get_next_fixup_ea(ea_t ea);
idaman ea_t ida_export get_prev_fixup_ea(ea_t ea);
//@}


/// Use fixup information for an address.
/// This function converts item_ea flags to offsets/segments.
/// For undefined bytes, you may set item_ea == fixup_ea. In this case this
/// function will create an item (byte, word, dword) there.
/// \param item_ea   start address of item to modify
/// \param fixup_ea  address of fixup record
/// \param n         number of operand. may be 0, 1, 2
/// \param is_macro  is the instruction at 'item_ea' a macro?
///                  if yes, then partial fixups (HIGH, LOW) won't be applied
/// \retval false  no fixup at fixup_ea or it has #FIXUP_UNUSED flag
/// \retval true   ok, the fixup information was applied

idaman bool ida_export apply_fixup(ea_t item_ea, ea_t fixup_ea, int n, bool is_macro);


/// Get base of fixup for set_offset() function

idaman ea_t ida_export get_fixup_base(ea_t source, const fixup_data_t *fd);

/// \name Custom fixups
/// File loaders should ask the processor module to register a custom fixup
/// handler. After registering a handler file loaders may call set_custom_fixup()
/// to create custom fixups.
/// The loaders can not register a custom fixup handler themselves because
/// they will eventually be unloaded but the handler must stay in the memory
/// until the database is closed. The processor module should unregister the
/// custom fixup handler before the database gets closed.
//@{

//--------------------------------------------------------------------------
/// Implements the core behavior of a custom fixup
struct custom_fixup_handler_t
{
  int32 cbsize;        ///< size of this structure
  const char *name;    ///< Format name, must be unique
  int props;           ///< properties (currently 0)
  int size;            ///< size in bytes

  /// Get fixup base ea, can be NULL
  ea_t (idaapi *get_base)(ea_t ea, const fixup_data_t *fdp);

  /// Get description, if NULL use #name
  int (idaapi *get_desc)(ea_t ea, const fixup_data_t *fdp, char *buf, size_t bufsize);

  /// Apply a fixup: take it into account while analyzing the file.
  /// Usually it consists of converting the operand into an offset expression.
  bool (idaapi *apply)(ea_t ea, const fixup_data_t *fdp, ea_t item_start, int opnum, bool is_macro);

  /// Move the fixup from one location to another (called when the program is rebased)
  void (idaapi *move)(ea_t ea, fixup_data_t *fdp, adiff_t delta);
};
/// Vector of custom fixup handlers
typedef qvector<const custom_fixup_handler_t *> custom_fixup_handlers_t;


/// Register a new custom fixup. This function must be called
/// by a processor module or plugin, but not by a file loader.
/// File loaders should use create_custom_fixup() convenience function
/// to ask the processor module to create of a new handler.

idaman int ida_export register_custom_fixup(const custom_fixup_handler_t *cfh);


/// Unregister a new custom fixup format. Should be called by the processor
/// module before the database gets closed.

idaman bool ida_export unregister_custom_fixup(int cfid);


/// Setup custom fixup information. This function is called by file loader.
/// Use this function if #FIXUP_REL bit is clear.

idaman void ida_export set_custom_fixup(ea_t ea, int cfid, const fixup_data_t *fd);


/// Setup custom fixup information. This function is called by file loader.
/// Use this function if #FIXUP_REL bit is set.

idaman void ida_export set_custom_fixup_ex(ea_t ea, int cfid, const fixup_data_t *fd, ea_t offbase);

//@}


//--------------------------------------------------------------------------
/// See get_fixup_extdef_ea(ea_t ea)

inline ea_t get_fixup_extdef_ea(ea_t source, const fixup_data_t *fd)
{
  return (fd != NULL && (fd->type & FIXUP_EXTDEF) != 0)
       ? get_fixup_base(source, fd) + fd->off
       : BADADDR;
}


//--------------------------------------------------------------------------
/// See get_fixup_segdef_sel(ea_t ea)

inline sel_t get_fixup_segdef_sel(const fixup_data_t *fd)
{
  return (fd != NULL && (fd->type & FIXUP_EXTDEF) == 0)
       ? fd->sel
       : BADSEL;
}


//--------------------------------------------------------------------------
/// Get fixup target location.
/// \return #BADADDR if target is a segment (#FIXUP_EXTDEF),
///                  or if no fixup info available.

inline ea_t get_fixup_extdef_ea(ea_t ea)
{
  fixup_data_t fd;
  if ( get_fixup(ea, &fd) )
    return get_fixup_extdef_ea(ea, &fd);
  return BADADDR;
}


//--------------------------------------------------------------------------
/// Get fixup target selector.
/// \return #BADSEL if target is a location (#FIXUP_EXTDEF),
///                 or if no fixup info available.

inline sel_t get_fixup_segdef_sel(ea_t ea)
{
  fixup_data_t fd;
  if ( get_fixup(ea, &fd) )
    return get_fixup_segdef_sel(&fd);
  return BADSEL;
}


/// Get FIXUP description comment.
/// fdp can't be NULL.

idaman char *ida_export get_fixup_desc(
        ea_t source,
        const fixup_data_t *fdp,
        char *buf,
        size_t bufsize);


/// Does the specified address range contain any fixup information?

idaman bool ida_export contains_fixups(ea_t ea, asize_t size);


/// Relocate the bytes with fixup information once more (generic function).
/// This function may be called from loader_t::move_segm() if it suits the goal.

idaman void ida_export gen_fix_fixups(ea_t from, ea_t to, asize_t size);



#pragma pack(pop)
#endif // FIXUP_HPP
