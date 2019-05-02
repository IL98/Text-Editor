#include "lib.h"
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "list.h"
#include "tech.h"
#include "io.h"

extern FILE * ass_file;
extern int settabwith;
extern int setnumbers;
extern int Save;
extern int setwarp;


/*
 * Считает предположительную длину строки c
 * при замене таббов на определенное число пробелов settab
 * Выводит длину
 * Аргументы: строка c, число пробелов settab
 */
double tab_string (wchar_t * c, int settab)
{
    int i = 0;
    int n = (int)wcslen(c);
    double l = 0;
    for (i = 0; i < n; i++)
    {
        if (c[i] != L'\t')
            l++;
        else l += settab;
    }
    return l;
}

/*
 * Осуществляет вывод строки в режиме обрезания c начиная с некоторого элемента с номером begin,
 * учитывая сколько надо вывести с помощью ширины width и отступа от левого края otstup
 * Аргументы: строка с, начиная с элемента с номером begin, число пробелов вместо таба settabwith
 * ширина width, сколько отступать - otstup
 * выводит -1, если ошибка
 * если все нормально, то 0
 */
int Print_string_obrez(wchar_t * c, int begin, int settabwith, int width, int otstup)
{
    if (begin < wcslen(c))
    {
        int i = 0;
        int j;
        int n = (int)wcslen(c);
        int cur_width = otstup;
        int l = -1;
        for (i = 0; i < n; i++)
        {
            if (cur_width < width)
            {
                if (c[i] != L'\t')
                {
                    l++;
                    if (l >= begin)
                    {
                        if (fputwc(c[i], stdout) == EOF)
                        {
                            fwprintf(stderr, L"ошибка записи символов\n");
                            return -1;
                        }
                        cur_width++;
                    }
                } else
                {
                    for (j = 0; j < settabwith; j++)
                    {
                        l++;
                        if (l >= begin)
                        {
                            if (fputwc(L' ', stdout) == EOF)
                            {
                                fwprintf(stderr, L"ошибка записи символов\n");
                                return -1;
                            }
                            cur_width++;
                        }
                    }
                }
            } else
            {
                if (fputwc(L'\n', stdout) == EOF)
                {
                    fwprintf(stderr, L"ошибка записи символов\n");
                    return -1;
                }
                return 0;
            }
        }
    }else
    {
        if (fputwc(L'\n', stdout) == EOF)
        {
            fwprintf(stderr, L"ошибка записи символов\n");
            return -1;
        }
    }
    return 0;
}


/*
 * выводит строку в режиме загибания, учитывая как надо выводить
 * с помощью ширины width и отступа от левого края otstup
 * Аргументы: строка с, число пробелов вместо таба settabwith
 * ширина width, сколько отступать - otstup
 * выводит -1, если ошибка
 * если все нормально, то 0
 */
int Print_string_sgib(wchar_t * c, int settabwith, int width, int otstup)
{
    int i;
    int n = (int)wcslen(c);
    int cur_width = otstup;
    int k = 0;
    for (i = 0; i < n - 1; i++)
    {
        if (((cur_width + 1) % width) > 0 )
        {
            if (c[i] == L'\t')
            {
                int j;
                for (j = 0; j < settabwith; j++)
                {
                    if ((cur_width +1) % width > 0)
                    {
                        if (fputwc(L' ', stdout) == EOF)
                        {
                            fwprintf(stderr, L"ошибка записи символов\n");
                            return -1;
                        }
                        cur_width++;
                    }else
                    {
                        if (fputwc(L'\n', stdout) == EOF)
                        {
                            fwprintf(stderr, L"ошибка записи символов\n");
                            return -1;
                        }
                        k = 0;
                        for (k = 0; k < otstup; k++)
                        {
                            if (fputwc(L' ', stdout) == EOF)
                            {
                                fwprintf(stderr, L"ошибка записи символов\n");
                                return -1;
                            }
                        }
                        if (fputwc(L' ', stdout) == EOF)
                        {
                            fwprintf(stderr, L"ошибка записи символов\n");
                            return -1;
                        }
                        cur_width = otstup + 1;
                    }
                }
            } else
            {
                if (fputwc(c[i], stdout) == EOF)
                {
                    fwprintf(stderr, L"ошибка записи символов\n");
                    return -1;
                }
                cur_width++;
            }
        } else
        {
            if (fputwc(L'\n', stdout) == EOF)
            {
                fwprintf(stderr, L"ошибка записи символов\n");
                return -1;
            }
            k = 0;
            for (k = 0; k < otstup; k++)
            {
                if (fputwc(L' ', stdout) == EOF)
                {
                    fwprintf(stderr, L"ошибка записи символов\n");
                    return -1;
                }
            }
            cur_width = otstup + 1;
            if (fputwc(c[i], stdout) == EOF)
            {
                fwprintf(stderr, L"ошибка записи символов\n");
                return -1;
            }
        }
    }
    if (!((cur_width + 1) % width))
    {
        if (fputwc(L'\n', stdout) == EOF)
        {
            fwprintf(stderr, L"ошибка записи символов\n");
            return -1;
        }
        k = 0;
        for (k = 0; k < otstup; k++)
        {
            if (fputwc(L' ', stdout) == EOF)
            {
                fwprintf(stderr, L"ошибка записи символов\n");
                return -1;
            }
        }
    }
    if (fputwc(L'|', stdout) == EOF)
    {
        fwprintf(stderr, L"ошибка записи символов\n");
        return -1;
    }
    if (fputwc(L'\n', stdout) == EOF)
    {
        fwprintf(stderr, L"ошибка записи символов\n");
        return -1;
    }
    return 0;
}


/*
 * Команда print range
 * выводит строки из списка List, с учетом режимов включения/выключения номеров,
 * числом таббов, обрезаются ли строки
 * в диапазоне от строки с номером begin до end
 * Аргументы:
 * список List, числа задающее режимы - setnumbers, settabwidth, setwarp, диапазон - числа begin,  end
 */
void Print_Dlist(struct DList *List, int setnumbers, int settabwith, int setwarp,  int begin, int end)
{

    int flag = 0;
    int max_length = 0;
    int fd = 0;
    int cur_height = 0;
    int cur_position = 0;
    int max_id = 0;
    struct Node* p = List->head;
    int index = 0;
    struct winsize g;
    for (index = 1; index < begin; index++)
    {
        p = p->next;
    }
    index = begin;
    if (isatty(fileno(stdout)) && isatty(fileno(stdin)))
    {
        int i_id;
        struct Node* begin_elem = p;
        int delta;
        if (setwarp)
        {
            delta = 2;
            begin_elem = NULL;
            i_id = -1;
        }else
        {
            delta = 1;
            begin_elem = p;
            i_id = index;
        }
        flag = 0;
        max_length = 0;
        fd = open("/dev/ttyS0", O_RDONLY);
        cur_position = 0;
        ioctl(1, TIOCGWINSZ, &g);
        if (setnumbers)
        {
            max_id = my_log10(end) + 2;
        } else
        {
            max_id = -1;
        }
        cur_height = 0;
        while ((p && (index <= end)) || (begin_elem && (i_id <= end)) )
        {
            double x;
            if (setwarp)
            {
                if (setnumbers)
                    x = (tab_string(p->value, settabwith) + 1) / (g.ws_col - max_id);
                else
                    x = (tab_string(p->value, settabwith) + 1) / (g.ws_col);
            } else
                x = 1;
            if ((my_ceil(x) + cur_height <= g.ws_row - delta))
            {
                if (p && (index <= end))
                {
                    if (setnumbers)
                    {
                        int cur_ = max_id - my_log10(index) - 2;
                        int k = 0;
                        for (k = 0; k < cur_; k++)
                        {
                            if (fputwc(L' ', stdout) == EOF)
                            {
                                fwprintf(stderr, L"ошибка записи символов\n");
                                return;
                            }
                        }
                        fwprintf(stdout, L"%d:", index);
                        if (setwarp)
                        {
                            if (Print_string_sgib(p->value, settabwith, g.ws_col, max_id) == -1)
                            {
                                return;
                            }
                        } else
                        {
                            if (Print_string_obrez(p->value, cur_position, settabwith, g.ws_col, max_id) == -1)
                            {
                                return;
                            }
                            max_length = max(max_length, (int) tab_string(p->value, settabwith) - cur_position);
                        }
                    } else
                    {
                        if (setwarp)
                        {
                            if (Print_string_sgib(p->value, settabwith, g.ws_col, 0) == -1)
                            {
                                return;
                            }
                        } else
                        {
                            if (Print_string_obrez(p->value, cur_position, settabwith, g.ws_col, 0) == -1)
                            {
                                return;
                            }
                            max_length = max(max_length, (int) tab_string(p->value, settabwith) - cur_position + 1);
                        }
                    }
                    cur_height += my_ceil(x);
                    p = p->next;
                    index++;
                } else
                {
                    while (cur_height < g.ws_row - 1)
                    {
                        if (fputwc(L'\n', stdout) == EOF)
                        {
                            fwprintf(stderr, L"ошибка записи символов\n");
                            return;
                        }
                        cur_height++;
                    }
                }
            } else
            {
                int h;
                if ((h = mygetch()) == 'q')
                {
                    break;
                } else {
                    while (h != ' ')
                    {
                        if (!setwarp)
                        {
                            if (h == 27)
                            {
                                if ((h = mygetch()) == 91)
                                {
                                    if ((h = mygetch()) == 'D')
                                    {
                                        if (cur_position)
                                        {
                                            h = L' ';
                                            flag = 1;
                                            p = begin_elem;
                                            index = i_id;
                                            cur_position--;
                                            cur_height = 0;
                                            max_length = 0;
                                            break;
                                        }
                                    }
                                    if (h == 'C')
                                    {
                                        if (max_length + max_id - 1 > g.ws_col)
                                        {
                                            h = L' ';
                                            flag = 2;
                                            p = begin_elem;
                                            index = i_id;
                                            cur_position++;
                                            cur_height = 0;
                                            max_length = 0;
                                            break;
                                        }
                                    }
                                }
                            } else {
                                if (h == 'q')
                                {
                                    break;
                                }
                                h = mygetch();
                            }
                        } else
                        {
                            h = mygetch();
                            if (h == 'q')
                            {
                                break;
                            }
                        }
                    }
                    if (!setwarp)
                    {
                        if (h == 'q')
                        {
                            break;
                        }
                        if (h == L' ' || flag == 0)
                        {
                            cur_height = 0;
                            begin_elem = p;
                            i_id = index;
                            max_length = 0;
                        }
                        flag = 0;
                    } else
                    {
                        if (h == 'q')
                        {
                            break;
                        }
                        cur_height = 0;
                    }
                }
            }
        }
        close(fd);
    }else
    {
        while (p && (index <= end))
        {
            fwprintf(stdout, L"%ls", p->value);
            p = p->next;
            index++;
        }
    }
}


/*
 * команда Help
 * выводит помощь по командам
 */
void Help()
{
    FILE * f = NULL;
    wchar_t * s = NULL;
    struct DList * List = NULL;
    if (!(f = fopen("Help", "r+")))
    {
        fwprintf(stderr, L"can't open file Help\n");
        return;
    }
    List = (struct DList*) malloc(sizeof(struct DList));
    List->size = 0;
    List->head = List->tail = NULL;
    file_to_text(&s, List, f);
    fclose(f);
    f = NULL;
    Print_Dlist(List, setnumbers, settabwith, setwarp, 1, List->size);
    delete_DLink(&List);
}