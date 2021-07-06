#include "rgl/linkedlist.h"
#include <rgl/queue.h>

Queue* queue_new()
{
  return linkedlist_new();
}

void queue_push(Queue* self, void* data, u64 data_size)
{
  linkedlist_append(self, node_new(data, data_size));
}

Node* queue_pop(Queue* self)
{
  if (self->length == 0)
    return NULL;

  Node* item = self->head;
  self->head = self->head->next;
  self->length--;
  return item;
}
