/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2015 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 */

#ifndef _AREA_HPP
#define _AREA_HPP

/*! \file area.hpp

   \brief Contains the definition of ::areacb_t.

   This is a base class used by many parts of IDA.
   It is a collection of address ranges in the program.
   It conceptually consists of separate ::area_t instances.

   An area is a non-empty contiguous range of addresses (specified by
   its start and end addresses, the end address is excluded from the
   range) with some characteristics. For example, the ensemble of program
   segments is represented by an "areacb_t" called "segs".

   Areas are stored in the Btree part of the IDA database.
   To learn more about Btrees (Balanced Trees):
    http://www.bluerwhite.org/btree/
*/

#pragma pack(push, 1)           ///< IDA uses 1 byte alignments!

#ifndef SWIG
struct area_t;
/// Helper function. Should not be called directly!
idaman size_t ida_export area_t_print(const area_t *, char *buf, size_t bufsize);
#endif

//--------------------------------------------------------------------------
/// Base class for an area. This class is used as a base class for
/// a class with real information - see segment.hpp for example.
/// The end address points beyond the area.
struct area_t
{
  friend size_t ida_export area_t_print(const area_t *cb, char *buf, size_t bufsize);
  ea_t startEA;     ///< startEA included
  ea_t endEA;       ///< endEA excluded
  /// Constructor
  area_t(void) {}
  /// Constructor
  area_t(ea_t ea1, ea_t ea2) : startEA(ea1), endEA(ea2) {}

  /// Compare two area_t instances, based on the startEA
  int compare(const area_t &r) const { return startEA > r.startEA ? 1 : startEA < r.startEA ? -1 : 0; }

  bool operator ==(const area_t &r) const { return compare(r) == 0; }  ///< Compare two area_t's with '=='
  bool operator !=(const area_t &r) const { return compare(r) != 0; }  ///< Compare two area_t's with '!='
  bool operator > (const area_t &r) const { return compare(r) >  0; }  ///< Compare two area_t's with '<'
  bool operator < (const area_t &r) const { return compare(r) <  0; }  ///< Compare two area_t's with '>'

  /// Is 'ea' in the address range?
  bool contains(ea_t ea) const { return startEA <= ea && endEA > ea; }

  /// Is every ea in 'r' also in this area_t?
  bool contains(const area_t &r) const { return r.startEA >= startEA && r.endEA <= endEA; }

  /// Is there an ea in 'r' that is also in this area_t?
  bool overlaps(const area_t &r) const { return r.startEA < endEA && startEA < r.endEA; }

  /// Set #startEA, #endEA to 0
  void clear(void) { startEA = endEA = 0; }

  /// Is the size of the area_t <= 0?
  bool empty(void) const { return startEA >= endEA; }

  /// Get #endEA - #startEA
  asize_t size(void) const { return endEA - startEA; }

  /// Assign the area_t to the intersection between the area_t and 'r'
  void intersect(const area_t &r)
  {
    if ( startEA < r.startEA ) startEA = r.startEA;
    if ( endEA   > r.endEA   ) endEA   = r.endEA;
    if ( endEA   < startEA   ) endEA   = startEA;
  }

  /// Ensure that the area_t includes 'ea'
  void extend(ea_t ea)
  {
    if ( startEA > ea ) startEA = ea;
    if ( endEA < ea ) endEA = ea;
  }

  /// Print the area_t.
  /// \param buf the output buffer
  /// \param bufsize the size of the buffer
  size_t print(char *buf, size_t bufsize) const { return area_t_print(this, buf, bufsize); };
};
DECLARE_TYPE_AS_MOVABLE(area_t);
struct areavec_t : public qvector<area_t> /// Vector of area_t instances
{
};

//--------------------------------------------------------------------------
/// Helper functions. Should not be called directly!
#ifndef SWIG
#define AREASET_HELPER_DEFINITIONS(decl) \
decl bool                      ida_export areaset_t_add(areaset_t *, const area_t &area);\
decl bool                      ida_export areaset_t_sub(areaset_t *, const area_t &area);\
decl bool                      ida_export areaset_t_add2(areaset_t *, const areaset_t &aset);\
decl bool                      ida_export areaset_t_sub2(areaset_t *, const areaset_t &aset);\
decl bool                      ida_export areaset_t_has_common(const areaset_t *, const area_t &area, bool strict);\
decl bool                      ida_export areaset_t_has_common2(const areaset_t *, const areaset_t &aset);\
decl bool                      ida_export areaset_t_contains(const areaset_t *, const areaset_t &aset);\
decl size_t                    ida_export areaset_t_print(const areaset_t *, char *buf, size_t bufsize);\
decl bool                      ida_export areaset_t_intersect(areaset_t *, const areaset_t &aset);\
decl const area_t             *ida_export areaset_t_find_area(const areaset_t *, ea_t ea);\
decl ea_t                      ida_export areaset_t_next_addr(const areaset_t *, ea_t ea);\
decl ea_t                      ida_export areaset_t_prev_addr(const areaset_t *, ea_t ea);\
decl ea_t                      ida_export areaset_t_next_area(const areaset_t *, ea_t ea);\
decl ea_t                      ida_export areaset_t_prev_area(const areaset_t *, ea_t ea);\
decl areavec_t::const_iterator ida_export areaset_t_lower_bound(const areaset_t *, ea_t ea); \
decl areavec_t::const_iterator ida_export areaset_t_upper_bound(const areaset_t *, ea_t ea);
#else
#define AREASET_HELPER_DEFINITIONS(decl)
#endif // SWIG

class areaset_t;

AREASET_HELPER_DEFINITIONS(idaman)

/// An ordered set of non-overlapping address ranges
class areaset_t
{
  areavec_t bag;
  mutable const area_t *cache;

  AREASET_HELPER_DEFINITIONS(friend)
  bool verify(void) const;
public:
  DEFINE_MEMORY_ALLOCATION_FUNCS()
  areaset_t(void) : cache(NULL) {}                                                          ///< Constructor
  areaset_t(const area_t &area): cache(NULL) { if ( !area.empty() ) bag.push_back(area); }  ///< Constructor - Initialize set with 'area'
  areaset_t(const areaset_t &ivs) : bag(ivs.bag), cache(NULL) {}                            ///< Constructor - Initialize set with 'ivs'
  areaset_t &operator=(const areaset_t &ivs) { bag = ivs.bag; cache = NULL; return *this; }
  void swap(areaset_t &r) { bag.swap(r.bag); std::swap(cache, r.cache); }                                              ///< Set this = 'r' and 'r' = this. See qvector::swap()

  /// Add an address range to the set.
  /// If 'area' intersects an existing element e, then e is extended
  /// to include 'area', and any superfluous elements (subsets of e) are removed.
  /// \param area  address range to add. can not be empty
  /// \return false if 'area' was not added (the set was unchanged)
  bool add(const area_t &area)    { return areaset_t_add(this, area); };

  /// Create a new area_t from 'start' and 'end' and add it to the set
  bool add(ea_t start, ea_t _end) { return add(area_t(start, _end)); }

  /// Add each element of 'aset' to the set.
  /// \return false if no elements were added (the set was unchanged)
  bool add(const areaset_t &aset) { return areaset_t_add2(this, aset); };

  /// Subtract an address range from the set.
  /// All subsets of 'area' will be removed, and all elements that intersect
  /// 'area' will be truncated/split so they do not include 'area'.
  /// \param area  address range to subtract. can not be empty.
  /// \return false if 'area' was not subtracted (the set was unchanged)
  bool sub(const area_t &area)    { return areaset_t_sub(this, area); };

  /// Subtract an ea (an area of size 1) from the set. See sub(const area_t &)
  bool sub(ea_t ea)               { return sub(area_t(ea, ea+1)); }

  /// Subtract each area in 'aset' from the set
  /// \return false if nothing was subtracted (the set was unchanged)
  bool sub(const areaset_t &aset) { return areaset_t_sub2(this, aset); };

  /// Is there an ea in 'area' that is also in the areaset?
  bool has_common(const area_t &area) const
    { return areaset_t_has_common(this, area, false); };

  /// Is every ea in 'area' contained in the areaset?
  bool includes(const area_t &area) const
    { return areaset_t_has_common(this, area, true); };

  /// Print each area_t in the areaset
  size_t print(char *buf, size_t bufsize) const
    { return areaset_t_print(this, buf, bufsize); };

  /// Size in bytes
  asize_t count(void) const;

  /// Get the area_t at index 'idx'
  const area_t &getarea(int idx) const { return bag[idx]; }

  /// Get the last area_t in the set
  const area_t &lastarea(void) const { return bag.back(); }

  /// Get the number of area_t elements in the set
  size_t nareas(void) const { return bag.size(); }

  /// Does the set have zero elements
  bool empty(void) const { return bag.empty(); }

  /// Delete all elements from the set. See qvector::clear()
  void clear(void) { bag.clear(); cache = NULL; }

  /// Does any element of 'aset' overlap with an element in this areaset?. See area_t::overlaps()
  bool has_common(const areaset_t &aset) const
    { return areaset_t_has_common2(this, aset); };

  /// Does an element of the areaset contain 'ea'? See area_t::contains(ea_t)
  bool contains(ea_t ea) const { return !empty() && find_area(ea) != NULL; }

  /// Is every element in 'aset' contained in an element of this areaset?. See area_t::contains(area_t)
  bool contains(const areaset_t &aset) const
     { return areaset_t_contains(this, aset); };

  /// Set the areaset to its intersection with 'aset'.
  /// \return false if the set was unchanged
  bool intersect(const areaset_t &aset)
     { return areaset_t_intersect(this, aset); };

  /// Is every element in the areaset contained in an element of 'aset'?
  bool is_subset_of(const areaset_t &aset) const { return aset.contains(*this); }

  /// Do this areaset and 'aset' have identical elements?
  bool is_equal(const areaset_t &aset)   const { return bag == aset.bag; };

  bool operator==(const areaset_t &aset) const { return is_equal(aset); }   ///< Compare two areasets with '=='
  bool operator!=(const areaset_t &aset) const { return !is_equal(aset); }  ///< Compare two areasets with '!='

  typedef areavec_t::iterator iterator;                     ///< Iterator for areasets
  typedef areavec_t::const_iterator const_iterator;         ///< Const iterator for areasets
  const_iterator begin(void) const { return bag.begin(); }  ///< Get an iterator that points to the first element in the set
  const_iterator end(void)   const { return bag.end(); }    ///< Get an iterator that points to the end of the set. (This is NOT the last element)
  iterator begin(void) { return bag.begin(); }              ///< \copydoc begin
  iterator end(void)   { return bag.end(); }                ///< \copydoc end

  /// Get the first area that contains at least one ea_t value greater than 'ea'
  const_iterator lower_bound(ea_t ea) const { return areaset_t_lower_bound(this, ea); };

  /// Get the first area such that every ea_t value in this area is strictly greater than 'ea'
  const_iterator upper_bound(ea_t ea) const { return areaset_t_upper_bound(this, ea); };

  /// Get the element from the set that contains 'ea'.
  /// \return NULL if there is no such element
  const area_t *find_area(ea_t ea) const
     { return areaset_t_find_area(this, ea); };

  /// When searching the areaset, we keep a cached element to help speed up searches.
  /// \return a pointer to the cached element
  const area_t *cached_area(void) const { return cache; }

  /// Get the smallest ea_t value greater than 'ea' contained in the areaset
  ea_t next_addr(ea_t ea) const { return areaset_t_next_addr(this, ea); };

  /// Get the largest ea_t value less than 'ea' contained in the areaset
  ea_t prev_addr(ea_t ea) const { return areaset_t_prev_addr(this, ea); };

  /// Get the smallest ea_t value greater than 'ea' that is not in the same area as 'ea'
  ea_t next_area(ea_t ea) const { return areaset_t_next_area(this, ea); };

  /// Get the largest ea_t value less than 'ea' that is not in the same area as 'ea'
  ea_t prev_area(ea_t ea) const { return areaset_t_prev_area(this, ea); };

  /// Subtract the address range (from, from+size) and add the range (to, to+size)
  int move_chunk(ea_t from, ea_t to, asize_t size);

  /// TODO: return code borrowed from va.hpp, same with move_chunk()
  int check_move_args(ea_t from, ea_t to, asize_t size);
};
DECLARE_TYPE_AS_MOVABLE(areaset_t);
typedef qvector<areaset_t> array_of_areasets; ///< Array of areaset_t objects

//--------------------------------------------------------------------------
// Helper functions. Should not be called directly!
#ifndef SWIG
#ifndef NO_OBSOLETE_FUNCS
#define OLD_AREA_VISITOR(decl) decl int ida_export areacb_t_for_all_areas(areacb_t *,ea_t ea1, ea_t ea2, area_visitor_t *av, void *ud);
#else
#define OLD_AREA_VISITOR(decl) ///< \copydoc AREASET_HELPER_DEFINITIONS
#endif
/// \copydoc AREASET_HELPER_DEFINITIONS
#define AREA_HELPER_DEFINITIONS(decl) \
decl void    ida_export areacb_t_zero             (areacb_t *);\
decl void    ida_export areacb_t_terminate        (areacb_t *);\
decl void    ida_export areacb_t_save             (const areacb_t *);\
decl bool    ida_export areacb_t_link             (areacb_t *,const char *file, const char *name, int infosize);\
decl bool    ida_export areacb_t_create           (areacb_t *,const char *file,const char *name,uint infosize);\
decl void    ida_export areacb_t_kill             (areacb_t *);\
decl bool    ida_export areacb_t_create_area      (const areacb_t *,const area_t *info);\
decl bool    ida_export areacb_t_update           (const areacb_t *,const area_t *info);\
decl area_t *ida_export areacb_t_get_area         (const areacb_t *,ea_t ea);\
decl area_t *ida_export areacb_t_getn_area        (const areacb_t *,unsigned int n);\
decl int     ida_export areacb_t_get_area_num     (const areacb_t *,ea_t ea);\
decl ea_t    ida_export areacb_t_prepare_to_create(areacb_t *,ea_t start,ea_t end);\
decl int     ida_export areacb_t_get_next_area    (const areacb_t *,ea_t ea);\
decl int     ida_export areacb_t_get_prev_area    (areacb_t *,ea_t ea);\
decl int     ida_export areacb_t_lock_area        (const areacb_t *,const area_t *);\
decl int     ida_export areacb_t_unlock_area      (const areacb_t *,const area_t *);\
decl bool    ida_export areacb_t_may_lock_area    (const areacb_t *,const area_t *);\
decl int     ida_export areacb_t_get_area_locks   (const areacb_t *,const area_t *);\
decl area_t *ida_export areacb_t_prev_area_ptr    (areacb_t *,ea_t ea);\
decl area_t *ida_export areacb_t_next_area_ptr    (areacb_t *,ea_t ea);\
decl area_t *ida_export areacb_t_first_area_ptr   (areacb_t *);\
decl bool    ida_export areacb_t_del_area         (areacb_t *,ea_t ea, bool delcmt);\
decl bool    ida_export areacb_t_may_start_at     (const areacb_t *,uint n,ea_t newstart);\
decl bool    ida_export areacb_t_may_end_at       (const areacb_t *,uint n,ea_t newend);\
decl bool    ida_export areacb_t_set_start        (areacb_t *,uint n,ea_t newstart);\
decl bool    ida_export areacb_t_set_end          (areacb_t *,uint n,ea_t newend);\
decl bool    ida_export areacb_t_resize_areas     (areacb_t *,uint n,ea_t newstart);\
decl uint    ida_export areacb_t_get_area_qty     (const areacb_t *);\
decl area_t *ida_export areacb_t_choose_area      (areacb_t *,int flags, int width, char *(idaapi*getl)(areacb_t *obj,uint32 n,char *buf), const char *title, int icon, int x0,int y0,int x1,int y1, const char * const *popup_menus, ea_t defea);\
decl area_t *ida_export areacb_t_choose_area2     (areacb_t *,int flags, int ncol, const int *widths, void (idaapi*getl)(areacb_t *obj,uint32 n,char * const *arrptr), const char *title, int icon, int x0,int y0,int x1,int y1, const char * const *popup_menus, ea_t defea);\
decl bool    ida_export areacb_t_set_area_cmt     (areacb_t *,const area_t *a, const char *cmt, bool repeatable);\
decl char   *ida_export areacb_t_get_area_cmt     (const areacb_t *,const area_t *a, bool repeatable);\
decl int     ida_export areacb_t_move_areas       (areacb_t *,ea_t from,ea_t to, asize_t size, int (idaapi*area_mover)(area_t *a, adiff_t delta, void *ud), void *ud);\
decl void    ida_export areacb_t_make_hole        (areacb_t *,ea_t ea1, ea_t ea2, bool create_tail_area);\
decl int     ida_export areacb_t_for_all_areas2   (areacb_t *,ea_t ea1, ea_t ea2, area_visitor2_t &av);\
OLD_AREA_VISITOR(decl)
#else
#define AREA_HELPER_DEFINITIONS(decl)
#endif // SWIG

class areacb_t;
class areas_cache_t;

#ifndef NO_OBSOLETE_FUNCS
typedef int idaapi area_visitor_t(area_t *a, void *ud);
#endif

/// Interface class for processing area_t objects
struct area_visitor2_t
{
  virtual int idaapi visit_area(area_t *a) = 0;
  DEFINE_VIRTUAL_DTOR(area_visitor2_t)
};

AREA_HELPER_DEFINITIONS(idaman)

//--------------------------------------------------------------------------
/// Definition of areas control block. Each type of areas has its own
/// control block. Access to all areas of one type is made using this
/// control block.

class areacb_t
{
  AREA_HELPER_DEFINITIONS(friend)


  // private definitions, should never be directly accessed
  uval_t areasCode;             // code in the database
  ushort infosize;              // sizeof info for area
  void   *lastreq;              // (internal)
  uint32 used_cache;            // (internal)

  areas_cache_t *areas;         // areas loaded from IDA database
  void *reserved2[128];         // not used

  bool pack_and_write_area(const area_t *a) const;   // returns 1-ok, 0-failure
  bool duplicate_area_contents(area_t *a) const;     // returns 1-ok, 0-failure
  bool move_away(int cur, ea_t ea1, ea_t ea2, bool create_tail_area);
  void remove_old_db(const char *file, bool del_other);
  void move_area_comment(ea_t oldea, ea_t newea);

public:

  /// Read callback: read area from the database.
  /// This function is called when a (possibly packed) area is read from the database.
  /// \param packed  stream of packed bytes
  /// \param end     ptr to the end of the stream
  /// \param a       place to put unpacked version of the area
  /// This callback may be NULL.
  /// MUST be set (if any) before call to link()
  void (idaapi *read_cb)(const uchar *packed, const uchar *end, area_t *a);

  /// Write callback: write area to the database.
  /// This function is called when an area is about to be written to the database.
  /// It may pack the the area to the stream of bytes.
  /// \param a        area to be written
  /// \param packbuf  buffer to hold packed version of the area
  /// \param packend  ptr to the end of packbuf
  /// \return the number of bytes in the packed form
  /// This callback may be NULL.
  /// MUST be set (if any) before call to link()
  size_t (idaapi *write_cb)(const area_t *a,uchar *packbuf, uchar *packend);

  /// Destroy callback: remove an area from the internal cache.
  /// This function is called when an area is freed from the cache.
  /// This callback may be NULL.
  void (idaapi *delcache_cb)(area_t *a);

  /// The following three callbacks are used in open_areas_window() function.
  /// When the user presses Ctrl-E key the following callback is called
  /// to edit the selected area.
  /// This callback may be NULL.
  int (idaapi *edit_cb)(area_t *a);

  /// Callback to handle "Del" keystroke in open_areas_window() function.
  /// This callback may be NULL.
  int (idaapi *kill_cb)(area_t *a);

  /// Callback to handle "Ins" keystroke in open_areas_window() function
  /// This callback may be NULL.
  int (idaapi *new_cb)(void);

  /// Constructor. Initialized area control block. You need to link
  /// area control block to existing area information in Btree (link) or
  /// to create a new area information structure in Btree (create).
  void zero(void)               { areacb_t_zero(this); }
  areacb_t(void)                { zero(); } ///< \copydoc zero()

  /// Destructor. Flushes all information to Btree, deallocates caches and
  /// unlinks area control block from Btree.
  void terminate(void)          { areacb_t_terminate(this); }
  ~areacb_t(void)               { terminate(); } ///< \copydoc terminate()

  /// Accessors
  uval_t get_netnode(void) const { return areasCode; }

  /// Flush area control block to Btree.
  void save(void) { areacb_t_save(this); }

  /// Link area control block to Btree. Allocate cache, etc.
  /// Btree should contain information about the specified areas.
  /// After calling this function you may work with areas.
  /// \param file       name of input file being disassembled.
  ///                   Doesn't matter if useva==0.
  ///                   This parameter is used to build name of the file
  ///                   with the virtual array.
  /// \param name       name of area information in Btree.
  ///                   The name should start with "$ " (yes, including a space)
  ///                   You may use any name you like. For example, area control
  ///                   block keeping information about separation of program regions
  ///                   to different output files might be:
  ///                   "$ here is info about output file areas"
  /// \param _infosize  size of a structure with actual area information
  ///                   (size of class based on class ::area_t)
  /// This function properly terminates work with the previous area control
  /// block in Btree if the current class was used to access information.
  ///
  /// \retval 1  ok
  /// \retval 0  failure (no such node in Btree)
  bool link(const char *file,            // Access to existing areas
            const char *name,
            int _infosize)
        { return areacb_t_link(this, file, name, _infosize); }

  /// Create area information node in Btree.
  /// This function usually is used when a new file is loaded into the database.
  /// See link() for explanations of input parameters.
  /// This function properly terminates work with the previous area control
  /// block in Btree if the current class was used to access information.
  /// \retval 1  ok
  /// \retval 0  failure (Btree already contains node with the specified name)
  bool create(const char *file, const char *name, uint _infosize)
        { return areacb_t_create(this, file, name, _infosize); }

  /// Delete area information in Btree.
  /// All information about the current type of areas is deleted.
  /// Deallocate cache.
  void kill(void)        { areacb_t_kill(this); }

  /// Create an area.
  /// The new area should not overlap with existing ones.
  /// \param info    structure containing information about a new area.
  ///                startEA and endEA specify address range.
  ///                startEA should be less than endEA
  /// \retval 1  ok
  /// \retval 0  failure, area overlaps with another area or bad address range.
  bool create_area(const area_t *info)
        { return areacb_t_create_area(this, info); }

  /// Update information about area in Btree.
  /// This function can't change startEA and endEA fields.
  /// Its only purpose is to update additional characteristics of the area.
  bool update(const area_t *info)
        { return areacb_t_update(this, info); }

  /// Get pointer to area structure by address.
  /// \param ea  any address in the area
  /// \return NULL: no area occupies the specified address
  ///         otherwise returns pointer to area structure.
  area_t *get_area(ea_t ea)
        { return areacb_t_get_area(this, ea); }

  /// Get pointer to area structure by its number.
  /// \param n - number of area (0..get_area_qty()-1)
  /// \return NULL if the specified area doesn't exist.
  ///         otherwise returns pointer to area structure.
  area_t *getn_area(unsigned int n)
        { return areacb_t_getn_area(this, n); }

  /// Get number of area by address.
  /// \param ea - any address in the area
  /// \return -1 if no area occupies the specified address.
  ///         otherwise returns number of the specified area (0..get_area_qty()-1)
  int get_area_num(ea_t ea)
        { return areacb_t_get_area_num(this, ea); }

  /// Prepare to create a new area.
  /// This function checks whether the new area overlap with an existing one
  /// and trims an existing area to make enough address range for the creation
  /// of the new area. If the trimming of the existing area doesn't make enough
  /// room for the new area, the existing area is simply deleted.
  /// \param start  start address of the new area
  /// \param end    end address of the new area
  /// \return an adjusted end address for the new area. The end address may
  ///          require some adjustment if another (next) area exists and occupies
  ///          some addresses from (start..end) range. In this case we don't
  ///          delete the existing area but adjust end address of the new area.
  ea_t prepare_to_create(ea_t start,ea_t end)
        { return areacb_t_prepare_to_create(this, start, end); }

  /// Get number of the next area.
  /// This function returns number of the next (higher in the addressing space)
  /// area.
  /// \param ea   any address in the program
  /// \return -1 if no (more) areas
  ///         otherwise returns number in the range (0..get_area_qty()-1)
  int get_next_area(ea_t ea)         // Return next area for EA
                                                // -1 - no (more) areas
        { return areacb_t_get_next_area(this, ea); }

  /// Get number of the previous area.
  /// This function returns number of the previous (lower in the addressing space)
  /// area.
  /// \param ea  any address in the program
  /// \return -1 if no (more) areas.
  ///         otherwise returns number in the range (0..get_area_qty()-1)
  int get_prev_area(ea_t ea)                  // Returns prev area for EA
                                                // Returns -1 if not found
        { return areacb_t_get_prev_area(this, ea); }

  /// Get the next area.
  /// \param ea  any address in the program
  /// \return NULL if there are no (more) areas
  area_t *next_area_ptr(ea_t ea) { return areacb_t_next_area_ptr(this, ea); }

  /// Get the previous area.
  /// \param ea  any address in the program
  /// \return NULL if there are no (more) areas
  area_t *prev_area_ptr(ea_t ea) { return areacb_t_prev_area_ptr(this, ea); }

  /// Get the first area.
  /// \return NULL if there are no areas
  area_t *first_area_ptr(void) { return areacb_t_first_area_ptr(this); }

  /// Get the last area.
  /// \return NULL if there are no areas
  area_t *last_area_ptr(void) { return areacb_t_prev_area_ptr(this, BADADDR); }

  /// Delete an area.
  /// \param ea      any address in the area
  /// \param delcmt  delete area comments.
  ///                you may choose not to delete comments if you want to
  ///                create a new area with the same start address immediately.
  ///                In this case the new area will inherit old area comments.
  /// \retval 1  ok
  /// \retval 0  failure (no such area)
  bool del_area(ea_t ea, bool delcmt=true)          // Delete area
        { return areacb_t_del_area(this, ea, delcmt); }

  /// Check if the specified area may start at the specified address.
  /// This function checks whether the specified area can be changed so
  /// that its start address would be 'newstart'.
  /// \param n         number of area to check
  /// \param newstart  new start address for the area
  /// \retval 1  yes, it can
  /// \retval 0  if:
  ///              - the specified area doesn't exist
  ///              - new start address is higher or equal to end address
  ///              - the area would overlap with another area
  bool may_start_at(uint n,ea_t newstart)      // can the area have 'newstart'
        { return areacb_t_may_start_at(this, n, newstart); }

  /// Check if the specified area may end at the specified address.
  /// This function checks whether the specified area can be changed so
  /// that its end address would be 'newend'.
  /// \param n         number of area to check
  /// \param newend    new end address for the area
  /// \retval 1  yes, it can
  /// \retval 0  if:
  ///              - the specified area doesn't exist
  ///              - new start address is higher or equal to end address
  ///              - the area would overlap with another area
  bool may_end_at(uint n,ea_t newend)          // can the area have 'newend'
        { return areacb_t_may_end_at(this, n, newend); }

  /// Change start address of the area.
  /// \param n         number of area to change
  /// \param newstart  new start address for the area
  /// This function doesn't modify other areas.
  /// \retval 1  ok, area is changed
  /// \retval 0  if:
  ///              - the specified area doesn't exist
  ///              - new start address is higher or equal to end address
  ///              - the area would overlap with another area
  bool set_start(uint n, ea_t newstart)
        { return areacb_t_set_start(this, n, newstart); }

  /// Change end address of the area.
  /// This function doesn't modify other areas.
  /// \param n       number of area to change
  /// \param newend  new end address for the area
  /// \retval 1  ok, area is changed
  /// \retval 0  if:
  ///              - the specified area doesn't exist
  ///              - new end address is lower or equal to start address
  ///              - the area would overlap with another area
  bool set_end(uint n, ea_t newend)
        { return areacb_t_set_end(this, n, newend); }

  /// Make a hole at the specified address by deleting or modifying existing areas.
  /// \param ea1,ea2           range to clear
  /// \param create_tail_area  in the case if there is a big area overlapping
  ///                          the specified range, should it be divided in two areas?
  ///                          if 'false', then it will be truncated and the tail
  ///                          will be left without any covering area
  void make_hole(ea_t ea1, ea_t ea2, bool create_tail_area)
        { areacb_t_make_hole(this, ea1, ea2, create_tail_area); }

  /// Resize adjacent areas simultaneously.
  /// \param n        number of area to change.
  ///                 The adjacent (previous) area will be trimmed or expanded
  ///                 if it exists and two areas are contiguous.
  ///                 Otherwise this function behaves like set_start() function.
  /// \return success
  bool resize_areas(uint n,ea_t newstart)      // Resize adjacent areas
        { return areacb_t_resize_areas(this, n, newstart); }

  /// Get number of areas.
  /// \return number of areas ( >= 0 )
  uint get_area_qty(void) const
        { return areacb_t_get_area_qty(this); }

  /// Let the user choose an area. (1-column chooser).
  /// This function displays a window with a list of areas
  /// and allows the user to choose an area from the list.
  /// \param flags        \ref CH_
  /// \param width        width of the window
  /// \param getl         callback function to get text representation of an area.
  ///                       - obj: pointer to area control block
  ///                       - n:   (number of area + 1). if n==0 then getl() should
  ///                              return text of a header line.
  ///                       - buf: buffer for the text representation
  ///
  ///                     getl() should return pointer to text representation string
  ///                     (not necessarily the same pointer as 'buf')
  /// \param title        title of the window.
  /// \param icon         number of icon to display
  /// \param defea        address which points to the default area. The cursor will be
  ///                     position to this area.
  /// \param x0,y0,x1,y1  window position on the screen.
  ///                     -1 values specify default window position.
  ///                       - txt:upper left corner of the screen
  ///                       - gui:centered on the foreground window
  /// \param popup_menus  default is insert, delete, edit, refresh
  /// \return NULL if the user pressed Esc.
  ///      otherwise a pointer to the selected area.
  area_t *choose_area(int flags,
                      int width,
                      char *(idaapi*getl)(areacb_t *obj,uint32 n,char *buf),
                      const char *title,
                      int icon,
                      int x0=-1,int y0=-1,int x1=-1,int y1=-1,
                      const char * const *popup_menus=NULL,
                      ea_t defea=BADADDR)
  {
    return areacb_t_choose_area(this, flags, width, getl, title, icon,
                        x0, y0, x1, y1, popup_menus, defea);
  }

  /// Let the user choose an area. (n-column chooser).
  /// This function displays a window with a list of areas
  /// and allows the user to choose an area from the list.
  /// \param flags        \ref CH_
  /// \param ncol         number of columns
  /// \param widths       widths of each column in characters (may be NULL)
  /// \param getl         callback function to get text representation of an area
  ///                       - obj:    pointer to area control block
  ///                       - n:      (number of area + 1). if n==0 then getl() should
  ///                                 return text of a header line.
  ///                       - arrptr: array of buffers for the text representation
  /// \param title        title of the window.
  /// \param icon         number of icon to display
  /// \param defea        address which points to the default area. The cursor will be
  ///                     position to this area.
  /// \param x0,y0,x1,y1  window position on the screen.
  ///                     -1 values specify default window position.
  ///                       - txt:upper left corner of the screen
  ///                       - gui:centered on the foreground window
  /// \param popup_menus  default is insert, delete, edit, refresh
  /// \return NULL if the user cancelled the selection
  ///     otherwise a pointer to the selected area.
  area_t *choose_area2(int flags,
                      int ncol,
                      const int *widths,
                      void (idaapi*getl)(areacb_t *obj,uint32 n,char * const *arrptr),
                      const char *title,
                      int icon,
                      int x0=-1,int y0=-1,int x1=-1,int y1=-1,
                      const char * const *popup_menus=NULL,
                      ea_t defea=BADADDR)
  {
    return areacb_t_choose_area2(this, flags, ncol, widths, getl, title, icon,
                        x0, y0, x1, y1, popup_menus, defea);
  }

  /// Find previous gap in areas.
  /// This function finds a gap between areas. Only enabled addresses
  /// (see bytes.hpp for explanations on addressing) are used in the search.
  /// \param ea  any linear address
  /// \return #BADADDR if no previous gap is found.
  ///      otherwise returns maximal address in the previous gap
  ea_t find_prev_gap(ea_t ea);

  /// Find next gap in areas.
  /// This function finds a gap between areas. Only enabled addresses
  /// (see bytes.hpp for explanations on addressing) are used in the search.
  /// \param ea  any linear address
  /// \return #BADADDR if no next gap is found.
  ///         otherwise returns start address of the next gap
  ea_t find_next_gap(ea_t ea);

  /// Set area comment.
  /// This function sets area comment.
  /// \param a           pointer to area structure (may be NULL)
  /// \param repeatable  0: set regular comment,
  ///                    1: set repeatable comment
  bool set_area_cmt(const area_t *a, const char *cmt, bool repeatable)
        { return areacb_t_set_area_cmt(this, a, cmt, repeatable); }

  /// Delete area comment.
  /// Each area may have its comment (a function may have a comment, for example)
  /// This function deletes such a comment.
  /// \param a           pointer to area structure (may be NULL)
  /// \param repeatable  0: delete regular comment,
  ///                    1: delete repeatable comment
  void del_area_cmt(const area_t *a, bool repeatable)
                { set_area_cmt(a, "", repeatable); }

  /// Get area comment.
  /// The caller must qfree() the result.
  /// \param a           pointer to area structure (may be NULL)
  /// \param repeatable  0: get regular comment,
  ///                    1: get repeatable comment
  char *get_area_cmt(const area_t *a, bool repeatable) const
        { return areacb_t_get_area_cmt(this, a, repeatable); }

  /// Move area information to the specified addresses.
  /// \return 0 if ok, otherwise the code returned by area_mover
  int move_areas(ea_t from,
                 ea_t to,
                 asize_t size,
                 int (idaapi *area_mover)(area_t *a, adiff_t delta, void *ud)=NULL,
                 void *ud=NULL)
        { return areacb_t_move_areas(this, from, to, size, area_mover, ud); }

  /// Call a function for all areas in the specified range.
  /// Stop the enumeration if the function returns non-zero
  /// \return 0 if all areas were visited, otherwise the code returned
  ///          by the callback
#ifndef NO_OBSOLETE_FUNCS
  DEPRECATED int for_all_areas(ea_t ea1, ea_t ea2, area_visitor_t *av, void *ud)
        { return areacb_t_for_all_areas(this, ea1, ea2, av, ud); }
#endif
  int for_all_areas2(ea_t ea1, ea_t ea2, area_visitor2_t &av)   ///< \copydoc for_all_areas()
        { return areacb_t_for_all_areas2(this, ea1, ea2, av); }

  /// It is ok to lock an area? If returns 'false', lock_area() should not be called
  bool may_lock_area(const area_t *a) { return areacb_t_may_lock_area(this, a); }

};

/// A helper class to lock an area pointer.
/// Locked pointers are guaranteed to remain valid until they are unlocked.
/// Areas with locked pointers can not be deleted or moved.
class lock_area
{
  areacb_t *cb;
  area_t *area;
public:
  lock_area(areacb_t *_cb, area_t *_area) : cb(_cb), area(_area)
  {
    areacb_t_lock_area(cb, area);
  }
  ~lock_area(void)
  {
    areacb_t_unlock_area(cb, area);
  }
};


#pragma pack(pop)
#endif  // _AREA_HPP
