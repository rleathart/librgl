#include <rgl/threadpool.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
  Task* task;
  for (;;)
  {
    // Aquire lock
    pthread_mutex_lock(&pool->mutex);
    // wait for condition
    while (pool->pending_tasks == 0)
      pthread_cond_wait(&pool->cond, &pool->mutex);
    // Get task from queue
    task = pool->queue[pool->head];
    pool->head = (pool->head + 1) % (pool->queue_size * 2);
    pool->pending_tasks--;

    // Release lock
    pthread_mutex_unlock(&pool->mutex);
    // Do some work
    task->result = task->func(task->args);
    task->is_complete = true;
  }
  return NULL;
}

void threadpool_new(ThreadPool* pool, u64 nthreads)
{
  memset(pool, 0, sizeof(*pool));
  pool->threads = malloc(nthreads * sizeof(*pool->threads));
  pool->queue_size = 64; // Initial queue size, we will grow if needed
  pool->queue = malloc(pool->queue_size * sizeof(Task*));

  pthread_mutex_init(&pool->mutex, NULL);
  pthread_cond_init(&pool->cond, NULL);

  for (int i = 0; i < nthreads; i++)
  {
    pthread_create(&pool->threads[i], NULL, worker_thread, pool);
    pool->nthreads++;
  }
}

void threadpool_queue_task(ThreadPool* pool, Task* task)
{
  pthread_mutex_lock(&pool->mutex);

  // Queue is full, let's resize
  if (pool->pending_tasks == pool->queue_size)
    pool->queue = realloc(pool->queue, (pool->queue_size *= 2) * sizeof(Task*));

  pool->queue[pool->tail] = task;
  pool->tail = (pool->tail + 1) % (pool->queue_size * 2);
  pool->pending_tasks++;

  pthread_cond_signal(&pool->cond);
  pthread_mutex_unlock(&pool->mutex);
}
