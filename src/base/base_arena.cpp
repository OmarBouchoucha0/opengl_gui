#pragma once
#include "base_arena.h"

Arena arenaInit(u8 *buffer, u32 size) {
  Arena arena = {};
  arena.buffer = buffer;
  arena.size = size;
  arena.offset = 0;
  return arena;
}

internal void arenaReset(Arena *arena) { arena->offset = 0; }

// TODO: (omar) memory alignment
internal void *arenaAlloc(Arena *arena, u32 memSize) {
  u32 new_offset = arena->offset + memSize;
  if (new_offset > arena->size) {
    return NULL;
  }
  void *mem_alloc_start = arena->buffer + arena->offset;
  memset(mem_alloc_start, 0, memSize);
  arena->offset = new_offset;
  return mem_alloc_start;
}
internal u32 arenaSizeLeft(Arena arena) { return arena.size - arena.offset; }
