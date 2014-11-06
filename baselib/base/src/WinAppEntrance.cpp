#include "stdafx.h"
#include "ASBaseFuns.h"
#include "WinAppEntrance.h"

FILE* g_pConsoleFile = NULL;

void SetTimerFrame(HANDLE hevent, int nFrame)
{
    LARGE_INTEGER liDueTime;
    liDueTime.QuadPart = -10000000LL / 16;

    if (nFrame <= 0)
        nFrame = 1;
    if (nFrame > 1000)
        nFrame = 1000;

    LONG lPeriod = 1000 / nFrame;

    SetWaitableTimer(hevent, &liDueTime, lPeriod, NULL, NULL, 0);
}

void MyAllocConsole()
{
    int nRetCode = false;

    nRetCode = AllocConsole();
    if (nRetCode)
    {
        freopen_s(&g_pConsoleFile, "CONOUT$", "w+t", stdout);
        printf("open console success.\n");
    }
}

#define EVENT_TIMER 0
#define EVENT_TOTAL 1

static DWORD s_dwTime = 0;

WinAppEntrance& WinAppEntrance::Instance()
{
    static WinAppEntrance s_inst;
    return s_inst;
}

WinAppEntrance::WinAppEntrance()
: m_SFUI(NULL)
{

}

WinAppEntrance::~WinAppEntrance()
{

}

int WinAppEntrance::Init(HWND hWnd)
{
    MyAllocConsole();

    m_DX11Mgr.Init(hWnd);
    m_Wnd = hWnd;

    m_SFUI = CreateScaleformUI();
    m_SFUI->Init(m_DX11Mgr.m_pDevice, m_DX11Mgr.m_pImmContext);

    ASBaseFuns::RegGlobalFuns();

    m_SFUI->Run();

    s_dwTime = GetTickCount();
    return true;
}

void WinAppEntrance::Uninit()
{
    if (g_pConsoleFile)
    {
        fclose(g_pConsoleFile);
        g_pConsoleFile = NULL;
    }

    if (m_SFUI)
    {
        m_SFUI->UnInit();
        delete m_SFUI;
    }
    m_DX11Mgr.Uninit();
}


LRESULT WinAppEntrance::OnWndProc(UINT message, WPARAM wParam, LPARAM lParam)
{
     if (message == WM_EXITSIZEMOVE)
     {
         if (m_DX11Mgr.m_pImmContext)
         {
             RECT rect;
             GetWindowRect(m_Wnd, &rect);
             m_DX11Mgr.OnClientSize(rect.right - rect.left, rect.bottom - rect.top);
         }
     }

	return m_SFUI->WndProc(message, wParam, lParam);
}


int WinAppEntrance::OnLoop()
{
    DWORD nowtime = GetTickCount();
    Render( (double)s_dwTime, (double)nowtime );

    m_SFUI->OnActive();

    s_dwTime = nowtime;
    return true;
}

void WinAppEntrance::Run()
{
    static int s_bLoop = true;
    static HANDLE s_aEvents[EVENT_TOTAL];

    s_aEvents[EVENT_TIMER] = CreateWaitableTimer(NULL, FALSE, NULL);
    SetTimerFrame(s_aEvents[EVENT_TIMER], 1000);

    while (s_bLoop)
    {
        DWORD dwResult = MsgWaitForMultipleObjectsEx(EVENT_TOTAL, s_aEvents, INFINITE, QS_ALLEVENTS, 0);

        switch (dwResult - WAIT_OBJECT_0) 
        {
        case EVENT_TIMER:
            s_bLoop = OnLoop();
            break;
        case EVENT_TOTAL: 
            {
                MSG msg;
                while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
                {
                    if (msg.message == WM_QUIT) 
                    {
                        s_bLoop = FALSE;
                    }

                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
                break;
            }
        }
    }
}

void WinAppEntrance::Render(double lasttime, double nowtime)
{
    // Just clear the backbuffer
    if (!m_DX11Mgr.m_pImmContext || !m_SFUI)
    {
        return;
    }
   //float ClearColor[4] = { 0.388f, 0.388f, 0.388f, 1.0f }; //red,green,blue,alpha
    float ClearColor[4] = {1.0f, 1.0f, 1.0f, 1.0f }; //red,green,blue,alpha
    m_DX11Mgr.m_pImmContext->ClearRenderTargetView( m_DX11Mgr.m_pRTView, ClearColor );

    m_SFUI->BeginRender();

    m_SFUI->Render();

    m_SFUI->EndRender();

    m_DX11Mgr.m_pSwapChain->Present( 0, 0 );
}
