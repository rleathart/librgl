#include <librgl.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
  char Input[64];
  rgl.Util.GetUserInput("Enter: ", 64, Input);

  printf("You Entered: %s\n", Input);
  return 0;
}
