/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2015 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 */

#ifndef _AUTO_HPP
#define _AUTO_HPP
#include <ida.hpp>
#pragma pack(push, 1)

/*! \file auto.hpp

  \brief Functions that work with the autoanalyzer queue.

  The autoanalyzer works when IDA is not busy processing
  the user keystrokes. It has several queues, each queue having
  its own priority. The analyzer stops when all queues are empty.

  A queue contains addresses or address ranges.
  The addresses are kept sorted by their values.
  The analyzer will process all addresses from the first queue, then
  switch to the second queue and so on.
  There are no limitations on the size of the queues.

  This file also contains functions that deal with the IDA status
  indicator and the autoanalysis indicator.
  You may use these functions to change the indicator value.
*/

typedef int atype_t; ///< identifies an autoanalysis queue - see \ref AU_

/// \defgroup AU_ Autoanalysis queues
/// Names and priorities of the analyzer queues
//@{
const atype_t
  AU_NONE = 00,         ///< placeholder, not used
  AU_UNK  = 10,         ///<  0: convert to unexplored
  AU_CODE = 20,         ///<  1: convert to instruction
  AU_WEAK = 25,         ///<  2: convert to instruction (ida decision)
  AU_PROC = 30,         ///<  3: convert to procedure start
  AU_TAIL = 35,         ///<  4: add a procedure tail
  AU_TRSP = 38,         ///<  5: trace stack pointer (not used yet)
  AU_USED = 40,         ///<  6: reanalyze
  AU_TYPE = 50,         ///<  7: apply type information
  AU_LIBF = 60,         ///<  8: apply signature to address
  AU_LBF2 = 70,         ///<  9: the same, second pass
  AU_LBF3 = 80,         ///< 10: the same, third pass
  AU_CHLB = 90,         ///< 11: load signature file (file name is kept separately)
  AU_FINAL=200;         ///< 12: final pass
//@}


typedef int idastate_t; ///< IDA status indicator - see \ref st_

/// \defgroup st_ Status indicator states
//@{
const idastate_t
                         //                      meaning
  st_Ready   = 0,        ///< READY:             IDA is doing nothing
  st_Think   = 1,        ///< THINKING:          Autoanalysis on, the user may press keys
  st_Waiting = 2,        ///< WAITING:           Waiting for the user input
  st_Work    = 3;        ///< BUSY:              IDA is busy
//@}


/// Current state of autoanalyzer.
/// If auto_state == ::AU_NONE, IDA is currently not running the analysis
/// (it could be temporarily interrupted to perform the user's requests, for example).

idaman atype_t ida_export_data auto_state;

/// See ::auto_display
struct auto_display_t
{
  atype_t type;
  ea_t ea;
  idastate_t state;
};

/// Structure to hold the autoanalysis indicator contents

idaman auto_display_t ida_export_data auto_display;

/// Change autoanalysis indicator value.
/// \param ea    linear address being analyzed
/// \param type  autoanalysis type (see \ref AU_)

inline void showAuto(ea_t ea, atype_t type=AU_NONE)
{
  auto_display.type = type;
  auto_display.ea = ea;
}


/// Show an address on the autoanalysis indicator.
/// The address is displayed in the form " @:12345678".
/// \param ea - linear address to display

inline void showAddr(ea_t ea) { showAuto(ea); }


/// Change IDA status indicator value
/// \param st - new indicator status
/// \return old indicator status

inline idastate_t setStat(idastate_t st)
{
  idastate_t old = auto_display.state;
  auto_display.state = st;
  return old;
}


/// Is it allowed to create stack variables automatically?.
/// This function should be used by IDP modules before creating stack vars.

inline bool may_create_stkvars(void)
{
  return should_create_stkvars() && auto_state == AU_USED;
}


/// Is it allowed to trace stack pointer automatically?.
/// This function should be used by IDP modules before tracing sp.

inline bool may_trace_sp(void)
{
  return should_trace_sp() && (auto_state == AU_USED || auto_state == AU_TRSP);
}


/// Put range of addresses into a queue.
/// 'start' may be higher than 'end', the kernel will swap them in this case.
/// 'end' doesn't belong to the range.

idaman void ida_export auto_mark_range(ea_t start,ea_t end,atype_t type);


/// Put single address into a queue. Queues keep addresses sorted.

inline void autoMark(ea_t ea, atype_t type)
{
  if ( ea != BADADDR )
    auto_mark_range(ea, ea+1, type);
}


/// Remove range of addresses from a queue.
/// 'start' may be higher than 'end', the kernel will swap them in this case.
/// 'end' doesn't belong to the range.

idaman void ida_export autoUnmark(ea_t start,ea_t end,atype_t type);

// Convenience functions

/// Plan to perform reanalysis
inline void noUsed(ea_t ea)
  { autoMark(ea,AU_USED); }
/// Plan to perform reanalysis
inline void noUsed(ea_t sEA,ea_t eEA)
  { auto_mark_range(sEA,eEA,AU_USED); }
/// Plan to make code
inline void auto_make_code(ea_t ea)
  { autoMark(ea,AU_CODE); }
/// Plan to make code&function
inline void auto_make_proc(ea_t ea)
  { auto_make_code(ea); autoMark(ea,AU_PROC); }

/// Plan to reanalyze callers of the specified address.
/// This function will add to ::AU_USED queue all instructions that
/// call (not jump to) the specified address.
/// \param ea     linear address of callee
/// \param noret  !=0: the callee doesn't return, mark to undefine subsequent
///               instructions in the caller. 0: do nothing.

idaman void ida_export reanalyze_callers(ea_t ea, bool noret);


/// Delete all analysis info that IDA generated for for the given range

idaman void ida_export revert_ida_decisions(ea_t ea1, ea_t ea2);


/// Plan to apply the callee's type to the calling point

idaman void ida_export auto_apply_type(ea_t caller, ea_t callee);


/// Analyze the specified area.
/// Try to create instructions where possible.
/// Make the final pass over the specified area.
/// This function doesn't return until the area is analyzed.
/// \retval 1  ok
/// \retval 0  Ctrl-Break was pressed

idaman int ida_export analyze_area(ea_t sEA,ea_t eEA);


/// Process everything in the queues and return true.
/// Return false if Ctrl-Break was pressed.

idaman bool ida_export autoWait(void);


/// Remove an address range (ea1..ea2) from queues ::AU_CODE, ::AU_PROC, ::AU_USED.
/// To remove an address range from other queues use autoUnmark() function.
/// 'ea1' may be higher than 'ea2', the kernel will swap them in this case.
/// 'ea2' doesn't belong to the range.

idaman void ida_export autoCancel(ea_t ea1,ea_t ea2);


/// Are all queues empty?
/// (i.e. has autoanalysis finished?).

idaman bool ida_export autoIsOk(void);


/// One step of autoanalyzer if 'autoEnabled' != 0.
/// \return true if some address was removed from queues and was processed.

idaman bool ida_export autoStep(void);


/// Peek into a queue 'type' for an address not lower than 'low_ea'.
/// Do not remove address from the queue.
/// \return the address or #BADADDR

idaman ea_t ida_export peek_auto_queue(ea_t low_ea, atype_t type);


/// Retrieve an address from queues regarding their priority.
/// Returns #BADADDR if no addresses not lower than 'lowEA' and less than
/// 'highEA' are found in the queues.
/// Otherwise *type will have queue type.

idaman ea_t ida_export auto_get(ea_t lowEA, ea_t highEA, atype_t *type);


/// Get two-character queue name to display on the indicator

idaman const char *ida_export autoGetName(atype_t type);



#pragma pack(pop)
#endif  //  _AUTO_HPP
