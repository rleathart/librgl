#pragma once

#include <stdlib.h>

typedef struct Node rgl_LinkedList_Node_t;

#define Node_t rgl_LinkedList_Node_t

struct Node
{
  void* Data;
  Node_t* Next;
  Node_t* Prev;
};

typedef struct
{
  Node_t* Head;
  Node_t* Tail;

  size_t Length;
} rgl_LinkedList_t;

#define rgl_LinkedList_ForEach(Node, List)                                         \
  for (Node_t* Node = List->Head; Node; Node = Node->Next)

#undef Node_t
