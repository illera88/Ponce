/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2015 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 */

#ifndef _UA_HPP
#define _UA_HPP

#include <kernwin.hpp>  // for btoa()
#include <lines.hpp>    // for colors
#include <xref.hpp>     // add_cref()

#pragma pack(push, 1)   // IDA uses 1 byte alignments!

/*! \file ua.hpp

  \brief Functions that deal with the disassembling of program instructions.

  There are 2 kinds of functions:
    - functions that are called from the kernel
      to disassemble an instruction. These functions
      call IDP module for it.
    - functions that are called from IDP module to
      disassemble an instruction. We will call them
      'helper functions'.

  Disassembly of an instruction is made in three steps:
    -# analysis:             ana.cpp
    -# emulation:            emu.cpp
    -# conversion to text:   out.cpp

  The kernel calls the IDP module to perform these steps.
  At first, the kernel always calls analysis. The analyzer
  must decode the instruction and fill ::cmd.
  It has no rights to change anything in the database.

  The second step, emulation, is called for each instruction.
  This step must make necessary changes to the database,
  plan analysis of subsequent instructions, track register
  values, memory contents, etc. However, the kernel may call the
  emulation step for any address in the program - there is no
  ordering of addresses. Usually, the emulation is called
  necessary for subsequent addresses but this is not guaranteed.
  The main goal of emulation step is to track down execution flow
  and to plan conversion of necessary bytes to instructions.

  The last step, conversion to text, is called each time an instruction
  is displayed on the screen. The kernel will always call the analysis step first
  (the analysis should be very fast) and then will call conversion to text.
  The emulation and conversion steps should use information stored
  in ::cmd. They should not access the bytes of the instruction
  and decode it again - this should only be done in the analysis step.
*/

/// \defgroup operands Operands
/// Work with instruction operands

//--------------------------------------------------------------------------
//      T Y P E   O F   O P E R A N D
//--------------------------------------------------------------------------
#ifndef SWIG
typedef uchar optype_t; ///< see \ref o_
/// \defgroup o_ Operand types
/// \ingroup operands
///
/// Various types of instruction operands.
/// The kernel already knows about some operand types and associates them
/// with fields in ::op_t.
///
/// IDA also allows you define processor specific operand types (o_idpspec...).
/// You are free to give any meaning to these types. I suggest you to create a
/// #define to use mnemonic names. However, don't forget that the kernel will
/// know nothing about operands of those types.
/// You may use any additional fields of ::op_t to store
/// processor specific operand information.
//@{
const optype_t
  o_void     =  0, ///< No Operand.
  o_reg      =  1, ///< General Register (al,ax,es,ds...).
  o_mem      =  2, ///< Direct Memory Reference  (DATA).
  o_phrase   =  3, ///< Memory Ref [Base Reg + Index Reg].
  o_displ    =  4, ///< Memory Reg [Base Reg + Index Reg + Displacement].
  o_imm      =  5, ///< Immediate Value.
  o_far      =  6, ///< Immediate Far Address  (CODE).
  o_near     =  7, ///< Immediate Near Address (CODE).
  o_idpspec0 =  8, ///< processor specific type.
  o_idpspec1 =  9, ///< processor specific type.
  o_idpspec2 = 10, ///< processor specific type.
  o_idpspec3 = 11, ///< processor specific type.
  o_idpspec4 = 12, ///< processor specific type.
  o_idpspec5 = 13; ///< processor specific type.
                   ///< (there can be more processor specific types)
//@}

/// \var o_reg
/// The register number should be stored in op_t::reg.
/// All processor registers, including special registers, can be
/// represented by this operand type.
///
/// \var o_mem
/// A direct memory data reference whose target address is known at compilation time.
/// The target virtual address is stored in op_t::addr and the full address
/// is calculated as toEA( \cmd{cs}, op_t::addr ). For the processors with
/// complex memory organization the final address can be calculated
/// using other segment registers. For flat memories, op_t::addr is the final
/// address and \cmd{cs} is usually equal to zero. In any case, the address
/// within the segment should be stored in op_t::addr.
///
/// \var o_phrase
/// A memory reference using register contents. Indexed, register based,
/// and other addressing modes can be represented with the operand type.
/// This addressing mode can not contain immediate values (use ::o_displ instead).
/// The phrase number should be stored in op_t::phrase. To denote the pre-increment
/// and similar features please use additional operand fields like op_t::specflag...
/// Usually op_t::phrase contains the register number and additional information
/// is stored in op_t::specflags... Please note that this operand type can not
/// contain immediate values (except the scaling coefficients).
///
/// \var o_displ
/// A memory reference using register contents with displacement.
/// The displacement should be stored in the op_t::addr field. The rest of information
/// is stored the same way as in ::o_phrase.
///
/// \var o_imm
/// Any operand consisting of only a number is represented by this operand type.
/// The value should be stored in op_t::value. You may sign extend short (1-2 byte) values.
/// In any case don't forget to specify op_t::dtyp (should be set for all operand types).
///
/// \var o_far
/// If the current processor has a special addressing mode for inter-segment
/// references, then this operand type should be used instead of ::o_near.
/// If you want, you may use #PR_CHK_XREF in \ph{flag} to disable inter-segment
/// calls if ::o_near operand type is used. Currently only IBM PC uses this flag.
///
/// \var o_near
/// A direct memory code reference whose target address is known at the compilation time.
/// The target virtual address is stored in op_t::addr and the final address
/// is always toEA(\cmd{cs}, op_t::addr). Usually this operand type is used for
/// the branches and calls whose target address is known. If the current
/// processor has 2 different types of references for inter-segment and intra-segment
/// references, then this should be used only for intra-segment references.
///
/// If the above operand types do not cover all possible addressing modes,
/// then use o_idpspec... operand types.


//--------------------------------------------------------------------------
//      O P E R A N D   O F   I N S T R U C T I O N
//--------------------------------------------------------------------------
/// \defgroup operands_t Operand structure
/// \ingroup operands
/// Definition of ::op_t and related items.

/// Operand of an instruction. \ingroup operands_t
/// This structure is filled by the analyzer.
/// Upon entrance to the analyzer, some fields of this structure are initialized:
///   - #type:    ::o_void
///   - #offb:    0
///   - #offo:    0
///   - #flags:   #OF_SHOW
class op_t
{
public:

  /// Number of operand (0,1,2). Initialized once at the start of work.
  /// You have no right to change its value.
  uchar         n;

  /// Type of operand (see \ref o_)
  optype_t      type;

  /// Offset of operand value from the instruction start (0 means unknown).
  /// Of course this field is meaningful only for certain types of operands.
  /// Leave it equal to zero if the operand has no offset.
  /// This offset should point to the 'interesting' part of operand.
  /// For example, it may point to the address of a function in
  ///      \v{call func}
  /// or it may point to bytes holding '5' in
  ///      \v{mov  ax, [bx+5]}
  /// Usually bytes pointed to this offset are relocated (have fixup information).
  char          offb;

  /// Same as #offb (some operands have 2 numeric values used to form an operand).
  /// This field is used for the second part of operand if it exists.
  /// Currently this field is used only for outer offsets of Motorola processors.
  /// Leave it equal to zero if the operand has no offset.
  char          offo;

  uchar         flags;          ///< \ref OF_
/// \defgroup OF_ Operand flags
/// \ingroup operands_t
/// Used by op_t::flags
//@{
#define OF_NO_BASE_DISP 0x80    ///< base displacement doesn't exist.
                                ///< meaningful only for ::o_displ type.
                                ///< if set, base displacement (op_t::addr)
                                ///< doesn't exist.
#define OF_OUTER_DISP   0x40    ///< outer displacement exists.
                                ///< meaningful only for ::o_displ type.
                                ///< if set, outer displacement (op_t::value) exists.
#define PACK_FORM_DEF   0x20    ///< packed factor defined.
                                ///< (!::o_reg + #dt_packreal)
#define OF_NUMBER       0x10    ///< the operand can be converted to a number only
#define OF_SHOW         0x08    ///< should the operand be displayed?
//@}

  /// Set operand to be shown
  void set_shown()     { flags |=  OF_SHOW; }
  /// Set operand to hidden
  void clr_shown()     { flags &= ~OF_SHOW; }
  /// Is operand set to be shown?
  bool shown() const   { return (flags & OF_SHOW) != 0; }

  /// Type of operand value (see \ref dt_). Usually first 9 types are used.
  /// This is the type of the operand itself, not the size of the addressing mode.
  /// for example, byte ptr [epb+32_bit_offset]  will have #dt_byte type.
  char dtyp;
/// \defgroup dt_ Operand value types
/// \ingroup operands_t
/// Used by op_t::dtyp
//@{
// from here..
#define dt_byte         0       ///< 8 bit
#define dt_word         1       ///< 16 bit
#define dt_dword        2       ///< 32 bit
#define dt_float        3       ///< 4 byte
#define dt_double       4       ///< 8 byte
#define dt_tbyte        5       ///< variable size (\ph{tbyte_size})
#define dt_packreal     6       ///< packed real format for mc68040
// ...to here the order should not be changed, see mc68000
#define dt_qword        7       ///< 64 bit
#define dt_byte16       8       ///< 128 bit
#define dt_code         9       ///< ptr to code (not used?)
#define dt_void         10      ///< none
#define dt_fword        11      ///< 48 bit
#define dt_bitfild      12      ///< bit field (mc680x0)
#define dt_string       13      ///< pointer to asciiz string
#define dt_unicode      14      ///< pointer to unicode string
#define dt_3byte        15      ///< 3-byte data
#define dt_ldbl         16      ///< long double (which may be different from tbyte)
#define dt_byte32       17      ///< 256 bit
#define dt_byte64       18      ///< 512 bit
//@}

  // REG
  union
  {
    uint16 reg;                 ///< number of register (::o_reg)
    uint16 phrase;              ///< number of register phrase (::o_phrase,::o_displ).
                                ///< you yourself define numbers of phrases
                                ///< as you like
  };

  /// Is register operand?
  bool is_reg(int r) const { return type == o_reg && reg == r; }

  //  Next 12 bytes are used by mc68k for some float types

  // VALUE
  union
  {
    uval_t value;               ///< operand value (::o_imm) or
                                ///< outer displacement (::o_displ+#OF_OUTER_DISP).
                                ///< integer values should be in IDA's (little-endian) order.
                                ///< when using ieee_realcvt(), floating point values should be in the processor's native byte order.
                                ///< #dt_double values take up 8 bytes (value and addr fields for 32-bit modules).
                                ///< NB: in case a #dt_dword/#dt_qword immediate is forced to float by user,
                                ///< the kernel converts it to processor's native order before calling FP conversion routines.

    /// This structure is defined for
    /// your convenience only
    struct
    {
        uint16 low;
        uint16 high;
    } value_shorts;
  };

  /// Is immediate operand?
  bool is_imm(uval_t v) const { return type == o_imm && value == v; }

  // VIRTUAL ADDRESS (offset within the segment)
  union
  {
    ea_t addr;                  ///< virtual address pointed or used by the operand.
                                ///< (::o_mem,::o_displ,::o_far,::o_near)

    /// This structure is defined for
    /// your convenience only
    struct
    {
        uint16 low;
        uint16 high;
    } addr_shorts;
  };

  // IDP SPECIFIC INFORMATION
  union
  {
    ea_t specval;               ///< This field may be used as you want.
    /// This structure is defined for
    /// your convenience only
    struct
    {
        uint16 low;             ///< IBM PC: segment register number (::o_mem,::o_far,::o_near)
        uint16 high;            ///< IBM PC: segment selector value  (::o_mem,::o_far,::o_near)
    } specval_shorts;
  };

  /// \name Special flags
  /// The following fields are used only in idp modules.
  /// You may use them as you want to store additional information about
  /// the operand.
  //@{
  char          specflag1;
  char          specflag2;
  char          specflag3;
  char          specflag4;
  //@}
};


//--------------------------------------------------------------------------
//      I N S T R U C T I O N
//--------------------------------------------------------------------------
/// \defgroup instruction Instruction
/// Definition of ::insn_t and related items.

/// Structure to hold information about an instruction. \ingroup instruction
/// This structure is filled by the analysis step of IDP and used by
/// the emulation and conversion to text steps. The kernel uses this structure too.
/// All structure fields except #cs, #ip, #ea, and op_t::n,op_t::flags of #Operands
/// are initialized to zero by the kernel. The rest should be filled by ana().
class insn_t
{
public:
  /// Current segment base paragraph. Initialized by the kernel.
  ea_t cs;

  /// Virtual address of the instruction (address within the segment).
  /// Initialized by the kernel.
  ea_t ip;

  /// Linear address of the instruction.
  /// Initialized by the kernel.
  ea_t ea;

  /// Internal code of instruction (only for canonical insns - not user defined!).
  /// IDP should define its own instruction codes. These codes are usually
  /// defined in ins.hpp. The array of instruction names and features (ins.cpp)
  /// is accessed using this code.
  uint16 itype;

  inline bool is_canon_insn(void) const;         ///< see \ph{is_canon_insn()}
  inline uint32 get_canon_feature(void) const;   ///< see instruc_t::feature
  inline const char *get_canon_mnem(void) const; ///< see instruc_t::name

  /// Size of instruction in bytes.
  /// The analyzer should put here the actual size of the instruction.
  uint16 size;

  union
  {
    uint16 auxpref;             ///< processor dependent field
    /// This structure is defined for
    /// your convenience only
    struct
    {
      uchar low;
      uchar high;
    } auxpref_chars;
  };
  char segpref;                 ///< processor dependent field
  char insnpref;                ///< processor dependent field

#define UA_MAXOP        6       ///< max number of operands allowed for an instruction
  op_t Operands[UA_MAXOP];      ///< array of operands

  /// \defgroup Op_ Operand shortcuts
  /// \ingroup instruction
  /// Used for accessing members of insn_t::Operands
  //@{
  #define Op1 Operands[0]       ///< first operand
  #define Op2 Operands[1]       ///< second operand
  #define Op3 Operands[2]       ///< third operand
  #define Op4 Operands[3]       ///< fourth operand
  #define Op5 Operands[4]       ///< fifth operand
  #define Op6 Operands[5]       ///< sixth operand
  //@}

  char flags;                   ///< \ref INSN_
/// \defgroup INSN_ Instruction flags
/// \ingroup instruction
/// Used by insn_t::flags
//@{
#define INSN_MACRO  0x01        ///< macro instruction
#define INSN_MODMAC 0x02        ///< may modify the database to make room for the macro insn
//@}

  /// Is a macro instruction?
  bool is_macro(void) const { return (flags & INSN_MACRO) != 0; }

};


//--------------------------------------------------------------------------
//      V A L U E   O F   O P E R A N D
//--------------------------------------------------------------------------

/// This structure is used to pass values of bytes to helper functions.
union value_u
{
  uint8  v_char;
  uint16 v_short;
  uint32 v_long;
  uint64 v_int64;
  uval_t v_uval;
  struct dq_t { uint32 low; uint32 high; } _dq;
  struct dt_t { uint32 low; uint32 high; uint16 upper; } dt;
  struct d128_t { uint64 low; uint64 high; } d128;
  uint8 byte16[16];
  uint32 dword3[3];
};

#endif // SWIG

/// Get immediate values used in the operand if it fits into ::uval_t.
/// \param ea  linear address
/// \param n   number of operand (0..#UA_MAXOP-1), -1 means all operands
/// \param v   array of immediate values (at least 2*#UA_MAXOP elements)
/// \return number of immediate values (0..2*#UA_MAXOP)

idaman size_t ida_export get_operand_immvals(ea_t ea, int n, uval_t *v);


//--------------------------------------------------------------------------
//      T H E   M A I N   S T R U C T U R E
//--------------------------------------------------------------------------

/// Structure holding information about the current instruction.
/// Analyzer should fill this structure.

idaman insn_t ida_export_data cmd;

/// Number of instructions to look back.
/// This variable is not used by the kernel.
/// Its value may be specified in ida.cfg:
///      LOOKBACK = <number>.
/// IDP may use it as you like it.
/// (TMS module uses it)

idaman int ida_export_data lookback;


//--------------------------------------------------------------------------
//      I D P   H E L P E R   F U N C T I O N S  -  C O M M O N
//--------------------------------------------------------------------------

/// Flags value for the byte at the start of the current instruction.
/// (see bytes.hpp for explanation of flags).
/// The kernel retrieves flags value and stores it in this variable for
/// your convenience. Anytime you change the database by calling functions
/// that change flags value you should refresh it using get_flags_novalue()
/// uFlag does not contain #MS_VAL and #FF_IVL fields of flags, so please
/// don't call hasValue() on it.

idaman flags_t ida_export_data uFlag;

/// \name Segment bases
/// The following functions return segment base linear addresses of
/// the data or code segment for the current instruction.
/// They use values of segment registers, operand types, etc.
//@{

/// Get data segment by operand.
/// 'opnum' and 'rgnum' are meaningful only if the processor
/// has segment registers.

idaman ea_t ida_export dataSeg_opreg(int opnum, int rgnum);

/// Get data segment by operand number

idaman ea_t ida_export dataSeg_op(int opnum);

/// Get data segment regardless of operand number

idaman ea_t ida_export dataSeg(void);

/// Get code segment.
/// This function takes into account the segment translations.
/// \param addr   the referenced address used by translations
/// \param opnum  operand number

idaman ea_t ida_export codeSeg(ea_t addr, int opnum);
//@}

//--------------------------------------------------------------------------
/// 3-byte (tribyte) data item order
enum tribyte_order_t
{
  tbo_123,      ///< regular most significant byte first (big endian) - default
  tbo_132,
  tbo_213,
  tbo_231,
  tbo_312,
  tbo_321,      ///< regular least significant byte first (little endian)
};

//--------------------------------------------------------------------------
//      I D P   H E L P E R   F U N C T I O N S  -  A N A L Y S I S
//--------------------------------------------------------------------------

/// \name Analysis helpers
/// The following functions return the next byte, 2 bytes, 4 bytes,
/// and 8 bytes of ::cmd. They use and modify the size field (\cmd{size}).
/// Normally they are used in the analyzer to get bytes of the instruction.
/// \warning These functions work only for normal (8bit) byte processors!
//@{
idaman uint8  ida_export ua_next_byte(void);
idaman uint16 ida_export ua_next_word(void);
idaman uint32 ida_export ua_next_long(void);
idaman uint64 ida_export ua_next_qword(void);
//@}

//--------------------------------------------------------------------------
//      I D P   H E L P E R   F U N C T I O N S  -  O U T P U T
//--------------------------------------------------------------------------

/// \defgroup output_helpers Output helpers
/// Functions to help display instruction information.
/// All output functions use ::cmd structure to get information.
//@{

/// Initialize output buffer.
/// Must be called before using any out/Out functions.

idaman void ida_export init_output_buffer(char *buf, size_t bufsize);


/// Terminate output buffer.
/// Must be called after using all out/Out functions to form the output string.
/// \return pointer to the end of the output string.

idaman char *ida_export term_output_buffer(void);


/// Append a formatted string to the output string.
/// \return the number of characters appended

idaman AS_PRINTF(1, 2) int ida_export out_snprintf(const char *format, ...);


/// Set the pointer to the output buffer.
/// \param ptr  if NULL, the pointer value is not changed (essentially this
///             function becomes get_output_ptr() in this case)
/// \return the old value of the pointer

idaman char *ida_export set_output_ptr(char *ptr);

/// Get pointer to the output buffer

inline char *idaapi get_output_ptr(void) { return set_output_ptr(NULL); }


/// Insert a string into the output string.
/// \param ptr     place to insert to. should come from get_output_ptr()
/// \param string  string to insert

idaman void ida_export out_insert(char *ptr, const char *string);


/// Output instruction mnemonics using information in ::cmd.
/// This function outputs a colored text.
/// It will output at least one space after the instruction
/// mnemonics even if the specified 'width' is not enough.
/// \param width    width of field with mnemonics.
///                 if < 0, then 'postfix' will be output before
///                 the mnemonics, i.e. as a prefix
/// \param postfix  optional postfix added to the instruction mnemonics
/// \retval 0  displayed as bytes
/// \retval 1  displayed as instruction

idaman int ida_export OutMnem(int width=8, const char *postfix=NULL);


/// Output instruction as a sequence of bytes,
/// followed by a comment character and instruction mnemonics.
/// This function is used to display undocumented instructions or
/// instructions that are improperly handled by the target assembler.
/// OutMnem() calls this function if the current instruction is present
/// in the array of bad instructions (\ash{badworks}).
/// This function outputs a colored text.

idaman void ida_export OutBadInstruction(void);


/// Use this function to output an operand of an instruction.
/// This function checks for the existence of a manually defined operand
/// and will output it if it exists.
/// Otherwise it will call \ph{u_outop()} to output operand.
/// This function outputs a colored text.
/// \param n  number of operand
/// \retval 1  operand is displayed
/// \retval 0  operand is hidden

idaman bool ida_export out_one_operand(int n);


/// Output immediate value.
/// Try to use this function to output all constants of instruction operands.
/// This function outputs a number from x.addr or x.value in the form
/// determined by ::uFlag. It outputs a colored text.
///   - -1 is output with #COLOR_ERROR
///   -  0 is output as a number or character or segment
/// \param x         value to output
/// \param outflags  \ref OOF_
/// \return flags of the output value

idaman flags_t ida_export OutValue(const op_t &x, int outflags=0);

/// \defgroup OOF_ Output value flags
/// Flags passed to OutValue() and get_immval().
/// (don't use #OOF_SIGNMASK and #OOF_WIDTHMASK, they are for the kernel)
//@{
#define OOF_SIGNMASK    0x0003      ///< sign symbol (+/-) output
#define   OOFS_IFSIGN   0x0000      ///<   output sign if needed
#define   OOFS_NOSIGN   0x0001      ///<   don't output sign, forbid the user to change the sign
#define   OOFS_NEEDSIGN 0x0002      ///<   always out sign         (+-)
#define OOF_SIGNED      0x0004      ///< output as signed if < 0
#define OOF_NUMBER      0x0008      ///< always as a number
#define OOF_WIDTHMASK   0x0070      ///< width of value in bits
#define   OOFW_IMM      0x0000      ///<   take from x.dtyp
#define   OOFW_8        0x0010      ///<   8 bit width
#define   OOFW_16       0x0020      ///<   16 bit width
#define   OOFW_24       0x0030      ///<   24 bit width
#define   OOFW_32       0x0040      ///<   32 bit width
#define   OOFW_64       0x0050      ///<   64 bit width
#define OOF_ADDR        0x0080      ///< output x.addr, otherwise x.value
#define OOF_OUTER       0x0100      ///< output outer operand
#define OOF_ZSTROFF     0x0200      ///< meaningful only if isStroff(uFlag)
                                    ///< append a struct field name if
                                    ///< the field offset is zero?
                                    ///< if #AFL_ZSTROFF is set, then this flag
                                    ///< is ignored.
#define OOF_NOBNOT      0x0400      ///< prohibit use of binary not
#define OOF_SPACES      0x0800      ///< do not suppress leading spaces
                                    ///< currently works only for floating point numbers
//@}

/// Extract immediate value from the operand according to the specified flags.
/// This is an internal function. Use get_operand_immvals() instead
/// \param x                 operand
/// \param outf              \ref OOF_
/// \param extend_sign       should the sign be extended?
/// \param[out] dtyp_ptr     pointer to the dtyp which will be filled by this function
/// \param[out] nbytes_ptr   pointer to the 'nbytes' which will be filled by this function -
///                          number of bytes required to store the value

uval_t get_immval(const op_t &x,
                  int outf=0,
                  bool extend_sign=false,
                  char *dtyp_ptr=NULL,
                  size_t *nbytes_ptr=NULL);


/// Output a character with #COLOR_SYMBOL color.

idaman void ida_export out_symbol(char c);


/// Output a string with the specified color.

idaman void ida_export out_line(const char *str, color_t color);


/// Output a string with #COLOR_KEYWORD color.

inline void out_keyword(const char *str)
{
  out_line(str, COLOR_KEYWORD);
}


/// Output a character with #COLOR_REG color.

inline void out_register(const char *str)
{
  out_line(str, COLOR_REG);
}


/// Output "turn color on" escape sequence

idaman void ida_export out_tagon(color_t tag);


/// Output "turn color off" escape sequence

idaman void ida_export out_tagoff(color_t tag);


/// Output "address" escape sequence

idaman void ida_export out_addr_tag(ea_t ea);


/// Output a colored line with register names in it.
/// The register names will be substituted by user-defined names (regvar_t)
/// Please note that out_tagoff tries to make substitutions too (when called with COLOR_REG)

idaman void ida_export out_colored_register_line(const char *str);


/// Output plain text without color codes.
/// see also out_line()

idaman void ida_export OutLine(const char *s);


/// Output one character.
/// The character is output without color codes.
/// see also out_symbol()

idaman void ida_export OutChar(char c);


/// Output a number with the specified base (binary, octal, decimal, hex)
/// The number is output without color codes.
/// see also out_long()

idaman void ida_export OutLong(uval_t Word, char radix);


/// Output operand value as a commented character constant.
/// This function is used to comment void operands with their representation
/// in the form of character constants.
/// This function outputs a colored text.

idaman void ida_export OutImmChar(const op_t &x);


/// Try to display value as a character constant.
/// This is low level function, it is called from OutValue().
/// This function outputs uncolored text.
/// \param ptr   pointer to value to convert
/// \param buf   output buffer
/// \param size  size of input value in bytes
/// \retval 1  ok, the buffer contains character constant.
///            its form depends on \ash{flags}.
/// \retval 0  failed, probably the constant is invalid for the target
///            assembler

idaman bool ida_export showAsChar(const void *ptr, char *buf, int size);


/// Output a floating point value.
/// Low level function. Use OutValue() if you can.
/// This function outputs uncolored text.
/// \param v        floating point value in processor native format
/// \param size     size of the value in bytes
/// \param buf      output buffer. may be NULL
/// \param bufsize  size of the output buffer
/// \return true    ok
/// \return false   can't represent as floating point number

idaman bool ida_export out_real(const void *v, int size, char *buf, size_t bufsize);


/// \fn void out_long(sval_t, char)
/// Output a number with appropriate color.
/// Low level function. Use OutValue() if you can.
/// if 'voidop' is set then
///   this function uses #COLOR_VOIDOP instead of #COLOR_NUMBER.
/// 'voidop' is initialized:
///   - in out_one_operand()
///   - in ..\ida\gl.cpp (before calling \ph{d_out()})
/// \param v      value to output
/// \param radix  base (2,8,10,16)
/// \param voidop ::voidop
///               - voidop==0: operand is ok
///               - voidop==1: operand is void and should be output with #COLOR_VOIDOP
///               - voidop==2: operand can't be output as requested and should be output with #COLOR_ERROR

idaman int ida_export_data voidop; ///< see out_long()
idaman void ida_export out_long(sval_t v, char radix);

/// Output a name expression.
/// \param x    instruction operand referencing the name expression
/// \param ea   address to convert to name expression
/// \param off  the value of name expression. this parameter is used only to
///             check that the name expression will have the wanted value.
///             You may pass #BADADDR for this parameter but I discourage it
///             because it prohibits checks.
/// \return true if the name expression has been produced

idaman bool ida_export out_name_expr(const op_t &x,
                                     ea_t ea,
                                     adiff_t off=BADADDR);

//@} output_helpers

//--------------------------------------------------------------------------
//      I D P   H E L P E R   F U N C T I O N S  -  E M U L A T O R
//--------------------------------------------------------------------------

/// Convert to data using information about operand value type (op_t::dtyp).
/// Emulator could use this function to convert unexplored bytes to data
/// when an instruction references them.
/// This function creates data only if the address was unexplored.
/// \param opoff  offset of the operand from the start of instruction
///               if the offset is unknown, then 0
/// \param ea     linear address to be converted to data
/// \param dtype  operand value type (from op_t::dtyp)
/// \retval 1  ok
/// \retval 0  failed to create data item

idaman bool ida_export ua_dodata2(int opoff, ea_t ea, int dtype);


/// Create or modify a stack variable in the function frame.
/// The emulator could use this function to create stack variables
/// in the function frame before converting the operand to a stack variable.
/// Please check with may_create_stkvars() before calling this function.
/// \param x      operand (used to determine the addressing type)
/// \param v      a displacement in the operand
/// \param flags  \ref STKVAR_2
/// \retval 1  ok, a stack variable exists now
/// \retval 0  no, couldn't create stack variable

idaman bool ida_export ua_stkvar2(const op_t &x, adiff_t v, int flags);

/// \defgroup STKVAR_2 Stack variable flags
/// Passed as 'flags' parameter to ua_stkvar2()
//@{
#define STKVAR_VALID_SIZE       0x0001 ///< x.dtyp contains correct variable type
                                       ///< (for insns like 'lea' this bit must be off)
                                       ///< in general, dr_O references do not allow
                                       ///< to determine the variable size
//@}

/// Add a code cross-reference from the current instruction (\cmd{ea}).
/// \param opoff  offset of the operand from the start of instruction.
///               if the offset is unknown, then 0.
/// \param to     target linear address
/// \param type   type of xref

idaman void ida_export ua_add_cref(int opoff, ea_t to, cref_t type);


/// Add a data cross-reference from the current instruction (\cmd{ea}).
/// See ua_add_off_drefs() - usually it can be used in most cases.
/// \param opoff  offset of the operand from the start of instruction
///               if the offset is unknown, then 0
/// \param to     target linear address
/// \param type   type of xref

idaman void ida_export ua_add_dref(int opoff, ea_t to, dref_t type);


/// Add xrefs for an operand of the current instruction (\cmd{ea}).
/// This function creates all cross references for 'offset' and
/// 'structure offset' operands.
/// Use ua_add_off_drefs2() in the presence of negative offsets.
/// \param x     reference to operand
/// \param type  type of xref
/// \param outf  OutValue() flags. These flags should match
///              the flags used to output the operand
/// \return if isOff(), the reference target address (the same as calc_reference_target).
///          else, #BADADDR, because for stroffs the target address is unknown

idaman ea_t ida_export ua_add_off_drefs(const op_t &x, dref_t type);
idaman ea_t ida_export ua_add_off_drefs2(const op_t &x, dref_t type, int outf); ///< See ua_add_off_drefs()

/// Get flags for op_t::dtyp field
idaman flags_t ida_export get_dtyp_flag(char dtype);
/// Get size of opt_::dtyp field
idaman size_t ida_export get_dtyp_size(char dtype);
/// Get op_t::dtyp from size
idaman char ida_export get_dtyp_by_size(asize_t size);


//--------------------------------------------------------------------------
//      K E R N E L   I N T E R F A C E   T O   I D P   F U N C T I O N S
//--------------------------------------------------------------------------
/// Create an instruction at the specified address.
/// This function checks if an instruction is present at the specified address
/// and will try to create one if there is none. It will fail if there is
/// a data item or other items hindering the creation of the new instruction.
/// This function will also fill the ::cmd structure.
/// \param ea  linear address
/// \return the length of the instruction or 0

idaman int ida_export create_insn(ea_t ea);


/// Analyze the specified address and fill ::cmd.
/// This function does not modify the database.
/// It just tries to interpret the specified address as an instruction and fills
/// the ::cmd structure with the results.
/// \param ea  linear address
/// \return the length of the (possible) instruction or 0

idaman int ida_export decode_insn(ea_t ea);


/// Generate text representation for operand #n.
/// This function will generate the text representation of the specified operand.
/// If the instruction is not present in the database, it will be created.
/// This function will also fill the ::cmd structure.
/// \param ea       linear address
/// \param buf      output buffer
/// \param bufsize  size of output buffer
/// \param n        operand number (0,1,2...)
/// \param flags    \ref GETN_ -
///                 Currently only #GETN_NODUMMY is allowed.
/// \return success

idaman bool ida_export ua_outop2(ea_t ea, char *buf, size_t bufsize, int n, int flags=0);


/// Generate text representation of the instruction mnemonics.
/// This function will generate the text representation of the instruction mnemonics,
/// like 'mov', 'add', etc.
/// If the instruction is not present in the database, it will be created.
/// This function will also fill the ::cmd structure.
/// \param ea       linear address
/// \param buf      output buffer
/// \param bufsize  size of output buffer
/// \return pointer to buf or NULL if failure

idaman const char *ida_export ua_mnem(ea_t ea, char *buf, size_t bufsize);

//--------------------------------------------------------------------------
//      Helper functions for the processor emulator/analyzer
//--------------------------------------------------------------------------

/// Decode previous instruction if it exists, fill ::cmd.
/// \return the previous instruction address (#BADADDR-no such insn)

idaman ea_t ida_export decode_prev_insn(ea_t ea);


/// Decode preceding instruction in the execution flow.
/// Prefer far xrefs from addresses < the current to ordinary flows.
/// \return the preceding instruction address (#BADADDR-no such insn) and fill ::cmd.
/// *p_farref will contain 'true' if followed an xref, false otherwise.

idaman ea_t ida_export decode_preceding_insn(ea_t ea, bool *p_farref);


/// Construct a macro instruction.
/// This function may be called from ana()
/// to generate a macro instruction.
///
/// The real work is done by the 'build_macro()' parameter.
/// This callback should be provided by the module writer.
///
/// Here we just create the instruction in the database when the macro
/// generation is turned on/off.
///
/// \param enable       enable macro generation
/// \param build_macro  try to grow the instruction in 'cmd' to a macro
/// \retval true   the macro instruction is generated in 's'
/// \retval false  no macro

idaman bool ida_export construct_macro(bool enable,
                        bool (idaapi *build_macro)(insn_t &s, bool may_go_forward));


/// Guess the jump table address (ibm pc specific)

idaman ea_t ida_export guess_table_address(void);


/// Guess the jump table size

idaman asize_t ida_export guess_table_size(ea_t jump_table);


/// Does the instruction in ::cmd spoil any register from 'regs'?.
/// This function checks the \ref CF_ flags from the instructions array.
/// Only ::o_reg operand types are consulted.
/// \return index in the 'regs' array or -1

idaman int ida_export get_spoiled_reg(const uint32 *regs, size_t n);



#ifndef NO_OBSOLETE_FUNCS
idaman DEPRECATED void ida_export ua_dodata(ea_t ea, int dtype);                       // use ua_dodata2
idaman DEPRECATED bool ida_export ua_outop(ea_t ea, char *buf, size_t bufsize, int n); // use ua_outop2
idaman DEPRECATED bool ida_export ua_stkvar(const op_t &x, adiff_t v);                 // use ua_stkvar2
idaman DEPRECATED int ida_export ua_ana0(ea_t ea);                                     // use decode_insn
idaman DEPRECATED int ida_export ua_code(ea_t ea);                                     // use create_insn
#endif

#pragma pack(pop)
#endif // _UA_HPP
