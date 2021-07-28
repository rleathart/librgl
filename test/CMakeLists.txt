cmake_minimum_required(VERSION 3.10)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(CTest)

function(rgl_add_tests)
  foreach(test_name IN LISTS ARGV)
    add_executable(test_${test_name} ${test_name})
    target_link_libraries(test_${test_name} ${PROJECT_NAME})
    add_test(NAME ${test_name} COMMAND test_${test_name})
  endforeach()
endfunction()

rgl_add_tests(
  threadpool.c
  queue.c
  ringbuffer.c
  logging.c
  array.c
  array_new.c
  array_push.c
  array_get.c
  array_set.c
  array_resize.c
  array_squash.c
  array_concat.c
  )
