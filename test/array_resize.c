#include <rgl/array.h>
#include <rgl/logging.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv)
{
  Array arr;
  array_new(&arr, 8, sizeof(int));

  int i = 42;
  array_set(&arr, 4, &i);

  void* data_copy = malloc(arr.capacity * arr.data_size);
  memcpy(data_copy, arr.data, arr.capacity * arr.data_size);

  array_resize(&arr, 16);
  if (arr.capacity != 16)
    return 1;

  if (arr.back != 4)
    return 2;

  array_resize(&arr, 2);
  if (arr.capacity != 2)
    return 3;

  return 0;
}
