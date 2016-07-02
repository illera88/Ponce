#ifndef IDA_HIGHLIGHTER
#define IDA_HIGHLIGHTER

#include <expr.hpp>

idaman void ida_export code_highlight_block(void *context, highlighter_cbs_t *highlighter_cbs, const qstring &text);

// returns the length of text to colorize
// negative values may have special meaning in the future.
typedef ssize_t (idaapi *external_colorizer_t)(syntax_highlight_style *color, const char *str);
typedef qvector<external_colorizer_t> external_colorizers_t;

// returns true if identifier is colorized
typedef bool (idaapi *external_ident_colorizer_t)(syntax_highlight_style *color, const qstring &ident);
typedef qvector<external_ident_colorizer_t> external_ident_colorizers_t;


#define MLTCMTMASK    0xF
#define PREPROC_FLAG  0x10

// Common implementation of syntax_highlighter_t used in different parts of IDA
//-V:ida_syntax_highlighter_t:730 not all members of a class are initialized inside the constructor
struct ida_syntax_highlighter_t : syntax_highlighter_t
{
protected:
  // keys of keywords_t point to memory from keyword_strings.
  // once allocated, this buffer won't be moved in the memory.
  qstrvec_t keyword_memory;
  // helper class so that keywords_t gets sorted by string contents, not pointer values.
  struct plain_char_ptr_t
  {
    const char *ptr;
    plain_char_ptr_t(const char *p = NULL) : ptr(p) {}
    bool operator <(const plain_char_ptr_t &r) const
    {
      return strcmp(ptr, r.ptr) < 0;
    }
    bool operator ==(const plain_char_ptr_t &r) const
    {
      return strcmp(ptr, r.ptr) == 0;
    }
  };
  struct multicmt_t
  {
    qstring open_multicmt;
    qstring close_multicmt;
    multicmt_t() {}
    multicmt_t(const char *_open_multicmt, const char *_close_multicmt) :
      open_multicmt(_open_multicmt),
      close_multicmt(_close_multicmt)
    {}
  };

  // typedef std::map<plain_char_ptr_t, syntax_highlight_style> keywords_t;
  struct keywords_style_t
  {
    qvector<plain_char_ptr_t> keywords;
    syntax_highlight_style style;
  };
  typedef qvector<keywords_style_t> keywords_t;
  //typedef qvector<plain_char_ptr_t> keywords_t;
  typedef qvector<multicmt_t> multicmtvec_t;
  keywords_t keywords;

  qstring open_cmt;             // string that opens a regular line comment
  multicmtvec_t multicmts;
  char literal_closer;          // either close_strconst or close_chrconst for the current literal

  // color mappings
  syntax_highlight_style text_color;
  syntax_highlight_style comment_color;
  syntax_highlight_style string_color;
  syntax_highlight_style preprocessor_color;

  external_colorizers_t external_colorizers;
  external_ident_colorizers_t external_ident_colorizers;

  // work variables
  const char *input;                  // pointer to the start of the input buffer
  const char *pending;                // pointer in the input buffer
  syntax_highlight_style style;       // current highlighting style

  bool pending_nonspaces_present(const char *end)
  {
    for ( const char *p = pending; p != end; ++p )
      if ( !qisspace(*p) )
        return true;
    return false;
  }

  const char *parse_literal_const(highlighter_cbs_t *highlighter_cbs, const char *ptr, char literal_closer);
  void flush_output(highlighter_cbs_t *highlighter_cbs, int32 len, syntax_highlight_style style);
  void handle_cmt(highlighter_cbs_t *highlighter_cbs, int mcmt_idx, const char **ptr);
  void handle_preproc(highlighter_cbs_t *highlighter_cbs, const char **ptr);

public:
  // if any of the following features is not needed, just zero them out:
  char open_strconst;           // character that opens a string constant
  char close_strconst;          // character that closes a string constant
  char open_chrconst;           // character that closes a character constant
  char close_chrconst;          // character that opens a character constant
  char escape_char;             // backslash
  char preprocessor_char;       // #

  ida_syntax_highlighter_t()
  {
    highlight_block = &code_highlight_block;
    text_color = HF_DEFAULT;
    comment_color = HF_COMMENT;
    string_color = HF_STRING;
    preprocessor_color = HF_PREPROC;
    style = HF_DEFAULT;
  }

  void highlight_block_ex(highlighter_cbs_t *highlighter_cbs, const qstring &text);
  void add_external_colorizer(external_colorizer_t th) { external_colorizers.push_back(th); }
  void add_external_ident_colorizer(external_ident_colorizer_t th) { external_ident_colorizers.push_back(th); }
  void set_open_cmt(const char *begin) { open_cmt = begin; }
  void add_multi_line_comment(const char *begin, const char *end)
  {
    multicmt_t &mcmt = multicmts.push_back();
    mcmt.open_multicmt = begin;
    mcmt.close_multicmt = end;
  }

  void add_keywords(const char *kwstr, syntax_highlight_style _style)
  {
    char *ctx;
    // in order not to allocate every keyword separately, we allocate the whole
    // kwstr string at once and will just store pointers to it in the map.
    qstring &mem = keyword_memory.push_back();
    mem = kwstr;
    keywords_style_t *pair_p = NULL;
    for ( int i = 0; i < keywords.size(); i++ )
    {
      if ( keywords[i].style == _style )
      {
        pair_p = &keywords[i];
        break;
      }
    }
    if ( pair_p == NULL )
    {
      keywords_style_t &pair = keywords.push_back();
      pair_p = &pair;
      pair_p->style = _style;
    }
    for ( char *kw = qstrtok(mem.begin(), "|", &ctx); kw != NULL; kw = qstrtok(NULL, "|", &ctx) )
      pair_p->keywords.push_back(kw);
  }
};


#endif //IDA_HIGHLIGHTER
