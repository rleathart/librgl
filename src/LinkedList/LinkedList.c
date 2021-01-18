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
  List->Head = List->Tail = NULL;
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

/* Initalise an empty list with a given node */
static void LinkedList_Init(LinkedList_t* List, Node_t* NewNode)
{
  List->Head = List->Tail = NewNode;
  List->Length = 1;
}

int LinkedList_InsertAfter(LinkedList_t* List, Node_t* NewNode, size_t Index)
{
  if (List->Length == 0 && Index == 0)
  {
    LinkedList_Init(List, NewNode);
    return EXIT_SUCCESS;
  }

  if (List->Length == 0 && Index > 0)
    return EXIT_FAILURE;

  if (Index > List->Length - 1)
    return EXIT_FAILURE;

  if (Index == List->Length - 1)
  {
    LinkedList_Append(List, NewNode);
    return EXIT_SUCCESS;
  }

  /* Start from the end of the list closest to the index. This avoids traversing
   * the entire list in order to insert after, say, List->Length - 2 */
  Node_t* Current;
  if (Index < List->Length / 2)
  {
    Current = List->Head;
    for (size_t i = 0; i < Index; ++i)
      Current = Current->Next;
  }
  else
  {
    Current = List->Tail;
    for (size_t i = List->Length - 1; i > Index; --i)
      Current = Current->Prev;
  }

  // This shouldn't happen but check anyway.
  if (!Current->Next)
    return EXIT_FAILURE;

  // We're inserting somewhere in the middle of the list.
  Current->Next->Prev = NewNode;
  NewNode->Prev = Current;
  NewNode->Next = Current->Next;
  Current->Next = NewNode;
  List->Length++;

  return EXIT_SUCCESS;
}

void LinkedList_Append(LinkedList_t* List, Node_t* NewNode)
{
  if (List->Length == 0)
    return LinkedList_Init(List, NewNode);

  NewNode->Prev = List->Tail;
  List->Tail->Next = NewNode;
  List->Length++;
  List->Tail = NewNode;
}

void LinkedList_Prepend(LinkedList_t* List, Node_t* NewNode)
{
  if (List->Length == 0)
    return LinkedList_Init(List, NewNode);

  NewNode->Prev = NULL;
  List->Length++;
  List->Head->Prev = NewNode;
  NewNode->Next = List->Head;
  List->Head = NewNode;
}

size_t LinkedList_FindIndex(LinkedList_t* List, void* Data, size_t DataSize)
{
  size_t Index = 0;
  for (Node_t* Current = List->Head; Current; Current = Current->Next)
  {
    if (memcmp(Current->Data, Data, DataSize) == 0)
      return Index;
    ++Index;
  }
  // Not ideal, but need a way of indicating that node not in list.
  return -1;
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
