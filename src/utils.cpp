#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>
#include <stdint.h>
#include <cstdio>

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
#define DegreeToRadian(x) ((f32)(x) * (PI) / 180.0f)
#define RadianToDegree(x) ((f32)(x) * (PI) / 180.0f)

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float f32;
typedef double f64;

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

internal void arenaReset(Arena *arena) { arena->offset = 0; }

internal void *arenaAlloc(Arena *arena, u32 memSize) {
  u32 new_offset = arena->offset + memSize;
  if (new_offset > arena->size) {
    return NULL;
  }
  void *mem_alloc_start = arena->buffer + arena->offset;
  arena->offset = new_offset;
  return mem_alloc_start;
}
internal u32 arenaSizeLeft(Arena arena) { return arena.size - arena.offset; }

internal V2f rotatePointRadian(V2f offset, f32 theta) {
  f32 c = cosf(theta);
  f32 s = sinf(theta);
  return {
      offset.x * c - offset.y * s,
      offset.x * s + offset.y * c,
  };
}

struct Color {
  u8 r;
  u8 g;
  u8 b;
  u8 a;
};

struct ColorNormolized {
  f32 r;
  f32 g;
  f32 b;
  f32 a;
};

internal V2f rotatePointDegree(V2f offset, f32 rotation) {
  f32 theta = DegreeToRadian(rotation);
  f32 c = cosf(theta);
  f32 s = sinf(theta);
  return {
      offset.x * c - offset.y * s,
      offset.x * s + offset.y * c,
  };
}

internal ColorNormolized normolizeColor(Color color) {
  f32 r = NormalizeColorChannel(color.r);
  f32 g = NormalizeColorChannel(color.g);
  f32 b = NormalizeColorChannel(color.b);
  f32 a = NormalizeColorChannel(color.a);
  return (ColorNormolized){r, g, b, a};
}

struct StringSlice {
  char *data;
  u32 size;
};

#define SS_FMT "%.*s"
#define SS_ARGS(s) (s).size, (s).data

// NOTE: (omar) if supplied a non null terminated string we will have problems
internal u32 cStringSize(char *cString) {
  char c = *cString;
  u32 count = 0;
  while (c != '\0') {
    count++;
    cString++;
    c = *cString;
  }
  return count;
}

internal StringSlice fromCStringtoStringSlice(char *cString) {
  return (StringSlice){
      .data = cString,
      .size = cStringSize(cString),
  };
}

internal void StringSliceChopRight(StringSlice *s) {
  if (s->size == 0) {
    return;
  }
  s->size--;
}

internal void StringSliceChopLeft(StringSlice *s) {
  if (s->size == 0) {
    return;
  }
  s->size--;
  s->data++;
}

internal void StringSliceTrimRight(StringSlice *s) {
  char *last = s->data + s->size;
  while (s->size > 0 && *last == ' ') {
    s->size--;
    last = s->data + s->size;
  }
}

internal void StringSliceTrimLeft(StringSlice *s) {
  while (s->size > 0 && *s->data == ' ') {
    s->size--;
    s->data++;
  }
}

internal void stringSliceConcat(Arena *arena, StringSlice s1, StringSlice s2) {
  u32 size = s1.size + s2.size;
  Assert(arenaSizeLeft(*arena) >= size);
  char data[size];
  for (i32 i = 0; i < s1.size; ++i) {
    data[i] = s1.data[i];
  }
  for (i32 i = 0; i < s2.size; ++i) {
    data[i + s1.size] = s2.data[i];
  }
  StringSlice *resultString =
      (StringSlice *)arenaAlloc(arena, sizeof(resultString));
  resultString->data = data;
  resultString->size = size;
}

enum LogLevel {
  INFO,
  DEBUG,
  WARNING,
  ERROR,
};

internal void traceLog(LogLevel logLevel, char *msg) {
  StringSlice msgSlice = {};
  msgSlice.data = msg;
  msgSlice.size = cStringSize(msg);

  switch (logLevel) {
  case INFO: {
    fprintf(stdout, "[INFO]: " SS_FMT "\n", SS_ARGS(msgSlice));
  } break;
  case DEBUG: {
    fprintf(stdout, "[DEBUG]: " SS_FMT "\n", SS_ARGS(msgSlice));
  } break;
  case WARNING: {
    fprintf(stdout, "[WARNING]: " SS_FMT "\n", SS_ARGS(msgSlice));
  } break;
  case ERROR: {
    fprintf(stderr, "[ERROR]: " SS_FMT "\n", SS_ARGS(msgSlice));
  } break;
  }
}

#endif // UTILS_H
