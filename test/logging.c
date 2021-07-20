#include <signal.h>
#include <stdio.h>
#include <string.h>

#ifndef _WIN32
#include <unistd.h> // unlink
#endif

#include <rgl/logging.h>
#include <rgl/util.h>

static void handle_signal(int signal)
{
  switch (signal)
  {
  case SIGSEGV:
    ELOG("Segfault!\n");
  }
}

void this_func_will_error()
{
  ELOG("A very bad error!\n");
}

int main(void)
{
  signal(SIGSEGV, handle_signal);
  rgl_logger_add_file("rgl.log");
  rgl_logger_add_stream(stderr);

  ILOG("Some info\n");
  WLOG("Some warn\n");
  ELOG("Some err\n");
  DLOG("Some debug\n");
  this_func_will_error();

  // Remove file, we just want to check that we can write to it ok
  unlink("rgl.log");

  return 0;
}
