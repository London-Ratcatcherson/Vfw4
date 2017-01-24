// Vfw4.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

// Global Variables:
WinLogger			winLog;			// message logging object
HANDLE				g_hCapEvent;	// semaphore for capture thread

// defined in VfwUtility.cpp
extern BOOL					dumpCAPDRIVERCAPS(PCAPDRIVERCAPS pCaps);
extern BOOL					dumpCAPSTATUS(PCAPSTATUS pCaps);
extern BOOL					dumpCAPTUREPARMS(PCAPTUREPARMS pCaps);
extern BOOL					SetCaptureFilename(HWND hWnd, HWND hWndCap, TCHAR *szCapFilename, int *nCapFileIndex);
extern BOOL					SetCapDIBFilename(HWND hWnd, TCHAR *szCapDIBFilename, int *nCapDIBFileIndex);
extern void					ResizeCaptureWindow(HWND hWndChild, HWND hWndCap);
extern LRESULT CALLBACK		CapWndProc(HWND, UINT, WPARAM, LPARAM);
extern LRESULT CALLBACK		VideoStreamCallback(HWND hWndCap, LPVIDEOHDR lpVHdr);
extern void					Cls_OnVideoCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify);
extern void					Cls_OnAudioCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify);
extern unsigned __stdcall	ShowCaptureStatusThread(void *pParams);

// Forward declarations of functions included in this code module:
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

ATOM				MyRegisterClass(HINSTANCE hInstance, TCHAR *szClass, TCHAR *szCapClass);
BOOL				InitInstance(HINSTANCE hInstance, int nCmdShow, TCHAR *szClass, TCHAR * szTitle);
bool				GetFontCaps(HWND hWnd, int *cxChar, int *cxCaps, int *cyChar);


// Windows Message Crackers for main (logging) window
void				Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void				Cls_OnPaint(HWND hwnd);
void				Cls_OnSize(HWND hWnd, UINT state, int cx, int cy);
void				Cls_OnVScroll(HWND hWnd, HWND hWndCtl, UINT code, int pos);
void				Cls_OnHScroll(HWND hWnd, HWND hWndCtl, UINT code, int pos);
void				Cls_OnDestroy(HWND hwnd);

// Non-cracker message handlers
BOOL				Cls_OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	MSG msg = { 0 };

	// Get the strings for the logging and Preview Window classes, and titles
	TCHAR szWindowClass[MAX_PATH] = { 0 };
	TCHAR szStringTwo[MAX_PATH] = { 0 };
	LoadString(hInstance, IDC_VFW4, szWindowClass, MAX_PATH);
	LoadString(hInstance, IDC_VFW4_CAPWINCLASS, szStringTwo, MAX_PATH);
	// Register both classes
	MyRegisterClass(hInstance, szWindowClass, szStringTwo);

	// Perform application initialization:
	LoadString(hInstance, IDS_APP_TITLE, szStringTwo, MAX_PATH);
	if (! InitInstance (hInstance, nCmdShow, szWindowClass, szStringTwo))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_VFW4));

    // Initialize the log with timestamping and filewriting turned ON
    winLog.Init(NULL, hInstance);
    winLog.TimeStamp(true);
    winLog.WriteLogfile(true);

    // Log the app path, name and command line
    TCHAR pApp[MAX_PATH + 1] = { 0 };
    if ((0 != GetModuleFileName(hInstance, pApp, MAX_PATH)) && (0 != *pApp)) 
    {
        winLog.writelog(_T("Application path and name: %s"), pApp);
    }
    winLog.writelog(_T("Command line: %s"), lpCmdLine);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance, TCHAR *szClass, TCHAR *szCapClass)
{
	WNDCLASSEX wcex = { 0 };

	// Register the main UI / logging window
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_VFWCAM));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_VFW4);
	wcex.lpszClassName	= szClass;
	//wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	RegisterClassEx(&wcex);

	// Register the preview window
	wcex.lpfnWndProc	= CapWndProc;
	wcex.lpszClassName	= szCapClass;
	wcex.lpszMenuName	= NULL;
	wcex.hIcon			= NULL;

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: creates main window
//
//   COMMENTS:
//
//        In this function, we create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow, TCHAR *szClass, TCHAR * szTitle)
{
   HWND hWnd = NULL;

   hWnd = CreateWindow(
	   szClass, szTitle, 
	   WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL, 
	   CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 
	   NULL, NULL,			// hWndParent, hMenu
	   hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   // In case we use a common control
   InitCommonControls();

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
        HANDLE_MSG(hWnd, WM_COMMAND,    Cls_OnCommand);
        HANDLE_MSG(hWnd, WM_PAINT,      Cls_OnPaint);
        HANDLE_MSG(hWnd, WM_SIZE,       Cls_OnSize);
        HANDLE_MSG(hWnd, WM_VSCROLL,    Cls_OnVScroll);
        HANDLE_MSG(hWnd, WM_HSCROLL,    Cls_OnHScroll);
        HANDLE_MSG(hWnd, WM_DESTROY,    Cls_OnDestroy);

    case WM_CREATE: 
		// For some reason, the OnCreate message cracker doesn't work, 
		//   so we use our own non-macro
        return Cls_OnCreate(hWnd, reinterpret_cast<LPCREATESTRUCT> (lParam) );

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message crackers
///* void Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) */
//#define HANDLE_WM_COMMAND(hwnd, wParam, lParam, fn) \
//    ((fn)((hwnd), (int)(LOWORD(wParam)), (HWND)(lParam), (UINT)HIWORD(wParam)), 0L)
//#define FORWARD_WM_COMMAND(hwnd, id, hwndCtl, codeNotify, fn) \
//    (void)(fn)((hwnd), WM_COMMAND, MAKEWPARAM((UINT)(id),(UINT)(codeNotify)), (LPARAM)(HWND)(hwndCtl))
void Cls_OnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify)
{
	HINSTANCE	hInst = NULL;

	// Parse the menu selections:
	switch (id)
	{
    // Process all the Vfw Audio commands in one function
	case ID_MCI_CREATE:
	case ID_MCI_PLAY:
	case ID_MCI_PAUSE:
	case ID_MCI_STOP:
	case ID_MCI_RELEASE:
        Cls_OnAudioCommand(hWnd, id, hwndCtl, codeNotify);
        break;

    // Process all the Vfw Video commands in one function
	// (Listed in menu order)
	case ID_CAP_PREVIEW:
    case ID_CAP_CAPTURE:
    case ID_CAP_CAPTURE_FRAME:

	case ID_VIDEO_GETCAPABILITIES:
	case ID_CAP_GET_STATUS:
	case ID_CAP_GET_PARAMETERS:

    case ID_CAP_SOURCE:
    case ID_CAP_FORMAT:
    case ID_CAP_DISPLAY:
    case ID_CAP_COMPRESSION:

	case ID_CAP_RELEASE:
        Cls_OnVideoCommand(hWnd, id, hwndCtl, codeNotify);
        break;

	case IDM_ABOUT:
		hInst = reinterpret_cast <HINSTANCE> (GetWindowLongPtr(hWnd, GWLP_HINSTANCE));
		DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
		break;
	case IDM_EXIT:
		// Close the MCI and Preview windows
		SendMessage(hWnd, WM_COMMAND, ID_MCI_RELEASE, 0);
		SendMessage(hWnd, WM_COMMAND, ID_CAP_RELEASE, 0);
		DestroyWindow(hWnd);
		break;
    }

    FORWARD_WM_COMMAND(hWnd, id, hwndCtl, codeNotify, DefWindowProc);
}

// Paint the content as part of the paint cycle
//
///* void Cls_OnPaint(HWND hwnd) */
//#define HANDLE_WM_PAINT(hwnd, wParam, lParam, fn) \
//    ((fn)(hwnd), 0L)
//#define FORWARD_WM_PAINT(hwnd, fn) \
//    (void)(fn)((hwnd), WM_PAINT, 0L, 0L)
//
void Cls_OnPaint(HWND hWnd)
{
	PAINTSTRUCT ps = { 0 };
	HDC hDC = BeginPaint(hWnd, &ps);

	// Paint the main (logging) window
    GetClientRect(hWnd, &ps.rcPaint);

	// Get the system fixed font
    HGDIOBJ hFnt = 0, hOldFnt = 0;
    hFnt = GetStockObject(ANSI_FIXED_FONT);
    hOldFnt = SelectObject(hDC, hFnt);

	// If we have lines offscreen, signal WM_SIZE so we get scroll bar updates
	int cxChar = 0, cxCaps = 0, cyChar = 0;
	GetFontCaps(hWnd, &cxChar, &cxCaps, &cyChar);

	int nLogSize = static_cast<int>(winLog.logSize());
	if (nLogSize > ps.rcPaint.bottom / cyChar)
	{
		// lparam lo-word is new width, hi-word is new height
		SendMessage(hWnd, WM_SIZE, SIZE_RESTORED, MAKELPARAM((ps.rcPaint.right), (ps.rcPaint.bottom)));
	}

    // Get vertical, horizontal scroll bar position
    SCROLLINFO si = { 0 };
    si.cbSize = sizeof(si);
    si.fMask  = SIF_POS;
    GetScrollInfo(hWnd, SB_VERT, &si);
    int iVertPos = si.nPos;

    GetScrollInfo(hWnd, SB_HORZ, &si);
    int iHorzPos = si.nPos;

	// Find painting limits
    int iPaintBegin = max(0, iVertPos + ps.rcPaint.top / cyChar);
    int iPaintEnd   = min(static_cast<int>(winLog.logSize() ), iVertPos + ps.rcPaint.bottom / cyChar);

    // Calc how many lines to draw
    for (int iNdex = iPaintBegin; iNdex <= iPaintEnd; ++iNdex)
    {
        TextOut(hDC, cxChar * (1 - iHorzPos), cyChar * (iNdex - iVertPos), 
			winLog.readlog(iNdex), static_cast<int> (winLog.lineLength(iNdex)));
    }

	// restore the original font
    if (hOldFnt)
    {
        SelectObject(hDC, hOldFnt);
    }
    EndPaint(hWnd, &ps);

    FORWARD_WM_PAINT(hWnd, DefWindowProc);
}

///* void Cls_OnSize(HWND hwnd, UINT state, int cx, int cy) */
//#define HANDLE_WM_SIZE(hwnd, wParam, lParam, fn) \
//    ((fn)((hwnd), (UINT)(wParam), (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam)), 0L)
//#define FORWARD_WM_SIZE(hwnd, state, cx, cy, fn) \
//    (void)(fn)((hwnd), WM_SIZE, (WPARAM)(UINT)(state), MAKELPARAM((cx), (cy)))
void Cls_OnSize(HWND hWnd, UINT state, int cx, int cy)
{
    // Get the font sizes
	int cxChar = 0, cxCaps = 0, cyChar = 0;
	GetFontCaps(hWnd, &cxChar, &cxCaps, &cyChar);

    // Set horizonal and vertical scroll bar range, page size
    SCROLLINFO si = { 0 };
    si.cbSize = sizeof(si);
    si.fMask  = SIF_RANGE | SIF_PAGE;
    si.nMin   = 0;
    si.nMax   = 20 + static_cast<int> (winLog.GetLongestLineLength());
    si.nPage  = cx / cxChar;
    SetScrollInfo(hWnd, SB_HORZ, &si, TRUE);

    si.nMax   = static_cast<int> (winLog.logSize());
    si.nPage  = cy / cyChar;
    SetScrollInfo(hWnd, SB_VERT, &si, TRUE);

    FORWARD_WM_SIZE(hWnd, state, cx, cy, DefWindowProc);
}

///* void Cls_OnVScroll(HWND hwnd, HWND hwndCtl, UINT code, int pos) */
//#define HANDLE_WM_VSCROLL(hwnd, wParam, lParam, fn) \
//    ((fn)((hwnd), (HWND)(lParam), (UINT)(LOWORD(wParam)),  (int)(short)HIWORD(wParam)), 0L)
//#define FORWARD_WM_VSCROLL(hwnd, hwndCtl, code, pos, fn) \
//    (void)(fn)((hwnd), WM_VSCROLL, MAKEWPARAM((UINT)(int)(code), (UINT)(int)(pos)), (LPARAM)(HWND)(hwndCtl))
void Cls_OnVScroll(HWND hWnd, HWND hWndCtl, UINT code, int pos)
{
    // Get current scroll info
    SCROLLINFO si = { 0 };
    si.cbSize = sizeof(si);
    si.fMask  = SIF_ALL;
    GetScrollInfo(hWnd, SB_VERT, &si);
    int iVertPos = si.nPos;

    switch(code)
    {
    case SB_TOP:
        si.nPos = si.nMin;
        break;
    case SB_BOTTOM:
        si.nPos = si.nMax;
        break;
    case SB_LINEUP:
        si.nPos -= 1;
        break;
    case SB_LINEDOWN:
        si.nPos += 1;
        break;
    case SB_PAGEUP:
        si.nPos -= si.nPage;
        break;
    case SB_PAGEDOWN:
        si.nPos += si.nPage;
        break;
    case SB_THUMBPOSITION:
        si.nPos += si.nMin;
        break;
    case SB_THUMBTRACK:
        si.nPos = si.nTrackPos;
        break;
    default:
        break;
    }

    // Set/get the position - Windows may have adjusted the set values.
    si.fMask = SIF_POS;
    SetScrollInfo(hWnd, SB_VERT, &si, TRUE);
    GetScrollInfo(hWnd, SB_VERT, &si);

    // Scroll the window and update if the position has changed
    if (si.nPos != iVertPos)
    {
		int cxChar = 0, cxCaps = 0, cyChar = 0;
		GetFontCaps(hWnd, &cxChar, &cxCaps, &cyChar);
		ScrollWindowEx(hWnd, 0, cyChar * (iVertPos - si.nPos), 
			NULL, NULL, NULL, NULL,
			SW_ERASE | SW_INVALIDATE);
    }

    FORWARD_WM_VSCROLL(hWnd, hWndCtl, code, pos, DefWindowProc);
}

///* void Cls_OnHScroll(HWND hwnd, HWND hwndCtl, UINT code, int pos) */
//#define HANDLE_WM_HSCROLL(hwnd, wParam, lParam, fn) \
//    ((fn)((hwnd), (HWND)(lParam), (UINT)(LOWORD(wParam)), (int)(short)HIWORD(wParam)), 0L)
//#define FORWARD_WM_HSCROLL(hwnd, hwndCtl, code, pos, fn) \
//    (void)(fn)((hwnd), WM_HSCROLL, MAKEWPARAM((UINT)(int)(code),(UINT)(int)(pos)), (LPARAM)(HWND)(hwndCtl))
void Cls_OnHScroll(HWND hWnd, HWND hWndCtl, UINT code, int pos)
{
    // Get current scroll info
    SCROLLINFO si = { 0 };
    si.cbSize = sizeof(si);
    si.fMask  = SIF_ALL;
    GetScrollInfo(hWnd, SB_HORZ, &si);
    int iHorzPos = si.nPos;

    switch(code)
    {
    case SB_LEFT:
        si.nPos = si.nMin;
        break;
    case SB_RIGHT:
        si.nPos = si.nMax;
        break;
    case SB_LINELEFT:
        si.nPos -= 1;
        break;
    case SB_LINERIGHT:
        si.nPos += 1;
        break;
    case SB_PAGELEFT:
        si.nPos -= si.nPage;
        break;
    case SB_PAGERIGHT:
        si.nPos += si.nPage;
        break;
    case SB_THUMBPOSITION:
        si.nPos += si.nMin;
        break;
    case SB_THUMBTRACK:
        si.nPos = si.nTrackPos;
        break;
    default:
        break;
    }

    // Set/get the position - Windows may have adjusted the set values.
    si.fMask = SIF_POS;
    SetScrollInfo(hWnd, SB_HORZ, &si, TRUE);
    GetScrollInfo(hWnd, SB_HORZ, &si);

    // Scroll the window if the position has changed
    if (si.nPos != iHorzPos)
    {
		int cxChar = 0, cxCaps = 0, cyChar = 0;
		GetFontCaps(hWnd, &cxChar, &cxCaps, &cyChar);
		ScrollWindowEx(hWnd, cxChar * (iHorzPos - si.nPos), 0, 
			NULL, NULL, NULL, NULL,
			SW_ERASE | SW_INVALIDATE);
    }

    FORWARD_WM_HSCROLL(hWnd, hWndCtl, code, pos, DefWindowProc);
}

///* void Cls_OnDestroy(HWND hwnd) */
//#define HANDLE_WM_DESTROY(hwnd, wParam, lParam, fn) \
//    ((fn)(hwnd), 0L)
//#define FORWARD_WM_DESTROY(hwnd, fn) \
//    (void)(fn)((hwnd), WM_DESTROY, 0L, 0L)
//
void Cls_OnDestroy(HWND hWnd)
{
    PostQuitMessage(0);

    FORWARD_WM_DESTROY(hWnd, DefWindowProc);
}

///* BOOL Cls_OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct) */
//#define HANDLE_WM_CREATE(hwnd, wParam, lParam, fn) \
//    ((fn)((hwnd), (LPCREATESTRUCT)(lParam)) ? 0L : (LRESULT)-1L)
//#define FORWARD_WM_CREATE(hwnd, lpCreateStruct, fn) \
//    (BOOL)(DWORD)(fn)((hwnd), WM_CREATE, 0L, (LPARAM)(LPCREATESTRUCT)(lpCreateStruct))
//
BOOL Cls_OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct)
{
    // Winlog needs the handle to the writing window to signal a write update needed
    winLog.SetWindow(hWnd);

    return FALSE;
    //return FORWARD_WM_CREATE(hWnd, lpCreateStruct, DefWindowProc);
}

// Select the system ANSI FIXED FONT, then get its x/y sizes
//
bool GetFontCaps(HWND hWnd, int *cxChar, int *cxCaps, int *cyChar)
{
    TEXTMETRIC tm;

    HDC hdc = GetDC(hWnd);
    HGDIOBJ hFnt = 0, hOldFnt = 0;
    hFnt = GetStockObject(ANSI_FIXED_FONT);
    hOldFnt = SelectObject(hdc, hFnt);

	// If we can't get the ANSI FIXED FONT, get whatever is on the system
	GetTextMetrics(hdc, &tm);
	*cxChar = tm.tmAveCharWidth;
	*cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * *cxChar / 2;
	*cyChar = tm.tmHeight + tm.tmExternalLeading;

	// If we DID get the FIXED FONT, restore the original font
    if (hOldFnt)
    {
        SelectObject(hdc, hOldFnt);
    }
    ReleaseDC(hWnd, hdc);

	return true;
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

