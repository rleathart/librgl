#pragma once

#define LIBRGL_PREFIX rgl

struct _librgl_namespace
{
  struct
  {
    int (*GetUserInput)(char*, int, char*);
  } Util;
};

extern const struct _librgl_namespace LIBRGL_PREFIX;
