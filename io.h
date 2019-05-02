

#ifndef IO_H
#define IO_H

#include <wchar.h>
#include <stdio.h>

void add_sym(wchar_t ** c, wchar_t h, int cur, int * size);

int read_command(wchar_t ** c);

int file_to_text(wchar_t ** c, struct DList * List, FILE * F);

FILE * file_wchar_t(wchar_t * c, char * c1);

int Read(struct DList * List, wchar_t * c);

int Open(struct DList * List, wchar_t * c);

void Write(struct DList * List, wchar_t * c);

void set_name(wchar_t * c);


#endif
