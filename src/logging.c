#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <rgl/logging.h>
#include <rgl/util.h>

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

__attribute__((constructor))
static void setup()
{
  g_debug_level = debug_level_default;
  t_debug_level = debug_level_default;
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
  va_list args;
  va_start(args, fmt);

  FILE* streams[] = {
      stderr,
  };

  time_t log_time = time(NULL);
  struct tm* time_info = localtime(&log_time);
  char* time_str = calloc(1, 4096);
  // ISO Datetime
  strftime(time_str, 4096, "%Y-%m-%dT%H:%M:%S%z", time_info);

  for (int i = 0; i < sizeof(streams) / sizeof(streams[0]); i++)
    if (streams[i] == stderr && target_level < level)
    {
      continue;
    }
    else
    {
      if (file) // If file is NULL, don't print the header
      {
        char buffer[128];
        sprintf(buffer, LOC_COL "%s:%03d:%s()%s: %s", file, line, func,
                col(level), conditional_escape(level));
        fprintf(streams[i], "%s: [%s] %s", debuglevel_tostring(level), time_str,
                buffer);

      }
      vfprintf(streams[i], fmt, args);
      fprintf(streams[i], ESC);
      if (!(streams[i] == stderr || streams[i] == stdout))
        fclose(streams[i]);

      if (file && (level == DebugLevelError || level == DebugLevelWarning))
      {
        u64 frames = 0;
        char** trace = stacktrace_tochararray(&frames);
        for (int j = 1; j < frames; j++)
          fprintf(streams[i], "\t%s\n", trace[j]);
      }
    }

  free(time_str);
  va_end(args);
}
