#include <rgl/linkedlist.h>

#include <memory.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

Node* node_new(void* data, size_t data_size)
{
  Node* node = malloc(sizeof(*node));
  node->data = malloc(data_size);
  memcpy(node->data, data, data_size);
  node->next = NULL;
  node->prev = NULL;
  return node;
}

void node_free(Node** node)
{
  free((*node)->data);
  (*node)->data = NULL;
  free(*node);
  *node = NULL;
}

LinkedList* linkedlist_new()
{
  LinkedList* List = malloc(sizeof(LinkedList));
  List->head = List->tail = NULL;
  List->length = 0;

  return List;
}

void linkedlist_free(LinkedList** list)
{
  Node* Current = (*list)->head;
  while (Current)
  {
    Node* Next = Current->next;
    free(Current->data);
    free(Current);
    Current = Next;
  }
  free(*list);
  *list = NULL;
}

/* Initalise an empty list with a given node */
static void linkedlist_init(LinkedList* list, Node* newnode)
{
  list->head = list->tail = newnode;
  list->length = 1;
}

int linkedlist_insert_after(LinkedList* list, Node* newnode, size_t index)
{
  if (list->length == 0 && index == 0)
  {
    linkedlist_init(list, newnode);
    return EXIT_SUCCESS;
  }

  if (list->length == 0 && index > 0)
    return EXIT_FAILURE;

  if (index > list->length - 1)
    return EXIT_FAILURE;

  if (index == list->length - 1)
  {
    linkedlist_append(list, newnode);
    return EXIT_SUCCESS;
  }

  /* Start from the end of the list closest to the index. This avoids traversing
   * the entire list in order to insert after, say, List->Length - 2 */
  Node* Current;
  if (index < list->length / 2)
  {
    Current = list->head;
    for (size_t i = 0; i < index; ++i)
      Current = Current->next;
  }
  else
  {
    Current = list->tail;
    for (size_t i = list->length - 1; i > index; --i)
      Current = Current->prev;
  }

  // This shouldn't happen but check anyway.
  if (!Current->next)
    return EXIT_FAILURE;

  // We're inserting somewhere in the middle of the list.
  Current->next->prev = newnode;
  newnode->prev = Current;
  newnode->next = Current->next;
  Current->next = newnode;
  list->length++;

  return EXIT_SUCCESS;
}

void linkedlist_append(LinkedList* list, Node* newnode)
{
  if (list->length == 0)
    return linkedlist_init(list, newnode);

  newnode->prev = list->tail;
  list->tail->next = newnode;
  list->length++;
  list->tail = newnode;
}

void LinkedList_Prepend(LinkedList* List, Node* NewNode)
{
  if (List->length == 0)
    return linkedlist_init(List, NewNode);

  NewNode->prev = NULL;
  List->length++;
  List->head->prev = NewNode;
  NewNode->next = List->head;
  List->head = NewNode;
}

size_t linkedlist_find_index(LinkedList* list, void* data, size_t data_size)
{
  size_t Index = 0;
  for (Node* Current = list->head; Current; Current = Current->next)
  {
    if (memcmp(Current->data, data, data_size) == 0)
      return Index;
    ++Index;
  }
  // Not ideal, but need a way of indicating that node not in list.
  return -1;
}

Node* linkedlist_find(LinkedList* list, void* data, size_t data_size)
{
  for (Node* Current = list->head; Current; Current = Current->next)
    if (memcmp(Current->data, data, data_size) == 0)
      return Current;

  return NULL;
}

void linkedlist_remove(LinkedList* list, Node* node)
{
  if (node == list->head)
  {
    list->head = node->next;
    list->head->prev = NULL;
  }
  else if (node == list->tail)
  {
    list->tail = node->prev;
    list->tail->next = NULL;
  }
  else
  {
    node->prev->next = node->next;
    node->next->prev = node->prev;
  }

  node_free(&node);

  list->length--;
}
