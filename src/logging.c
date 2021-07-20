#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <rgl/array.h>
#include <rgl/logging.h>
#include <rgl/util.h>

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

/*
 * Colour related code contributed by Elwyn John
 * https://github.com/ElwynJohn
 */

/* ANSI escape codes for colouring the terminal */
#define LOC_COL "\033[38;2;7;102;114m"
#define ERR_COL "\033[38;2;220;50;47m"
#define WAR_COL "\033[38;2;181;157;0m"
#define INF_COL "\033[38;2;53;193;90m"
#define DBG_COL "\033[38;2;88;109;210m"
#define ESC "\033[0m"

#ifdef RGL_DEBUG_LEVEL
static DebugLevel debug_level_default = RGL_DEBUG_LEVEL;
#else
#ifdef DEBUG
static DebugLevel debug_level_default = DebugLevelDebug;
#else
static DebugLevel debug_level_default = DebugLevelInfo;
#endif
#endif

static DebugLevel g_debug_level;
static _Thread_local DebugLevel t_debug_level;

static Array logger_streams;

__attribute__((constructor))
static void setup()
{
  array_new(&logger_streams, 8, sizeof(LoggerStream));
  g_debug_level = debug_level_default;
  t_debug_level = debug_level_default;

#ifdef _WIN32
  // Enable termnial sequences
  HANDLE out[] = {
    GetStdHandle(STD_OUTPUT_HANDLE),
    GetStdHandle(STD_ERROR_HANDLE),
  };
  for (int i = 0; i < 2; i++)
  {
    DWORD mode = 0;
    GetConsoleMode(out[i], &mode);
    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(out[i], mode);
  }
#endif
}

void rgl_logger_add_stream(LoggerStream stream)
{
  array_push(&logger_streams, &stream);
}

void t_debug_level_set(DebugLevel level)
{
  t_debug_level = level;
}

DebugLevel t_debug_level_get()
{
  return t_debug_level;
}

static char* debuglevel_tostring(DebugLevel level)
{
  switch (level)
  {
  case DebugLevelError:
    return ERR_COL "ERROR" ESC;
  case DebugLevelWarning:
    return WAR_COL "WARN " ESC;
  case DebugLevelInfo:
    return INF_COL "INFO " ESC;
  case DebugLevelDebug:
    return DBG_COL "DEBUG" ESC;
  default:
    return "";
  }
}
static char* col(DebugLevel level)
{
  switch (level)
  {
  case DebugLevelError:
    return ERR_COL;
  case DebugLevelWarning:
    return WAR_COL;
  case DebugLevelInfo:
    return INF_COL;
  case DebugLevelDebug:
    return DBG_COL;
  default:
    return "";
  }
}
static char* conditional_escape(DebugLevel level)
{
  switch (level)
  {
  case DebugLevelError:
  case DebugLevelWarning:
    return "";
  default:
    return ESC;
  }
}

void _rgl_logger(DebugLevel level, char* file, int line, const char* func,
                 const char* fmt, ...)
{
  DebugLevel target_level = t_debug_level;

  time_t log_time = time(NULL);
  struct tm* time_info = localtime(&log_time);
  char* time_str = calloc(1, 4096);
  // ISO Datetime
  strftime(time_str, 4096, "%Y-%m-%dT%H:%M:%S%z", time_info);

  for (int i = 0; i < logger_streams.used; i++)
  {
    va_list args;
    va_start(args, fmt);

    LoggerStream log_stream = *(LoggerStream*)array_get(&logger_streams, i);
    FILE* stream = log_stream.stream;
    char* filename = log_stream.filename;

    if (filename)
      stream = fopen(filename, "a");

    if (stream && isatty(fileno(stream)) && target_level < level)
      continue;

    if (file) // If file is NULL, don't print the header
    {
      char buffer[128];
      sprintf(buffer, LOC_COL "%s:%03d:%s()%s: %s", file, line, func,
          col(level), conditional_escape(level));
      fprintf(stream, "%s: [%s] %s", debuglevel_tostring(level), time_str,
          buffer);
    }

    vfprintf(stream, fmt, args);
    fprintf(stream, ESC);

    if (file && (level == DebugLevelError || level == DebugLevelWarning))
    {
      u64 frames = 0;
      char** trace = stacktrace_tochararray(&frames);
      for (int j = 1; j < frames; j++)
        fprintf(stream, "\t%s\n", trace[j]);
    }

    if (filename)
      fclose(stream);
    va_end(args);
  }

  free(time_str);
}
