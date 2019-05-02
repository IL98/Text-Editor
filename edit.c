#include "lib.h"
#include "list.h"
#include "tech.h"
#include "string.h"


/*
 * (команда insert after)
 * вставляет в список List строку value после номера index
 * Аргумнты: список List, номер index, строка value
 */
void insert_after(struct DList *List, int index, wchar_t *value)
{
    struct Node * p = NULL;
    if (index > List->size)
        return;
    if (index == 0)
    {
        Add_string(List, value, NULL);
        return;
    } else
    {
        p = getNth(List, index);
        Add_string(List, value, p);
    }
}


/*
 * (команда delete range)
 * удаляет строки из списка List с номера begin по end
 * Аргументы: список List, номера begin и end
 */
void delete_ranges(struct DList * List, int begin, int end)
{
    struct Node * p = getNth(List, begin);
    struct Node * tmp = NULL;
    int i = 0;
    for (i = begin; i <=end; i++ )
    {
        tmp = p->next;
        deleteNth_Node(List, p);
        p = tmp;
    }
}


/*
 * (команда edit string)
 * заменяет символ с координатами x и y символом symbol[0]
 * Аргументы: список List, номер строки x, номер столбца y, широкая
 * строка в которой один элемент - символ
 */
void edit_string(struct DList * List, int x, int y, wchar_t * symbol)
{
    struct Node * predeccessor = NULL;
    struct Node * p = getNth(List, x);
    wchar_t * c = NULL;
    size_t len = 0;
    size_t count = wcslen(p->value);
    if (y < 1 || y > count)
    {
        fwprintf(stderr, L"incorrect argument\n");
        return;
    }
    predeccessor = getNth(List, x - 1);
    len = wcslen(symbol);
    if (len > 1)
    {
        fwprintf(stderr, L"can't do operation edit string\n");
        return;
    }
    if (!(c = (wchar_t *)calloc(count + 1, sizeof(wchar_t))))
    {
        fwprintf(stderr, L"can't aloccate memory\n");
        return;/*-----------------------здесь надо почистить всю память*/
    }
    wmemcpy(c, p->value, count);
    c[y - 1] = symbol[0];
    c[count] = L'\0';
    deleteNth_Node(List, p);
    if (Add_string(List, c, predeccessor) == -1)
    {
        if (c != NULL)
        {
            free(c);
            c = NULL;
            free(c);
        }
        return;
    }
    free(c);
}


/*
 * (команда insert symbol)
 * вставляет символ с координатами x и y символом symbol[0]
 * Аргументы: список List, номер строки x, номер столбца y, широкая
 * строка в которой один элемент - символ
 */
void insert_symbol(struct DList * List, int x, int y, wchar_t * symbol)
{
    struct Node * p = getNth(List, x);
    wchar_t * tmp = p->value;
    struct Node * predeccessor = getNth(List, x - 1);
    wchar_t * c = NULL;
    size_t count = wcslen(p->value);
    size_t len = wcslen(symbol);
    if (y == -10)
    {
        y = (int)count;
    }
    if (y < 1)
    {
        y = 1;
    }
    if (y > count)
    {
        y = (int)count;
    }
    if (len > 1)
    {
        fwprintf(stderr, L"can't do operation edit string\n");
        return;
    }
    if (!(c = (wchar_t *)calloc(count + 2, sizeof(wchar_t))))
    {
        fwprintf(stderr, L"can't aloccate memory\n");
        return;
    }
    wmemcpy(c, tmp, (size_t)y - 1);
    c[y - 1] = symbol[0];
    tmp += y - 1 ;
    wmemcpy(c + y, tmp, wcslen(tmp));
    c[count + 1] = L'\0';
    deleteNth_Node(List, p);
    if (Add_string(List, c, predeccessor) == -1)
    {
        if (c != NULL){
            free(c);
            c = NULL;
        }
        return;
    }
    free(c);
}


/*
 * выводит длину предполагаемой строки
 * после удаления внутри фигурных скобок из этой строки
 * Аргумент - строка c, над которой проводятся наблюдение
 */
int len_without_braces(wchar_t * c)
{
    int k = 0;
    int i = 0;
    int balance = 0;
    int result = 0;
    int len = (int)wcslen(c);
    while (i < len)
    {
        if (c[i] == L'{')
        {
            balance++;
            if (balance == 1)
            {
                result += i - k;
            }
        }
        if (c[i] == L'}')
        {
            balance--;
            if (!balance)
            {
                k = i + 1;
            }
        }
        i++;
    }
    if (balance <= 0)
    {
        result += i - k;
    }
    return result;
}


/*
 * команда delete braces
 * удаляет символы между фигурными скобками в указанном диапазоне
 * Аргументы: список List, диапазон -  номера begin и end
 */
void delete_braces(struct DList * List, int begin, int end)
{
    wchar_t * c =NULL;
    wchar_t * new_c = NULL;
    wchar_t * cur = NULL;
    struct Node * precessor = getNth(List, begin - 1);
    int k = 0;
    int i = 0;
    int balance = 0;
    int count;
    int len;
    if (Text_to_string(List, &c, L"", begin, end) == -1)
    {
        return;
    }
    count = len_without_braces(c);
    len = (int)wcslen(c);
    if (!(new_c = (wchar_t *)calloc((size_t)count + 2, sizeof(wchar_t))))
    {
        fwprintf(stderr, L"can't aloccate memory\n");
        if (c != NULL)
        {
            free(c);
            c = NULL;
        }
        return;
    }
    cur = new_c;
    while (i < len)
    {
        if (c[i] == L'{')
        {
            balance++;
            if (balance == 1)
            {
                wmemcpy(cur, c + k, (size_t)i - k);
                cur += i - k;
            }
        }
        if (c[i] == L'}')
        {
            balance--;
            if (!balance)
            {
                k = i + 1;
            }
        }
        i++;
    }
    if (balance <= 0)
    {
        wmemcpy(cur, c + k,(size_t)i - k);
    }
    if (balance > 0)
    {
        if ((new_c[count -1] != L'\n') && (count != 0) )
        {
            new_c[count] = L'\n';
            count++;
        }
    }
    new_c[count] = L'\0';
    free(c);
    c =NULL;
    Add_string(List, new_c, precessor);
    free(new_c);
}


/*
 * считает z-функцию для строки c длинной length
 * результат формируется в массив а
 * Аргументы: строка с, целочисленный массив а, длинна массива
 */
void z_function(wchar_t *  c, int * a, int length)
{
    int i = 1;
    int l = 0;
    int r = 0;
    for (i = 1; i < length; ++i)
    {
        a[i] = max(0,  min (r - i + 1, a[i - l]));
        while (i+a[i] < length && c[a[i]] == c[i+a[i]])
            ++a[i];
        if (i + a[i] - 1 > r)
        {
            l = i;
            r = i + a[i] - 1;
        }
    }
}

/*
 * команда replace substring - замены подстроки в строке
 * производит замену в списке строк List, строки patt на строку substr
 * в диапазоне от n до m
 * Аргументы: список List, строка path - ее требуется заменить,
 * строка substr, которой заменяем,
 * диапазон -  числа n и m
 */
void wcs_replace(struct DList *List, wchar_t * patt, wchar_t * substr, int n, int m)
{
    int length;
    int i = 0;
    int j = 0;
    int length1 = (int)wcslen(patt);
    int length2 = (int)wcslen(substr);
    int work_count = 1;
    int  *a = NULL;
    struct Node * predecessor = getNth(List, n - 1);
    wchar_t * c = NULL;
    wchar_t * new_str = NULL;
    wchar_t * work_str = NULL;
    wchar_t * s = NULL;

    if ((length = Text_to_string(List, &c, L"", n, m)) == -1)
    {
        return;
    }
    if (!(work_str = (wchar_t *)calloc((size_t)(length1 + length + 2), sizeof(wchar_t))))
    {
        fwprintf(stderr, L"can't aloccate memory\n");
        if (c != NULL)
        {
            free(c);
            c = NULL;
            return;
        }
    }

    wmemcpy(work_str, patt, (size_t)length1);
    work_str[length1] = L'|';
    wmemcpy(work_str + length1 + 1, c, (size_t)length + 1);
    free(c);
    c = NULL;
    work_str[length1 + length + 1]  = L'\0';
    work_str[length1] = L'\0';
    if (!(a = malloc((length + length1 + 2)* sizeof(int))))
    {
        fwprintf(stderr, L"can't aloccate memory\n");
        if (c != NULL)
        {
            free(c);
            c = NULL;
        }
        if (work_str != NULL)
        {
            free(work_str);
            work_str = NULL;
        }
        return;
    }
    memset(a, 0, (size_t)length + length1 + 2);
    z_function(work_str, a, length + length1 + 2);
    for (i = length1 + 1; i < length + 1 + length1; i++)
    {
        if (a[i] == length1)
        {
            work_count += length2;
            i += length1 - 1;
        }else
        {
            work_count++;
        }
    }
    if (!(new_str = (wchar_t *)malloc((work_count + 5)  * sizeof(wchar_t))))
    {
        if (c != NULL)
        {
            free(c);
            c = NULL;
        }
        if (work_str != NULL)
        {
            free(work_str);
            work_str = NULL;
        }
        if (a != NULL)
        {
            free(a);
            a = NULL;
        }
    }
    for(i = 0; i < length - 1; i++)
    {
        if (a[i + length1 + 1] == length1)
        {
            wmemcpy(new_str + j, substr, length2);
            j += length2;
            i += length1 - 1;
        }else
        {
            new_str[j] = work_str[i + length1 + 1];
            j++;
        }
    }
    new_str[j] = L'\0';
    if (a[length + length1] == length1 + 1)
    {
        wmemcpy(new_str + j, substr, length2);
        j += length2;
    }  else
    {
        new_str[j] = work_str[length + length1];
        new_str[++j] = L'\0';
    }
    if (new_str[j - 1] != L'\n')
    {
        new_str[j] = L'\n';
        new_str[++j] = L'\0';
    }
    free(a);
    a =NULL;
    free(work_str);
    work_str = NULL;
    s = new_str;
    Add_string(List, s, predecessor);
    free(new_str);
    new_str = NULL;
}


/*
 * команда replace substring - вставка в начало
 * вставляет в начало каждой строки текста List указанного диапазона
 * данную строку substr
 * Аргументы: список List, строка substr, которую вставляем в начало
 * диапазон -  числа n и m
 */
void replace_begin(struct DList *List, wchar_t * substr, int n, int m)
{
    struct Node * predecessor = getNth(List, n - 1);
    wchar_t * c = NULL;
    if (Text_to_string(List, &c, substr, n, m) == -1)
    {
        return;
    }
    Add_string(List, c, predecessor);
    free(c);
    c = NULL;
}


/*
 * команда replace substring - вставка в конец
 * вставляет в конец каждой строки текста List указанного диапазона
 * данную строку substr
 * Аргументы: список List, строка substr, которую вставляем в конец
 * диапазон -  числа n и m
 */
void replace_end(struct DList *List, wchar_t * substr, int n, int m)
{
    wchar_t * c = NULL;
    size_t len = wcslen(substr);
    wchar_t *p = NULL;
    if (!(c = (wchar_t *)calloc(len + 2, sizeof(wchar_t))))
    {
        fwprintf(stderr, L"can't aloccate memory\n");
        return;/*здесь надо почистить всю память*/
    }
    wmemcpy(c, substr, len);
    c[len] = L'\n';
    c[len +1] = L'\0';
    p = c;
    wcs_replace(List, L"\n", p, n, m);
    free(c);
    c = NULL;
}
