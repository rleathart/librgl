#include <stdio.h>

#include <stdbool.h>

// using namespace rgl;
/* #define LIBRGL_NO_PREFIX */
#define LIBRGL_FULLY_QUALIFY_TYPE_NAMES
#include <rgl/rgl.h>

// 'Import' types
typedef rgl_LinkedList_t LinkedList_t;
typedef rgl_LinkedList_Node_t Node_t;

int main(int argc, char* argv[])
{

  // namespace Node = LinkedList::Node;
  struct rgl_Node Node = rgl.LinkedList.Node;
  struct rgl_LinkedList LinkedList = rgl.LinkedList;
  struct rgl_FileInfo FileInfo = rgl.FileInfo;
  struct rgl_Util Util = rgl.Util;

  char Input[64];
  Util.GetUserInput("Enter: ", sizeof(Input), Input);
  printf("You entered: %s\n", Input);

  FileInfo_t* Test = FileInfo.New("main.c");

  int LineCount = 0;
  char** Lines = FileInfo.ReadLines(Test, &LineCount);

  for (int i = 0; i < LineCount; ++i)
    printf("%s\n", Lines[i]);

  LinkedList_t* List = LinkedList.New();

  char Data = 'H';
  LinkedList.Append(List, Node.New(&Data, sizeof(Data)));
  Data = 'I';
  LinkedList.Append(List, Node.New(&Data, sizeof(Data)));

  for (Node_t* Item = List->Head; Item; Item = Item->Next)
    printf("%c\n", *(char*)Item->Data);

  return 0;
}
