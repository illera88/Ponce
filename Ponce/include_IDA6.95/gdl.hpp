/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2015 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 *
 *      Graph drawing support
 *
 */

#ifndef __GDLDRAW_HPP
#define __GDLDRAW_HPP

#include <map>
#include <set>

#include <area.hpp>
#include <funcs.hpp>

/*! \file gdl.hpp

  \brief Low level graph drawing operations

*/

//-------------------------------------------------------------------------
// forward declarations:
class node_iterator;
class qflow_chart_t;
class gdl_graph_t;

/// Flow chart block types
enum fc_block_type_t
{
  fcb_normal,    ///< normal block
  fcb_indjump,   ///< block ends with indirect jump
  fcb_ret,       ///< return block
  fcb_cndret,    ///< conditional return block
  fcb_noret,     ///< noreturn block
  fcb_enoret,    ///< external noreturn block (does not belong to the function)
  fcb_extern,    ///< external normal block
  fcb_error,     ///< block passes execution past the function end
};

#ifndef SWIG
#define DECLARE_HELPER(decl)                                        \
decl node_iterator *ida_export node_iterator_goup(node_iterator *); \
decl void ida_export create_qflow_chart(qflow_chart_t &);           \
decl bool ida_export append_to_flowchart(qflow_chart_t &, ea_t, ea_t); \
decl fc_block_type_t ida_export fc_calc_block_type(const qflow_chart_t &, size_t); \
decl bool ida_export create_multirange_qflow_chart(qflow_chart_t &, const areavec_t &);
#else
#define DECLARE_HELPER(decl)
#endif // SWIG

DECLARE_HELPER(idaman)

//-------------------------------------------------------------------------
/// Ordered sequence of integer constants
class intseq_t : public intvec_t
{
public:
  int idaapi index(int value) const
  {
    for ( size_t i=0; i < size(); i++ )
      if ( (*this)[i] == value )
        return i;
    return -1;
  }
  bool idaapi contains(int value) const { return index(value) != -1; }
  void idaapi add(int value) { push_back(value); }
  bool idaapi add_unique(int value)
  {
    if ( contains(value) )
      return false;
    add(value);
    return true;
  }
  void idaapi add_unique(const intseq_t &s);
  int  idaapi get(void) { int v = back(); pop_back(); return v; }
  size_t idaapi print(char *buf, size_t bufsize) const;
  const char *idaapi dstr(void) const;
  void idaapi add_block(int nb); // insert a block before 'nb'
  void idaapi move_block(int from, int to); // move block 'from' just before 'to'
  void idaapi remove_block(int from, int to);
};

/// Set of integer constants
class intset_t : public std::set<int>
{
public:
  DEFINE_MEMORY_ALLOCATION_FUNCS()
  size_t idaapi print(char *buf, size_t bufsize) const;
  const char *idaapi dstr(void) const;
  bool has(int value) const
  {
    const_iterator p = find(value);
    const_iterator q = end();
    return p != q;
  }
};

typedef qvector<intseq_t> array_of_intseq_t;

/// Map of integer constants to integer constants
class intmap_t : public std::map<int, int>
{
public:
  DEFINE_MEMORY_ALLOCATION_FUNCS()
  size_t idaapi print(char *buf, size_t bufsize) const;
  const char *idaapi dstr(void) const;
};

typedef qvector<intmap_t> array_of_intmap_t;

//-------------------------------------------------------------------------
/// Set of graph nodes
class node_set_t : public intset_t
{
public:
  idaapi node_set_t(void) {}
  idaapi node_set_t(int node) { insert(node); }
  idaapi node_set_t(const gdl_graph_t *g);
  bool idaapi add(int node) { return insert(node).second; }
  void idaapi sub(int node) { erase(node); }
  void idaapi sub(const node_set_t &r);
  void idaapi add(const node_set_t &r);
  void idaapi intersect(const node_set_t &r);
  void idaapi extract(intseq_t &out) const;
  int  idaapi first(void) const { return empty() ? -1 : *begin(); }
};

typedef qvector<node_set_t> array_of_node_set_t;

//-------------------------------------------------------------------------
/// Node iterator (used to draw graphs)
class node_iterator
{
  DECLARE_HELPER(friend)
  friend class gdl_graph_t;
  const gdl_graph_t *g;
  int i;
  node_iterator &_goup(void);
  node_iterator &goup(void) { return *node_iterator_goup(this); }
public:
  node_iterator(const gdl_graph_t *_g, int n) : g(_g), i(n) {}
  node_iterator &operator++(void) { i++; return goup(); }
  bool operator==(const node_iterator &n) const { return i == n.i && g == n.g; }
  bool operator!=(const node_iterator &n) const { return !(*this == n); }
  int operator*(void) const { return i; }
};

//-------------------------------------------------------------------------
/// gdl graph interface - includes only functions required to draw it
class gdl_graph_t
{
  // does a path from 'm' to 'n' exist?
  bool idaapi path(node_set_t &visited, int m, int n) const;
public:
  DEFINE_MEMORY_ALLOCATION_FUNCS()
  DEFINE_VIRTUAL_DTOR(gdl_graph_t)
  virtual char *idaapi get_node_label(int /*n*/, char *buf, int /*bufsize*/) const { buf[0] = '\0'; return buf; }
  virtual void idaapi print_graph_attributes(FILE * /*fp*/) const {}
  virtual bool idaapi print_node(FILE * /*fp*/, int /*n*/) const { return false; }
  virtual bool idaapi print_edge(FILE * /*fp*/, int /*i*/, int /*j*/) const { return false; }
  virtual void idaapi print_node_attributes(FILE * /*fp*/, int /*n*/) const {}
  virtual int  idaapi size(void) const = 0;                    // number of the max node number
  virtual int  idaapi node_qty(void) const { return size(); }  // number of alive nodes
  virtual bool idaapi exists(int /*node*/) const { return true; }
  virtual int  idaapi entry(void) const { return 0; }
  virtual int  idaapi exit(void) const { return size()-1; }
  virtual int  idaapi nsucc(int node) const = 0;
  virtual int  idaapi npred(int node) const = 0;
  virtual int  idaapi succ(int node, int i) const = 0;
  virtual int  idaapi pred(int node, int i) const = 0;
  virtual bool idaapi empty(void) const { return node_qty() == 0; }
  virtual bgcolor_t idaapi get_node_color(int /*n*/) const { return DEFCOLOR; }
  virtual bgcolor_t idaapi get_edge_color(int /*i*/, int /*j*/) const { return DEFCOLOR; }
          void idaapi gen_gdl(FILE *fp) const;
          void idaapi gen_gdl(const char *file) const;
          size_t idaapi nedge(int node, bool ispred) const { return ispred ? npred(node) : nsucc(node); }
          int  idaapi edge(int node, int i, bool ispred) const { return ispred ? pred(node, i) : succ(node, i); }
          int  idaapi front(void) { return *begin(); }
  node_iterator idaapi begin(void) const { return node_iterator(this, 0).goup(); }
  node_iterator idaapi end(void)   const { return node_iterator(this, size()); }
  // does a path from 'm' to 'n' exist?
  bool idaapi path_exists(int m, int n) const { node_set_t v; return path(v, m, n); }

  void idaapi gen_dot(FILE *fp) const;
  void idaapi gen_dot(const char *file) const;
};


/// Create GDL file for graph

idaman void ida_export gen_gdl(const gdl_graph_t *g, const char *fname);


/// Display GDL file by calling wingraph32.
/// The exact name of the grapher is taken from the configuration file
/// and set up by setup_graph_subsystem().
/// \return error code from os, 0 if ok

idaman int ida_export display_gdl(const char *fname);


//-------------------------------------------------------------------------
// Build and display program graphs

/// Build and display a flow graph.
/// \param filename  output file name. the file extension is not used. maybe NULL.
/// \param title     graph title
/// \param pfn       function to graph
/// \param ea1, ea2  if pfn == NULL, then the address range
/// \param gflags    combination of \ref CHART_1.
///                  if none of #CHART_GEN_DOT, #CHART_GEN_GDL, #CHART_WINGRAPH
///                  is specified, the function will return false
/// \return success. if fails, a warning message is displayed on the screen

idaman bool ida_export gen_flow_graph(
        const char *filename,
        const char *title,
        func_t *pfn,
        ea_t ea1, ea_t ea2,
        int gflags);

/// \defgroup CHART_1 Flow graph building flags
/// Passed as flags parameter to:
///   - gen_flow_graph()
///   - gen_simple_call_chart()
///   - gen_complex_call_chart()
//@{
#define CHART_PRINT_NAMES 0x1000 ///< print labels for each block?
#define CHART_GEN_DOT     0x2000 ///< generate .dot file (file extension is forced to .dot)
#define CHART_GEN_GDL     0x4000 ///< generate .gdl file (file extension is forced to .gdl)
#define CHART_WINGRAPH    0x8000 ///< call grapher to display the graph
//@}


/// Format to generate when not specified in the flags. (e.g. called from UI).
/// the default is set in ida.cfg.
/// 0 - GDL, 1 - DOT.
extern int default_graph_format;


/// Build and display a simple function call graph.
/// \param filename  output file name. the file extension is not used. maybe NULL.
/// \param wait      message to display during graph building
/// \param title     graph title
/// \param gflags    combination of #CHART_NOLIBFUNCS and \ref CHART_1.
///                  if none of #CHART_GEN_DOT, #CHART_GEN_GDL, #CHART_WINGRAPH
///                  is specified, the function will return false.
/// \return success. if fails, a warning message is displayed on the screen

idaman bool ida_export gen_simple_call_chart(
        const char *filename,
        const char *wait,
        const char *title,
        int gflags);


/// Build and display a complex xref graph.
/// \param filename         output file name. the file extension is not used. maybe NULL.
/// \param wait             message to display during graph building
/// \param title            graph title
/// \param ea1, ea2         address range
/// \param flags            combination of \ref CHART_2 and \ref CHART_1.
///                         if none of #CHART_GEN_DOT, #CHART_GEN_GDL, #CHART_WINGRAPH
///                         is specified, the function will return false.
/// \param recursion_depth  optional limit of recursion
/// \return success. if fails, a warning message is displayed on the screen

idaman bool ida_export gen_complex_call_chart(
        const char *filename,
        const char *wait,
        const char *title,
        ea_t ea1,
        ea_t ea2,
        int flags,
        int32 recursion_depth=-1);

/// \defgroup CHART_2 Call chart building flags
/// Passed as flags parameter to gen_complex_call_chart()
//@{
#define CHART_NOLIBFUNCS       0x0400 ///< don't include library functions in the graph
#define CHART_REFERENCING      0x0001 ///< references to the addresses in the list
#define CHART_REFERENCED       0x0002 ///< references from the addresses in the list
#define CHART_RECURSIVE        0x0004 ///< analyze added blocks
#define CHART_FOLLOW_DIRECTION 0x0008 ///< analyze references to added blocks only in the direction of the reference who discovered the current block
#define CHART_IGNORE_XTRN      0x0010
#define CHART_IGNORE_DATA_BSS  0x0020
#define CHART_IGNORE_LIB_TO    0x0040 ///< ignore references to library functions
#define CHART_IGNORE_LIB_FROM  0x0080 ///< ignore references from library functions
#define CHART_PRINT_COMMENTS   0x0100
#define CHART_PRINT_DOTS       0x0200 ///< print dots if xrefs exist outside of the range recursion depth
//@}


/// Setup the user-defined graph colors and graph viewer program.
/// This function is called by the GUI at the beginning, so no need to call
/// it again.

idaman void ida_export setup_graph_subsystem(const char *_grapher, bgcolor_t (idaapi *get_graph_color)(int color));


//--------------------------------------------------------------------------
//
//      The following definitions are for the kernel and the user interface only.
//      Third party plugins or modules should not use them.
//
/// \cond
//-V:cancellable_graph_t:730 not all members of a class are initialized inside the constructor
class cancellable_graph_t : public gdl_graph_t
{
public:
  mutable bool cancelled;
  char padding[3]; // make the class nicely aligned. otherwise we have problems
                   // with gcc in qflow_chart_t.
  cancellable_graph_t(void) : cancelled(false) {}
  DEFINE_VIRTUAL_DTOR(cancellable_graph_t)
  bool idaapi check_cancel(void) const;
};

//--------------------------------------------------------------------------
struct qbasic_block_t : public area_t
{
  intseq_t succ; // list of node successors
  intseq_t pred; // list of node predecessors
};

inline bool is_noret_block(fc_block_type_t btype)
{
  return btype == fcb_noret || btype == fcb_enoret;
}

inline bool is_ret_block(fc_block_type_t btype)
{
  return btype == fcb_ret || btype == fcb_cndret;
}

class qflow_chart_t : public cancellable_graph_t
{
public:
  typedef qvector<qbasic_block_t> blocks_t;
  DECLARE_HELPER(friend)
  qstring title;
  area_t bounds;
  func_t *pfn;
  int flags;
#define FC_PRINT     0x0001 // print names (used only by display_flow_chart())
#define FC_NOEXT     0x0002 // do not compute external blocks
#define FC_PREDS     0x0004 // compute predecessor lists
#define FC_APPND     0x0008 // multirange flowchart (set by append_to_flowchart)
#define FC_CHKBREAK  0x0010 // build_qflow_chart() may be aborted by user
  blocks_t blocks;
  int nproper;          // number of basic blocks belonging to the specified area

  idaapi qflow_chart_t(void) : pfn(NULL), flags(0), nproper(0) {}
  idaapi qflow_chart_t(const char *_title, func_t *_pfn, ea_t _ea1, ea_t _ea2, int _flags)
    : title(_title), bounds(_ea1, _ea2), pfn(_pfn), flags(_flags), nproper(0)
  {
    refresh();
  }
  DEFINE_VIRTUAL_DTOR(qflow_chart_t)
  void idaapi create(const char *_title, func_t *_pfn, ea_t _ea1, ea_t _ea2, int _flags)
  {
    title  = _title;
    pfn    = _pfn;
    bounds = area_t(_ea1, _ea2);
    flags  = _flags;
    refresh();
  }
  void idaapi create(const char *_title, const areavec_t &ranges, int _flags)
  {
    title  = _title;
    flags  = _flags;
    create_multirange_qflow_chart(*this, ranges);
  }
  void idaapi append_to_flowchart(ea_t ea1, ea_t ea2) { ::append_to_flowchart(*this, ea1, ea2); }
  void idaapi refresh(void) { create_qflow_chart(*this); }
  fc_block_type_t calc_block_type(size_t blknum) const
    { return fc_calc_block_type(*this, blknum); }
  bool is_ret_block(size_t blknum) const { return ::is_ret_block(calc_block_type(blknum)); }
  bool is_noret_block(size_t blknum) const { return ::is_noret_block(calc_block_type(blknum)); }
  void idaapi print_node_attributes(FILE * /*fp*/, int /*n*/) const {}
  int  idaapi nsucc(int node) const { return int(blocks[node].succ.size()); }
  int  idaapi npred(int node) const { return int(blocks[node].pred.size()); } // note: specify FC_PREDS for preds!
  int  idaapi succ(int node, int i) const { return blocks[node].succ[i]; }
  int  idaapi pred(int node, int i) const { return blocks[node].pred[i]; } // note: specify FC_PREDS for preds!
  bool idaapi print_names(void) const { return (flags & FC_PRINT) != 0; }
  char *idaapi get_node_label(int /*n*/, char * /*buf*/, int /*bufsize*/) const { return NULL; }
  int  idaapi size(void) const { return int(blocks.size()); }
};
/// \endcond

//--------------------------------------------------------------------------
// The definitions below are obsolete and should not be used
#ifndef NO_OBSOLETE_FUNCS
idaman DEPRECATED bool ida_export display_flow_graph(const char *title,
                                          func_t *pfn,
                                          ea_t ea1, ea_t ea2,
                                          bool print_names);
idaman DEPRECATED bool ida_export display_simple_call_chart(const char *wait,
                                                 const char *title,
                                                 bool ignore_libfuncs);
idaman DEPRECATED bool ida_export display_complex_call_chart(const char *wait,
                                                  const char *title,
                                                  ea_t ea1,
                                                  ea_t ea2,
                                                  int flags,
                                                  int32 recursion_depth=-1);

#endif // NO_OBSOLETE_FUNCS

#endif // __GDLDRAW_HPP
