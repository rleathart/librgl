#include <rgl/rgl.h>

#include "Util/Util.h"
#include "FileInfo/FileInfo.h"

#include <stdio.h>

static void Test()
{
  printf("Test\n");
}

const _rglNamespace rgl = {
  .Test = Test,
  .Util = {
    .GetUserInput = Util_GetUserInput,
    .Test = Util_Test
  },
  .FileInfo = {
    .New = FileInfo_New,
    .Free = FileInfo_Free,
    .Open = FileInfo_Open,
    .Close = FileInfo_Close,
    .ReadLine = FileInfo_ReadLine,
    .ReadLines = FileInfo_ReadLines
  }
};
