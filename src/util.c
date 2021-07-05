#include <rgl/util.h>

#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <time.h>
#endif

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

void sleep_ms(u64 ms)
{
#ifdef _WIN32
  Sleep(ms);
#else
#endif
}
