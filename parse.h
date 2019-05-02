

#ifndef PARSE_H
#define PARSE_H

#include <wchar.h>

int word_count(wchar_t * c);

wchar_t ** _parse(wchar_t* c);

wchar_t * parsing_string(wchar_t * c, int length, int n, int flag);

#endif