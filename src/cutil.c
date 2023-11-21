#include "../include/cutil.h"

// Function naming rules:
//
// Structs/Objects:
// CreateObject   -> Returns an object
// NewObject      -> Returns a pointer to an allocated object
// AllocateObject -> Returns a boolean, if allocation for object was successful
//
// Functions:
// NamespaceFunction         -> Pure function
// NamespaceFunctionToBuffer -> Writes result to buffer, returns boolean
// NamespaceFunctionAlloc    -> Returns new heap allocated memory as the result

/*

# References

- Fast int to string: http://www.strudel.org.uk/itoa/
- djb2 hash: http://www.cse.yorku.ca/~oz/hash.html

*/

#pragma region Includes

#ifndef _STDARG_H
#include <stdarg.h>
#endif

#ifndef _INC_STDIO
#include <stdio.h>
#endif

#ifndef _INC_STDLIB
#include <stdlib.h>
#endif

#ifndef _INC_STRING
#include <string.h>
#endif

#ifndef _INC_STAT
#include <sys/stat.h>
#endif

#pragma endregion
#pragma region Definitions

#define FREAD_BUFFER_SIZE 4096

#pragma endregion
#pragma region Char and String

inline bool CharIsAlpha(char c) {
  return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

inline bool CharIsDigit(char c) {
  return '0' <= c && c <= '9';
}

inline bool CharIsUpper(char c) {
  return 'A' <= c && c <= 'Z';
}

inline bool CharIsLower(char c) {
  return 'a' <= c && c <= 'z';
}

inline bool CharIn(char c, const char *haystack) {
  while (*haystack != '\0') {
    if (c == *haystack++) {
      return true;
    }
  }
  return false;
}

inline char CharUpper(char c) {
  return CharIsLower(c) ? c - 32 : c;
}

inline char CharLower(char c) {
  return CharIsUpper(c) ? c + 32 : c;
}

inline size_t StringLength(const char *s) {
  size_t length = 0;
  while (*s++ != '\0') {
    length += 1;
  }
  return length;
}

inline bool StringIsAlpha(const char *s) {
  while (*s != '\0') {
    if (!CharIsAlpha(*s)) {
      return false;
    }
    s++;
  }
  return true;
}

inline bool StringIsNumeric(const char *s) {
  while (*s != '\0') {
    if (!CharIsDigit(*s)) {
      return false;
    }
    s++;
  }
  return true;
}

inline bool StringIsAlphaNumeric(const char *s) {
  while (*s != '\0') {
    if (!CharIsAlpha(*s) && !CharIsDigit(*s)) {
      return false;
    }
    s++;
  }
  return true;
}

inline bool StringUpperToBuffer(char *buffer, size_t buffer_size, const char *s) {
  while (*s != '\0') {
    if (--buffer_size == 0) {
      return false;
    }
    *buffer++ = *s - ('a' <= *s && *s <= 'z') * 32;
    s++;
  }
  return true;
}

inline char* StringUpperAlloc(const char *s) {
  StringBuilder sb = CreateDynamicStringBuilder(StringLength(s) + 1);
  while (*s != '\0') {
    StringBuilderAddChar(&sb, *s - ('a' <= *s && *s <= 'z') * 32);
    s++;
  }
  return sb.string;
}

inline bool StringLowerToBuffer(char *buffer, size_t buffer_size, const char *s) {
  while (*s != '\0') {
    if (--buffer_size == 0) {
      return false;
    }
    *buffer++ = *s + ('A' <= *s && *s <= 'Z') * 32;
    s++;
  }
  return true;
}

inline char* StringLowerAlloc(const char *s) {
  StringBuilder sb = CreateDynamicStringBuilder(StringLength(s) + 1);
  while (*s != '\0') {
    StringBuilderAddChar(&sb, *s + ('A' <= *s && *s <= 'Z') * 32);
    s++;
  }
  return sb.string;
}

inline bool StringTitleToBuffer(char *buffer, size_t buffer_size, const char *s) {
  bool title = true;
  while (*s != '\0') {
    if (--buffer_size == 0) {
      return false;
    }
    char c = *s;
    if (!CharIsAlpha(c)) {
      title = true;
    }
    else if (CharIsLower(c)) {
      if (title) {
        c = CharUpper(c);
        title = false;
      }
    }
    *buffer++ = c;
    s++;
  }
  return true;
}

inline void StringTitleInPlace(char *s) {
  bool title = true;
  while (*s != '\0') {
    char c = *s;
    if (!CharIsAlpha(c)) {
      title = true;
    }
    else if (CharIsLower(c)) {
      if (title) {
        c = CharUpper(c);
        title = false;
      }
    }
    *s = c;
    s++;
  }
}

inline char* StringTitleAlloc(const char *s) {
  StringBuilder sb = CreateDynamicStringBuilder(StringLength(s) + 1);
  bool title = true;
  while (*s != '\0') {
    char c = *s;
    if (!CharIsAlpha(c)) {
      title = true;
    }
    else if (CharIsLower(c)) {
      if (title) {
        c = CharUpper(c);
        title = false;
      }
    }
    StringBuilderAddChar(&sb, c);
    s++;
  }
  return sb.string;
}

inline bool StringConcatToBuffer(char *buffer, size_t buffer_size, const char *s1, const char *s2) {
  while (*s1 != '\0') {
    if (--buffer_size == 0) {
      return false;
    }
    *buffer++ = *s1++;
  }
  while (*s2 != '\0') {
    if (--buffer_size == 0) {
      return false;
    }
    *buffer++ = *s2++;
  }
  return true;
}

inline char* StringConcatAlloc(const char *s1, const char *s2) {
  StringBuilder sb = CreateDynamicStringBuilder(StringLength(s1) + StringLength(s2) + 1);
  while (*s1 != '\0') {
    StringBuilderAddChar(&sb, *s1++);
  }
  while (*s2 != '\0') {
    StringBuilderAddChar(&sb, *s2++);
  }
  return sb.string;
}

inline bool StringSliceToBuffer(char *buffer, size_t buffer_size, const char *s, int64_t start, int64_t end) {
  if (start < end) {
    while (start < end && s[start] != '\0') {
      if (--buffer_size == 0) {
        return false;
      }
      *buffer++ = s[start++];
    }
  }
  else if (start > end) {
    while (start > end && start >= 0) {
      if (--buffer_size == 0) {
        return false;
      }
      *buffer++ = s[start--];
    }
  }
  return true;
}

inline char* StringSliceAlloc(const char *s, int64_t start, int64_t end) {
  StringBuilder sb = CreateDynamicStringBuilder(0);
  if (start < end) {
    while (start < end && s[start] != '\0') {
      StringBuilderAddChar(&sb, s[start++]);
    }
  }
  else if (start > end) {
    while (start > end && start >= 0) {
      StringBuilderAddChar(&sb, s[start--]);
    }
  }
  return sb.string;
}

inline bool StringFirstNCharsToBuffer(char *buffer, size_t buffer_size, const char *s, uint64_t n) {
  while (n-- && *s != '\0') {
    if (--buffer_size == 0) {
      return false;
    }
    *buffer++ = *s++;
  }
  return true;
}

inline char* StringFirstNCharsAlloc(const char *s, uint64_t n) {
  StringBuilder sb = CreateDynamicStringBuilder(n + 1);
  while (n-- && *s != '\0') {
    StringBuilderAddChar(&sb, *s++);
  }
  return sb.string;
}

inline bool StringLastNCharsToBuffer(char *buffer, size_t buffer_size, const char *s, uint64_t n) {
  return StringFirstNCharsToBuffer(buffer, buffer_size,s + StringLength(s) - n, n);
}

inline char* StringLastNCharsAlloc(const char *s, uint64_t n) {
  return StringFirstNCharsAlloc(s + StringLength(s) - n, n);
}

inline bool StringReverseToBuffer(char *buffer, size_t buffer_size, const char *s) {
  size_t i = StringLength(s);
  if (i > 0) {
    do {
      if (--buffer_size == 0) {
        return false;
      }
      *buffer++ = s[--i];
    } while (i > 0);
  }
  return true;
}

inline char* StringReverseAlloc(const char *s) {
  size_t i = StringLength(s);
  StringBuilder sb = {0};
  if (i > 0) {
    sb = CreateDynamicStringBuilder(i + 1);
    do {
      StringBuilderAddChar(&sb, s[--i]);
    } while (i > 0);
  }
  return sb.string;
}

inline bool StringReplaceToBuffer(char *buffer, size_t buffer_size, const char *s, const char *search, const char *replace) {
  size_t search_length = StringLength(search);
  size_t replace_length = StringLength(replace);
  while (*s != '\0') {
    if (StringStartsWith(s, search)) {
      if (buffer_size < replace_length || (buffer_size -= replace_length) == 0) {
        return false;
      }
      for (size_t i = 0; i < replace_length; i++) {
        *buffer++ = replace[i];
      }
      s += search_length;
    } else {
      if (--buffer_size == 0) {
        return false;
      }
      *buffer++ = *s++;
    }
  }
  return true;
}

inline char* StringReplaceAlloc(const char *s, const char *search, const char *replace) {
  size_t search_length = StringLength(search);
  StringBuilder sb = CreateDynamicStringBuilder(0);
  while (*s != '\0') {
    if (StringStartsWith(s, search)) {
      StringBuilderAddString(&sb, replace);
      s += search_length;
    } else {
      StringBuilderAddChar(&sb, *s++);
    }
  }
  return sb.string;
}

inline bool StringRepeatToBuffer(char *buffer, size_t buffer_size, const char *s, uint64_t n) {
  size_t length = StringLength(s);
  if (length * n > buffer_size - 1) {
    return false;
  }
  while (n--) {
    for (size_t i = 0; i < StringLength(s); i++) {
      *buffer++ = s[i];
    }
  }
  return true;
}

inline char* StringRepeatAlloc(const char *s, int n) {
  StringBuilder sb = CreateDynamicStringBuilder(1);
  while (n--) {
    StringBuilderAddString(&sb, s);
  }
  return sb.string;
}

inline bool StringContainsChar(const char *s, char search) {
  while (*s != '\0') {
    if (*s++ == search) {
      return true;
    }
  }
  return false;
}

inline bool StringContainsString(const char *s, const char *search) {
  if (StringLength(s) >= StringLength(search)) {
    while (*s != '\0') {
      for (size_t i = 0; search[i] != '\0'; i++) {
        if (*(s + i) != search[i]) {
          goto ContinueWhile;
        }
      }
      return true;
ContinueWhile:
      s++;
    }
  }
  return false;
}

inline bool StringContainsAny(const char *s, const char *chars) {
  while (*s != '\0') {
    for (size_t i = 0; chars[i] != '\0'; i++) {
      if (*s == chars[i]) {
        return true;
      }
    }
    s++;
  }
  return false;
}

inline bool StringIn(const char *s, const char *haystack, const char *separator) {
  size_t i = 0, j = 0;
  size_t separator_length = StringLength(separator);
  do {
    if (StringStartsWith(haystack + j, separator) || haystack[j] == '\0') {
      for (size_t k = i, h = 0; k < j && s[h] != '\0'; k++, h++) {
        if (haystack[k] != s[h]) {
          goto ContinueDoWhile;
        }
      }
      return true;
ContinueDoWhile:
      i = j + separator_length;
    }
  } while (haystack[j++] != '\0');
  return false;
}

inline bool StringEquals(const char *s1, const char *s2) {
  do {
    if (*s1 != *s2) {
      return false;
    }
  } while (*s1++ != '\0' && *s2++ != '\0');
  return true;
}

inline bool StringStartsWith(const char *s1, const char *s2) {
  size_t s2_length = StringLength(s2);
  if (StringLength(s1) < s2_length) {
    return false;
  }
  for (size_t i = 0; i < s2_length; i++) {
    if (s1[i] != s2[i]) {
      return false;
    }
  }
  return true;
}

inline bool StringEndsWith(const char *s1, const char *s2) {
  size_t s1_length = StringLength(s1);
  size_t s2_length = StringLength(s2);
  if (s1_length < s2_length) {
    return false;
  }
  return StringEquals(s1 + (s1_length - s2_length), s2);
}

inline int64_t StringFirstIndexOf(const char *s, const char *search) {
  for (int64_t i = 0; s[i] != '\0'; i++) {
    if (StringStartsWith(s + i, search)) {
      return i;
    }
  }
  return -1;
}

inline int64_t StringLastIndexOf(const char *s, const char *search) {
  for (int64_t i = StringLength(s) - 1; i >= 0; i--) {
    if (StringStartsWith(s + i, search)) {
      return i;
    }
  }
  return -1;
}

inline char* StringDuplicate(const char *s) {
  return strdup(s);
}

inline bool StringsJoinToBuffer(char *buffer, size_t buffer_size, char **strings, uint64_t number_of_strings, char *separator) {
  size_t separator_length = StringLength(separator);
  for (size_t i = 0; i < number_of_strings; i++) {
    size_t string_length = StringLength(strings[i]);
    if (string_length < buffer_size) {
      strcat(buffer, strings[i]);
      buffer_size -= string_length;
    } else {
      return false;
    }
    if (i + 1 < number_of_strings) {
      if (separator_length < buffer_size) {
        strcat(buffer, separator);
        buffer_size -= separator_length;
      } else {
        return false;
      }
    }
  }
  return true;
}

inline char* StringsJoinAlloc(char **strings, uint64_t number_of_strings, char *separator) {
  StringBuilder sb = CreateDynamicStringBuilder(1);
  for (size_t i = 0; i < number_of_strings; i++) {
    StringBuilderAddString(&sb, strings[i]);
    if (i + 1 < number_of_strings) {
      StringBuilderAddString(&sb, separator);
    }
  }
  return sb.string;
}

void Sprintf(char *buffer, size_t buffer_size, const char *format, ...) {
  va_list valist;
  va_start(valist, format);
  while (*format != '\0') {
    if (*format != '%') {
      *buffer++ = *format++;
      if (--buffer_size - 1 == 0) {
        goto End;
      }
      continue;
    }
    switch (*(++format)) {
      case 'd':
      case 'i': {
        char s[20] = {0};
        size_t i = 0;
        Int64ToStringToBuffer(s, ArraySize(s), va_arg(valist, int64_t), 10);
        while (s[i] != '\0') {
          *buffer++ = s[i++];
          if (--buffer_size - 1 == 0) {
            goto End;
          }
        }
        break;
      }
      case 'u': {
        char s[21] = {0};
        size_t i = 0;
        Uint64ToStringToBuffer(s, ArraySize(s), va_arg(valist, uint64_t), 10);
        while (s[i] != '\0') {
          *buffer++ = s[i++];
          if (--buffer_size - 1 == 0) {
            goto End;
          }
        }
        break;
      }
      case 'c':
        *buffer++ = (char)va_arg(valist, int);
        if (--buffer_size - 1 == 0) {
          goto End;
        }
        break;
      case 's': {
        char *s = va_arg(valist, char*);
        while (*s != '\0') {
          *buffer++ = *s;
          if (--buffer_size - 1 == 0) {
            goto End;
          }
          s++;
        }
        break;
      }
      case 'b': {
        char *s = va_arg(valist, int) ? "true" : "false";
        while (*s != '\0') {
          *buffer++ = *s;
          if (--buffer_size - 1 == 0) {
            goto End;
          }
          s++;
        }
        break;
      }
      case 'B': {
        char *s = va_arg(valist, int) ? "TRUE" : "FALSE";
        while (*s != '\0') {
          *buffer++ = *s;
          if (--buffer_size - 1 == 0) {
            goto End;
          }
          s++;
        }
        break;
      }
      default:
        *buffer++ = '%';
        if (--buffer_size - 1 == 0) {
          goto End;
        }
        *buffer++ = *format;
        if (--buffer_size - 1 == 0) {
          goto End;
        }
        break;
    }
    format++;
  }
End:
  va_end(valist);
}

void Sappendf(char *buffer, size_t buffer_size, const char *format, ...) {
  while (*buffer != '\0') {
    buffer++;
    if (--buffer_size == 0) {
      goto End;
    }
  }
  va_list valist;
  va_start(valist, format);
  while (*format != '\0') {
    if (*format != '%') {
      *buffer++ = *format++;
      if (--buffer_size == 0) {
        goto End;
      }
      continue;
    }
    switch (*(++format)) {
      case 'd':
      case 'i': {
        char s[20] = {0};
        size_t i = 0;
        Int64ToStringToBuffer(s, ArraySize(s), va_arg(valist, int64_t), 10);
        while (s[i] != '\0') {
          *buffer++ = s[i++];
          if (--buffer_size == 0) {
            goto End;
          }
        }
        break;
      }
      case 'u': {
        char s[21] = {0};
        size_t i = 0;
        Uint64ToStringToBuffer(s, ArraySize(s), va_arg(valist, uint64_t), 10);
        while (s[i] != '\0') {
          *buffer++ = s[i++];
          if (--buffer_size == 0) {
            goto End;
          }
        }
        break;
      }
      case 'c':
        *buffer++ = (char)va_arg(valist, int);
        if (--buffer_size == 0) {
          goto End;
        }
        break;
      case 's': {
        char *s = va_arg(valist, char*);
        while (*s != '\0') {
          *buffer++ = *s;
          if (--buffer_size == 0) {
            goto End;
          }
          s++;
        }
        break;
      }
      case 'b': {
        char *s = va_arg(valist, int) ? "true" : "false";
        while (*s != '\0') {
          *buffer++ = *s;
          if (--buffer_size == 0) {
            goto End;
          }
          s++;
        }
        break;
      }
      case 'B': {
        char *s = va_arg(valist, int) ? "TRUE" : "FALSE";
        while (*s != '\0') {
          *buffer++ = *s;
          if (--buffer_size == 0) {
            goto End;
          }
          s++;
        }
        break;
      }
      default:
        *buffer++ = '%';
        if (--buffer_size == 0) {
          goto End;
        }
        *buffer++ = *format;
        if (--buffer_size == 0) {
          goto End;
        }
        break;
    }
    format++;
  }
End:
  va_end(valist);
}

#pragma endregion
#pragma region String Builder

inline StringBuilder CreateStaticStringBuilder(char *buffer, size_t buffer_size) {
  return (StringBuilder) {
    .is_dynamic = false,
    .capacity = buffer_size,
    .length = 0,
    .string = buffer,
  };
}

inline StringBuilder CreateDynamicStringBuilder(size_t initial_capacity) {
  if (initial_capacity == 0) {
    initial_capacity = 1;
  }
  return (StringBuilder) {
    .is_dynamic = true,
    .capacity = initial_capacity,
    .length = 0,
    .string = calloc(initial_capacity, 1),
  };
}

inline bool AllocateStringBuilder(StringBuilder *sb, size_t initial_capacity) {
  if (DeallocateStringBuilder(sb)) {
    if (initial_capacity == 0) {
      initial_capacity = 1;
    }
    sb->is_dynamic = true,
    sb->capacity = initial_capacity;
    sb->length = 0;
    sb->string = calloc(initial_capacity, sizeof(char));
    return sb->string != NULL;
  }
  return false;
}

inline bool DeallocateStringBuilder(StringBuilder *sb) {
  if (sb == NULL) {
    return false;
  }
  StringBuilderClear(sb);
  if (sb->is_dynamic) {
    free(sb->string);
  }
  return true;
}

inline bool StringBuilderCapacityRealloc(StringBuilder *sb, size_t padding) {
  if (sb == NULL) {
    return false;
  }
  while (sb->length + padding >= sb->capacity) {
    sb->capacity <<= 1;
  }
  sb->string = realloc(sb->string, sizeof(char) * sb->capacity);
  if (sb->string == NULL) {
    return false;
  }
  memset(sb->string + sb->length, '\0', sb->capacity - sb->length);
  return true;
}

inline bool StringBuilderAddChar(StringBuilder *sb, char c) {
  if (sb == NULL) {
    return false;
  }
  if (sb->length + 1 >= sb->capacity) {
    if (!sb->is_dynamic) {
      return false;
    }
    if (!StringBuilderCapacityRealloc(sb, 1)) {
      return false;
    }
  }
  sb->string[sb->length++] = c;
  return true;
}

inline bool StringBuilderAddString(StringBuilder *sb, const char *s) {
  if (sb == NULL) {
    return false;
  }
  size_t padding = StringLength(s);
  if (sb->length + padding >= sb->capacity) {
    if (!sb->is_dynamic) {
      return false;
    }
    if (!StringBuilderCapacityRealloc(sb, padding)) {
      return false;
    }
  }
  while (*s != '\0') {
    sb->string[sb->length++] = *s++;
  }
  return true;
}

inline bool StringBuilderReadFile(StringBuilder *sb, const char *file_path) {
  FILE *f = fopen(file_path, "rb");
  if (f == NULL) {
    return false;
  }
  uint64_t n;
  do {
    char fread_buffer[FREAD_BUFFER_SIZE] = {0};
    n = fread(fread_buffer, sizeof(char), FREAD_BUFFER_SIZE, f);
    for (size_t i = 0; i < n; i++) {
      StringBuilderAddChar(sb, fread_buffer[i]);
    }
  } while (n == FREAD_BUFFER_SIZE);
  return fclose(f) == 0;
}

inline bool StringBuilderClear(StringBuilder *sb) {
  if (sb != NULL && sb->length > 0) {
    do {
      sb->string[sb->length - 1] = '\0';
    } while (--sb->length > 0);
    sb->length = 0;
    return true;
  }
  return false;
}

inline bool StringBuilderPrintf(StringBuilder *sb, const char *format, ...) {
  if (sb == NULL) {
    return false;
  }
  va_list valist;
  va_start(valist, format);
  while (*format != '\0') {
    if (*format != '%') {
      if (!StringBuilderAddChar(sb, *format++)) {
        return false;
      }
      continue;
    }
    switch (*(++format)) {
      case 'd':
      case 'i': {
        char buffer[20] = {0};
        Int64ToStringToBuffer(buffer, ArraySize(buffer), va_arg(valist, int64_t), 10);
        if (!StringBuilderAddString(sb, buffer)) {
          return false;
        }
        break;
      }
      case 'u': {
        char buffer[21] = {0};
        Uint64ToStringToBuffer(buffer, ArraySize(buffer), va_arg(valist, uint64_t), 10);
        if (!StringBuilderAddString(sb, buffer)) {
          return false;
        }
        break;
      }
      case 'c':
        if (!StringBuilderAddChar(sb, (char)va_arg(valist, int))) {
          return false;
        }
        break;
      case 's':
        if (!StringBuilderAddString(sb, va_arg(valist, char*))) {
          return false;
        }
        break;
      case 'b':
        if (!StringBuilderAddString(sb, va_arg(valist, int) ? "true" : "false")) {
          return false;
        }
        break;
      case 'B':
        if (!StringBuilderAddString(sb, va_arg(valist, int) ? "TRUE" : "FALSE")) {
          return false;
        }
        break;
      default:
        if (!StringBuilderAddChar(sb, '%')) {
          return false;
        }
        if (!StringBuilderAddChar(sb, *format)) {
          return false;
        }
        break;
    }
    format++;
  }
  va_end(valist);
  return true;
}

#pragma endregion
#pragma region File System

inline bool FileExists(const char *file_path) {
  return access(file_path, F_OK) == 0;
}

inline bool PathIsFile(const char *path) {
  struct stat s;
  if (stat(path, &s) == 0) {
    return S_ISREG(s.st_mode);
  }
  return false;
}

inline bool PathIsDir(const char *path) {
  struct stat s;
  if (stat(path, &s) == 0) {
    return S_ISDIR(s.st_mode);
  }
  return false;
}

inline bool CreateNewFile(const char *file_path) {
  FILE *f = fopen(file_path, "wb");
  if (f == NULL) {
    return false;
  }
  return fclose(f) == 0;
}

inline bool RemoveFile(const char *file_path) {
  return remove(file_path) == 0;
}

inline char* PathBaseName(const char *file_path) {
  int64_t index = StringLastIndexOf(file_path, "/");
  if (index == -1) {
    return (char*)file_path;
  } else {
    return (char*)(file_path + index + 1);
  }
}

inline bool PathBaseNameToBuffer(char *buffer, size_t buffer_size, const char *file_path) {
  char *base_name = PathBaseName(file_path);
  if (StringLength(base_name) < buffer_size) {
    strcpy(buffer, base_name);
    return true;
  }
  return false;
}

inline bool PathDirNameToBuffer(char *buffer, size_t buffer_size, const char *file_path) {
  int64_t index = StringLastIndexOf(file_path, "/");
  if (index == -1) {
    size_t length = StringLength(file_path);
    if (length < buffer_size) {
      strcpy(buffer, file_path);
      return true;
    }
    return false;
  }
  return StringSliceToBuffer(buffer, buffer_size, file_path, 0, index);
}

inline char* PathDirNameAlloc(const char *file_path) {
  int64_t index = StringLastIndexOf(file_path, "/");
  if (index == -1) {
    return strdup(file_path);
  }
  return StringSliceAlloc(file_path, 0, index);
}

inline char* PathExt(const char *file_path) {
  int64_t index = StringLastIndexOf(file_path, "/");
  if (index == -1) {
    index = 0;
  }
  int64_t dot_index = StringLastIndexOf(file_path + index, ".");
  if (dot_index == -1) {
    return NULL;
  }
  return (char*)(file_path + index + dot_index);
}

inline bool PathExtToBuffer(char *buffer, size_t buffer_size, const char *file_path) {
  int64_t index = StringLastIndexOf(file_path, "/");
  if (index == -1) {
    index = 0;
  }
  int64_t dot_index = StringLastIndexOf(file_path + index, ".");
  if (dot_index == -1) {
    return false;
  }
  return StringSliceToBuffer(buffer, buffer_size, file_path, index + dot_index, StringLength(file_path));
}

inline char* PathExtAlloc(const char *file_path) {
  int64_t index = StringLastIndexOf(file_path, "/");
  if (index == -1) {
    index = 0;
  }
  int64_t dot_index = StringLastIndexOf(file_path + index, ".");
  if (dot_index == -1) {
    return NULL;
  }
  return StringSliceAlloc(file_path, index + dot_index, StringLength(file_path));
}

#pragma endregion
#pragma region IO

inline bool ReadFileToBuffer(char *buffer, size_t buffer_size, const char *file_path) {
  FILE *f = fopen(file_path, "rb");
  if (f == NULL) {
    return false;
  }
  size_t buffer_index = 0, n;
  do {
    char fread_buffer[FREAD_BUFFER_SIZE] = {0};
    n = fread(fread_buffer, sizeof(char), FREAD_BUFFER_SIZE, f);
    for (size_t i = 0; i < n && buffer_index < buffer_size - 1; i++) {
      buffer[buffer_index++] = fread_buffer[i];
    }
  } while (n == FREAD_BUFFER_SIZE);
  return fclose(f) == 0;
}

inline char* ReadFileAlloc(const char *file_path) {
  StringBuilder sb = CreateDynamicStringBuilder(0);
  if (StringBuilderReadFile(&sb, file_path)) {
    return sb.string;
  }
  return NULL;
}

inline bool ReadUserInputToBuffer(char *buffer, size_t buffer_size) {
  return fgets(buffer, buffer_size - 1, stdin) != NULL;
}

inline bool ReadUserInputToStringBuilder(StringBuilder *sb) {
  char c;
  while ((c = fgetc(stdin), c != EOF && c != '\r' && c != '\n')) {
    if (!StringBuilderAddChar(sb, c)) {
      return false;
    }
  }
  return true;
}

inline char* ReadUserInputAlloc(void) {
  StringBuilder sb = CreateDynamicStringBuilder(0);
  if (ReadUserInputToStringBuilder(&sb)) {
    return sb.string;
  }
  return NULL;
}

inline bool WriteToFile(const char *file_path, const char *s) {
  FILE *f = fopen(file_path, "wb");
  if (f == NULL) {
    return false;
  }
  size_t length = StringLength(s);
  uint64_t n = fwrite(s, sizeof(char), length, f);
  return n == length && fclose(f) == 0;
}

inline bool AppendToFile(const char *file_path, const char *s) {
  FILE *f = fopen(file_path, "ab");
  if (f == NULL) {
    return false;
  }
  size_t length = StringLength(s);
  uint64_t n = fwrite(s, sizeof(char), length, f);
  return n == length && fclose(f) == 0;
}

void WriteStringToFile(FILE *file, const char *s) {
  while (*s != '\0') {
    putc(*s++, file);
  }
}

inline void WriteStringToStdOut(const char *s) {
  while (*s != '\0') {
    putc(*s++, stdout);
  }
}

inline void WriteStringToStdErr(const char *s) {
  while (*s != '\0') {
    putc(*s++, stderr);
  }
}

void WriteInt64ToFile(FILE *file, int64_t value) {
  char buffer[20] = {0};
	char *ptr = buffer;
	while (true) {
		int64_t tmp = value;
		value /= 10;
		*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + (tmp - value * 10)];
		if (value == 0) {
			if (tmp < 0) {
				*ptr++ = '-';
			}
			*ptr-- = '\0';
			break;
		}
	}
	while(ptr >= buffer) {
		putc(*ptr--, file);
	}
}

void WriteUint64ToFile(FILE *file, uint64_t value) {
  char buffer[21] = {0};
	char *ptr = buffer;
	while (true) {
		uint64_t tmp = value;
		value /= 10;
		*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + (tmp - value * 10)];
		if (value == 0) {
			*ptr-- = '\0';
			break;
		}
	}
	while(ptr >= buffer) {
		putc(*ptr--, file);
	}
}

inline void WriteInt64ToStdOut(int64_t value) {
	WriteInt64ToFile(stdout, value);
}

inline void WriteInt64ToStdErr(int64_t value) {
	WriteInt64ToFile(stderr, value);
}

inline void WriteUint64ToStdOut(uint64_t value) {
	WriteUint64ToFile(stdout, value);
}

inline void WriteUint64ToStdErr(uint64_t value) {
	WriteUint64ToFile(stderr, value);
}

void Printf(const char *format, ...) {
  va_list valist;
  va_start(valist, format);
  while (*format != '\0') {
    if (*format != '%') {
      putchar(*format++);
      continue;
    }
    switch (*(++format)) {
      case 'd':
      case 'i':
        WriteInt64ToStdOut(va_arg(valist, int64_t));
        break;
      case 'u':
        WriteUint64ToStdOut(va_arg(valist, uint64_t));
        break;
      case 'c':
        putchar((char)va_arg(valist, int));
        break;
      case 's':
        WriteStringToStdOut(va_arg(valist, char*));
        break;
      case 'b':
        WriteStringToStdOut(va_arg(valist, int) ? "true" : "false");
        break;
      case 'B':
        WriteStringToStdOut(va_arg(valist, int) ? "TRUE" : "FALSE");
        break;
      default:
        putchar('%');
        putchar(*format);
        break;
    }
    format++;
  }
  va_end(valist);
}

void Errorf(const char *format, ...) {
  va_list valist;
  va_start(valist, format);
  while (*format != '\0') {
    if (*format != '%') {
      putc(*format++, stderr);
      continue;
    }
    switch (*(++format)) {
      case 'd':
      case 'i':
        WriteInt64ToStdErr(va_arg(valist, int64_t));
        break;
      case 'u':
        WriteUint64ToStdErr(va_arg(valist, uint64_t));
        break;
      case 'c':
        putc((char)va_arg(valist, int), stderr);
        break;
      case 's':
        WriteStringToStdErr(va_arg(valist, char*));
        break;
      case 'b':
        WriteStringToStdErr(va_arg(valist, int) ? "true" : "false");
        break;
      case 'B':
        WriteStringToStdErr(va_arg(valist, int) ? "TRUE" : "FALSE");
        break;
      default:
        putc('%', stderr);
        putc(*format, stderr);
        break;
    }
    format++;
  }
  va_end(valist);
}

bool WriteFormatToFile(const char *file_path, const char *format, ...) {
  FILE *f = fopen(file_path, "wb");
  if (f == NULL) {
    return false;
  }
  va_list valist;
  va_start(valist, format);
  while (*format != '\0') {
    if (*format != '%') {
      putc(*format++, f);
      continue;
    }
    switch (*(++format)) {
      case 'd':
      case 'i':
        WriteInt64ToFile(f, va_arg(valist, int64_t));
        break;
      case 'u':
        WriteUint64ToFile(f, va_arg(valist, uint64_t));
        break;
      case 'c':
        putc((char)va_arg(valist, int), f);
        break;
      case 's':
        WriteStringToFile(f, va_arg(valist, char*));
        break;
      case 'b':
        WriteStringToFile(f, va_arg(valist, int) ? "true" : "false");
        break;
      case 'B':
        WriteStringToFile(f, va_arg(valist, int) ? "TRUE" : "FALSE");
        break;
      default:
        putc('%', f);
        putc(*format, f);
        break;
    }
    format++;
  }
  va_end(valist);
  return fclose(f) == 0;
}

bool AppendFormatToFile(const char *file_path, const char *format, ...) {
  FILE *f = fopen(file_path, "ab");
  if (f == NULL) {
    return false;
  }
  va_list valist;
  va_start(valist, format);
  while (*format != '\0') {
    if (*format != '%') {
      putc(*format++, f);
      continue;
    }
    switch (*(++format)) {
      case 'd':
      case 'i':
        WriteInt64ToFile(f, va_arg(valist, int64_t));
        break;
      case 'u':
        WriteUint64ToFile(f, va_arg(valist, uint64_t));
        break;
      case 'c':
        putc((char)va_arg(valist, int), f);
        break;
      case 's':
        WriteStringToFile(f, va_arg(valist, char*));
        break;
      case 'b':
        WriteStringToFile(f, va_arg(valist, int) ? "true" : "false");
        break;
      case 'B':
        WriteStringToFile(f, va_arg(valist, int) ? "TRUE" : "FALSE");
        break;
      default:
        putc('%', f);
        putc(*format, f);
        break;
    }
    format++;
  }
  va_end(valist);
  return fclose(f) == 0;
}

inline void WriteCharToStdOut(char c) {
  putc(c, stdout);
}

inline void WriteCharToStdErr(char c) {
  putc(c, stderr);
}

#pragma endregion
#pragma region Util

inline bool CreateAllocation(Allocation *a, size_t size) {
  a->size = size;
  a->memory = malloc(size);
  if (a->memory == NULL) {
    return false;
  }
  memset(a->memory, 0, size);
  return true;
}

inline bool ResizeAllocation(Allocation *a, size_t size) {
  if (size == a->size) {
    return true;
  }
  a->memory = realloc(a->memory, size);
  if (a->memory == NULL) {
    return false;
  }
  memset(a->memory + a->size, 0, size - a->size);
  a->size = size;
  return true;
}

inline void FreeAllocation(Allocation *a) {
  a->size = 0;
  free(a->memory);
}

inline bool CreateMemoryAllocation(MemoryAllocation *ma, size_t size_of_item, uint64_t number_of_items) {
  ma->number_of_items = number_of_items;
  ma->size_of_item = size_of_item;
  ma->memory = calloc(number_of_items, size_of_item);
  return ma->memory != NULL;
}

inline bool ResizeMemoryAllocation(MemoryAllocation *ma, uint64_t number_of_items) {
  if (number_of_items == ma->number_of_items) {
    return true;
  }
  size_t old_size = ma->size_of_item * ma->number_of_items;
  uint64_t new_size = ma->size_of_item * number_of_items;
  ma->memory = realloc(ma->memory, new_size);
  if (ma->memory == NULL) {
    return false;
  }
  if (new_size > old_size) {
    memset(ma->memory + old_size, 0, new_size - old_size);
  }
  ma->number_of_items = number_of_items;
  return true;
}

inline void FreeMemoryAllocation(MemoryAllocation *ma) {
  ma->number_of_items = 0;
  free(ma->memory);
}

inline uint64_t Hash(const char *s) {
  unsigned char *us = (unsigned char*)s;
  uint64_t hash = 5381;
  while (*us != '\0') {
    hash = ((hash << 5) + hash) + *us++;
  }
  return hash;
}

inline Url UrlParse(const char *s) {
  Url url = {0};
  int64_t scheme_index = StringFirstIndexOf(s, "://");
  int64_t host_begin_index = 0;
  int64_t port_begin_index = 0;
  int64_t path_begin_index = 0;
  int64_t query_begin_index = 0;
  if (scheme_index != -1) {
    StringSliceToBuffer(url.scheme, ArraySize(url.scheme), s, 0, scheme_index);
    host_begin_index = scheme_index + 3;
  }
  for (int64_t i = host_begin_index + 1;; i++) {
    switch (s[i]) {
      case ':':
        StringSliceToBuffer(url.host, ArraySize(url.host), s, host_begin_index, i);
        port_begin_index = i + 1;
        goto ReadPort;
      case '/':
        StringSliceToBuffer(url.host, ArraySize(url.host), s, host_begin_index, i);
        path_begin_index = i;
        goto ReadPath;
      case '?':
        StringSliceToBuffer(url.host, ArraySize(url.host), s, host_begin_index, i);
        query_begin_index = i;
        goto ReadQuery;
      case '\0':
        StringSliceToBuffer(url.host, ArraySize(url.host), s, host_begin_index, i);
        goto FunctionReturn;
    }
  }
  ReadPort: {
    for (int64_t i = port_begin_index;; i++) {
      switch (s[i]) {
        case '/':
          StringSliceToBuffer(url.port, ArraySize(url.port), s, port_begin_index, i);
          path_begin_index = i;
          goto ReadPath;
        case '?':
          StringSliceToBuffer(url.port, ArraySize(url.port), s, port_begin_index, i);
          query_begin_index = i;
          goto ReadQuery;
        case '\0':
          StringSliceToBuffer(url.port, ArraySize(url.port), s, port_begin_index, i);
          goto FunctionReturn;
      }
    }
  }
  ReadPath: {
    for (int64_t i = path_begin_index;; i++) {
      switch (s[i]) {
        case '?':
          StringSliceToBuffer(url.path, ArraySize(url.path), s, path_begin_index, i);
          query_begin_index = i;
          goto ReadQuery;
        case '\0':
          StringSliceToBuffer(url.path, ArraySize(url.path), s, path_begin_index, i);
          goto FunctionReturn;
      }
    }
  }
  ReadQuery: {
    url.query = (char*)(s + query_begin_index);
  }
  FunctionReturn: {
    if (url.path[0] == '\0') {
      url.path[0] = '/';
    }
    if (StringEquals(url.scheme, "http")) {
      strcpy(url.port, "80");
    }
    else if (StringEquals(url.scheme, "https")) {
      strcpy(url.port, "443");
    }
    return url;
  }
}

#pragma endregion
#pragma region Conversions

inline int CharToInt(char c) {
  if (CharIsDigit(c)) {
    return c - 48;
  }
  return 0;
}

inline int StringToInt32(const char *s) {
  return atoi(s);
}

inline int64_t StringToInt64(const char *s) {
  return atoll(s);
}

inline bool CharToStringToBuffer(char *buffer, size_t buffer_size, char c) {
  if (buffer_size > 1) {
    buffer[0] = c;
    buffer[1] = 0;
    return true;
  }
  return false;
}

inline char* CharToStringAlloc(char c) {
  StringBuilder sb = CreateDynamicStringBuilder(2);
  StringBuilderAddChar(&sb, c);
  return sb.string;
}

inline bool Int32ToStringToBuffer(char *buffer, size_t buffer_size, int32_t value, int32_t base) {
  if (base < 2 || base > 36) {
    *buffer = '\0';
    return false;
  }
  char *ptr1 = buffer;
  while (true) {
    if (--buffer_size == 0) {
      return false;
    }
    int tmp = value;
    value /= base;
    *ptr1++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + (tmp - value * base)];
    if (value == 0) {
      if (tmp < 0) {
        *ptr1++ = '-';
      }
      *ptr1-- = '\0';
      break;
    }
  }
  while(buffer < ptr1) {
    char tmp = *ptr1;
    *ptr1-- = *buffer;
    *buffer++ = tmp;
  }
  return true;
}

inline bool Int64ToStringToBuffer(char *buffer, size_t buffer_size, int64_t value, int32_t base) {
  if (base < 2 || base > 36) {
    *buffer = '\0';
    return false;
  }
  char *ptr1 = buffer;
  while (true) {
    if (--buffer_size == 0) {
      return false;
    }
    int tmp = value;
    value /= base;
    *ptr1++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + (tmp - value * base)];
    if (value == 0) {
      if (tmp < 0) {
        *ptr1++ = '-';
      }
      *ptr1-- = '\0';
      break;
    }
  }
  while(buffer < ptr1) {
    char tmp = *ptr1;
    *ptr1-- = *buffer;
    *buffer++ = tmp;
  }
  return true;
}

inline bool Uint64ToStringToBuffer(char *buffer, size_t buffer_size, uint64_t value, int32_t base) {
  if (base < 2 || base > 36) {
    *buffer = '\0';
    return false;
  }
  char *ptr1 = buffer;
  while (true) {
    if (--buffer_size == 0) {
      return false;
    }
    uint64_t tmp = value;
    value /= base;
    *ptr1++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + (tmp - value * base)];
    if (value == 0) {
      *ptr1-- = '\0';
      break;
    }
  }
  while(buffer < ptr1) {
    char tmp = *ptr1;
    *ptr1-- = *buffer;
    *buffer++ = tmp;
  }
  return true;
}

#pragma endregion
#pragma region Hash map

inline StringHashMapValue* NewStringHashMapValue(char *key, char *value) {
  StringHashMapValue *v = malloc(sizeof(StringHashMapValue));
  if (v == NULL) {
    return NULL;
  }
  v->key = key;
  v->value = value;
  v->next = NULL;
  return v;
}

inline bool AllocateStringHashMap(StringHashMap *hm, size_t capacity) {
  if (DeallocateStringHashMap(hm)) {
    hm->capacity = capacity;
    hm->length = 0;
    hm->items = calloc(capacity, sizeof(StringHashMapValue*));
    return hm->items != NULL;
  }
  return false;
}

inline bool DeallocateStringHashMap(StringHashMap *hm) {
  if (hm != NULL) {
    hm->length = 0;
    free(hm->items);
    return true;
  }
  return false;
}

inline StringHashMap CreateStringHashMap(size_t capacity) {
  return (StringHashMap) {
    .capacity = capacity,
    .length = 0,
    .items = calloc(capacity, sizeof(StringHashMapValue*)),
  };
}

inline void StringHashMapSet(StringHashMap *hm, char *key, char *value) {
  StringHashMapValue *v = NewStringHashMapValue(key, value);
  size_t index = Hash(key) % hm->capacity;
  if (hm->items[index] == NULL) {
    hm->items[index] = v;
  } else {
    StringHashMapValue *tmp = hm->items[index];
    while (tmp->next != NULL) {
      tmp = tmp->next;
    }
    tmp->next = v;
  }
}

inline char* StringHashMapGet(StringHashMap *hm, char *key) {
  StringHashMapValue *tmp = hm->items[Hash(key) % hm->capacity];
  while (tmp != NULL && !StringEquals(key, tmp->key)) {
    tmp = tmp->next;
  }
  return tmp == NULL ? NULL : tmp->value;
}

inline bool StringHashMapRemove(StringHashMap *hm, char *key) {
  size_t index = Hash(key) % hm->capacity;
  StringHashMapValue *tmp = hm->items[index];
  if (tmp != NULL) {
    if (StringEquals(tmp->key, key)) {
      hm->items[index] = tmp->next;
      return true;
    }
    while (tmp->next != NULL) {
      if (StringEquals(tmp->next->key, key)) {
        tmp->next = tmp->next->next;
        return true;
      }
      tmp = tmp->next;
    }
  }
  return false;
}

#pragma endregion
