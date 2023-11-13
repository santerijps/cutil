#ifndef CUTIL_H
#define CUTIL_H

#pragma region Includes

#ifndef true
#include <stdbool.h>
#endif

#ifndef _INC_STDDEF
#include <stddef.h>
#endif

#ifndef _STDINT_H
#include <stdint.h>
#endif

#pragma endregion
#pragma region Char and String

bool CharIsAlpha(char c);
bool CharIsDigit(char c);
bool CharIsUpper(char c);
bool CharIsLower(char c);
bool CharIn(char c, const char *haystack);

char CharUpper(char c);
char CharLower(char c);

size_t StringLength(const char *s);

bool StringIsAlpha(const char *s);
bool StringIsNumeric(const char *s);
bool StringIsAlphaNumeric(const char *s);

bool  StringUpperToBuffer(char *buffer, size_t buffer_size, const char *s);
char* StringUpperAlloc(const char *s);

bool  StringLowerToBuffer(char *buffer, size_t buffer_size, const char *s);
char* StringLowerAlloc(const char *s);

bool  StringTitleToBuffer(char *buffer, size_t buffer_size, const char *s);
char* StringTitleAlloc(const char *s);

bool  StringConcatToBuffer(char *buffer, size_t buffer_size, const char *s1, const char *s2);
char* StringConcatAlloc(const char *s1, const char *s2);

bool  StringSliceToBuffer(char *buffer, size_t buffer_size, const char *s, int64_t start, int64_t end);
char* StringSliceAlloc(const char *s, int64_t start, int64_t end);

bool  StringFirstNCharsToBuffer(char *buffer, size_t buffer_size, const char *s, uint64_t n);
char* StringFirstNCharsAlloc(const char *s, uint64_t n);

bool  StringLastNCharsToBuffer(char *buffer, size_t buffer_size, const char *s, uint64_t n);
char* StringLastNCharsAlloc(const char *s, uint64_t n);

bool  StringReverseToBuffer(char *buffer, size_t buffer_size, const char *s);
char* StringReverseAlloc(const char *s);

bool  StringReplaceToBuffer(char *buffer, size_t buffer_size, const char *s, const char *search, const char *replace);
char* StringReplaceAlloc(const char *s, const char *search, const char *replace);

bool  StringRepeatToBuffer(char *buffer, size_t buffer_size, const char *s, uint64_t n);
char* StringRepeatAlloc(const char *s, int n);

bool StringContainsChar(const char *s, char search);
bool StringContainsString(const char *s, const char *search);
bool StringContainsAny(const char *s, const char *chars);
bool StringIn(const char *s, const char *haystack, const char *separator);
bool StringEquals(const char *s1, const char *s2);
bool StringStartsWith(const char *s1, const char *s2);
bool StringEndsWith(const char *s1, const char *s2);

int64_t StringFirstIndexOf(const char *s, const char *search);
int64_t StringLastIndexOf(const char *s, const char *search);

char* StringDuplicate(const char *s);

bool  StringsJoinToBuffer(char *buffer, size_t buffer_size, char **strings, uint64_t number_of_strings, char *separator);
char* StringsJoinAlloc(char **strings, uint64_t number_of_strings, char *separator);

#pragma endregion
#pragma region String Builder

typedef struct StringBuilder {
  bool is_dynamic;
  char *string;
  size_t capacity;
  size_t length;
} StringBuilder;

StringBuilder CreateStaticStringBuilder(char *buffer, size_t buffer_size);
StringBuilder CreateDynamicStringBuilder(size_t initial_capacity);
bool AllocateStringBuilder(StringBuilder *sb, size_t initial_capacity);
bool DeallocateStringBuilder(StringBuilder *sb);

bool StringBuilderCapacityRealloc(StringBuilder *sb, size_t padding);
bool StringBuilderAddChar(StringBuilder *sb, char c);
bool StringBuilderAddString(StringBuilder *sb, const char *s);
bool StringBuilderReadFile(StringBuilder *sb, const char *file_path);
bool StringBuilderClear(StringBuilder *sb);

// TODO: StringBuilderPrintf

#pragma endregion
#pragma region File System

bool FileExists(const char *file_path);
bool PathIsFile(const char *path);
bool PathIsDir(const char *path);
bool CreateNewFile(const char *file_path);
bool RemoveFile(const char *file_path);

char* PathBaseName(const char *file_path);
bool  PathBaseNameToBuffer(char *buffer, size_t buffer_size, const char *file_path);

bool  PathDirNameToBuffer(char *buffer, size_t buffer_size, const char *file_path);
char* PathDirNameAlloc(const char *file_path);

char* PathExt(const char *file_path);
bool  PathExtToBuffer(char *buffer, size_t buffer_size, const char *file_path);
char* PathExtAlloc(const char *file_path);

#pragma endregion
#pragma region IO

// Depends on stdio.h
#define DebugLine(fmt, ...)\
  fprintf(stderr, "[%s:%d %s] " fmt "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)

#define PrintLine(fmt, ...)\
  fprintf(stdout, fmt "\n", ##__VA_ARGS__)

bool  ReadFileToBuffer(char *buffer, size_t buffer_size, const char *file_path);
char* ReadFileAlloc(const char *file_path);

bool  ReadUserInputToBuffer(char *buffer, size_t buffer_size);
bool  ReadUserInputToStringBuilder(StringBuilder *sb);
char* ReadUserInputAlloc(void);

bool WriteToFile(const char *file_path, const char *s);
bool AppendToFile(const char *file_path, const char *s);

void WriteToStdOut(const char *s);
void WriteToStdErr(const char *s);

// TODO: WriteFormatToFile, AppendFormatToFile

#pragma endregion
#pragma region Util

#define ArraySize(array) (sizeof(array) / sizeof(*(array)))
#define BoolToString(b) ((b) ? "TRUE" : "FALSE")

typedef struct Allocation {
  void *memory;
  size_t size;
} Allocation;

bool CreateAllocation(Allocation *a, size_t size);
bool ResizeAllocation(Allocation *a, size_t size);
void FreeAllocation(Allocation *a);

typedef struct MemoryAllocation {
  void *memory;
  size_t size_of_item;
  uint64_t number_of_items;
} MemoryAllocation;

bool CreateMemoryAllocation(MemoryAllocation *ma, size_t size_of_item, uint64_t number_of_items);
bool ResizeMemoryAllocation(MemoryAllocation *ma, uint64_t number_of_items);
void FreeMemoryAllocation(MemoryAllocation *ma);

uint64_t Hash(const char *s);

typedef struct Url {
  char scheme[16];
  char host[64];
  char port[8];
  char path[64];
  char *query;
} Url;

Url UrlParse(const char *s);

#pragma endregion
#pragma region Bit Operations

#define BitSize(x) (sizeof(x) * 8)

#define BitSetLeft(x, n) (x) | (1 << (sizeof(x) * 8 - (n)))
#define BitSetRight(x, n) (x) | (1 << ((n) - 1))

#define BitClearLeft(x, n) (x) & ~(1 << (sizeof(x) * 8 - (n)))
#define BitClearRight(x, n) (x) & ~(1 << ((n) - 1))

#define BitToggleLeft(x, n) (x) ^ (1 << (sizeof(x) * 8 - (n)))
#define BitToggleRight(x, n) (x) ^ (1 << ((n) - 1))

#define BitGetLeft(x, n) ((x) >> (sizeof(x) * 8 - n)) & 1
#define BitGetRight(x, n) (x) >> (n) - 1 & 1

/**
 * Print the bits of a number.
 * The number of bits will equal to the size of the data type.
 * @param x The number to convert to bits.
*/
#define BitPrint(x)\
do {\
  uint64_t i = 1ULL << (sizeof(x) * 8 - 1);\
  do putchar((((x) & i) > 0) + 48); while (i >>= 1);\
  printf("\n");\
} while (0)

#pragma endregion
#pragma region Conversions

int CharToInt(char c);
int StringToInt32(const char *s);
int64_t StringToInt64(const char *s);

bool  CharToStringToBuffer(char *buffer, size_t buffer_size, char c);
char* CharToStringAlloc(char c);

bool Int32ToStringToBuffer(char *buffer, size_t buffer_size, int x);
bool Int64ToStringToBuffer(char *buffer, size_t buffer_size, int64_t x);
bool Uint64ToStringToBuffer(char *buffer, size_t buffer_size, uint64_t x);

#pragma endregion
#pragma region Hash map

typedef struct StringHashMapValue {
  char *key, *value;
  struct StringHashMapValue *next;
} StringHashMapValue;

StringHashMapValue* NewStringHashMapValue(char *key, char *value);

typedef struct StringHashMap {
  size_t capacity;
  size_t length;
  StringHashMapValue **items;
} StringHashMap;

bool AllocateStringHashMap(StringHashMap *hm, size_t capacity);
bool DeallocateStringHashMap(StringHashMap *hm);
StringHashMap CreateStringHashMap(size_t capacity);

void  StringHashMapSet(StringHashMap *hm, char *key, char *value);
char* StringHashMapGet(StringHashMap *hm, char *key);
bool  StringHashMapRemove(StringHashMap *hm, char *key);

#pragma endregion

#endif
