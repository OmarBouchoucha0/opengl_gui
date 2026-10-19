#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>
#include <stdint.h>

#define NormalizeColorChannel(color) ((float)(color) / 255.0f)
#define Assert(Expression)                                                     \
  if (!(Expression)) {                                                         \
    *(int *)0 = 0;                                                             \
  }

#define Kilobytes(x) ((u64)(x) * 1024)
#define Megabytes(x) ((u64)(x) * 1024 * 1024)
#define Gigabytes(x) ((u64)(x) * 1024 * 1024 * 1024)
#define Terabytes(x) ((u64)(x) * 1024 * 1024 * 1024 * 1024)

#define global_variable static
#define local_persist static
#define internal static

#define PI 3.14159265359f

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float f32;
typedef double f64;

struct Color {
  u8 r;
  u8 g;
  u8 b;
  u8 a;
};

struct V2 {
  i32 x;
  i32 y;
};

struct V2f {
  f32 x;
  f32 y;
};

struct Rectangle {
  f32 x;
  f32 y;
  f32 width;
  f32 height;
};

typedef struct {
  u8 *buffer;
  u32 offset;
  u32 size;
} Arena;

Arena arenaInit(u8 *buffer, u32 size) {
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

#endif // UTILS_H
