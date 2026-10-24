#pragma once
#include "base_arena.h"

#ifndef DEFAULT_ALIGNMENT
#define DEFAULT_ALIGNMENT (2 * (sizeof(void *)))
#endif

Arena arenaInit(u8 *buffer, u32 size) {
  Arena arena = {};
  arena.buffer = buffer;
  arena.size = size;
  arena.offset = 0;
  return arena;
}

internal void arenaReset(Arena *arena) { arena->offset = 0; }

b32 isPowerOfTwo(u32 x) { return (x & (x - 1)) == 0; }

void *alignForward(void *mem, u64 alignment) {
  u64 p = (u64)mem;
  u64 modulo = p % alignment;
  if (modulo != 0) {
    p += alignment - modulo;
  }
  return (void *)p;
}

internal void *arenaAllocAlign(Arena *arena, u64 memSize, u64 alignment) {
  void *mem_alloc_start = arena->buffer + arena->offset;
  mem_alloc_start = alignForward(mem_alloc_start, alignment);
  u32 new_offset = (u64)(mem_alloc_start - (void *)arena->buffer);
  if (new_offset > arena->size) {
    return NULL;
  }
  memset(mem_alloc_start, 0, memSize);
  arena->offset = new_offset;
  return mem_alloc_start;
}

internal void *arenaAlloc(Arena *arena, u64 memSize) {
  return arenaAllocAlign(arena, memSize, DEFAULT_ALIGNMENT);
}

internal u32 arenaSizeLeft(Arena arena) { return arena.size - arena.offset; }
