#include <chrono>
#include <thread>
#include <stdio.h>
#include "../src/ui/ui.h"
int main()
{
    block ba(MPoint(3, 6));
    ba.SetBlockBlackgroundColor(Yellow);
    ba.SetTxtBlackgroundColor(Blue);
    ba.SetTxtStle(tBold, Green);
    ba.SetText("你好呀!");
    ba.SetPosition(MPoint(0, -1));

    canvas c1;
    c1.AddBlock(&ba);
    c1.SetLevel(0);

    camera ca1;
    ca1.AddCanvas(&c1);
    ca1.CatchScreen();

    // getchar();

    for (int y = 0; y < ui_context.ScreenXY.y; y++)
    {
        ba.SetPosition(MPoint(0, y));
        ca1.CatchScreen();
        // fflush(stdout);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        // getchar();
    }

    return 0;
}