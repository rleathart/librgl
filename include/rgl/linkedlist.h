#pragma once

#include <stdlib.h>

typedef struct Node Node;
struct Node
{
  void* data;
  Node* next;
  Node* prev;
};

typedef struct
{
  Node* head;
  Node* tail;

  size_t length;
} LinkedList;


Node* linkedlist_node_new(void* data, size_t data_size);
void linkedlist_node_free(Node** node);
LinkedList* linkedlist_new();
void linkedlist_free(LinkedList** list);
int linkedlist_insert_after(LinkedList* list, Node* newnode, size_t index);
void linkedlist_append(LinkedList* list, Node* newnode);
void linkedlist_prepend(LinkedList* list, Node* newnode);
size_t linkedlist_findindex(LinkedList* list, void* data, size_t datasize);
Node* linkedlist_find(LinkedList* list, void* data, size_t datasize);
void linkedlist_remove(LinkedList* list, Node* node);
