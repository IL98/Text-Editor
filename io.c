#include "lib.h"
#include "list.h"
#include "edit.h"
#include "print.h"
#include "parse.h"

extern FILE * ass_file;
extern int settabwith;
extern int setnumbers;
extern int Save;
extern int setwarp;

/*
 * печать списка без каких либо режимов в файл f
 * Аргументы: список list, указатель на FILE f
 */
void printDblLinkedList(struct DList *list, FILE * f)
{
    struct Node *tmp = list->head;
    while (tmp)
    {
        fwprintf(f, L"%ls", tmp->value);
        tmp = tmp->next;
    }
}


/*
 * добавляет символм h в строку в позицию cur
 * Аргуметы:указатель на строку  с, символ  h, позиция cur, указатель на размер строки size
 */
void add_sym(wchar_t ** c, wchar_t h, int cur, int * size)
{
    if (cur >= (*size - 2))
    {
        *c = (wchar_t*)realloc(*c, ((*size)*2)*sizeof(wchar_t));
        (*size) *= 2;
    }
    (*c)[cur] = h;
    (*c)[cur + 1] = L'\0';
}



/*
 * считывает строку-будущую команду с аргументами
 * Аргументы:указатель на строку с
 * выводит 1 если все верно, -1 в противном случае
 */
int read_command(wchar_t ** c)
{
    int flag = 0;/*1, если находимся в трех двойных кавычек*/
    int flag_com = 0;/*если мы  в комменатриях*/
    int flag_one = 0;/*1 , если находимся в одной паре двойных кавычек*/
    int size = 32;
    int i = 0;
    int h = 0;
    int cur = -1;
    if (!(*c = (wchar_t*)calloc(32, sizeof(wchar_t))))
    {
        fwprintf(stderr, L"can't aloccate memory\n");
        return -1;/*-----------------------здесь надо почистить всю память*/
    }
    (*c)[0] = L'\0';
    while ((h = fgetwc(stdin)))
    {
        if (h == EOF){
            return 5;
        }
        if (flag_com && !flag_one)
        {
            if (h == L'\n')
            {
                flag_com = 0;
            }else
            {
                continue;
            }
        }
        if (h == L'#' && !flag_one)
        {
            flag_com = 1;
            continue;
        }
        cur++;
        add_sym(c, h, cur, &size);
        if (!flag)
        {
            if (h == L'\n')
            {
                wchar_t  ** arr = _parse(*c);
                int count = word_count(*c);
                if (count < 4 && count > 1)
                {
                    if (!(wcscmp(arr[0],(wchar_t *)L"insert")) && !(wcscmp(arr[1],(wchar_t *)L"after")))
                    {
                        if (arr[count - 1][0] == L'\"' && arr[count - 1][wcslen(arr[count - 1]) - 1] != L'\"')
                        {
                            for (i = 0; i < count; i++)
                            {
                                free(arr[i]);
                            }
                            free(arr);
                            return 1;
                        }
                        if (arr[count - 1][0] != L'\"' && arr[count - 1][wcslen(arr[count - 1]) - 1] == L'\"')
                        {
                            for (i = 0; i < count; i++)
                            {
                                free(arr[i]);
                            }
                            free(arr);
                            return 1;
                        }
                        if ((arr[count - 1][0] != L'\"' || arr[count - 1][wcslen(arr[count - 1]) - 1] != L'\"'))
                        {
                            for (i = 0; i < count; i++)
                            {
                                free(arr[i]);
                            }
                            free(arr);
                        }else
                        {
                            for (i = 0; i < count; i++)
                            {
                                free(arr[i]);
                            }
                            free(arr);
                            return 1;
                        }

                    }else
                    {
                        for (i = 0; i < count; i++)
                        {
                            free(arr[i]);
                        }
                        free(arr);
                        return 1;
                    }
                }else
                {
                    for (i = 0; i < count; i++)
                    {
                        free(arr[i]);
                    }
                    free(arr);
                    return 1;
                }
            }else
            {
                if (h == L'\"')
                {
                    cur++;
                    flag_one = 1 - flag_one;
                    if((h = fgetwc(stdin)) == L'\"')
                    {
                        add_sym(c, h, cur, &size);
                        cur++;
                        if((h = fgetwc(stdin)) == L'\"')
                        {
                            add_sym(c, h, cur, &size);
                            flag = 1;
                        }else
                        {
                            add_sym(c, h, cur, &size);
                            if (h == L'\n'){
                                return 1;
                            }
                        }
                    }else
                    {
                        add_sym(c, h, cur, &size);
                        if (h == L'\n')
                        {
                            return 1;
                        }
                    }
                }
            }
        }else
        {
            if (h == L'\"')
            {
                cur++;
                if ((h = fgetwc(stdin)) == L'\"')
                {
                    add_sym(c, h, cur, &size);
                    cur++;
                    if ((h = fgetwc(stdin)) == L'\"')
                    {
                        add_sym(c, h, cur, &size);
                        cur++;
                        if ((h = fgetwc(stdin)) == L'\n')
                        {
                            add_sym(c, L'\n', cur, &size);
                            return 1;
                        }
                        flag = 0;
                        flag_com = 0;
                        flag_one = 0;
                    } else
                    {
                        add_sym(c, h, cur, &size);
                    }
                } else
                {
                    add_sym(c, h, cur, &size);
                }
            }

        }
    }
    return 1;
}


/*
 * преобразует преобразует текст из файла F в список List с помощью строки
 * Аргументы: указатель на строку с, список List, указатель на FILE F
 * выводит -1 в случае ошибки и 0 в противом случае
 */
int file_to_text(wchar_t ** c, struct DList * List, FILE * F){
    int h = 0;
    int cur = -1;
    int size = 32;
    if (!(*c = (wchar_t*)calloc(32, sizeof(wchar_t))))
    {
        fwprintf(stderr, L"can't aloccate memory\n");
        return -1;
    }
    (*c)[0] = L'\0';
    while ((h = fgetwc(F)) != EOF)
    {
        cur++;
        add_sym(c, h, cur, &size);
    }
    if ((*c)[cur] != L'\n' && cur != -1)
    {
        cur++;
        add_sym(c, L'\n', cur, &size);
    }
    if (cur != -1)
        Add_string(List, (*c), NULL);
    free(*c);
    *c = NULL;
    return 0;
}

/*
 * открывает файл
 * по пути широкой строкой с с режимом с1
 * Аргументы:
 * строка с, строка с1
 * Выводит указатель на FILE открытого файла
 * Выводит NULL  в случае неудачи
 */
FILE * file_wchar_t(wchar_t * c, char * c1){
    FILE * f = NULL;
    char * cur = NULL;
    size_t len = wcslen(c);
    if (!(cur = (char *)malloc((len + 1)*sizeof(wchar_t)))){
        fwprintf(stderr, L"can't allocate memory\n");
        return NULL;
    }
    if (wcstombs(cur, c, len) == -1){
        fwprintf(stderr, L"reading fail\n");
        free(cur);
        return NULL;
    }
    cur[len] = '\0';
    printf("%s\n", cur);
    f = fopen(cur, c1);
    if (f == NULL){
        free(cur);
        return NULL;
    }
    free(cur);
    cur = NULL;
    return f;
}


/*
 * команда Read
 * заменяет текст из List на текст из файла с именем с
 * Аргументы: список List, строка с
 * выводит -1 в случае ошибки, иначе 1
 */
int Read(struct DList * List, wchar_t * c)
{
    FILE * f = NULL;
    wchar_t * s = NULL;
    if (!(f = file_wchar_t(c, "r+")))
    {
        wprintf(L"can't open the file\n");
        return -1;
    }
    delete_ranges(List, 1, List->size);
    if (file_to_text(&s, List, f) == -1)
    {
        fclose(f);
        f = NULL;
        return -1;
    }
    fclose(f);
    f = NULL;
    return 1;
}

/*
 * команда open
 * заменяет текст из List на текст из файла с именем с
 * и заменяет ассоциацию с файла
 * Аргументы:список List, строка с - название файла
 * выводит -1 в случае ошибки, иначе 1
 */
int Open(struct DList * List, wchar_t * c)
{
    wchar_t * s = NULL;
    if (  ass_file != NULL)
    {
        fclose(ass_file);
    }
    ass_file = NULL;
    if (!(ass_file = file_wchar_t(c, "r+")))
    {
        fwprintf(stderr, L"can't open file\n");
        return -1;
    }
    delete_ranges(List, 1, List->size);
    file_to_text(&s, List, ass_file);
    fclose(ass_file);
    ass_file = NULL;
    if (!(ass_file = file_wchar_t(c, "w+")))
    {
        fwprintf(stderr, L"can't open file\n");
        return -1;
    }
    return 1;
}

/*
 * команда write
 * производит печать тескста в файл
 * Аргументы:список List, строка с - название файла
 */
void Write(struct DList * List, wchar_t * c)
{
    FILE * f = NULL;
    if (c == NULL)
    {
        if (ass_file == NULL)
        {
            fwprintf(stderr, L"can't write text in file, associate file or select file\n");
            return;
        }
        printDblLinkedList(List, ass_file);
        return;
    }
    if (!(f = file_wchar_t(c, "w+")))
    {
        wprintf(L"can't open the file\n");
        return;
    }
    printDblLinkedList(List, f);
    if (ass_file == NULL)
    {
        ass_file = f;
    }else
    {
        fclose(f);
    }
    Save = 1;
}


/*
 * команда set name
 * производит ассоциацию с файлом
 * Аргументы: строка с - название файла
 */
void set_name(wchar_t * c)
{
    if (wcslen(c) == 0)
    {
        if (ass_file != NULL)
        {
            fclose(ass_file);
            ass_file = NULL;
            return;
        }else
        {
            fwprintf(stdout, L"file is alredy associated");
            return;
        }
    }
    if (ass_file != NULL)
    {
        fclose(ass_file);
        ass_file = NULL;
    }
    if (!(ass_file = file_wchar_t(c, "w+")))
    {
        fwprintf(stderr, L"can't open file\n");
    }
    return;
}
