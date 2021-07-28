#include <rgl/array.h>

int main(int argc, char** argv)
{
  Array arr;
  array_new(&arr, 1, sizeof(int));

  int i = 42;
  array_push(&arr, &i);

  if (arr.back != 0 || arr.front != 0)
    return 1;
  if (*(int*)array_get(&arr, arr.back) != i)
    return 2;

  i++;

  array_push(&arr, &i);

  // Should resize the array
  if (arr.back != 1 || arr.front != 0)
    return 3;
  if (*(int*)array_get(&arr, arr.back) != i)
    return 4;
  if (arr.capacity != 2)
    return 5;

  i++;

  // array_push should push after the last allocated index, not into the first
  // unallocated slot
  array_resize(&arr, 8);
  array_set(&arr, 5, &i);
  i++;
  array_push(&arr, &i);

  if (arr.back != 6)
    return 6;
  if (*(int*)array_get(&arr, arr.back) != i)
    return 7;
  for (int i = 2; i < 5; i++)
    if (array_index_is_allocated(&arr, i))
      return 8;

  return 0;
}
