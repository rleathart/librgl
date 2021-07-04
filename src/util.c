#include <stdio.h>
#include <string.h>

int get_user_input(char* Prompt, int BytesToRead, char* OutString)
{
  printf("%s", Prompt);
  fgets(OutString, BytesToRead, stdin);

  int StringLength = strlen(OutString);

  if (OutString[StringLength - 1] == '\n')
    OutString[StringLength - 1] = '\0';
  else
  {
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
      ;
  }

  return 0;
}
