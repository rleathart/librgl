#include <rgl/array.h>

#include <string.h>

int main(int argc, char** argv)
{
  Array arr;
  array_new(&arr, 8, sizeof(int));
  int i = 42;
  array_set(&arr, 3, &i);

  if (memcmp(&arr.data[3 * sizeof(int)], &i, sizeof(int)) != 0)
    return 1;
  if (!array_index_is_allocated(&arr, 3))
    return 2;

  // array_set should update arr.back
  if (arr.back != 3)
    return 3;
  if (arr.front != arr.back)
    return 4;

  i++;

  array_set(&arr, 1, &i);
  // array_set should update arr.front
  if (arr.front != 1)
    return 5;
  array_set(&arr, 2, &i);
  if (arr.front != 1)
    return 6;

  for (int i = 0; i < arr.capacity; i++)
  {
    if (i == 1 || i == 2 || i == 3)
    {
      if (!array_index_is_allocated(&arr, i))
        return 7;
    }
    else
    {
      if (array_index_is_allocated(&arr, i))
        return 8;
    }
  }

  return 0;
}
