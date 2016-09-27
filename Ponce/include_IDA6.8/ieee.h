/*
 *      Interactive disassembler (IDA)
 *      Copyright (c) 1990-2015 Hex-Rays
 *      ALL RIGHTS RESERVED.

 *      Floating Point Number Libary.
 *      Copyright (c) 1995-2006 by Iouri Kharon.
 *                        E-mail: yjh@styx.cabel.net
 *
 */

#ifndef _IEEE_H_
#define _IEEE_H_
#pragma pack(push, 1)

/*! \file ieee.h

  \brief IEEE floating point functions

*/

//-------------------------------------------------------------------
/// Number of 16 bit words in ::eNE
#define IEEE_NE 6

/// Number of 16 bit words in ::eNI
#define IEEE_NI (IEEE_NE+3)

//==========================================================================
/// Array offset to exponent
#define IEEE_E 1

/// Array offset to high guard word
#define IEEE_M 2

/// The exponent of 1.0
#define IEEE_EXONE (0x3fff)

//===================================================================
/// External x type format
typedef uint16 eNE[IEEE_NE];

/// Internal format:
///  - 0 : sign (0/1)
///  - 1 : exponent (based of #IEEE_EXONE). If exp = 0, value = 0.
///  - 2 : high word of mantissa (always zero after normalize)
typedef uint16 eNI[IEEE_NI];

/// 0.0
idaman const eNE ida_export_data ieee_ezero;

/// 1.0
idaman const eNE ida_export_data ieee_eone;

/// 2.0
extern const eNE ieee_etwo;

/// 32.0
extern const eNE ieee_e32;

/// 6.93147180559945309417232121458176568075500134360255E-1
extern const eNE ieee_elog2;

/// 1.41421356237309504880168872420969807856967187537695E0
extern const eNE ieee_esqrt2;

/// 2/sqrt(PI) =  1.12837916709551257389615890312154517168810125865800E0
extern const eNE ieee_eoneopi;

/// 3.14159265358979323846264338327950288419716939937511E0
extern const eNE ieee_epi;

/// 5.7721566490153286060651209008240243104215933593992E-1
extern const eNE ieee_eeul;


/// Clear (zero-out) the given value

inline void ecleaz(eNI x) { memset(x, 0, sizeof(eNI)); }


/// Move eNI => eNE

idaman THREAD_SAFE void ida_export emovo(const eNI a, eNE b);


/// Move eNE => eNI

idaman THREAD_SAFE void ida_export emovi(const eNE a, eNI b);


/// Shift NI format up (+) or down

idaman THREAD_SAFE int  ida_export eshift(eNI x, int sc);


/// Normalize and round off.
/// \param s         the internal format number to be rounded
/// \param lost      indicates whether or not the number is exact.
///                  this is the so-called sticky bit.
/// \param subflg    indicates whether the number was obtained
///                  by a subtraction operation.  In that case if lost is nonzero
///                  then the number is slightly smaller than indicated.
/// \param exp       the biased exponent, which may be negative.
///                  the exponent field of "s" is ignored but is replaced by
///                  "exp" as adjusted by normalization and rounding.
/// \param rndbase   if 0 => is the rounding control.
///                  else    is processor defined base (rndprc)
/// \return success

idaman THREAD_SAFE int  ida_export emdnorm(eNI s, int lost, int subflg, int32 exp, int rndbase);


/// \name Prototypes
/// IDP module function prototypes -- should be implemented in idp
//@{

/// Floating point conversion function: implemented by \ph{realcvt()}.
/// \param m    pointer to data
/// \param e    internal IEEE format data
/// \param swt  operation:
///               - 000: load trunc. float (DEC ^F)    2 bytes (m->e)
///               - 001: load float                    4 bytes (m->e)
///               - 003: load double                   8 bytes (m->e)
///               - 004: load long double             10 bytes (m->e)
///               - 005: load long double             12 bytes (m->e)
///               - 010: store trunc. float (DEC ^F)   2 bytes (e->m)
///               - 011: store float                   4 bytes (e->m)
///               - 013: store double                  8 bytes (e->m)
///               - 014: store long double            10 bytes (e->m)
///               - 015: store long double            12 bytes (e->m)
/// \retval  0  ok
/// \retval -1  not supported format for current .idp
/// \retval -2  number too big (small) for store (mem NOT modified)
/// \retval -3  illegal real data for load (IEEE data not filled)

int idaapi realcvt(void *m, eNE e, uint16 swt);


/// Little endian

int l_realcvt(void *m, eNE e, uint16 swt);


/// Big endian

int b_realcvt(void *m, eNE e, uint16 swt);

//@}


/// Standard IEEE 754 floating point conversion function.
/// See comment for realcvt().

idaman THREAD_SAFE int ida_export ieee_realcvt(void *m, eNE e, uint16 swt);


/// \name Misc arithmetic/conversion functions
/// \retval 0  ok
/// \retval 1  overfloat / underfloat
/// \retval 2  illegal data (asctoreal())
/// \retval 3  divide by 0 (ediv())
/// \retval 4  too big for integer (eetol())
//@{

/// IEEE to ascii string.
/// \param mode  broken down into:
///                - low byte: number of digits after '.'
///                - second byte: FPNUM_LENGTH
///                - third byte: FPNUM_DIGITS

idaman THREAD_SAFE void ida_export realtoasc(const eNE x, char *buf, size_t bufsize, uint mode);


/// ascii string to IEEE

idaman THREAD_SAFE int ida_export asctoreal(const char **sss, eNE y);


/// long to IEEE

idaman THREAD_SAFE void ida_export eltoe(sval_t l, eNE e);


/// int64 to IEEE

idaman THREAD_SAFE void ida_export eltoe64(int64 l, eNE e);


/// uint64 to IEEE

idaman THREAD_SAFE void ida_export eltoe64u(uint64 l, eNE e);


/// IEEE to long (+-0.5 if flg)

idaman THREAD_SAFE int ida_export eetol(const eNE a, sval_t *l, bool roundflg);


/// IEEE to long (+-0.5 if flg)

idaman THREAD_SAFE int ida_export eetol64(const eNE a, int64 *l, bool roundflg);


/// IEEE to ulong (+-0.5 if flg)

idaman THREAD_SAFE int ida_export eetol64u(const eNE a, uint64 *l, bool roundflg);


/// b = a*(2**pwr2)

idaman THREAD_SAFE int ida_export eldexp(const eNE a, int32 pwr2, eNE b);


/// if(!subflg) c = a + b,
/// else        c = a - b

idaman THREAD_SAFE int ida_export eadd(const eNE a, const eNE b, eNE c, int subflg);


/// c = a * b

idaman THREAD_SAFE int ida_export emul(const eNE a, const eNE b, eNE c);


/// c = a / b

idaman THREAD_SAFE int ida_export ediv(const eNE a, const eNE b, eNE c);


// predefined functions
/// \cond
void eclear(eNE a);
void emov(eNE a, eNE b);
void eabs(eNE x);
int  esign(eNE x);
/// \endcond

/// x = 0
#define eclear(a) memset(a, 0, sizeof(eNE))

/// b = a
#define emov(a, b) memcpy(b, a, sizeof(eNE))

/// x = |x|
#define eabs(x) (x[IEEE_NE-1] &= 0x7fff)

#ifdef __cplusplus
/// x = -x
inline void eneg(eNE x)
{
  if(x[IEEE_NE-1])
    x[IEEE_NE-1] ^= 0x8000;
}
#endif

/// x < 0 ?
/// \note non standard answer is returned
#define esign(x) (x[IEEE_NE-1] & 0x8000)

//@}


/// Comparison.
/// \retval  0  if a = b
/// \retval  1  if a > b
/// \retval -1  if a < b

idaman THREAD_SAFE int ida_export ecmp(const eNE a, const eNE b);


/// Check for NaN/Inf
enum fpvalue_kind_t
{
  FPV_BADARG,  ///< wrong value of max_exp
  FPV_NORM,    ///< regular value
  FPV_NAN,     ///< NaN
  FPV_PINF,    ///< positive infinity
  FPV_NINF,    ///< negative infinity
};

/// \name max_exp values
/// common values for max_exp (for IEEE floating point values)
//@{
const uint32
  MAXEXP_FLOAT  = 0x80,
  MAXEXP_DOUBLE = 0x400,
  MAXEXP_LNGDBL = 0x4000;
//@}


/// See ::fpvalue_kind_t

idaman THREAD_SAFE fpvalue_kind_t ida_export get_fpvalue_kind(const eNE a, uint16 max_exp);

#pragma pack(pop)
#endif
