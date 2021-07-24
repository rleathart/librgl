#include <rgl/array.h>

int main(void)
{
  Array one;
  Array two;
  array_new(&one, 5, sizeof(int));
  array_new(&two, 7, sizeof(int));

  int i = 1;
  array_set(&one, 0, &i);
  i++;
  array_set(&one, 2, &i);
  i++;
  array_set(&one, 3, &i);
  i++;

  array_set(&two, 1, &i);
  i++;
  array_set(&two, 2, &i);
  i++;
  array_set(&two, 4, &i);
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
      if (*(int*)array_get(&one, i) != expected[i])
        return 1;
  }

  return 0;
}
