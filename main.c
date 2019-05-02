#include "lib.h"
#include <unistd.h>
#include <locale.h>
#include "tech.h"
#include "list.h"
#include "edit.h"
#include "print.h"
#include "parse.h"
#include "io.h"


int setnumbers = 0;
int settabwith = 8;
int setwarp = 1;
int Ex = 0;
int Save = 0;
FILE * ass_file = NULL;


/*
 * функция распарсивания строки, которую мы принимаем на входе,
 * по аргументам, и выполнение соответсвующей команды
 * параметры: строка, считанная со стандартного потока ввода и список
 */
void parsing_arguments(wchar_t* c, struct DList* List)
{
    wchar_t** a = NULL;
    wchar_t * b =  NULL;
    wchar_t * ap = NULL;
    struct Node * predecessor = NULL;
    int in = 0;
    int n = 0;
    int m = 0;
    int begin = 0;
    int end= 0;
    int count = word_count(c);
    if (count == 0){
        return;
    }
    a = _parse(c);
    if (count >= 2)
    {
        if (!(wcscmp(a[0], (wchar_t *) L"insert")) && !(wcscmp(a[1], (wchar_t *) L"after")))
        {
            if (count == 2)
            {
                fwprintf(stderr, L"incorrrect argument\n");
                for (n = 0; n < count; n++)
                {
                    free(a[n]);
                }
                free(a);
                return;
            }
            if (a[2][0] == L'\"')
            {

                if (a[2][wcslen(a[2]) - 1] != L'\"')
                {
                    fwprintf(stderr, L"incorrrect argument\n");
                    for (n = 0; n < count; n++)
                    {
                        free(a[n]);
                    }
                    free(a);
                    return;
                }
                if (a[2][1] != L'\"' || a[2][2] != L'\"')
                {
                    ap = parsing_string(a[2], (int) wcslen(a[2]), 1, 1);
                    insert_after(List, List->size, ap);
                    free(ap);
                    ap = NULL;
                } else
                {
                    if (wcslen(a[2]) < 6)
                    {
                        fwprintf(stderr, L"incorrrect argument\n");
                        for (n = 0; n < count; n++)
                        {
                            free(a[n]);
                        }
                        free(a);
                        return;
                    }
                    if (a[2][wcslen(a[2]) - 2] != L'\"' || a[2][wcslen(a[2]) - 3] != L'\"')
                    {
                        fwprintf(stderr, L"incorrrect argument\n");
                        for (n = 0; n < count; n++)
                        {
                            free(a[n]);
                        }
                        free(a);
                        return;
                    }
                    begin = 3;
                    end = (int) wcslen(a[2]) - 4;
                    if (a[2][3] == L'\n')
                    {
                        begin++;
                    }
                    if (a[2][end] == L'\n')
                    {
                        end--;
                    }
                    b = (wchar_t *) calloc((size_t) end - begin + 3, sizeof(wchar_t));
                    wmemcpy(b, a[2] + begin, (size_t) end - begin + 1);
                    b[end - begin + 1] = L'\n';
                    b[end - begin + 2] = L'\0';
                    Add_string(List, b, List->tail);
                    free(b);
                    b = NULL;
                    predecessor = NULL;
                }
            } else
            {
                n = wtoi(a[2]);
                if (n > List->size)
                {
                    fwprintf(stderr, L"incorrrect argument\n");
                    for (n = 0; n < count; n++)
                    {
                        free(a[n]);
                    }
                    free(a);
                    return;
                }
                if (a[3][wcslen(a[3]) - 1] != L'\"' || a[3][0] != L'\"')
                {
                    fwprintf(stderr, L"incorrrect argument\n");
                    for (n = 0; n < count; n++) {
                        free(a[n]);
                    }
                    free(a);
                    return;
                }
                if (a[3][1] != L'\"' || a[3][2] != L'\"')
                {

                    ap = parsing_string(a[3], (int) wcslen(a[3]), 1, 1);
                    insert_after(List, n, ap);
                    free(ap);
                    ap = NULL;
                } else
                {
                    if (wcslen(a[3]) < 6)
                    {
                        fwprintf(stderr, L"incorrrect argument\n");
                        for (n = 0; n < count; n++)
                        {
                            free(a[n]);
                        }
                        free(a);
                        return;
                    }
                    if (a[3][wcslen(a[3]) - 2] != L'\"' || a[3][wcslen(a[3]) - 3] != L'\"' || a[3][1] != L'\"' || a[3][2] != L'\"')
                    {
                        fwprintf(stderr, L"incorrrect argument\n");
                        for (n = 0; n < count; n++)
                        {
                            free(a[n]);
                        }
                        free(a);
                        return;
                    }
                    begin = 3;
                    end = (int) wcslen(a[3]) - 4;
                    if (a[3][3] == L'\n')
                    {
                        begin++;
                    }
                    if (a[3][end] == L'\n')
                    {
                        end--;
                    }
                    if (!(b = (wchar_t *) calloc((size_t) end - begin + 3, sizeof(wchar_t))))
                    {
                        fwprintf(stderr, L"can't allocate memory\n");
                        for (n = 0; n < count; n++) {
                            free(a[n]);
                        }
                        free(a);
                        return;
                    }
                    wmemcpy(b, a[3] + begin, (size_t) end - begin + 1);
                    b[end - begin + 1] = L'\n';
                    b[end - begin + 2] = L'\0';
                    predecessor = getNth(List, n);
                    Add_string(List, b, predecessor);
                    free(b);
                    b = NULL;
                    predecessor = NULL;
                }

            }
            in = 1;
        }

        if (!wcscmp(a[0], (wchar_t *) L"set") && !wcscmp(a[1], (wchar_t *) L"tabwidth"))
        {
            n = wtoi(a[2]);
            if (n < 0)
            {
                fwprintf(stderr, L"incorrect argument\n");
            } else {
                settabwith = n;
            }
            in = 1;
        }
        if (!wcscmp(a[0], (wchar_t *) L"set") && !wcscmp(a[1], (wchar_t *) L"numbers"))
        {
            if (!wcscmp(a[2], (wchar_t *) L"yes"))
            {
                setnumbers = 1;
            } else {
                if (!wcscmp(a[2], (wchar_t *) L"no"))
                {
                    setnumbers = 0;
                } else {
                    fwprintf(stderr, L"incorrect argument\n");
                }
            }
            in = 1;
        }

        if (!wcscmp(a[0], (wchar_t *)L"print") && !wcscmp(a[1], (wchar_t *)L"pages"))
        {
            Print_Dlist(List, setnumbers, settabwith, setwarp, 1, List->size);
            in = 1;
        }

        if (!wcscmp(a[0], (wchar_t *)L"print") && !wcscmp(a[1], (wchar_t *)L"range"))
        {
            if (count == 2)
            {
                Print_Dlist(List, setnumbers, settabwith, setwarp, 1, List->size);
            }
            if (count == 3)
            {
                n = wtoi(a[2]);
                if (n <= 0 || n > List->size)
                {
                    fwprintf(stderr, L"incorrect argument\n");
                }else
                {
                    Print_Dlist(List, setnumbers, settabwith, setwarp, n, List->size);
                }
            }
            if (count == 4)
            {
                n = wtoi(a[2]);
                m = wtoi(a[3]);
                if (n <= 0 || m <= 0 || n >m || n>List->size)
                {
                    fwprintf(stderr, L"incorrect argument\n");
                }else
                {
                    if (m >List->size){
                        m = List->size;
                    }
                    Print_Dlist(List, setnumbers, settabwith, setwarp, n, m);
                }
            }
            in = 1;
        }

        if (!wcscmp(a[0], (wchar_t *)L"set") && !wcscmp(a[1], (wchar_t *)L"wrap"))
        {
            if (!wcscmp(a[2], (wchar_t *)L"no") )
            {
                setwarp = 0;
            }else{
                if (!wcscmp(a[2], (wchar_t *)L"yes") )
                {
                    setwarp = 1;
                }else
                {
                    fwprintf(stderr, L"incorrect argument\n");
                }
            }
            in = 1;
        }
        if ((!wcscmp(a[0], (wchar_t *)L"replace") && !wcscmp(a[1], (wchar_t *)L"substring")))
        {
            if (count < 4)
            {
                fwprintf(stderr, L"incorrect argument\n");
                for (n = 0; n < count; n++)
                {
                    free(a[n]);
                }
                free(a);
                return;
            }
            if (count == 4)
            {
                n = 1;
                m = List->size;
            }
            if (count == 5)
            {
                n = wtoi(a[2]);
                if (n < 1 || n > List->size)
                {
                    fwprintf(stderr, L"incorrect argument\n");
                    for (n = 0; n < count; n++)
                    {
                        free(a[n]);
                    }
                    free(a);
                    return;
                }
                m = List->size;
            }
            if (count == 6)
            {
                n = wtoi(a[2]);
                m = wtoi(a[3]);
                if (n < 0 || m <0 || n >m || n > List->size || m  >List->size)
                {
                    fwprintf(stderr, L"incorrect argument\n");
                    for (n = 0; n < count; n++)
                    {
                        free(a[n]);
                    }
                    free(a);
                    return;
                }
            }
            if(!wcscmp(a[count - 2], L"^") && !wcscmp(a[count - 2], L"$") && (a[count - 2][0] != L'\"' || a[count - 2][wcslen(a[count - 2]) - 1] != L'\"'))
            {
                fwprintf(stderr, L"incorrect argument\n");
                for (n = 0; n < count; n++)
                {
                    free(a[n]);
                }
                free(a);
                return;
            }
            if ((a[count - 1][0] != L'\"' || a[count - 1][wcslen(a[count - 1]) - 1] != L'\"'))
            {
                fwprintf(stderr, L"incorrect argument\n");
                for (n = 0; n < count; n++)
                {
                    free(a[n]);
                }
                free(a);
                return;
            }
            if (!wcscmp(a[count - 2], (wchar_t *)L"^"))
            {
                ap = parsing_string(a[count - 1], (size_t)wcslen(a[count - 1]), 1, 0);
                replace_begin(List, ap, n, m);
                free(ap);
                ap = NULL;
            } else
            {
                if (!wcscmp(a[count - 2], (wchar_t *) L"$"))
                {
                    ap = parsing_string(a[count - 1], wcslen(a[count - 1]), 1, 0);
                    replace_end(List, ap, n, m);
                    free(ap);
                    ap = NULL;
                }else
                {
                    ap = parsing_string(a[count - 2], wcslen(a[count - 2]), 1, 0);
                    b = parsing_string(a[count - 1], wcslen(a[count - 1]), 1, 0);
                    wcs_replace(List, ap, b, n, m);
                    free(ap);
                    ap = NULL;
                    free(b);
                    b = NULL;
                }
            }
            Save = 0;
            in = 1;
        }
        if ((!wcscmp(a[0], (wchar_t *)L"edit") && !wcscmp(a[1], (wchar_t *)L"string")))
        {
            if (count < 5)
            {
                fwprintf(stderr, L"too few arguments\n");
                for (n = 0; n < count; n++)
                {
                    free(a[n]);
                }
                free(a);
                return;
            }
            n = wtoi(a[2]);
            m = wtoi(a[3]);
            if (n < 1 || (n >List->size))
            {
                fwprintf(stderr, L"incorrect argument\n");
                for (n = 0; n < count; n++){
                    free(a[n]);
                }
                free(a);
                return;
            }
            ap = parsing_string(a[4], (int)wcslen(a[4]), 0, 0);
            edit_string(List, n, m, ap);
            free(ap);
            ap = NULL;
            Save = 0;
            in = 1;
        }
        if ((!wcscmp(a[0], (wchar_t *)L"insert") && !wcscmp(a[1], (wchar_t *)L"symbol")))
        {
            if (count < 5)
            {
                fwprintf(stderr, L"too few arguments\n");
                return;
            }
            n = wtoi(a[2]);
            m = wtoi(a[3]);
            if (n < 1){
                n = 1;
                m = 0;
            }else{
                if (n > List->size)
                {
                    n = List->size;
                    m = -10;
                }
            }
            ap = parsing_string(a[4], (int)wcslen(a[4]), 0, 0);
            insert_symbol(List, n, m, ap);
            free(ap);
            ap = NULL;
            Save = 0;
            in = 1;
        }

        if (!wcscmp(a[0], (wchar_t *)L"delete") && !wcscmp(a[1], (wchar_t *)L"braces"))
        {
            if (count == 2)
            {
                delete_braces(List, 1, List->size);
            }
            if (count == 3)
            {
                n = wtoi(a[2]);
                if (n < 1 || n > List->size)
                {
                    fwprintf(stderr, L"incorrect argument\n");
                }else
                {
                    delete_braces(List, n, List->size);
                }
            }
            if (count == 4)
            {
                n = wtoi(a[2]);
                m = wtoi(a[3]);
                if (n < 0 || m <0 || n >m)
                {
                    fwprintf(stderr, L"incorrect argument\n");
                }else
                {
                    delete_braces(List, n, m);
                }
            }
            Save = 0;
            in = 1;
        }

        if ((!wcscmp(a[0], (wchar_t *)L"delete") && !wcscmp(a[1], (wchar_t *)L"range")))
        {
            if (count < 3)
            {
                fwprintf(stderr, L"too few arguments\n");
                return;
            }
            if (count == 3)
            {
                n = wtoi(a[2]);
                m = List->size;
            }
            if (count >= 4)
            {
                n = wtoi(a[2]);
                m = wtoi(a[3]);
            }
            if (n <= 0 || n > List->size || n > m || m <= 0)
            {
                fwprintf(stderr, L"incorrect argument\n");
                for (n = 0; n < count; n++)
                {
                    free(a[n]);
                }
                free(a);
                return;
            }
            if (m > List->size){
                m = List->size;
            }
            delete_ranges(List, n, m);
            Save = 0;
            in = 1;
        }

        if ((!wcscmp(a[0], (wchar_t *)L"set") && !wcscmp(a[1], (wchar_t *)L"name")))
        {
            if (count != 3)
            {
                fwprintf(stderr, L"too few arguments\n");
                for (n = 0; n < count; n++)
                {
                    free(a[n]);
                }
                free(a);
                return;
            }
            if (a[2][0] != L'\"' || a[2][wcslen(a[2]) - 1] != L'\"')
            {
                fwprintf(stderr, L"incorrrect argument\n");
                for (n = 0; n < count; n++)
                {
                    free(a[n]);
                }
                free(a);
                return;
            }
            ap = parsing_string(a[2], (int)wcslen(a[2]), 1, 0);
            set_name(ap);
            free(ap);
            ap = NULL;
            in = 1;
        }
    }

    if (!wcscmp(a[0], (wchar_t *)L"read"))
    {
       if (count != 2)
       {
           fwprintf(stderr, L"too few arguments\n");
           for (n = 0; n < count; n++)
           {
               free(a[n]);
           }
           free(a);
           return;
       }
        if (a[1][0] != L'\"' || a[1][wcslen(a[1]) - 1] != L'\"')
        {
            fwprintf(stderr, L"incorrect argument\n");
            for (n = 0; n < count; n++)
            {
                free(a[n]);
            }
            free(a);
            return;
        }
       if (!(ap = (wchar_t*)calloc(wcslen(a[1]), sizeof(wchar_t))))
       {
           fwprintf(stderr, L"can't allocate memory");
           for (n = 0; n < count; n++)
           {
               free(a[n]);
           }
           free(a);
           return;
       }
       wmemcpy(ap, a[1] + 1, wcslen(a[1]) - 2);
        ap[wcslen(a[1]) - 1] = L'\0';
       Read(List, ap);
       free(ap);
        ap = NULL;
        in = 1;
    }
    if (!wcscmp(a[0], (wchar_t *)L"open"))
    {
        if (count != 2){
            fwprintf(stderr, L"too few arguments\n");
            for (n = 0; n < count; n++)
            {
                free(a[n]);
            }
            free(a);
            return;
        }
        if (a[1][0] != L'\"' || a[1][wcslen(a[1]) - 1] != L'\"')
        {
            fwprintf(stderr, L"incorrrect argument\n");
            for (n = 0; n < count; n++)
            {
                free(a[n]);
            }
            free(a);
            return;
        }

        if (!(ap = (wchar_t*)calloc(wcslen(a[1]), sizeof(wchar_t))))
        {
            fwprintf(stderr, L"can't allocate memory");
            for (n = 0; n < count; n++)
            {
                free(a[n]);
            }
            free(a);
            return;
        }
        wmemcpy(ap, a[1] + 1, wcslen(a[1]) - 2);
        ap[wcslen(a[1]) - 1] = L'\0';
        Open(List, ap);
        free(ap);
        ap = NULL;
        in = 1;
    }
    if (!wcscmp(a[0], (wchar_t *)L"write"))
    {
        if (count == 1)
        {
            Write(List, NULL);
        } else
        {
            if (count != 2)
            {
                fwprintf(stderr, L"too few arguments\n");
                for (n = 0; n < count; n++)
                {
                    free(a[n]);
                }
                free(a);
                return;
            }
            if (a[1][0] != L'\"' || a[1][wcslen(a[1]) - 1] != L'\"'){
                fwprintf(stderr, L"incorrrect argument\n");
                for (n = 0; n < count; n++){
                    free(a[n]);
                }
                free(a);
                return;
            }
            if (!(ap = (wchar_t*)calloc(wcslen(a[1]), sizeof(wchar_t))))
            {
                fwprintf(stderr, L"can't allocate memory");
                for (n = 0; n < count; n++)
                {
                    free(a[n]);
                }
                free(a);
                return;
            }
            wmemcpy(ap, a[1] + 1, wcslen(a[1]) - 2);
            ap[wcslen(a[1]) - 1] = L'\0';
            Write(List, ap);
            free(ap);
            ap = NULL;
            in = 1;
        }
        in = 1;
    }

    if ((!wcscmp(a[0], (wchar_t *)L"exit")))
    {
        if (count == 2)
        {
            if ((!wcscmp(a[1], (wchar_t *)L"force")))
            {
                Ex = 1;
            } else
            {
                fwprintf(stderr, L"incorrrect command\n");
                for (n = 0; n < count; n++)
                {
                    free(a[n]);
                }
                free(a);
                return;
            }
        }else
        {
            if (count != 1)
            {
                fwprintf(stderr, L"too few arguments\n");
                for (n = 0; n < count; n++)
                {
                    free(a[n]);
                }
                free(a);
                return;
            }
            if (Save == 0)
            {
                fwprintf(stderr, L"text isn't saved\n");
            }else
            {
                Ex = 1;
            }
        }
        in = 1;
    }
    if ((!wcscmp(a[0], (wchar_t *)L"help")))
    {
        if (count > 1)
        {
            fwprintf(stderr, L"too many arguments\n");
            for (n = 0; n < count; n++)
            {
                free(a[n]);
            }
            free(a);
            return;
        }
        Help();
        in = 1;
    }
    for (n = 0; n < count; n++)
    {
        free(a[n]);
    }
    free(a);
    if (in == 0)
    {
        wprintf(L"\nnot find this command\n");
    }
}


int main (int argc, char **argv)
{
    wchar_t * c = NULL;
    struct DList* List = NULL;
    int del = 0;
    setlocale(LC_ALL, "");
    if (argc == 1)
    {
        ass_file = NULL;
    }else
    {
        if (argc == 2)
        {
            if (!(ass_file = fopen(argv[1], "r+")))
            {
                wprintf(L"%ls\n", L"can't open file on Reading\n");
            }
        }else
        {
            wprintf(L"%ls\n", L"too many arguments\n");
            exit(34);
        }
    }
    List = (struct DList*) malloc(sizeof(struct DList));
    List->size = 0;
    List->head = List->tail = NULL;
    if (ass_file != NULL){
        file_to_text(&c, List, ass_file);
        fclose(ass_file);
        ass_file = NULL;
    }
    if (argc == 2 && !(ass_file = fopen(argv[1], "w+")))
    {
        wprintf(L"%ls\n", L"can't open file\n");
    }
    while (!Ex)
    {
        if (del == 5){
            break;
        }
        if (isatty(fileno(stdin)))
        {
            wprintf(L"Editor: ");
        }else{
            wprintf(L"");
        }
        del = read_command(&c);
        parsing_arguments(c, List);
        free(c);
    }

    delete_DLink(&List);
    if (ass_file !=  NULL)
    {
        fclose(ass_file);
        ass_file = NULL;
    }
    return 0;
}