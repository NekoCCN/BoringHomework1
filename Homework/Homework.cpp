#include <clocale>
#include <graphics.h>
#include <windows.h>
#include "UI/Sidebar/Sidebar.h"
#include "UI/QueryBook/QueryBook.h"
#include "UI/Insert/Insert.h"
#include "UI/QueryBookType/QueryBookType.h"
#include "Data/StringSkipList/StringSkipList.h"
#include "Data/SRVector/SRVector.h"
#include "Tools/Tools.h"
#include "UI/Insert/Insert.h"

void writeTestData(StringSkipList* list)
{
    BaseData buf;
    buf.id = 1;
    wcscpy(buf.course, L"C"); // 使用 strcpy
    buf.daily_score = 100;
    buf.daily_score_ratio = 0.5;
    buf.final_score = 100;

    STRINGSKIPLIST_insertW(list, L"哇哇哇", &buf);
    STRINGSKIPLIST_insertW(list, L"awa", &buf);
    STRINGSKIPLIST_insertW(list, L"呜呜呜", &buf);
    STRINGSKIPLIST_insertW(list, L"QAQ", &buf);
    STRINGSKIPLIST_insertW(list, L"123", &buf);
    STRINGSKIPLIST_insertW(list, L"23344", &buf);
    STRINGSKIPLIST_insertW(list, L"adsea", &buf);
    STRINGSKIPLIST_insertW(list, L"Q123", &buf);
    STRINGSKIPLIST_insertW(list, L"sad", &buf);
    STRINGSKIPLIST_insertW(list, L"aaseqw", &buf);
    STRINGSKIPLIST_insertW(list, L"asxsa", &buf);
    STRINGSKIPLIST_insertW(list, L"qweqws", &buf);
}
int main()
{
    setlocale(LC_ALL, "");

    const int WIDTH = 1140;
    const int HEIGHT = 640;
    const int LIST_PADDING = 16;

    initgraph(WIDTH, HEIGHT, INIT_RENDERMANUAL | INIT_UNICODE);
    setfont(16, 0, "Arial");

    StringSkipList* list = STRINGSKIPLIST_construct(FIBONACCI_NUMBERS_MAX_LEVEL, FIBONACCI_NUMBERS);
    writeTestData(list);

    Sidebar* g_sidebar = SIDEBAR_createGraySidebar(0, 0, WIDTH / 7, HEIGHT);

    // MaterialList* g_materialList = MATERIALLIST_createGrayMaterialList(WIDTH / 6, HEIGHT / 6, WIDTH - (WIDTH / 5), HEIGHT - (HEIGHT / 5), 60, LIST_PADDING);

    Query* g_query = QUERYBOOK_createGrey(WIDTH / 7, 0, WIDTH - (WIDTH / 7), HEIGHT, list);
    Insert* g_insert = INSERT_createGrey(WIDTH / 7, 0, WIDTH - (WIDTH / 7), HEIGHT, list);

    int count = 0;
    // 获取数据并设置给 MaterialList
    SearchResult* allData = STRINGSKIPLIST_getAll(list, &count);

    while (is_run())
    {
        while (mousemsg())
        {
            mouse_msg msg = getmouse();

            if (msg.is_move())
            {
                SIDEBAR_handleMouseMove(g_sidebar, msg.x, msg.y);

                QUERYBOOK_handleMouseMove(g_query, msg.x, msg.y);
                INSERT_handleMouseMove(g_insert, msg.x, msg.y);
            }
            else if (msg.is_down())
            {
                SIDEBAR_handleButtonDown(g_sidebar, msg.x, msg.y);

                QUERYBOOK_handleButtonDown(g_query, msg.x, msg.y, g_insert, g_sidebar);
                INSERT_handleMouseDown(g_insert, msg.x, msg.y);
            }
            else if (msg.is_wheel())
            {
                QUERYBOOK_handleMouseWheel(g_query, msg.wheel);
            }
            else if (msg.is_up())
            {
                QUERYBOOK_handleMouseUp(g_query, msg.x, msg.y);
                INSERT_handleMouseUp(g_insert, msg.x, msg.y);
            }
        }
        while (kbmsg())
        {
            key_msg msg = getkey();

            QUERYBOOK_handleInput(g_query, msg);
            INSERT_handleInput(g_insert, msg);
        }

        if (SIDEBAR_isExit(g_sidebar))
        {
            break;
        }

        SidebarButtonType status = SIDEBAR_getStatus(g_sidebar);
        if (status == SIDEBAR_BUTTON_INSERTBOOK)
        {
            INSERT_SetStatus(g_insert, true);
            QUERYBOOK_SetStatus(g_query, false);
        }
        else if (status == SIDEBAR_BUTTON_QUERYBOOK)
        {
            QUERYBOOK_SetStatus(g_query, true);
            INSERT_SetStatus(g_insert, false);
        }

        setbkcolor(RGB(255, 255, 255));
        cleardevice();

        SIDEBAR_draw(g_sidebar);
        QUERYBOOK_draw(g_query);
        INSERT_draw(g_insert);

        delay_fps(60);
    }

    SIDEBAR_destroy(g_sidebar);
    QUERYBOOK_destroy(g_query);

    STRINGSKIPLIST_destruct(list);
    closegraph();

    return 0;
}