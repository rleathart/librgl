#include <rgl/array.h>

// array_remove(self, index) should:
//  Mark the index as unallocated.
//  Update front and back.

int main(int argc, char** argv)
{
  Array arr;
  array_new(&arr, 8, sizeof(int));

  for (int i = 0; i < 8; i++)
    array_push(&arr, &i);

  if (arr.front != 0 || arr.back != 7)
    return 1;

  array_remove(&arr, 4);

  if (array_index_is_allocated(&arr, 4))
    return 2;

  if (arr.front != 0 || arr.back != 7)
    return 3;

  array_remove(&arr, 0);

  if (arr.front != 1)
    return 4;

  array_remove(&arr, 7);

  if (arr.back != 6)
    return 5;

  return 0;

}
