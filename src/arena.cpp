#ifndef ARENA_H
#define ARENA_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
  uint8_t *buffer;
  size_t offset;
  size_t size;
} Arena;

Arena arenaInit(uint8_t *buffer, size_t size) {
  Arena arena = {};
  arena.buffer = buffer;
  arena.size = size;
  arena.offset = 0;
  return arena;
}

void arenaReset(Arena *arena) { arena->offset = 0; }

void *arenaAlloc(Arena *arena, size_t memSize) {
  size_t new_offset = arena->offset + memSize;
  if (new_offset > arena->size) {
    return NULL;
  }
  void *mem_alloc_start = arena->buffer + arena->offset;
  arena->offset = new_offset;
  return mem_alloc_start;
}

#endif // ARENA_H
