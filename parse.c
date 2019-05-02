#include "lib.h"

/*
 * выводит количество слов в строке c
 * нужно для обработки аргументов
 * Аргументы: строка с
 * выводит количество слов
 */
int word_count(wchar_t * c)
{
    int i;
    int n = (int)wcslen(c);
    int f = 0;/*проверяет находимся ли мы в символах разделителей или нет*/
    int f1 = 0;/*проверяет в двойных кавычках ли мы*/
    int count = 0;/*количество слов*/
    for (i = 0; i < n; i++)
    {
        if (c[i] == L'\\' && f1)
        {
            i++;
        } else
        if (!f1)
        {

            if (!f)
            {
                if (c[i] != L'\t' && c[i] != L'\n' && c[i] != L'\r' && c[i] != L' ')
                {
                    f = 1;
                    count++;
                }
            } else
            {
                if (c[i] == L'\t' || c[i] == L'\n' || c[i] == L'\r' || c[i] == L' ')
                {
                    f = 0;
                }
            }
            if (c[i] == L'\"')
            {
                if (c[i + 1] == L'\"' && c[i + 2] == L'\"')
                {
                    i += 3;
                    while (c[i] != L'\"' || c[i + 1] != L'\"' || c[i + 2] != L'\"')
                    {
                        i++;
                    }
                } else
                    f1 = 1;
            }
        }else
        {
            if (c[i] == L'"')
                f1 = 0;
        }
    }
    return count;
}


/*
 * извлекает и сохраняет слова из строки с
 * Аргументы : строка с
 * выводит массив слов, полученных при парсинге строки c
 */
wchar_t ** _parse(wchar_t* c)
{
    int n = word_count(c);
    int m = 0;
    int k = -1;/*первый символ текущего слова*/
    int f = 1;/*находимся ли мы в знаках разделителях или нет*/
    int cav = 0;/*находимся ли мы в кавычках или нет*/
    int tch = -1; /*на каком месте мы в массиве*/
    int tricav = 0;/*находимся ли мы в тройных кавычках или нет*/
    wchar_t ** a= (wchar_t **) calloc((size_t)n, sizeof(wchar_t*));
    int i;
    m = (int)wcslen(c);
    for (i = 0; i < m; i++)
    {
        if (c[i] == '\\' && cav) {
            i++;
        } else
        if (!cav) {
            if (f)
            {
                if (c[i] != '\t' && c[i] != '\n' && c[i] != '\r' && c[i] != ' ')
                {
                    f =  0;
                    k = i;
                }
            } else
            {
                if (c[i] == '\t' || c[i] == '\n' || c[i] == '\r' || c[i] == ' ')
                {
                    tch++;
                    a[tch] = (wchar_t*)calloc((size_t)i - k + 3, sizeof(wchar_t));
                    wmemcpy(a[tch], c + k, (size_t)i - k);
                    a[tch][i - k] = L'\0';
                    k = -1;
                    f = 1;
                }
            }
            if (c[i] == '\"')
            {
                cav = 1;
                k = i;
                if (c[i+1] == '\"' && c[i+2] == '\"')
                    tricav = 1;
            }

        }else
        {
            if (!tricav)
            {
                if (c[i] == '\"')
                {
                    tch++;
                    a[tch] = (wchar_t*)calloc((size_t)i - k + 3, sizeof(wchar_t));
                    wmemcpy(a[tch], c + k, (size_t)i - k + 1);
                    a[tch][i - k + 1] = L'\0';
                    k = -1;
                    cav = 0;
                    f = 1;
                }
            } else
            {
                if (c[i] == '\"' && c[i+1] == '\"' && c[i+2] == '\"')
                {
                    tch++;
                    a[tch] = (wchar_t*)calloc((size_t)i - k + 5, sizeof(wchar_t));
                    wmemcpy(a[tch], c + k, (size_t)i - k + 3);
                    a[tch][i - k + 3] = L'\0';
                    k = -1;
                    cav = 0;
                    tricav = 0;
                    i += 3;
                }
            }
        }
    }
    if (k != -1)
    {
        tch++;
        a[tch] = (wchar_t*)calloc((size_t)i - k + 3, sizeof(wchar_t));
        wmemcpy(a[tch], c + k, (size_t)i - k);
        a[tch][i - k] = L'\0';
    }
    return a;
}


/*
 * преобразовние строки с имитацией экраннированных символов ввиде двух
 * в строку с реальными экранированными символами
 * причем иногда добавляет к строке в конец символ переноса строки если нужно
 * Аргументы:
 * строка с, длина строки length, число n - обрезает с двух сторон на n симвлов рассматриваемую строку
 * flag - если равен единице, то прибавляем символ переноса строки, иначе нет
 * выводит новую преобразованную строку
 */
wchar_t * parsing_string(wchar_t * c, int length, int n, int flag)
{
    int i = n;
    int k = n;
    wchar_t * ap = NULL;
    wchar_t * helper = NULL;
    int count = 1;
    while (i  < length - n){
        if (c[i] == L'\\')
        {
            if ((c[i+1] == L'n') || (c[i+1] == L'r') || (c[i+1] == L't') || (c[i+1] == L'\\'))
            {
                i += 2;
            }
        } else
        {
            i++;
        }
        count++;
    }
    i = n;
    if (!(ap = (wchar_t*)calloc((size_t)count + 1, sizeof(wchar_t))))
    {
        fwprintf(stderr, L"can't aloccate memory\n");
        return NULL;/*-----------------------здесь надо почистить всю память*/
    }
    helper = ap;
    while (i < length - n)
    {
        if (c[i] == L'\\')
        {
            wmemcpy(helper, c + k, (size_t)i - k);
            helper += i - k;
            if (c[i+1] == L'n')
            {
                wmemcpy(helper, L"\n", 1);
                helper++;
            }
            if (c[i+1] == L'r')
            {
                wmemcpy(helper, L"\r", 1);
                helper++;
            }
            if (c[i+1] == L't')
            {
                wmemcpy(helper, L"\t", 1);
                helper++;
            }
            if (c[i+1] == L'\\')
            {
                wmemcpy(helper, L"\\", 1);
                helper++;
            }
            i += 2;
            k = i;
        } else
        {
            i++;
        }
    }
    wmemcpy(helper, c + k, (size_t)i - k);
    k = (int)wcslen(ap);
    if (flag == 1)
    {

        ap[k] = L'\n';
        k++;
    }
    ap[k] = L'\0';
    return ap;
}
