#pragma once

#include <rgl/FileInfo.h>
#include <rgl/LinkedList.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

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
    bool (*Exists)(FileInfo_t*);
    int (*ReadLine)(FileInfo_t*, char*, size_t);
    char** (*ReadLines)(FileInfo_t*, int*);
  } FileInfo;

#define Node_t rgl_LinkedList_Node_t
#define LinkedList_t rgl_LinkedList_t
  struct
  {
    struct
    {
      Node_t* (*New)(void*, size_t);
      void (*Free)(Node_t**);
    } Node;

    LinkedList_t* (*New)();
    void (*Free)(LinkedList_t**);

    void (*Append)(LinkedList_t* List, Node_t*);
    void (*Prepend)(LinkedList_t* List, Node_t*);
    void (*Remove)(LinkedList_t* List, Node_t*);
    Node_t* (*Find)(LinkedList_t*, void*, size_t);
  } LinkedList;
#ifdef LIBRGL_FULLY_QUALIFY_TYPE_NAMES
#undef Node_t
#undef LinkedList_t
#endif
} _rglNamespace;

extern const _rglNamespace rgl;
