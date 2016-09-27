/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2015 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 */

#ifndef _LINES_HPP
#define _LINES_HPP

#include <ida.hpp>

#pragma pack(push, 1)           // IDA uses 1 byte alignments!

/*! \file lines.hpp

  \brief High level functions that deal with the generation
  of the disassembled text lines.

  This file also contains definitions for the syntax highlighting.

  Finally there are functions that deal with anterior/posterior
  user-defined lines.
*/

struct area_t;

//---------------------------------------------------------------------------
//      C O L O R   D E F I N I T I O N S
//---------------------------------------------------------------------------

/// \defgroup color_def Color definitions
///
/// Here we describe the structure of embedded escape sequences used to
/// implement syntax highlighting.
///
/// IDP module should insert appropriate escape characters into the
/// output lines as necessary. This approach allows to create an IDP
/// module without the syntax highlighting too - just don't use
/// escape sequences.
///
/// A typical color sequence looks like this:
///
/// #COLOR_ON COLOR_xxx text #COLOR_OFF COLOR_xxx
///
/// The first 2 items turn color 'xxx' on, then the text follows,
/// and the color is turned off by two last items.
///
/// For the convenience we've defined a set of macro definitions
/// and functions to deal with colors.
//@{

/// \defgroup color_esc Color escape characters
/// Initiate/Terminate a color tag
//@{
#define COLOR_ON        '\1'     ///< Escape character (ON).
                                 ///< Followed by a color code (::color_t).
#define COLOR_OFF       '\2'     ///< Escape character (OFF).
                                 ///< Followed by a color code (::color_t).
#define COLOR_ESC       '\3'     ///< Escape character (Quote next character).
                                 ///< This is needed to output '\1' and '\2'
                                 ///< characters.
#define COLOR_INV       '\4'     ///< Escape character (Inverse foreground and background colors).
                                 ///< This escape character has no corresponding #COLOR_OFF.
                                 ///< Its action continues until the next #COLOR_INV or end of line.

#define SCOLOR_ON       "\1"     ///< Escape character (ON)
#define SCOLOR_OFF      "\2"     ///< Escape character (OFF)
#define SCOLOR_ESC      "\3"     ///< Escape character (Quote next character)
#define SCOLOR_INV      "\4"     ///< Escape character (Inverse colors)

/// Is the given char a color escape character?
inline bool requires_color_esc(char c) { return c >= COLOR_ON && c <= COLOR_INV; }
//@}

typedef uchar color_t;           ///< color tag - see \ref COLOR_
/// \defgroup COLOR_ Color tags
/// Specify a color for a syntax item
//@{
const color_t
  COLOR_DEFAULT  = 0x01,         ///< Default
  COLOR_REGCMT   = 0x02,         ///< Regular comment
  COLOR_RPTCMT   = 0x03,         ///< Repeatable comment (comment defined somewhere else)
  COLOR_AUTOCMT  = 0x04,         ///< Automatic comment
  COLOR_INSN     = 0x05,         ///< Instruction
  COLOR_DATNAME  = 0x06,         ///< Dummy Data Name
  COLOR_DNAME    = 0x07,         ///< Regular Data Name
  COLOR_DEMNAME  = 0x08,         ///< Demangled Name
  COLOR_SYMBOL   = 0x09,         ///< Punctuation
  COLOR_CHAR     = 0x0A,         ///< Char constant in instruction
  COLOR_STRING   = 0x0B,         ///< String constant in instruction
  COLOR_NUMBER   = 0x0C,         ///< Numeric constant in instruction
  COLOR_VOIDOP   = 0x0D,         ///< Void operand
  COLOR_CREF     = 0x0E,         ///< Code reference
  COLOR_DREF     = 0x0F,         ///< Data reference
  COLOR_CREFTAIL = 0x10,         ///< Code reference to tail byte
  COLOR_DREFTAIL = 0x11,         ///< Data reference to tail byte
  COLOR_ERROR    = 0x12,         ///< Error or problem
  COLOR_PREFIX   = 0x13,         ///< Line prefix
  COLOR_BINPREF  = 0x14,         ///< Binary line prefix bytes
  COLOR_EXTRA    = 0x15,         ///< Extra line
  COLOR_ALTOP    = 0x16,         ///< Alternative operand
  COLOR_HIDNAME  = 0x17,         ///< Hidden name
  COLOR_LIBNAME  = 0x18,         ///< Library function name
  COLOR_LOCNAME  = 0x19,         ///< Local variable name
  COLOR_CODNAME  = 0x1A,         ///< Dummy code name
  COLOR_ASMDIR   = 0x1B,         ///< Assembler directive
  COLOR_MACRO    = 0x1C,         ///< Macro
  COLOR_DSTR     = 0x1D,         ///< String constant in data directive
  COLOR_DCHAR    = 0x1E,         ///< Char constant in data directive
  COLOR_DNUM     = 0x1F,         ///< Numeric constant in data directive
  COLOR_KEYWORD  = 0x20,         ///< Keywords
  COLOR_REG      = 0x21,         ///< Register name
  COLOR_IMPNAME  = 0x22,         ///< Imported name
  COLOR_SEGNAME  = 0x23,         ///< Segment name
  COLOR_UNKNAME  = 0x24,         ///< Dummy unknown name
  COLOR_CNAME    = 0x25,         ///< Regular code name
  COLOR_UNAME    = 0x26,         ///< Regular unknown name
  COLOR_COLLAPSED= 0x27,         ///< Collapsed line
  COLOR_FG_MAX   = 0x28,         ///< Max color number

  // Fictive colors

  COLOR_ADDR     = COLOR_FG_MAX, ///< hidden address marks.
                                 ///< the address is represented as 8digit
                                 ///< hex number: 01234567.
                                 ///< it doesn't have #COLOR_OFF pair.
                                 ///< NB: for 64-bit IDA, the address is 16digit.

  COLOR_OPND1    = COLOR_ADDR+1, ///< Instruction operand 1
  COLOR_OPND2    = COLOR_ADDR+2, ///< Instruction operand 2
  COLOR_OPND3    = COLOR_ADDR+3, ///< Instruction operand 3
  COLOR_OPND4    = COLOR_ADDR+4, ///< Instruction operand 4
  COLOR_OPND5    = COLOR_ADDR+5, ///< Instruction operand 5
  COLOR_OPND6    = COLOR_ADDR+6, ///< Instruction operand 6


  COLOR_UTF8     = COLOR_ADDR+10,///< Following text is UTF-8 encoded
  COLOR_RESERVED1= COLOR_ADDR+11;///< This tag is reserved for internal IDA use
//@}

/// Size of a tagged address (see ::COLOR_ADDR)
#define COLOR_ADDR_SIZE (sizeof(ea_t)*2)

/// \defgroup SCOLOR_ Color string constants
/// These definitions are used with the #COLSTR macro
//@{
#define SCOLOR_DEFAULT   "\x01"  ///< Default
#define SCOLOR_REGCMT    "\x02"  ///< Regular comment
#define SCOLOR_RPTCMT    "\x03"  ///< Repeatable comment (defined not here)
#define SCOLOR_AUTOCMT   "\x04"  ///< Automatic comment
#define SCOLOR_INSN      "\x05"  ///< Instruction
#define SCOLOR_DATNAME   "\x06"  ///< Dummy Data Name
#define SCOLOR_DNAME     "\x07"  ///< Regular Data Name
#define SCOLOR_DEMNAME   "\x08"  ///< Demangled Name
#define SCOLOR_SYMBOL    "\x09"  ///< Punctuation
#define SCOLOR_CHAR      "\x0A"  ///< Char constant in instruction
#define SCOLOR_STRING    "\x0B"  ///< String constant in instruction
#define SCOLOR_NUMBER    "\x0C"  ///< Numeric constant in instruction
#define SCOLOR_VOIDOP    "\x0D"  ///< Void operand
#define SCOLOR_CREF      "\x0E"  ///< Code reference
#define SCOLOR_DREF      "\x0F"  ///< Data reference
#define SCOLOR_CREFTAIL  "\x10"  ///< Code reference to tail byte
#define SCOLOR_DREFTAIL  "\x11"  ///< Data reference to tail byte
#define SCOLOR_ERROR     "\x12"  ///< Error or problem
#define SCOLOR_PREFIX    "\x13"  ///< Line prefix
#define SCOLOR_BINPREF   "\x14"  ///< Binary line prefix bytes
#define SCOLOR_EXTRA     "\x15"  ///< Extra line
#define SCOLOR_ALTOP     "\x16"  ///< Alternative operand
#define SCOLOR_HIDNAME   "\x17"  ///< Hidden name
#define SCOLOR_LIBNAME   "\x18"  ///< Library function name
#define SCOLOR_LOCNAME   "\x19"  ///< Local variable name
#define SCOLOR_CODNAME   "\x1A"  ///< Dummy code name
#define SCOLOR_ASMDIR    "\x1B"  ///< Assembler directive
#define SCOLOR_MACRO     "\x1C"  ///< Macro
#define SCOLOR_DSTR      "\x1D"  ///< String constant in data directive
#define SCOLOR_DCHAR     "\x1E"  ///< Char constant in data directive
#define SCOLOR_DNUM      "\x1F"  ///< Numeric constant in data directive
#define SCOLOR_KEYWORD   "\x20"  ///< Keywords
#define SCOLOR_REG       "\x21"  ///< Register name
#define SCOLOR_IMPNAME   "\x22"  ///< Imported name
#define SCOLOR_SEGNAME   "\x23"  ///< Segment name
#define SCOLOR_UNKNAME   "\x24"  ///< Dummy unknown name
#define SCOLOR_CNAME     "\x25"  ///< Regular code name
#define SCOLOR_UNAME     "\x26"  ///< Regular unknown name
#define SCOLOR_COLLAPSED "\x27"  ///< Collapsed line
#define SCOLOR_ADDR      "\x28"  ///< Hidden address mark
//@}

/// This macro is used to build colored string constants
#define COLSTR(str,tag) SCOLOR_ON tag str SCOLOR_OFF tag


//------------------------------------------------------------------------

/// \defgroup color_conv Convenience functions
/// Higher level convenience functions are defined in ua.hpp.
/// Please use the following functions only if functions from ua.hpp
/// are not useful in your case.
//@{

/// Append 'turn on color' sequence to a string.
/// \param ptr  pointer to the output buffer
/// \param end  pointer to the end of the buffer
/// \param tag  one of \ref COLOR_
/// \return ptr to end of string

idaman char *ida_export tag_on(char *ptr, char *end, color_t tag);


/// Append 'turn off color' sequence to a string.
/// \param ptr  pointer to the output buffer
/// \param end  pointer to the end of the buffer
/// \param tag  one of \ref COLOR_
/// \return ptr to end of string

idaman char *ida_export tag_off(char *ptr, char *end, color_t tag);


/// Append a colored character to a string.
/// \param ptr  pointer to the output buffer
/// \param end  pointer to the end of the buffer
/// \param tag  one of \ref COLOR_
/// \param chr  character to append
/// \return ptr to end of string

idaman char *ida_export tag_addchr(char *ptr, char *end, color_t tag, char chr);


/// Append a colored substring to a string.
/// \param ptr     pointer to the output buffer
/// \param end     pointer to the end of the buffer
/// \param tag     one of \ref COLOR_
/// \param string  substring to append
/// \return ptr to end of string

idaman char *ida_export tag_addstr(char *ptr, char *end, color_t tag, const char *string);


/// Append an address mark to a string.
/// \param ptr  pointer to the output buffer
/// \param end  pointer to the end of the buffer
/// \param ea   address to include
/// \return ptr to end of string

idaman char *ida_export tag_addr(char *ptr, char *end, ea_t ea);


/// Move pointer to a 'line' to 'cnt' positions right.
/// Take into account escape sequences.
/// \param line  pointer to string
/// \param cnt   number of positions to move right
/// \return moved pointer

idaman const char *ida_export tag_advance(const char *line, int cnt);


/// Move the pointer past all color codes.
/// \param line  can't be NULL
/// \return moved pointer, can't be NULL

idaman const char *ida_export tag_skipcodes(const char *line);


/// Skip one color code.
/// This function should be used if you are interested in color codes
/// and want to analyze all of them.
/// Otherwise tag_skipcodes() function is better since it will skip all colors at once.
/// This function will skip the current color code if there is one.
/// If the current symbol is not a color code, it will return the input.
/// \return moved pointer

idaman const char *ida_export tag_skipcode(const char *line);


/// Calculate length of a colored string.
/// \return -1 if error

idaman ssize_t ida_export tag_strlen(const char *line);


/// Remove color escape sequences from a string.
/// Input and output buffer may be the same.
/// \param instr    input colored string.
/// \param buf      output buffer.
///                 if == NULL, then return -1.
/// \param bufsize  size of output buffer.
///                 if == 0, then don't check size of output buffer.
/// \return length of resulting string, -1 if error

idaman ssize_t ida_export tag_remove(const char *instr, char *buf, size_t bufsize);
//@} color_conv

//@} color_def

//----------------- Line prefix colors --------------------------------------
/// \defgroup COLOR_PFX Line prefix colors
/// Line prefix colors are not used in modules
//@{
#define COLOR_DEFAULT    0x01   ///< Default
#define COLOR_SELECTED   0x02   ///< Selected
#define COLOR_LIBFUNC    0x03   ///< Library function
#define COLOR_REGFUNC    0x04   ///< Regular function
#define COLOR_CODE       0x05   ///< Single instruction
#define COLOR_DATA       0x06   ///< Data bytes
#define COLOR_UNKNOWN    0x07   ///< Unexplored byte
#define COLOR_EXTERN     0x08   ///< External name definition segment
#define COLOR_CURITEM    0x09   ///< Current item
#define COLOR_CURLINE    0x0A   ///< Current line
#define COLOR_HIDLINE    0x0B   ///< Hidden line
#define COLOR_BG_MAX     0x0C   ///< Max color number

#define PALETTE_SIZE       (COLOR_FG_MAX+COLOR_BG_MAX)
//@}

/// Get prefix color for line at 'ea'
/// \return \ref COLOR_PFX
idaman color_t   ida_export calc_prefix_color(ea_t ea);

/// Get background color for line at 'ea'
/// \return RGB color
idaman bgcolor_t ida_export calc_bg_color(ea_t ea);

/// Structure to keep some background colors configurable in ida.cfg
struct bgcolors_t
{
  bgcolor_t prolog_color;
  bgcolor_t epilog_color;
  bgcolor_t switch_color;
};
extern bgcolors_t bgcolors;

//------------------------------------------------------------------------
//      S O U R C E   F I L E S
//------------------------------------------------------------------------

/// \name Source files
/// IDA can keep information about source files used to create the program.
/// Each source file is represented by a range of addresses.
/// A source file may contain several address ranges.
//@{

/// Mark a range of address as belonging to a source file.
/// An address range may belong only to one source file.
/// A source file may be represented by several address ranges.
/// \param ea1       linear address of start of the address range
/// \param ea2       linear address of end of the address range (excluded)
/// \param filename  name of source file.
/// \return success

idaman bool ida_export add_sourcefile(ea_t ea1,ea_t ea2,const char *filename);


/// Get name of source file occupying the given address.
/// \param ea      linear address
/// \param bounds  pointer to the output buffer with the address range
///                for the current file. May be NULL.
/// \return NULL if source file information is not found,
///          otherwise returns pointer to file name

idaman const char *ida_export get_sourcefile(ea_t ea, area_t *bounds=NULL);


/// Delete information about the source file.
/// \param ea  linear address
/// \return success

idaman bool ida_export del_sourcefile(ea_t ea);
//@}

//------------------------------------------------------------------------
//      G E N E R A T I O N  O F  D I S A S S E M B L E D  T E X T
//------------------------------------------------------------------------

/// \name Additional information
/// The following variables control generation of additional information.
/// Initially they are set to 0, you should set them to 1 when you want
/// additional information generated upon calling MakeLine()
//@{
idaman char ida_export_data gl_comm;   ///< generate comment at the next call to MakeLine()
idaman char ida_export_data gl_name;   ///< generate name    at the next call to MakeLine()
idaman char ida_export_data gl_xref;   ///< generate xrefs   at the next call to MakeLine()
//@}

/// \name Prefix lengths
/// The following variables contain lengths of line prefix and binary line prefix
/// accordingly. You can use them IDP modules to calculate necessary indentions
/// and the resulting string length if you need to.
//@{
idaman int ida_export_data gl_psize;   ///< Line prefix width (set by setup_makeline)
idaman int ida_export_data gl_bpsize;  ///< Binary line prefix width (set by setup_makeline)
//@}

/// \name Generation of disassembled text
//@{

/// User-defined line-prefixes are displayed just after the autogenerated
/// line prefixes. In order to use them, the plugin should call the
/// following function to specify its width and contents.
/// \param width                     the width of the user-defined prefix
/// \param get_user_defined_prefix   a callback to get the contents of the prefix.
///                                  Its arguments:
///                                    - ea:       linear address
///                                    - indent:   indent of the line contents.
///                                                -1 means the default instruction
///                                                indent and is used for instruction
///                                                itself. see explanations for printf_line().
///                                    - line:     the line to be generated.
///                                                the line usually contains color tags.
///                                                this argument can be examined to decide
///                                                whether to generated the prefix.
///                                    - buf:      the output buffer
///                                    - bufsize:  the size of the output buffer
///
/// In order to remove the callback before unloading the plugin,
/// specify the width or the callback == NULL.

idaman void ida_export set_user_defined_prefix(size_t width,
                        void (idaapi*get_user_defined_prefix)(ea_t ea,
                                                        int lnnum,
                                                        int indent,
                                                        const char *line,
                                                        char *buf,
                                                        size_t bufsize));


/// Generate ONE line of disassembled text. You may call this function from
/// out.cpp as many times as you need to generate all lines for an item
/// (instruction or data).
/// \param contents  colored line to generate
/// \param indent    see explanation for printf_line()
/// \retval 1  you've made too many calls to this function, you should stop
///            calling it and return to the caller.
///            The current limit is 500 lines per item.
/// \retval 0  ok

idaman bool ida_export MakeLine(const char *contents,int indent=-1);


/// See printf_line()

idaman AS_PRINTF(2, 0) bool ida_export printf_line_v(
        int indent,
        const char *format,
        va_list va);


/// Generate ONE line of disassembled text. You may call this function from
/// out.cpp as many times as you need to generate all lines for an item
/// (instruction or data).
/// \param format   printf style colored line to generate
/// \param indent   indention of the line.
///                 if indent == -1, the kernel will indent the line
///                 at \inf{indent}. if indent < 0, -indent will be used for indention.
///                 The first line printed with indent < 0 is considered as the
///                 most important line at the current address. Usually it is
///                 the line with the instruction itself. This line will be
///                 displayed in the cross-reference lists and other places.
///                 If you need to output an additional line before the main line
///                 then pass \inf{indent} instead of -1. The kernel will know
///                 that your line is not the most important one.
/// \retval 1  you've made too many calls to this function, you should stop
///            calling it and return to the caller.
///            The current limit is 500 lines per item.
/// \retval 0  ok

AS_PRINTF(2, 3) inline bool printf_line(int indent, const char *format, ...)
{
  va_list va;
  va_start(va,format);
  bool code = printf_line_v(indent,format,va);
  va_end(va);
  return code;
}


/// Generate empty line. This function does nothing if generation of empty
/// lines is disabled.
/// \retval 1  the limit of lines per item is reached.
///            The current limit is determined by MAX_ITEM_LINES in IDA.CFG
/// \retval 0  ok

idaman bool ida_export MakeNull(void);


/// Generate thin border line. This function does nothing if generation
/// of border lines is disabled.
/// \retval 1  the limit of lines per item is reached.
///            The current limit is determined by MAX_ITEM_LINES in IDA.CFG
/// \retval 0  ok

idaman bool ida_export MakeBorder(void);


/// Generate solid border line.
/// \retval 1  the limit of lines per item is reached.
///            The current limit is determined by MAX_ITEM_LINES in IDA.CFG
/// \retval 0  ok

idaman bool ida_export MakeSolidBorder (void);


/// See gen_cmt_line()

idaman AS_PRINTF(2, 0) bool ida_export gen_colored_cmt_line_v(
        color_t color,
        const char *format,
        va_list va);

/// See gen_cmt_line()

AS_PRINTF(1, 0) inline bool gen_cmt_line_v(const char *format, va_list va)
{
  return gen_colored_cmt_line_v(COLOR_AUTOCMT, format, va);
}


/// Generate one non-indented comment line, colored with ::COLOR_AUTOCMT.
/// \param format  printf() style format line. The resulting comment line
///                should not include comment character (;)
/// \retval 1  the limit of lines per item is reached
/// \retval 0  ok

AS_PRINTF(1, 2) inline bool gen_cmt_line(const char *format, ...)
{
  va_list va;
  va_start(va, format);
  bool code = gen_cmt_line_v(format, va);
  va_end(va);
  return code;
}


/// Generate one non-indented comment line, colored with ::COLOR_COLLAPSED.
/// \param format  printf() style format line. The resulting comment line
///                should not include comment character (;)
/// \retval 1  you've made too many calls to MakeLine(), you should stop
///            calling MakeLine() and return to the caller.
/// \retval 0  ok

AS_PRINTF(1, 2) inline bool gen_collapsed_line(const char *format, ...)
{
  va_list va;
  va_start(va,format);
  bool answer = gen_colored_cmt_line_v(COLOR_COLLAPSED, format, va);
  va_end(va);
  return answer;
}


/// Generate big non-indented comment lines.
/// \param cmt    comment text. may contain \\n characters to denote new lines.
///               should not contain comment character (;)
/// \param color  color of comment text (one of \ref COLOR_)
/// \retval 1  you've made too many calls to MakeLine(), you should stop
///            calling MakeLine() and return to the caller.
///            The current limit is 500 lines per item.
/// \retval 0  ok

idaman bool ida_export generate_big_comment(const char *cmt, color_t color);


/// Generate many non-indented lines.
/// \param string  text. may contain \\n characters to denote new lines.
/// \param color   color of the text (one of \ref COLOR_)
/// \retval 1  you've made too many calls to MakeLine(), you should stop
///            calling MakeLine() and return to the caller.
///            The current limit is 500 lines per item.
/// \retval 0  ok

idaman bool ida_export generate_many_lines(const char *string, color_t color);

//@}

//------------------------------------------------------------------------
//      A N T E R I O R / P O S T E R I O R  L I N E S
//------------------------------------------------------------------------

/// \name Anterior/Posterior lines
//@{

/// Add anterior/posterior line(s).
/// This is low level function. Use describe() or add_long_cmt() instead.
/// \param ea      linear address
/// \param prefix  prefix to use at the start of each line
/// \param isprev  do we add anterior lines? (0-no, posterior)
/// \param format  printf() style format string. may contain \\n to denote new lines.
/// \param va      parameters for format

idaman AS_PRINTF(4, 0) void ida_export describex(
        ea_t ea,
        const char *prefix,
        bool isprev,
        const char *format,
        va_list va);


/// Add anterior/posterior line(s).
/// \param ea      linear address
/// \param isprev  do we add anterior lines? (0-no, posterior)
/// \param format  printf() style format string. may contain \\n to denote new lines.

AS_PRINTF(3, 4) inline void describe(ea_t ea, bool isprev, const char *format, ...)
{
  va_list va;
  va_start(va,format);
  describex(ea,NULL,isprev,format,va);
  va_end(va);
}


/// See add_long_cmt()

idaman AS_PRINTF(3, 0) void ida_export add_long_cmt_v(
        ea_t ea,
        bool isprev,
        const char *format,
        va_list va);


/// Add anterior/posterior comment line(s).
/// \param ea      linear address
/// \param isprev  do we add anterior lines? (0-no, posterior)
/// \param format  printf() style format string. may contain \\n to denote
///                new lines. The resulting string should not contain comment
///                characters (;), the kernel will add them automatically.

AS_PRINTF(3, 4) inline void add_long_cmt(ea_t ea, bool isprev, const char *format, ...)
{
  va_list va;
  va_start(va,format);
  add_long_cmt_v(ea, isprev, format, va);
  va_end(va);
}


/// Add anterior/posterior comment line(s) at the start of program.
/// \param format  printf() style format string. may contain \\n to denote
///                new lines. The resulting string should not contain comment
///                characters (;), the kernel will add them automatically.

AS_PRINTF(1, 2) inline void add_pgm_cmt(const char *format, ...)
{
  va_list va;
  va_start(va,format);
  add_long_cmt_v(inf.minEA, true, format, va);
  va_end(va);
}

//@}

///---------------------------------------------------------------------\cond
///         The following functions are used in kernel only:

typedef ssize_t idaapi ml_getcmt_t(color_t *cmttype, char *buf, size_t bufsize);
typedef ssize_t idaapi ml_getnam_t(color_t *namtype, char *buf, size_t bufsize);
typedef bool    idaapi ml_genxrf_t(void); // returns: overflow
typedef bool    idaapi ml_saver_t(const char *line); // returns: overflow

idaman void ida_export setup_makeline(
        ea_t ea,                                // address to generate lines for
        const char *prefix,
        ml_getcmt_t *getcmt,
        ml_getnam_t *getnam,
        ml_genxrf_t *genxrf,
        ml_saver_t *saver,
        int makeline_flags);
#define MAKELINE_NONE           0x00
#define MAKELINE_BINPREF        0x01    // allow display of binary prefix
#define MAKELINE_VOID           0x02    // allow display of '<suspicious>' marks
#define MAKELINE_STACK          0x04    // allow display of sp trace prefix

idaman bool ida_export save_line_in_array(const char *line);      // a standard line saver()
idaman void ida_export init_lines_array(char *lnar[],int maxsize);// initialization function for it

// if keep_makeline_inited, then simply generate the pending comment lines
// returns number of generated lines
idaman int ida_export finish_makeline(bool keep_makeline_inited=false);

idaman int ida_export generate_disassembly(
                                // Generate disassembly (many lines)
                                // and put them into a buffer
                                // Returns number of generated lines
        ea_t ea,                // address to generate disassembly for
        char *lines[],          // buffer to hold pointer to generated lines
        int bufsize,            // size of buffer
        int *lnnum,             // number of "the most interesting" line
                                // may be NULL
        bool as_stack);         // Display undefined items as 2/4/8 bytes

idaman bool ida_export generate_disasm_line(
                                // Generate one line of disassembly
                                // This function discards all "non-interesting" lines
                                // It is designed to generate one-line descriptions
                                // of addresses for lists, etc.
        ea_t ea,                // address to generate disassembly for
        char *buf,              // pointer to the output buffer
        size_t bufsize,         // size of the output buffer
        int flags=0);
#define GENDSM_FORCE_CODE 1     // generate a disassembly line as if
                                // there is an instruction at 'ea'
#define GENDSM_MULTI_LINE 2     // if the instruction consists of several lines,
                                // produce all of them (useful for parallel instructions)

// Get pointer to the sequence of characters denoting 'close comment'
// empty string means no comment (the current assembler has no open-comment close-comment pairs)
// This function uses ash.cmnt2

idaman const char *ida_export closing_comment(void);


// Generate the closing comment if any
//      ptr - pointer to the output buffer
//      end - the end of the output buffer
// returns: pointer past the comment

inline char *close_comment(char *ptr, char *end)
{
  APPEND(ptr, end, closing_comment());
  return ptr;
}

// Every anterior/posterior line has its number.
// Anterior  lines have numbers from E_PREV
// Posterior lines have numbers from E_NEXT

const int E_PREV = 1000;
const int E_NEXT = 2000;

idaman int ida_export get_first_free_extra_cmtidx(ea_t ea, int start);
idaman void ida_export update_extra_cmt(ea_t ea, int what, const char *str);
idaman void ida_export del_extra_cmt(ea_t ea, int what);
idaman ssize_t ida_export get_extra_cmt(ea_t ea, int what, char *buf, size_t bufsize);
idaman void ida_export delete_extra_cmts(ea_t ea, int cmtidx);

inline void save_sourcefiles(void) {}
inline void init_lines(void) {}
inline void save_lines(void) {}
idaman ea_t ida_export align_down_to_stack(ea_t newea);
idaman ea_t ida_export align_up_to_stack(ea_t ea1, ea_t ea2=BADADDR);

// A makeline producer is a function which completes the generation
// of a line. Its usual duties are to attach indented comments, xrefs,
// void marks and similar things to the line and call the saver function.
// Actually the producer gets what you send to MakeLine as argumens.
// There are several line producers in the kernel. They are invisible outside.

typedef bool idaapi makeline_producer_t(const char *line, int indent);

///-------------------------------------------------------------------\endcond



#ifndef NO_OBSOLETE_FUNCS
idaman DEPRECATED int ida_export ExtraFree(ea_t ea, int start);
#endif

#pragma pack(pop)
#endif
