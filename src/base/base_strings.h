#ifndef BASE_STRING_H
#define BASE_STRING_H

#define SS_FMT "%.*s"
#define SS_ARGS(s) (s).size, (s).data

#include "base_types.cpp"
#include "base_arena.cpp"

struct String {
  char *data;
  u32 size;
};

internal u32 cStringSize(char *cString);
internal String fromCStringtoString(char *cString);
internal void StringChopRight(String *s, u32 count);
internal void StringChopLeft(String *s, u32 count);
internal void StringTrimRight(String *s);
internal void StringTrimLeft(String *s);
internal String *stringConcat(Arena *arena, String s1, String s2);
internal void stringChopByDelimiter(String *s, char delimiter);
internal String *stringSplitByDelimiter(Arena *arena, String s, char delimiter,
                                        u32 *outCount);

#endif // !BASE_STRING_H
