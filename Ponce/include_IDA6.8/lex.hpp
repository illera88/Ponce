/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2015 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 */

#ifndef LEX_HPP
#define LEX_HPP
#pragma pack(push, 1)   // IDA uses 1 byte alignments!

/*! \file lex.hpp

  \brief Tools for parsing C-like input

  Functions in this file use objects of opaque type lexer_t.
  To create a lexer_t instance, use create_lexer().
*/

typedef ushort lxtype;  ///< see \ref lx_

/// \defgroup lx_ Parser token types
/// See token_t::type
/// \note All separators have their ASCII codes as lxtype
//@{
const lxtype
  lx_end      = 1,      ///< no more tokens
  lx_ident    = 2,      ///< ident
  lx_number   = 3,      ///< long constant
  lx_string   = 4,      ///< string constant (token_t.chr != 0 => unicode string)
  lx_char     = 5,      ///< char constant
  lx_typename = 6,      ///< user-defined type
  lx_float    = 7,      ///< IEEE floating point constant
  lx_int64    = 8,      ///< int64 constant
  lx_key      = 128;    ///< keywords start. All keys are lx_key + keynum. \n
                        ///< Two-char separators are: (c1 + (c2 << 8)).    \n
                        ///< Three-char separators:
                        ///<   - "<<=" = ('<' + ('<'<<8)) + '='
                        ///<   - ">>=" = ('>' + ('>'<<8)) + '='
//@}


/// Parser token
struct token_t
{
  lxtype type;          ///< see \ref lx_
  char str[MAXSTR];     ///< idents & strings
  sval_t num;           ///< long & char constants
  bool unicode;         ///< (::lx_string: != 0 => unicode string)
  union
  {
    ushort fnum[6];     ///< floating point constant
    size_t slen;        ///< ::lx_string: string length
    int64 i64;          ///< ::lx_int64
  };
};
DECLARE_TYPE_AS_MOVABLE(token_t);

class lexer_t;          // lexical analyzer, opaque structure


/// Preprocessor callback for unknown tokens.
/// Will be called when preprocessor calculates the value of #if expression.

typedef error_t lx_resolver_t(lexer_t *lx, void *ud, token_t *curtok, sval_t *res);


/// Preprocessor callback.
/// It will be called for each input line.
/// \return an error code (0-ok)

typedef int idaapi lx_preprocessor_cb(void *ud, const char *fname, int nl, char *line);


/// Callback for #pragma directives.
/// \return an error code (0-ok)

typedef int idaapi lx_pragma_cb(void *ud, const char *line);


/// Callback for #warning directives.
/// \return an error code (0-ok)

typedef int idaapi lx_warning_cb(void *ud, const char *line);


/// Callback for #define directives
/// \return an error code (0-ok)

typedef int idaapi lx_macro_cb(
        void *ud,
        const char *name,
        const char *body,
        int nargs,
        bool isfunc,
        bool is_new_macro);


/// Create new lexical analyzer and set its keyword table.
/// If keys==NULL, then set the default C keyword table

idaman lexer_t *ida_export create_lexer(const char *const *keys,
                                        size_t size,
                                        void *ud=NULL);


/// Destroy a lexical analyzer

idaman void ida_export destroy_lexer(lexer_t *lx);


/// Define a macro

idaman error_t ida_export lex_define(lexer_t *lx,
                                     const char *macro,
                                     const char *body,
                                     int nargs=0,
                                     bool isfunc=false);

/// Undefine a macro

idaman void ida_export lex_undef(lexer_t *lx,
                                 const char *macro);

/// Set lexer options.
/// \param options  \ref LXOPT_
/// \return the old options

idaman int ida_export lex_set_options(lexer_t *lx, int options);

/// \defgroup LXOPT_ Lexer options
/// Passed as 'options' parameter to lex_set_options().
/// By default all options are on.
//@{
#define LXOPT_PARSE_FLOATS  0x0001 ///< enable floating point constants
#define LXOPT_REQ_SEPARATOR 0x0002 ///< require a separator between a number and an ident or a character/string constant or dot
//@}


/// Get next token

idaman error_t ida_export lxget(lexer_t *lx, token_t *t);


/// Enumerate all macros.
/// Do so until 'cb' returns non-zero.

idaman int ida_export lex_enum_macros(
        const lexer_t *lx,
        int idaapi cb(const char *name, const char *body, int nargs, bool isfunc, void *ud),
        void *ud);


/// Debug: get text representation of token

idaman const char *ida_export lxascii(const token_t *t, char *buf, size_t bufsize);


//-------------------------------------------------------------------------
/// \name String oriented functions
//@{
struct macro_t;


/// Set the input line and the macro table.
/// if macros==NULL, the macro table will not be changed.

idaman error_t ida_export lxini(
        lexer_t *lx,
        const char *line,
        macro_t **macros=NULL);
//@}

//-------------------------------------------------------------------------
/// \name File oriented functions
//@{

/// Initialization: file may be NULL.
/// Also see lxgetstrm().

idaman error_t ida_export lxgetsini(lexer_t *lx, const char *file);


/// Error handling.
/// if level > 0, then return information about the enclosing file which
/// included the current one.

idaman const char *ida_export lxgetserr(
        lexer_t *lx,
        int32 *linenum,
        const char **lineptr,
        int level=0);


/// Termination: also see lxgetsini()

idaman void ida_export lxgetstrm(lexer_t *lx, bool del_macros);
//@}

//-------------------------------------------------------------------------
/// \name Token stack
//@{
typedef qstack<token_t> tokenstack_t; ///< see get_token(), unget_token()


/// Retrieve token from a stack or lexer.
/// If buf is not empty then get the token on top of the stack.
/// If buf is empty then gen the next token from the lexer.
/// \return success

inline bool get_token(token_t *t, lexer_t *lx, tokenstack_t &buf)
{
  if ( !buf.empty() )
    *t = buf.pop();
  else if ( lxget(lx, t) != eOk )
    return false;
  return true;
}


/// Push a token back onto the token stack

inline void unget_token(const token_t &t, tokenstack_t &buf)
{
  buf.push(t);
}
//@}

//-------------------------------------------------------------------------
#ifndef NO_OBSOLETE_FUNCS
typedef const char *idaapi lxgets_t(lexer_t *lx, error_t *res);
idaman DEPRECATED const char *ida_export lxgets(lexer_t *lx, error_t *res);
idaman DEPRECATED error_t ida_export line_lxget(lexer_t *lx, token_t *t);
#endif

#pragma pack(pop)
#endif  //  LEX_HPP
