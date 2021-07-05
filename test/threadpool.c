#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include <rgl/fileinfo.h>
#include <rgl/linkedlist.h>
#include <rgl/threadpool.h>
#include <rgl/util.h>

typedef struct
{
  char* printed;
} Args;

void* print_hello(void* void_args)
{
  Args* args = (Args*)void_args;
  args->printed = "Hello";
  printf("%s\n", args->printed);
  return (void*)42;
}

void* print_world(void* void_args)
{
  Args* args = (Args*)void_args;
  args->printed = "World";
  printf("%s\n", args->printed);
  return (void*)43;
}

// @@FIXME For some reason this test works fine just running the executable but
// segfaults sometimes when using CTest?
int main(int argc, char* argv[])
{
  ThreadPool pool;
  threadpool_new(&pool, 4);
  Task* tasks[8];
  Args args[8];

  for (int i = 0; i < 8; i++)
  {
    tasks[i] = task_new(NULL, i % 2 == 0 ? print_hello : print_world, &args[i]);
    threadpool_queue_task(&pool, tasks[i]);
  }

  for (;;)
  {
    bool should_exit = true;
    for (int i = 0; i < 8; i++)
      should_exit = should_exit && tasks[i]->is_complete;

    if (should_exit)
      break;
  }

  for (int i = 0; i < 8; i++)
    printf("task[%d] returned: %lld and printed %s\n", i, (u64)tasks[i]->result,
           ((Args*)tasks[i]->args)->printed);

  return 0;
}
