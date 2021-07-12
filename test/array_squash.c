#include <rgl/array.h>

int main(void)
{
  Array arr;
  int i = 0;
  array_new(&arr, 7, sizeof(int));
  i = 1;
  array_set(&arr, &i, 0);
  i = 2;
  array_set(&arr, &i, 3);
  i = 3;
  array_set(&arr, &i, 5);
  i = 4;
  array_set(&arr, &i, 6);

  array_squash(&arr);

  int expected[] = {1, 2, 3, 4};
  int expected_len = sizeof(expected) / sizeof(expected[0]);

  for (int i = 0; i < arr.capacity; i++)
  {
    if (!array_index_is_allocated(&arr, i))
      return 1;
    if (array_get_as(&arr, i, int) != expected[i])
      return 1;
  }

  return 0;
}
