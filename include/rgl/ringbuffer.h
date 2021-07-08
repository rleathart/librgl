#pragma once

#include <stdbool.h>

#include <rgl/defs.h>

typedef struct
{
  u64 head;      // Index of item to be read next
  u64 tail;      // Index of the most recently pushed item
  u64 used;      // How many slots are currently in use
  u64 data_size; // Size of the data that is being stored. Used to copy on push
  u64 capacity;  // Max capacity of this buffer
  bool resize_when_full; // Either we can resize or overwrite when full
  void** data;
} RingBuffer;

/**
 * @brief Initialises a new ring buffer.
 * @param[out, non-allocated] self
 */
void ringbuffer_new(RingBuffer* self, u64 capacity, u64 data_size,
                    bool resize_when_full);
void ringbuffer_push(RingBuffer* self, void* data);
void* ringbuffer_pop(RingBuffer* self);
