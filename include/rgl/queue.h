#pragma once

#include <rgl/defs.h>
#include <rgl/linkedlist.h>

typedef LinkedList Queue;

Queue* queue_new();
Node* queue_pop(Queue* self);
void queue_push(Queue* self, void* data, u64 data_size);
