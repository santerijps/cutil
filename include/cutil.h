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

unsigned long long StringLength(char *s);

bool StringIsAlpha(char *s);
bool StringIsNumeric(char *s);
bool StringIsAlphaNumeric(char *s);

void StringToUpper(char *buffer, char *s);
void StringToLower(char *buffer, char *s);
void StringToTitle(char *buffer, char *s);


void StringConcat(char *buffer, char *s1, char *s2);
bool StringContainsChar(char *s, char search);
bool StringContainsString(char *s, char *search);
bool StringContainsAny(char *s, char *chars);
bool StringEquals(char *s1, char *s2);

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
bool StringBuilderPrintf(StringBuilder *sb, char *format, ...);

#endif
