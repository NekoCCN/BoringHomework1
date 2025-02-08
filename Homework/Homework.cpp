#include <graphics.h>
#include <windows.h>
#include "Widget/Buttom/Buttom.h"
#include "Tools/Tools.h"

Button* g_button = NULL;

int main()
{
    initgraph(640, 480, INIT_RENDERMANUAL);

    setfont(16, 0, "Arial");

    g_button = Button_create(100, 50, 200, 75, "Button",
        hexToColorref("#00BFFF"), hexToColorref("#00FFFF"), hexToColorref("#008B8B"), 10);
    
    if (g_button == NULL)
    {
        closegraph();
        return 1;
    }

    Button_draw(g_button);

    while (is_run())
    {
        while (mousemsg())
        {
            mouse_msg msg = getmouse();

            if (msg.is_move())
            {
                Button_handleMouseMove(g_button, msg.x, msg.y);
            }
            else if (msg.is_left())
            {
                Button_handleButtonDown(g_button, msg.x, msg.y);
            }
        }
         
        setbkcolor(RGB(255, 255, 255));
        cleardevice();

        Button_draw(g_button);

        delay_fps(60);
    }

    Button_destroy(g_button);
    closegraph();
    return 0;
}