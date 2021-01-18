#include "LinkedList.h"

#include <memory.h>
#include <stdio.h>
#include <string.h>

#define Node_t rgl_LinkedList_Node_t
#define LinkedList_t rgl_LinkedList_t

Node_t* LinkedList_Node_New(void* Data, size_t DataSize)
{
  Node_t* Node = malloc(sizeof(Node_t));
  Node->Data = malloc(DataSize);
  memcpy(Node->Data, Data, DataSize);
  Node->Next = NULL;
  Node->Prev = NULL;
  return Node;
}

void LinkedList_Node_Free(Node_t** Node)
{
  free((*Node)->Data);
  (*Node)->Data = NULL;
  free(*Node);
  *Node = NULL;
}

LinkedList_t* LinkedList_New()
{
  LinkedList_t* List = malloc(sizeof(LinkedList_t));
  List->Head = LinkedList_Node_New(NULL, 0);
  List->Tail = List->Head;
  List->Length = 0;

  return List;
}

void LinkedList_Free(LinkedList_t** List)
{
  Node_t* Current = (*List)->Head;
  while (Current)
  {
    Node_t* Next = Current->Next;
    free(Current->Data);
    free(Current);
    Current = Next;
  }
  free(*List);
  *List = NULL;
}

void LinkedList_Append(LinkedList_t* List, Node_t* NewNode)
{
  if (List->Length == 0)
  {
    List->Head = List->Tail = NewNode;
    List->Length = 1;
    return;
  }

  NewNode->Prev = List->Tail;
  List->Tail->Next = NewNode;
  List->Length++;
  List->Tail = NewNode;
}

void LinkedList_Prepend(LinkedList_t* List, Node_t* NewNode)
{
  if (List->Length == 0)
  {
    List->Head = List->Tail = NewNode;
    List->Length = 1;
    return;
  }

  NewNode->Prev = NULL;
  List->Length++;
  List->Head->Prev = NewNode;
  NewNode->Next = List->Head;
  List->Head = NewNode;
}

Node_t* LinkedList_Find(LinkedList_t* List, void* Data, size_t DataSize)
{
  for (Node_t* Current = List->Head; Current; Current = Current->Next)
    if (memcmp(Current->Data, Data, DataSize) == 0)
      return Current;

  return NULL;
}

void LinkedList_Remove(LinkedList_t* List, Node_t* Node)
{
  if (Node == List->Head)
  {
    List->Head = Node->Next;
    List->Head->Prev = NULL;
  }
  else if (Node == List->Tail)
  {
    List->Tail = Node->Prev;
    List->Tail->Next = NULL;
  }
  else
  {
    Node->Prev->Next = Node->Next;
    Node->Next->Prev = Node->Prev;
  }

  LinkedList_Node_Free(&Node);

  List->Length--;
}

#undef Node
#undef Node_t
#undef LinkedList_t
