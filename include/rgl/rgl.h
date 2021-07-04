#pragma once

#include <rgl/FileInfo.h>
#include <rgl/LinkedList.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "rgl_exports.h"

#define Node_t rgl_LinkedList_Node_t
#define LinkedList_t rgl_LinkedList_t

typedef struct
{
  void (*Test)();

  struct rgl_Util
  {
    int (*GetUserInput)(char*, int, char*);
    void (*Test)();
  } Util;

  struct rgl_FileInfo
  {
    FileInfo_t* (*New)(char*);
    void (*Free)(FileInfo_t**);

    int (*Open)(FileInfo_t*, char*);
    int (*Close)(FileInfo_t*);
    bool (*Exists)(FileInfo_t*);
    bool (*ReadLine)(FileInfo_t*, char*, size_t);
    char** (*ReadLines)(FileInfo_t*, int*);
  } FileInfo;

  struct rgl_LinkedList
  {
    struct rgl_Node
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
    size_t (*FindIndex)(LinkedList_t*, void*, size_t);
    int (*InsertAfter)(LinkedList_t*, Node_t*, size_t);
  } LinkedList;
} _rglNamespace;

extern const RGL_EXPORT _rglNamespace rgl;

#ifdef LIBRGL_FULLY_QUALIFY_TYPE_NAMES
#undef Node_t
#undef LinkedList_t
#endif

#ifdef LIBRGL_NO_PREFIX
#define Util rgl.Util
#define FileInfo rgl.FileInfo
#define LinkedList rgl.LinkedList
#define LinkedList_ForEach rgl_LinkedList_ForEach
#endif
