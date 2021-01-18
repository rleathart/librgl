#pragma once

#include <rgl/LinkedList.h>

#include <stdlib.h>

#define Node_t rgl_LinkedList_Node_t
#define LinkedList_t rgl_LinkedList_t

Node_t* LinkedList_Node_New(void* Data, size_t DataSize);
void LinkedList_Node_Free(Node_t** Node);

LinkedList_t* LinkedList_New();
void LinkedList_Free(LinkedList_t** List);
void LinkedList_Append(LinkedList_t* List, Node_t* NewNode);
void LinkedList_Prepend(LinkedList_t* List, Node_t* NewNode);
Node_t* LinkedList_Find(LinkedList_t* List, void* Data, size_t DataSize);
size_t LinkedList_FindIndex(LinkedList_t* List, void* Data, size_t DataSize);
void LinkedList_Remove(LinkedList_t* List, Node_t* Node);
int LinkedList_InsertAfter(LinkedList_t* List, Node_t* NewNode, size_t Index);

#undef Node_t
#undef LinkedList_t
