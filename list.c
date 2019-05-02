#include "lib.h"


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


/*
 * вставка в начало списка
 * Аргумент : список List, строка value
 */
void push_front(struct DList* List, wchar_t *value)
{
    struct Node *tmp = (struct Node*) malloc(sizeof(struct Node));
    tmp->next = List->head;
    tmp->prev = NULL;
    tmp->value = value;
    if  (List->head)
        List->head->prev = tmp;
    if (!List->tail)
        List ->tail = tmp;
    List->head = tmp;
    List->size++;
}


/*
 * удаление из начала списка
 * Аргумент : список List
 */
void* pop_front(struct DList* List)
{
    struct Node *p;
    void *tmp;
    if (!List->head)
        exit(2);
    p = List->head;
    List->head = List->head->next;
    if (List->head)
        List->head->prev = NULL;
    if (p == List->tail)
        List->tail = NULL;
    tmp = p->value;
    free(p->value);
    free(p);
    List->size--;
    return tmp;

}


/*
 * вставка в конец списка
 * Аргумент : список List, строка value
 */
void push_back(struct DList* List, wchar_t *value)
{
    struct Node* tmp = (struct Node*) malloc(sizeof(struct Node));
    tmp->prev = List->tail;
    tmp->value = value;
    tmp->next = NULL;
    if (List->tail)
        List->tail->next = tmp;
    if (!List->head)
        List->head = tmp;
    List->tail = tmp;
    List->size++;
}


/*
 * удаление из конца списка
 * Аргумент : список List
 */
void* pop_back(struct DList* List)
{
    struct Node* p;
    void* tmp;
    if (!List->tail)
        exit(3);
    p =List->tail;
    List->tail = List->tail ->prev;
    if (List->tail)
        List->tail->next = NULL;
    if (p == List->head)
        List->head = NULL;
    tmp = p->value;
    free(p->value);
    free(p);
    List->size--;
    return tmp;
}


/*
 * получение элемента списка по номеру index
 * Аргументы : список list, номер index
 * выводит элемент списка
 */
struct Node* getNth(struct DList *list, int index)
{
    struct Node *tmp = NULL;
    int i = 0;
    if (index == 0)
        return NULL;
    tmp = list->head;
    while (tmp && i < index - 1)
    {
        tmp = tmp->next;
        i++;
    }
    return tmp;
}

/*
 * удаление элемента из списка по
 * Аргументы : список list, элемент elm, который нужно удалить
 */
void deleteNth_Node(struct DList *list, struct Node * elm)
{
    if (elm == NULL)
    {
        exit(5);
    }
    if (elm->prev)
    {
        elm->prev->next = elm->next;
    }
    if (elm->next)
    {
        elm->next->prev = elm->prev;
    }
    if (!elm->prev)
    {
        list->head = elm->next;
    }
    if (!elm->next)
    {
        list->tail = elm->prev;
    }
    free(elm->value);
    elm->value = NULL;
    free(elm);
    elm = NULL;
    list->size--;
}


/*
 * удление элемента списка по номеру index
 * Аргументы : список list, номер index
 */
void deleteNth_index(struct DList *list, int index)
{
    struct Node *elm = NULL;
    elm = getNth(list, index);
    if (elm == NULL)
    {
        exit(5);
    }
    if (elm->prev)
    {
        elm->prev->next = elm->next;
    }
    if (elm->next)
    {
        elm->next->prev = elm->prev;
    }

    if (!elm->prev)
    {
        list->head = elm->next;
    }
    if (!elm->next)
    {
        list->tail = elm->prev;
    }
    free(elm->value);
    free(elm);
    list->size--;
}


/*
 * удаление списка
 * Аргумент: указатель на список
 */
void delete_DLink(struct DList ** List)
{
    struct Node *tmp = (*List)->head;
    struct Node *next = NULL;
    while (tmp)
    {
        next = tmp->next;
        deleteNth_Node(*List, tmp);
        tmp = next;
    }
    free(*List);
    (*List) = NULL;

}


/*
 * преобразование диапазона списка в одну строку
 * если вначало каждой строки из диапазона списка приставить cтркоу substr
 * Аргументы: список List, указател на строку c, строка substr, которую вставляем в начало
 * диапазон - номера begin и end
 * выводит -1 в случае ошибки и 0 в противном случае
 */
int Text_to_string(struct DList * List, wchar_t ** c, wchar_t * substr, int begin, int end)
{
    int i = 0;
    int count = 1;
    int len = (int)wcslen(substr);
    size_t hl = 0;
    wchar_t * f = NULL;
    struct Node *p = getNth(List, begin);
    struct Node *tmp = p;
    for (i = begin; i <= end; i++)
    {
        count += wcslen(tmp->value) + len;
        tmp = tmp->next;
    }
    if (!(*c = (wchar_t *)calloc((size_t)count, sizeof(wchar_t))))
    {
        fwprintf(stderr, L"can't aloccate memory\n");
        return -1;
    }
    f = *c;
    tmp = p;
    for (i = begin; i <= end; i++)
    {
        hl = wcslen(p->value);
        wmemcpy(f, substr, len);
        wmemcpy(f + len,p->value, hl + 1);
        f += len + hl;
        tmp = p->next;
        deleteNth_Node(List, p);
        p = tmp;
    }
    (*c)[count -1] = L'\0';
    return count - 1;
}

/*
 * вставка списка subList в список List после  элемента p, если в начало, то p = NULL
 * Аргументы: список List, список subList, элемента списка List p
 */
void insert_list_to_list(struct DList * List, struct DList * subList, struct Node * p)
{/* */
    if (subList-> head == NULL)
    {
        return;
    }
    List->size += subList->size;
    if (List->size == subList->size)
    {
        List->head = subList->head;
        List->tail = subList->tail;
        return;
    }
    if (p == NULL)
    {
        subList->tail->next = List->head;
        List->head->prev = subList->tail;
        List->head = subList->head;
        return;
    }
    if (p == List->tail)
    {
        subList->head->prev = List->tail;
        List->tail->next = subList->head;
        List->tail = subList->tail;
        return;
    }
    subList->tail->next = p->next;
    p->next->prev = subList->tail;
    subList->head->prev = p;
    p->next = subList->head;
    return;
}




/*
 * добавлет строку в которую нужно разбить на много строк в список после элемента списка p
 * Аргументы: список List, строка  с, элемент списка p
 * выводит -1 в случае ошибки и 0 в противном случае
 */
int Add_string(struct DList * List, wchar_t * c, struct Node * p)
{
    int n = (int)wcslen(c);
    int i = 0;
    int k = 0;
    struct DList* subList = NULL;
    if (!(subList= (struct DList*)calloc(1, sizeof(struct DList))))
    {
        fwprintf(stderr, L"can't aloccate memory\n");
        return -1;
    }
    subList->size = 0;
    subList->head = subList->tail = NULL;
    for (i = 0; i < n; i++)
    {
        if (c[i] == L'\n')
        {
            wchar_t * a = (wchar_t *)calloc( (size_t)i - k + 2,sizeof(wchar_t));
            wcsncpy(a, c + k, (size_t)i - k + 1);
            a[i - k + 1] = L'\0';
            push_back(subList, a);
            k = i + 1;
        }
    }
    insert_list_to_list(List, subList, p);
    free(subList);
    return 0;
}

