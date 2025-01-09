#pragma once

#include <sys/types.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>

#include "../dataClass/MPoint.h"

#define TABLEN 4
#define IsASCIIcanSeeCharacter(c) (32 <= c && c <= 126)

MPoint<int> WinSize() // 返回窗口大小数据
{
    struct winsize size;
    ioctl(STDIN_FILENO, TIOCGWINSZ, &size);
    return MPoint<int>(size.ws_row, size.ws_col);
}

int HowLongUtf8(const char *str) // 判断这是多长的UTF-8编码
{
    if (str[0] == '\0') // 防空
        return 0;
    int i = 7;
    for (i = 7; i >= 0; i--)
    {
        unsigned int tmp = 1;
        tmp <<= i;
        tmp = tmp & str[0];
        tmp >>= i;
        if (tmp == 0)
            break;
    }
    if (7 - i == 0)
        return 1;
    return 7 - i;
}