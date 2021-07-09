#pragma once

#include <stdio.h>

#include <rgl/defs.h>

typedef struct
{
  char* filename;
  FILE* stream;
} LoggerStream;

typedef enum
{
  // Ordered by decreasing severity. If we specify DebugLevelWarning, then we
  // should print errors and warnings but not info.
  DebugLevelNone,

  DebugLevelError,
  DebugLevelWarning,
  DebugLevelInfo,
  DebugLevelDebug, // Used for throwaway prints

  DebugLevelAll, // Keep this at the end of the enum
} DebugLevel;

// clang-format off
#define ILOG(fmt, ...) _rgl_logger(DebugLevelInfo, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)
#define WLOG(fmt, ...) _rgl_logger(DebugLevelWarning, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)
#define ELOG(fmt, ...) _rgl_logger(DebugLevelError, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)
#define DLOG(fmt, ...) _rgl_logger(DebugLevelDebug, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)
#define IPRINT(fmt, ...) _rgl_logger(DebugLevelInfo, NULL, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)
#define WPRINT(fmt, ...) _rgl_logger(DebugLevelWarning, NULL, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)
#define EPRINT(fmt, ...) _rgl_logger(DebugLevelError, NULL, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)
#define DPRINT(fmt, ...) _rgl_logger(DebugLevelDebug, NULL, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)

__attribute__((__format__ (__printf__, 5, 6)))
void _rgl_logger(DebugLevel level, char* file, int line, const char* func, const char* fmt, ...);
// clang-format on

void rgl_logger_add_stream(LoggerStream stream);

void t_debug_level_set(DebugLevel level);
DebugLevel t_debug_level_get(void);
void tdebug_level_set_prev();
