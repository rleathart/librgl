#include <stdio.h>

#include <rgl/rgl.h>

int main(int argc, char* argv[])
{
  char Input[64];
  rgl.Util.GetUserInput("Enter: ", sizeof(Input), Input);
  printf("You entered: %s\n", Input);

  FileInfo_t* Test = rgl.FileInfo.New("main.c");

  char Buffer[1024];
  while (rgl.FileInfo.ReadLine(Test, Buffer, sizeof(Buffer)))
    printf("%s\n", Buffer);
  return 0;
}
