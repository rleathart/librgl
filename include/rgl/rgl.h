#pragma once

#include <rgl/FileInfo.h>

#include <stdlib.h>
#include <stdio.h>

typedef struct
{
  void (*Test)();

  struct
  {
    int (*GetUserInput)(char*, int, char*);
    void (*Test)();
  } Util;

  struct
  {
    FileInfo_t* (*New)(char*);
    void (*Free)(FileInfo_t**);

    int (*Open)(FileInfo_t*, char*);
    int (*Close)(FileInfo_t*);
    int (*ReadLine)(FileInfo_t*, char*, size_t);
    char** (*ReadLines)(FileInfo_t*, int*);
  } FileInfo;
} _rglNamespace;

extern const _rglNamespace rgl;
