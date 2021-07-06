#include <stdio.h>
#include <stdlib.h>

#include <rgl/ringbuffer.h>

int main(int argc, char** argv)
{
  RingBuffer rb;
  ringbuffer_new(&rb, 128, true);

  for (int i = 0; i < 256; i++)
  {
    int* ptr = malloc(sizeof(int*));
    *ptr = i;
    ringbuffer_push(&rb, ptr);
    if (i % 2 == 1)
      ringbuffer_pop(&rb);
  }

  u64 oldused = rb.used;
  for (int i = 0; i < oldused; i++)
    printf("rb[%d]: %d\n", i, *(int*)ringbuffer_pop(&rb));
}
