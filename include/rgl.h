#pragma once

typedef struct
{
  void (*Test)();

  struct
  {
    int (*GetUserInput)(char*, int, char*);
    void (*Test)();
  } Util;
} _rglNamespace;

extern _rglNamespace const rgl;
