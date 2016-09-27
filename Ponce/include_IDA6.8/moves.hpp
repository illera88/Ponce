/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2015 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 */

#ifndef __MOVES_HPP
#define __MOVES_HPP

#include <sistack.hpp>

#pragma pack(push, 1)           // IDA uses 1 byte alignments!

struct graph_location_info_t
{
  double zoom;          // zoom level, 1.0 == 100%, 0 means auto position
  double orgx;          // graph origin, x coord
  double orgy;          // graph origin, y coord
  graph_location_info_t(void) : zoom(0), orgx(0), orgy(0) {}
  bool operator == (const graph_location_info_t &r) const
    { return zoom == r.zoom && orgx == r.orgx && orgy == r.orgy; }  //-V550 An odd precise comparison: zoom == r.zoom
  bool operator != (const graph_location_info_t &r) const
    { return !(*this == r); }
};

#ifndef SWIG
// Helper functions. Should not be called directly!
class curloc;
class location_t;

struct segm_move_info_t
{
  segm_move_info_t(ea_t _from = 0, ea_t _to = 0, size_t _sz = 0)
    : from(_from), to(_to), size(_sz) {}
  ea_t from, to;
  size_t size;
};
DECLARE_TYPE_AS_MOVABLE(segm_move_info_t);
typedef qvector<segm_move_info_t> segm_move_infos_t;

#define DEFINE_CURLOC_HELPERS(decl) \
decl void ida_export curloc_linkTo   (curloc &, const char *stackName);\
decl void ida_export curloc_jump_push(curloc &, bool try_to_unhide, ea_t ea, int lnnum, int x, int y);\
decl bool ida_export curloc_pop      (curloc &, bool try_tohide);\
decl bool ida_export curloc_get      (curloc &, size_t depth);\
decl bool ida_export curloc_get_entry(curloc *out, const curloc &, size_t depth); \
decl int  ida_export curloc_mark     (const curloc &, int marker,const char *title, const char *desc);\
decl ea_t ida_export curloc_markedpos(const curloc &, int *marker);\
decl bool ida_export curloc_jump     (curloc &, int marker);\
decl ssize_t ida_export curloc_markdesc(const curloc &, int marker, char *buf, size_t bufsize);\
decl void ida_export curloc_unhide_if_necessary(curloc &, ea_t ea); \
decl void ida_export curloc_hide_if_necessary(const curloc &);

#define DEFINE_LOCATION_HELPERS(decl) \
decl void ida_export location_linkTo   (location_t &, const char *name);\
decl void ida_export location_push_and_jump(location_t &, bool try_to_unhide, ea_t ea, int lnnum, int x, int y, const graph_location_info_t *gi);\
decl bool ida_export location_pop      (location_t &, bool try_tohide);\
decl bool ida_export location_get      (location_t &, size_t depth);\
decl bool ida_export location_get_entry(location_t *out, const location_t &, size_t depth); \
decl int  ida_export location_mark     (const location_t &, int marker, const char *title, const char *desc);\
decl bool ida_export location_jump     (location_t &, int marker);\

DEFINE_CURLOC_HELPERS(idaman)
DEFINE_LOCATION_HELPERS(idaman)
#else
#define DEFINE_CURLOC_HELPERS(decl)
#define DEFINE_LOCATION_HELPERS(decl)
#endif // SWIG

#define CURLOC_SISTACK_ITEMS 4
#define CURLOC_LIST "$ curlocs"

class curloc : public sistack_t
{
  void push(void);
  DEFINE_CURLOC_HELPERS(friend)
protected:
  void toup(ea_t _ea) { ea = _ea; lnnum = 0; x = 0; y = 0; flags = 0; target = BADADDR; };
public:
  ea_t ea;                // Address
  ushort x,y;             // coords on the screen
#define DEFAULT_CURSOR_Y 0xFFFF
  ushort lnnum;           // number of line for the current address
#define DEFAULT_LNNUM 0xFFFF
  ushort flags;           // unhid something?
#define UNHID_SEGM 0x0001 // unhid a segment at 'target'
#define UNHID_FUNC 0x0002 // unhid a function at 'target'
#define UNHID_AREA 0x0004 // unhid an area at 'target'
  ea_t target;

  curloc(void)                   { ea = target = BADADDR; flags = 0; x = 0; y = 0; }
  curloc(const char *stackName)  { linkTo(stackName); }
  void linkTo(const char *stackName)
    { curloc_linkTo(*this, stackName); }
  void setx(int xx)              { x  = ushort(xx); }
  void jump_push(bool try_to_unhide, ea_t _ea=BADADDR, int _lnnum=0, int _x=0, int _y=0)
    { curloc_jump_push(*this, try_to_unhide, _ea, _lnnum, _x, _y); }
  bool pop(bool try_tohide)
    { return curloc_pop(*this, try_tohide); }
  bool get(size_t depth)
    { return curloc_get(*this, depth); }
  bool get_entry(curloc *out, size_t depth) const
    { return curloc_get_entry(out, *this, depth); }
  size_t size(void) const        { return sistack_t::size()/CURLOC_SISTACK_ITEMS; }
  void copy_current_location(const curloc &loc)
  {
    ea    = loc.ea;
    lnnum = loc.lnnum;
    x     = loc.x;
    y     = loc.y;
  }

  void unhide_if_necessary(ea_t tgt) { curloc_unhide_if_necessary(*this, tgt); }
  void hide_if_necessary(void) const { curloc_hide_if_necessary(*this); }


  // Mark/unmark position
  // marker - the marked position number (1..MAX_MARK_SLOT)
  //          if specified as <=0: ask the user to select the mark slot.
  // title  - if marker<=0, then the window caption of the dialog which
  //          will appear on the screen. title==NULL will lead to the default
  //          caption: "please select a mark slot"
  // desc   - description of the marked position. If NULL, IDA will show a dialog
  //          box asking the user to enter the description.
  //          If desc is specified as "", then the specified marked position
  //          will be deleted and the remaining positions will be shifted.
  // returns used marker number (<=0 - none)
  int mark(int marker, const char *title, const char *desc) const
        { return curloc_mark(*this, marker, title, desc); }

  ea_t markedpos(int *marker) const     // get address of marked location
        { return curloc_markedpos(*this, marker); }
  bool jump(int marker)                 // Jump to marker
        { return curloc_jump(*this, marker); }
  ssize_t markdesc(int marker, char *buf, size_t bufsize) const
        { return curloc_markdesc(*this, marker, buf, bufsize); }
  static void rebase_stack(nodeidx_t node, const segm_move_infos_t &infos);
};

#define MAX_MARK_SLOT   1024     // Max number of marked locations

// Since we can not modify the curloc class (for compatibility reasons)
// we create a derived class and will exclusively use it in IDA GUI
class location_t : public curloc
{
  typedef curloc inherited;
  DEFINE_LOCATION_HELPERS(friend)
public:
  graph_location_info_t gli;
  location_t(void) {}
  location_t(const char *name) { linkTo(name); }
  void linkTo(const char *name) { location_linkTo(*this, name); }

  void push_and_jump(bool try_to_unhide, ea_t _ea=BADADDR, int _lnnum=0,
                int _x=0, int _y=0,  graph_location_info_t *_gli=NULL)
    { location_push_and_jump(*this, try_to_unhide, _ea, _lnnum, _x, _y, _gli); }
  bool pop(bool try_tohide)
    { return location_pop(*this, try_tohide); }
  bool get(size_t depth)
    { return location_get(*this, depth); }
  bool get_entry(location_t *out, size_t depth) const
    { return location_get_entry(out, *this, depth); }
  void copy_current_location(const location_t &loc)
  {
    inherited::copy_current_location(loc);
    gli = loc.gli;
  }

  void jump_nopush(/*bool try_to_unhide,*/ ea_t _ea=BADADDR, int _lnnum=0,
                int _x=0, int _y=0,  graph_location_info_t *_gli=NULL)
  {
    flags = 0;
    target = BADADDR;
    if ( _ea == BADADDR )
      return;
    //if ( try_to_unhide )
    //  unhide_if_necessary(_ea);
    ea    = _ea;
    lnnum = uint16(_lnnum);
    x = uint16(_x);
    y = uint16(_y);
    if ( _gli != NULL )
      gli = *_gli;
  }

  int mark(int marker, const char *title, const char *desc) const
    { return location_mark(*this, marker, title, desc); }
  bool jump(int marker)
    { return location_jump(*this, marker); }
};

//----------------------------------------------------------------------

#pragma pack(pop)
#endif // __MOVES_HPP

