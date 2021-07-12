#include <rgl/util.h>

#include <stdio.h>
#include <string.h>

#ifdef _WIN32
// clang-format off
#include <Windows.h>
#include <DbgHelp.h>

#pragma comment(lib, "DbgHelp")
// clang-format on
#else
#include <time.h>
#include <execinfo.h>
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
  struct timespec ts = {
    .tv_sec = ms / 1000,
    .tv_nsec = (ms % 1000) * 1e6,
  };
  nanosleep(&ts, NULL);
#endif
}

char** stacktrace_tochararray(u64* _frames)
{
  void* stack[100];

#ifdef _WIN32
  HANDLE process = GetCurrentProcess();

  SymInitialize(process, NULL, TRUE);

  u64 frames = CaptureStackBackTrace(0, 100, stack, NULL);
  SYMBOL_INFO* symbol =
      (SYMBOL_INFO*)calloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char), 1);
  symbol->MaxNameLen = 255;
  symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

  IMAGEHLP_LINE* line = (IMAGEHLP_LINE*)malloc(sizeof(IMAGEHLP_LINE));
  line->SizeOfStruct = sizeof(IMAGEHLP_LINE);

  char* buffer = calloc(1, 4096);
  char** rv = malloc(frames * sizeof(char*));

  int written = 0;
  for (int i = 0; i < frames - 1; i++)
  {
    rv[i] = malloc(4096);
    SymFromAddr(process, (DWORD64)(stack[i + 1]), 0, symbol);
    DWORD dwDisplacement;
    SymGetLineFromAddr(process, (DWORD64)stack[i + 1], &dwDisplacement, line);

    sprintf(rv[i], "frame %d: 0x%016llx %s:%ld:%s()",
                       i, symbol->Address, line->FileName, line->LineNumber,
                       symbol->Name);
    *_frames += 1;
    if (strcmp(symbol->Name, "main") == 0)
      break;
  }

  free(symbol);

  return rv;
#else
  *_frames = backtrace(stack, 100);
  return backtrace_symbols(stack, *_frames);
#endif
}
