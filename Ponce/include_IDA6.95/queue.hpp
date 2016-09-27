/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2015 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 */

#ifndef _QUEUE_HPP
#define _QUEUE_HPP
#pragma pack(push, 1)   // IDA uses 1 byte alignments!

/*! \file queue.hpp

  \brief Functions that deal with the list of problems.

  There are several problem lists. An address may be inserted to any list.
  The kernel simply maintains these lists, no additional processing
  is done.

  The problem lists are accessible for the user
  from the View->Subviews->Problems menu item.
  Addresses in the lists are kept sorted.

  See \ref Q_ for the different types of problem lists.
*/

typedef uchar qtype_t; ///< see \ref Q_

/// \defgroup Q_ Problem lists
//@{
const qtype_t
  Q_noBase  =  1,            ///< Can't find offset base
  Q_noName  =  2,            ///< Can't find name
  Q_noFop   =  3,            ///< Can't find forced op
  Q_noComm  =  4,            ///< Can't find comment !!!! not used anymore !!!
  Q_noRef   =  5,            ///< Can't find references
  Q_jumps   =  6,            ///< Jump by table !!!! ignored
  Q_disasm  =  7,            ///< Can't disasm
  Q_head    =  8,            ///< Already head
  Q_noValid =  9,            ///< Exec flows beyond limits
  Q_lines   = 10,            ///< Too many lines
  Q_badstack= 11,            ///< Failed to trace the value of the stack pointer
  Q_att     = 12,            ///< Attention! Probably erroneous situation.
  Q_final   = 13,            ///< Decision to convert to instruction/data is made by IDA
  Q_rolled  = 14,            ///< The decision made by IDA was wrong and rolled back
  Q_collsn  = 15,            ///< FLAIR collision: the function with the given name already exists
  Q_decimp  = 16,            ///< FLAIR match indecision: the patterns matched, but not the function(s) being referenced
  Q_Qnum    = Q_decimp + 1;  ///< Number of qtypes
//@}


/// Get an address from any problem list.
/// The address is not removed from the list. The kernel returns an address
/// from a list with the smallest type value.
/// \param type  problem queue type the address is got from
/// \return linear address or #BADADDR

idaman ea_t ida_export QueueGet(qtype_t *type);


/// Get the human-friendly description of the problem,
/// if one was provided to QueueSet.
/// \param t        problem queue type.
/// \param ea       linear address.
/// \param buf      a buffer to store the message into.
/// \param bufsize  the size of the buffer.
/// \return the message length or -1 if none

idaman ssize_t ida_export QueueGetMessage(qtype_t t, ea_t ea, char *buf, size_t bufsize);


/// Insert an address to a list of problems.
/// Display a message saying about the problem (except of ::Q_att,::Q_final)
/// ::Q_jumps is temporarily ignored.
/// \param ea    linear address
/// \param type  problem queue type
/// \param msg   a user-friendly message to be displayed instead of
///              the default more generic one associated with
///              the type of problem. Defaults to NULL.

idaman void ida_export QueueSet(qtype_t type, ea_t ea, const char *msg = NULL);


/// Get an address from the specified problem list.
/// The address is not removed from the list.
/// \param type   problem queue type
/// \param lowea  the returned address will be higher or equal
///               than the specified address
/// \return linear address or #BADADDR

idaman ea_t ida_export QueueGetType(qtype_t type,ea_t lowea);


/// Remove an address from a problem list
/// \param ea    linear address
/// \param type  problem queue type

idaman void ida_export QueueDel(qtype_t type,ea_t ea);


/// Remove an address from all problem lists
/// \param ea  linear address

void QueueDel(ea_t ea);


/// \name Get queue problem description string
//@{
idaman const char *ida_export get_long_queue_name(qtype_t type);
idaman const char *ida_export get_short_queue_name(qtype_t type);
//@}


/// Check if the specified address is present in the queue

idaman bool ida_export QueueIsPresent(qtype_t t, ea_t ea);


// The kernel only functions:
/// \cond
inline void mark_ida_decision(ea_t ea) { QueueSet(Q_final, ea); }
inline void unmark_ida_decision(ea_t ea) { QueueDel(Q_final, ea); }
inline bool was_ida_decision(ea_t ea) { return QueueIsPresent(Q_final, ea); }
/// \endcond

//-----------------------------------------------------------------------

#ifndef NO_OBSOLETE_FUNCS
idaman DEPRECATED void ida_export QueueMark(qtype_t type,ea_t ea);
#endif

#pragma pack(pop)
#endif  //  _QUEUE_HPP
