#pragma once
#include "base_strings.h"

internal u32 cStringSize(const char *cString) {
  char c = *cString;
  u32 count = 0;
  while (c != '\0') {
    count++;
    cString++;
    c = *cString;
  }
  return count;
}

internal String fromCStringtoString(const char *cString) {
  return (String){
      .data = (char *)cString,
      .size = cStringSize(cString),
  };
}

internal void StringChopRight(String *s, u32 count) {
  for (i32 i = 0; i < count; ++i) {
    if (s->size == 0) {
      return;
    }
    s->size--;
  }
}

internal void StringChopLeft(String *s, u32 count) {
  for (i32 i = 0; i < count; ++i) {
    if (s->size == 0) {
      return;
    }
    s->size--;
    s->data++;
  }
}

internal void StringTrimRight(String *s) {
  char *last = s->data + s->size;
  while (s->size > 0 && *last == ' ') {
    s->size--;
    last = s->data + s->size;
  }
}

internal void StringTrimLeft(String *s) {
  while (s->size > 0 && *s->data == ' ') {
    s->size--;
    s->data++;
  }
}

internal String *stringConcat(Arena *arena, String s1, String s2) {
  u32 size = s1.size + s2.size;
  Assert(arenaSizeLeft(*arena) >= size);
  char data[size];
  for (i32 i = 0; i < s1.size; ++i) {
    data[i] = s1.data[i];
  }
  for (i32 i = 0; i < s2.size; ++i) {
    data[i + s1.size] = s2.data[i];
  }
  String *resultString = (String *)arenaAlloc(arena, sizeof(String));
  resultString->data = data;
  resultString->size = size;
  return resultString;
}

internal void stringChopByDelimiter(String *s, char delimiter) {
  u32 i = 0;
  while (i < s->size) {
    if (s->data[i] == delimiter) {
      StringChopLeft(s, i);
      return;
    }
    ++i;
  }
  StringChopLeft(s, s->size);
}

internal String *stringSplitByDelimiter(Arena *arena, String s, char delimiter,
                                        u32 *outCount) {
  u32 count = 1;
  for (u32 i = 0; i < s.size; ++i) {
    if (s.data[i] == delimiter)
      count++;
  }
  Assert(arenaSizeLeft(*arena) >= count * sizeof(String));
  String *parts = (String *)arenaAlloc(arena, count * sizeof(String));
  *outCount = 0;

  u32 start = 0;
  for (u32 i = 0; i < s.size; ++i) {
    if (i == s.size || s.data[i] == delimiter) {
      parts[*outCount].data = s.data + start;
      parts[*outCount].size = i - start;
      *outCount++;
      start = i + 1;
    }
  }
  return parts;
}
