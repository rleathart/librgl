#include <rgl/ringbuffer.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

void ringbuffer_new(RingBuffer* self, u64 capacity, u64 data_size,
                    bool resize_when_full)
{
  memset(self, 0, sizeof(*self));
  self->data_size = data_size;
  self->capacity = capacity;
  self->data = malloc(self->capacity * sizeof(void*));
  self->resize_when_full = resize_when_full;
}

void ringbuffer_push(RingBuffer* self, void* data)
{
  if (self->resize_when_full && ++self->used == self->capacity)
  {
    // Essentially we have 2 conditions here:
    //  1. Tail is at the end of the buffer i.e. self->tail = self->capacity - 1
    //  2. Tail is immediately behind head. i.e. self->tail = self->head - 1
    //
    //  In the first case, we can just resize the buffer and write into the new
    //  memory block.
    //  In the second case we need to resize the buffer and then copy all the
    //  data from self->head onwards to the end of the new memory block. E.g.
    //  If d: some data, _: unused space, t: tail, h: head
    //  On another push, this
    //  d t h d
    //  0 1 2 3
    //  becomes this:
    //  d t _ _ _ _ h d
    //  0 1 2 3 4 5 6 7
    self->data = realloc(self->data, (self->capacity *= 2) * sizeof(void*));
    assert(self->data);
    if (self->tail == self->head - 1)
    {
      int into = self->capacity - self->capacity / 2 + self->head;
      int from = self->head;
      int nidx = self->capacity / 2 - self->head;
      memcpy(&self->data[into], &self->data[from], nidx * sizeof(void*));
      self->head = into;
    }
  }
  else if (!self->resize_when_full)
    self->used++;

  self->data[self->tail] = malloc(self->data_size);
  memcpy(self->data[self->tail], data, self->data_size);
  self->tail = ++self->tail % self->capacity;
}

void* ringbuffer_pop(RingBuffer* self)
{
  if (self->used == 0)
    return NULL;
  int oldhead = self->head;
  self->head = ++self->head % self->capacity;
  self->used--;
  return self->data[oldhead];
}
