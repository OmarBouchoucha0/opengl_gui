#ifndef BASE_ARENA_H
#define BASE_ARENA_H
#include <cstring>
#include "base_types.cpp"

typedef struct {
  u8 *buffer;
  u32 offset;
  u32 size;
} Arena;

Arena arenaInit(u8 *buffer, u32 size);
internal void arenaReset(Arena *arena);
internal void *arenaAlloc(Arena *arena, u32 memSize);
internal u32 arenaSizeLeft(Arena arena);

#endif // BASE_ARENA_H
