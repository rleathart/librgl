#pragma once

#include <rgl/rgl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdbool.h>

void FileInfo_Free(FileInfo_t** self);

FileInfo_t* FileInfo_New(char* FilePath);

int FileInfo_Open(FileInfo_t* self, char* Mode);

int FileInfo_Close(FileInfo_t* self);

char** FileInfo_ReadLines(FileInfo_t* self, int* LineCount);

bool FileInfo_ReadLine(FileInfo_t* self, char* Buffer, size_t size);

bool FileInfo_Exists(FileInfo_t* self);
