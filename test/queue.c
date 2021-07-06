#include <rgl/queue.h>

#include <stdio.h>

int main(int argc, char** argv)
{
  Queue* queue = queue_new();

  for (int i = 0; i < 1238; i++)
  {
    int* ptr = malloc(sizeof(int));
    *ptr = i;
    queue_push(queue, ptr, sizeof(int));
    if (i % 2 == 1)
      queue_pop(queue);
  }

  int idx = 0;
  for (Node* item = queue->head; item; item = item->next)
    printf("queue[%d]: %d\n", idx++, *(int*)queue_pop(queue)->data);

  return 0;
}
