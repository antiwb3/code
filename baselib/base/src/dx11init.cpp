// dx11init.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "dx11init.h"
#include "dx11mgr.h"
#include "gfxmgr.h"
#include "ASBaseFuns.h"
#include "UIWindowMgr.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

HWND g_hWnd;
static DX11Mgr g_DX11Mgr;
void Render(double lasttime, double nowtime);
int Init();
void Uinit();
void Run();

FILE* g_pConsoleFile = NULL;

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

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_DX11INIT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DX11INIT));

	// Main message loop:

	
	Init();
	Run();
	Uinit();

	return 0;
}

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

#define EVENT_TIMER 0
#define EVENT_TOTAL 1

static DWORD s_dwTime = 0;
//IGFxUI* g_iGFxUI = NULL;

int Init()
{
    MyAllocConsole();

	g_DX11Mgr.Init(g_hWnd);

    //g_iGFxUI = CreateGFxUI();
    //g_iGFxUI->Init(g_DX11Mgr.m_pDevice, g_DX11Mgr.m_pImmContext);

	GFx::System::Init();
	GFXMgr::Instance().Init(g_DX11Mgr.m_pDevice, g_DX11Mgr.m_pImmContext);

	char* path = "ui\\main.swf";
    //char* path = "ui\\ascript\\app\\login\\login.swf";

    ASBaseFuns::RegGlobalFuns();

	UIWindowMgr::Instance().OpenWindow(0, path, "first_sfx");

	s_dwTime = GetTickCount();
	return true;
}

void Uinit()
{
    UIWindowMgr::Instance().Uninit();
	GFXMgr::Instance().Uninit();
	g_DX11Mgr.Uninit();
}

int OnLoop()
{
	DWORD nowtime = GetTickCount();
	::Render( (double)s_dwTime, (double)nowtime );

    UIWindowMgr::Instance().OnActive();

	s_dwTime = nowtime;
	return true;
}

void Run()
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
						WndProc(g_hWnd, WM_QUIT, 0, 0);
						s_bLoop = FALSE;
					}
					else
					{
						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}
				}
				break;
			}
		}
	}
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DX11INIT));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_DX11INIT);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{

   hInst = hInstance; // Store instance handle in our global variable

   /*g_hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
*/
   g_hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
       0, 0, 1480, 960, NULL, NULL, hInstance, NULL);

   if (!g_hWnd)
   {
      return FALSE;
   }

   ShowWindow(g_hWnd, nCmdShow);
   UpdateWindow(g_hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//

LRESULT OnWndProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message >= WM_KEYFIRST && message <= WM_KEYLAST)
	{
		return UIWindowMgr::Instance().WndProc(message, wParam, lParam);
	}
	else if (message >= WM_MOUSEFIRST && message <= WM_MOUSELAST || message == WM_MOUSEHOVER)
	{
		if (message == WM_MOUSEMOVE || message == WM_MOUSEHOVER)
		{
			/*		TRACKMOUSEEVENT tme;
			tme.cbSize = sizeof(tme);
			tme.hwndTrack = hWnd;
			tme.dwFlags = TME_HOVER;
			tme.dwHoverTime = m_dwHoverTime;
			TrackMouseEvent(&tme);*/
		}
		return UIWindowMgr::Instance().WndProc(message, wParam, lParam);
	}
	return 0;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	if (OnWndProc(message, wParam, lParam))
		return 1;

	switch (message)
	{
	case WM_COMMAND:
		{
			wmId    = LOWORD(wParam);
			wmEvent = HIWORD(wParam);
			// Parse the menu selections:
			//GFXMgr::Instance().

			switch (wmId)
			{
			case IDM_ABOUT:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
				break;
			case IDM_EXIT:
				DestroyWindow(hWnd);
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		//Render(0, 0);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
    case WM_SIZE:
        break;
    case WM_EXITSIZEMOVE:
        if (g_DX11Mgr.m_pImmContext)
        {
            RECT rect;
            GetWindowRect(g_hWnd, &rect);
            g_DX11Mgr.OnClientSize(rect.right - rect.left, rect.bottom - rect.top);
            //GFXMgr::Instance().OnResetDevice();
            UIWindowMgr::Instance().SetClientSize(rect.right - rect.left, rect.bottom - rect.top);
        }
        break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}


void Render(double lasttime, double nowtime)
{
	// Just clear the backbuffer
	if (!g_DX11Mgr.m_pImmContext)
	{
		return;
	}
	float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; //red,green,blue,alpha
    g_DX11Mgr.m_pImmContext->ClearRenderTargetView( g_DX11Mgr.m_pRTView, ClearColor );
	
    
	GFXMgr::Instance().BeginRender();

	UIWindowMgr::Instance().Render(lasttime, nowtime);

	GFXMgr::Instance().EndRender();
	g_DX11Mgr.m_pSwapChain->Present( 0, 0 );
}
