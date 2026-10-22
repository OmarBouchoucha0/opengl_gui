#ifndef BASE_LOG_H
#define BASE_LOG_H

#include <stdio.h>
#include "base_types.cpp"
#include "base_strings.cpp"
#include "base_arena.cpp"

enum LogLevel {
  INFO,
  DEBUG,
  WARNING,
  ERROR,
};

internal void traceLog(LogLevel logLevel, char *msg);
#endif // BASE_LOG_H
