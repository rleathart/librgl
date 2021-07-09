#include <stdio.h>

#include <rgl/array.h>

int main(void)
{
  Array arr;
  array_new(&arr, 7, sizeof(int));

  int i = 42;
  array_push(&arr, &i);
  i = 43;
  array_push(&arr, &i);

  for (int i = 0; i < arr.back; i++)
    printf("arr[%d]: %d\n", i, *(int*)array_get(&arr, i));
  array_get(&arr, 2);
  array_set(&arr, &i, 6);
  for (int i = 0; i < arr.capacity; i++)
    printf("is_allocated[%d]: %d\n", i, array_index_is_allocated(&arr, i));
  for (int i = 0; i < 12; i++)
    array_push(&arr, &i);
  for (int i = 0; i < arr.capacity; i++)
    printf("is_allocated[%d]: %d\n", i, array_index_is_allocated(&arr, i));
}
