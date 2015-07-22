#include "stdafx.h"
#include "DxAssist.h"
#include "Render_Test.h"
#include "PaintEngine.h"
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
static int s_bInit = false;
WinAppEntrance& WinAppEntrance::Instance()
{
    static WinAppEntrance s_inst;
    return s_inst;
}

WinAppEntrance::WinAppEntrance()
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

    DxAssist::Init(m_DX11Mgr.m_pDevice, m_DX11Mgr.m_pImmContext);
    PaintEngine::Instance().Init(hWnd);
    RenderTest::Instance().Init(hWnd);
    s_bInit = true;
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
    m_DX11Mgr.Uninit();
    PaintEngine::Instance().Uninit();
}


LRESULT WinAppEntrance::OnWndProc(UINT message, WPARAM wParam, LPARAM lParam)
{
     if (message == WM_EXITSIZEMOVE)
     {
        
     }
     else if (message == WM_SIZE)
     {
         int width  = (int)LOWORD(lParam);
         int height = (int)HIWORD(lParam);
         if (m_DX11Mgr.m_pImmContext)
         {
             RECT rect;
             m_DX11Mgr.OnClientSize(width, height);
         }
     }

	return 1;//m_SFUI->WndProc(message, wParam, lParam);
}


int WinAppEntrance::OnLoop()
{
    DWORD nowtime = GetTickCount();
    Render( (double)s_dwTime, (double)nowtime );

    //OnActive();

    s_dwTime = nowtime;
    return true;
}

void WinAppEntrance::Run()
{
    static int s_bLoop = true;
    static HANDLE s_aEvents[EVENT_TOTAL];

    s_aEvents[EVENT_TIMER] = CreateWaitableTimer(NULL, FALSE, NULL);
    SetTimerFrame(s_aEvents[EVENT_TIMER], 66);

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
    if (!m_DX11Mgr.m_pImmContext || !s_bInit)
    {
        return;
    }
    //static float s_ClearColor[4] = { 0.388f, 0.388f, 0.388f, 1.0f }; //red,green,blue,alpha
    float s_ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red, green, blue, alpha

    //float ClearColor[4] = {1.0f, 1.0f, 1.0f, 1.0f }; //red,green,blue,alpha
    m_DX11Mgr.m_pImmContext->ClearRenderTargetView( m_DX11Mgr.m_pRTView, s_ClearColor );

   // m_DX11Mgr.m_pRTView
    //
    // Clear the depth buffer to 1.0 (max depth)
    //
    m_DX11Mgr.m_pImmContext->ClearDepthStencilView( m_DX11Mgr.m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );
    
    RenderTest::Instance().Render(0, 0);

    m_DX11Mgr.m_pSwapChain->Present( 0, 0 );
}
