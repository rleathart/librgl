#include <stdio.h>

#include <stdbool.h>

#include <rgl/rgl.h>

#define LinkedList rgl.LinkedList
#define Node LinkedList.Node

int main(int argc, char* argv[])
{
  char Input[64];
  rgl.Util.GetUserInput("Enter: ", sizeof(Input), Input);
  printf("You entered: %s\n", Input);

  FileInfo_t* Test = rgl.FileInfo.New("main.c");

  int LineCount = 0;
  char** Lines = rgl.FileInfo.ReadLines(Test, &LineCount);

  for (int i = 0; i < LineCount; ++i)
    printf("%s\n", Lines[i]);

  LinkedList_t* List = LinkedList.New();

  char Data = 'H';
  LinkedList.Append(List, Node.New(&Data, sizeof(Data)));
  Data = 'I';
  LinkedList.Append(List, Node.New(&Data, sizeof(Data)));

  LinkedList_ForEach(Item, List)
    printf("%c\n", *(char*)Item->Data);

  return 0;
}
