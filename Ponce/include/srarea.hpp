/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2015 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 */

#ifndef _SRAREA_HPP
#define _SRAREA_HPP

#include <area.hpp>

#pragma pack(push, 1)           // IDA uses 1 byte alignments!

/*! \file srarea.hpp

  \brief Functions that deal with the segment registers.

  If your processor doesn't use segment registers, then these functions
  are of no use for you. However, you should define
  two virtual segment registers - CS and DS (for code segment and
  data segment) and specify their internal numbers in the LPH structure
  (processor_t::regCodeSreg and processor_t::regDataSreg).
*/

//-------------------------------------------------------------------------
/// The values of the segment registers are kept as address ranges. The segment
/// register does not change its value within one address range.
/// The processor module finds segment register change points and splits
/// ::segreg_area_t areas so that a new segreg_area_t area is started at each segment
/// register change point. The kernel deletes segreg_area_t
/// if an instruction is converted back to unexplored bytes.
///
/// So, we have information about a segment register by keeping information
/// about the range of addresses where segment register does not change the value.
///
/// Note that each segment has information about the default values of
/// the segment registers. This information is used if the value of a segment
/// register could not be determined.
struct segreg_area_t : public area_t
{
  sel_t val;                ///< segment register value
  uchar tag;                ///< \ref SR_

/// \defgroup SR_ Segment register area tags
/// Used by segreg_area_t::tag
//@{
#define SR_inherit      1   ///< the value is inherited from the previous area
#define SR_user         2   ///< the value is specified by the user
#define SR_auto         3   ///< the value is determined by IDA
#define SR_autostart    4   ///< used as #SR_auto for segment starting address
//@}
};
DECLARE_TYPE_AS_MOVABLE(segreg_area_t);


/// Get value of a segment register.
/// This function uses segment register area and default segment register
/// values stored in the segment structure.
/// \param ea  linear address in the program
/// \param rg  number of the segment register
/// \return value of the segment register, #BADSEL if value is unknown.

idaman sel_t ida_export get_segreg(ea_t ea, int rg);


/// Create a new segment register area.
/// This function is used when the IDP emulator detects that a segment
/// register changes its value.
/// \param ea      linear address where the segment register will
///                have a new value. if ea==#BADADDR, nothing to do.
/// \param rg      the number of the segment register
/// \param v       the new value of the segment register. If the value is
///                unknown, you should specify #BADSEL.
/// \param tag     the register info tag. see \ref SR_
/// \param silent  if false, display a warning() in the case of failure
/// \return success

idaman bool ida_export split_srarea(
        ea_t ea,
        int rg,
        sel_t v,
        uchar tag,
        bool silent=false);


/// Set default value of a segment register for a segment.
/// \param sg     pointer to segment structure
///               if NULL, then set the register for all segments
/// \param rg     number of segment register
/// \param value  its default value. this value will be used by get_segreg()
///               if value of the register is unknown at the specified address.
/// \return success

idaman bool ida_export set_default_segreg_value(segment_t *sg, int rg, sel_t value);


/// Set the segment register value at the next instruction.
/// This function is designed to be called from \ph{setsgr}
/// in order to contain the effect of changing a segment
/// register value only until the next instruction.
///
/// It is useful, for example, in the ARM module: the modification
/// of the T register does not affect existing instructions later in the code.
/// \param ea1    address to start to search for an instruction
/// \param ea2    the maximal address
/// \param rg     the segment register number
/// \param value  the segment register value

idaman void ida_export set_sreg_at_next_code(ea_t ea1, ea_t ea2, int rg, sel_t value);


/// Get segment register area by linear address.
/// \param rg   the segment register number
/// \param ea   any linear address in the program
/// \param out  segment register area
/// \return success

idaman bool ida_export get_srarea2(segreg_area_t *out, ea_t ea, int rg);


/// Get segment register area previous to one with address.
/// \note more efficient then get_srarea2(reg, ea-1)
/// \param rg   the segment register number
/// \param ea   any linear address in the program
/// \param out  segment register area
/// \return success

idaman bool ida_export get_prev_srarea(segreg_area_t *out, ea_t ea, int rg);


/// Set default value of DS register for all segments

idaman void ida_export set_default_dataseg(sel_t ds_sel);


/// Get number of segment register areas.
/// \param rg  the segment register number

idaman size_t ida_export get_srareas_qty2(int rg);


/// Get segment register area by its number.
/// \param rg   the segment register number
/// \param n    number of area (0..qty()-1)
/// \param out  segment register area
/// \return success

idaman bool ida_export getn_srarea2(segreg_area_t *out, int rg, int n);


/// Get number of segment register area by address.
/// \param rg  the segment register number
/// \param ea  any address in the area
/// \return -1 if no area occupies the specified address.
///         otherwise returns number of
///         the specified area (0..get_srareas_qty2()-1)

idaman int ida_export get_srarea_num(int rg, ea_t ea);


/// Delete segment register area started at ea.
/// When a segment register area is deleted,
/// the previous area is extended to cover the empty space.
/// The segment register area at the beginning of a segment can not be deleted.
/// \param rg  the segment register number
/// \param ea  startEA of the deleted area
/// \return success

idaman bool ida_export del_srarea(int rg, ea_t ea);


/// Duplicate segment register areas.
/// \param dst_rg        number of destination segment register
/// \param src_rg        copy areas from
/// \param map_selector  map selectors to linear addresses using sel2ea()

idaman void ida_export copy_srareas(int dst_rg, int src_rg, bool map_selector=false);


//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
/// Segment register area control block. See area.hpp for explanations.
/// OBSOLETE
idaman areacb_t ida_export_data SRareas;
#define SUPPORT_OBSOLETE_SRAREA

#if !defined(NO_OBSOLETE_FUNCS) || defined(DEFINE_OBSOLETE_SEGREG)
/// OBSOLETE
class segreg_t : public area_t
{
  sel_t _sRegs[SREG_NUM];
  uchar _tags [SREG_NUM];
public:
        sel_t &reg(int n)       { return _sRegs[n-ph.regFirstSreg]; }
  const sel_t &reg(int n) const { return _sRegs[n-ph.regFirstSreg]; }
        uchar &tag(int n)       { return _tags [n-ph.regFirstSreg]; }
  const uchar &tag(int n) const { return _tags [n-ph.regFirstSreg]; }
  void undefregs(void)  { memset(_sRegs,0xFF,sizeof(_sRegs)); }
  void setregs(sel_t Regs[]) { memcpy(_sRegs,Regs,sizeof(_sRegs)); }
  void settags(uchar v) { memset(_tags,v,sizeof(_tags)); }
};
#endif

#ifndef NO_OBSOLETE_FUNCS
idaman DEPRECATED segreg_t *ida_export get_srarea(ea_t ea);
idaman DEPRECATED size_t ida_export get_srareas_qty(void);
idaman DEPRECATED segreg_t *ida_export getn_srarea(int n);

class lock_segreg
{
public:
  lock_segreg(const segreg_t *) {}
};
inline bool is_segreg_locked(const segreg_t *) { return false; }

class segment_t;
idaman DEPRECATED sel_t ida_export getSR(ea_t ea,int rg);
idaman DEPRECATED bool ida_export SetDefaultRegisterValue(segment_t *sg, int rg, sel_t value);
idaman DEPRECATED bool ida_export splitSRarea1(ea_t ea, int rg, sel_t v, uchar tag, bool silent=false);
#endif

#pragma pack(pop)
#endif // _SRAREA_HPP
