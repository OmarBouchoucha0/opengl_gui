#pragma once
#include "base_log.h"

internal void traceLog(LogLevel logLevel, const char *msg) {
  String msgSlice = {};
  msgSlice.data = (char *)msg;
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
