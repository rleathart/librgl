#include <stdio.h>

#include <stdbool.h>

#include <rgl/rgl.h>

int main(int argc, char* argv[])
{
  char Input[64];
  rgl.Util.GetUserInput("Enter: ", sizeof(Input), Input);
  printf("You entered: %s\n", Input);

  FileInfo_t* Test = rgl.FileInfo.New("main.d");

  int LineCount = 0;
  char** Lines = rgl.FileInfo.ReadLines(Test, &LineCount);

  for (int i = 0; i < LineCount; ++i)
    printf("%s\n", Lines[i]);
  return 0;
}
