/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2015 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 */


#ifndef _INTS_HPP
#define _INTS_HPP
#pragma pack(push, 1)           // IDA uses 1 byte alignments!

/*! \file ints.hpp

  \brief Functions that deal with the predefined comments

*/

class insn_t;
class WorkReg;

//--------------------------------------------------------------------
//      P R E D E F I N E D   C O M M E N T S
//--------------------------------------------------------------------

/// Get predefined comment.
/// \param cmd      current instruction information
/// \param buf      buffer for the comment
/// \param bufsize  size of the output buffer
/// \return size of comment or -1

idaman ssize_t ida_export get_predef_insn_cmt(
        const insn_t &cmd,
        char *buf,
        size_t bufsize);


/// Get predefined comment.
/// \param info     text string with description of operand and register values.
///                 This string consists of equations:
///                   - reg=value ...
///                 where reg may be any word register name,
///                 or Op1,Op2 - for first or second operands
/// \param wrktyp   icode of instruction to get comment about
/// \param buf      buffer for the comment
/// \param bufsize  size of the output buffer
/// \return size of comment or -1

idaman ssize_t ida_export get_predef_cmt(
        const char *info,
        int wrktyp,
        char *buf,
        size_t bufsize);


/// Get predefined VxD function name.
/// \param vxdnum   number of VxD
/// \param funcnum  number of function in the VxD
/// \param buf      buffer for the comment
/// \param bufsize  size of the output buffer
/// \return comment or NULL

#ifdef _IDP_HPP
inline char *idaapi get_vxd_func_name(
        int vxdnum,
        int funcnum,
        char *buf,
        size_t bufsize)
{
  buf[0] = '\0';
  ph.notify(ph.get_vxd_name, vxdnum, funcnum, buf, bufsize);
  return buf[0] ? buf : NULL;
}
#endif


//--------------------------------------------------------------------
// Private definitions
//-------------------------------------------------------------------

#pragma pack(pop)
#endif // _INTS_HPP
