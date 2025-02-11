#include <clocale>
#include <graphics.h>
#include <windows.h>
#include "Widget/Buttom/Buttom.h"
#include "Widget/ListButtom/ListButtom.h"
#include "UI/Sidebar/Sidebar.h"
#include "Data/StringSkipList/StringSkipList.h"
#include "Tools/Tools.h"

int main()
{
    setlocale(LC_ALL, "");

    const int WIDTH = 1140;
    const int HEIGHT = 640;

    initgraph(WIDTH, HEIGHT, INIT_RENDERMANUAL);

    setfont(16, 0, "Arial");

    Sidebar* g_sidebar = SIDEBAR_createGraySidebar(0, 0, WIDTH / 7, HEIGHT);

    while (is_run())
    {
        while (mousemsg())
        {
            mouse_msg msg = getmouse();

            if (msg.is_move())
            {
                SIDEBAR_handleMouseMove(g_sidebar, msg.x, msg.y);
            }
            else if (msg.is_down())
            {
                SIDEBAR_handleButtonDown(g_sidebar, msg.x, msg.y);
            }
        }

        if (SIDEBAR_isExit(g_sidebar))
        {
            break;
        }
         
        setbkcolor(RGB(255, 255, 255));
        cleardevice();

        SIDEBAR_draw(g_sidebar);
        //BUTTON_draw(g_button);
        //LISTBUTTON_draw(g_list_button);

        delay_fps(60);
    }

    SIDEBAR_destroy(g_sidebar);

    closegraph();

    return 0;
}