#include <clocale>
#include <graphics.h>
#include <windows.h>
#include "UI/Sidebar/Sidebar.h"
#include "UI/QueryBook/QueryBook.h"
#include "UI/InsertBook/InsertBook.h"
#include "UI/QueryBookType/QueryBookType.h"
#include "Data/StringSkipList/StringSkipList.h"
#include "Data/SRVector/SRVector.h"
#include "Tools/Tools.h"
#include "UI/InsertBook/InsertBook.h"
#include "UI/Login/Login.h"
#include "DataStream/DataRStream/DataRStream.h"
#include "Data/UserList/UserList.h"
#include "UI/InsertUser/InsertUser.h"
#include "UI/QueryUser/QueryUser.h"
#include "UI/AdminUser/AdminUser.h"

//void writeTestData(StringSkipList* list)
//{
//    BaseData buf;
//    buf.id = 1;
//    wcscpy(buf.course, L"C"); // 使用 strcpy
//    buf.daily_score = 100;
//    buf.daily_score_ratio = 0.5;
//    buf.final_score = 100;
//
//    STRINGSKIPLIST_insertW(list, L"哇哇哇", &buf);
//    STRINGSKIPLIST_insertW(list, L"awa", &buf);
//    STRINGSKIPLIST_insertW(list, L"呜呜呜", &buf);
//    STRINGSKIPLIST_insertW(list, L"QAQ", &buf);
//    STRINGSKIPLIST_insertW(list, L"123", &buf);
//    STRINGSKIPLIST_insertW(list, L"23344", &buf);
//    STRINGSKIPLIST_insertW(list, L"adsea", &buf);
//    STRINGSKIPLIST_insertW(list, L"Q123", &buf);
//    STRINGSKIPLIST_insertW(list, L"sad", &buf);
//    STRINGSKIPLIST_insertW(list, L"aaseqw", &buf);
//    STRINGSKIPLIST_insertW(list, L"asxsa", &buf);
//    STRINGSKIPLIST_insertW(list, L"qweqws", &buf);
//}
int main()
{
    setlocale(LC_ALL, "");

    const int WIDTH = 1140;
    const int HEIGHT = 640;
    const int LIST_PADDING = 16;

    initgraph(WIDTH, HEIGHT, INIT_RENDERMANUAL | INIT_UNICODE | INIT_NOFORCEEXIT);
    HWND hwnd = getHWnd();
    SetWindowTextA(hwnd, "图书借阅管理系统");

    setfont(20, 0, "微软雅黑");

    DataRStream* data = DATARSTREAM_create("admin.hwdat", "book.hwdat", "user.hwdat");
    StringSkipList* skip_data = data->data_;
    list* user_data = data->user_list_;

    Login* login = LOGIN_createGrey(0, 0, WIDTH, HEIGHT, data->user_, data->user_num_);

    LOGIN_setActivedStatus(login, true);

    Sidebar* g_sidebar = SIDEBAR_createGraySidebar(0, 0, WIDTH / 7, HEIGHT);
    SIDEBAR_setActive(g_sidebar, false);

    Query* query_book = QUERYBOOK_createGrey(WIDTH / 7, 0, WIDTH - WIDTH / 7, HEIGHT, skip_data, login->login_status_);
    QUERYBOOK_SetStatus(query_book, false);

    InsertBook* insert_book = INSERTBOOK_createGrey(WIDTH / 7, 0, WIDTH - WIDTH / 7, HEIGHT, skip_data);
    INSERTBOOK_SetStatus(insert_book, false);

    InsertUser* insert_user = INSERTUSER_createGrey(WIDTH / 7, 0, WIDTH - WIDTH / 7, HEIGHT, user_data, skip_data);
    INSERTUSER_SetStatus(insert_user, false);

    QueryUser* query_user = QUERYUSER_createGrey(WIDTH / 7, 0, WIDTH - WIDTH / 7, HEIGHT, user_data, skip_data, login->login_status_);
    QUERYUSER_SetStatus(query_user, false);

    AdminUser* admin_user = ADMINUSER_createGrey(WIDTH / 7, 0, WIDTH - WIDTH / 7, HEIGHT, data->user_, data->user_num_, login->login_status_);

    // MaterialList* g_materialList = MATERIALLIST_createGrayMaterialList(WIDTH / 6, HEIGHT / 6, WIDTH - (WIDTH / 5), HEIGHT - (HEIGHT / 5), 60, LIST_PADDING);

    //Query* g_query = QUERYBOOK_createGrey(WIDTH / 7, 0, WIDTH - (WIDTH / 7), HEIGHT, list);
    //Insert* g_insert = INSERT_createGrey(WIDTH / 7, 0, WIDTH - (WIDTH / 7), HEIGHT, list);

    int count = 0;
    // 获取数据并设置给 MaterialList
    //SearchResult* allDat 
    // a = STRINGSKIPLIST_getAll(list, &count);

    while (is_run())
    {
        while (mousemsg())
        {
            mouse_msg msg = getmouse();

            if (msg.is_move())
            {
                SIDEBAR_handleMouseMove(g_sidebar, msg.x, msg.y);

                LOGIN_handleMouseMove(login, msg.x, msg.y);

                QUERYBOOK_handleMouseMove(query_book, msg.x, msg.y);
                INSERTBOOK_handleMouseMove(insert_book, msg.x, msg.y);

                QUERYUSER_handleMouseMove(query_user, msg.x, msg.y);
                INSERTUSER_handleMouseMove(insert_user, msg.x, msg.y);

                ADMINUSER_handleMouseMove(admin_user, msg.x, msg.y);
            }
            else if (msg.is_down())
            {
                SIDEBAR_handleButtonDown(g_sidebar, msg.x, msg.y);

                LOGIN_handleButtonDown(login, msg.x, msg.y);

                QUERYBOOK_handleButtonDown(query_book, msg.x, msg.y, insert_book, g_sidebar, insert_user);
                INSERTBOOK_handleMouseDown(insert_book, msg.x, msg.y);

                QUERYUSER_handleButtonDown(query_user, msg.x, msg.y, insert_user, g_sidebar);
                INSERTUSER_handleMouseDown(insert_user, msg.x, msg.y);

                ADMINUSER_handleMouseDown(admin_user, msg.x, msg.y);
            }
            else if (msg.is_wheel())
            {
                QUERYBOOK_handleMouseWheel(query_book, msg.wheel);
                QUERYUSER_handleMouseWheel(query_user, msg.wheel);

                ADMINUSER_handleMouseWheel(admin_user, msg.wheel);
            }
            else if (msg.is_up())
            {
                LOGIN_handleMouseUp(login, msg.x, msg.y);

                QUERYBOOK_handleMouseUp(query_book, msg.x, msg.y);
                INSERTBOOK_handleMouseUp(insert_book, msg.x, msg.y);

                QUERYUSER_handleMouseUp(query_user, msg.x, msg.y);
                INSERTUSER_handleMouseUp(insert_user, msg.x, msg.y);

                ADMINUSER_handleMouseUp(admin_user, msg.x, msg.y);
            }
        }
        while (kbmsg())
        {
            key_msg msg = getkey();

            LOGIN_handleInput(login, msg);

            QUERYBOOK_handleInput(query_book, msg);
            INSERTBOOK_handleInput(insert_book, msg);

            QUERYUSER_handleInput(query_user, msg);
            INSERTUSER_handleInput(insert_user, msg);

            ADMINUSER_handleInput(admin_user, msg);
        }

        if (SIDEBAR_isExit(g_sidebar) || LOGIN_isExit(login))
        {
            break;
        }

        if (LOGIN_isDone(login))
        {
            QUERYBOOK_setLoginStatus(query_book, login->login_status_);
            INSERTBOOK_setLoginStatus(insert_book, login->login_status_);
            
            QUERYUSER_setLoginStatus(query_user, login->login_status_);
            INSERTUSER_setLoginStatus(insert_user, login->login_status_);

            ADMINUSER_setLoginStatus(admin_user, login->login_status_);

            LOGIN_setActivedStatus(login, false);

            SIDEBAR_setActive(g_sidebar, true);
            INSERTBOOK_SetStatus(insert_book, true);
        }

        if (!LOGIN_isActived(login))
        {
            SidebarButtonType status = SIDEBAR_getStatus(g_sidebar);
            if (status == SIDEBAR_BUTTON_INSERTBOOK)
            {
                INSERTBOOK_SetStatus(insert_book, true);
                QUERYBOOK_SetStatus(query_book, false);
                QUERYUSER_SetStatus(query_user, false);
                INSERTUSER_SetStatus(insert_user, false);

                ADMINUSER_SetStatus(admin_user, false);
            }
            else if (status == SIDEBAR_BUTTON_QUERYBOOK)
            {
                INSERTBOOK_SetStatus(insert_book, false);
                QUERYBOOK_SetStatus(query_book, true);
                QUERYUSER_SetStatus(query_user, false);
                INSERTUSER_SetStatus(insert_user, false);

                ADMINUSER_SetStatus(admin_user, false);
            }
            else if (status == SIDEBAR_BUTTON_QUERYUSER)
            {
                INSERTBOOK_SetStatus(insert_book, false);
                QUERYBOOK_SetStatus(query_book, false);
                QUERYUSER_SetStatus(query_user, true);
                INSERTUSER_SetStatus(insert_user, false);

                ADMINUSER_SetStatus(admin_user, false);
            }
            else if (status == SIDEBAR_BUTTON_INSERTUSER)
            {
                INSERTBOOK_SetStatus(insert_book, false);
                QUERYBOOK_SetStatus(query_book, false);
                QUERYUSER_SetStatus(query_user, false);
                INSERTUSER_SetStatus(insert_user, true);

                ADMINUSER_SetStatus(admin_user, false);
            }
            else if (status == SIDEBAR_BUTTON_ACCOUNT)
            {
                INSERTBOOK_SetStatus(insert_book, false);
                QUERYBOOK_SetStatus(query_book, false);
                QUERYUSER_SetStatus(query_user, false);
                INSERTUSER_SetStatus(insert_user, false);

                ADMINUSER_SetStatus(admin_user, true);
            }
        }

        setbkcolor(hexToColorref("#F8ECE0"));
        cleardevice();

        LOGIN_draw(login);

        SIDEBAR_draw(g_sidebar);

        QUERYBOOK_draw(query_book);
        INSERTBOOK_draw(insert_book);
        QUERYUSER_draw(query_user);
        INSERTUSER_draw(insert_user);

        ADMINUSER_draw(admin_user);

        //INSERT_draw(g_insert);

        delay_fps(60);
    }

    //STRINGSKIPLIST_destruct(list);

    data->user_ = admin_user->user_;
    data->user_num_ = admin_user->user_num_;

    //SIDEBAR_destroy(g_sidebar);

    //QUERYBOOK_destroy(query_book);
    //QUERYUSER_destroy(query_user);
    //INSERTUSER_destroy(insert_user);
    //INSERTBOOK_destroy(insert_book);

    //ADMINUSER_destroy(admin_user);

    DataWStream* dataw = DATAWSTREAM_create("admin.hwdat", "book.hwdat", "user.hwdat", false);
    DATAWSTREAM_start(dataw, data->user_, data->user_num_, data->data_, data->user_list_);
    DATAWSTREAM_destory(dataw);

    DATARSTREAM_destory(data);
    
    closegraph();

    return 0;
}