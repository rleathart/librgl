#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <rgl/array.h>
#include <rgl/logging.h>

bool array_index_is_allocated(Array* self, u64 index)
{
  u64 offset = index / 8;
  u64 bit = index % 8;
  byte b = self->allocated_indexes[offset];
  return (b >> bit) & 1;
}

void set_index_allocated(Array* self, u64 index, bool is_allocated)
{
  u64 offset = index / 8;
  u64 bit = index % 8;

  if (is_allocated)
    self->allocated_indexes[offset] |= 1 << bit;
  else
    self->allocated_indexes[offset] &= ~(1 << bit);
}

void array_new(Array* self, u64 initial_capacity, u64 data_size)
{
  memset(self, 0, sizeof(*self));
  self->data_size = data_size;
  self->capacity = initial_capacity;
  self->allocated_indexes =
      calloc(initial_capacity / 8 + (initial_capacity % 8 != 0), sizeof(byte));
  self->data = malloc(self->capacity * sizeof(void*));
}

void array_free(Array* self)
{
  for (u64 i = 0; i < self->capacity; i++)
    if (array_index_is_allocated(self, i))
      free(self->data[i]);
}

void array_resize(Array* self, u64 new_nelem)
{
  byte* old_block = self->allocated_indexes;
  u64 old_size = self->capacity / 8 + (self->capacity % 8 != 0);

  self->data = realloc(self->data, (self->capacity = new_nelem) * sizeof(void*));

  self->allocated_indexes =
      calloc(self->capacity / 8 + (self->capacity % 8 != 0), sizeof(byte));

  memcpy(self->allocated_indexes, old_block, old_size);
}

void array_push(Array* self, void* data)
{
  if (self->back == self->capacity - 1)
    array_resize(self, self->capacity * 2);
  array_set(self, data, self->back + array_index_is_allocated(self, self->back));
}

void array_set(Array* self, void* data, u64 index)
{
  if (index >= self->capacity || index < 0)
    ELOG("Index %llu out of bounds for array %p\n", index, self);

  if (array_index_is_allocated(self, index)) // Can just reuse exisitng memory block
    memcpy(self->data[index], data, self->data_size);
  else // Need to allocate new memory
    memcpy((self->data[index] = malloc(self->data_size)), data,
           self->data_size);

  if (index > self->back)
    self->back = index;
  if (index < self->front)
    self->front = index;

  set_index_allocated(self, index, true);
  self->used++;
}

void* array_get(Array* self, u64 index)
{
  if (index >= self->capacity || index < 0)
  {
    ELOG("Index %llu out of bounds for array %p\n", index, self);
    return NULL;
  }
  if (!array_index_is_allocated(self, index))
    WLOG("Accessing uninitialised memory at index %llu in array %p\n", index,
         self);

  return self->data[index];
}

void array_remove(Array* self, u64 index)
{
  if (index >= self->capacity || index < 0)
  {
    ELOG("Index %llu out of bounds for array %p\n", index, self);
    return;
  }
  if (!array_index_is_allocated(self, index))
  {
    WLOG("Index %llu has not been assigned for array %p\n", index,
        self);
    return;
  }

  set_index_allocated(self, index, false);
  self->used--;
  free(self->data[index]);
}
