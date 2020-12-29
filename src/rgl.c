#include <rgl.h>

#include "Util/Util.h"

#include <stdio.h>

static void Test()
{
  printf("Test\n");
}

_rglNamespace const rgl = {
  .Test = Test,
  .Util = {
    .GetUserInput = Util_GetUserInput,
    .Test = Util_Test
  }
};
