#include <rgl/array.h>

int main(void)
{
  Array arr;
  int i = 0;
  array_new(&arr, 7, sizeof(int));
  i = 1;
  array_set(&arr, 0, &i);
  i = 2;
  array_set(&arr, 3, &i);
  i = 3;
  array_set(&arr, 5, &i);
  i = 4;
  array_set(&arr, 6, &i);

  array_squash(&arr);

  int expected[] = {1, 2, 3, 4};
  int expected_len = sizeof(expected) / sizeof(expected[0]);

  for (int i = 0; i < arr.capacity; i++)
  {
    if (!array_index_is_allocated(&arr, i))
      return 1;
    if (*(int*)array_get(&arr, i) != expected[i])
      return 1;
  }

  return 0;
}
