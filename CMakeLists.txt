cmake_minimum_required(VERSION 3.10)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (WIN32)
  add_compile_definitions(
    _CRT_SECURE_NO_WARNINGS
    _CRT_NONSTDC_NO_DEPRECATE
    )
endif()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "" FORCE)
endif()

if(CMAKE_BUILD_TYPE MATCHES Debug)
  add_compile_definitions(_DEBUG=1 DEBUG=1)
endif()

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS YES CACHE BOOL "Export all symbols")

# Read CMAKE_PREFIX_PATH from environment if it's not set
if(NOT CMAKE_PREFIX_PATH)
  set(CMAKE_PREFIX_PATH $ENV{CMAKE_PREFIX_PATH})
  # Convert from Unix path sep
  string(REPLACE ":" ";" CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH}")
endif()

foreach(path ${CMAKE_PREFIX_PATH})
  include_directories(${path}/include)
  link_directories(${path}/lib)
endforeach(path)

project(rgl C)

include_directories(include)

set(SRC
  src/util.c
  src/linkedlist.c
  src/queue.c
  src/ringbuffer.c
  src/fileinfo.c
  src/threadpool.c
  src/logging.c
  src/array.c
  )

set(RGL_LIBS)
if(WIN32)
  find_package(pthread REQUIRED)
  list(APPEND RGL_LIBS pthread)
endif()

add_library(${PROJECT_NAME} ${SRC})
target_link_libraries(${PROJECT_NAME} ${RGL_LIBS})

if(RGL_BUILD_TESTS)
  include(CTest)
  add_executable(test_threadpool test/threadpool.c)
  target_link_libraries(test_threadpool ${PROJECT_NAME})
  add_test(NAME threadpool COMMAND test_threadpool)

  add_executable(test_queue test/queue.c)
  target_link_libraries(test_queue ${PROJECT_NAME})
  add_test(NAME queue COMMAND test_queue)

  add_executable(test_ringbuffer test/ringbuffer.c)
  target_link_libraries(test_ringbuffer ${PROJECT_NAME})
  add_test(NAME ringbuffer COMMAND test_ringbuffer)

  add_executable(test_logging test/logging.c)
  target_link_libraries(test_logging ${PROJECT_NAME})
  add_test(NAME logging COMMAND test_logging)

  add_executable(test_array test/array.c)
  target_link_libraries(test_array ${PROJECT_NAME})
  add_test(NAME array COMMAND test_array)

  add_executable(test_array_squash test/array_squash.c)
  target_link_libraries(test_array_squash ${PROJECT_NAME})
  add_test(NAME array_squash COMMAND test_array_squash)

  add_executable(test_array_concat test/array_concat.c)
  target_link_libraries(test_array_concat ${PROJECT_NAME})
  add_test(NAME array_concat COMMAND test_array_concat)
endif()

################################################################################
#                         INSTALLATION
# Read CMAKE_INSTALL_PREFIX from environment if not given on the command-line
if(DEFINED ENV{CMAKE_INSTALL_PREFIX} AND
    CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX $ENV{CMAKE_INSTALL_PREFIX} CACHE
    PATH "Installation directory" FORCE)
endif()

message("Install prefix: " ${CMAKE_INSTALL_PREFIX})
target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE src)

install(
  TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Config
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  )

install(DIRECTORY include/ DESTINATION include)
install(EXPORT ${PROJECT_NAME}Config DESTINATION lib/cmake/${PROJECT_NAME})
export(TARGETS ${PROJECT_NAME} FILE ${PROJECT_NAME}Config.cmake)
################################################################################
