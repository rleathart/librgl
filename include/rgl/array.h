#pragma once

/// Generic dynamic array implementation

#include <stdbool.h>

#include <rgl/defs.h>

typedef struct
{
  u64 capacity;
  u64 used;
  u64 data_size;
  u64 front; // First index that has some data
  u64 back;  // Last index that has some data
  void* data;

  // This is essentially an array of bits where the state of each bit represents
  // whether that index in the array has been allocated. This saves us some
  // space vs a bool array.
  byte* allocation_bitmask;
} Array;

bool array_index_is_allocated(Array* self, u64 index);
void array_new(Array* self, u64 initial_capacity, u64 data_size);
void array_free(Array* self);
void array_push(Array* self, void* data);
void array_resize(Array* self, u64 new_nelem);
void array_set(Array* self, u64 index, void* data);
void* array_get(Array* self, u64 index);
void array_remove(Array* self, u64 index);
Array* array_copy(Array* dest, Array src);
/// @brief Converts to a contiguous array in place. Also resizes.
/// @return Array* self
Array* array_squash(Array* self);
/// @brief Concats arr onto self in place.
/// @return Array* self
Array* array_concat(Array* self, Array arr);
