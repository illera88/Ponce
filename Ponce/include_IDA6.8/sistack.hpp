/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2015 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 */

#ifndef __SISTACK_HPP
#define __SISTACK_HPP
#pragma pack(push, 1)           // IDA uses 1 byte alignments!

const int ST_SIZE = (MAXSPECSIZE/sizeof(uval_t));

class sistack_t;
idaman void ida_export sistack_t_flush(const sistack_t *ss);
idaman size_t ida_export sistack_t_size(const sistack_t *ss);

class sistack_t
{
  friend void ida_export sistack_t_flush(const sistack_t *ss);
  friend size_t ida_export sistack_t_size(const sistack_t *ss);
  uval_t code;                     // net node number
  mutable size_t chunk;            // number of current chunk
  mutable size_t ptr;              // pointer into chunk
  mutable uval_t cache[ ST_SIZE ]; // current stack chunk

  void fill_cache() const;

public:
  sistack_t(void) : code(BADADDR), chunk(0), ptr(0) {}
  ~sistack_t(void)      { if ( code != BADADDR ) flush(); }
  int  attach(const char *name);
  void create(const char *name);
  void kill(void);

  void flush(void) const { sistack_t_flush(this); }

  size_t size(void) const { return sistack_t_size(this); }
  void doempty(void)    { chunk = ptr = 0; }

  void push(uval_t x);
  uval_t pop(void);
  uval_t get(size_t depth) const; // return the value of the item at a given depth.
                                  // return BADADDR if the item doesn't exist (depth > size()).
  uval_t top(void) const;
  uval_t dup(void)      { uval_t x = top(); push(x); return x; }

  static bool get_stack_info(size_t *out_chunk, size_t *out_ptr, netnode node);

protected:
  uval_t netcode(void) const { return code; } // graph info uses loc_gtag in this netnode
};

#pragma pack(pop)
#endif // __SISTACK_HPP
