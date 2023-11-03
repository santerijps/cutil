#include "../include/cutil.h"

#pragma region Standard Library Includes

#include <stdarg.h>

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
  return CharIsLower(c) || CharIsUpper(c);
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

inline unsigned long long StringLength(char *s) {
  unsigned long long length = 0;
  while (*s++ != '\0') {
    length += 1;
  }
  return length;
}

inline bool StringIsAlpha(char *s) {
  while (*s != '\0') {
    if (!CharIsAlpha(*s)) {
      return 0;
    }
    s++;
  }
  return 1;
}

inline bool StringIsNumeric(char *s) {
  while (*s != '\0') {
    if (!CharIsDigit(*s)) {
      return 0;
    }
    s++;
  }
  return 1;
}

inline bool StringIsAlphaNumeric(char *s) {
  while (*s != '\0') {
    if (!CharIsAlpha(*s) && !CharIsDigit(*s)) {
      return 0;
    }
    s++;
  }
  return 1;
}

inline void StringToUpper(char *buffer, char *s) {
  while (*s != '\0') {
    *buffer++ = *s - ('a' <= *s && *s <= 'z') * 32;
    s++;
  }
}

inline void StringToLower(char *buffer, char *s) {
  while (*s != '\0') {
    *buffer++ = *s + ('A' <= *s && *s <= 'Z') * 32;
    s++;
  }
}

inline void StringToTitle(char *buffer, char *s) {
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

inline void StringConcat(char *buffer, char *s1, char *s2) {
  // sprintf(buffer, "%s%s", s1, s2);
  while (*s1 != '\0') {
    *buffer++ = *s1++;
  }
  while (*s2 != '\0') {
    *buffer++ = *s2++;
  }
}

inline bool StringContainsChar(char *s, char search) {
  while (*s != '\0') {
    if (*s++ == search) {
      return 1;
    }
  }
  return 0;
}

inline bool StringContainsString(char *s, char *search) {
  if (StringLength(s) >= StringLength(search)) {
    while (*s != '\0') {
      for (size_t i = 0; search[i] != '\0'; i++) {
        if (*(s + i) != search[i]) {
          goto ContinueWhile;
        }
      }
      return 1;
ContinueWhile:
      s++;
    }
  }
  return 0;
}

inline bool StringContainsAny(char *s, char *chars) {
  while (*s != '\0') {
    for (size_t i = 0; chars[i] != '\0'; i++) {
      if (*s == chars[i]) {
        return 1;
      }
    }
    s++;
  }
  return 0;
}

inline bool StringEquals(char *s1, char *s2) {
  do {
    if (*s1 != *s2) {
      return 0;
    }
  } while (*s1++ != '\0' && *s2++ != '\0');
  return 1;
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

bool StringBuilderPrintf(StringBuilder *sb, char *format, ...) {
  char buffer[512] = {0};
  va_list args;
  va_start(args, format);
  while (*format != '\0') {
    if (*format == '%') {
      char specifier[8] = {0};
      for (size_t i = 0; i < sizeof(specifier) && format[i] != '\0'; i++) {
        specifier[i] = *(format + i);
        if (CharIsAlpha(specifier[i])) {
          break;
        }
      }
      strcat(buffer, specifier)
      printf("Specifier: %s\n", specifier);
    }
    format++;
  }
  va_end(args);
  return true;
}

#pragma endregion
