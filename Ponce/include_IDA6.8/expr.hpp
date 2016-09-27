/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2015 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 */

#ifndef _EXPR_H
#define _EXPR_H
#pragma pack(push, 1)   // IDA uses 1 byte alignments!

/*! \file expr.hpp

  \brief Functions that deal with C-like expressions and built-in IDC language.

  Functions marked #THREAD_SAFE may be called from any thread.
  No simultaneous calls should be made for the same variable.
  We protect only global structures, individual variables must
  be protected manually.
*/

//------------------------------------------------------------------------

// Forward declarations
class idc_value_t;
class idc_class_t;
class idc_object_t;

/// IDC script extension
#define IDC_LANG_EXT             "idc"

/// Convert IDC variable to a long (32/64bit) number.
/// \return v = 0 if impossible to convert to long

idaman THREAD_SAFE error_t ida_export VarLong(idc_value_t *v);


/// Convert IDC variable to a 64bit number.
/// \return v = 0 if impossible to convert to int64

idaman THREAD_SAFE error_t ida_export VarInt64(idc_value_t *v);


/// Convert IDC variable to a long number.
/// \return
///   - v = 0         if IDC variable = "false" string
///   - v = 1         if IDC variable = "true" string
///   - v = number    if IDC variable is number or string containing a number
///   - eTypeConflict if IDC variable = empty string

idaman THREAD_SAFE error_t ida_export VarNum(idc_value_t *v);


/// Convert IDC variable to a text string

idaman THREAD_SAFE error_t ida_export VarString2(idc_value_t *v);


/// Convert IDC variable to a floating point

idaman THREAD_SAFE error_t ida_export VarFloat(idc_value_t *v);


/// Create an IDC object. The original value of 'v' is discarded (freed).
/// \param v     variable to hold the object. any previous value will be cleaned
/// \param icls  ptr to the desired class. NULL means "object" class
///              this ptr must be returned by add_idc_class() or find_idc_class()
/// \return always eOk

idaman THREAD_SAFE error_t ida_export VarObject(idc_value_t *v, const idc_class_t *icls=NULL);


/// Copy an IDC object.
/// If 'src' is not an object, simple variable assignment will be performed

idaman THREAD_SAFE error_t ida_export VarCopy(idc_value_t *dst, const idc_value_t *src);


/// Free storage used by #VT_STR2/#VT_OBJ IDC variables.
/// After this call the variable has a numeric value 0

idaman THREAD_SAFE void ida_export VarFree(idc_value_t *v);


/// Swap 2 variables

idaman THREAD_SAFE void ida_export VarSwap(idc_value_t *v1, idc_value_t *v2);


/// Retrieves the IDC object class name.
/// \param obj   class instance variable
/// \param name  qstring ptr for the class name. Can be NULL.
/// \return error code, eOk on success

idaman THREAD_SAFE error_t ida_export VarGetClassName(const idc_value_t *obj, qstring *name);


/// Get an object attribute.
/// \param obj              variable that holds an object reference.
///                         if obj is NULL it searches global variables, then user functions
/// \param attr             attribute name
/// \param res              buffer for the attribute value
/// \param may_use_getattr  may call getattr functions to calculate the attribute if it does not exist
/// \return error code, eOk on success

idaman THREAD_SAFE error_t ida_export VarGetAttr(
        const idc_value_t *obj,
        const char *attr,
        idc_value_t *res,
        bool may_use_getattr=false);


/// Set an object attribute.
/// \param obj              variable that holds an object reference.
///                         if obj is NULL then it tries to modify a global variable with the attribute name
/// \param attr             attribute name
/// \param value            new attribute value
/// \param may_use_setattr  may call setattr functions for the class
/// \return error code, eOk on success

idaman THREAD_SAFE error_t ida_export VarSetAttr(
        idc_value_t *obj,
        const char *attr,
        const idc_value_t *value,
        bool may_use_setattr=false);


/// Delete an object attribute.
/// \param obj   variable that holds an object reference
/// \param attr  attribute name
/// \return error code, eOk on success

idaman THREAD_SAFE error_t ida_export VarDelAttr(
        idc_value_t *obj,
        const char *attr);


/// \name Enumerate object attributes
//@{
idaman THREAD_SAFE const char *ida_export VarFirstAttr(const idc_value_t *obj);
idaman THREAD_SAFE const char *ida_export VarLastAttr(const idc_value_t *obj);
idaman THREAD_SAFE const char *ida_export VarNextAttr(const idc_value_t *obj, const char *attr);
idaman THREAD_SAFE const char *ida_export VarPrevAttr(const idc_value_t *obj, const char *attr);
//@}


/// Assign 'src' to 'dst'

idaman THREAD_SAFE error_t ida_export VarAssign(idc_value_t *dst, const idc_value_t *src);


/// Move 'src' to 'dst'.
/// This function is more effective than VarAssign since it never copies big
/// amounts of data.

idaman THREAD_SAFE error_t ida_export VarMove(idc_value_t *dst, idc_value_t *src);


/// Get text representation of idc_value_t

idaman bool ida_export VarPrint(
        qstring *out,
        const idc_value_t *v,
        const char *name=NULL,
        int indent=0);


/// Get slice.
/// \param v      input variable (string or object)
/// \param i1     slice start index
/// \param i2     slice end index (excluded)
/// \param res    output variable that will contain the slice
/// \param flags  \ref VARSLICE_ or 0
/// \returns eOk if success

idaman THREAD_SAFE error_t ida_export VarGetSlice(
        const idc_value_t *v,
        uval_t i1,
        uval_t i2,
        idc_value_t *res,
        int flags=0);

/// \defgroup VARSLICE_ IDC variable slice flags
/// Passed as 'flags' parameter to VarGetSlice() and VarSetSlice()
//@{
#define VARSLICE_SINGLE 0x0001  ///< return single index (i2 is ignored)
//@}

/// Set slice.
/// \param v      variable to modify (string or object)
/// \param i1     slice start index
/// \param i2     slice end index (excluded)
/// \param in     new value for the slice
/// \param flags  \ref VARSLICE_ or 0
/// \return eOk on success

idaman THREAD_SAFE error_t ida_export VarSetSlice(
        idc_value_t *v,
        uval_t i1,
        uval_t i2,
        const idc_value_t *in,
        int flags=0);


//-------------------------------------------------------------------------
// IDC class related functions

/// Create a new IDC class.
/// \param name   name of the new class
/// \param super  the base class for the new class. if the new class is not based
///               on any other class, pass NULL
/// \return pointer to the created class. If such a class already exists, a pointer
/// to it will be returned.
/// Pointers to other existing classes may be invalidated by this call.

idaman THREAD_SAFE idc_class_t *ida_export add_idc_class(
        const char *name,
        const idc_class_t *super=NULL);


/// Find an existing IDC class by its name.
/// \param name  name of the class
/// \return pointer to the class or NULL.
/// The returned pointer is valid until a new call to add_idc_class()

idaman THREAD_SAFE idc_class_t *ida_export find_idc_class(const char *name);


/// Set an IDC class method.
/// \param icls          pointer to the class
/// \param fullfuncname  name of the function to call. use full method name: classname.funcname
/// \retval true   success
/// \retval false  the function could not be found

idaman THREAD_SAFE bool ida_export set_idc_method(idc_class_t *icls, const char *fullfuncname);


/// \name Set user-defined functions to work with object attributes.
/// If the function name is NULL, the definitions are removed.
/// \return name of the old attribute function. NULL means error, "" means no previous attr func
//@{
idaman THREAD_SAFE const char *ida_export set_idc_getattr(idc_class_t *icls, const char *fullfuncname);
idaman THREAD_SAFE const char *ida_export set_idc_setattr(idc_class_t *icls, const char *fullfuncname);
//@}

/// Set a destructor for an idc class.
/// The destructor is called before deleting any object of the specified class.
/// Exceptions that escape the destructor are silently ignored, runtime errors too.

idaman THREAD_SAFE const char *ida_export set_idc_dtor(idc_class_t *icls, const char *fullfuncname);


/// Dereference a #VT_REF variable.
/// \param v           variable to dereference
/// \param vref_flags  \ref VREF_
/// \return pointer to the dereference result or NULL.
/// If returns NULL, qerrno is set to eExecBadRef "Illegal variable reference"

idaman THREAD_SAFE idc_value_t *ida_export VarDeref(idc_value_t *v, int vref_flags);

/// \defgroup VREF_ Dereference IDC variable flags
/// Passed as 'vref_flags' parameter to VarDeref()
//@{
#define VREF_LOOP 0x0000        ///< dereference until we get a non #VT_REF
#define VREF_ONCE 0x0001        ///< dereference only once, do not loop
#define VREF_COPY 0x0002        ///< copy the result to the input var (v)
//@}


/// Create a variable reference.
/// Currently only references to global variables can be created.
/// \param ref  ptr to the result
/// \param v    variable to reference
/// \return success

idaman THREAD_SAFE bool ida_export VarRef(idc_value_t *ref, const idc_value_t *v);


/// Add global IDC variable.
/// \param name  name of the global variable
/// \return pointer to the created variable or existing variable.
/// NB: the returned pointer is valid until a new global var is added.

idaman THREAD_SAFE idc_value_t *ida_export add_idc_gvar(const char *name);


/// Find an existing global IDC variable by its name.
/// \param name  name of the global variable
/// \return pointer to the variable or NULL.
/// NB: the returned pointer is valid until a new global var is added.
/// FIXME: it is difficult to use this function in a thread safe manner

idaman THREAD_SAFE idc_value_t *ida_export find_idc_gvar(const char *name);


//-------------------------------------------------------------------------
/// Class to hold idc values
class idc_value_t
{
public:
  char vtype;                   ///< \ref VT_
/// \defgroup VT_ IDC value types
/// Used by idc_value_t::vtype
//@{
#if !defined(NO_OBSOLETE_FUNCS) || defined(__EXPR_SRC)
#define  VT_STR         1       ///< String (obsolete because it cannot store zero bytes).
                                ///< See #VT_STR2
#endif
#define  VT_LONG        2       ///< Integer (see idc_value_t::num)
#define  VT_FLOAT       3       ///< Floating point (see idc_value_t::e)
#define  VT_WILD        4       ///< Function with arbitrary number of arguments.
                                ///< The actual number of arguments will be passed in idc_value_t::num.
                                ///< This value should not be used for ::idc_value_t.
#define  VT_OBJ         5       ///< Object (see idc_value_t::obj)
#define  VT_FUNC        6       ///< Function (see idc_value_t::funcidx)
#define  VT_STR2        7       ///< String (see qstr() and similar functions)
#define  VT_PVOID       8       ///< void *
#define  VT_INT64       9       ///< i64
#define  VT_REF        10       ///< Reference
//@}

#ifndef SWIG
  union
  {
#endif //SWIG
#if !defined(NO_OBSOLETE_FUNCS) || defined(__EXPR_SRC)
    char *str;                  ///< #VT_STR
#endif
    sval_t num;                 ///< #VT_LONG
    ushort e[6];                ///< #VT_FLOAT
    idc_object_t *obj;
    int funcidx;                ///< #VT_FUNC
    void *pvoid;                ///< #VT_PVOID
    int64 i64;                  ///< #VT_INT64
    uchar reserve[sizeof(qstring)]; ///< internal housekeeping: 64-bit qstring is bigger than 12 bytes
#ifndef SWIG
  };
#endif // SWIG

  /// Create a #VT_LONG value
  idc_value_t(sval_t n=0) : vtype(VT_LONG), num(n) {}
  /// Create a $VT_LONG with an existing idc value
  idc_value_t(const idc_value_t &r) : vtype(VT_LONG) { VarAssign(this, &r); }
  /// Create a #VT_STR2 value
  idc_value_t(const char *_str) : vtype(VT_STR2) { new(&qstr()) qstring(_str); }
  /// Create a #VT_STR2 value
  idc_value_t(const qstring &_str) : vtype(VT_STR2) { new(&qstr()) qstring(_str); }
  /// Destructor
  ~idc_value_t(void) { clear(); }
  /// See VarFree()
  void clear(void) { VarFree(this); } // put num 0
  /// Assign this value to an existing value
  idc_value_t &operator = (const idc_value_t &r)
  {
    VarAssign(this, &r);
    return *this;
  }
        qstring &qstr(void)       { return *(qstring *)&num; }       ///< #VT_STR2
  const qstring &qstr(void) const { return *(qstring *)&num; }       ///< #VT_STR2
  const char *c_str(void) const   { return qstr().c_str(); }         ///< #VT_STR2
  const uchar *u_str(void) const  { return (const uchar *)c_str(); } ///< #VT_STR2
  void swap(idc_value_t &v) { VarSwap(this, &v); }                   ///< Set this = r and v = this
  bool is_zero(void) const { return vtype == VT_LONG && num == 0; }  ///< Does value represent the integer 0?
  bool is_integral(void)   { return vtype == VT_LONG || vtype == VT_INT64; } ///< Does value represent a whole number?
  /// Convertible types are #VT_LONG, #VT_FLOAT, #VT_INT64, and #VT_STR2
  bool is_convertible(void) const { return (vtype >= 1 && vtype <= VT_FLOAT) || vtype == VT_STR2 || vtype == VT_INT64; }

  /// \name Warning
  /// The following functions do not free the existing data!
  /// When the contents are unknown, use the functions without a leading underscore.
  //@{
  void _create_empty_string(void) { vtype = VT_STR2; new (&qstr()) qstring; }
  void _set_string(const qstring &_str)
  {
    vtype = VT_STR2;
    new (&qstr()) qstring(_str);
  }
  void _set_string(const char *_str, size_t len)
  {
    vtype = VT_STR2;
    new (&qstr()) qstring(_str, len);
  }
  void _set_string(const char *_str)
  {
    size_t len = _str == NULL ? 0 : strlen(_str);
    _set_string(_str, len);
  }
  void _set_long(sval_t v) { vtype = VT_LONG; num = v; }
  //@}

  /// \name Setters
  /// These functions ensure the previous value is cleared
  //@{
  void create_empty_string(void) { clear(); _create_empty_string(); }
  void set_string(const char *_str, size_t len) { clear(); _set_string(_str, len); }
  void set_string(const char *_str) { clear(); _set_string(_str); }
  void set_string(const qstring &_str) { clear(); _set_string(_str); }
  void set_long(sval_t v) { clear(); _set_long(v); }
  void set_pvoid(void *p) { clear(); vtype = VT_PVOID; pvoid = p; }
  void set_int64(int64 v) { clear(); vtype = VT_INT64; i64 = v; }
  void set_float(const ushort f[6]) { clear(); vtype = VT_FLOAT; memcpy(e, f, sizeof(e)); }
  //@}
};

/// Global idc variable
struct idc_global_t
{
  qstring name;
  idc_value_t value;
  idc_global_t(void) {}
  idc_global_t(const char *n) : name(n) {}
};
typedef qvector<idc_global_t> idc_vars_t; ///< vector of global idc variables

/// Prototype of an external IDC function (implemented in C).
/// \param argv  vector of input arguments. IDA will convert all arguments
///              to types specified by extfun_t::args, except for #VT_WILD
/// \param r     return value of the function or exception
/// \return 0 if ok, all other values indicate error.
///         the error code must be set with set_qerrno():
///         - #eExecThrow - a new exception has been generated, see 'r'
///         - other values - runtime error has occurred

typedef error_t idaapi idc_func_t(idc_value_t *argv,idc_value_t *r);

#define eExecThrow 90           ///< See return value of ::idc_func_t

/// Element of functions table. See funcset_t::f
struct extfun_t
{
  const char *name;             ///< Name of function
  idc_func_t *fp;               ///< Pointer to the Function
  const char *args;             ///< Type of arguments. Terminated with 0
                                ///< #VT_WILD means a function with arbitrary
                                ///< number of arguments. Actual number of
                                ///< arguments will be passed in res->num
  int flags;                    ///< \ref EXTFUN_
/// \defgroup EXTFUN_ Function description flags
/// Used by extfun_t::flags
//@{
#define EXTFUN_BASE  0x0001     ///< requires open database.
#define EXTFUN_NORET 0x0002     ///< does not return. the interpreter may
                                ///< clean up its state before calling it.
#define EXTFUN_SAFE  0x0004     ///< thread safe function. may be called
                                ///< from any thread.
//@}
};

/// Describes an array of IDC functions
struct funcset_t
{
  int qnty;                     ///< Number of functions
  extfun_t *f;                  ///< Function table

  /// \name IDC Engine
  /// IDC engine requires the following functions (all of them may be NULL)
  //@{

  /// Start IDC engine. Called before executing any IDC code.
  error_t (idaapi *startup)(void);

  /// Stop IDC engine. Called when all IDC engines finish.
  /// In other words, nested IDC engines do not call startup/shutdown.
  error_t (idaapi *shutdown)(void);

  /// Initialize IDC engine. Called once at the very beginning of work.
  /// This callback may create additional IDC classes, methods, etc.
  void (idaapi *init_idc)(void);

  /// Terminate IDC engine. Called once at the very end of work.
  void (idaapi *term_idc)(void);

  /// Is the database open? (used for #EXTFUN_BASE functions).
  /// if this pointer is NULL, #EXTFUN_BASE is not checked.
  bool (idaapi *is_database_open)(void);

  /// Convert an address to a string.
  /// if this pointer is NULL, '%a' will be used.
  size_t (idaapi *ea2str)(ea_t ea, char *buf, size_t bufsize);

  /// Should a variable name be accepted without declaration?.
  /// When the parser encounters an unrecognized variable, this callback is called.
  /// If it returns false, the parser generates the 'undefined variable' error
  /// else the parser generates code to call to a set or get function,
  /// depending on the current context.
  /// If this pointer is NULL, undeclared variables won't be supported.
  /// However, if 'getname' function is provided to the parser, it will be used
  /// to resolve such names to constants at the compilation time.
  /// This callback is used by IDA to handle processor register names.
  bool (idaapi *undeclared_variable_ok)(const char *name);

  //@}

  /// \name Indexes
  /// Indexes into the 'f' array. non-positive values mean that the function does not exist
  //@{

  /// Retrieve value of an undeclared variable.
  /// Expected prototype: get(#VT_STR2 varname)
  int get_unkvar;

  /// Store a value to an undeclared variable.
  /// Expected prototype: set(#VT_WILD new_value, #VT_STR2 varname)
  int set_unkvar;

  /// Execute resolved function.
  /// If 'getname' was used to resolve an unknown name to a constant in a function
  /// call context, such a call will be redirected here.
  /// Expected prototype: exec_resolved_func(#VT_LONG func, #VT_WILD typeinfo, ...)
  /// This callback is used in IDA for Appcall.
  int exec_resolved_func;

  /// Calculate sizeof(type).
  /// This function is used by the interpreter to calculate sizeof() expressions.
  /// Please note that the 'type' argument is an IDC object of typeinfo class.
  /// Expected prototype: calc_sizeof(#VT_OBJ typeinfo)
  /// This callback requires support of the type system (available only in IDA kernel)
  /// It should not be used by standalone IDC interpreters.
  int calc_sizeof;

  /// Get address of the specified field using the type information from the idb.
  /// This function is used to resolve expressions like 'mystr.field' where
  /// mystr does not represent an IDC object but just a plain number.
  /// The number is interpreted as an address in the current idb.
  /// This function retrieves type information at this address and tried to find
  /// the specified 'field'. It returns the address of the 'field' in the idb.
  /// This callback should not be used by standalone IDC interpreters.
  int get_field_ea;

  //@}
};

// Our idc_value_t and idc_global_t classes are freely movable with memcpy()
DECLARE_TYPE_AS_MOVABLE(idc_value_t);
DECLARE_TYPE_AS_MOVABLE(idc_global_t);

//------------------------------------------------------------------------

/// Array of built-in IDA functions
idaman funcset_t ida_export_data IDCFuncs;


/// Add/remove a built-in IDC function.
/// This function does not modify the predefined kernel functions
/// Example:
/// \code
///  static const char myfunc5_args[] = { VT_LONG, VT_STR, 0 };
///
///  static error_t idaapi myfunc5(idc_value_t *argv, idc_value_t *res)
///  {
///    msg("myfunc is called with arg0=%a and arg1=%s\n", argv[0].num, argv[1].str);
///    res->num = 5;     // let's return 5
///    return eOk;
///  }
///
///  // after this:
///  set_idc_func("MyFunc5", myfunc5, myfunc5_args);
///
///  // there is a new IDC function which can be called like this:
///  MyFunc5(0x123, "test");
///
/// \endcode
/// \param name           function name to modify
/// \param fp             pointer to the function which will handle this IDC function.
///                       == NULL: remove the specified function
/// \param args           prototype of the function, zero terminated array of \ref VT_
/// \param extfunc_flags  \ref EXTFUN_ or 0
/// \return success

idaman THREAD_SAFE bool ida_export set_idc_func_ex(
        const char *name,
        idc_func_t *fp,
        const char *args,
        int extfunc_flags);

//------------------------------------------------------------------------
/// External language (to support third party language interpreters)
struct extlang_t
{
  size_t size;                  ///< Size of this structure
  uint32 flags;                 ///< Language features, currently 0
  const char *name;             ///< Language name

  /// Compile an expression.
  /// \param name         name of the function which will
  ///                     hold the compiled expression
  /// \param current_ea   current address. if unknown then #BADADDR
  /// \param expr         expression to compile
  /// \param[out] errbuf  error message if compilation fails
  /// \param errbufsize   size of the error buffer
  /// \return success
  bool (idaapi *compile)(
        const char *name,
        ea_t current_ea,
        const char *expr,
        char *errbuf,
        size_t errbufsize);

  /// Evaluate a previously compiled expression.
  /// \param name         function to run
  /// \param nargs        number of input arguments
  /// \param args         input arguments
  /// \param[out] result  function result or exception
  /// \param[out] errbuf  error message if evaluation fails
  /// \param errbufsize   size of the error buffer
  /// \return success
  bool (idaapi *run)(
        const char *name,
        int nargs,
        const idc_value_t args[],
        idc_value_t *result,
        char *errbuf,
        size_t errbufsize);

  /// Compile and evaluate an expression.
  /// \param current_ea   current address. if unknown then BADADDR
  /// \param expr         expression to evaluate
  /// \param[out] rv      expression value or exception
  /// \param[out] errbuf  error message if evaluation fails
  /// \param errbufsize   size of the error buffer
  /// \return success
  bool (idaapi *calcexpr)(
        ea_t current_ea,
        const char *expr,
        idc_value_t *rv,
        char *errbuf,
        size_t errbufsize);

  /// Compile (load) a file.
  /// \param file         file name
  /// \param[out] errbuf  error message if compilation fails
  /// \param errbufsize   size of the error buffer
  bool (idaapi *compile_file)(
        const char *file,
        char *errbuf,
        size_t errbufsize);

  const char *fileext;          ///< File name extension for the language

  /// Create an object instance.
  /// \param name        object class name
  /// \param nargs       number of input arguments
  /// \param args        input arguments
  /// \param result      created object or exception
  /// \param errbuf      error message if evaluation fails
  /// \param errbufsize  size of the error buffer
  /// \return success
  bool (idaapi *create_object)(
        const char *name,
        int nargs,
        const idc_value_t args[],
        idc_value_t *result,
        char *errbuf,
        size_t errbufsize);

  /// Returns the attribute value of a given object from the global scope.
  /// \param obj          object (may be NULL)
  /// \param attr         attribute name.
  ///                     if NULL or empty string then the object instance name
  ///                     (i.e. class name) should be returned.
  /// \param[out] result  attribute value
  /// \return success
  bool (idaapi *get_attr)(
        const idc_value_t *obj,
        const char *attr,
        idc_value_t *result);

  /// Sets the attribute value of a given object in the global scope.
  /// \param obj    object (may be NULL)
  /// \param attr   attribute name
  /// \param value  attribute value
  /// \return success
  bool (idaapi *set_attr)(
        idc_value_t *obj,
        const char *attr,
        idc_value_t *value);

  /// Calls a member function.
  /// \param obj          object instance
  /// \param name         method name to call
  /// \param nargs        number of input arguments
  /// \param args         input arguments
  /// \param[out] result  function result or exception
  /// \param[out] errbuf  error message if evaluation fails
  /// \param errbufsize   size of the error buffer
  /// \return success
  bool (idaapi *call_method)(
    const idc_value_t *obj,
    const char *name,
    int nargs,
    const idc_value_t args[],
    idc_value_t *result,
    char *errbuf,
    size_t errbufsize);

  /// Compile and execute a string with statements.
  /// (see also: calcexpr() which works with expressions)
  /// \param str          input string to execute
  /// \param[out] errbuf  error message
  /// \param errbufsize   size of the error buffer
  /// \return success
  bool (idaapi *run_statements)(
    const char *str,
    char *errbuf,
    size_t errbufsize);

  /// Compile (load) a file with processor module.
  /// If is absent then compile_file() is used.
  /// \param file          processor module file name
  /// \param[out] procobj  created object or exception
  /// \param[out] errbuf   error message if compilation fails
  /// \param errbufsize    size of the error buffer
  /// \retval true   success
  /// \retval false  if errbuf is empty then file has been
  ///                 loaded (compiled) successfully but
  ///                it doesn't contain processor module
  bool (idaapi *load_procmod)(
        const char *file,
        idc_value_t *procobj,
        char *errbuf,
        size_t errbufsize);

  /// Unload previously loaded processor module.
  /// \param file          processor module file name
  /// \param[out] errbuff  error message if compilation fails
  /// \param errbufsize    size of the error buffer
  /// \return success
  bool (idaapi *unload_procmod)(
        const char *file,
        char *errbuf,
        size_t errbufsize);
};
typedef qvector<const extlang_t *> extlangs_t; ///< vector of external language descriptions

idaman ida_export_data const extlang_t *extlang; ///< current active external language


/// Install an external language interpreter.
/// Any previously registered interpreter will be automatically unregistered.
/// The installed extlang can be used in select_extlang().
/// \param el  description of the new language. must point to static storage.
/// \return success

idaman bool ida_export install_extlang(const extlang_t *el);


/// Uninstall an external language interpreter.
/// \return success

idaman bool ida_export remove_extlang(const extlang_t *el);


/// Selects the external language interpreter.
/// The specified extlang must be registered before selecting it.
/// It will be used to evaluate expressions entered in dialog boxes.
/// It will also replace the calcexpr() and calcexpr_long() functions.
/// \return success

idaman bool ida_export select_extlang(const extlang_t *el);


/// Get the file extension for the current language

inline const char *get_extlang_fileext(void)
{
  const extlang_t *el = extlang;
  if ( el != NULL && el->size > qoffsetof(extlang_t, fileext) )
    return el->fileext;
  return NULL;
}


/// Get the list of the registered extlangs

idaman const extlangs_t *ida_export get_extlangs();


/// Get the extlang that can handle the given file extension

idaman const extlang_t *ida_export find_extlang_by_ext(const char *ext);


/// Find an extlang by name

idaman const extlang_t *ida_export find_extlang_by_name(const char *name);

//------------------------------------------------------------------------

/// Get name of directory that contains IDC scripts.
/// This directory is pointed by IDCPATH environment variable or
/// it is in IDC subdirectory in IDA directory

idaman THREAD_SAFE const char *ida_export get_idcpath(void);


/// Set or append a header path.
/// IDA looks for the include files in the appended header paths,
/// then in the ida executable directory.
/// \param path  list of directories to add (separated by ';')
///              may be NULL, in this case nothing is added
/// \param add   true: append.
///              false: remove old paths.
/// \retval true   success
/// \retval false  no memory

idaman THREAD_SAFE bool ida_export set_header_path(const char *path, bool add);


/// Get full name of IDC file name.
/// Search for file in list of include directories, IDCPATH directory
/// and the current directory.
/// \param buf      buffer for the answer
/// \param bufsize  buffer size
/// \param file    file name without full path
/// \return NULL is file not found.
///          otherwise returns pointer to buf

idaman THREAD_SAFE char *ida_export get_idc_filename(
        char *buf,
        size_t bufsize,
        const char *file);


/// Compile and execute "main" function from system file.
/// \param file  file name with IDC function(s).
///              The file will be searched in:
///                      - the current directory
///                      - IDA.EXE directory
///                      - in PATH
/// \param complain_if_no_file
///              - 1: display warning if the file is not found
///              - 0: don't complain if file doesn't exist
/// \retval 1  ok, file is compiled and executed
/// \retval 0  failure, compilation or execution error, warning is displayed

idaman THREAD_SAFE bool ida_export dosysfile(bool complain_if_no_file, const char *file);


/// Compile and calculate an expression.
/// \param where        the current linear address in the addressing space of the
///                     program being disassembled. it will be used to resolve
///                     names of local variables, etc.
///                     if not applicable, then should be #BADADDR
/// \param line         a text line with IDC expression
/// \param res          pointer to result. The result will be converted
///                     to 32/64bit number. Use calcexpr() if you
///                     need the result of another type.
/// \param[out] errbuf  buffer for the error message
/// \param errbufsize   size of errbuf
/// \retval true   ok
/// \retval false  error, see errbuf

idaman bool ida_export calcexpr_long(
        ea_t where,
        const char *line,
        sval_t *res,
        char *errbuf,
        size_t errbufsize);

/// See calcexpr_long()

inline bool idaapi calcexpr_long(
        ea_t where,
        const char *line,
        uval_t *res,
        char *errbuf,
        size_t errbufsize)
{
  return calcexpr_long(where, line, (sval_t *)res, errbuf, errbufsize);
}


/// Compile and calculate an expression.
/// \param where        the current linear address in the addressing space of the
///                     program being disassembled. If will be used to resolve
///                     names of local variables etc.
///                     if not applicable, then should be #BADADDR.
/// \param line         the expression to evaluate
/// \param rv           pointer to the result
/// \param[out] errbuf  buffer for the error message
/// \param errbufsize   size of errbuf
/// \retval true   ok
/// \retval false  error, see errbuf

idaman bool ida_export calcexpr(
        ea_t where,
        const char *line,
        idc_value_t *rv,
        char *errbuf,
        size_t errbufsize);


/// Same as calcexpr(), but will always use the IDC interpreter regardless of the
/// currently installed extlang. One subtle difference: the current value of rv
/// will be discarded while calcexpr() frees it before storing the return value.

idaman bool ida_export calc_idc_expr(
        ea_t where,
        const char *buf,
        idc_value_t *rv,
        char *errbuf,
        size_t errbufsize);


/// Compile and execute IDC expression.
/// \param line  a text line with IDC expression
/// \retval 1    ok
/// \retval 0    failure, a warning message is displayed

idaman bool ida_export execute(const char *line);


/// Compile a text file with IDC function(s).
/// \param file         name of file to compile
///                     if NULL, then "File not found" is returned.
/// \param cpl_flags    \ref CPL_ or 0
/// \param[out] errbuf  buffer for the error message
/// \param errbufsize   size of errbuf
/// \retval true   ok
/// \retval false  error, see errbuf

idaman THREAD_SAFE bool ida_export CompileEx(
        const char *file,
        int cpl_flags,
        char *errbuf,
        size_t errbufsize);

/// \defgroup CPL_ Compile IDC file flags
/// Passed as 'cpl_flags' parameter to CompileEx()
//@{
#define CPL_DEL_MACROS 0x0001  ///< delete macros at the end of compilation
#define CPL_USE_LABELS 0x0002  ///< allow program labels in the script
#define CPL_ONLY_SAFE  0x0004  ///< allow calls of only thread-safe functions
//@}


/// Compile a text file with IDC functions
/// (Also see CompileEx()).

inline bool idaapi Compile(
        const char *file,
        char *errbuf,
        size_t errbufsize)
{
  return CompileEx(file, CPL_DEL_MACROS|CPL_USE_LABELS, errbuf, errbufsize);
}


/// Does the extlang_t::compile_file() callback exist?

inline bool idaapi extlang_compile_file_exists(const extlang_t *el = NULL)
{
  if ( el == NULL )
    el = extlang;
  return el != NULL
      && el->size > qoffsetof(extlang_t, compile_file)
      && el->compile_file != NULL;
}


/// Compiles a script using the active extlang or with Compile() if no extlang is active

inline bool compile_script_file(
        const char *file,
        char *errbuf,
        size_t errbufsize)
{
  bool (idaapi *func)(const char *, char *, size_t);
  func = extlang_compile_file_exists() ? extlang->compile_file : Compile;
  return func(file, errbuf, errbufsize);
}


/// Unload processor module

inline bool extlang_unload_procmod(
        const char *file,
        char *errbuf,
        size_t errbufsize)
{
  if ( extlang != NULL
    && extlang->size > qoffsetof(extlang_t, unload_procmod)
    && extlang->unload_procmod != NULL )
  {
    return extlang->unload_procmod(file, errbuf, errbufsize);
  }
  return true;
}


/// Compiles a file using the appropriate extlang, otherwise Compile() is used.
/// (extlang is determined based on the extension of the file)
/// \param file         script file name (can't be NULL!)
/// \param[out] errbuf  buffer for the error message
/// \param errbufsize   size of errbuf
/// \param el           the extlang that was used to compile the script.
///                     NULL indicates that IDC was used.
/// \retval true   ok
/// \retval false  error, see errbuf

idaman bool ida_export extlang_compile_file(
        const char *file,
        char *errbuf,
        size_t errbufsize,
        const extlang_t **el);


/// Compile one text line with IDC function(s).
/// \param line             line with IDC function(s) (can't be NULL!)
/// \param[out] errbuf      buffer for the error message
/// \param bufsize          size of errbuf
/// \param _getname         callback function to get values of undefined variables
///                         This function will be called if IDC function contains
///                         references to undefined variables. May be NULL.
/// \param only_safe_funcs  if true, any calls to functions without #EXTFUN_SAFE flag
///                         will lead to a compilation error.
/// \retval true   ok
/// \retval false  error, see errbuf

idaman THREAD_SAFE bool ida_export CompileLineEx(
        const char *line,
        char *errbuf,
        size_t bufsize,
        uval_t (idaapi*_getname)(const char *name)=NULL,
        bool only_safe_funcs=false);


/// Compile idc or extlang function

idaman bool ida_export compile_script_func(
        const char *name,
        ea_t current_ea,
        const char *expr,
        char *errbuf,
        size_t errbufsize);


// Execution of IDC code can generate exceptions. Exception objects
// will have the following attributes:
//      file - the source file name
//      line - the line number that was executing when the exception occurred
//      func - the function name
//      pc   - bytecode program counter
// For runtime errors, the following additional attributes exist:
//      qerrno - runtime error code
//      description - text description of the runtime error

/// Execute an IDC function.
/// \param fname        function name. User-defined functions, built-in functions,
///                     and plugin-defined functions are accepted.
/// \param argsnum      number of parameters to pass to 'fname'.
///                     This number should be equal to number of parameters
///                     the function expects.
/// \param args         array of parameters
/// \param[out] result  pointer to idc_value_t to hold the return value of the function.
///                     If execution fails, this variable will contain
///                     the exception information.
///                     Can be NULL if return value is not required.
/// \param[out] errbuf  buffer for the error message
/// \param errbufsize   size of errbuf
/// \retval true   ok
/// \retval false  error, see errbuf

idaman bool ida_export Run(
        const char *fname,
        int argsnum,
        const idc_value_t args[],
        idc_value_t *result,
        char *errbuf,
        size_t errbufsize);


/// Execute idc or extlang function

inline bool run_script_func(
        const char *fname,
        int argsnum,
        const idc_value_t args[],
        idc_value_t *result,
        char *errbuf,
        size_t errbufsize)
{
  bool (idaapi *func)(const char *, int, const idc_value_t[], idc_value_t *,
                                                               char *, size_t);
  func = extlang != NULL ? extlang->run : Run;
  return func(fname, argsnum, args, result, errbuf, errbufsize);
}



/// Create an IDC object.
/// \param name         class name. May be NULL for the built-in object_t class.
/// \param argsnum      number of arguments to pass to object constructor.
/// \param args         array of arguments
/// \param[out] result  pointer to idc_value_t to hold the created object.
///                     If execution fails, this variable will contain
///                     the exception information.
/// \param[out] errbuf  buffer for the error message
/// \param errbufsize   size of errbuf
/// \retval true   ok
/// \retval false  error, see errbuf and exception info in 'result'

idaman bool ida_export create_idc_object(
        const char *name,
        int argsnum,
        const idc_value_t args[],
        idc_value_t *result,
        char *errbuf,
        size_t errbufsize);


/// Does the get_attr() extlang callback exist?

inline bool idaapi extlang_get_attr_exists(void)
{
  const extlang_t *el = extlang;
  return el != NULL
      && el->size > qoffsetof(extlang_t, get_attr)
      && el->get_attr != NULL;
}


/// Get idc or extlang object attribute

inline bool get_script_attr(
        const idc_value_t *obj,
        const char *attr,
        idc_value_t *result)
{
  return extlang_get_attr_exists() ? extlang->get_attr(obj, attr, result) : VarGetAttr(obj, attr, result) == eOk;
}


/// Does the set_attr() extlang callback exist?

inline bool idaapi extlang_set_attr_exists(void)
{
  const extlang_t *el = extlang;
  return el != NULL
      && el->size > qoffsetof(extlang_t, set_attr)
      && el->set_attr != NULL;
}


/// Set idc or extlang object attribute

inline bool set_script_attr(
        idc_value_t *obj,
        const char *attr,
        idc_value_t *value)
{
  return extlang_set_attr_exists() ? extlang->set_attr(obj, attr, value) : VarSetAttr(obj, attr, value) == eOk;
}


/// Does the create_object() extlang callback exist?

inline bool idaapi extlang_create_object_exists(void)
{
  const extlang_t *el = extlang;
  return el != NULL
      && el->size > qoffsetof(extlang_t, create_object)
      && el->create_object != NULL;
}


/// Create idc or extlang object

inline bool create_script_object(
        const char *name,
        int nargs,
        const idc_value_t args[],
        idc_value_t *result,
        char *errbuf,
        size_t errbufsize)
{
  bool (idaapi *func)(const char *, int,
                        const idc_value_t [], idc_value_t *, char *, size_t);
  func = extlang_create_object_exists() ? extlang->create_object : create_idc_object;
  return func(name, nargs, args, result, errbuf, errbufsize);
}


/// Call an IDC object method.
/// \param obj          object. if NULL and name != NULL, a gvar or global func
///                     specified by 'name' will be called.
/// \param name         name of the method to call. if NULL, obj must be a function
///                     reference. the referenced function will be called.
///                     both obj and name can not be NULL.
/// \param nargs        number of arguments to pass to method.
/// \param args         array of arguments. 'this' argument will be supplied by ida.
/// \param[out] result  pointer to idc_value_t to hold the created object.
///                     If execution fails, this variable will contain
///                     the exception information.
/// \param[out] errbuf  buffer for the error message
/// \param errbufsize   size of errbuf
/// \retval true   ok
/// \retval false  error, see errbuf and exception info in 'result'

idaman bool ida_export call_idc_method(
        const idc_value_t *obj,
        const char *name,
        int nargs,
        const idc_value_t args[],
        idc_value_t *result,
        char *errbuf,
        size_t errbufsize);


/// Does the call_method() extlang callback exist?

inline bool idaapi extlang_call_method_exists(void)
{
  const extlang_t *el = extlang;
  return el != NULL
    && el->size > qoffsetof(extlang_t, call_method)
    && el->call_method != NULL;
}


/// Call a member function of a script object.
/// \param obj          object instance
/// \param name         method name to call
/// \param nargs        number of input arguments
/// \param args         input arguments
/// \param[out] result  function result or exception
/// \param[out] errbuf  error message if evaluation fails
/// \param errbufsize   size of the error buffer
/// \retval true   ok
/// \retval false  error, see errbuf and exception info in 'result'

inline bool idaapi call_script_method(
        const idc_value_t *obj,
        const char *name,
        int nargs,
        const idc_value_t args[],
        idc_value_t *result,
        char *errbuf,
        size_t errbufsize)
{
  bool (idaapi *func)(const idc_value_t *, const char *, int,
                      const idc_value_t [], idc_value_t *, char *, size_t);
  func = extlang_call_method_exists() ? extlang->call_method : call_idc_method;
  return func(obj, name, nargs, args, result, errbuf, errbufsize);
}


/// Does the extlang_t::run_statements callback exist?

inline bool idaapi extlang_run_statements_exists(const extlang_t *elang = NULL)
{
  const extlang_t *el = elang == NULL ? extlang : elang;
  return el != NULL
    && el->size > qoffsetof(extlang_t, run_statements)
    && el->run_statements != NULL;
}

/// Run statements using extlang.
/// \param str          input string to execute
/// \param[out] errbuf  error message if evaluation fails
/// \param errbufsize   size of the error buffer
/// \param elang        optional extlang to use.

idaman bool ida_export run_statements(
        const char *str,
        char *errbuf,
        size_t errbufsize,
        const extlang_t *elang = NULL);


/// Compile and execute IDC function(s) on one line of text.
/// \param line         text of IDC functions
/// \param func         function name to execute
/// \param getname      callback function to get values of undefined variables
///                     This function will be called if IDC function contains
///                     references to a undefined variable. May be NULL.
/// \param argsnum      number of parameters to pass to 'fname'
///                     This number should be equal to the number of parameters
///                     the function expects.
/// \param args         array of parameters
/// \param result       ptr to idc_value_t to hold result of the function.
///                     If execution fails, this variable will contain
///                     the exception information.
///                     You may pass NULL if you are not interested in the returned
///                     value.
/// \param[out] errbuf  buffer for the error message
/// \param errbufsize  size of errbuf
/// \retval true   ok
/// \retval false  error, see errbuf

idaman THREAD_SAFE bool ida_export ExecuteLine(
                const char *line,
                const char *func,
                uval_t (idaapi*getname)(const char *name),
                int argsnum,
                const idc_value_t args[],
                idc_value_t *result,                // may be NULL. Any previous
                                                // value is DISCARDED (not freed)
                char *errbuf,
                size_t errbufsize);


/// Compile and execute IDC function(s) from file.
/// \param file         text file containing text of IDC functions
/// \param func         function name to execute
/// \param argsnum      number of parameters to pass to 'fname'
///                     This number should be equal to number of parameters
///                     the function expects.
/// \param args         array of parameters
/// \param result       ptr to idc_value_t to hold result of the function.
///                     If execution fails, this variable will contain
///                     the exception information.
///                     You may pass NULL if you are not interested in the returned
///                     value.
/// \param[out] errbuf  buffer for the error message
/// \param errbufsize   size of errbuf
/// \retval true   ok
/// \retval false  error, see errbuf

idaman THREAD_SAFE bool ida_export ExecuteFile(
                const char *file,
                const char *func,
                int argsnum,
                const idc_value_t args[],
                idc_value_t *result,                // may be NULL. Any previous
                                                // value is DISCARDED (not freed)
                char *errbuf,
                size_t errbufsize);


/// Add a compiled IDC function to the pool of compiled functions.
/// This function makes the input function available to be executed.
/// \param name  name of the function
/// \param narg  number of the function parameters
/// \param body  compiled body of the function
/// \param len   length of the function body in bytes.
/// \return success (may fail on funcs that are being executed/compiled)

idaman THREAD_SAFE bool ida_export set_idc_func_body(
                const char *name,
                int narg,
                const uchar *body,
                size_t len);


/// Get the body of a compiled IDC function.
/// \param name  name of the function
/// \param narg  pointer to the number of the function parameters (out)
/// \param len   out: length of the function body (may be NULL)
/// \return pointer to the buffer with the function body.
///         buffer will be allocated using qalloc().
///         NULL indicates failure (no such defined function)

idaman THREAD_SAFE uchar *ida_export get_idc_func_body(
                const char *name,
                int *narg,
                size_t *len);


//------------------------------------------------------------------------
/// Setup lowcnd callbacks to read/write registers.
/// These callbacks will be used by the idc engine to read/write registers
/// while calculating low level breakpoint conditions for local debuggers.

idaman void ida_export setup_lowcnd_regfuncs(idc_func_t *getreg, idc_func_t *setreg);

//------------------------------------------------------------------------



#ifndef NO_OBSOLETE_FUNCS
typedef idc_value_t value_t;
idaman DEPRECATED error_t ida_export VarString(idc_value_t *v);
idaman DEPRECATED bool ida_export set_idc_func(const char *name, idc_func_t *fp, const char *args);
idaman DEPRECATED bool ida_export CompileLine(const char *line, char *errbuf, size_t errbufsize, uval_t (idaapi*_getname)(const char *name)=NULL);
#endif

#pragma pack(pop)
#endif /* _EXPR_H */
