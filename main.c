#include <stdio.h>

#include <rgl.h>

int GetUserInput(char *Prompt, int BytesToRead, char *OutString)
{
  printf("Main GetUserInput\n");
  return 0;
}

int main(int argc, char* argv[])
{
  rgl.Util.GetUserInput("", 1, "");
  rgl.Util.Test();
  rgl.Test();
  GetUserInput("", 1, "");
  return 0;
}
