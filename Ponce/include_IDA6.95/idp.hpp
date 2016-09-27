/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2015 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 */

#ifndef _IDP_HPP
#define _IDP_HPP

#include <fpro.h>
#include <nalt.hpp>
#include <segment.hpp>
#include <funcs.hpp>
#include <ua.hpp>
#include <bitrange.hpp>

#pragma pack(push, 1)           // IDA uses 1 byte alignments!

/*! \file idp.hpp

  \brief Contains definition of the interface to IDP modules.

  The interface consists of 2 structures:
    - definition of target assembler: ::ash
    - definition of current processor: ::ph

  These structures contain information about processor features,
  function pointers, etc.
*/

typedef int help_t; ///< help screen number

struct regval_t;
struct simd_info_t;

/// The interface version number.
/// It must match the version number on the IDA modules.
/// \note This will not change anymore. Use #IDA_SDK_VERSION from pro.h
#define IDP_INTERFACE_VERSION 76

/// Size of the output array for processor_t::calc_next_eas.
#define NEXTEAS_ANSWER_SIZE 32

//-----------------------------------------------------------------------
/// AbstractRegister is deprecated
struct AbstractRegister
{
  virtual uval_t idaapi value(void) const = 0;
  virtual bool idaapi isDef(void) const = 0;
};

/// This structure is used only when detailed
/// information on processor register is needed.
/// Actually is used only for 80x86 processors.
struct rginfo
{
  AbstractRegister *low;
  AbstractRegister *high;
  AbstractRegister *word;
};

//-----------------------------------------------------------------------
/// Structure used to describe byte streams
/// (for "ret" instruction and empirics)
struct bytes_t
{
  uchar len;
  const uchar *bytes;
};

//-----------------------------------------------------------------------
/// \defgroup IDPOPT_T Option value types
/// Passed as 'value_type' parameter to ::set_options_t callbacks
//@{
#define IDPOPT_STR 1    ///< string constant (char *)
#define IDPOPT_NUM 2    ///< number (uval_t *)
#define IDPOPT_BIT 3    ///< bit, yes/no (int *)
#define IDPOPT_I64 5    ///< 64bit number (int64 *)
#define IDPOPT_CST 6    ///< lexer (lexer_t*)
                        ///< Custom type, starting with a '{'
                        ///< Values of this type should be handled by
                        ///< ::set_options_t callbacks. E.g.,:
                        ///< \code
                        ///< ERROR_STRINGS =
                        ///< {
                        ///<   {0, "Unknown error"},
                        ///<   {1, "Missing filename"},
                        ///<   {5, "Out-of-memory"}
                        ///< }
                        ///< \endcode
                        ///< For values of this type, the data that will
                        ///< be passed as the callback's 'value' parameter
                        ///< is the lexer instance that is being used
                        ///< to parse the configuration file.
                        ///< You can use \ref parse_json() (see parsejson.hpp)
                        ///< to parse JSON-format data
                        ///< NB: the '{' is already consumed by the parser,
                        ///< so you need to push it again if it's a part of the JSON object
//@}

/// \defgroup IDPOPT_RET Option result codes
/// Predefined return values for ::set_options_t callbacks
//@{
#define IDPOPT_OK       NULL            ///< ok
#define IDPOPT_BADKEY   ((char*)1)      ///< illegal keyword
#define IDPOPT_BADTYPE  ((char*)2)      ///< illegal type of value
#define IDPOPT_BADVALUE ((char*)3)      ///< illegal value (bad range, for example)
//@}


/// Callback - called when a config directive is processed in IDA.
/// Also see read_config_file() and processor_t::set_idp_options
/// \param keyword     keyword encountered in IDA.CFG/user config file.
///                    if NULL, then an interactive dialog form should be displayed
/// \param value_type  type of value of the keyword - one of \ref IDPOPT_T
/// \param value       pointer to value
/// \return one of \ref IDPOPT_RET, otherwise a pointer to an error message

typedef const char *(idaapi set_options_t)(
        const char *keyword,
        int value_type,
        const void *value);

/// \defgroup IDAOPT_PRIO Option priority
/// Not used yet in processor modules but is used in debugger (idd.hpp).
/// Normally default priority option does not overwrite existing value whereas
/// high priority one does.
/// High priority options may be stored in the database to be available
/// in the next session
//@{
#define IDPOPT_PRI_DEFAULT 1  ///< default priority - taken from config file
#define IDPOPT_PRI_HIGH    2  ///< high priority - received from UI or script function
//@}


//-------------------------------------------------------------------------
/// Parse the value type for the value token 'value'.
/// This is mostly used for converting from values that a cfgopt_handler_t
/// receives, into data that callbacks
///  - processor_t::set_idp_options
///  - debugger_t::set_dbg_options
/// expect.
///
/// Plugins that wish to use options shouldn't rely on this,
/// and use the cfgopt_t utility instead.
///
/// \param out parsed data
/// \param lx the lexer in use
/// \param value the value token
/// \return true if guessing didn't lead to an error, false otherwise.
///         note that even if 'true' is returned, it doesn't mean the
///         type could be guessed: merely that no syntax error occured.
class lexer_t;
struct token_t;
idaman bool ida_export parse_config_value(
        idc_value_t *out,
        lexer_t *lx,
        const token_t &value);

//-------------------------------------------------------------------------
typedef const char *(idaapi cfgopt_handler_t)(
        lexer_t *lx,
        const token_t &keyword,
        const token_t &value);

//-----------------------------------------------------------------------
/// used by cfgopt_t. You shouldn't have to deal with those directly.
#define IDPOPT_NUM_INT     (0)
#define IDPOPT_NUM_CHAR    (1 << 24)
#define IDPOPT_NUM_SHORT   (2 << 24)
#define IDPOPT_NUM_RANGE   (1 << 26)
#define IDPOPT_NUM_UNS     (1 << 27)

#define IDPOPT_BIT_UINT    0
#define IDPOPT_BIT_UCHAR   (1 << 24)
#define IDPOPT_BIT_USHORT  (2 << 24)
#define IDPOPT_BIT_BOOL    (3 << 24)
#define IDPOPT_BIT_INVRES  (1 << 26)

#define IDPOPT_STR_QSTRING (1 << 24)
#define IDPOPT_STR_LONG    (1 << 25)

#define IDPOPT_I64_RANGES  (1 << 24)
#define IDPOPT_I64_UNS     (1 << 25)

//-------------------------------------------------------------------------
struct cfgopt_t;
idaman const char *ida_export cfgopt_t__apply(
        const cfgopt_t *_this,
        int vtype,
        const void *vdata);

//-------------------------------------------------------------------------
// cfgopt_t objects are suitable for being statically initialized, and
// passed to 'read_config_file'.
//
// E.g.,
// ---
// static const cfgopt_t g_opts[] =
// {
//   cfgopt_t("AUTO_UNDEFINE", &auto_undefine, -1, 1),
//   cfgopt_t("NOVICE", &novice, true),
//   cfgopt_t("EDITOR", editor_buf, sizeof(editor_buf)),
//   cfgopt_t("SCREEN_PALETTE", set_screen_palette), // specific handler for SCREEN_PALETTE
// };
//
// ...
//
// read_config_file("myfile", g_opts, qnumber(g_opts), other_handler)
// ---
//
// NOTES:
//   * so-called 'long' strings (the default) can span on multiple lines,
//     and are terminated by a ';'
struct cfgopt_t
{
  const char *name;
  void *ptr;
  int flags;
  union
  {
    size_t buf_size;
    struct
    {
      int64 min;
      int64 max;
    } num_range;
    uint32 bit_flags;
  };

  // IDPOPT_STR
  cfgopt_t(const char *_n, char *_p, size_t _sz, bool _long = true)
    : name(_n), ptr(_p), flags(IDPOPT_STR | (_long ? IDPOPT_STR_LONG : 0))
  { buf_size = _sz; }
  cfgopt_t(const char *_n, qstring *_p, bool _long = true)
    : name(_n), ptr(_p), flags(IDPOPT_STR | IDPOPT_STR_QSTRING | (_long ? IDPOPT_STR_LONG : 0))
  {}

  // IDPOPT_NUM
  cfgopt_t(const char *_n, int *_p)
    : name(_n), ptr(_p), flags(IDPOPT_NUM) {}
  cfgopt_t(const char *_n, uint *_p)
    : name(_n), ptr(_p), flags(IDPOPT_NUM | IDPOPT_NUM_UNS) {}
  cfgopt_t(const char *_n, char *_p)
    : name(_n), ptr(_p), flags(IDPOPT_NUM | IDPOPT_NUM_CHAR) {}
  cfgopt_t(const char *_n, uchar *_p)
    : name(_n), ptr(_p), flags(IDPOPT_NUM | IDPOPT_NUM_UNS | IDPOPT_NUM_CHAR) {}
  cfgopt_t(const char *_n, short *_p)
    : name(_n), ptr(_p), flags(IDPOPT_NUM | IDPOPT_NUM_SHORT) {}
  cfgopt_t(const char *_n, ushort *_p)
    : name(_n), ptr(_p), flags(IDPOPT_NUM | IDPOPT_NUM_UNS | IDPOPT_NUM_SHORT) {}
  // IDPOPT_NUM + ranges
  cfgopt_t(const char *_n, int *_p, int _min, int _max)
    : name(_n), ptr(_p), flags(IDPOPT_NUM | IDPOPT_NUM_RANGE) { num_range.min = _min; num_range.max = _max; }
  cfgopt_t(const char *_n, uint *_p, uint _min, uint _max)
    : name(_n), ptr(_p), flags(IDPOPT_NUM | IDPOPT_NUM_UNS | IDPOPT_NUM_RANGE) { num_range.min = _min; num_range.max = _max; }
  cfgopt_t(const char *_n, char *_p, char _min, char _max)
    : name(_n), ptr(_p), flags(IDPOPT_NUM | IDPOPT_NUM_CHAR | IDPOPT_NUM_RANGE) { num_range.min = _min; num_range.max = _max; }
  cfgopt_t(const char *_n, uchar *_p, uchar _min, uchar _max)
    : name(_n), ptr(_p), flags(IDPOPT_NUM | IDPOPT_NUM_UNS | IDPOPT_NUM_CHAR | IDPOPT_NUM_RANGE) { num_range.min = _min; num_range.max = _max; }
  cfgopt_t(const char *_n, short *_p, short _min, short _max)
    : name(_n), ptr(_p), flags(IDPOPT_NUM | IDPOPT_NUM_RANGE | IDPOPT_NUM_SHORT) { num_range.min = _min; num_range.max = _max; }
  cfgopt_t(const char *_n, ushort *_p, ushort _min, ushort _max)
    : name(_n), ptr(_p), flags(IDPOPT_NUM | IDPOPT_NUM_UNS | IDPOPT_NUM_RANGE | IDPOPT_NUM_SHORT) { num_range.min = _min; num_range.max = _max; }

  // IDPOPT_BIT
  cfgopt_t(const char *_n, bool *_p, bool _flags, const int *optflgs = NULL) : name(_n), ptr(_p), flags(IDPOPT_BIT | IDPOPT_BIT_BOOL) { bit_flags = _flags; if ( optflgs != NULL ) flags |= *optflgs; }
  cfgopt_t(const char *_n, uchar *_p, uchar _flags, const int *optflgs = NULL) : name(_n), ptr(_p), flags(IDPOPT_BIT | IDPOPT_BIT_UCHAR) { bit_flags = _flags; if ( optflgs != NULL ) flags |= *optflgs; }
  cfgopt_t(const char *_n, ushort *_p, ushort _flags, const int *optflgs = NULL) : name(_n), ptr(_p), flags(IDPOPT_BIT | IDPOPT_BIT_USHORT) { bit_flags = _flags; if ( optflgs != NULL ) flags |= *optflgs; }
  cfgopt_t(const char *_n, uint32 *_p, uint32 _flags, const int *optflgs = NULL) : name(_n), ptr(_p), flags(IDPOPT_BIT) { bit_flags = _flags; if ( optflgs != NULL ) flags |= *optflgs; }

  // IDPOPT_I64
  cfgopt_t(const char *_n, int64 *_p) : name(_n), ptr(_p), flags(IDPOPT_I64) {}
  cfgopt_t(const char *_n, uint64 *_p) : name(_n), ptr(_p), flags(IDPOPT_I64 | IDPOPT_NUM_UNS) {}
  // IDPOPT_I64 + ranges
  cfgopt_t(const char *_n, int64 *_p, int64 _min, int64 _max)
    : name(_n), ptr(_p), flags(IDPOPT_I64 | IDPOPT_I64_RANGES) { num_range.min = _min; num_range.max = _max; }
  cfgopt_t(const char *_n, uint64 *_p, uint64 _min, uint64 _max)
    : name(_n), ptr(_p), flags(IDPOPT_I64 | IDPOPT_I64_UNS | IDPOPT_I64_RANGES) { num_range.min = _min; num_range.max = _max; }

  // IDPOPT_CST
  cfgopt_t(const char *_n, cfgopt_handler_t *_p) : name(_n), ptr((void *) _p), flags(IDPOPT_CST) {}

  int type() const { return flags & 0xf; }
  int qualifier() const { return flags & 0xf000000; }

  const char *apply(int vtype, const void *vdata) const { return cfgopt_t__apply(this, vtype, vdata); }
};

/// Parse the input, and apply options.
///
/// \param input      input file name, or string
/// \param is_file    is input a string, or a file name
/// \param opts       options destcriptions
/// \param nopts      the number of entries present in the 'opts' array
/// \param defhdlr    a handler to be called, if a directive couldn't be found in 'opts'
/// \param defines    a list of preprocessor identifiers to define (so it is
///                   possible to use #ifdef checks in the file.)
///                   NB: the actual identifier defined by the parser will be
///                   surrounded with double underscores (e.g., passing 'FOO'
///                   will result in '__FOO__' being defined)
///                   Additionally, the parser will also define a similar macro
///                   with the current processor name (e.g., __ARM__)
/// \param ndefines   the number of defines in the list
/// \return true if parsing finished without errors, false if there was a
///         syntax error, callback returned an error, or no file was found
///         at all.

idaman bool ida_export read_config(
        const char *input,
        bool is_file,
        const cfgopt_t opts[],
        size_t nopts,
        cfgopt_handler_t *defhdlr = NULL,
        const char *const *defines = NULL,
        size_t ndefines = 0);


/// Search for all IDA system files with the given name.
/// This function will search, in that order, for the following files:
///   -# %IDADIR%/cfg/<file>
///   -# for each directory 'ONEDIR' in %IDAUSR%: %ONEDIR%/cfg/<file>
///
/// For each directive in each of those files, the same processing as
/// that of read_config will be performed.

inline bool read_config_file(
        const char *filename,
        const cfgopt_t opts[],
        size_t nopts,
        cfgopt_handler_t *defhdlr = NULL,
        const char *const *defines = NULL,
        size_t ndefines = 0)
{
  return read_config(filename, true, opts, nopts, defhdlr, defines, ndefines);
}


/// For each directive in 'string', the same processing as that of
/// read_config will be performed.
inline bool read_config_string(
        const char *string,
        const cfgopt_t opts[],
        size_t nopts,
        cfgopt_handler_t *defhdlr = NULL,
        const char *const *defines = NULL,
        size_t ndefines = 0)
{
  return read_config(string, false, opts, nopts, defhdlr, defines, ndefines);
}


/// Get one of config parameters defined by CC_PARMS in ida.cfg.
/// All parameters for all compilers are stored in local map during last read
/// of ida.cfg - this function just returns previously stored parameter value for
/// given compiler (NULL if no such parameter)
idaman const char *ida_export cfg_get_cc_parm(comp_t compid, const char *name);


/// Get header path config parameter from ida.cfg.
/// Also see cfg_get_cc_parm()

inline const char *cfg_get_cc_header_path(comp_t compid)
{
  return cfg_get_cc_parm(compid, "HEADER_PATH");
}


/// Get predefined macros config parameter from ida.cfg.
/// Also see cfg_get_cc_parm()

inline const char *cfg_get_cc_predefined_macros(comp_t compid)
{
  return cfg_get_cc_parm(compid, "PREDEFINED_MACROS");
}

//-----------------------------------------------------------------------
/// Internal representation of processor instructions.
/// Definition of all internal instructions are kept in special arrays.
/// One of such arrays describes instruction names and features.
struct instruc_t
{
  const char *name;       ///< instruction name
  uint32 feature;         ///< combination of \ref CF_
/// \defgroup CF_ Instruction feature bits
/// Used by instruc_t::feature
//@{
#define CF_STOP 0x00001   ///< Instruction doesn't pass execution to the
                          ///< next instruction
#define CF_CALL 0x00002   ///< CALL instruction (should make a procedure here)
#define CF_CHG1 0x00004   ///< The instruction modifies the first operand
#define CF_CHG2 0x00008   ///< The instruction modifies the second operand
#define CF_CHG3 0x00010   ///< The instruction modifies the third operand
#define CF_CHG4 0x00020   ///< The instruction modifies 4 operand
#define CF_CHG5 0x00040   ///< The instruction modifies 5 operand
#define CF_CHG6 0x00080   ///< The instruction modifies 6 operand
#define CF_USE1 0x00100   ///< The instruction uses value of the first operand
#define CF_USE2 0x00200   ///< The instruction uses value of the second operand
#define CF_USE3 0x00400   ///< The instruction uses value of the third operand
#define CF_USE4 0x00800   ///< The instruction uses value of the 4 operand
#define CF_USE5 0x01000   ///< The instruction uses value of the 5 operand
#define CF_USE6 0x02000   ///< The instruction uses value of the 6 operand
#define CF_JUMP 0x04000   ///< The instruction passes execution using indirect
                          ///< jump or call (thus needs additional analysis)
#define CF_SHFT 0x08000   ///< Bit-shift instruction (shl,shr...)
#define CF_HLL  0x10000   ///< Instruction may be present in a high level
                          ///< language function.
//@}
};


/// Does the specified instruction have the specified feature?

idaman bool ida_export InstrIsSet(int icode,int bit);


/// Is the instruction at the specified address a "call"?

idaman bool ida_export is_call_insn(ea_t ea);


/// Is the instruction at the specified address a "return"?

idaman bool ida_export is_ret_insn(ea_t ea, bool strict=true);


/// Is the instruction at the specified address an indirect jump?

idaman bool ida_export is_indirect_jump_insn(ea_t ea);


/// Is the current instruction (::cmd) the end of a basic block?

idaman bool ida_export is_basic_block_end(bool call_insn_stops_block);

//-----------------------------------------------------------------------
// Structures related to checkarg_dispatch()

/// This structure is used as a parameter for ::chkarg_preline.
/// It contains pointers to output buffers.
/// All buffers are initialized with empty strings before
/// calling ::chkarg_preline.
struct s_preline
{
    char  *iaflg;   ///< flag of indirect addressing ([] in x86 or ()/@ in pdp11)
                    ///< This is not an array but a single byte.
                    ///< 0-no indirection, 1-operand uses indirection
    char  *prefix;  ///< place for all prefixes (near, far, word ptr, etc.)
    char  *seg;     ///< place for segment descriptor substring (e.g. 'es:')
    char  *reg;     ///< place for register's part (e.g. eax*8+edx)
    char  *offset;  ///< place for all others (label names, constant, etc.)
#define PRELINE_SIZE 100 ///< all output arrays are 100 bytes long
};

#ifdef __BORLANDC__
#pragma option push -b-
#endif

/// Operation binary codes.
/// The kernel replaces all operations in the operand by binary code
/// from the following enumeration.
/// The order of operations is FIXED!
enum ca_operation_t ENUM_SIZE(char)
{
  ca_lev_on   = 020,  ///< (
  ca_lev_off  = 1,    ///< )
  ca_not      = 2,    ///< ~              (NOT in x86 assembler)
  ca_minus    = 3,    ///< --
  ca_plus     = 4,    ///< +
#define CA_PLUS_STR     "\4"
  ca_mod      = 5,    ///< %              (MOD in x86 assembler)
  ca_div_u    = 6,    ///< / (unsigned)   (not implemented in x86 assembler)
  ca_div      = 7,    ///< /
  ca_mul      = 010,  ///< *
  ca_and      = 011,  ///< &
  ca_or       = 012,  ///< |
  ca_xor      = 013,  ///< ^
  ca_shl      = 014,  ///< <<
  ca_shr      = 015,  ///< >>
        // end of current implementation of 'operdim' table
//  ca_RESERVED_1 = 016,
//  ca_RESERVED_2 = 017,
  ca_UN_MSK   = 020,                    ///< mask
  ca_not_UN   = ca_UN_MSK | ca_not,     ///< 022
  ca_minus_UN = ca_UN_MSK | ca_minus,   ///< 023
  ca_h_delim  = 037,                    ///< delimeter for holerite constant
};

/// Command codes for \ash{checkarg_dispatch()}
enum _chkarg_cmd ENUM_SIZE(uchar)
{
  chkarg_atomprefix = 1,   ///< Scan the operand for special prefixes like SEG, OFFSET, SIZE (see \ref CA_PRF_).
                           ///< arguments: char **operand, char *flgoff
/// \defgroup CA_PRF_ Operand prefixes
/// Used with ::chkarg_atomprefix
//@{
#define CA_PRF_NONE       0   ///< operand has no prefixes (unknown)
#define CA_PRF_SEG        -1  ///< is segment reference (x86: seg arg)
#define CA_PRF_OFFSET     1   ///< is offset to (x86: offset arg)
#define CA_PRF_SIZE       2   ///< request size of (x86: SIZE name)
//@}
  chkarg_preline    = 2,   ///< Parse operand string to and fill the ::s_preline structure
                           ///< arguments: char *operstr, ::s_preline *S
  chkarg_operseg    = 3,   ///< Get the default segment for the specified operand
                           ///< arguments: char *outseg, int opernum
  chkarg_cmpseg     = 4,   ///< Check if the selectors are interchangeable
                           ///< arguments: ::sel_t sel1, ::sel_t sel2
  chkarg_gettable   = 5    ///< Request the operation mnemonic table
                           ///< arguments: NULL, char * const **outptr
                           ///< This callback must be implemented
};

#ifdef __BORLANDC__
#pragma option pop
#endif

//=====================================================================
/// Describes the target assembler.
/// An IDP module may have several target assemblers.
/// In this case you should create a structure for each supported
/// assembler.
struct asm_t
{
  uint32 flag;                          ///< \ref AS_
/// \defgroup AS_ Assembler feature bits
/// Used by asm_t::flag.
//@{
#define AS_OFFST      0x00000001L       ///< offsets are 'offset xxx' ?
#define AS_COLON      0x00000002L       ///< create colons after data names ?
#define AS_UDATA      0x00000004L       ///< can use '?' in data directives

#define AS_2CHRE      0x00000008L       ///< double char constants are: "xy
#define AS_NCHRE      0x00000010L       ///< char constants are: 'x
#define AS_N2CHR      0x00000020L       ///< can't have 2 byte char consts

                                        // ASCII directives:
#define AS_1TEXT      0x00000040L       ///<   1 text per line, no bytes
#define AS_NHIAS      0x00000080L       ///<   no characters with high bit
#define AS_NCMAS      0x00000100L       ///<   no commas in ascii directives

#define AS_HEXFM      0x00000E00L       ///< mask - hex number format
#define ASH_HEXF0     0x00000000L       ///<   34h
#define ASH_HEXF1     0x00000200L       ///<   h'34
#define ASH_HEXF2     0x00000400L       ///<   34
#define ASH_HEXF3     0x00000600L       ///<   0x34
#define ASH_HEXF4     0x00000800L       ///<   $34
#define ASH_HEXF5     0x00000A00L       ///<   <^R   > (radix)
#define AS_DECFM      0x00003000L       ///< mask - decimal number format
#define ASD_DECF0     0x00000000L       ///<   34
#define ASD_DECF1     0x00001000L       ///<   #34
#define ASD_DECF2     0x00002000L       ///<   34.
#define ASD_DECF3     0x00003000L       ///<   .34
#define AS_OCTFM      0x0001C000L       ///< mask - octal number format
#define ASO_OCTF0     0x00000000L       ///<   123o
#define ASO_OCTF1     0x00004000L       ///<   0123
#define ASO_OCTF2     0x00008000L       ///<   123
#define ASO_OCTF3     0x0000C000L       ///<   @123
#define ASO_OCTF4     0x00010000L       ///<   o'123
#define ASO_OCTF5     0x00014000L       ///<   123q
#define ASO_OCTF6     0x00018000L       ///<   ~123
#define ASO_OCTF7     0x0001C000L       ///<   q'123
#define AS_BINFM      0x000E0000L       ///< mask - binary number format
#define ASB_BINF0     0x00000000L       ///<   010101b
#define ASB_BINF1     0x00020000L       ///<   ^B010101
#define ASB_BINF2     0x00040000L       ///<   %010101
#define ASB_BINF3     0x00060000L       ///<   0b1010101
#define ASB_BINF4     0x00080000L       ///<   b'1010101
#define ASB_BINF5     0x000A0000L       ///<   b'1010101'

#define AS_UNEQU      0x00100000L       ///< replace undefined data items with EQU (for ANTA's A80)
#define AS_ONEDUP     0x00200000L       ///< One array definition per line
#define AS_NOXRF      0x00400000L       ///< Disable xrefs during the output file generation
#define AS_XTRNTYPE   0x00800000L       ///< Assembler understands type of extern symbols as ":type" suffix
#define AS_RELSUP     0x01000000L       ///< Checkarg: 'and','or','xor' operations with addresses are possible
#define AS_LALIGN     0x02000000L       ///< Labels at "align" keyword are supported.
#define AS_NOCODECLN  0x04000000L       ///< don't create colons after code names
#define AS_NOTAB      0x08000000L       ///< Disable tabulation symbols during the output file generation
#define AS_NOSPACE    0x10000000L       ///< No spaces in expressions
#define AS_ALIGN2     0x20000000L       ///< .align directive expects an exponent rather than a power of 2
                                        ///< (.align 5 means to align at 32byte boundary)
#define AS_ASCIIC     0x40000000L       ///< ascii directive accepts C-like escape sequences
                                        ///< (\\n,\\x01 and similar)
#define AS_ASCIIZ     0x80000000L       ///< ascii directive inserts implicit zero byte at the end
//@}
  uint16 uflag;                         ///< user defined flags (local only for IDP)
                                        ///< you may define and use your own bits
  const char *name;                     ///< Assembler name (displayed in menus)
  help_t help;                          ///< Help screen number, 0 - no help
  const char *const *header;            ///< array of automatically generated header lines
                                        ///< they appear at the start of disassembled text
  const uint16 *badworks;               ///< array of unsupported instructions
                                        ///< (array of \cmd{itype}, zero terminated)
  const char *origin;                   ///< org directive
  const char *end;                      ///< end directive
  const char *cmnt;                     ///< comment string (see also cmnt2)
  char ascsep;                          ///< ASCII string delimiter
  char accsep;                          ///< ASCII char constant delimiter
  const char *esccodes;                 ///< ASCII special chars
                                        ///< (they can't appear in character and
                                        ///< ascii constants)

  // Data representation (db,dw,...):
  const char *a_ascii;                  ///< ASCII string directive
  const char *a_byte;                   ///< byte directive
  const char *a_word;                   ///< word directive
  const char *a_dword;                  ///< NULL if not allowed
  const char *a_qword;                  ///< NULL if not allowed
  const char *a_oword;                  ///< NULL if not allowed
  const char *a_float;                  ///< float;  4bytes; NULL if not allowed
  const char *a_double;                 ///< double; 8bytes; NULL if not allowed
  const char *a_tbyte;                  ///< long double;    NULL if not allowed
  const char *a_packreal;               ///< packed decimal real NULL if not allowed
  const char *a_dups;                   ///< array keyword. the following
                                        ///< sequences may appear:
                                        ///<      - #h  header
                                        ///<      - #d  size
                                        ///<      - #v  value
                                        ///<      - #s(b,w,l,q,f,d,o)  size specifiers
                                        ///<                        for byte,word,
                                        ///<                            dword,qword,
                                        ///<                            float,double,oword
  const char *a_bss;                    ///< uninitialized data directive
                                        ///< should include '%s' for the
                                        ///< size of data
  const char *a_equ;                    ///< 'equ' Used if AS_UNEQU is set
  const char *a_seg;                    ///< 'seg ' prefix (example: push seg seg001)

  /// Pointer to checkarg_dispatch function. If NULL, checkarg won't be called.
  bool (idaapi *checkarg_dispatch)(void *a1, void *a2, uchar cmd);
  void *_UNUSED1_was_atomprefix;
  void *_UNUSED2_was_checkarg_operations;

  /// Translation to use in character and string constants.
  /// Usually 1:1, i.e. trivial translation (may specify NULL)
  /// If specified, must be 256 chars long
  const uchar *XlatAsciiOutput;

  const char *a_curip;                  ///< current IP (instruction pointer) symbol in assembler

  /// Generate function header lines.
  /// If NULL, then function headers are displayed as normal lines
  void (idaapi *func_header)(func_t *);

  /// Generate function footer lines.
  /// If NULL, then a comment line is displayed
  void (idaapi *func_footer)(func_t *);

  const char *a_public;                 ///< "public" name keyword. NULL-gen default, ""-do not generate
  const char *a_weak;                   ///< "weak"   name keyword. NULL-gen default, ""-do not generate
  const char *a_extrn;                  ///< "extern" name keyword
  const char *a_comdef;                 ///< "comm" (communal variable)

  /// Get name of type of item at ea or id.
  /// (i.e. one of: byte,word,dword,near,far,etc...)
  ssize_t (idaapi *get_type_name)(flags_t flag,
                                  ea_t ea_or_id,
                                  char *buf,
                                  size_t bufsize);

  const char *a_align;                  ///< "align" keyword

  char lbrace;                          ///< left brace used in complex expressions
  char rbrace;                          ///< right brace used in complex expressions

  const char *a_mod;                    ///< %  mod     assembler time operation
  const char *a_band;                   ///< &  bit and assembler time operation
  const char *a_bor;                    ///< |  bit or  assembler time operation
  const char *a_xor;                    ///< ^  bit xor assembler time operation
  const char *a_bnot;                   ///< ~  bit not assembler time operation
  const char *a_shl;                    ///< << shift left assembler time operation
  const char *a_shr;                    ///< >> shift right assembler time operation
  const char *a_sizeof_fmt;             ///< size of type (format string)

  uint32 flag2;                         ///< \ref AS2_
/// \defgroup AS2_ Secondary assembler feature bits
/// Used by asm_t::flag2
//@{
#define AS2_BRACE     0x00000001        ///< Use braces for all expressions
#define AS2_STRINV    0x00000002        ///< Invert meaning of \inf{wide_high_byte_first} for text strings
                                        ///< (For processors with bytes bigger than 8 bits)
#define AS2_BYTE1CHAR 0x00000004        ///< One symbol per processor byte
                                        ///< Meaningful only for wide byte processors
#define AS2_IDEALDSCR 0x00000008        ///< Description of struc/union is in
                                        ///< the 'reverse' form (keyword before name)
                                        ///< the same as in borland tasm ideal
#define AS2_TERSESTR  0x00000010        ///< 'terse' structure initialization form
                                        ///< NAME<fld,fld,...> is supported
#define AS2_COLONSUF  0x00000020        ///< addresses may have ":xx" suffix
                                        ///< this suffix must be ignored when extracting
                                        ///< the address under the cursor
#define AS2_YWORD     0x00000040        ///< a_yword field is present and valid
//@}

  const char *cmnt2;                    ///< comment close string (usually NULL)
                                        ///< this is used to denote a string which
                                        ///< closes comments, for example, if the
                                        ///< comments are represented with (* ... *)
                                        ///< then cmnt = "(*" and cmnt2 = "*)"
  const char *low8;                     ///< low8 operation, should contain %s for the operand
  const char *high8;                    ///< high8
  const char *low16;                    ///< low16
  const char *high16;                   ///< high16
  const char *a_include_fmt;            ///< the include directive (format string)
  const char *a_vstruc_fmt;             ///< if a named item is a structure and displayed
                                        ///< in the verbose (multiline) form then display the name
                                        ///< as printf(a_strucname_fmt, typename)
                                        ///< (for asms with type checking, e.g. tasm ideal)
  const char *a_3byte;                  ///< 3-byte data
  const char *a_rva;                    ///< 'rva' keyword for image based offsets
                                        ///< (see #REFINFO_RVAOFF)
  const char *a_yword;                  ///< 32-byte (256-bit) data; NULL if not allowed
                                        ///< requires #AS2_YWORD
};

#ifdef NO_OBSOLETE_FUNCS
#define OBSOLETE(event) obsolete_ ## event
#else
#define OBSOLETE(event) event
#endif

//=====================================================================
/// Describes a processor module (IDP).
/// An IDP file may have only one such structure called LPH.
/// The kernel will copy it to ::ph structure and use ::ph.
struct processor_t
{
  int version;                    ///< Expected kernel version,
                                  ///<   should be #IDP_INTERFACE_VERSION
  int id;                         ///< one of \ref PLFM_
/// \defgroup PLFM_ Processor IDs
/// Used by processor_t::id.
/// Numbers above 0x8000 are reserved for the third-party modules
//@{
#define PLFM_386        0         ///< Intel 80x86
#define PLFM_Z80        1         ///< 8085, Z80
#define PLFM_I860       2         ///< Intel 860
#define PLFM_8051       3         ///< 8051
#define PLFM_TMS        4         ///< Texas Instruments TMS320C5x
#define PLFM_6502       5         ///< 6502
#define PLFM_PDP        6         ///< PDP11
#define PLFM_68K        7         ///< Motorola 680x0
#define PLFM_JAVA       8         ///< Java
#define PLFM_6800       9         ///< Motorola 68xx
#define PLFM_ST7        10        ///< SGS-Thomson ST7
#define PLFM_MC6812     11        ///< Motorola 68HC12
#define PLFM_MIPS       12        ///< MIPS
#define PLFM_ARM        13        ///< Advanced RISC Machines
#define PLFM_TMSC6      14        ///< Texas Instruments TMS320C6x
#define PLFM_PPC        15        ///< PowerPC
#define PLFM_80196      16        ///< Intel 80196
#define PLFM_Z8         17        ///< Z8
#define PLFM_SH         18        ///< Renesas (formerly Hitachi) SuperH
#define PLFM_NET        19        ///< Microsoft Visual Studio.Net
#define PLFM_AVR        20        ///< Atmel 8-bit RISC processor(s)
#define PLFM_H8         21        ///< Hitachi H8/300, H8/2000
#define PLFM_PIC        22        ///< Microchip's PIC
#define PLFM_SPARC      23        ///< SPARC
#define PLFM_ALPHA      24        ///< DEC Alpha
#define PLFM_HPPA       25        ///< Hewlett-Packard PA-RISC
#define PLFM_H8500      26        ///< Hitachi H8/500
#define PLFM_TRICORE    27        ///< Tasking Tricore
#define PLFM_DSP56K     28        ///< Motorola DSP5600x
#define PLFM_C166       29        ///< Siemens C166 family
#define PLFM_ST20       30        ///< SGS-Thomson ST20
#define PLFM_IA64       31        ///< Intel Itanium IA64
#define PLFM_I960       32        ///< Intel 960
#define PLFM_F2MC       33        ///< Fujistu F2MC-16
#define PLFM_TMS320C54  34        ///< Texas Instruments TMS320C54xx
#define PLFM_TMS320C55  35        ///< Texas Instruments TMS320C55xx
#define PLFM_TRIMEDIA   36        ///< Trimedia
#define PLFM_M32R       37        ///< Mitsubishi 32bit RISC
#define PLFM_NEC_78K0   38        ///< NEC 78K0
#define PLFM_NEC_78K0S  39        ///< NEC 78K0S
#define PLFM_M740       40        ///< Mitsubishi 8bit
#define PLFM_M7700      41        ///< Mitsubishi 16bit
#define PLFM_ST9        42        ///< ST9+
#define PLFM_FR         43        ///< Fujitsu FR Family
#define PLFM_MC6816     44        ///< Motorola 68HC16
#define PLFM_M7900      45        ///< Mitsubishi 7900
#define PLFM_TMS320C3   46        ///< Texas Instruments TMS320C3
#define PLFM_KR1878     47        ///< Angstrem KR1878
#define PLFM_AD218X     48        ///< Analog Devices ADSP 218X
#define PLFM_OAKDSP     49        ///< Atmel OAK DSP
#define PLFM_TLCS900    50        ///< Toshiba TLCS-900
#define PLFM_C39        51        ///< Rockwell C39
#define PLFM_CR16       52        ///< NSC CR16
#define PLFM_MN102L00   53        ///< Panasonic MN10200
#define PLFM_TMS320C1X  54        ///< Texas Instruments TMS320C1x
#define PLFM_NEC_V850X  55        ///< NEC V850 and V850ES/E1/E2
#define PLFM_SCR_ADPT   56        ///< Processor module adapter for processor modules written in scripting languages
#define PLFM_EBC        57        ///< EFI Bytecode
#define PLFM_MSP430     58        ///< Texas Instruments MSP430
#define PLFM_SPU        59        ///< Cell Broadband Engine Synergistic Processor Unit
#define PLFM_DALVIK     60        ///< Android Dalvik Virtual Machine
#define PLFM_65C816     61        ///< 65802/65816
#define PLFM_M16C       62        ///< Renesas M16C
#define PLFM_ARC        63        ///< Argonaut RISC Core
#define PLFM_UNSP       64        ///< SunPlus unSP
#define PLFM_TMS320C28  65        ///< Texas Instruments TMS320C28x
#define PLFM_DSP96K     66        ///< Motorola DSP96000
#define PLFM_SPC700     67        ///< Sony SPC700
//@}

  uint32 flag;                    ///< \ref PR_
/// \defgroup PR_ Processor feature bits
/// Used by processor_t::flag
//@{
#define PR_SEGS       0x000001    ///< has segment registers?
#define PR_USE32      0x000002    ///< supports 32-bit addressing?
#define PR_DEFSEG32   0x000004    ///< segments are 32-bit by default
#define PR_RNAMESOK   0x000008    ///< allow user register names for location names
//#define PR_DB2CSEG    0x0010    // .byte directive in code segments
//                                // should define even number of bytes
//                                // (used by AVR processor)
#define PR_ADJSEGS    0x000020    ///< IDA may adjust segments' starting/ending addresses.
#define PR_DEFNUM     0x0000C0    ///< mask - default number representation
#define PRN_HEX       0x000000    ///<      hex
#define PRN_OCT       0x000040    ///<      octal
#define PRN_DEC       0x000080    ///<      decimal
#define PRN_BIN       0x0000C0    ///<      binary
#define PR_WORD_INS   0x000100    ///< instruction codes are grouped 2bytes in binary line prefix
#define PR_NOCHANGE   0x000200    ///< The user can't change segments and code/data attributes
                                  ///< (display only)
#define PR_ASSEMBLE   0x000400    ///< Module has a built-in assembler and understands IDP_ASSEMBLE
#define PR_ALIGN      0x000800    ///< All data items should be aligned properly
#define PR_TYPEINFO   0x001000    ///< the processor module supports type information callbacks
                                  ///< ALL OF THEM SHOULD BE IMPLEMENTED!
#define PR_USE64      0x002000    ///< supports 64-bit addressing?
#define PR_SGROTHER   0x004000    ///< the segment registers don't contain the segment selectors.
#define PR_STACK_UP   0x008000    ///< the stack grows up
#define PR_BINMEM     0x010000    ///< the processor module provides correct segmentation for binary files
                                  ///< (i.e. it creates additional segments)
                                  ///< The kernel will not ask the user to specify the RAM/ROM sizes
#define PR_SEGTRANS   0x020000    ///< the processor module supports the segment translation feature
                                  ///< (meaning it calculates the code
                                  ///< addresses using the codeSeg() function)
#define PR_CHK_XREF   0x040000    ///< don't allow near xrefs between segments with different bases
#define PR_NO_SEGMOVE 0x080000    ///< the processor module doesn't support move_segm()
                                  ///< (i.e. the user can't move segments)
#define PR_FULL_HIFXP 0x100000    ///< ::REF_VHIGH operand value contains full operand
                                  ///< (not only the high bits) Meaningful if \ph{high_fixup_bits}
#define PR_USE_ARG_TYPES 0x200000 ///< use \ph{use_arg_types3} callback
#define PR_SCALE_STKVARS 0x400000 ///< use \ph{get_stkvar_scale} callback
#define PR_DELAYED    0x800000    ///< has delayed jumps and calls
                                  ///< if this flag is set, \ph{is_basic_block_end}, \ph{has_delay_slot}
                                  ///< should be implemented
#define PR_ALIGN_INSN 0x1000000   ///< allow ida to create alignment instructions arbitrarily.
                                  ///< Since these instructions might lead to other wrong instructions
                                  ///< and spoil the listing, IDA does not create them by default anymore
#define PR_PURGING    0x2000000   ///< there are calling conventions which may purge bytes from the stack
#define PR_CNDINSNS   0x4000000   ///< has conditional instructions
#define PR_USE_TBYTE  0x8000000   ///< ::BTMT_SPECFLT means _TBYTE type
#define PR_DEFSEG64  0x10000000   ///< segments are 64-bit by default
#define PR_TINFO     0x20000000   ///< has support for ::tinfo_t
#define PR_OUTER     0x40000000   ///< has outer operands (currently only mc68k)
//@}
  bool has_segregs(void) const  { return (flag & PR_SEGS)     != 0; }          ///< #PR_SEGS
  bool use32(void) const        { return (flag & (PR_USE64|PR_USE32)) != 0; }  ///< #PR_USE64 or #PR_USE32
  bool use64(void) const        { return (flag & PR_USE64)    != 0; }          ///< #PR_USE64
  bool ti(void) const           { return (flag & PR_TYPEINFO) != 0; }          ///< #PR_TYPEINFO
  bool ti2(void) const          { return (flag & PR_TINFO) != 0; }             ///< #PR_TINFO
  bool stkup(void) const        { return (flag & PR_STACK_UP) != 0; }          ///< #PR_STACK_UP
  bool use_tbyte(void) const    { return (flag & PR_USE_TBYTE) != 0; }         ///< #PR_USE_TBYTE


  /// Get segment bitness
  /// \retval 2  #PR_DEFSEG64
  /// \retval 1  #PR_DEFSEG32
  /// \retval 0  none specified

  int get_segm_bitness(void) const { return (flag & PR_DEFSEG64) != 0 ? 2 : (flag & PR_DEFSEG32) != 0; }

  int cnbits;                     ///< Number of bits in a byte
                                  ///< for code segments (usually 8).
                                  ///< IDA supports values up to 32 bits
  int dnbits;                     ///< Number of bits in a byte
                                  ///< for non-code segments (usually 8).
                                  ///< IDA supports values up to 32 bits

  /// \name Byte size
  /// Number of 8bit bytes required to hold one byte of the target processor.
  //@{
  int cbsize(void) { return (cnbits+7)/8; }  ///< for code segments
  int dbsize(void) { return (dnbits+7)/8; }  ///< for non-code segments
  //@}

  /// \name Names
  /// IDP module may support several compatible processors.
  /// The following arrays define processor names:
  //@{
  const char *const *psnames;     ///< short processor names (NULL terminated).
                                  ///< Each name should be shorter than 9 characters
  const char *const *plnames;     ///< long processor names (NULL terminated).
                                  ///< No restriction on name lengths.
  //@}

  const asm_t *const *assemblers; ///< pointer to array of target
                                  ///< assembler definitions. You may
                                  ///< change this array when current
                                  ///< processor is changed.
                                  ///< (NULL terminated)

/// Custom instruction codes defined by processor extension plugins
/// must be greater than or equal to this
#define CUSTOM_CMD_ITYPE 0x8000


/// processor_t::use_regarg_type3 uses this bit in the return value
/// to indicate that the register value has been spoiled
#define REG_SPOIL 0x80000000L

  typedef const regval_t & (idaapi *regval_getter_t) (const char *name, const regval_t *regvalues);

  //<hookgen IDP>

  /// Callback notification codes.
  ///
  /// These are passed to notify() when certain events occur in the kernel,
  /// allowing the processor module to take appropriate action.
  ///
  /// If you are not developing a processor module, many of these
  /// codes already have a corresponding function to use instead
  /// (\idpcode{is_call_insn} vs is_call_insn(ea_t), for example).
  ///
  /// If you are developing a processor module, your notify() function
  /// must implement the desired behavior when called with a given code.
  enum idp_notify
  {
        init,                   ///< The IDP module is just loaded.
                                ///< \param idp_modname  (const char *) processor module name
                                ///< \return 0 on failure

        term,                   ///< The IDP module is being unloaded

        newprc,                 ///< Before changing processor type.
                                ///< \param pnum  (int) processor number in the array of processor names
                                ///< \retval 1  ok
                                ///< \retval 0  prohibit

        newasm,                 ///< Before setting a new assembler.
                                ///< \param asmnum  (int)

        newfile,                ///< A new file is loaded (already).
                                ///< \param fname  (char *) input file name

        oldfile,                ///< An old file is loaded (already).
                                ///< \param fname  (char *) input file name

        newbinary,              ///< Before loading a binary file.
                                ///< \param filename  (char *)   binary file name
                                ///< \param fileoff   (::uint32) offset in the file
                                ///< \param basepara  (::ea_t)   base loading paragraph
                                ///< \param binoff    (::ea_t)   loader offset
                                ///< \param nbytes    (::uint32) number of bytes to load

        endbinary,              ///< After loading a binary file.
                                ///< \param ok  (bool) file loaded successfully?

        newseg,                 ///< A new segment is about to be created.
                                ///< \param seg  (::segment_t *)
                                ///< \retval 1  ok
                                ///< \retval <=0  segment should not be created

        assemble,               ///< Assemble an instruction.
                                ///< (display a warning if an error is found).
                                ///< \param ea     (::ea_t) linear address of instruction
                                ///< \param cs     (::ea_t) cs of instruction
                                ///< \param ip     (::ea_t) ip of instruction
                                ///< \param use32  (bool) is 32bit segment?
                                ///< \param line   (const char *) line to assemble
                                ///< \param bin    (::uchar *) pointer to output opcode buffer
                                ///< \return size of the instruction in bytes

        obsolete_makemicro,     ///< Generate microcode for the instruction in 'cmd' structure.
                                ///< \param block  (mblock_t *)
                                ///< \return MICRO_... error codes

        outlabel,               ///< The kernel is going to generate an instruction
                                ///< label line or a function header.
                                ///< \param ea            (::ea_t)
                                ///< \param colored_name  (const char *)
                                ///< \return <=0 if the kernel should not generate the label

        rename,                 ///< The kernel is going to rename a byte.
                                ///< \param ea       (::ea_t)
                                ///< \param new_name (const char *)
                                ///< \param flags    (int) \ref SN_
                                ///< \return <=0 if the kernel should not rename it.
                                ///< \return 2 to inhibit the notification. I.e.,
                                ///<           the kernel should not rename, but
                                ///<           'set_name()' should return 'true'.
                                ///<         also see \idpcode{renamed}

        may_show_sreg,          ///< The kernel wants to display the segment registers
                                ///< in the messages window.
                                ///< \param current_ea  (::ea_t)
                                ///< \return <=0 if the kernel should not show the segment registers.
                                ///< (assuming that the module has done it)

        closebase,              ///< The database will be closed now

        load_idasgn,            ///< FLIRT signature has been loaded
                                ///< for normal processing (not for
                                ///< recognition of startup sequences).
                                ///< \param short_sig_name  (const char *)
                                ///< \return void

        coagulate,              ///< Try to define some unexplored bytes.
                                ///< This notification will be called if the
                                ///< kernel tried all possibilities and could
                                ///< not find anything more useful than to
                                ///< convert to array of bytes.
                                ///< The module can help the kernel and convert
                                ///< the bytes into something more useful.
                                ///< \param start_ea  (::ea_t)
                                ///< \return number of converted bytes + 1

        auto_empty,             ///< Info: all analysis queues are empty.
                                ///< This callback is called once when the
                                ///< initial analysis is finished. If the queue is
                                ///< not empty upon the return from this callback,
                                ///< it will be called later again.
                                ///< See also \idpcode{auto_empty_finally}.
                                ///< \param none
                                ///< \return void

        auto_queue_empty,       ///< One analysis queue is empty.
                                ///< This callback can be called many times, so
                                ///< only the autoMark() functions can be used from it
                                ///< (other functions may work but it is not tested).
                                ///< \param type  (::atype_t)
                                ///< \retval 1    yes, keep the queue empty (default)
                                ///< \retval <=0  no, the queue is not empty anymore

        func_bounds,            ///< find_func_bounds() finished its work.
                                ///< The module may fine tune the function bounds
                                ///< \param possible_return_code  (int *)
                                ///< \param pfn                   (::func_t *)
                                ///< \param max_func_end_ea       (::ea_t) (from the kernel's point of view)
                                ///< \return void

        may_be_func,            ///< Can a function start here?.
                                ///< the instruction is in 'cmd',
                                ///< the idp module is allowed to modify 'cmd'
                                ///< \param state (int)  autoanalysis phase
                                ///<   0: creating functions
                                ///<   1: creating chunks
                                ///< \return probability 0..100

        is_sane_insn,           ///< Is the instruction sane for the current file type?.
                                ///< The instruction is in 'cmd'.
                                ///< \param no_crefs  (int)
                                ///<   1: the instruction has no code refs to it.
                                ///<      ida just tries to convert unexplored bytes
                                ///<      to an instruction (but there is no other
                                ///<      reason to convert them into an instruction)
                                ///<   0: the instruction is created because
                                ///<      of some coderef, user request or another
                                ///<      weighty reason.
                                ///< \retval 1    ok
                                ///< \retval <=0  no, the instruction isn't
                                ///<              likely to appear in the program

        is_jump_func,           ///< Is the function a trivial "jump" function?.
                                ///< \param pfn           (::func_t *)
                                ///< \param jump_target   (::ea_t *)
                                ///< \param func_pointer  (::ea_t *)
                                ///< \retval <=0  no
                                ///< \retval 1  don't know
                                ///< \retval 2  yes, see 'jump_target' and 'func_pointer'

        gen_regvar_def,         ///< Generate register variable definition line.
                                ///< \param v  (::regvar_t *)
                                ///< \retval <=0  ok, generated the definition text

        setsgr,                 ///< The kernel has changed a segment register value.
                                ///< \param startEA    (::ea_t)
                                ///< \param endEA      (::ea_t)
                                ///< \param regnum     (int)
                                ///< \param value      (::sel_t)
                                ///< \param old_value  (::sel_t)
                                ///< \param tag        (::uchar) \ref SR_
                                ///< \return success

        set_compiler,           ///< The kernel has changed the compiler information.
                                ///< (\inf{cc} structure; \inf{abiname})

        is_basic_block_end,     ///< Is the current instruction end of a basic block?.
                                ///< This function should be defined for processors
                                ///< with delayed jump slots. The current instruction
                                ///< is stored in ::cmd
                                ///< \param call_insn_stops_block  (bool)
                                ///< \retval 1  unknown
                                ///< \retval <=0  no
                                ///< \retval 2  yes

        reglink,                ///< IBM PC only, ignore it

        get_vxd_name,           ///< IBM PC only, ignore it.
                                ///< Get Vxd function name
                                ///< \param vxdnum   (int)
                                ///< \param funcnum  (int)
                                ///< \param outbuf   (char *)
                                ///< \return void

        // PROCESSOR EXTENSION NOTIFICATIONS - TODO: get this in the doxygen output
        // They are used to add support of new instructions
        // to the existing processor modules.
        // They should be processed only in notification callbacks
        // set by hook_to_notification_point(HK_IDP,...)

        custom_ana,             ///< \param none the address to analyze is in \cmd{ea}.
                                ///<   - \cmd{ip} and \cmd{cs} are initialized too
                                ///<   - \cmd{itype} must be set >= 0x8000
                                ///<   - \cmd{size} must be set to the instruction length
                                ///<
                                ///< (a good plugin would fill the whole ::cmd including the operand fields)
                                ///< in the case of error the cmd structure should be kept intact
                                ///< \return 1 + \cmd{size}

        custom_out,             ///< Generate instruction text using the printf_line() function.
                                ///< Optional notification (depends on the processor module)
                                ///< \param none (::cmd structure contains information about the instruction)
                                ///< \return 2

        custom_emu,             ///< \param none (cmd structure contains information about the instruction)
                                ///<
                                ///< Optional notification. if absent,
                                ///< the instruction is supposed to be an regular one
                                ///< the kernel will proceed to the analysis of the next instruction
                                ///< \return 2

        custom_outop,           ///< \param op  (::op_t *)
                                ///
                                ///< Optional notification to generate operand text. if absent,
                                ///<   the standard operand output function will be called.
                                ///<   the output buffer is inited with init_output_buffer()
                                ///<   and this notification may use out_...() functions from ua.hpp
                                ///<   to form the operand text
                                ///< \return 2

        custom_mnem,            ///< \param buf     (char *)
                                ///< \param bufsize (size_t)
                                ///<
                                ///< ::cmd structure contains information about the instruction.
                                ///< Optional notification. if absent,
                                ///<   the IDC function GetMnem() won't work.
                                ///< At least one of \idpcode{custom_out} or \idpcode{custom_mnem}
                                ///< should be implemented. \idpcode{custom_ana} should always be
                                ///< implemented. These custom_... callbacks will be
                                ///< called for all instructions. It is the responsibility
                                ///< of the plugin to ignore the undesired callbacks
                                ///< \return 2

        // END OF PROCESSOR EXTENSION NOTIFICATIONS

        undefine,               ///< An item in the database (insn or data) is being deleted.
                                ///< \param ea  (ea_t)
                                ///< \return >0-ok, <=0-the kernel should stop.
                                ///< if the return value is positive:
                                ///< - bit0  ignored
                                ///< - bit1  do not delete srareas at the item end

        make_code,              ///< An instruction is being created.
                                ///< \param ea    (::ea_t)
                                ///< \param size  (::asize_t)
                                ///< \retval 1    ok
                                ///< \retval <=0  the kernel should stop

        make_data,              ///< A data item is being created.
                                ///< \param ea     (::ea_t)
                                ///< \param flags  (::flags_t)
                                ///< \param tid    (::tid_t)
                                ///< \param len    (::asize_t)
                                ///< \retval 1    ok
                                ///< \retval <=0  the kernel should stop

        moving_segm,            ///< May the kernel move the segment?
                                ///< \param seg    (::segment_t *) segment to move
                                ///< \param to     (::ea_t) new segment start address
                                ///< \param flags  (int) combination of \ref MSF_
                                ///< \retval 1    yes
                                ///< \retval <=0  the kernel should stop

        move_segm,              ///< A segment is moved.
                                ///< Fix processor dependent address sensitive information
                                ///< \param from  (::ea_t) old segment address
                                ///< \param seg   (::segment_t *) moved segment
                                ///< \return void

        is_call_insn,           ///< Is the instruction a "call"?
                                ///< \param ea  (::ea_t) instruction address
                                ///< \retval 1   unknown
                                ///< \retval <=0 no
                                ///< \retval 2   yes

        is_ret_insn,            ///< Is the instruction a "return"?
                                ///< \param ea      (::ea_t) instruction address
                                ///< \param strict  (bool)
                                ///<          1: report only ret instructions
                                ///<          0: include instructions like "leave"
                                ///<             which begins the function epilog
                                ///< \retval 1   unknown
                                ///< \retval <=0 no
                                ///< \retval 2   yes

        get_stkvar_scale_factor,///< Should stack variable references be multiplied by
                                ///< a coefficient before being used in the stack frame?.
                                ///< Currently used by TMS320C55 because the references into
                                ///< the stack should be multiplied by 2
                                ///< \note #PR_SCALE_STKVARS should be set to use this callback
                                ///< \return scaling factor

        create_flat_group,      ///< Create special segment representing the flat group.
                                ///< (to use for PC mainly)
                                ///< \param image_base  (::ea_t)
                                ///< \param bitness     (int)
                                ///< \param dataseg_sel (::sel_t)

        kernel_config_loaded,   ///< This callback is called when ida.cfg is parsed.
                                ///< \param none
                                ///< \return void

        might_change_sp,        ///< Does the instruction at 'ea' modify the stack pointer?
                                ///< (not used yet).
                                ///< \param ea  (::ea_t)
                                ///< \retval 1   yes
                                ///< \retval <=0 no

        is_alloca_probe,        ///< Does the function at 'ea' behave as __alloca_probe?
                                ///< \param ea  (::ea_t)
                                ///< \retval 2  yes
                                ///< \retval 1  no

        out_3byte,              ///< Generate text representation of 3byte data.
                                ///< init_output_buffer() is called before this function
                                ///< and all Out... function can be used.
                                ///< ::uFlag contains the flags.
                                ///< This callback might be implemented by the processor
                                ///< module to generate custom representation of 3byte data.
                                ///< \param dataea       (::ea_t) address of the data item
                                ///< \param value        (::uint32) value value to output
                                ///< \param analyze_only (bool) only create xrefs if necessary.
                                ///<                            do not generate text representation
                                ///< \retval 2  success
                                ///< \retval 1  failed

        get_reg_name,           ///< Generate text representation of a register.
                                ///< Most processor modules do not need to implement this callback.
                                ///< It is useful only if \ph{regNames}[reg] does not provide
                                ///< the correct register name.
                                ///< \param reg     (int) internal register number as defined in the processor module
                                ///< \param width   (size_t) register width in bytes
                                ///< \param buf     (char *) output buffer
                                ///< \param bufsize (size_t) size of output buffer
                                ///< \param reghi   (int) if not -1 then this function will return the register pair
                                ///< \return -1 if error, strlen(buf)+2 otherwise

        savebase,               ///< The database is being saved. Processor module should
                                ///< save its local data

        gen_asm_or_lst,         ///< Callback: generating asm or lst file.
                                ///< The kernel calls this callback twice, at the beginning
                                ///< and at the end of listing generation. The processor
                                ///< module can intercept this event and adjust its output
                                ///< \param starting  (bool) beginning listing generation
                                ///< \param fp        (FILE *) output file
                                ///< \param is_asm    (bool) true:assembler, false:listing
                                ///< \param flags     (int) flags passed to gen_file()
                                ///< \param outline   (gen_outline_t **) ptr to ptr to outline callback.
                                ///<                  if this callback is defined for this code, it will be
                                ///<                  used by the kernel to output the generated lines
                                ///< \return void

        out_src_file_lnnum,     ///< Callback: generate analog of:
                                ///<
                                ///< #line "file.c" 123
                                ///<
                                ///< directive.
                                ///< \param file   (const char *) source file (may be NULL)
                                ///< \param lnnum  (size_t) line number
                                ///< \retval 2 directive has been generated

        get_autocmt,            ///< Callback: get dynamic auto comment.
                                ///< Will be called if the autocomments are enabled
                                ///< and the comment retrieved from ida.int starts with
                                ///< '$!'. ::cmd contains valid info.
                                ///< \param buf     (char *) output buffer
                                ///< \param bufsize (size_t) output buffer size
                                ///< \retval 2  new comment has been generated
                                ///< \retval 1  callback has not been handled.
                                ///<            the buffer must not be changed in this case

        is_insn_table_jump,     ///< Callback: determine if instruction is a table jump or call.
                                ///< If #CF_JUMP bit can not describe all kinds of table
                                ///< jumps, please define this callback.
                                ///< It will be called for insns with #CF_JUMP bit set.
                                ///< \param none  ::cmd structure contains the current instruction
                                ///< \retval 1    yes
                                ///< \retval <=0  no

        auto_empty_finally,     ///< Info: all analysis queues are empty definitively.
                                ///< This callback is called only once.
                                ///< See also \idpcode{auto_empty}.
                                ///< \param none
                                ///< \return void

        loader_finished,        ///< Event: external file loader finished its work.
                                ///< Use this event to augment the existing loader functionality.
                                ///< \param li            (linput_t *)
                                ///< \param neflags       (::uint16) \ref NEF_
                                ///< \param filetypename  (const char *)

        loader_elf_machine,     ///< Event: ELF loader machine type checkpoint.
                                ///< A plugin check of the 'machine_type'. If it is the desired one,
                                ///< the the plugin fills 'p_procname' with the processor name
                                ///< (one of the names present in \ph{psnames}).
                                ///< 'p_pd' is used to handle relocations, otherwise can be left untouched.
                                ///< 'set_reloc' can be later used by the plugin to specify relocations.
                                ///< This event occurs for each newly loaded ELF file
                                ///< \param li            (linput_t *)
                                ///< \param machine_type  (int)
                                ///< \param p_procname    (const char **)
                                ///< \param p_pd          (proc_def **) (see ldr\elf.h)
                                ///< \param set_reloc     (set_elf_reloc_t *)
                                ///< \return  e_machine value (if it is different from the
                                ///<          original e_machine value, procname and 'p_pd' will be ignored
                                ///<          and the new value will be used)

        is_indirect_jump,       ///< Callback: determine if instruction is an indirect jump.
                                ///< If #CF_JUMP bit can not describe all jump types
                                ///< jumps, please define this callback.
                                ///< \param none  ::cmd structure contains the current instruction
                                ///< \retval 1  use #CF_JUMP
                                ///< \retval 2  no
                                ///< \retval 3  yes

        verify_noreturn,        ///< The kernel wants to set 'noreturn' flags for a function.
                                ///< \param pfn  (::func_t *)
                                ///< \return 1: ok. any other value: do not set 'noreturn' flag

        verify_sp,              ///< All function instructions have been analyzed.
                                ///< Now the processor module can analyze the stack pointer
                                ///< for the whole function
                                ///< \param pfn  (::func_t *)
                                ///< \retval 1   ok
                                ///< \retval <=0 bad stack pointer

        renamed,                ///< The kernel has renamed a byte.
                                ///< See also the \idpcode{rename} event
                                ///< \param ea          (::ea_t)
                                ///< \param new_name    (const char *)
                                ///< \param local_name  (bool)
                                ///< \return void

        add_func,               ///< The kernel has added a function.
                                ///< \param pfn  (::func_t *)
                                ///< \return void

        del_func,               ///< The kernel is about to delete a function.
                                ///< \param pfn  (::func_t *)
                                ///< \retval 1    ok
                                ///< \retval <=0  do not delete

        set_func_start,         ///< Function chunk start address will be changed.
                                ///< \param pfn        (::func_t *)
                                ///< \param new_start  (::ea_t)
                                ///< \retval 1    ok
                                ///< \retval <=0  do not change

        set_func_end,           ///< Function chunk end address will be changed.
                                ///< \param pfn      (::func_t *)
                                ///< \param new_end  (::ea_t)
                                ///< \retval 1    ok
                                ///< \retval <=0  do not change

        treat_hindering_item,   ///< An item hinders creation of another item.
                                ///< \param hindering_item_ea  (::ea_t)
                                ///< \param new_item_flags     (::flags_t)  (0 for code)
                                ///< \param new_item_ea        (::ea_t)
                                ///< \param new_item_length    (::asize_t)
                                ///< \retval 1    no reaction
                                ///< \retval <=0  the kernel may delete the hindering item

        str2reg,                ///< Convert a register name to a register number.
                                ///< The register number is the register index in the \ph{regNames} array
                                ///< Most processor modules do not need to implement this callback
                                ///< It is useful only if \ph{regNames}[reg] does not provide
                                ///< the correct register names
                                ///< \param regname  (const char *)
                                ///< \return register number + 2

        create_switch_xrefs,    ///< Create xrefs for a custom jump table.
                                ///< \param jumpea   (::ea_t) address of the jump insn
                                ///< \param si       (::switch_info_ex_t *) switch information
                                ///< \return must return 2

        calc_switch_cases,      ///< Calculate case values and targets for a custom jump table.
                                ///< \param insn_ea  (::ea_t) address of the 'indirect jump' instruction
                                ///< \param si       (::switch_info_ex_t *) switch information
                                ///< \param casevec  (::casevec_t *) vector of case values (may be NULL)
                                ///< \param targets  (::eavec_t *) corresponding target addresses (my be NULL)
                                ///< \retval 2   ok
                                ///< \retval 1  failed

        determined_main,        ///< The main() function has been determined.
                                ///< \param main (::ea_t) address of the main() function
                                ///< \return void

        preprocess_chart,       ///< Gui has retrieved a function flow chart.
                                ///< Plugins may modify the flow chart in this callback.
                                ///< \param fc  (qflow_chart_t *)
                                ///< \return void

        get_bg_color,           ///< Get item background color.
                                ///< Plugins can hook this callback to color disassembly lines dynamically
                                ///< \param ea     (::ea_t)
                                ///< \param color  (::bgcolor_t)
                                ///< \retval 1  not implemented
                                ///< \retval 2  color set

        validate_flirt_func,    ///< Flirt has recognized a library function.
                                ///< This callback can be used by a plugin or proc module
                                ///< to intercept it and validate such a function.
                                ///< The idp module is allowed to modify ::cmd
                                ///< \param start_ea  (::ea_t)
                                ///< \param funcname  (const char *)
                                ///< \retval -1  do not create a function,
                                ///< \retval  1  function is validated

        get_operand_string,     ///< Request text string for operand (cli, java, ...).
                                ///< (::cmd structure must contain info for the desired insn)
                                ///< \param opnum  (int) operand number, -1 means any string operand
                                ///< \param buf    (char *)
                                ///< \param buflen (size_t)
                                ///< \return  1  no string (or empty string)
                                ///<         >1  original string length with terminating zero

        // the following 5 events are very low level
        // take care of possible recursion
        add_cref,               ///< A code reference is being created.
                                ///< \param from  (::ea_t)
                                ///< \param to    (::ea_t)
                                ///< \param type  (::cref_t)
                                ///< \return < 0 - cancel cref creation
        add_dref,               ///< A data reference is being created.
                                ///< \param from  (::ea_t)
                                ///< \param to    (::ea_t)
                                ///< \param type  (::dref_t)
                                ///< \return < 0 - cancel dref creation
        del_cref,               ///< A code reference is being deleted.
                                ///< \param from    (::ea_t)
                                ///< \param to      (::ea_t)
                                ///< \param expand  (bool)
                                ///< \return < 0 - cancel cref deletion
        del_dref,               ///< A data reference is being deleted.
                                ///< \param from    (::ea_t)
                                ///< \param to      (::ea_t)
                                ///< \return < 0 - cancel dref deletion
        coagulate_dref,         ///< Data reference is being analyzed.
                                ///< plugin may correct 'code_ea' (e.g. for thumb mode refs, we clear the last bit)
                                ///< \param from        (::ea_t)
                                ///< \param to          (::ea_t)
                                ///< \param may_define  (bool)
                                ///< \param code_ea     (::ea_t *)
                                ///< \return < 0 - cancel dref analysis

        register_custom_fixup,  ///< Request to register a new custom fixup type.
                                ///< \param name  (const char *)
                                ///< \return fixup id + 1

        custom_refinfo,         ///< Called from get_offset_expr, when ::refinfo_t
                                ///< contains flag #REFINFO_CUSTOM. Normally this
                                ///< notification used in a combination with custom_fixup
                                ///< \param ea          (::ea_t)
                                ///< \param numop       (int)
                                ///< \param opval       (::ea_t *)
                                ///< \param ri          (const ::refinfo_t*)
                                ///< \param buf         (char *)
                                ///< \param bufsize     (size_t)
                                ///< \param target      (::ea_t *)
                                ///< \param fullvalue   (::ea_t *)
                                ///< \param from        (::ea_t)
                                ///< \param getn_flags  (int)
                                ///< \retval 2      buf filled as simple expression
                                ///< \retval 3      buf filled as complex expression
                                ///< \retval 4      apply standard processing (with possible changed values)
                                ///< \retval other  can't convert to offset expression

        set_proc_options,       ///< Called if the user specified an option string in the command line:
                                ///<  -p<processor name>:<options>.
                                ///< Can be used for setting a processor subtype.
                                ///< Also called if option string is passed to set_processor_type()
                                ///< and IDC's SetProcessorType().
                                ///< \param options     (const char *)
                                ///< \param confidence  (int)
                                ///<          0: loader's suggestion
                                ///<          1: user's decision
                                ///< \return < 0 if bad option string

        adjust_libfunc_ea,      ///< Called when a signature module has been matched against
                                ///< bytes in the database. This is used to compute the
                                ///< offset at which a particular module's libfunc should
                                ///< be applied.
                                ///< \param sig     (const idasgn_t *)
                                ///< \param libfun  (const libfunc_t *)
                                ///< \param ea      (::ea_t *) \note 'ea' initially contains the ea_t of the
                                ///<                                 start of the pattern match
                                ///< \retval 2      the ea_t pointed to by the third argument was modified.
                                ///< \retval other  not modified. use default algorithm.

        extlang_changed,        ///< The list of extlangs or the default extlang was changed.
                                ///< \param kind  (int)
                                ///<          0: extlang installed
                                ///<          1: extlang removed
                                ///<          2: default extlang changed
                                ///< \param el (const ::extlang_t *) pointer to the extlang affected
                                ///< \return void

        delay_slot_insn,        ///< Get delay slot instruction
                                ///< \param ea    (::ea_t *) instruction address in question,
                                ///<                         if answer is positive then set 'ea' to
                                ///<                         the delay slot insn address
                                ///< \param bexec (bool *)   execute slot if jumping,
                                ///<                         initially set to 'true'
                                ///< \param fexec (bool *)   execute slot if not jumping,
                                ///<                         initally set to 'true'
                                ///< \retval 2  positive answer
                                ///< \retval <2 ordinal insn
                                ///< \note Input 'ea' may point to the instruction with a delay slot or
                                ///<       to the delay slot instruction itself.

        adjust_refinfo,         ///< Called from apply_fixup before converting operand to reference.
                                ///< Can be used for changing the reference info.
                                ///< \param ea      (::ea_t) instruction address
                                ///< \param n       (int) operand number
                                ///< \param fd      (const fixup_data_t *)
                                ///< \param ri      (refinfo_t *)
                                ///< \return < 0 - do not create an offset

        last_cb_before_debugger,///< START OF DEBUGGER CALLBACKS

        obsolete_get_operand_info = 100,
                                ///< Get operand information.
                                ///< same as \idpcode{get_operand_info} (below), but uses
                                ///< idd_opinfo_old_t* as the last argument (less info)

        OBSOLETE(get_reg_info), // the same as get_reg_info2 but the
                                // position and size of the subvalue is defined
                                // by uint64 (not bitrange_t) and
                                // the 3d argument is uint64 *mask (0-no mask)
                                // example: "ah" returns main_regname="eax" and mask=0xFF00
#ifdef NO_OBSOLETE_FUNCS
        next_exec_insn,         ///< Get next address to be executed
#else                           ///<
        get_jump_target,        ///< Get jump target (old name)
#endif                          ///<
                                ///< This function must return the next address to be executed.
                                ///< If the instruction following the current one is executed, then it must return #BADADDR
                                ///< Usually the instructions to consider are: jumps, branches, calls, returns.
                                ///< This function is essential if the 'single step' is not supported in hardware.
                                ///< \param ea         (::ea_t) instruction address
                                ///< \param tid        (int) current therad id
                                ///< \param getreg     (::processor_t::regval_getter_t) function to get register values
                                ///< \param regvalues  (const ::regval_t *) register values array
                                ///< \param target     (::ea_t *) pointer to the answer
                                ///< \retval 1   unimplemented
                                ///< \retval <=0 implemented

        calc_step_over,         ///< Calculate the address of the instruction which will be
                                ///< executed after "step over". The kernel will put a breakpoint there.
                                ///< If the step over is equal to step into or we can not calculate
                                ///< the address, return #BADADDR.
                                ///< \param ip      (::ea_t) instruction address
                                ///< \param target  (::ea_t *) pointer to the answer
                                ///< \retval 1   unimplemented
                                ///< \retval <=0 implemented

        get_macro_insn_head,    ///< Calculate the start of a macro instruction.
                                ///< This notification is called if IP points to the middle of an instruction
                                ///< \param ip    (::ea_t) instruction address
                                ///< \param head  (::ea_t *) answer, #BADADDR means normal instruction
                                ///< \retval 1   unimplemented
                                ///< \retval <=0 implemented

        get_dbr_opnum,          ///< Get the number of the operand to be displayed in the
                                ///< debugger reference view (text mode).
                                ///< \param ea     (::ea_t) instruction address
                                ///< \param opnum  (int *) operand number (out, -1 means no such operand)
                                ///< \retval 1   unimplemented
                                ///< \retval <=0 implemented

        insn_reads_tbit,        ///< Check if insn will read the TF bit.
                                ///< \param ea         (::ea_t) instruction address
                                ///< \param getreg     (::processor_t::regval_getter_t) function to get register values
                                ///< \param regvalues  (const ::regval_t *) register values array
                                ///< \retval 3  yes, will generate 'step' exception
                                ///< \retval 2  yes, will store the TF bit in memory
                                ///< \retval 1  no

        get_operand_info,       ///< Get operand information.
                                ///< This callback is used to calculate the operand
                                ///< value for double clicking on it, hints, etc.
                                ///< \param ea         (::ea_t) instruction address
                                ///< \param n          (int) operand number
                                ///< \param thread_id  (int) current thread id
                                ///< \param getreg     (::processor_t::regval_getter_t) function to get register values
                                ///< \param regvalues  (const ::regval_t *) register values array
                                ///< \param opinf      (::idd_opinfo_t *) the output buffer
                                ///< \return <=0-ok, otherwise failed

        calc_next_eas,          ///< Calculate list of addresses the instruction in ::cmd
                                ///< may pass control to.
                                ///< This callback is required for source level debugging.
                                ///< \param over      (bool) calculate for step over (ignore call targets)
                                ///< \param res       (::ea_t *) array for the results.
                                ///<                  this array has #NEXTEAS_ANSWER_SIZE elements.
                                ///< \param nsubcalls (int *) number of addresses of called functions
                                ///<                  in the above array. they must be put
                                ///<                  at the beginning of the array.
                                ///<                  if over=true, this answer will be zero.
                                ///< \return number of calculated addresses+1.
                                ///< If there are too many addresses or they are
                                ///< incalculable (indirect jumps, for example), return -1.

        clean_tbit,             ///< Clear the TF bit after an insn like pushf stored it in memory.
                                ///< \param ea  (::ea_t) instruction address
                                ///< \param getreg     (::processor_t::regval_getter_t) function to get register values
                                ///< \param regvalues  (const ::regval_t *) register values array
                                ///< \retval 2  ok
                                ///< \retval 1  failed

        get_reg_info2,          ///< Get register information by its name.
                                ///< example: "ah" returns:
                                ///<   - main_regname="eax"
                                ///<   - bitrange_t = { offset==8, nbits==8 }
                                ///<
                                ///< This callback may be unimplemented if the register
                                ///< names are all present in \ph{regNames} and they all have
                                ///< the same size
                                ///< \param regname       (const char *)
                                ///< \param main_regname  (const char **) (NULL-failed)
                                ///< \param bitrange      (::bitrange_t *) position and size of the value within 'main_regname' (empty bitrange == whole register)
                                ///< \retval 1  unimplemented
                                ///< \retval 2  implemented

        // END OF DEBUGGER CALLBACKS

        // START OF TYPEINFO CALLBACKS TODO: get this into doxygen output
        // The codes below will be called only if #PR_TYPEINFO is set.
        // The codes based_ptr, max_ptr_size, get_default_enum_size MUST be implemented.
        // (other codes are optional but still require for normal
        // operation of the type system. without calc_arglocs3,
        // for example, ida will not know about the argument
        // locations for function calls.

        last_cb_before_type_callbacks,
        OBSOLETE(decorate_name)=500,
                                // Decorate/undecorate a C symbol name
                                // const til_t *ti    - pointer to til
                                // const char *name   - name of symbol
                                // const type_t *type - type of symbol. If NULL then it will try to guess.
                                // char *outbuf       - output buffer
                                // size_t bufsize     - size of the output buffer
                                // bool mangle        - true-mangle, false-unmangle
                                // cm_t cc            - real calling convention for VOIDARG functions
                                // returns: true if success

        setup_til,              ///< Setup default type libraries. (called after loading
                                ///< a new file into the database).
                                ///< The processor module may load tils, setup memory
                                ///< model and perform other actions required to set up
                                ///< the type system.
                                ///< This is an optional callback.
                                ///< \param none
                                ///< \return void

        based_ptr,              ///< Get prefix and size of 'segment based' ptr
                                ///< type. (something like char _ss *ptr,
                                ///< see based_ptr_name_and_size())
                                ///< \param ptrt     (unsigned)
                                ///< \param ptrname  (const char **)
                                ///< \return size of type

        max_ptr_size,           ///< Get maximal size of a pointer in bytes.
                                ///< \param none
                                ///< \return max possible size of a pointer plus 1

        get_default_enum_size,  ///< Get default enum size.
                                ///< \param cm  (::cm_t)
                                ///< \returns sizeof(enum)

        OBSOLETE(calc_arglocs),
        OBSOLETE(use_stkarg_type),
        OBSOLETE(use_regarg_type),
        OBSOLETE(use_arg_types),
        OBSOLETE(get_fastcall_regs),
        OBSOLETE(get_thiscall_regs),
        OBSOLETE(calc_cdecl_purged_bytes),
        OBSOLETE(get_stkarg_offset),
        OBSOLETE(calc_purged_bytes),
        OBSOLETE(calc_arglocs2),
        OBSOLETE(calc_retloc),
        OBSOLETE(calc_varglocs),
        OBSOLETE(get_varcall_regs),
        OBSOLETE(use_regarg_type2),
        OBSOLETE(use_arg_types2),
        OBSOLETE(get_fastcall_regs2),
        OBSOLETE(get_thiscall_regs2),
        OBSOLETE(get_varcall_regs2),

        calc_cdecl_purged_bytes2,
                                ///< Calculate number of purged bytes after call.
                                ///< param ea  (::ea_t) address of the call instruction
                                ///< \returns number of purged bytes+2 (usually add sp, N)

        get_stkarg_offset2,     ///< Get offset from SP to the first stack argument.
                                ///< For example: pc: 0, hppa: -0x34, ppc: 0x38
                                ///< \param none
                                ///< \returns the offset + 2

        til_for_file,           ///< Internal notification, do not use

        equal_reglocs,          ///< Are 2 register arglocs the same?.
                                ///< We need this callback for the pc module.
                                ///< \param a1  (::argloc_t *)
                                ///< \param a2  (::argloc_t *)
                                ///< \retval  1  not implemented
                                ///< \retval  2  yes
                                ///< \retval -1  no

        decorate_name3,         ///< Decorate/undecorate a C symbol name.
                                ///< The kernel uses this callback only if #PR_TINFO is set.
                                ///< \param outbuf  (::qstring *) output buffer
                                ///< \param name    (const char *) name of symbol
                                ///< \param mangle  (bool) true-mangle, false-unmangle
                                ///< \param cc      (::cm_t) calling convention
                                ///< \returns 2 if success

        calc_retloc3,           ///< Calculate return value location.
                                ///< The kernel uses this callback only if #PR_TINFO is set.
                                ///< \param rettype      (const tinfo_t *)
                                ///< \param cc           (::cm_t)
                                ///< \param[out] retloc  (::argloc_t *)
                                ///< \return  1  not implemented
                                ///< \return  2  ok,
                                ///< \return -1  error

        calc_varglocs3,         ///< Calculate locations of the arguments that correspond to '...'.
                                ///< The kernel uses this callback only if #PR_TINFO is set.
                                ///< \param ftd      (const ::func_type_data_t *) info about all arguments (including varargs)
                                ///< \param regs     (::regobjs_t *) buffer for register values
                                ///< \param stkargs  (::relobj_t *) stack arguments
                                ///< \param nfixed   (int) number of fixed arguments
                                ///< \retval  1  not implemented
                                ///< \retval  2  ok
                                ///< \retval -1  error

        calc_arglocs3,          ///< Calculate function argument locations.
                                ///< This callback should fill retloc, all arglocs, and stkargs.
                                ///< This callback supersedes calc_argloc2.
                                ///< This callback is never called for ::CM_CC_SPECIAL functions.
                                ///< The kernel uses this callback only if #PR_TINFO is set.
                                ///< \param fti  (::func_type_data_t *) points to the func type info
                                ///< \retval  1  not implemented
                                ///< \retval  2  ok
                                ///< \retval -1  error

        use_stkarg_type3,       ///< Use information about a stack argument.
                                ///< The kernel uses this callback only if #PR_TINFO is set.
                                ///< \param ea  (::ea_t) address of the push instruction which
                                ///<                     pushes the function argument into the stack
                                ///< \param arg  (const ::funcarg_t *) argument info
                                ///< \retval true   ok
                                ///< \retval false  failed, the kernel will create a comment with the
                                ///<                argument name or type for the instruction

        use_regarg_type3,       ///< Use information about register argument.
                                ///< The kernel uses this callback only if #PR_TINFO is set.
                                ///< \param[out] idx (int *) pointer to the returned value, may contain:
                                ///<                         - idx of the used argument, if the argument is defined
                                ///<                           in the current instruction, a comment will be applied by the kernel
                                ///<                         - idx | #REG_SPOIL - argument is spoiled by the instruction
                                ///<                         - -1 if the instruction doesn't change any registers
                                ///<                         - -2 if the instruction spoils all registers
                                ///< \param ea       (::ea_t) address of the instruction
                                ///< \param rargs    (const ::funcargvec_t *) vector of register arguments
                                ///<                               (including regs extracted from scattered arguments)
                                ///< \return 2

        use_arg_types3,         ///< Use information about callee arguments.
                                ///< The kernel uses this callback only if #PR_TINFO and #PR_USE_ARG_TYPES are set.
                                ///< \param ea     (::ea_t) address of the call instruction
                                ///< \param fti    (::func_type_data_t *) info about function type
                                ///< \param rargs  (::funcargvec_t *) array of register arguments
                                ///< \return 2 (and removes handled arguments from fti and rargs)

        calc_purged_bytes3,     ///< Calculate number of purged bytes by the given function type.
                                ///< The kernel uses this callback only if #PR_TINFO is set.
                                ///< \param[out] p_purged_bytes  (int *) ptr to output
                                ///< \param fti                  (const ::func_type_data_t *) func type details
                                ///< \return 2

        shadow_args_size,       ///< Get size of shadow args in bytes.
                                ///< \param[out] shadow_args_size  (int *)
                                ///< \param pfn                    (::func_t *) (may be NULL)
                                ///< \return 2 if filled *shadow_args_size

        get_varcall_regs3,      ///< Get register allocation convention used in the ellipsis (...) calling convention.
                                ///< \param regs  (::callregs_t *)
                                ///< \return max_possible_number_of_varcall_regs+2

        get_fastcall_regs3,     ///< Get register allocation convention used in the fastcall calling convention.
                                ///< \param regs  (::callregs_t *)
                                ///< \return max_possible_number_of_fastcall_regs+2

        get_thiscall_regs3,     ///< Get register allocation convention used in the thiscall calling convention.
                                ///< \param regs  (::callregs_t *)
                                ///< \return max_possible_number_of_fastcall_regs+2

        get_func_cvtarg_map,    ///< Get function arguments which should be converted to pointers lowering function prototype
                                ///< \param fti     (const ::func_type_data_t *) func type details
                                ///< \param argnums (intvec_t *) - number of arguments to be converted to pointers (special values -1/-2 for return value, position of hidden 'retstr' argument: -1 - at the beginning, -2 - at the end)
                                ///< \return 2

        get_simd_types,         ///< Get SIMD-related types according to given attributes ant/or argument location
                                ///< \param simd_attrs (const ::simd_info_t *)
                                ///< \param argloc (const ::argloc_t *)
                                ///< \param out (::simd_info_vec_t *)
                                ///< \param create_tifs (bool) return valid tinfo_t objects, create if neccessary
                                ///< \return number of found types+1, -1-error
                                ///< If name==NULL, initialize all SIMD types

        // END OF TYPEINFO CALLBACKS

        loader=1000,            ///< This code and higher ones are reserved
                                ///< for the loaders.
                                ///< The arguments and the return values are
                                ///< defined by the loaders
  };

  /// Various notifications for the idp
  int (idaapi *notify)(idp_notify msgid, ...);

  /// Get the stack variable scaling factor.
  /// Useful for processors who refer to the stack with implicit scaling factor.
  /// TMS320C55 for example: SP(#1) really refers to (SP+2)
  int get_stkvar_scale(void)
    { return (flag & PR_SCALE_STKVARS) ? notify(get_stkvar_scale_factor) : 1; }

  /// \name Disassembly sections
  /// The following functions generate portions of the disassembled text.
  //@{
  void  (idaapi *header)(void);        ///< Function to produce start of disassembled text
  void  (idaapi *footer)(void);        ///< Function to produce end of disassembled text

  void  (idaapi *segstart)(ea_t ea);   ///< Function to produce start of segment
  void  (idaapi *segend)  (ea_t ea);   ///< Function to produce end of segment

  void  (idaapi *assumes) (ea_t ea);   ///< Function to produce assume directives
                                       ///< when segment register value changes.
                                       ///< If your processor has no segment
                                       ///< registers, you may define it as NULL
  //@}

  /// Analyze one instruction and fill 'cmd' structure.
  /// This function shouldn't change the database, flags or anything else.
  /// All these actions should be performed only by u_emu() function.
  /// \cmd{ea} contains address of instruction to analyze.
  /// \return length of the instruction in bytes, 0 if instruction can't be decoded.
  int   (idaapi *u_ana)   (void);

  /// Emulate instruction, create cross-references, plan to analyze
  /// subsequent instructions, modify flags etc. Upon entrance to this function,
  /// all information about the instruction is in ::cmd structure.
  /// If zero is returned, the kernel will delete the instruction.
  int   (idaapi *u_emu)   (void);

  /// Generate text representation of an instruction in ::cmd structure.
  /// This function shouldn't change the database, flags or anything else.
  /// All these actions should be performed only by u_emu() function.
  void  (idaapi *u_out)   (void);

  /// Generate text representation of an instruction operand.
  /// This function shouldn't change the database, flags or anything else.
  /// All these actions should be performed only by u_emu() function.
  /// The output text is placed in the output buffer initialized with init_output_buffer()
  /// This function uses out_...() functions from ua.hpp to generate the operand text
  /// \retval 1  ok
  /// \retval 0  operand is hidden.
  bool  (idaapi *u_outop) (op_t &op);


  /// Generate text representation of data items.
  /// This function MAY change the database and create cross-references, etc.
  void  (idaapi *d_out)   (ea_t ea);          // disassemble data

  /// Compare instruction operands.
  /// This pointer may be NULL.
  /// \retval 1  equal
  /// \retval 0  not equal
  bool  (idaapi *cmp_opnd)(const op_t &op1, const op_t &op2);

  /// Can the operand have a type as offset, segment, decimal, etc.
  /// (for example, a register AX can't have a type, meaning that the user can't
  /// change its representation. see bytes.hpp for information about types and flags)
  /// This pointer may be NULL.
  bool  (idaapi *can_have_type)(op_t &op);



  //  Processor register information:
  int   regsNum;                        ///< number of registers
  const char *const *regNames;          ///< array of register names

  /// \name Warning
  /// The following pointers should be NULL:
  //@{
  const AbstractRegister *(idaapi *getreg)(int regnum);
                                        ///< Get register value.
                                        ///< If specified, will be
                                        ///< used in the determining predefined
                                        ///< comment based on the register value

  int   rFiles;                         ///< number of register files
  const char *const *rFnames;           ///< register names for files
  const rginfo *rFdescs;                ///< description of registers
  const class WorkReg *CPUregs;         ///< pointer to CPU registers
  //@}

  /// \name Segment registers
  /// Segment register information (use virtual CS and DS registers if your
  /// processor doesn't have segment registers):
  //@{
  int   regFirstSreg;                   ///< number of first segment register
  int   regLastSreg;                    ///< number of last segment register
  int   segreg_size;                    ///< size of a segment register in bytes
  //@}

  /// \name Virtual segment registers
  /// If your processor doesn't have segment registers,
  /// you should define 2 virtual segment registers for CS and DS.
  /// Let's call them rVcs and rVds.
  //@{
  int   regCodeSreg;                    ///< number of CS register
  int   regDataSreg;                    ///< number of DS register
  //@}


  /// \name Empirics
  //@{
  const bytes_t *codestart;             ///< Array of typical code start sequences.
                                        ///< This array is used when a new file
                                        ///< is loaded to find the beginnings of code
                                        ///< sequences.
                                        ///< This array is terminated with
                                        ///< a zero length item.
  const bytes_t *retcodes;              ///< Array of 'return' instruction opcodes.
                                        ///< This array is used to determine
                                        ///< form of autogenerated locret_...
                                        ///< labels.
                                        ///< The last item of it should be { 0, NULL }
                                        ///< This array may be NULL
                                        ///< Better way of handling return instructions
                                        ///< is to define the \idpcode{is_ret_insn} callback in
                                        ///< the notify() function
  //@}

  /// \name Instruction set
  //@{
  int   instruc_start;                  ///< icode of the first instruction
  int   instruc_end;                    ///< icode of the last instruction + 1

  /// Does the given value specify a valid instruction for this instruction set?.
  /// See #instruc_start and #instruc_end
  bool is_canon_insn(uint16 itype) const { return itype >= instruc_start && itype < instruc_end; }

  const instruc_t *instruc;             ///< Array of instructions
  //@}

  /// is indirect far jump or call instruction?.
  /// meaningful only if the processor has 'near' and 'far' reference types
  int   (idaapi *is_far_jump)(int icode);

  /// Translation function for offsets.
  /// Currently used in the offset display functions
  /// to calculate the referenced address
  ea_t (idaapi *translate)(ea_t base, adiff_t offset);

  /// Size of long double (tbyte) for this processor
  /// (meaningful only if \ash{a_tbyte} != NULL)
  size_t tbyte_size;

/// \defgroup REAL_ERROR_ Floating point/IEEE Conversion codes
/// Return values for processor_t::realcvt()
//@{
#define REAL_ERROR_FORMAT  -1 ///< not supported format for current .idp
#define REAL_ERROR_RANGE   -2 ///< number too big (small) for store (mem NOT modified)
#define REAL_ERROR_BADDATA -3 ///< illegal real data for load (IEEE data not filled)
//@}
  /// Floating point -> IEEE conversion function.
  /// \return \ref REAL_ERROR_ on error
  int (idaapi *realcvt)(void *m, uint16 *e, uint16 swt);

  /// Number of digits in floating numbers after the decimal point.
  /// If an element of this array equals 0, then the corresponding
  /// floating point data is not used for the processor.
  /// This array is used to align numbers in the output.
  /// - real_width[0] - number of digits for short floats (only PDP-11 has them)
  /// - real_width[1] - number of digits for "float"
  /// - real_width[2] - number of digits for "double"
  /// - real_width[3] - number of digits for "long double"
  ///
  /// Example: IBM PC module has { 0,7,15,19 }
  char real_width[4];

  /// Find 'switch' idiom.
  /// This function may use (and modify) ::cmd
  /// (you can assume cmd is correct).
  /// It will be called for instructions marked with #CF_JUMP.
  /// \retval 1  switch is found, 'si' is filled
  /// \retval 0  switch is not found
  bool (idaapi *is_switch)(switch_info_ex_t *si);

  /// Generate map file. If this pointer is NULL, the kernel itself
  /// will create the map file.
  /// \retval 0      empty file
  /// \retval -1     write error
  /// \retval other  number of lines in output file
  int32 (idaapi *gen_map_file)(FILE *fp);

  /// Extract address from a string.
  /// \retval BADADDR    if can't extract.
  /// \retval BADADDR-1  if kernel should use standard algorithm.
  ea_t (idaapi *extract_address)(ea_t ea,const char *string,int x);

/// \defgroup OP_FP_SP SP/FP operand flags
/// Return values for processor_t::is_sp_based()
//@{
#define OP_FP_BASED  0x00000000 ///< operand is FP based
#define OP_SP_BASED  0x00000001 ///< operand is SP based
#define OP_SP_ADD    0x00000000 ///< operand value is added to the pointer
#define OP_SP_SUB    0x00000002 ///< operand value is subtracted from the pointer
//@}
  /// Check whether the operand is relative to stack pointer or frame pointer.
  /// This function is used to determine how to output a stack variable
  /// This function may be absent. If it is absent, then all operands
  /// are sp based by default.
  /// Define this function only if some stack references use frame pointer
  /// instead of stack pointer.
  /// \return combination of \ref OP_FP_SP
  int (idaapi *is_sp_based)(const op_t &x);

  /// Create a function frame for a newly created function.
  /// Set up frame size, its attributes etc.
  /// This function may be absent.
  bool (idaapi *create_func_frame)(func_t *pfn);

  /// Get size of function return address in bytes.
  /// If this function is absent, the kernel will assume
  ///   - 4 bytes for 32-bit function
  ///   - 2 bytes otherwise
  /// \param pfn  pointer to function structure, can't be NULL
  int (idaapi *get_frame_retsize)(func_t *pfn);

  /// Generate stack variable definition line.
  /// If this function is NULL, then the kernel will create this line itself.
  /// Default line is
  ///             varname = type ptr value,
  /// where 'type' is one of byte,word,dword,qword,tbyte
  void (idaapi *gen_stkvar_def)(char *buf,
                                 size_t bufsize,
                                 const class member_t *mptr,
                                 sval_t v);

  /// Generate text representation of an item in a special segment.
  /// i.e. absolute symbols, externs, communal definitions etc.
  /// \retval 1  overflow
  /// \retval 0  ok
  bool (idaapi *u_outspec)(ea_t ea,uchar segtype);

  /// Icode of return instruction. It is ok to give any of possible return instructions
  int icode_return;

  /// Set IDP-specific option
  set_options_t *set_idp_options;

  /// Is the instruction created only for alignment purposes?.
  /// Do not directly call this function, use ::is_align_insn()
  /// returns: number of bytes in the instruction
  int (idaapi *is_align_insn)(ea_t ea);

  /// Reserved, currently equals to NULL
  void *unused_slot;

  /// If the #FIXUP_VHIGH and #FIXUP_VLOW fixup types are supported
  /// then the number of bits in the HIGH part. For example,
  /// SPARC will have here 22 because it has HIGH22 and LOW10 relocations.
  /// See also: the description of #PR_FULL_HIFXP bit
  int high_fixup_bits;
};

// The following two structures contain information about the current
// processor and assembler.

idaman processor_t ida_export_data ph;   ///< current processor
idaman asm_t ida_export_data ash;        ///< current assembler

idaman int ida_export str2regf(const char *p);    ///< Get word reg number (-1 on error)
idaman int ida_export str2reg(const char *p);     ///< Get any reg number (-1 on error)


/// If the instruction at 'ea' looks like an alignment instruction,
/// return its length in bytes. Otherwise return 0.

idaman int ida_export is_align_insn(ea_t ea);


/// Get text representation of a register.
/// For most processors this function will just return \ph{regNames}[reg].
/// If the processor module has implemented processor_t::get_reg_name, it will be
/// used instead
/// \param reg      internal register number as defined in the processor module
/// \param width    register width in bytes
/// \param buf      output buffer
/// \param bufsize  size of output buffer
/// \param reghi    if specified, then this function will return the register pair
/// \return length of register name in bytes or -1 if failure

idaman ssize_t ida_export get_reg_name(int reg, size_t width, char *buf, size_t bufsize, int reghi=-1);


/// Get register information - useful for registers like al, ah, dil, etc.
/// \return NULL no such register

inline const char *get_reg_info2(const char *regname, bitrange_t *bitrange)
{
  const char *r2;
  if ( ph.notify != NULL )
  {
    if ( ph.notify(ph.get_reg_info2, regname, &r2, bitrange) == 2 )
      return r2;
    uint64 mask;
    if ( ph.notify(ph.OBSOLETE(get_reg_info), regname, &r2, &mask) == 0 )
    {
      if ( bitrange != NULL )
        bitrange->assign_max_nonzero(mask);
      return r2;
    }
    if ( str2reg(regname) != -1 )
    {
      if ( bitrange != NULL )
        bitrange->reset();
      return regname;
    }
  }
  return NULL;
}

/// Get register number and size from register name
struct reg_info_t
{
  int reg;              ///< register number
  int size;             ///< register size
  DECLARE_COMPARISONS(reg_info_t)
  {
    if ( reg != r.reg )
      return reg > r.reg ? 1 : -1;
    if ( size != r.size )
      return size > r.size ? 1 : -1;
    return 0;
  }
};
DECLARE_TYPE_AS_MOVABLE(reg_info_t);
typedef qvector<reg_info_t> reginfovec_t; ///< vector of register info objects


/// Get register info by name.
/// \param regname  name of register
/// \param[out] ri  result
/// \return success

idaman bool ida_export parse_reg_name(const char *regname, reg_info_t *ri);


inline bool insn_t::is_canon_insn(void) const // see ::insn_t in ua.hpp
{
  return ph.is_canon_insn(itype);
}

inline const char *insn_t::get_canon_mnem(void) const // see ::insn_t in ua.hpp
{
  return is_canon_insn() ? ph.instruc[itype-ph.instruc_start].name : NULL;
}

inline uint32 insn_t::get_canon_feature(void) const // ::insn_t in ua.hpp
{
  return is_canon_insn() ? ph.instruc[itype-ph.instruc_start].feature : 0;
}


/// Get size of long double

inline size_t sizeof_ldbl(void)
{
  return inf.size_ldbl ? inf.size_ldbl : ph.tbyte_size;
}


idaman void ida_export intel_data(ea_t ea);
                                        ///< Kernel function to display data items
                                        ///< and undefined bytes.
                                        ///< This function should be used to
                                        ///< display data.
idaman bool ida_export gen_spcdef(ea_t ea,uchar segtype);
                                        ///< Generate declaration for item
                                        ///< in a special segment.
                                        ///< \retval 1  overflow
                                        ///< \retval 0  ok
idaman bool ida_export gen_extern(ea_t ea,const char *name);
                                        ///< Generate declaration of extern symbol.
                                        ///< \retval 1  overflow
                                        ///< \retval 0  ok
idaman bool ida_export gen_abssym(ea_t ea,const char *name);
                                        ///< Generate declaration of absolute symbol.
                                        ///< \retval 1  overflow
                                        ///< \retval 0  ok
idaman bool ida_export gen_comvar(ea_t ea,const char *name);
                                        ///< Generate declaration of communal variable
                                        ///< \retval 1  overflow
                                        ///< \retval 0  ok

/// \defgroup SETPROC_ Set processor levels
/// Flags passed as 'level' parameter to set_processor_type()
//@{
#define SETPROC_COMPAT  0     ///< search for the processor type in the current module
#define SETPROC_ALL     1     ///< search for the processor type in all modules,
                              ///< only if there were not calls with #SETPROC_USER
#define SETPROC_USER    2     ///< search for the processor type in all modules
                              ///< and prohibit level #SETPROC_USER
#define SETPROC_FATAL   0x80  ///< if the processor can't be set, IDA should display
                              ///< an error message and exit (can be combined with previous bits)
//@}
/// Set target processor type.
/// \param procname  name of processor type (one of names present in \ph{psnames})
/// \param level     \ref SETPROC_
/// \return NULL if failed, otherwise path of file with processor module

idaman char *ida_export set_processor_type(const char *procname,int level);


/// Get name of the current processor module.
/// The name is derived from the file name.
/// For example, for IBM PC the module is named "pc.w32" (windows version),
/// then the module name is "PC" (uppercase).
/// If no processor module is loaded, this function will return NULL
/// \param buf  the output buffer, should be at least #QMAXFILE length

idaman char *ida_export get_idp_name(char *buf, size_t bufsize);


/// Set target assembler.
/// \param asmnum  number of assembler in the current processor module
/// \return success

idaman bool ida_export set_target_assembler(int asmnum);


/// Helper function to register a custom fixup. It is can be called by
/// the file loader and the processor module will receive the processor_t::register_custom_fixup
/// event. It returns the custom format id (<=0 if failed)

inline int create_custom_fixup(const char *name)
{
  return ph.notify(processor_t::register_custom_fixup, name) - 1;
}

/// Helper function to get the delay slot instruction
inline bool delay_slot_insn(ea_t *ea, bool *bexec, bool *fexec)
{
  bool ok = (ph.flag & PR_DELAYED) != 0;
  if ( ok )
  {
    bool be = true;
    bool fe = true;
    ok = ph.notify(processor_t::delay_slot_insn, ea, &be, &fe) > 1;
    if ( ok )
    {
      if ( bexec != NULL )
        *bexec = be;
      if ( fexec != NULL )
        *fexec = fe;
    }
  }
  return ok;
}

/// IDB event group. Some events are still in the processor group, so you will
/// need to hook to both groups. These events do not return anything.
///
/// The callback function should return 0 but the kernel won't check it.
/// Use the hook_to_notification_point() function to install your callback.
namespace idb_event
{
  //<hookgen IDB>

  /// IDB event codes
  enum event_code_t
  {
    byte_patched,           ///< A byte has been patched.
                            ///< \param ea         (::ea_t)
                            ///< \param old_value  (::uint32)

    cmt_changed,            ///< An item comment has been changed.
                            ///< \param ea              (::ea_t)
                            ///< \param repeatable_cmt  (bool)

    ti_changed,             ///< An item typestring (c/c++ prototype) has been changed.
                            ///< \param ea      (::ea_t)
                            ///< \param type    (const ::type_t *)
                            ///< \param fnames  (const ::p_list *)

    op_ti_changed,          ///< An operand typestring (c/c++ prototype) has been changed.
                            ///< \param ea (::ea_t)
                            ///< \param n  (int)
                            ///< \param type (const ::type_t *)
                            ///< \param fnames (const ::p_list *)

    op_type_changed,        ///< An operand type (offset, hex, etc...) has been set or deleted.
                            ///< \param ea  (::ea_t)
                            ///< \param n   (int) eventually or'ed with OPND_OUTER

    enum_created,           ///< An enum type has been created.
                            ///< \param id  (::enum_t)

    enum_deleted,           ///< An enum type has been deleted.
                            ///< \param id  (::enum_t)

    enum_bf_changed,        ///< An enum type 'bitfield' attribute has been changed.
                            ///< \param id  (::enum_t)

    enum_renamed,           ///< An enum or member has been renamed.
                            ///< \param id  (::tid_t)

    enum_cmt_changed,       ///< An enum or member type comment has been changed.
                            ///< \param id          (::tid_t)
                            ///< \param repeatable  (bool)
#ifndef NO_OBSOLETE_FUNCS
    enum_const_created,
    enum_const_deleted,
#else
    enum_member_created,    ///< An enum member has been created.
                            ///< \param id   (::enum_t)
                            ///< \param cid  (::const_t)

    enum_member_deleted,    ///< An enum member has been deleted.
                            ///< \param id   (::enum_t)
                            ///< \param cid  (::const_t)
#endif
    struc_created,          ///< A new structure type has been created.
                            ///< \param struc_id  (::tid_t)

    struc_deleted,          ///< A structure type has been deleted.
                            ///< \param struc_id  (::tid_t)

    struc_renamed,          ///< A structure type has been renamed.
                            ///< \param sptr (::struc_t *)

    struc_expanded,         ///< A structure type has been expanded/shrank.
                            ///< \param sptr (::struc_t *)

    struc_cmt_changed,      ///< A structure type comment has been changed.
                            ///< \param struc_id        (::tid_t)
                            ///< \param repeatable_cmt  (bool)

    struc_member_created,   ///< A structure member has been created.
                            ///< \param sptr  (::struc_t *)
                            ///< \param mptr  (::member_t *)

    struc_member_deleted,   ///< A structure member has been deleted.
                            ///< \param sptr       (::struc_t *)
                            ///< \param member_id  (::tid_t)
                            ///< \param offset     (::ea_t)

    struc_member_renamed,   ///< A structure member has been renamed.
                            ///< \param sptr  (::struc_t *)
                            ///< \param mptr  (::member_t *)

    struc_member_changed,   ///< A structure member has been changed.
                            ///< \param sptr  (::struc_t *)
                            ///< \param mptr  (::member_t *)

    thunk_func_created,     ///< A thunk bit has been set for a function.
                            ///< \param pfn  (::func_t *)

    func_tail_appended,     ///< A function tail chunk has been appended.
                            ///< \param pfn   (::func_t *)
                            ///< \param tail  (::func_t *)

    func_tail_removed,      ///< A function tail chunk has been removed.
                            ///< \param pfn      (::func_t *)
                            ///< \param tail_ea  (::ea_t)

    tail_owner_changed,     ///< A tail chunk owner has been changed.
                            ///< \param tail        (::func_t *)
                            ///< \param owner_func  (::ea_t)
                            ///< \param old_owner   (::ea_t)

    func_noret_changed,     ///< #FUNC_NORET bit has been changed.
                            ///< \param pfn  (::func_t *)

    segm_added,             ///< A new segment has been created.
                            ///< \param s  (::segment_t *)

    segm_deleted,           ///< A segment has been deleted.
                            ///< \param startEA  (::ea_t)
                            ///< \param endEA    (::ea_t)

    segm_start_changed,     ///< Segment start address has been changed.
                            ///< \param s        (::segment_t *)
                            ///< \param oldstart (::ea_t)

    segm_end_changed,       ///< Segment end address has been changed.
                            ///< \param s      (::segment_t *)
                            ///< \param oldend (::ea_t)

    segm_moved,             ///< Segment has been moved.
                            ///< \param from    (::ea_t)
                            ///< \param to      (::ea_t)
                            ///< \param size    (::asize_t)
                            ///< See also \ref idb_event::allsegs_moved

    area_cmt_changed,       ///< Area comment has been changed.
                            ///< \param cb          (::areacb_t *) may be: &funcs, &segs, etc.
                            ///< \param a           (const ::area_t *)
                            ///< \param cmt         (const char *)
                            ///< \param repeatable  (bool)

                            // Events that happen before database modifications
                            // IDA may not to call the corresponding ...changed event
                            // if there are any errors.

    changing_cmt,           ///< An item comment is to be changed.
                            ///< \param ea              (::ea_t)
                            ///< \param repeatable_cmt  (bool)
                            ///< \param newcmt          (const char *)

    changing_ti,            ///< An item typestring (c/c++ prototype) is to be changed.
                            ///< \param ea          (::ea_t)
                            ///< \param new_type    (const ::type_t *)
                            ///< \param new_fnames  (const ::p_list *)

    changing_op_ti,         ///< An operand typestring (c/c++ prototype) is to be changed.
                            ///< \param ea          (::ea_t)
                            ///< \param n           (int)
                            ///< \param new_type    (const ::type_t *)
                            ///< \param new_fnames  (const ::p_list *)

    changing_op_type,       ///< An operand type (offset, hex, etc...) is to be changed.
                            ///< \param ea  (::ea_t)
                            ///< \param n   (int) eventually or'ed with OPND_OUTER

    deleting_enum,          ///< An enum type is to be deleted.
                            ///< \param id  (::enum_t)

    changing_enum_bf,       ///< An enum type 'bitfield' attribute is to be changed.
                            ///< \param id      (::enum_t)
                            ///< \param new_bf  (bool)

    renaming_enum,          ///< An enum or enum member is to be renamed.
                            ///< \param id       (::tid_t)
                            ///< \param is_enum  (bool)
                            ///< \param newname  (const char *)

    changing_enum_cmt,      ///< An enum or member type comment is to be changed.
                            ///< \param id          (::tid_t)
                            ///< \param repeatable  (bool)
                            ///< \param newcmt      (const char *)
#ifndef NO_OBSOLETE_FUNCS
    deleting_enum_const,
#else
    deleting_enum_member,   ///< An enum member is to be deleted.
                            ///< \param id   (::enum_t)
                            ///< \param cid  (::const_t)
#endif
    deleting_struc,         ///< A structure type is to be deleted.
                            ///< \param sptr  (::struc_t *)

    renaming_struc,         ///< A structure type is to be renamed.
                            ///< \param id       (::tid_t)
                            ///< \param oldname  (const char *)
                            ///< \param newname  (const char *)

    expanding_struc,        ///< A structure type is to be expanded/shrunk.
                            ///< \param sptr    (::struc_t *)
                            ///< \param offset  (::ea_t)
                            ///< \param delta   (::adiff_t)

    changing_struc_cmt,     ///< A structure type comment is to be changed.
                            ///< \param struc_id    (::tid_t)
                            ///< \param repeatable  (bool)
                            ///< \param newcmt      (const char *)

    deleting_struc_member,  ///< A structure member is to be deleted.
                            ///< \param sptr  (::struc_t *)
                            ///< \param mptr  (::member_t *)

    renaming_struc_member,  ///< A structure member is to be renamed.
                            ///< \param sptr     (::struc_t *)
                            ///< \param mptr     (::member_t *)
                            ///< \param newname  (const char *)

    changing_struc_member,  ///< A structure member is to be changed.
                            ///< \param sptr    (::struc_t *)
                            ///< \param mptr    (::member_t *)
                            ///< \param flag    (::flags_t)
                            ///< \param ti      (const ::opinfo_t *)
                            ///< \param nbytes  (::asize_t)

    removing_func_tail,     ///< A function tail chunk is to be removed.
                            ///< \param pfn   (::func_t *)
                            ///< \param tail  (const ::area_t *)

    deleting_segm,          ///< A segment is to be deleted.
                            ///< \param startEA  (::ea_t)

    changing_segm_start,    ///< Segment start address is to be changed.
                            ///< \param s             (::segment_t *)
                            ///< \param new_start     (::ea_t)
                            ///< \param segmod_flags  (int)

    changing_segm_end,      ///< Segment end address is to be changed.
                            ///< \param s             (::segment_t *)
                            ///< \param new_end       (::ea_t)
                            ///< \param segmod_flags  (int)

    changing_area_cmt,      ///< Area comment is to be changed.
                            ///< \param cb          (::areacb_t *)
                            ///< \param a           (const ::area_t *)
                            ///< \param cmt         (const char *)
                            ///< \param repeatable  (bool)

    changing_segm_name,     ///< Segment name is being changed.
                            ///< \param s        (::segment_t *)
                            ///< \param oldname  (const char *)

    changing_segm_class,    ///< Segment class is being changed.
                            ///< \param s  (::segment_t *)

    segm_name_changed,      ///< Segment name has been changed.
                            ///< \param s        (::segment_t *)
                            ///< \param name     (const char *)

    segm_class_changed,     ///< Segment class has been changed.
                            ///< \param s        (::segment_t *)
                            ///< \param sclass   (const char *)

    destroyed_items,        ///< Instructions/data have been destroyed in [ea1,ea2).
                            ///< \param ea1                 (::ea_t)
                            ///< \param ea2                 (::ea_t)
                            ///< \param will_disable_range  (bool)

    changed_stkpnts,        ///< Stack change points have been modified.
                            ///< \param pfn  (::func_t *)

    extra_cmt_changed,      ///< An extra comment has been changed.
                            ///< \param ea        (::ea_t)
                            ///< \param line_idx  (int)
                            ///< \param cmt       (const char *)

    changing_struc,         ///< A structure type is being changed (the struct alignment).
                            ///< \param sptr  (::struc_t *)

    changed_struc,          ///< A structure type has been changed (the struct alignment).
                            ///< \param sptr  (::struc_t *)

    local_types_changed,    ///< Local types have been changed

    segm_attrs_changed,     ///< Segment attributes has been changed.
                            ///< \param s        (::segment_t *)
                            ///< This event is generated for secondary segment
                            ///< attributes (examples: color, permissions, etc)

    allsegs_moved,          ///< Program rebasing is complete.
                            ///< This event is generated after series of
                            ///< segm_moved events
                            ///< \param info     (::segm_move_infos_t *)
  };
}




#ifndef NO_OBSOLETE_FUNCS
idaman DEPRECATED bool ida_export read_user_config_file(
        const char *file,
        set_options_t *callback,
        const char *macroname = NULL);
#endif // NO_OBSOLETE_FUNCS

#pragma pack(pop)
#endif // _IDP_HPP
