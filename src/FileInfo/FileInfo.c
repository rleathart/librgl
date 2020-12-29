#include "FileInfo.h"

int FileInfo_Open(FileInfo_t* self, char* Mode)
{
  self->FilePointer = fopen(self->FilePath, Mode);
  return !self->FilePointer;
}

int FileInfo_Close(FileInfo_t* self)
{
  int Return = fclose(self->FilePointer);
  self->FilePointer = NULL;
  return Return;
}

char** FileInfo_ReadLines(FileInfo_t* self, int* LineCount)
{
  *LineCount = 0;

  if (!self->FilePointer)
    FileInfo_Open(self, "r");
  if (!self->FilePointer)
    return NULL;

  int c;
  while ((c = fgetc(self->FilePointer)) != EOF)
    if (c == '\n')
      ++*LineCount;
  FileInfo_Close(self);

  if (*LineCount == 0)
    return NULL;

  // Count length of each line into array
  int LineLengths[*LineCount];
  FileInfo_Open(self, "r");
  for (unsigned long i = 0; i < *LineCount; ++i)
  {
    LineLengths[i] = 1; // Need n+1 chars for null terminator
    while ((c = fgetc(self->FilePointer)) != EOF)
    {
      ++LineLengths[i];
      if (c == '\n')
        break;
    }
  }
  FileInfo_Close(self);

  char** LineArray = calloc(*LineCount, sizeof(char*));

  FileInfo_Open(self, "r");
  for (int i = 0; i < *LineCount; ++i)
  {
    LineArray[i] = calloc(LineLengths[i], sizeof(char));
    fgets(LineArray[i], LineLengths[i], self->FilePointer);

    int StringLength = strlen(LineArray[i]);

    // Remove newline from each string.
    if (LineArray[i][StringLength - 1] == '\n')
      LineArray[i][StringLength - 1] = '\0';
  }
  FileInfo_Close(self);

  return LineArray;
}

int FileInfo_ReadLine(FileInfo_t* self, char* Buffer, size_t size)
{
  if (!self->FilePointer)
    FileInfo_Open(self, "r");
  if (!self->FilePointer)
    return EXIT_FAILURE;

  Buffer[0] = '\0';
  Buffer[size - 1] = '\0';

  if (fgets(Buffer, size, self->FilePointer))
  {
    strtok(Buffer, "\n");
    return 1;
  }

  return 0;
}

void FileInfo_Free(FileInfo_t** self)
{
  FileInfo_Close(*self);
  free(*self);
  *self = NULL;
}

FileInfo_t* FileInfo_New(char* FilePath)
{
  FileInfo_t* self = malloc(sizeof(FileInfo_t));
  self->FilePath = FilePath;
  self->FilePointer = NULL;

  return self;
}
