#pragma once

#include "../dataClass/MPoint.h"
#include "../other/toolFuncs.h"

#define DEBUG 1

// ansi escape code
#define moveXY(x, y) printf("\033[%d;%dH", x, y) // 1开始
#define moveUP(r) printf("\033[%dA", r)
#define moveDW(r) printf("\033[%dB", r)
#define moveR(r) printf("\033[%dC", r)
#define moveL(r) printf("\033[%dD", r)
#define saveCPoint() printf("\033[s")
#define moveSavedCPoint() printf("\033[u")

#define clearscreen() printf("\033[2J")
#define clearline() printf("\033[2K")
#define HideC() printf("\033[?25l")
#define ShowC() printf("\033[?25h")

#define Regular 0 // 默认，慎用，会清除所有设置
// text stles
#define tBold 1      // 粗体
#define tGrey 2      // 灰化
#define tItalic 3    // 斜体
#define tUnderline 4 // 下划线
#define tFlash 5     // 闪烁(vsc内置终端无法表示)
#define tInverted 7  // 反色
#define tInvisible 8 // 隐形
#define tDelline 9   // 删除键
// color
#define Black 0
#define Red 1
#define Green 2
#define Yellow 3
#define Blue 4
#define Magenta 5 // 紫
#define Cyan 6    // 青
#define White 7
// ues to
#define Texts 30
#define Bakegrounds 40

// --------define area
struct _UI_CONTEXT;

// --------make area

// context
struct _UI_CONTEXT
{
public:
    MPoint<int> CameraXY;   // 当前屏幕相机位置
    MPoint<int> ScreenXY;   // 屏幕大小
    MPoint<int> C_point;    // 当前光标位置,(1,1)开始
    MPoint<int> TXT_status; // 默认文本色彩和其状态
    _UI_CONTEXT();
} ui_context;

_UI_CONTEXT::_UI_CONTEXT()
{
    this->CameraXY = MPoint<int>(0, 0);
    ui_context.ScreenXY = WinSize();
    this->C_point = MPoint<int>(1, 1);
    this->TXT_status = MPoint<int>(Regular, White);
    return;
}