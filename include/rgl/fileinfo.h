#pragma once

#include <stdio.h>
#include <stdbool.h>

typedef struct
{
  char* file_path;
  FILE* file_pointer;
} FileInfo;

void fileinfo_free(FileInfo** self);
FileInfo* fileinfo_new(char* file_path);
int fileinfo_open(FileInfo* self, char* mode);
int fileinfo_close(FileInfo* self);
char** fileinfo_read_lines(FileInfo* self, int* line_count);
bool fileinfo_read_line(FileInfo* self, char* buffer, size_t size);
bool fileinfo_exists(FileInfo* self);
