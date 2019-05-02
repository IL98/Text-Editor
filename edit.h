

#ifndef EDIT_H
#define EDIT_H

#include <wchar.h>

void insert_after(struct DList *List, int index, wchar_t *value);

void delete_ranges(struct DList * List, int begin, int end);

void edit_string(struct DList * List, int x, int y, wchar_t * symbol);

void insert_symbol(struct DList * List, int x, int y, wchar_t * symbol);

int len_without_braces(wchar_t * c);

void delete_braces(struct DList * List, int begin, int end);

void z_function(wchar_t *  c, int * a, int length);

void wcs_replace(struct DList *List, wchar_t * patt, wchar_t * substr, int n, int m);

void replace_begin(struct DList *List, wchar_t * substr, int n, int m);

void replace_end(struct DList *List, wchar_t * substr, int n, int m);

#endif
