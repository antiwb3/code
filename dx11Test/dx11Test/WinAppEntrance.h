#ifndef _WIN_APP_ENTRANCE_H
#define _WIN_APP_ENTRANCE_H

#include "dx11mgr.h"

class WinAppEntrance
{
public:
    static WinAppEntrance& Instance();

    WinAppEntrance();
    ~WinAppEntrance();

    int Init(HWND hWnd);
    void Uninit();

    LRESULT OnWndProc(UINT message, WPARAM wParam, LPARAM lParam);

    void Run();

private:
    int  OnLoop();
    void Render(double lasttime, double nowtime);
public:
    HWND          m_Wnd;
    DX11Mgr       m_DX11Mgr;
};

#endif //!_WIN_APP_ENTRANCE_H