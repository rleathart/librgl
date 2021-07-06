#pragma once

#include <stdbool.h>

#include <rgl/defs.h>

typedef struct
{
  int head;      // Item to be read next
  int tail;      // Item at the end of the 'queue'
  int used;      // How many slots are currently in use
  int capacity;  // Max capacity of this buffer
  bool resize_when_full; // Either we can resize or overwrite when full
  void** data;
} RingBuffer;

void ringbuffer_new(RingBuffer* self, u64 capacity, bool resize_when_full);
void ringbuffer_push(RingBuffer* self, void* data);
void* ringbuffer_pop(RingBuffer* self);
