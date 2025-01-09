#pragma once

#include <unistd.h>
#include <termios.h>

struct termios __StdInTerm;

void setNonBlockingInput(int fd, unsigned int CloseMask) // 修改标准输入流为指定状态
{
    tcgetattr(fd, &__StdInTerm); // 保存旧有的状态
    struct termios term;
    tcgetattr(fd, &term);
    term.c_lflag &= ~CloseMask; // 禁用Mask
    tcsetattr(fd, TCSANOW, &term);
}

void ReTerm(int fd) // 恢复标准输入流
{
    tcsetattr(fd, TCSANOW, &__StdInTerm);
}