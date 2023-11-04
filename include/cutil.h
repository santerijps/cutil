#ifndef CUTIL_H
#define CUTIL_H

#include <stdbool.h>

// Depends on stdio.h
#define DebugLog(fmt, ...)\
  fprintf(stderr, "[%s:%d %s] " fmt "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)

#define PrintLine(fmt, ...)\
  fprintf(stdout, fmt "\n", ##__VA_ARGS__)

/*
CHAR & STRING FUNCTIONS
*/

bool CharIsAlpha(char c);
bool CharIsDigit(char c);
bool CharIsUpper(char c);
bool CharIsLower(char c);

char CharToUpper(char c);
char CharToLower(char c);

unsigned long long StringLength(const char *s);

bool StringIsAlpha(const char *s);
bool StringIsNumeric(const char *s);
bool StringIsAlphaNumeric(const char *s);

void StringToUpper(char *buffer, const char *s);
void StringToLower(char *buffer, const char *s);
void StringToTitle(char *buffer, const char *s);
void StringConcat(char *buffer, const char *s1, const char *s2);
void StringSlice(char *buffer, const char *s, long long start, long long end);
void StringHead(char *buffer, const char *s, unsigned long long n);
void StringTail(char *buffer, const char *s, unsigned long long n);
void StringReverse(char *buffer, const char *s);

bool StringContainsChar(const char *s, char search);
bool StringContainsString(const char *s, const char *search);
bool StringContainsAny(const char *s, const char *chars);
bool StringIn(const char *s, const char *haystack, const char *separator);
bool StringEquals(const char *s1, const char *s2);
bool StringStartsWith(const char *s1, const char *s2);
bool StringEndsWith(const char *s1, const char *s2);

/*
STRING BUILDER FUNCTIONS
*/

typedef struct StringBuilder {
  bool is_dynamic;
  char *string;
  unsigned long long capacity;
  unsigned long long length;
} StringBuilder;

StringBuilder NewStringBuilder(unsigned long long capacity);
StringBuilder CreateStringBuilder(char *buffer, unsigned long long capacity);
bool InitStringBuilder(StringBuilder *sb, unsigned long long capacity);
void DestroyStringBuilder(StringBuilder *sb);

bool StringBuilderIncreaseCapacity(StringBuilder *sb, unsigned long long padding);
bool StringBuilderAddChar(StringBuilder *sb, char c);
bool StringBuilderAddString(StringBuilder *sb, char *s);

#endif
