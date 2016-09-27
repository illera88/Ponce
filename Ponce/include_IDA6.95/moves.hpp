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

struct segm_move_info_t
{
  segm_move_info_t(ea_t _from = 0, ea_t _to = 0, size_t _sz = 0)
    : from(_from), to(_to), size(_sz) {}
  ea_t from, to;
  size_t size;

  bool operator == (const segm_move_info_t &r) const
    { return from == r.from && to == r.to && size == r.size; }
  bool operator != (const segm_move_info_t &r) const
    { return !(*this == r); }
};
DECLARE_TYPE_AS_MOVABLE(segm_move_info_t);
typedef qvector<segm_move_info_t> segm_move_info_vec_t;

struct segm_move_infos_t : public segm_move_info_vec_t
{
  const segm_move_info_t *find(ea_t ea) const
  {
    for ( size_t i = 0; i < size(); ++i )
    {
      const segm_move_info_t &cur = at(i);
      if ( ea >= cur.from && ea < cur.from + cur.size )
        return &cur;
    }
    return NULL;
  }
};


#ifndef SWIG
// Helper functions. Should not be called directly!
class curloc;
class location_t;

#define DEFINE_CURLOC_HELPERS(decl) \
decl void ida_export curloc_linkTo(curloc &, const char *stackName);\
decl void ida_export curloc_jump_push(curloc &, bool try_to_unhide, ea_t ea, int lnnum, int x, int y);\
decl bool ida_export curloc_pop(curloc &, bool try_tohide);\
decl bool ida_export curloc_get(curloc &, size_t depth);\
decl bool ida_export curloc_get_entry(curloc *out, const curloc &, size_t depth); \
decl int ida_export curloc_mark(const curloc &, int marker,const char *title, const char *desc);\
decl ea_t ida_export curloc_markedpos(const curloc &, int *marker);\
decl bool ida_export curloc_jump(curloc &, int marker);\
decl ssize_t ida_export curloc_markdesc(const curloc &, int marker, char *buf, size_t bufsize);\
decl void ida_export curloc_unhide_if_necessary(curloc &, ea_t ea); \
decl void ida_export curloc_hide_if_necessary(const curloc &);

#define DEFINE_LOCATION_HELPERS(decl) \
decl void ida_export location_linkTo(location_t &, const char *name);\
decl void ida_export location_push_and_jump(location_t &, bool try_to_unhide, ea_t ea, int lnnum, int x, int y, const graph_location_info_t *gi);\
decl bool ida_export location_pop(location_t &, bool try_tohide);\
decl bool ida_export location_get(location_t &, size_t depth);\
decl bool ida_export location_get_entry(location_t *out, const location_t &, size_t depth); \
decl int ida_export location_mark(const location_t &, int marker, const char *title, const char *desc);\
decl bool ida_export location_jump(location_t &, int marker);\

DEFINE_CURLOC_HELPERS(idaman)
DEFINE_LOCATION_HELPERS(idaman)
#else
#define DEFINE_CURLOC_HELPERS(decl)
#define DEFINE_LOCATION_HELPERS(decl)
#endif // SWIG

#define CURLOC_SISTACK_ITEMS 4
#define CURLOC_LIST "$ curlocs"

//-V:curloc:730 Not all members of a class are initialized inside the constructor
class curloc : public sistack_t
{
  void push(void);
  DEFINE_CURLOC_HELPERS(friend)
protected:
  void toup(ea_t _ea) { ea = _ea; lnnum = 0; x = 0; y = 0; flags = 0; target = BADADDR; }
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

  curloc(void)                   { ea = target = BADADDR; flags = 0; x = 0; y = 0; lnnum = 0; }
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
  bool is_compat;
public:
  graph_location_info_t gli;
  location_t(void) : is_compat(false) {}
  location_t(bool _compat) : is_compat(_compat) {}
  location_t(const char *name) : is_compat(false) { linkTo(name); }
  void linkTo(const char *name) { location_linkTo(*this, name); }

  void push_and_jump(bool try_to_unhide, ea_t _ea=BADADDR, int _lnnum=0,
                int _x=0, int _y=0, graph_location_info_t *_gli=NULL)
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
                int _x=0, int _y=0, graph_location_info_t *_gli=NULL)
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

//-------------------------------------------------------------------------
class place_t;

//-------------------------------------------------------------------------
struct renderer_info_t
{
  renderer_info_t() { clear(); }
  graph_location_info_t gli;
  struct pos_t
  {
    int node;
    short cx;
    short cy;

    pos_t() : node(-1), cx(-1), cy(-1) {}
    bool operator == (const pos_t &r) const
      { return node == r.node && cx == r.cx && cy == r.cy; }
    bool operator != (const pos_t &r) const
      { return !(*this == r); }
  };
  pos_t pos;
  tcc_renderer_type_t rtype;

  bool operator == (const renderer_info_t &r) const
    { return rtype == r.rtype && pos == r.pos && gli == r.gli; }
  bool operator != (const renderer_info_t &r) const
    { return !(*this == r); }

  void clear()
  {
    memset(this, 0, sizeof(*this));
    rtype = TCCRT_INVALID;
  }
};


//-------------------------------------------------------------------------
class lochist_t;
struct lochist_entry_t;
struct expanded_area_t;

#define LSEF_PLACE (1 << 0)
#define LSEF_RINFO (1 << 1)
#define LSEF_PTYPE (1 << 2)
#define LSEF_ALL   (LSEF_PLACE|LSEF_RINFO|LSEF_PTYPE)

#ifndef SWIG
#define DEFINE_LOCHIST_T_HELPERS(decl) \
  decl void ida_export lochist_t_register_live(lochist_t &);            \
  decl void ida_export lochist_t_deregister_live(lochist_t &);          \
  decl bool ida_export lochist_t_init     (lochist_t &, const char *, const place_t &, void *, uint32); \
  decl void ida_export lochist_t_jump     (lochist_t &, bool try_to_unhide, const lochist_entry_t &e); \
  decl bool ida_export lochist_t_fwd      (lochist_t &, uint32 cnt, bool try_to_unhide);   \
  decl bool ida_export lochist_t_back     (lochist_t &, uint32 cnt, bool try_to_unhide);   \
  decl bool ida_export lochist_t_seek     (lochist_t &, uint32 index, bool try_to_unhide, bool apply_cur); \
  decl const lochist_entry_t *\
            ida_export lochist_t_get_current(const lochist_t &);                  \
  decl uint32 ida_export lochist_t_current_index(const lochist_t &);        \
  decl void ida_export lochist_t_set      (lochist_t &, uint32, const lochist_entry_t &); \
  decl bool ida_export lochist_t_get      (lochist_entry_t *, const lochist_t &, uint32); \
  decl uint32 ida_export lochist_t_size   (const lochist_t &);\
  decl void ida_export lochist_t_save     (const lochist_t &); \
  decl void ida_export lochist_t_clear    (lochist_t &);
#else
#define DEFINE_LOCHIST_T_HELPERS(decl)
#endif // SWIG

DEFINE_LOCHIST_T_HELPERS(idaman)

//-------------------------------------------------------------------------
struct lochist_entry_t
{
  renderer_info_t rinfo;
  place_t *plce;

  lochist_entry_t() : plce(NULL) {}
  lochist_entry_t(const place_t *p, const renderer_info_t &r)
    : rinfo(r), plce((place_t *) p)
  {
    if ( plce != NULL )
      plce = plce->clone();
  }
#ifndef SWIG
  lochist_entry_t(const lochist_t &s);
#endif // SWIG
  lochist_entry_t(const lochist_entry_t &other) : plce(NULL) { *this = other; }
  ~lochist_entry_t() { clear(); }
  const renderer_info_t &renderer_info() const { return rinfo; }
  const place_t *place() const { return plce; }

  renderer_info_t &renderer_info() { return rinfo; }
  place_t *place() { return plce; }
  void set_place(const place_t *p) { clear(); plce = p == NULL ? NULL : p->clone(); }
  void set_place(const place_t &p) { set_place(&p); }

  bool is_valid() const { return plce != NULL; }

  lochist_entry_t &operator=(const lochist_entry_t &r)
  {
    clear();
    memcpy(this, &r, sizeof(*this));
    if ( plce != NULL )
      plce = plce->clone();
    return *this;
  }

  bool operator==(const lochist_entry_t &r) const
  { return !is_valid() ? !r.is_valid() : (l_compare(plce, r.plce) == 0 && rinfo == r.rinfo); }
  bool operator!=(const lochist_entry_t &r) const
  { return !(*this == r); }

  void acquire_place(place_t *p)
  { clear(); plce = p; }

private:
  void clear()
  {
    if ( plce != NULL )
      qfree(plce);
  }

  friend class lochist_t;
  DEFINE_LOCHIST_T_HELPERS(friend)
};
DECLARE_TYPE_AS_MOVABLE(lochist_entry_t);

//-------------------------------------------------------------------------
class lochist_t
{
  void *ud;

  DEFINE_LOCHIST_T_HELPERS(friend)

  lochist_entry_t cur;
  netnode node;

#define LHF_HISTORY_DISABLED (1 << 0) // enable history?
#define LHF_HEXRAYS_68       (1 << 1) // hexrays < 6.9 compat mode
  uint32 flags;

public:
  lochist_t() : flags(0) { lochist_t_register_live(*this); }
  ~lochist_t() { lochist_t_deregister_live(*this); }
  bool is_history_enabled() const { return (flags & LHF_HISTORY_DISABLED) == 0; }
  bool is_hexrays68_compat() const { return (flags & LHF_HEXRAYS_68) != 0; }
  int get_place_id() const
  {
    const place_t *p = cur.place();
    return p == NULL ? -1 : p->id();
  }
  bool init(const char *stream_name, const place_t *_defpos, void *_ud, uint32 _flags)
  { return lochist_t_init(*this, stream_name, *_defpos, _ud, _flags); }

  nodeidx_t netcode() const
  { return node; }

  void jump(bool try_to_unhide, const lochist_entry_t &e)
  { lochist_t_jump(*this, try_to_unhide, e); }

  uint32 current_index() const
  { return lochist_t_current_index(*this); }

  bool seek(uint32 index, bool try_to_unhide)
  { return lochist_t_seek(*this, index, try_to_unhide, true); }

  bool fwd(uint32 cnt, bool try_to_unhide)
  { return lochist_t_fwd(*this, cnt, try_to_unhide); }

  bool back(uint32 cnt, bool try_to_unhide)
  { return lochist_t_back(*this, cnt, try_to_unhide); }

  void save() const
  { lochist_t_save(*this); }

  void clear()
  { lochist_t_clear(*this); }

  const lochist_entry_t &get_current() const
  { return *lochist_t_get_current(*this); }

  void set_current(const lochist_entry_t &e)
  { return set(current_index(), e); }

  void set(uint32 index, const lochist_entry_t &e)
  { lochist_t_set(*this, index, e); }

  bool get(lochist_entry_t *out, uint32 index) const
  { return lochist_t_get(out, *this, index); }

  uint32 size(void) const
  { return lochist_t_size(*this); }

  const place_t *get_template_place() const
  { return cur.place(); }
};
DECLARE_TYPE_AS_MOVABLE(lochist_t);

//-------------------------------------------------------------------------
#ifndef SWIG
idaman uint32 ida_export bookmarks_t_mark(const lochist_entry_t &, uint32, const char *, const char *, void *);
idaman bool ida_export bookmarks_t_get(lochist_entry_t *, qstring *, uint32 *, void *);
idaman bool ida_export bookmarks_t_get_desc(qstring *, const lochist_entry_t &, uint32, void *);
idaman uint32 ida_export bookmarks_t_find_index(const lochist_entry_t &, void *);
idaman uint32 ida_export bookmarks_t_size(const lochist_entry_t &, void *);
idaman bool ida_export bookmarks_t_erase(const lochist_entry_t &, uint32, void *);
#endif // SWIG

//-------------------------------------------------------------------------
class bookmarks_t
{
  bookmarks_t(); // No.
  ~bookmarks_t() {}
public:
#define BOOKMARKS_CHOOSE_INDEX (uint32(-1))
#define BOOKMARKS_BAD_INDEX (uint32(-1))

  // Mark/unmark position
  // index  - the marked position number (1..MAX_MARK_SLOT)
  //          if specified as BOOKMARKS_CHOOSE_INDEX: ask the user to select the mark slot. // FIXME: CHECK DOC IS COHERENT!
  // title  - if index == BOOKMARKS_CHOOSE_INDEX, then the window caption of
  //          the dialog which will appear on the screen. title==NULL will
  //          lead to the default caption: "please select a mark slot"
  // desc   - description of the marked position. If NULL, IDA will show a
  //          dialog box asking the user to enter the description.
  // returns used marker number (BOOKMARKS_BAD_INDEX - none)
  static uint32 mark(
          const lochist_entry_t &e,
          uint32 index,
          const char *title,
          const char *desc,
          void *ud)
  { return bookmarks_t_mark(e, index, title, desc, ud); }

  // 'out_entry' MUST:
  //  - contain a valid place_t*; data will be deserialized into it
  //  - have a valid, corresponding tcc_place_type_t
  static bool get(
          lochist_entry_t *out_entry,
          qstring *out_desc,
          uint32 *index, // index==BOOKMARKS_CHOOSE_INDEX? let the user choose
          void *ud)
  { return bookmarks_t_get(out_entry, out_desc, index, ud); }

  static bool get_desc(
          qstring *out,
          const lochist_entry_t &e,
          uint32 index,
          void *ud)
  { return bookmarks_t_get_desc(out, e, index, ud); }

  static uint32 find_index(
          const lochist_entry_t &e,
          void *ud)
  { return bookmarks_t_find_index(e, ud); }

  static uint32 size(
          const lochist_entry_t &e,
          void *ud)
  { return bookmarks_t_size(e, ud); }

  static bool erase(
          const lochist_entry_t &e,
          uint32 index,
          void *ud)
  { return bookmarks_t_erase(e, index, ud); }
};

//-------------------------------------------------------------------------
inline lochist_entry_t::lochist_entry_t(const lochist_t &lh)
  : plce(NULL)
{
  *this = lh.get_current();
}

//----------------------------------------------------------------------

#pragma pack(pop)
#endif // __MOVES_HPP

