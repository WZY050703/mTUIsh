#pragma once

#include <string>
#include "ui_context.h"
#include "../dataClass/MPoint.h"
#include "../dataClass/mlink.h"

// define
class block;
class canvas;
class camera;

// make

class block
{
private:
    MPoint<int> BlockPoint; // 锚点在画布上的绝对位置
    MPoint<int> RootPoint;  // 锚点的相对位置
    MPoint<int> BlockHW;    // block的大小
public:
    MPoint<int> TxtStatus;     // 文本色彩和其状态
    int TxtBlackgroundColor;   // 文字背景色
    int BlockBlackgroundColor; // 块背景色
    std::string text;          // 内容

    block(MPoint<int> HW);
    void SetTxtStle(int stle, int color);
    void SetTxtBlackgroundColor(int color);
    void SetBlockBlackgroundColor(int color);
    void SetSize(MPoint<int> HW);
    void SetPosition(MPoint<int> point);
    void SetRootP(MPoint<int> point);
    void SetText(std::string text);
    MPoint<int> BlockHideWide(); // 返回block大小
    MPoint<int> PrintPoint();    // 返回左上角在画布上的位置
    MPoint<int> EndPoint();      // 返回右下角在画布上的位置
};

block::block(MPoint<int> HW)
{
    this->BlockHW = HW;
    this->BlockPoint = MPoint(0, 0);
    this->RootPoint = MPoint(0, 0);
    this->TxtStatus = ui_context.TXT_status;
    this->TxtBlackgroundColor = Black;
    this->BlockBlackgroundColor = Black;
}

void block::SetTxtStle(int stle, int color)
{
    this->TxtStatus = MPoint(stle, color);
    return;
}

void block::SetTxtBlackgroundColor(int color)
{
    this->TxtBlackgroundColor = color;
    return;
}

void block::SetBlockBlackgroundColor(int color)
{
    this->BlockBlackgroundColor = color;
    return;
}

void block::SetSize(MPoint<int> HW)
{
    this->BlockHW = HW;
}

void block::SetPosition(MPoint<int> point)
{
    this->BlockPoint = point;
}

void block::SetRootP(MPoint<int> point)
{
    this->RootPoint = point;
}

void block::SetText(std::string text)
{
    this->text = text;
    return;
}

MPoint<int> block::BlockHideWide()
{
    return this->BlockHW;
}

MPoint<int> block::PrintPoint()
{
    return this->BlockPoint - this->RootPoint;
}

MPoint<int> block::EndPoint()
{
    return this->PrintPoint() + this->BlockHW - MPoint(1, 1);
}

// canvas
class canvas
{
private:
    int level;         // 层级
    mlink<block> HEAD; // 块链表头
public:
    canvas();
    void SetLevel(int level);
    void AddBlock(block *obj);
    Node<block> *BlockHead();
    int TheLevel();
};

canvas::canvas()
{
    this->level = 0;
}

void canvas::SetLevel(int level)
{
    this->level = level;
    return;
}

void canvas::AddBlock(block *obj)
{
    this->HEAD.AddNode(obj, 0);
    return;
}

Node<block> *canvas::BlockHead()
{
    return this->HEAD.TheHead();
}

int canvas::TheLevel()
{
    return this->level;
}

// camera
class camera
{
private:
    mlink<canvas> HEAD;   // 画布链表头
    MPoint<int> CameraXY; // 当前屏幕相机位置
public:
    camera();
    void AddCanvas(canvas *obj);
    void SetCameraXY(MPoint<int> XY);
    void CatchScreen();
    Node<canvas> *CanvasHead();
};

camera::camera()
{
    this->CameraXY = ui_context.CameraXY;
    return;
}

void camera::AddCanvas(canvas *obj)
{
    this->HEAD.AddNode(obj, obj->TheLevel());
    return;
}

void camera::SetCameraXY(MPoint<int> XY)
{
    this->CameraXY = XY;
    ui_context.CameraXY = XY;
    return;
}

void camera::CatchScreen() // 输出
{
    HideC(); // 隐藏光标
    clearscreen();
    for (Node<canvas> *pc = this->HEAD.TheHead(); pc != nullptr; pc = pc->next)
    {
        for (Node<block> *p = pc->obj->BlockHead(); p != nullptr; p = p->next)
        {
            MPoint<int> PP, PB, PE; // 输出位置，开始位置，结束位置(右下角)
            if (pc->Key <= 0)       // 计算block左上角的输出位置
            {
                PB = p->obj->PrintPoint() - this->CameraXY + MPoint(1, 1); // 小于等于0的层可移动
                PE = p->obj->EndPoint() - this->CameraXY + MPoint(1, 1);   // 结束位置
            }
            else
            {
                PB = p->obj->PrintPoint() + MPoint(1, 1); // 大于0的层不可移动
                PE = p->obj->EndPoint() + MPoint(1, 1);   // 结束位置
            }
            PP = PB; // 指针赋值

            for (PP.x; PP.x <= PE.x; PP.x++) // 绘制块背景色
            {
                PP.y = PB.y;
                for (PP.y; PP.y <= PE.y; PP.y++)
                    if (ui_context.ScreenXY[PP]) // 在屏幕内
                    {
                        printf("\033[%d;%dH\033[%dm ", PP.x, PP.y, Bakegrounds + p->obj->BlockBlackgroundColor);
                        ui_context.C_point = PP; // 更新结构体中光标位置记录
#ifdef DEBUG
                        fflush(stdout);
#endif
                    }
            }

            PP = PB; // 指针赋值
            // 防止regular把所有都清除了
            printf("\033[%d;%dm", p->obj->TxtStatus.x, Texts + p->obj->TxtStatus.y);                         // 文字颜色和样式
            printf("\033[%dm", Bakegrounds + (p->obj->TxtBlackgroundColor | p->obj->BlockBlackgroundColor)); // 文字背景色(有混色

            const char *str = p->obj->text.c_str(); // 转换为c字符串输出
            for (int i = 0; str[i] != '\0'; i++)
            {
                int charlen = HowLongUtf8(&str[i]);                                    // 字符宽
                if ((charlen == 1 && PP.y > PE.y) || (charlen > 1 && PP.y + 1 > PE.y)) // 超block的界限了
                {
                    if (PP.x < PE.x) // 自动换行
                        PP = MPoint(PP.x + 1, PB.y);
                    else // 到末尾还没完，输出省略号
                    {
                        if (PE.y - PB.y + 1 >= 3 && ui_context.ScreenXY[PE - MPoint(0, 3 - 1)]) // 在屏幕内
                            printf("\033[%d;%dH...", PE.x, PE.y - 3 + 1);                       // 空间够，输出完整的三点
                        else
                        { // 空间不够，能输出多少是多少
                            for (int iy = PB.y; iy <= PE.y; iy++)
                                if (ui_context.ScreenXY[MPoint(PE.x, iy)]) // 在屏幕内
                                    printf("\033[%d;%dH.", PE.x, iy);
                        }
                        ui_context.C_point = PE; // 更新结构体中光标位置记录
#ifdef DEBUG
                        fflush(stdout);
#endif
                        break;
                    }
                }

                if (charlen == 1) // 单宽字符
                {
                    if (ui_context.ScreenXY[PP]) // 在屏幕内
                    {
                        printf("\033[%d;%dH", PP.x, PP.y); // 定位
                        printf("%c", str[i]);              // 输出
                    }
#ifdef DEBUG
                    fflush(stdout);
#endif
                }
                else if (charlen > 1) // 多宽字符
                {
                    if (ui_context.ScreenXY[PP] && ui_context.ScreenXY[PP + MPoint(0, 1)]) // 头尾都在屏幕内
                    {
                        printf("\033[%d;%dH", PP.x, PP.y); // 定位
                        for (int k = 0; k < charlen; k++)
                            printf("%c", str[i + k]); // 输出
                    }
                    i += charlen - 1;       // 字符定位(预留for的+1)
                    PP = PP + MPoint(0, 1); // 输出指针定位，单独加一
#ifdef DEBUG
                    fflush(stdout);
#endif
                }
                PP = PP + MPoint(0, 1);
                ui_context.C_point = PP; // 更新结构体中光标位置记录
            }
        }
    }
    fflush(stdout);    // 防连续动画输出不完全
    printf("\033[0m"); // 恢复文字样式
    return;
}

Node<canvas> *camera::CanvasHead()
{
    return this->HEAD.TheHead();
}