#ifndef LIST_H
#define LIST_H

#include <wchar.h>

struct Node
{
    wchar_t *value;
    struct Node *next;
    struct Node *prev;
};


struct DList
{
    int size;
    struct Node *head;
    struct Node *tail;
};


void push_front(struct DList* List, wchar_t *value);

void* pop_front(struct DList* List);

void push_back(struct DList* List, wchar_t *value);

void* pop_back(struct DList* List);

struct Node* getNth(struct DList *list, int index);

void deleteNth_Node(struct DList *list, struct Node * elm);

void deleteNth_index(struct DList *list, int index);

void delete_DLink(struct DList ** List);

int Text_to_string(struct DList * List, wchar_t ** c, wchar_t * substr, int begin, int end);

void insert_list_to_list(struct DList * List, struct DList * subList, struct Node * p);

int Add_string(struct DList * List, wchar_t * c, struct Node * p);
#endif
