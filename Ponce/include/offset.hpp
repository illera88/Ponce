/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2015 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 */

#ifndef _OFFSET_HPP
#define _OFFSET_HPP

#include <nalt.hpp>

#pragma pack(push, 1)           // IDA uses 1 byte alignments!

/*! \file offset.hpp

  \brief Functions that deal with offsets.

  "Being an offset" is a characteristic of an operand.
  This means that operand or its part represent offset from
  some address in the program. This linear address is called
  "offset base". Some operands may have 2 offsets simultaneously.
  Generally, IDA doesn't handle this except for Motorola outer offsets.
  Thus there may be two offset values in an operand: simple offset and
  outer offset.

  Outer offsets are handled by specifying special operand number:
  it should be ORed with #OPND_OUTER value.

  See bytes.hpp for further explanation of operand numbers.
*/


/// Convert operand to offset - OBSOLETE FUNCTION, use op_offset() instead.
/// To delete an offset, use noType() function.
/// \param ea  linear address.
///            if 'ea' has unexplored bytes, try to convert them to
///              - no segment: fail
///              - 16bit segment: to 16bit word data
///              - 32bit segment: to dword
/// \param n  number of operand (may be ORed with #OPND_OUTER)
///             - 0: first
///             - 1: second
///             - #OPND_MASK: both operands
/// \param base  base of offset (linear address)
/// \return success

idaman bool ida_export set_offset(ea_t ea, int n, ea_t base);


/// Convert operand to a reference.
/// To delete an offset, use noType() function.
/// \param ea  linear address.
///            if 'ea' has unexplored bytes, try to convert them to
///              - no segment: fail
///              - 16bit segment: to 16bit word data
///              - 32bit segment: to dword
/// \param n   number of operand (may be ORed with #OPND_OUTER)
///              - 0: first
///              - 1: second
///              - 2: third
///              - #OPND_MASK: all operands
/// \param ri  reference information
/// \return success

idaman int ida_export op_offset_ex(ea_t ea, int n, const refinfo_t *ri);


/// See op_offset_ex()

idaman int ida_export op_offset(ea_t ea, int n, reftype_t type, ea_t target=BADADDR,
                        ea_t base=0, adiff_t tdelta=0);


/// Get offset base value - OBSOLETE FUNCTION, use get_refinfo() instead
/// \param ea  linear address
/// \param n   number of operand (may be ORed with #OPND_OUTER)
///              - 0: first
///              - 1: second
///              - #OPND_MASK: try to get base of the first operand,
///                         get the second if the first doesn't exist
/// \return offset base or #BADADDR

idaman ea_t ida_export get_offbase(ea_t ea,int n);


/// Get offset expression (in the form "offset name+displ").
/// This function uses offset translation function (\ph{translate}) if your IDP
/// module has such a function. Translation function is used to map linear
/// addresses in the program (only for offsets).
///
/// Example: suppose we have instruction at linear address 0x00011000:
///              \v{mov     ax, [bx+7422h]}
/// and at ds:7422h:
///     \v{array   dw      ...}
/// We want to represent the second operand with an offset expression, so
/// then we call:
/// \v{
/// get_offset_expresion(0x001100, 1, 0x001102, 0x7422, buf);
///                      |         |  |         |       |
///                      |         |  |         |       +output buffer
///                      |         |  |         +value of offset expression
///                      |         |  +address offset value in the instruction
///                      |         +the second operand
///                      +address of instruction
/// }
/// and the function will return a colored string:
///     \v{offset array}
/// \param ea          start of instruction or data with the offset expression
/// \param n           number of operand (may be ORed with #OPND_OUTER)
///                      - 0: first operand
///                      - 1: second operand
/// \param from        linear address of instruction operand or data referring to
///                    the name. This address will be used to get fixup information,
///                    so it should point to exact position of operand in the
///                    instruction.
/// \param offset      value of operand or its part. The function will return
///                    text representation of this value as offset expression.
/// \param buf         output buffer to hold offset expression
/// \param bufsize     size of the output buffer
/// \param getn_flags  combination of:
///                      - #GETN_APPZERO: meaningful only if the name refers to
///                                       a structure. appends the struct field name
///                                       if the field offset is zero
///                      - #GETN_NODUMMY: do not generate dummy names for the expression
///                                       but pretend they already exist
///                                       (useful to verify that the offset expression
///                                       can be represented)
/// \retval 0  can't convert to offset expression
/// \retval 1  ok, a simple offset expression
/// \retval 2  ok, a complex offset expression


idaman int ida_export get_offset_expression(
                          ea_t ea,
                          int n,
                          ea_t from,
                          adiff_t offset,
                          char *buf,
                          size_t bufsize,
                          int getn_flags=0);


/// See get_offset_expression()

idaman int ida_export get_offset_expr(
                          ea_t ea,
                          int n,
                          refinfo_t &ri,
                          ea_t from,
                          adiff_t offset,
                          char *buf,
                          size_t bufsize,
                          int getn_flags=0);


/// Does the specified address contain a valid OFF32 value?.
/// For symbols in special segments the displacement is not taken into account.
/// If yes, then the target address of OFF32 will be returned.
/// If not, then #BADADDR is returned.

idaman ea_t ida_export can_be_off32(ea_t ea);


/// Try to calculate the offset base.
/// 2 bases are checked: current ds and cs.
/// If fails, return #BADADDR

idaman ea_t ida_export calc_probable_base_by_value(ea_t ea, uval_t off);


/// Get default reference type depending on the segment.
/// \return one of ::REF_OFF8,::REF_OFF16,::REF_OFF32

idaman reftype_t ida_export get_default_reftype(ea_t ea);


/// Calculate the target address of an offset expression.
/// \note this function may change 'ri' structure.
///
/// If ri.base is #BADADDR, it calculates the offset base address
/// from the referencing instruction/data address.
/// \param ri     reference info block from the database
/// \param opval  operand value (usually op_t::value or op_t::addr)
/// \return the target address of the reference

idaman ea_t ida_export calc_reference_target(ea_t from, refinfo_t &ri, adiff_t opval);


/// Calculate the value of the reference base.
/// The reference basevalue is used like this:  "offset target - reference_basevalue"
/// Usually the basevalue is equal to 0.
/// If it is not equal to 0, then ri.base contains the address of reference_basevalue
/// (which is not equal to reference_basevalue for 16-bit programs!).
/// \param  from    the referencing instruction/data address
/// \param  ri      reference info block from the database
/// \param  opval   operand value (usually op_t::value or op_t::addr)
/// \param  target  the reference target. If #BADADDR, it will be calculated automatically
/// \return the reference basevalue

idaman ea_t ida_export calc_reference_basevalue(ea_t from, refinfo_t &ri, adiff_t opval, ea_t target);


/// Retrieves refinfo_t structure and calculates the target

inline ea_t calc_target(ea_t from, ea_t ea, int n, adiff_t opval)
{
  refinfo_t ri;
  if ( get_refinfo(ea, n, &ri) )
    return calc_reference_target(from, ri, opval);
  return BADADDR;
}



#pragma pack(pop)
#endif  // _OFFSET_HPP
