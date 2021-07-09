#include <stdio.h>

#include <rgl/logging.h>
#include <rgl/util.h>
#include <signal.h>

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
  int* ptr = 0;
  *ptr = 1;
}

int main(void)
{
  signal(SIGSEGV, handle_signal);
  ILOG("Some info\n");
  WLOG("Some warn\n");
  ELOG("Some err\n");
  DLOG("Some debug\n");
  this_func_will_error();
  int* ptr = 0;
  *ptr = 1;
}
