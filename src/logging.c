#include <stdarg.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <rgl/array.h>
#include <rgl/logging.h>
#include <rgl/util.h>

#ifdef _WIN32
#include <Windows.h>
#include <io.h>
#else
#include <unistd.h>
#endif

typedef struct
{
  char* filename;
  FILE* stream;
} LoggerStream;

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
const DebugLevel debug_level_default = RGL_DEBUG_LEVEL;
#elif defined(DEBUG)
const DebugLevel debug_level_default = DebugLevelDebug;
#else
const DebugLevel debug_level_default = DebugLevelInfo;
#endif

static DebugLevel g_debug_level = debug_level_default;
static _Thread_local DebugLevel t_debug_level = debug_level_default;

static Array g_logger_streams;
static _Thread_local Array t_logger_streams;
// There doesn't seem to be a way to call a constructor on thread creation, so
// this bool is a workaround.
static volatile atomic_bool is_logging = false;

void rgl_logger_thread_setup()
{
  // t_debug_level inherits the current global level
  array_copy(&t_logger_streams, g_logger_streams);
  t_debug_level = g_debug_level;
}

static void __attribute__((constructor)) rgl_logger_setup()
{
  array_new(&g_logger_streams, 8, sizeof(LoggerStream));
  rgl_logger_thread_setup();

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

void rgl_logger_thread_remove_all()
{
  for (u64 i = 0; i < t_logger_streams.capacity; i++)
    if (array_index_is_allocated(&t_logger_streams, i))
      array_remove(&t_logger_streams, i);
}

static void rgl_logger_add(Array* logger_streams, LoggerStream log_stream)
{
  array_push(logger_streams, &log_stream);
}

static void rgl_logger_remove(Array* logger_streams, LoggerStream log_stream)
{
  for (u64 i = 0; i < logger_streams->capacity; i++)
  {
    if (!array_index_is_allocated(logger_streams, i))
      continue;

    LoggerStream ls = *(LoggerStream*)array_get(logger_streams, i);

    if (log_stream.stream && ls.stream == log_stream.stream)
      array_remove(logger_streams, i);
    else if (log_stream.filename && ls.filename &&
             strcmp(ls.filename, log_stream.filename) == 0)
      array_remove(logger_streams, i);
  }
}

void rgl_logger_add_stream(FILE* stream)
{
  LoggerStream log_stream = {
      .stream = stream,
  };
  rgl_logger_add(&g_logger_streams, log_stream);
}

void rgl_logger_thread_add_stream(FILE* stream)
{
  LoggerStream log_stream = {
      .stream = stream,
  };
  rgl_logger_add(&t_logger_streams, log_stream);
}

void rgl_logger_add_file(char* filename)
{
  LoggerStream log_stream = {
      .filename = strdup(filename),
  };
  rgl_logger_add(&g_logger_streams, log_stream);
}

void rgl_logger_thread_add_file(char* filename)
{
  LoggerStream log_stream = {
      .filename = strdup(filename),
  };
  rgl_logger_add(&t_logger_streams, log_stream);
}

void rgl_logger_remove_stream(FILE* stream)
{
  LoggerStream log_stream = {
      .stream = stream,
  };
  rgl_logger_remove(&g_logger_streams, log_stream);
}

void rgl_logger_thread_remove_stream(FILE* stream)
{
  LoggerStream log_stream = {
      .stream = stream,
  };
  rgl_logger_remove(&t_logger_streams, log_stream);
}

void rgl_logger_remove_file(char* filename)
{
  LoggerStream log_stream = {
      .filename = strdup(filename),
  };
  rgl_logger_remove(&g_logger_streams, log_stream);
}

void rgl_logger_thread_remove_file(char* filename)
{
  LoggerStream log_stream = {
      .filename = strdup(filename),
  };
  rgl_logger_remove(&t_logger_streams, log_stream);
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
  while (is_logging)
    sleep_ms(10);
  is_logging = true;
  DebugLevel target_level = t_debug_level;

  time_t log_time = time(NULL);
  struct tm* time_info = localtime(&log_time);
  char* time_str = calloc(1, 4096);
  // ISO Datetime
  strftime(time_str, 4096, "%Y-%m-%dT%H:%M:%S%z", time_info);

  if (target_level < level)
    goto end;

  for (int i = 0; i < t_logger_streams.capacity; i++)
  {
    if (!array_index_is_allocated(&t_logger_streams, i))
      continue;
    va_list args;
    va_start(args, fmt);

    LoggerStream log_stream = *(LoggerStream*)array_get(&t_logger_streams, i);
    FILE* stream = log_stream.stream;
    char* filename = log_stream.filename;

    if (filename)
      stream = fopen(filename, "a");

    if (!stream)
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

end:
  free(time_str);
  is_logging = false;
}
