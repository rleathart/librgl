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

if(CMAKE_BUILD_TYPE STREQUAL RelWithDebInfo)
  add_compile_definitions(REL_WITH_DEB_INFO=1)
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

# Just dump everything in the build dir so the executables can find dlls
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

project(rgl C)

if(WIN32)
  add_subdirectory(extern/pthreads-win32-CMake)
endif()

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

add_library(${PROJECT_NAME} ${SRC})
target_link_libraries(${PROJECT_NAME} pthread)

if(RGL_BUILD_TESTS)
  include(CTest)
  add_subdirectory(test)
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
