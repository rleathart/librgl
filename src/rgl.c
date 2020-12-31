#include <rgl/rgl.h>

#include "Util/Util.h"
#include "FileInfo/FileInfo.h"
#include "LinkedList/LinkedList.h"

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
    .Exists = FileInfo_Exists,
    .ReadLine = FileInfo_ReadLine,
    .ReadLines = FileInfo_ReadLines
  },
  .LinkedList = {
    .Node = {
      .New = LinkedList_Node_New,
      .Free = LinkedList_Node_Free
    },
    .New = LinkedList_New,
    .Free = LinkedList_Free,
    .Append = LinkedList_Append,
    .Prepend = LinkedList_Prepend,
    .Find = LinkedList_Find,
    .Remove = LinkedList_Remove
  }
};
