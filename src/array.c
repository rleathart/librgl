#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <rgl/array.h>
#include <rgl/logging.h>

bool array_index_is_allocated(Array* self, u64 index)
{
  if (index >= self->capacity || index < 0)
    ELOG("Index %" PRIu64 " out of bounds for array %p\n", index, self);
  u64 offset = index / 8;
  u64 bit = index % 8;
  byte b = self->allocation_bitmask[offset];
  return (b >> bit) & 1;
}

void set_index_allocated(Array* self, u64 index, bool is_allocated)
{
  if (index >= self->capacity || index < 0)
    ELOG("Index %" PRIu64 " out of bounds for array %p\n", index, self);
  u64 offset = index / 8;
  u64 bit = index % 8;

  if (is_allocated)
    self->allocation_bitmask[offset] |= 1 << bit;
  else
    self->allocation_bitmask[offset] &= ~(1 << bit);
}

void array_new(Array* self, u64 initial_capacity, u64 data_size)
{
  memset(self, 0, sizeof(*self));
  self->data_size = data_size;
  self->capacity = initial_capacity;
  self->allocation_bitmask =
      calloc(initial_capacity / 8 + (initial_capacity % 8 != 0), sizeof(byte));
  self->data = calloc(self->capacity, self->data_size);
}

void array_free(Array* self)
{
  free(self->data);
  free(self->allocation_bitmask);
}

void array_resize(Array* self, u64 new_nelem)
{
  if (new_nelem < self->back)
    WLOG("Shrinking array from capacity %" PRIu64 " to capacity %" PRIu64
         "! Data will be lost!\n",
         self->capacity, new_nelem);

  byte* old_block = self->allocation_bitmask;
  u64 old_size = self->capacity / 8 + (self->capacity % 8 != 0);

  self->data =
      realloc(self->data, (self->capacity = new_nelem) * self->data_size);

  self->allocation_bitmask =
      calloc(self->capacity / 8 + (self->capacity % 8 != 0), sizeof(byte));

  memcpy(self->allocation_bitmask, old_block, old_size);
  free(old_block);
}

void array_push(Array* self, void* data)
{
  if (self->back == self->capacity - 1)
    array_resize(self, self->capacity * 2);
  array_set(self, self->back + array_index_is_allocated(self, self->back),
            data);
}

void array_set(Array* self, u64 index, void* data)
{
  if (index >= self->capacity || index < 0)
    ELOG("Index %" PRIu64 " out of bounds for array %p\n", index, self);

  memcpy(&self->data[index * self->data_size], data, self->data_size);

  if (index > self->back)
    self->back = index;
  // Since self->front is initialsed to 0, we need this to handle the case that
  // the first index set is not 0.
  if (index < self->front ||
      (self->front == 0 && !array_index_is_allocated(self, self->front)))
    self->front = index;

  set_index_allocated(self, index, true);
  self->used++;
}

void* array_get(Array* self, u64 index)
{
  if (index >= self->capacity || index < 0)
  {
    ELOG("Index %" PRIu64 " out of bounds for array %p\n", index, self);
    return NULL;
  }
  if (!array_index_is_allocated(self, index))
    WLOG("Accessing uninitialised memory at index %" PRIu64 " in array %p\n",
         index, self);

  return &self->data[index * self->data_size];
}

void array_remove(Array* self, u64 index)
{
  if (index >= self->capacity || index < 0)
  {
    ELOG("Index %" PRIu64 " out of bounds for array %p\n", index, self);
    return;
  }
  if (!array_index_is_allocated(self, index))
  {
    WLOG("Index %" PRIu64 " has not been assigned for array %p\n", index, self);
    return;
  }

  set_index_allocated(self, index, false);
  self->used--;

  if (index == self->front)
    for (u64 i = self->front; i < self->capacity; i++)
      if (array_index_is_allocated(self, i))
      {
        self->front = i;
        break;
      }

  if (index == self->back)
    for (u64 i = self->back - 1;; i--)
    {
      if (array_index_is_allocated(self, i))
      {
        self->back = i;
        break;
      }

      if (i == 0)
        break;
    }
}

Array* array_copy(Array* dest, Array src)
{
  *dest = src;

  dest->data = calloc(dest->capacity, dest->data_size);
  dest->allocation_bitmask =
      calloc(dest->capacity / 8 + (dest->capacity % 8 != 0), 1);

  memcpy(dest->allocation_bitmask, src.allocation_bitmask,
         src.capacity / 8 + (src.capacity % 8 != 0));
  memcpy(dest->data, src.data, src.capacity * src.data_size);

  return dest;
}

Array* array_squash(Array* self)
{
  // Just make a new array and push all the elements of self onto it
  Array* squashed = malloc(sizeof(Array));
  array_new(squashed, self->used, self->data_size);

  for (u64 i = 0; i < self->capacity; i++)
    if (array_index_is_allocated(self, i))
      array_push(squashed, array_get(self, i));

  array_free(self);
  *self = *squashed;
  free(squashed);

  return self;
}

Array* array_concat(Array* self, Array arr)
{
  if (self->data_size != arr.data_size)
  {
    ELOG("Cannot concat arrays with sizes %" PRIu64 " and %" PRIu64 "!\n",
         self->data_size, arr.data_size);
    return NULL;
  }
  u64 idx = self->capacity;
  array_resize(self, self->capacity + arr.capacity);
  for (u64 i = 0; i < arr.capacity; i++)
  {
    if (array_index_is_allocated(&arr, i))
      array_set(self, idx, array_get(&arr, i));
    idx++;
  }

  return self;
}
