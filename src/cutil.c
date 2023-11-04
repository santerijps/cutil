#include "../include/cutil.h"

#pragma region Standard Library Includes

#ifndef _INC_STDIO
#include <stdio.h>
#endif

#ifndef _INC_STDLIB
#include <stdlib.h>
#endif

#ifndef _INC_STRING
#include <string.h>
#endif

#pragma endregion
#pragma region Char and String Functions

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

inline char CharToUpper(char c) {
  return CharIsLower(c) ? c - 32 : c;
}

inline char CharToLower(char c) {
  return CharIsUpper(c) ? c + 32 : c;
}

inline unsigned long long StringLength(const char *s) {
  unsigned long long length = 0;
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

inline void StringToUpper(char *buffer, const char *s) {
  while (*s != '\0') {
    *buffer++ = *s - ('a' <= *s && *s <= 'z') * 32;
    s++;
  }
}

inline void StringToLower(char *buffer, const char *s) {
  while (*s != '\0') {
    *buffer++ = *s + ('A' <= *s && *s <= 'Z') * 32;
    s++;
  }
}

inline void StringToTitle(char *buffer, const char *s) {
  char title = 1;
  while (*s != '\0') {
    char c = *s;
    if (!CharIsAlpha(c)) {
      title = 1;
    }
    else if (CharIsLower(c)) {
      if (title) {
        c = CharToUpper(c);
        title = 0;
      }
    }
    *buffer++ = c;
    s++;
  }
}

inline void StringConcat(char *buffer, const char *s1, const char *s2) {
  while (*s1 != '\0') {
    *buffer++ = *s1++;
  }
  while (*s2 != '\0') {
    *buffer++ = *s2++;
  }
}

inline void StringSlice(char *buffer, const char *s, long long start, long long end) {
  if (start < end) {
    while (start < end && s[start] != '\0') {
      *buffer++ = s[start++];
    }
  }
  else if (start > end) {
    while (start > end && start >= 0) {
      *buffer++ = s[start--];
    }
  }
}

inline void StringHead(char *buffer, const char *s, unsigned long long n) {
  while (n-- && *s != '\0') {
    *buffer++ = *s++;
  }
}

inline void StringTail(char *buffer, const char *s, unsigned long long n) {
  return StringHead(buffer, s + StringLength(s) - n, n);
}

inline void StringReverse(char *buffer, const char *s) {
  size_t i = StringLength(s);
  if (i > 0) {
    do {
      *buffer++ = s[--i];
    } while (i > 0);
  }
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

#pragma endregion
#pragma region String Builder Functions

inline StringBuilder NewStringBuilder(unsigned long long capacity) {
  return (StringBuilder) {
    .is_dynamic = true,
    .capacity = capacity,
    .length = 0,
    .string = calloc(capacity, 1),
  };
}

inline StringBuilder CreateStringBuilder(char *buffer, unsigned long long capacity) {
  return (StringBuilder) {
    .is_dynamic = false,
    .capacity = capacity,
    .length = 0,
    .string = buffer,
  };
}

inline bool InitStringBuilder(StringBuilder *sb, unsigned long long capacity) {
  sb->is_dynamic = true,
  sb->capacity = capacity;
  sb->length = 0;
  sb->string = calloc(capacity, sizeof(char));
  return sb->string != NULL;
}

inline void DestroyStringBuilder(StringBuilder *sb) {
  if (sb->is_dynamic) {
    free(sb->string);
  }
}

inline bool StringBuilderIncreaseCapacity(StringBuilder *sb, unsigned long long padding) {
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
  if (sb->length + 1 >= sb->capacity) {
    if (!sb->is_dynamic) {
      return false;
    }
    if (!StringBuilderIncreaseCapacity(sb, 1)) {
      return false;
    }
  }
  sb->string[sb->length++] = c;
  return true;
}

inline bool StringBuilderAddString(StringBuilder *sb, char *s) {
  unsigned long long padding = StringLength(s);
  if (sb->length + padding >= sb->capacity) {
    if (!sb->is_dynamic) {
      return false;
    }
    if (!StringBuilderIncreaseCapacity(sb, padding)) {
      return false;
    }
  }
  while (*s != '\0') {
    sb->string[sb->length++] = *s++;
  }
  return true;
}

#pragma endregion
