/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2015 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 */

#ifndef _NETNODE_HPP
#define _NETNODE_HPP
#pragma pack(push, 1)           // IDA uses 1 byte alignments!

/*! \file netnode.hpp

  \brief Functions that provide the lowest level public interface to the database.

  (Namely, Btree). To learn more about Balanced Trees:
  http://www.bluerwhite.org/btree/

  We don't use Btree directly.
  Instead, we have another layer built on the top of Btree.
  Here is a brief explanation of this layer.

  An object called "netnode" is modeled on the top of Btree.
  Each netnode has a unique id: a 32-bit value (64-bit for ida64).
  Usually there is a trivial mapping of the linear addresses used in
  the program to netnodes. If we have additional information about
  an address (a comment is attached to it, for example), this
  information is stored in the corresponding netnode.
  See nalt.hpp to see how the kernel uses netnodes.
  Also, some netnodes have no corresponding linear address (however, they still
  have an id). They are used to store information not related to a particular
  address.

  Each netnode _may_ have the following attributes:

    - a name: an arbitrary non-empty string, up to 255KB-1 bytes

    - a value: arbitrary sized object, max size is MAXSPECSIZE

    - altvals: a sparse array of 32-bit values.
      indexes in this array may be 8-bit or 32-bit values

    - supvals: an array of arbitrary sized objects. (size of each
      object is limited by MAXSPECSIZE)
      indexes in this array may be 8-bit or 32-bit values

    - charvals: a sparse array of 8-bit values.
      indexes in this array may be 8-bit or 32-bit values

    - hashvals: a hash (an associative array).
      indexes in this array are strings
      values are arbitrary sized (max size is MAXSPECSIZE)

  Initially a new netnode contains no information at all so no disk space
  is used for it. As you add new information, the netnode grows.

  All arrays that are attached to the netnode behave in the same manner.
  Initially:
    - all members of altvals/charvals array are zeroes
    - all members of supvals/hashvals array are undefined

  If you need to store objects bigger that MAXSPECSIZE, please note that
  there are high-level functions to store arbitrary sized objects in supvals.
  See setblog/getblob and other blob-related functions.

  You may use netnodes to store additional information about the program.
  Limitations on the use of netnodes are the following:

    - use netnodes only if you could not find a kernel service to
      store your type of information

    - do not create netnodes with valid identifier names.
      Use the "$ " prefix (or any other prefix with characters not allowed
      in the identifiers for the names of your netnodes.
      Although you will probably not destroy anything by
      accident, using already defined names for the names of your
      netnodes is still discouraged.

    - you may create as many netnodes as you want (creation of an unnamed
      netnode does not increase the size of the database).
      however, since each netnode has a number, creating too many netnodes
      could lead to the exhaustion of the netnode numbers (the numbering
      starts at 0xFF000000)

    - remember that netnodes are automatically saved to the disk
      by the kernel.

  Advanced info:

  In fact a netnode may contain up to 256 arrays of arbitrary sized
  objects (not only the 4 listed above). Each array has an 8-bit tag.
  Usually tags are represented by character constants. For example, altvals
  and supvals are simply 2 of 256 arrays, with the tags 'A' and 'S' respectively.

*/

//--------------------------------------------------------------------------
/// Maximum length of a netnode name. WILL BE REMOVED IN THE FUTURE
const int MAXNAMESIZE = 512;

/// Maximum length of a name. We permit names up to 32KB-1 bytes.
const int MAX_NODENAME_SIZE = 32*1024;

/// Maximum length of strings or objects stored in a supval array element
const int MAXSPECSIZE = 1024;

/// \typedef{nodeidx_t, numbers are 64 bit for 64 bit IDA}
#ifdef __EA64__
typedef uint64 nodeidx_t;
#else
typedef uint32 nodeidx_t;
#endif

/// A number to represent a bad netnode reference
#define BADNODE nodeidx_t(-1)

/// \defgroup nn_res Reserved netnode tags
/// Tags internally used in netnodes. You should not use them
/// for your tagged alt/sup/char/hash arrays.
//@{
const char atag = 'A';                  ///< Array of altvals
const char stag = 'S';                  ///< Array of supvals
const char htag = 'H';                  ///< Array of hashvals
const char vtag = 'V';                  ///< Value of netnode
const char ntag = 'N';                  ///< Name of netnode
const char ltag = 'L';                  ///< Links between netnodes
//@}


/// \name Helper functions
/// They should not be called directly! See ::netnode
//@{
class netnode;
class linput_t;
idaman bool  ida_export netnode_check           (netnode *, const char *name, size_t namlen, bool create);
idaman void  ida_export netnode_kill            (netnode *);
idaman bool  ida_export netnode_start           (netnode *);
idaman bool  ida_export netnode_end             (netnode *);
idaman bool  ida_export netnode_next            (netnode *);
idaman bool  ida_export netnode_prev            (netnode *);
idaman ssize_t ida_export netnode_name          (nodeidx_t num, char *buf, size_t bufsize);
idaman ssize_t ida_export netnode_get_name      (nodeidx_t num, qstring *out);
idaman bool  ida_export netnode_rename          (nodeidx_t num, const char *newname, size_t namlen);
idaman ssize_t ida_export netnode_valobj        (nodeidx_t num, void *buf, size_t bufsize);
idaman ssize_t ida_export netnode_valstr        (nodeidx_t num, char *buf, size_t bufsize);
idaman bool  ida_export netnode_set             (nodeidx_t num, const void *value, size_t length);
idaman bool  ida_export netnode_delvalue        (nodeidx_t num);
idaman nodeidx_t ida_export netnode_altval      (nodeidx_t num, nodeidx_t alt, char tag);
idaman uchar ida_export netnode_charval         (nodeidx_t num, nodeidx_t alt, char tag);
idaman nodeidx_t ida_export netnode_altval_idx8 (nodeidx_t num, uchar alt, char tag);
idaman uchar ida_export netnode_charval_idx8    (nodeidx_t num, uchar alt, char tag);
idaman ssize_t ida_export netnode_supval        (nodeidx_t num, nodeidx_t alt, void *buf, size_t bufsize, char tag);
idaman ssize_t ida_export netnode_supstr        (nodeidx_t num, nodeidx_t alt, char *buf, size_t bufsize, char tag);
idaman bool  ida_export netnode_supset          (nodeidx_t num, nodeidx_t alt, const void *value, size_t length, char tag);
idaman bool  ida_export netnode_supdel          (nodeidx_t num, nodeidx_t alt, char tag);
idaman nodeidx_t ida_export netnode_sup1st      (nodeidx_t num, char tag);
idaman nodeidx_t ida_export netnode_supnxt      (nodeidx_t num, nodeidx_t cur, char tag);
idaman nodeidx_t ida_export netnode_suplast     (nodeidx_t num, char tag);
idaman nodeidx_t ida_export netnode_supprev     (nodeidx_t num, nodeidx_t cur, char tag);
idaman ssize_t ida_export netnode_supval_idx8   (nodeidx_t num, uchar alt, void *buf, size_t bufsize, char tag);
idaman ssize_t ida_export netnode_supstr_idx8   (nodeidx_t num, uchar alt, char *buf, size_t bufsize, char tag);
idaman bool  ida_export netnode_supset_idx8     (nodeidx_t num, uchar alt, const void *value, size_t length, char tag);
idaman bool  ida_export netnode_supdel_idx8     (nodeidx_t num, uchar alt, char tag);
idaman nodeidx_t ida_export netnode_sup1st_idx8 (nodeidx_t num, char tag);
idaman nodeidx_t ida_export netnode_supnxt_idx8 (nodeidx_t num, uchar alt, char tag);
idaman nodeidx_t ida_export netnode_suplast_idx8(nodeidx_t num, char tag);
idaman nodeidx_t ida_export netnode_supprev_idx8(nodeidx_t num, uchar alt, char tag);
idaman bool  ida_export netnode_supdel_all      (nodeidx_t num, char tag);
idaman int ida_export netnode_supdel_range      (nodeidx_t num, nodeidx_t idx1, nodeidx_t idx2, char tag);
idaman int ida_export netnode_supdel_range_idx8 (nodeidx_t num, nodeidx_t idx1, nodeidx_t idx2, char tag);
idaman ssize_t ida_export netnode_hashval       (nodeidx_t num, const char *idx, void *buf, size_t bufsize, char tag);
idaman ssize_t ida_export netnode_hashstr       (nodeidx_t num, const char *idx, char *buf, size_t bufsize, char tag);
idaman nodeidx_t ida_export netnode_hashval_long(nodeidx_t num, const char *idx, char tag);
idaman bool  ida_export netnode_hashset         (nodeidx_t num, const char *idx, const void *value, size_t length, char tag);
idaman bool  ida_export netnode_hashdel         (nodeidx_t num, const char *idx, char tag);
idaman ssize_t ida_export netnode_hash1st       (nodeidx_t num, char *buf, size_t bufsize, char tag);
idaman ssize_t ida_export netnode_hashnxt       (nodeidx_t num, const char *idx, char *buf, size_t bufsize, char tag);
idaman ssize_t ida_export netnode_hashlast      (nodeidx_t num, char *buf, size_t bufsize, char tag);
idaman ssize_t ida_export netnode_hashprev      (nodeidx_t num, const char *idx, char *buf, size_t bufsize, char tag);
idaman size_t ida_export netnode_blobsize       (nodeidx_t num, nodeidx_t start, char tag);
idaman void *ida_export netnode_getblob         (nodeidx_t num, void *buf, size_t *bufsize, nodeidx_t start, char tag);
idaman bool  ida_export netnode_setblob         (nodeidx_t num, const void *buf, size_t size, nodeidx_t start, char tag);
idaman int   ida_export netnode_delblob         (nodeidx_t num, nodeidx_t start, char tag);
idaman bool  ida_export netnode_inited          (void);
idaman size_t ida_export netnode_copy           (nodeidx_t num, nodeidx_t count, nodeidx_t target, bool move);
idaman size_t ida_export netnode_altshift       (nodeidx_t num, nodeidx_t from, nodeidx_t to, nodeidx_t size, char tag);
idaman size_t ida_export netnode_charshift      (nodeidx_t num, nodeidx_t from, nodeidx_t to, nodeidx_t size, char tag);
idaman size_t ida_export netnode_supshift       (nodeidx_t num, nodeidx_t from, nodeidx_t to, nodeidx_t size, char tag);
idaman void  ida_export netnode_altadjust       (nodeidx_t num, nodeidx_t from, nodeidx_t to, nodeidx_t size, bool (idaapi *should_skip)(nodeidx_t ea));
idaman bool  ida_export netnode_exist           (const netnode &n);
//@}

//--------------------------------------------------------------------------
//      N E T N O D E
//--------------------------------------------------------------------------

/// Definition of the IDA database node.
/// Note that the size of the 'netnode' class is 4 bytes and it can be
/// freely casted to 'uint32' and back. This makes it easy to store
/// information about the program location in the netnodes.
/// Please pass netnodes to functions by value.
class netnode
{
  /// \cond
  friend class netlink;
  friend bool ida_export netnode_check(netnode *, const char *name, size_t namlen, bool create);
  friend void ida_export netnode_kill (netnode *);
  friend bool ida_export netnode_start(netnode *);
  friend bool ida_export netnode_end  (netnode *);
  friend bool ida_export netnode_next (netnode *);
  friend bool ida_export netnode_prev (netnode *);
  friend size_t ida_export netnode_altshift (nodeidx_t num, nodeidx_t from, nodeidx_t to, nodeidx_t size, char tag);
  friend size_t ida_export netnode_charshift(nodeidx_t num, nodeidx_t from, nodeidx_t to, nodeidx_t size, char tag);
  friend size_t ida_export netnode_supshift (nodeidx_t num, nodeidx_t from, nodeidx_t to, nodeidx_t size, char tag);
  /// \endcond
public:

  //--------------------------------------------------------------------------
  /// \name Constructors, conversions and assignments
  //@{

  /// Empty constructor
  netnode(void)                     { netnodenumber = BADNODE; }

  /// Constructor to create a netnode to access information about the
  /// specified linear address
  netnode(nodeidx_t num)            { netnodenumber = num; }

  /// Conversion from netnode to linear address
  operator nodeidx_t() const        { return netnodenumber; }

  /// Construct an instance of netnode class to access the specified netnode.
  /// \param _name      name of netnode
  /// \param namlen     length of the name. may be omitted, in this
  ///                   case the length will be calculated with strlen()
  /// \param do_create  true:  create the netnode if it doesn't exist yet.
  ///                   false: don't create the netnode, set netnumber to #BADNODE if
  ///                         it doesn't exist
  netnode(const char *_name, size_t namlen=0, bool do_create=false)
  {
    netnode_check(this, _name, namlen, do_create);
  }

  //@}

  /// See exist()
  friend bool ida_export netnode_exist(const netnode &n);
  /// Does the specified netnode exist?.
  /// \retval true   there is some information attached to the netnode.
  /// \retval false  unnamed netnode without any information
  friend bool ida_export exist(const netnode &n) { return netnode_exist(n); }

  //--------------------------------------------------------------------------
  /// \name Create/Delete
  /// \note You can check if a netnode already exists with exist()
  //@{

  /// Create a named netnode.
  /// \param _name   name of netnode to create.
  ///                names of user-defined netnodes must have the "$ " prefix
  ///                in order to avoid clashes with program byte names.
  /// \param namlen  length of the name. if not specified, it will be
  ///                calculated using strlen()
  /// \retval 1  ok, the node is created
  /// \retval 0  the node already exists. you may use the netnode class to access it.
  bool create(const char *_name, size_t namlen=0)
  {
    return netnode_check(this, _name, namlen, true);
  }

  /// Create unnamed netnode.
  /// \retval 1  ok
  /// \retval 0  should not happen, indicates internal error
  bool create(void) { return create((char *)0); }

  /// Delete a netnode with all information attached to it
  void kill(void) { netnode_kill(this); }

  //@}

  //--------------------------------------------------------------------------
  /// \name Netnode names
  //@{

  /// Get the netnode name.
  /// \return -1 if netnode is unnamed (buf is untouched in this case),
  ///            otherwise the name length
  ssize_t get_name(qstring *buf) const
  {
    return netnode_get_name(*this, buf);
  }
#ifndef NO_OBSOLETE_FUNCS
  DEPRECATED ssize_t name(char *buf, size_t bufsize) const
  {
    return netnode_name(*this, buf, bufsize);
  }
#endif

  /// Rename a netnode.
  /// \param newname  new name of netnode. NULL or "" means to delete name.
  ///                 names of user-defined netnodes must have the "$ " prefix
  ///                 in order to avoid clashes with program byte names.
  /// \param namlen   length of new name. if not specified, it will be
  ///                 calculated using strlen()
  /// \retval 1  ok
  /// \retval 0  failed, newname is already used
  bool rename(const char *newname, size_t namlen=0)
  {
    return netnode_rename(*this, newname, namlen);
  }

  //@}

  //--------------------------------------------------------------------------
  /// \name Value of netnode
  //@{

  /// Get value of netnode.
  /// Netnode values are arbitrary sized objects with max size is #MAXSPECSIZE.
  /// NB: do not use this function for strings - see valstr().
  /// \return length of value, -1 if no value present
  ssize_t valobj(void *buf, size_t bufsize) const
  {
    return netnode_valobj(*this, buf, bufsize);
  }

  /// Get string value of netnode.
  /// See explanations for supstr() function about the differences between valobj()
  /// and valstr()
  /// \return length of value, -1 if no value present
  ssize_t valstr(char *buf, size_t bufsize) const
  {
    return netnode_valstr(*this, buf, bufsize);
  }

  /// Set value of netnode.
  /// \param value   pointer to value
  /// \param length  length of value. if not specified, it will be calculated
  ///                using strlen()
  /// \returns 1 - ok
  bool set(const void *value, size_t length=0)
  {
    return netnode_set(*this, value, length);
  }

  /// Delete value of netnode.
  /// \retval 1  ok
  /// \retval 0  failed, netnode is bad or other error
  bool delvalue(void)
  {
    return netnode_delvalue(*this);
  }

  /// Value of netnode as a long number:
  bool set_long(nodeidx_t x) { return set(&x, sizeof(x)); }
  bool value_exists(void) const { return valobj(NULL, 0) >= 0; }
  nodeidx_t long_value(void) const
  {
    nodeidx_t v = 0;
    if ( valobj(&v, sizeof(v)) > 0 )
      return v;
    return BADNODE;
  }

  //@}

  //--------------------------------------------------------------------------
  /// \name Arrays of altvals.
  /// altvals: a sparse array of 32-bit values.
  /// indexes in this array may be 8-bit or 32-bit values
  //@{

  /// Get altval element of the specified array.
  /// \param alt  index into array of altvals
  /// \param tag  tag of array. may be omitted
  /// \return value of altval element. nonexistent altval members are returned
  ///          as zeroes
  nodeidx_t altval(nodeidx_t alt, char tag=atag) const
  {
    return netnode_altval(*this, alt, tag);
  }

  /// Set value of altval array.
  /// \param alt    index into array of altvals
  /// \param value  new value of altval element
  /// \param tag    tag of array
  /// \retval 1  ok
  /// \retval 0  failed, normally should not occur
  bool altset(nodeidx_t alt, nodeidx_t value, char tag=atag)
  {
    return supset(alt, &value, sizeof(value), tag);
  }

  /// Delete element of altval array.
  /// \param alt  index into array of altvals
  /// \param tag  tag of array
  /// \retval 1  ok
  /// \retval 0  failed, element doesn't exist
  bool altdel(nodeidx_t alt, char tag=atag)
  {
    return supdel(alt, tag);
  }

  /// Get first existing element of altval array.
  /// \param tag  tag of array
  /// \return index of first existing element of altval array,
  ///          #BADNODE if altval array is empty
  nodeidx_t alt1st(char tag=atag) const
  {
    return sup1st(tag);
  }

  /// Get next existing element of altval array.
  /// \param cur  current index
  /// \param tag  tag of array
  /// \return index of the next existing element of altval array,
  ///          #BADNODE if no more altval array elements exist
  nodeidx_t altnxt(nodeidx_t cur, char tag=atag) const
  {
    return supnxt(cur, tag);
  }

  /// Get last element of altval array.
  /// \param tag  tag of array
  /// \return index of last existing element of altval array,
  ///          #BADNODE if altval array is empty
  nodeidx_t altlast(char tag=atag) const
  {
    return suplast(tag);
  }

  /// Get previous existing element of altval array.
  /// \param cur  current index
  /// \param tag  tag of array
  /// \return index of the previous existing element of altval array,
  ///          #BADNODE if no more altval array elements exist
  nodeidx_t altprev(nodeidx_t cur, char tag=atag) const
  {
    return supprev(cur, tag);
  }

  /// Shift the altval array elements.
  /// Moves the array elements at (from..from+size) to (to..to+size)
  /// \return number of shifted elements
  size_t altshift(nodeidx_t from, nodeidx_t to, nodeidx_t size, char tag=atag)
  {
    return netnode_altshift(*this, from, to, size, tag);
  }

  /// Adjust values of altval arrays elements.
  /// All altvals in the range from+1..from+size+1 and adjusted to have
  /// values in the range to+1..to+size+1.
  /// The parameter should_skip() can be used to skip the adjustment of some altvals
  void altadjust(nodeidx_t from, nodeidx_t to, nodeidx_t size, bool (idaapi *should_skip)(nodeidx_t ea)=NULL)
  {
    netnode_altadjust(*this, from, to, size, should_skip);
  }

  //@}

  /// \name Arrays of altvals: 8-bit values
  /// The following functions behave in the same manner as the functions
  /// described above. The only difference is that the array value is 8-bits.
  ///   - index: 32 bits
  ///   - value: 8  bits
  //@{
  uchar charval(nodeidx_t alt, char tag) const      { return netnode_charval(*this, alt, tag); }
  bool charset(nodeidx_t alt, uchar val, char tag)  { return supset(alt, &val, sizeof(val), tag); }
  bool chardel(nodeidx_t alt, char tag)             { return supdel(alt, tag); }
  nodeidx_t char1st(char tag) const                 { return sup1st(tag); }
  nodeidx_t charnxt(nodeidx_t cur, char tag) const  { return supnxt(cur, tag); }
  nodeidx_t charlast(char tag) const                { return suplast(tag); }
  nodeidx_t charprev(nodeidx_t cur, char tag) const { return supprev(cur, tag); }
  size_t charshift(nodeidx_t from, nodeidx_t to, nodeidx_t size, char tag)
    { return netnode_charshift(*this, from, to, size, tag); }
  //@}

  /// \name Arrays of altvals: 8-bit indexes
  /// Another set of functions to work with altvals.
  /// The only difference is that the array index is 8-bits,
  /// and therefore the array may contain up to 256 elements only.
  ///   - index: 8  bits
  ///   - value: 32 bits
  //@{
  nodeidx_t altval_idx8(uchar alt, char tag) const   { return netnode_altval_idx8(*this, alt, tag); }
  bool altset_idx8(uchar alt, nodeidx_t val, char tag){ return supset_idx8(alt, &val, sizeof(val), tag); }
  bool altdel_idx8(uchar alt, char tag)              { return supdel_idx8(alt, tag); }
  nodeidx_t alt1st_idx8(char tag) const              { return sup1st_idx8(tag); }
  nodeidx_t altnxt_idx8(uchar cur, char tag) const   { return supnxt_idx8(cur, tag); }
  nodeidx_t altlast_idx8(char tag) const             { return suplast_idx8(tag); }
  nodeidx_t altprev_idx8(uchar cur, char tag) const  { return supprev_idx8(cur, tag); }
  //@}

  /// \name More altvals
  /// Another set of functions to work with altvals.
  ///   - index: 8 bits
  ///   - value: 8 bits
  //@{
  uchar charval_idx8(uchar alt, char tag) const     { return netnode_charval_idx8(*this, alt, tag); }
  bool charset_idx8(uchar alt, uchar val, char tag) { return supset_idx8(alt, &val, sizeof(val), tag); }
  bool chardel_idx8(uchar alt, char tag)            { return supdel_idx8(alt, tag); }
  nodeidx_t char1st_idx8(char tag) const            { return sup1st_idx8(tag); }
  nodeidx_t charnxt_idx8(uchar cur, char tag) const { return supnxt_idx8(cur, tag); }
  nodeidx_t charlast_idx8(char tag) const           { return suplast_idx8(tag); }
  nodeidx_t charprev_idx8(uchar cur, char tag) const{ return supprev_idx8(cur, tag); }
  //@}

  /// \name Delete altvals
  /// \note To delete range of elements in an altval array, see supdel_range()
  //@{

  /// Delete all elements of altval array.
  /// This function may be applied to 32-bit and 8-bit altval arrays.
  /// This function deletes the whole altval array.
  /// \return success
  bool altdel(void)
  {
    return supdel_all(atag);
  }

  /// Delete all elements of the specified altval array.
  /// This function may be applied to 32-bit and 8-bit altval arrays.
  /// This function deletes the whole altval array.
  /// \param tag  tag of array
  /// \return success
  bool altdel_all(char tag)
  {
    return supdel_all(tag);
  }

  //@}

  //--------------------------------------------------------------------------
  /// \name Arrays of supvals
  /// supvals: an array of arbitrary sized objects.
  /// (size of each object is limited by #MAXSPECSIZE).
  /// indexes in this array may be 8-bit or 32-bit values.
  //@{

  /// Get value of the specified supval array element.
  /// NB: do not use this function to retrieve strings, see supstr()!
  /// \param alt      index into array of supvals
  /// \param buf      output buffer, may be NULL
  /// \param bufsize  size of output buffer
  /// \param tag      tag of array. Default: stag
  /// \return size of value, -1 if element doesn't exist
  ssize_t supval(nodeidx_t alt, void *buf, size_t bufsize, char tag=stag) const
        { return netnode_supval(*this, alt, buf, bufsize, tag); }

  /// Get string value of the specified supval array element.
  /// The differences between supval() and supstr() are the following:
  ///  -# Strings are stored with the terminating zero in the old databases.
  ///     supval() returns the exact size of the stored object (with
  ///     the terminating zero) but supstr returns the string length without
  ///     the terminating zero. supstr() can handle strings stored with or
  ///     without the terminating zero.
  ///  -# supstr() makes sure that the string is terminated with 0 even if
  ///     the string was stored in the database without it or the output
  ///     buffer is too small to hold the entire string. In the latter case
  ///     the string will be truncated but still will have the terminating zero.
  ///
  /// If you do not use the string length returned by supval/supstr() functions
  /// and you are sure that the output buffer is big enough to hold the entire
  /// string and the string has been stored in the database with the terminating
  /// zero, then you can continue to use supval() instead of supstr().
  ///
  /// \param alt      index into array of supvals
  /// \param buf      output buffer, may be NULL
  /// \param bufsize  size of output buffer
  /// \param tag      tag of array. Default: stag
  /// \return length of the output string, -1 if element doesn't exist
  ssize_t supstr(nodeidx_t alt, char *buf, size_t bufsize, char tag=stag) const
        { return netnode_supstr(*this, alt, buf, bufsize, tag); }

  /// Set value of supval array element.
  /// \param alt     index into array of supvals
  /// \param value   pointer to supval value
  /// \param length  length of 'value'. If not specified, the length is calculated
  ///                using strlen()+1.
  /// \param tag     tag of array
  /// \retval 1  ok
  /// \retval 0  should not occur - indicates internal error
  bool supset(nodeidx_t alt, const void *value, size_t length=0, char tag=stag)
        { return netnode_supset(*this, alt, value, length, tag); }

  /// Delete supval element.
  /// \param alt  index into array of supvals
  /// \param tag  tag of array
  /// \retval true   deleted
  /// \retval false  element does not exist
  bool supdel(nodeidx_t alt, char tag=stag)
        { return netnode_supdel(*this, alt, tag); }

  /// Get first existing element of supval array.
  /// \param tag  tag of array
  /// \return index of first existing element of supval array,
  ///          #BADNODE if supval array is empty
  nodeidx_t sup1st(char tag=stag) const
        { return netnode_sup1st(*this, tag); }

  /// Get next existing element of supval array.
  /// \param cur  current index
  /// \param tag  tag of array
  /// \return index of the next existing element of supval array,
  ///          #BADNODE if no more supval array elements exist
  nodeidx_t supnxt(nodeidx_t cur, char tag=stag) const
        { return netnode_supnxt(*this, cur, tag); }

  /// Get last existing element of supval array.
  /// \param tag  tag of array
  /// \return index of last existing element of supval array,
  ///          #BADNODE if supval array is empty
  nodeidx_t suplast(char tag=stag) const
        { return netnode_suplast(*this, tag); }

  /// Get previous existing element of supval array.
  /// \param cur  current index
  /// \param tag  tag of array
  /// \return index of the previous existing element of supval array
  ///          #BADNODE if no more supval array elements exist
  nodeidx_t supprev(nodeidx_t cur, char tag=stag) const
        { return netnode_supprev(*this, cur, tag); }


  /// Shift the supval array elements.
  /// Moves the array elements at (from..from+size) to (to..to+size)
  /// \return number of shifted elements
  size_t supshift(nodeidx_t from, nodeidx_t to, nodeidx_t size, char tag=stag)
    { return netnode_supshift(*this, from, to, size, tag); }

  //@}

  /// \name Arrays of supvals: 8-bit indexes
  /// The following functions behave in the same manner as the functions
  /// described above. The only difference is that the array index is 8-bits
  /// and therefore the array may contains up to 256 elements only.
  //@{
  ssize_t   supval_idx8(uchar alt, void *buf, size_t bufsize, char tag) const { return netnode_supval_idx8(*this, alt, buf, bufsize, tag); }
  ssize_t   supstr_idx8(uchar alt, char *buf, size_t bufsize, char tag) const { return netnode_supstr_idx8(*this, alt, buf, bufsize, tag); }
  bool      supset_idx8(uchar alt, const void *value, size_t length, char tag) { return netnode_supset_idx8(*this, alt, value, length, tag); }
  bool      supdel_idx8(uchar alt, char tag)        { return netnode_supdel_idx8(*this, alt, tag); }
  nodeidx_t sup1st_idx8(char tag) const             { return netnode_sup1st_idx8(*this, tag); }
  nodeidx_t supnxt_idx8(uchar alt, char tag) const  { return netnode_supnxt_idx8(*this, alt, tag); }
  nodeidx_t suplast_idx8(char tag) const            { return netnode_suplast_idx8(*this, tag); }
  nodeidx_t supprev_idx8(uchar alt, char tag) const { return netnode_supprev_idx8(*this, alt, tag); }
  //@}

  /// \name Delete supvals
  //@{

  /// Delete all elements of supval array.
  /// This function may be applied to 32-bit and 8-bit supval arrays.
  /// This function deletes the whole supval array.
  /// \return success
  bool   supdel(void)
  {
    return supdel_all(stag);
  }

  /// Delete all elements of the specified supval array.
  /// This function may be applied to 32-bit and 8-bit supval arrays.
  /// This function deletes the whole supval array.
  /// \return success
  bool supdel_all(char tag)
  {
    return netnode_supdel_all(*this, tag);
  }

  /// Delete range of elements in the specified supval array.
  /// Elements in range [idx1, idx2) will be deleted.
  /// \note This function can also be used to delete a range of altval elements
  /// \param idx1  first element to delete
  /// \param idx2  last element to delete + 1
  /// \param tag   tag of array
  /// \return number of deleted elements
  int supdel_range(nodeidx_t idx1, nodeidx_t idx2, char tag)
  {
    return netnode_supdel_range(*this, idx1, idx2, tag);
  }
  /// Same as above, but accepts 8-bit indexes
  int supdel_range_idx8(uchar idx1, uchar idx2, char tag)
  {
    return netnode_supdel_range_idx8(*this, idx1, idx2, tag);
  }

  //@}

  //--------------------------------------------------------------------------
  /// \name Hashes
  /// Associative arrays indexed by strings.
  /// hashvals: Indexes in this array are strings.
  /// Values are arbitrary sized (max size is #MAXSPECSIZE)
  //@{

  /// Get value of the specified hash element.
  /// \param idx      index into hash
  /// \param buf      output buffer, may be NULL
  /// \param bufsize  output buffer size
  /// \param tag      tag of hash. Default: htag
  /// \return -1 if element doesn't exist or idx is NULL.
  ///          otherwise returns the value size in bytes
  ssize_t hashval(const char *idx, void *buf, size_t bufsize, char tag=htag) const
        { return netnode_hashval(*this, idx, buf, bufsize, tag); }

  /// Similar to  supstr(), but accepts a hash index
  ssize_t hashstr(const char *idx, char *buf, size_t bufsize, char tag=htag) const
        { return netnode_hashstr(*this, idx, buf, bufsize, tag); }

  /// Get value of the specified hash element.
  /// \param idx  index into hash
  /// \param tag  tag of hash. Default: htag
  /// \return value of hash element (it should be set using hashset(nodeidx_t)),
  ///          0 if the element does not exist
  nodeidx_t hashval_long(const char *idx, char tag=htag) const
        { return netnode_hashval_long(*this, idx, tag); }

  /// Set value of hash element.
  /// \param idx     index into hash
  /// \param value   pointer to value
  /// \param length  length of 'value'. If not specified, the length is calculated
  ///                using strlen()+1.
  /// \param tag     tag of hash. Default: htag
  /// \retval 1  ok
  /// \retval 0  should not occur - indicates internal error
  bool hashset(const char *idx, const void *value, size_t length=0, char tag=htag)
        { return netnode_hashset(*this, idx, value, length, tag); }

  /// Set value of hash element to long value.
  /// \param idx    index into hash
  /// \param value  new value of hash element
  /// \param tag    tag of hash. Default: htag
  /// \retval 1  ok
  /// \retval 0  should not occur - indicates internal error
  bool hashset(const char *idx, nodeidx_t value, char tag=htag)
        { return hashset(idx, &value, sizeof(value), tag); }

  /// Delete hash element.
  /// \param idx  index into hash
  /// \param tag  tag of hash. Default: htag
  /// \retval true   deleted
  /// \retval false  element does not exist
  bool hashdel(const char *idx, char tag=htag)
        { return netnode_hashdel(*this, idx, tag); }

  /// Get first existing element of hash.
  /// \note elements of hash are kept sorted in lexical order
  /// \param buf      output buffer, may be NULL
  /// \param bufsize  output buffer size
  /// \param tag      tag of hash. Default: htag
  /// \return size of index of first existing element of hash,
  ///          -1 if hash is empty
  ssize_t hash1st(char *buf, size_t bufsize, char tag=htag) const
        { return netnode_hash1st(*this, buf, bufsize, tag); }

  /// Get next existing element of hash.
  /// \note elements of hash are kept sorted in lexical order
  /// \param idx      current index into hash
  /// \param buf      output buffer, may be NULL
  /// \param bufsize  output buffer size
  /// \param tag      tag of hash. Default: htag
  /// \return size of index of the next existing element of hash,
  ///          -1 if no more hash elements exist
  ssize_t hashnxt(const char *idx, char *buf, size_t bufsize, char tag=htag) const
        { return netnode_hashnxt(*this, idx, buf, bufsize, tag); }

  /// Get last existing element of hash.
  /// \note elements of hash are kept sorted in lexical order
  /// \param buf      output buffer, may be NULL
  /// \param bufsize  output buffer size
  /// \param tag      tag of hash. Default: htag
  /// \return size of index of last existing element of hash,
  ///          -1 if hash is empty
  ssize_t hashlast(char *buf, size_t bufsize, char tag=htag) const
        { return netnode_hashlast(*this, buf, bufsize, tag); }

  /// Get previous existing element of supval array.
  /// \note elements of hash are kept sorted in lexical order
  /// \param idx      current index into hash
  /// \param buf      output buffer, may be NULL
  /// \param bufsize  output buffer size
  /// \param tag      tag of hash. Default: htag
  /// \return size of index of the previous existing element of hash,
  ///          -1 if no more hash elements exist
  ssize_t hashprev(const char *idx, char *buf, size_t bufsize, char tag=htag) const
        { return netnode_hashprev(*this, idx, buf, bufsize, tag); }

  /// Delete all elements of hash.
  /// This function deletes the whole hash.
  /// \param tag  tag of hash. Default: htag
  /// \return success
  bool hashdel_all(char tag=htag)
  {
    return supdel_all(tag);
  }

  //@}

  //--------------------------------------------------------------------------
  /// \name Blobs
  /// Virtually unlimited size binary objects.
  /// Blobs are stored in several supval array elements.
  //@{

  /// Get size of blob.
  /// \param _start  index of the first supval element used to store blob
  /// \param tag     tag of supval array
  /// \return number of bytes required to store a blob
  size_t blobsize(nodeidx_t _start, char tag)
  {
    return netnode_blobsize(*this, _start, tag);
  }

  /// Get blob from a netnode.
  /// \param buf              buffer to read into. if NULL, the buffer will be
  ///                         allocated using qalloc()
  /// \param[in, out] bufsize  in:  size of 'buf' in bytes (if buf == NULL then meaningless).
  ///                         out: size of the blob if it exists.
  ///                         bufsize may be NULL
  /// \param _start           index of the first supval element used to store blob
  /// \param tag              tag of supval array
  /// \return NULL if blob doesn't exist,
  ///          otherwise returns pointer to blob
  void *getblob(void *buf,
                size_t *bufsize,
                nodeidx_t _start,
                char tag)
  {
    return netnode_getblob(*this, buf, bufsize, _start, tag);
  }

  /// Store a blob in a netnode.
  /// \param buf      pointer to blob to save
  /// \param size     size of blob in bytes
  /// \param _start   index of the first supval element used to store blob
  /// \param tag      tag of supval array
  /// \return success
  bool setblob(const void *buf,
              size_t size,
              nodeidx_t _start,
              char tag)
  {
    return netnode_setblob(*this, buf, size, _start, tag);
  }

  /// Delete a blob.
  /// \param _start  index of the first supval element used to store blob
  /// \param tag     tag of supval array
  /// \return number of deleted supvals
  int delblob(nodeidx_t _start, char tag)
  {
    return netnode_delblob(*this, _start, tag);
  }

  //@}

  //--------------------------------------------------------------------------
  /// \name Links
  /// \warning links are deprecated!
  /// (We can not delete them because they are used in ancient databases)
  //@{

// GNUC v4.x complains about the following functions
#if !defined(__GNUC__)

  // Create a link between two nodes.
  //      to       - target netnode
  //      linktype - type of link to create
  //      linkspec - arbitrary text stored in the link
  // returns: 1 - ok
  int   link(netnode to, netlink linktype, const char *linkspec);

  // Delete a link between two nodes
  //      to       - target netnode
  //      linktype - type of link to create
  void  unlink(netnode to, netlink linktype);

  // Get text associated with the link
  //      to       - target netnode
  //      buf   - output buffer, may be NULL
  //      bufsize - output buffer size
  //      linktype - type of link
  // returns: -1 - the link doesn't exist
  ssize_t linkspec(netnode to, char *buf, size_t bufsize, netlink linktype) const;
#endif

  //@}

  //--------------------------------------------------------------------------
  /// \name Enumerate all netnodes
  //@{

  /// Get first netnode in the graph.
  /// Sets netnodenumber to the lowest existing number.
  /// \retval true  ok
  /// \retval false graph is empty
  bool start(void)
  {
    return netnode_start(this);
  }

  /// Get last netnode in the graph.
  /// Sets netnodenumber to the highest existing number.
  /// \retval true  ok
  /// \retval false graph is empty
  bool end(void)
  {
    return netnode_end(this);
  }

  /// Get next netnode in the graph.
  /// Sets netnodenumber to the next existing number
  /// \retval true  ok
  /// \retval false no more netnodes
  bool next(void)
  {
    return netnode_next(this);
  }

  /// Get prev netnode in the graph.
  /// Sets netnodenumber to the previous existing number
  /// \retval true  ok
  /// \retval false no more netnodes
  bool prev(void)
  {
    return netnode_prev(this);
  }

  //@}

  //--------------------------------------------------------------------------
  /// \name Move and copy netnodes
  /// \param target  the target netnode
  /// \param count   how many netnodes to copy
  /// \return number of copied/moved keys, #BADNODE if failure or not enough memory
  //@{
  size_t copyto(netnode target, nodeidx_t count=1) { return netnode_copy(netnodenumber, count, target.netnodenumber, false); }
  size_t moveto(netnode target, nodeidx_t count=1) { return netnode_copy(netnodenumber, count, target.netnodenumber, true); }
  //@}

  //--------------------------------------------------------------------------
  /// \name Netnode comparisons
  //@{
  bool operator==(netnode &n) const { return netnodenumber == n.netnodenumber; }
  bool operator!=(netnode &n) const { return netnodenumber != n.netnodenumber; }
  bool operator==(nodeidx_t x) const { return netnodenumber == x; }
  bool operator!=(nodeidx_t x) const { return netnodenumber != x; }
  //@}

  //--------------------------------------------------------------------------
  /// \name Internals
  /// \warning The following netnode definitions are for the kernel only.
  ///
  /// Functions for global base manipulating.
  //@{
  static bool createbase(const char *fname, nodeidx_t initial_nodeid=0); // Create base
  static int  checkbase(const char *fname);
#define NNBASE_OK      0        // ok
#define NNBASE_REPAIR  1        // repair database
#define NNBASE_IOERR   2        // i/o error
#define NNBASE_PAGE16  3        // 16-bit database
  static void set_close_flag(bool closeflag);   // Set "closed" flag of database
  static nodeidx_t reserve_nodes(nodeidx_t n);  // Reserve 'n' node numbers. Returns first reserved number
  static int32 validate(const char *badbase, const char *newbase, void (*cb)(uint32)); // return: number of recovered keys
  static void upgrade16(const char *oldbase, const char *newbase, void (*cb)(uint32));
  static void upgrade(void (*cb)(uint32));
  static void compress(const char *oldbase, const char *newbase, void (*cb)(uint32));

  static bool inited(void) { return netnode_inited(); }
  static bool can_write();
  static void init(const char *file, size_t cachesize, bool can_modify);
  static void flush(void);
  static linput_t *get_linput();                // must be closed by close_linput()
  static void term(void);
  static void killbase(nodeidx_t iniNumber=0);      // Clean up ALL base
  static int  getdrive(void);                   // Get current drive
  static int  getgraph(void)                    // Get current graph
                                                // (for compatibility:
               { return atag; }                 //   always returns 'A'
  static int  registerbase(const char *filename, size_t cachesize, bool writeFlag=true);
                                                // Register aux base file
                                                // 0 - too many bases
                                                // else - drive number
  static bool setbase(int drive, int graph=atag);// Set current base
                                                // Base -- got from setbasefile
                                                // 'graph' parameter is not used.
                                                // (must be equal to atag)
                                                // (for compatibility)
  static int32 validate_names(qstrvec_t *errmsgs);// check consistency of name
                                                // records, return number of
                                                // bad ones and error messages
                                                // for them
  //@}

  //--------------------------------------------------------------------------
  // Private definitions

private:

  // Number of netnode. Usually this is linear address the netnode keeps
  // information about.
  nodeidx_t netnodenumber;                  // Reference number for the node


  bool check(const char *oldname, size_t namlen=0) // Check and access node type
        { return netnode_check(this, oldname, namlen, false); }

  qstring qsupval(nodeidx_t ea, char tag) const;
  void qsupset(nodeidx_t ea, const qstring &x, char tag);
};


//-----------------------------------------------------------------------

/// The root node is used by the kernel, do not use it directly in your modules.
idaman netnode ida_export_data RootNode;             // name: "Root Node"



#pragma pack(pop)
#endif // _NETNODE_HPP
