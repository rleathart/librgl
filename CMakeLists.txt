cmake_minimum_required(VERSION 3.10)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(GenerateExportHeader)

project(rgl C)

add_library(${PROJECT_NAME} SHARED
  src/rgl.c
  src/Util/Util.c
  src/Util/GetUserInput.c
  src/LinkedList/LinkedList.c
  src/FileInfo/FileInfo.c
  )

if (WIN32) # No-one cares about your *_s functions, Microsoft.
  target_compile_definitions(${PROJECT_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

target_include_directories(${PROJECT_NAME} PUBLIC include)
GENERATE_EXPORT_HEADER(${PROJECT_NAME}
  BASE_NAME ${PROJECT_NAME}
  EXPORT_MACRO_NAME RGL_EXPORT
  EXPORT_FILE_NAME rgl_exports.h
  STATIC_DEFINE SHARED_EXPORTS_BUILT_AS_STATIC # Is this required?
  )

SET(TEST_EXE librgl_test)

add_executable(${TEST_EXE} main.c)
target_link_libraries(${TEST_EXE} ${PROJECT_NAME})
target_include_directories(${TEST_EXE} PUBLIC include)
