#include <rgl/array.h>

int main(int argc, char** argv)
{
  Array arr;
  array_new(&arr, 8, sizeof(int));
  int i = 42;
  array_push(&arr, &i);

  if (*(int*)array_get(&arr, 0) != i)
    return 1;
  return 0;
}
