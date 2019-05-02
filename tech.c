#include "lib.h"
#include <unistd.h>
#include <termios.h>


/*
 * переделывает широкую строку в число
 * Аргументы:широкая строка
 * вывод - число
 */
int wtoi(wchar_t* crap)
{
    char * t = (char*)calloc(wcslen(crap) + 1, sizeof(char));
    int  n = (int)wcslen(crap);
    int k = 0;
    int i = 0;
    for (i = 0; i < n; i++)
    {
        t[i] = (char)crap[i];
    }
    t[n] = '\0';
    k = atoi(t);
    free(t);
    return k;
}

/*
 * округление числа вверх
 * вывод новое округленное число
 */
int my_ceil(double x)
{
    if ((int)x == x)
        return (int)x;
    else
        return (int)x + 1;

}


/*
 * подсчет целой части десятичного логарифма от целого числа
 * Аргуметы: число number
 * вывод целая часть логаримфма
 */
int my_log10(int number)
{
    int count = 0;
    if ( number == 0 )
    {
        return 1;
    }
    while ( number != 0 )
    {
        number /= 10;
        ++count;
    }
    return count - 1;
}

/*
 * минимум среди целых чисел a и b
 * Аргументы: числа a и b
 * вывод - их минимум
 */
int min(int a, int b)
{
    return (a < b)? a : b;
}


/*
 * максимум среди целых чисел a и b
 * Аргументы: числа a и b
 * вывод - их максимум
 */
int max(int a, int b){
    return (a > b)? a :b;
}


/*
 * функция считыания с входного потока символа сразу без ожидания на символ переноса строки
 * выводит этото символ
 */
int mygetch(){
    struct termios oldt,
            newt;
    int ch;
    tcgetattr( STDIN_FILENO, &oldt );
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newt );
    ch = fgetwc(stdin);
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
    return ch;
}