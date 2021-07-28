#include <rgl/array.h>

#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv)
{
  Array arr;
  array_new(&arr, 8, sizeof(int));

  void* null_data = calloc(8, sizeof(int));
  void* null_bitmask = calloc(1, 1);

  if (arr.capacity != 8)
    return 1;
  if (arr.back != 0 || arr.front != 0)
    return 2;
  if (arr.data_size != sizeof(int))
    return 3;
  if (memcmp(null_data, arr.data, 8 * sizeof(int)) != 0)
    return 4;
  if (memcmp(null_bitmask, arr.allocation_bitmask, 1) != 0)
    return 5;
  if (arr.used != 0)
    return 6;

  return 0;
}
