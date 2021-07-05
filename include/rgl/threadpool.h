#pragma once

#include <pthread.h>
#include <stdbool.h>

#include <rgl/defs.h>

// What we want here is
//  Create a pool of n threads at application startup
//  Can queue tasks to run on these threads

typedef struct
{
  void* (*func)(void*);
  void* args;
  void* result;
  bool is_complete;
} Task;

typedef struct
{
  u64 nthreads;
  pthread_t* threads;

  Task** queue; // Array of Task pointers, means we can keep track of tasks
  u64 queue_size;
  u64 head;
  u64 tail;
  u64 pending_tasks;

  pthread_mutex_t mutex;
  pthread_cond_t cond;
} ThreadPool;

Task* task_new(Task* task, void* (*func)(void*), void* args);

void threadpool_new(ThreadPool* pool, u64 nthreads);
void threadpool_queue_task(ThreadPool* pool, Task* task);
