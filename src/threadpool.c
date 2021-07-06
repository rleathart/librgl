#include <rgl/threadpool.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

Task* task_new(Task* task, void* (*func)(void*), void* args)
{
  if (!task)
    task = malloc(sizeof(*task));

  memset(task, 0, sizeof(*task));
  task->is_complete = false;
  task->func = func;
  task->args = args;

  return task;
}

static void* worker_thread(void* void_args)
{
  ThreadPool* pool = (ThreadPool*)void_args;
  int err = 0;
  for (;;)
  {
    // Aquire lock
    err = pthread_mutex_lock(&pool->mutex);
    assert(!err);
    // No pending tasks so wait for one to be queued
    while (pool->queue->used == 0)
      err = pthread_cond_wait(&pool->cond, &pool->mutex);
    assert(!err);

    // Get task from queue
    Task* task = ringbuffer_pop(pool->queue);
    assert(task);

    // Release lock
    err = pthread_mutex_unlock(&pool->mutex);
    assert(!err);
    // Do some work
    if (task)
    {
      printf("task: %p\n", task);
      task->result = task->func(task->args);
      task->is_complete = true;
    }
  }
  return NULL;
}

void threadpool_new(ThreadPool* pool, u64 nthreads)
{
  int err = 0;
  memset(pool, 0, sizeof(*pool));
  pool->threads = malloc(nthreads * sizeof(*pool->threads));
  pool->queue = malloc(sizeof(*pool->queue));
  ringbuffer_new(pool->queue, 128, true);

  err = pthread_mutex_init(&pool->mutex, NULL);
  assert(!err);
  err = pthread_cond_init(&pool->cond, NULL);
  assert(!err);

  for (int i = 0; i < nthreads; i++)
  {
    err = pthread_create(&pool->threads[i], NULL, worker_thread, pool);
    assert(!err);
    pool->nthreads++;
  }
}

void threadpool_queue_task(ThreadPool* pool, Task* task)
{
  int err = 0;
  err = pthread_mutex_lock(&pool->mutex);
  assert(!err);

  ringbuffer_push(pool->queue, task);

  err = pthread_cond_signal(&pool->cond);
  assert(!err);
  err = pthread_mutex_unlock(&pool->mutex);
  assert(!err);
}
