#include <rgl/array.h>

int main(void)
{
  Array one;
  Array two;
  array_new(&one, 5, sizeof(int));
  array_new(&two, 7, sizeof(int));

  int i = 1;
  array_set(&one, &i, 0);
  i++;
  array_set(&one, &i, 2);
  i++;
  array_set(&one, &i, 3);
  i++;

  array_set(&two, &i, 1);
  i++;
  array_set(&two, &i, 2);
  i++;
  array_set(&two, &i, 4);
  i++;

  array_concat(&one, two);

  int expected[] = {1, 0, 2, 3, 0, 0, 4, 5, 0, 6, 0, 0};
  int expected_len = sizeof(expected) / sizeof(expected[0]);

  if (one.capacity != expected_len)
    return 1;

  for (int i = 0; i < expected_len; i++)
  {
    if (expected[i] == 0 && array_index_is_allocated(&one, i))
      return 1;
    if (expected[i] != 0 && !array_index_is_allocated(&one, i))
      return 1;
    if (array_index_is_allocated(&one, i))
      if (array_get_as(&one, i, int) != expected[i])
        return 1;
  }

  return 0;
}
