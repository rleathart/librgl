#include <rgl/fileinfo.h>

#include <stdlib.h>
#include <string.h>

int fileinfo_open(FileInfo* self, char* mode)
{
  self->file_pointer = fopen(self->file_path, mode);
  return !self->file_pointer;
}

int fileinfo_close(FileInfo* self)
{
  int Return = fclose(self->file_pointer);
  self->file_pointer = NULL;
  return Return;
}

char** fileinfo_read_lines(FileInfo* self, int* line_count)
{
  *line_count = 0;

  if (!self->file_pointer)
    fileinfo_open(self, "r");
  if (!self->file_pointer)
    return NULL;

  int c;
  while ((c = fgetc(self->file_pointer)) != EOF)
    if (c == '\n')
      ++*line_count;
  fileinfo_close(self);

  if (*line_count == 0)
    return NULL;

  // Count length of each line into array
  int LineLengths[*line_count];
  fileinfo_open(self, "r");
  for (unsigned long i = 0; i < *line_count; ++i)
  {
    LineLengths[i] = 1; // Need n+1 chars for null terminator
    while ((c = fgetc(self->file_pointer)) != EOF)
    {
      ++LineLengths[i];
      if (c == '\n')
        break;
    }
  }
  fileinfo_close(self);

  char** LineArray = calloc(*line_count, sizeof(char*));

  fileinfo_open(self, "r");
  for (int i = 0; i < *line_count; ++i)
  {
    LineArray[i] = calloc(LineLengths[i], sizeof(char));
    fgets(LineArray[i], LineLengths[i], self->file_pointer);

    int StringLength = strlen(LineArray[i]);

    // Remove newline from each string.
    if (LineArray[i][StringLength - 1] == '\n')
      LineArray[i][StringLength - 1] = '\0';
  }
  fileinfo_close(self);

  return LineArray;
}

bool fileinfo_read_line(FileInfo* self, char* buffer, size_t size)
{
  if (!self->file_pointer)
    fileinfo_open(self, "r");
  if (!self->file_pointer)
    return false;

  buffer[0] = '\0';
  buffer[size - 1] = '\0';

  if (fgets(buffer, size, self->file_pointer))
  {
    strtok(buffer, "\n");
    return true;
  }

  return false;
}

bool fileinfo_exists(FileInfo *self)
{
  int Return = fileinfo_open(self, "r");
  if (Return == 0)
    fileinfo_close(self);

  return Return == 0;
}

void fileinfo_free(FileInfo** self)
{
  fileinfo_close(*self);
  free(*self);
  *self = NULL;
}

FileInfo* fileinfo_new(char* file_path)
{
  FileInfo* self = malloc(sizeof(FileInfo));
  self->file_path = file_path;
  self->file_pointer = NULL;

  return self;
}
