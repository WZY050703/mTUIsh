#pragma once

#include <stdio.h>
#include <string>
#include "../ui/ui.h"
#include "../other/toolFuncs.h"
#include "beforeBash.h"

std::string get_command(bool isECHO) // 输入
{
    saveCPoint();           // 保存初始光标位置
    int pi = 0;             // 目前指针在字符串的哪个位置
    std::string bstr, hstr; // 字符串buffer，分光标前后
    if (isECHO)
        setNonBlockingInput(STDIN_FILENO, (ICANON | ECHO)); // 禁用规范模式和回显
    else
        setNonBlockingInput(STDIN_FILENO, (ICANON)); // 禁用规范模式
    HideC();                                         // 隐藏光标
    char cin;
    int clen = 0;
    printf("\033[7m "); // 初始光标
    while ((cin = getchar()) != '\n')
    {
        clen = HowLongUtf8(&cin);
        if (IsASCIIcanSeeCharacter(cin) || clen > 1) // 可见字符
        {
            for (int i = 0; i < clen; i++) // 读取多宽字符
            {
                bstr += cin;
                pi++;
                if (i < clen - 1) // 最后一位不读
                    cin = getchar();
            }
        }
        else if (cin == '\033') // 033转序字符
        {
            if (getchar() == '[')
            {
                cin = getchar();
                if (cin == 'D' && pi > 0) // 左移动
                {
                    std::string tstr = bstr + hstr; // 重新分割
                    {                               // 找上一个字符的开头位置
                        const char *tcstr = bstr.c_str();
                        int last = 0;
                        for (int ti = 0; ti < bstr.size();)
                        {
                            last = ti;
                            ti += HowLongUtf8(tcstr + ti);
                        }
                        pi = last;
                    }
                    hstr = tstr.substr(pi);
                    bstr = tstr.erase(pi);
                }
                else if (cin == 'C' && pi < bstr.size() + hstr.size()) // 右移动
                {
                    std::string tstr = bstr + hstr;
                    pi += HowLongUtf8(hstr.c_str());
                    hstr = tstr.substr(pi);
                    bstr = tstr.erase(pi);
                }
                else if (cin == 'H') // home
                {
                    hstr = bstr + hstr; // 注意顺序
                    bstr.clear();
                    pi = 0;
                }
                else if (cin == 'F') // end
                {
                    bstr = bstr + hstr;
                    hstr.clear();
                    pi = bstr.size();
                }
                else if (cin == '3' && getchar() == '~') // del
                {                                        // 由于保证了指针一定在字符开头，直接删除就好
                    hstr.erase(0, HowLongUtf8(hstr.c_str()));
                }
            }
        }
        else if (cin == 127) // 删除,backspace
        {
            if (pi > 0)
            {
                { // 找上一个字符的开头位置
                    const char *tcstr = bstr.c_str();
                    int last = 0;
                    for (int ti = 0; ti < bstr.size();)
                    {
                        last = ti;
                        ti += HowLongUtf8(tcstr + ti);
                    }
                    pi = last;
                }
                bstr = bstr.erase(pi);
            }
        }
        else if (cin == 9) // tab 转换成空格
        {
            for (int i = 0; i < TABLEN; i++)
                bstr += ' ';
            pi += TABLEN;
        }
        // 输出回显
        const char *coutput = (bstr + hstr).c_str();
        const int len = (bstr + hstr).size();
        clearline(); // 清空行并到行首
        moveSavedCPoint();
        for (int i = 0; i < len; i++)
        {
            if (i == pi)
                printf("\033[7m");
            else
                printf("\033[0m");
            int strlen = HowLongUtf8(coutput + i);
            for (int k = 0; k < strlen; k++)
                printf("%c", coutput[i + k]);
            i += strlen - 1;
        }
        if (pi == len)
            printf("\033[7m ");
    }
    printf("\033[0m"); // 恢复初始状态
    ReTerm(STDIN_FILENO);
    return bstr + hstr;
}