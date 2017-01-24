// VfwUtility.cpp
// Structure dumpers from VFW.H from the Windows SDK and other 
//   helper functions for Vfw development
//

#include "stdafx.h"

extern WinLogger winLog;	// debugging output log object

// Return string corresponding to TRUE or FALSE
TCHAR *TrueOrFalse(BOOL bTF)
{
    if (TRUE == bTF)
    {
        return _T("true ");
    }
    return _T("false");
}

// Log the current Vfw CAPDRIVERCAPS structure
//typedef struct tagCapDriverCaps {
//    UINT        wDeviceIndex;               // Driver index in system.ini
//    BOOL        fHasOverlay;                // Can device overlay?
//    BOOL        fHasDlgVideoSource;         // Has Video source dlg?
//    BOOL        fHasDlgVideoFormat;         // Has Format dlg?
//    BOOL        fHasDlgVideoDisplay;        // Has External out dlg?
//    BOOL        fCaptureInitialized;        // Driver ready to capture?
//    BOOL        fDriverSuppliesPalettes;    // Can driver make palettes?
//
//// following always NULL on Win32.
//    HANDLE      hVideoIn;                   // Driver In channel
//    HANDLE      hVideoOut;                  // Driver Out channel
//    HANDLE      hVideoExtIn;                // Driver Ext In channel
//    HANDLE      hVideoExtOut;               // Driver Ext Out channel
//} CAPDRIVERCAPS, *PCAPDRIVERCAPS, FAR *LPCAPDRIVERCAPS;
//
BOOL dumpCAPDRIVERCAPS(PCAPDRIVERCAPS pCaps)
{
	// Check pointer is valid
	if (NULL == pCaps)
	{
		return FALSE;
	}
    winLog.writelog(_T("CAPDRIVERCAPS :"));
    winLog.writelog(_T("wDeviceIndex;                   0x%.4x     // Driver index in system.ini "), pCaps->wDeviceIndex);
    winLog.writelog(_T("fHasOverlay;                    %s      // Can device overlay?        "), TrueOrFalse(pCaps->fHasOverlay));
    winLog.writelog(_T("fHasDlgVideoSource;             %s      // Has Video source dlg?      "), TrueOrFalse(pCaps->fHasDlgVideoSource));
    winLog.writelog(_T("fHasDlgVideoFormat;             %s      // Has Format dlg?            "), TrueOrFalse(pCaps->fHasDlgVideoFormat));
    winLog.writelog(_T("fHasDlgVideoDisplay;            %s      // Has External out dlg?      "), TrueOrFalse(pCaps->fHasDlgVideoDisplay));
    winLog.writelog(_T("fCaptureInitialized;            %s      // Driver ready to capture?   "), TrueOrFalse(pCaps->fCaptureInitialized));
    winLog.writelog(_T("fDriverSuppliesPalettes;        %s      // Can driver make palettes?  "), TrueOrFalse(pCaps->fDriverSuppliesPalettes));
    winLog.writelog(_T(""));
    winLog.writelog(_T("The following are always NULL on Win32"));
    winLog.writelog(_T("hVideoIn;                       0x%.4x     // Driver In channel          "), pCaps->hVideoIn);
    winLog.writelog(_T("hVideoOut;                      0x%.4x     // Driver Out channel         "), pCaps->hVideoOut);
    winLog.writelog(_T("hVideoExtIn;                    0x%.4x     // Driver Ext In channel      "), pCaps->hVideoExtIn);
    winLog.writelog(_T("hVideoExtOut;                   0x%.4x     // Driver Ext Out channel     "), pCaps->hVideoExtOut);
    winLog.writelog(_T(""));

    return TRUE;
}

// Log the current Vfw CAPDRIVERCAPS structure
//typedef struct tagCapStatus {
//    UINT        uiImageWidth;               // Width of the image
//    UINT        uiImageHeight;              // Height of the image
//    BOOL        fLiveWindow;                // Now Previewing video?
//    BOOL        fOverlayWindow;             // Now Overlaying video?
//    BOOL        fScale;                     // Scale image to client?
//    POINT       ptScroll;                   // Scroll position
//    BOOL        fUsingDefaultPalette;       // Using default driver palette?
//    BOOL        fAudioHardware;             // Audio hardware present?
//    BOOL        fCapFileExists;             // Does capture file exist?
//    DWORD       dwCurrentVideoFrame;        // # of video frames cap'td
//    DWORD       dwCurrentVideoFramesDropped;// # of video frames dropped
//    DWORD       dwCurrentWaveSamples;       // # of wave samples cap'td
//    DWORD       dwCurrentTimeElapsedMS;     // Elapsed capture duration
//    HPALETTE    hPalCurrent;                // Current palette in use
//    BOOL        fCapturingNow;              // Capture in progress?
//    DWORD       dwReturn;                   // Error value after any operation
//    UINT        wNumVideoAllocated;         // Actual number of video buffers
//    UINT        wNumAudioAllocated;         // Actual number of audio buffers
//} CAPSTATUS, *PCAPSTATUS, FAR *LPCAPSTATUS;
//
BOOL dumpCAPSTATUS(PCAPSTATUS pCaps)
{
	// Check pointer is valid
	if (NULL == pCaps)
	{
		return FALSE;
	}
    winLog.writelog(_T("CAPSTATUS :"));
    winLog.writelog(_T("uiImageWidth;             %.4d  0x%.4x   // Width of the image "), pCaps->uiImageWidth);
    winLog.writelog(_T("uiImageHeight;            %.4d  0x%.4x   // Height of the image "), pCaps->uiImageHeight);
    winLog.writelog(_T("fLiveWindow;                    %s      // Now Previewing video?"), TrueOrFalse(pCaps->fLiveWindow));
    winLog.writelog(_T("fOverlayWindow;                 %s      // Now Overlaying video?"), TrueOrFalse(pCaps->fOverlayWindow));
    winLog.writelog(_T("fScale;                         %s      // Scale image to client?"), TrueOrFalse(pCaps->fScale));
    winLog.writelog(_T("ptScroll;               x: %.4d y: %.4d    // Scroll position"), pCaps->ptScroll.x, pCaps->ptScroll.y);
    winLog.writelog(_T("fUsingDefaultPalette;           %s      // Using default driver palette?"), TrueOrFalse(pCaps->fUsingDefaultPalette));
    winLog.writelog(_T("fAudioHardware;                 %s      // Audio hardware present?"), TrueOrFalse(pCaps->fAudioHardware));
    winLog.writelog(_T("fCapFileExists;                 %s      // Does capture file exist?"), TrueOrFalse(pCaps->fCapFileExists));
    winLog.writelog(_T("dwCurrentVideoFrame;      %.4d  0x%.4x     // # of video frames cap'td "), 
        pCaps->dwCurrentVideoFrame, pCaps->dwCurrentVideoFrame);
    winLog.writelog(_T("dwCurrentVideoFramesDropped;    0x%.4x     // # of video frames dropped "), pCaps->dwCurrentVideoFramesDropped);
    winLog.writelog(_T("dwCurrentWaveSamples;           0x%.8x // # of wave samples cap'td "), pCaps->dwCurrentWaveSamples);
    winLog.writelog(_T("dwCurrentTimeElapsedMS;         0x%.8x // Elapsed capture duration "), pCaps->dwCurrentTimeElapsedMS);               // Milliseconds, hex
    winLog.writelog(_T("dwCurrentTimeElapsedMS;         %.2d:%.2d:%.3d  // Elapsed capture duration "), 
        pCaps->dwCurrentTimeElapsedMS / (1000 * 60), pCaps->dwCurrentTimeElapsedMS / (100 * 60),  // Minutes, seconds
        pCaps->dwCurrentTimeElapsedMS % 1000);               // Milliseconds
    winLog.writelog(_T("hPalCurrent;                    0x%.8x // Current palette in use"), pCaps->hPalCurrent);
    winLog.writelog(_T("fCapturingNow;                  %s      // Capture in progress?"), TrueOrFalse(pCaps->fCapturingNow));
    winLog.writelog(_T("dwReturn;                       0x%.4x     // Error value after any operation "), pCaps->dwReturn);
    winLog.writelog(_T("wNumVideoAllocated;             0x%.4x     // Actual number of video buffers "), pCaps->wNumVideoAllocated);
    winLog.writelog(_T("wNumAudioAllocated;             0x%.4x     // Actual number of audio buffers "), pCaps->wNumAudioAllocated);
    winLog.writelog(_T(""));
   
    return TRUE;
}

// Log the current Vfw CAPTUREPARMS structure
//typedef struct tagCaptureParms {
//    DWORD       dwRequestMicroSecPerFrame;  // Requested capture rate
//    BOOL        fMakeUserHitOKToCapture;    // Show "Hit OK to cap" dlg?
//    UINT        wPercentDropForError;       // Give error msg if > (10%)
//    BOOL        fYield;                     // Capture via background task?
//    DWORD       dwIndexSize;                // Max index size in frames (32K)
//    UINT        wChunkGranularity;          // Junk chunk granularity (2K)
//    BOOL        fUsingDOSMemory;            // Use DOS buffers?
//    UINT        wNumVideoRequested;         // # video buffers, If 0, autocalc
//    BOOL        fCaptureAudio;              // Capture audio?
//    UINT        wNumAudioRequested;         // # audio buffers, If 0, autocalc
//    UINT        vKeyAbort;                  // Virtual key causing abort
//    BOOL        fAbortLeftMouse;            // Abort on left mouse?
//    BOOL        fAbortRightMouse;           // Abort on right mouse?
//    BOOL        fLimitEnabled;              // Use wTimeLimit?
//    UINT        wTimeLimit;                 // Seconds to capture
//    BOOL        fMCIControl;                // Use MCI video source?
//    BOOL        fStepMCIDevice;             // Step MCI device?
//    DWORD       dwMCIStartTime;             // Time to start in MS
//    DWORD       dwMCIStopTime;              // Time to stop in MS
//    BOOL        fStepCaptureAt2x;           // Perform spatial averaging 2x
//    UINT        wStepCaptureAverageFrames;  // Temporal average n Frames
//    DWORD       dwAudioBufferSize;          // Size of audio bufs (0 = default)
//    BOOL        fDisableWriteCache;         // Attempt to disable write cache
//    UINT        AVStreamMaster;             // Which stream controls length?
//} CAPTUREPARMS, *PCAPTUREPARMS, FAR *LPCAPTUREPARMS;
//
BOOL dumpCAPTUREPARMS(PCAPTUREPARMS pCaps)
{
	// Check pointer is valid
	if (NULL == pCaps)
	{
		return FALSE;
	}
    winLog.writelog(_T("CAPTUREPARMS :"));
    winLog.writelog(_T("dwRequestMicroSecPerFrame;      0x%.4x    // Requested capture rate"), pCaps->dwRequestMicroSecPerFrame);
    winLog.writelog(_T("fMakeUserHitOKToCapture;        %s      // Show \"Hit OK to cap\" dlg?"), TrueOrFalse(pCaps->fMakeUserHitOKToCapture));
    winLog.writelog(_T("wPercentDropForError;           0x%.4x     // Give error msg if > (10%%)"), pCaps->wPercentDropForError);
    winLog.writelog(_T("fYield;                         %s      // Capture via background task?"), TrueOrFalse(pCaps->fYield));
    winLog.writelog(_T("dwIndexSize;                    0x%.4x     // Max index size in frames (32K)"), pCaps->dwIndexSize);
    winLog.writelog(_T("wChunkGranularity;              0x%.4x     // Junk chunk granularity (2K)"), pCaps->wChunkGranularity);
    winLog.writelog(_T("fUsingDOSMemory;                %s      // Use DOS buffers?"), TrueOrFalse(pCaps->fUsingDOSMemory));
    winLog.writelog(_T("wNumVideoRequested;             0x%.4x     // # video buffers, If 0, autocalc"), pCaps->wNumVideoRequested);
    winLog.writelog(_T("fCaptureAudio;                  %s      // Capture audio?"), TrueOrFalse(pCaps->fCaptureAudio));
    winLog.writelog(_T("wNumAudioRequested;             0x%.4x     // # audio buffers, If 0, autocalc"), pCaps->wNumAudioRequested);
    winLog.writelog(_T("vKeyAbort;                      0x%.4x     // Virtual key causing abort"), pCaps->vKeyAbort);
    winLog.writelog(_T("fAbortLeftMouse;                %s      // Abort on left mouse?"), TrueOrFalse(pCaps->fAbortLeftMouse));
    winLog.writelog(_T("fAbortRightMouse;               %s      // Abort on right mouse?"), TrueOrFalse(pCaps->fAbortRightMouse));
    winLog.writelog(_T("fLimitEnabled;                  %s      // Use wTimeLimit?"), TrueOrFalse(pCaps->fLimitEnabled));
    winLog.writelog(_T("wTimeLimit;                     0x%.4x     // Seconds to capture"), pCaps->wTimeLimit);
    winLog.writelog(_T("fMCIControl;                    %s      // Use MCI video source?"), TrueOrFalse(pCaps->fMCIControl));
    winLog.writelog(_T("fStepMCIDevice;                 %s      // Step MCI device?"), TrueOrFalse(pCaps->fStepMCIDevice));
    winLog.writelog(_T("dwMCIStartTime;                 0x%.4x     // Time to start in MS"), pCaps->dwMCIStartTime);
    winLog.writelog(_T("dwMCIStopTime;                  0x%.4x     // Time to stop in MS"), pCaps->dwMCIStopTime);
    winLog.writelog(_T("fStepCaptureAt2x;               %s      // Perform spatial averaging 2x"), TrueOrFalse(pCaps->fStepCaptureAt2x));
    winLog.writelog(_T("wStepCaptureAverageFrames;      0x%.4x     // Temporal average n Frames"), pCaps->wStepCaptureAverageFrames);
    winLog.writelog(_T("dwAudioBufferSize;              0x%.4x     // Size of audio bufs (0 = default)"), pCaps->dwAudioBufferSize);
    winLog.writelog(_T("fDisableWriteCache;             %s      // Attempt to disable write cache"), TrueOrFalse(pCaps->fDisableWriteCache));
    winLog.writelog(_T("AVStreamMaster;                 0x%.4x     // Which stream controls length?"), pCaps->AVStreamMaster);
    winLog.writelog(_T(""));
   
    return TRUE;
}

// Set Capture filename
// Default is current directory plus "VfwCap00.avi", with subsequent captures sequentially incremented
//
BOOL SetCaptureFilename(HWND hWnd, HWND hWndCap, TCHAR *szCapFilename, int *nCapFileIndex)
{
    // Get the base current directory
	TCHAR pPath [MAX_PATH + 1] = { 0 };
    TCHAR pCapFile [MAX_PATH + 1] = { 0 };

    // Get the application path (location and filename)
	HINSTANCE hInst = reinterpret_cast <HINSTANCE> (GetWindowLongPtr(hWnd, GWLP_HINSTANCE));
    if ((0 == GetModuleFileName(hInst, pPath, MAX_PATH)) || (0 == *pPath)) 
    {
		return FALSE;
	}
    // find the last backslash in the path
    size_t nDex = 0;
    StringCchLength(pPath, MAX_PATH, &nDex);
    while ('\\' != pPath[nDex - 1])
    {
        --nDex;
    }
    // Zero-term the path right after that slash to get the path
    pPath[nDex] = 0;

	// Load the base capture filename - "VfwCap00"
    LoadString(hInst, IDS_CAPTURE_FILENAME, pCapFile, MAX_PATH);
    StringCchLength(pCapFile, MAX_PATH, &nDex);

    // Update the base name with the current sequential counter
    pCapFile[nDex - 2] = '0' + ((*nCapFileIndex) / 10);
    pCapFile[nDex - 1] = '0' + ((*nCapFileIndex) % 10);

    // Increment sequential file counter
    ++(*nCapFileIndex);

    // Concatenate the path, Capture filename and file extension
    StringCchCopy(szCapFilename, MAX_PATH, pPath);
    StringCchCat(szCapFilename, MAX_PATH, pCapFile);
    StringCchCat(szCapFilename, MAX_PATH, _T(".avi"));

    // Set this as capture file
    capFileSetCaptureFile(hWndCap, szCapFilename);

    return TRUE;
}

// Set DIB filename
// Default is current directory plus "VfwDIB00.dib", with subsequent captures sequentially incremented
//
BOOL SetCapDIBFilename(HWND hWnd, TCHAR *szCapFilename, int *nCapFileIndex)
{
    // Get the base current directory
	TCHAR pPath [MAX_PATH + 1] = { 0 };
    TCHAR pCapFile [MAX_PATH + 1] = { 0 };

    // Get the application path (location and filename)
	HINSTANCE hInst = reinterpret_cast <HINSTANCE> (GetWindowLongPtr(hWnd, GWLP_HINSTANCE));
    if ((0 == GetModuleFileName(hInst, pPath, MAX_PATH)) || (0 == *pPath)) 
    {
		return FALSE;
	}
    // find the last backslash in the path
    size_t nDex = 0;
    StringCchLength(pPath, MAX_PATH, &nDex);
    while ('\\' != pPath[nDex - 1])
    {
        --nDex;
    }
    // Zero-term right after that slash to get the path
    pPath[nDex] = 0;

	// Load the base capture filename - "VfwDIB00"
    LoadString(hInst, IDS_DIB_FILENAME, pCapFile, MAX_PATH);
    StringCchLength(pCapFile, MAX_PATH, &nDex);

    // Update the base name with the current sequential counter
    pCapFile[nDex - 2] = '0' + ((*nCapFileIndex) / 10);
    pCapFile[nDex - 1] = '0' + ((*nCapFileIndex) % 10);

    // Increment sequential file counter
    ++(*nCapFileIndex);

    // Concatenate the path, Capture filename and file extension
    StringCchCopy(szCapFilename, MAX_PATH, pPath);
    StringCchCat(szCapFilename, MAX_PATH, pCapFile);
    StringCchCat(szCapFilename, MAX_PATH, _T(".dib"));

	// The DIB filename is returned to the caller to use

	return TRUE;
}

// Thread function to get and update current video processing status
// sleep is needed to keep function from overwhelming the logger 
unsigned __stdcall	ShowCaptureStatusThread(void *args)
{
	CAPSTATUS capStatus = { 0 };
	HWND *hWndCapChild = static_cast<HWND*> (args);

	do 
	{
		if (capGetStatus(*hWndCapChild, &capStatus, sizeof(CAPSTATUS)))
		{
			winLog.writelog(_T("fCapturingNow;                  %s      // Capture in progress?"), 
				TrueOrFalse(capStatus.fCapturingNow));
			winLog.writelog(_T("dwCurrentVideoFrame;      %.4d  0x%.4x     // # of video frames cap'td "), 
				capStatus.dwCurrentVideoFrame, capStatus.dwCurrentVideoFrame);
			winLog.writelog(_T("dwCurrentVideoFramesDropped;    0x%.4x     // # of video frames dropped "), 
				capStatus.dwCurrentVideoFramesDropped);
			
		}
		// Wait for 1 second and exit if we're not capturing anymore
		SleepEx(1000, TRUE);
	} while (capStatus.fCapturingNow);

	return 0;
}

//
//  FUNCTION: CapWndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK CapWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// We need this callback function for a unique video output window, 
	//   but we don't process any specific messages.
	// Add switch(message) if we need to process a message.
	return DefWindowProc(hWnd, message, wParam, lParam);
}

// 
// ResizeCaptureWindow - Resize the capture Window ....
// This function resizes the capture window based upon the Video Capture
// parameters found in the capStatus structure returned from Win32
// 
void ResizeCaptureWindow(HWND hWndChild, HWND hWndCap)
{
    CAPSTATUS    capStatus = { 0 };

    // Get the capture window attributes .. width and height
    capGetStatus(hWndCap, &capStatus, sizeof(capStatus));

    // Resize the capture window to the capture size
    SetWindowPos(hWndCap, NULL, 0, 0,
        capStatus.uiImageWidth,    // Image width from camera
        capStatus.uiImageHeight,   // Image height from camera
        SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOZORDER | SWP_SHOWWINDOW);

    // Resize the child window to the capture window 
    SetWindowPos(hWndChild, NULL, 4, 4,
        capStatus.uiImageWidth+8,     // Image width from camera
        capStatus.uiImageHeight+8,    // Image height from camera
        SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOZORDER | SWP_SHOWWINDOW);

    return;
}

LRESULT CALLBACK VideoStreamCallback(HWND hwndC, LPVIDEOHDR lpVHdr)
{
    //
    // Process Video Callbacks Here!!
    // (We get a pointer to the video buffer)
    //
	winLog.writelog(_T("Video callback, captured %d seconds"), lpVHdr->dwTimeCaptured/1000);
	// Sleep so other thread can access the winLog
	SleepEx(1000, TRUE);
	
	return (0);
}

// Process all the Vfw Video commands
void Cls_OnVideoCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify)
{
    BOOL				bSucceed = FALSE;
	HINSTANCE			hInst = NULL;
    TCHAR				*szStringOne = NULL;		// Two scratch strings
    TCHAR				*szStringTwo = NULL;
	static int			nCapFileIndex = 0;          // Capture file sequential name counter
	static int			nCapDIBFileIndex = 0;		// Capture DIB file sequential name counter
	static HWND			hWndCap = NULL;				// Handle to the Preview Window parent
	static HWND			hWndCapChild = NULL;		// Handle to the Preview Window
	unsigned int		threadID = 0;				// ID of the Capture helper thread
	static uintptr_t	hCapThread = NULL;			// Handle to the Capture helper thread

	switch (id)
    {
    case ID_CAP_PREVIEW:
		// This command enumerates video devices and loads one
		// If that succeeds, then it enables the menu items for device functionality
		// 
        winLog.writelog(_T("Capture start - Video for Windows version 0x%x %x"), 
            HIWORD(VideoForWindowsVersion()), LOWORD(VideoForWindowsVersion()) );

		// Get the title and class for our Capture window
		szStringOne = szStringTwo = NULL;
		szStringOne = new TCHAR[MAX_PATH + 1];
		szStringTwo = new TCHAR[MAX_PATH + 1];
		if ((NULL == szStringOne) || (NULL == szStringTwo))
		{
            DWORD dwError = GetLastError();
            winLog.writelog(_T("ERROR: memory allocation - Last Error 0x%.4x"), dwError);
            break;
		}
		memset(szStringOne, 0, MAX_PATH * sizeof(szStringOne[0]));
		memset(szStringTwo, 0, MAX_PATH * sizeof(szStringTwo[0]));
		hInst = reinterpret_cast <HINSTANCE> (GetWindowLongPtr(hWnd, GWLP_HINSTANCE));
		LoadString(hInst, IDC_VFW4_CAPWINCLASS, szStringOne, MAX_PATH);
        LoadString(hInst, IDS_CAP_WINDOW_TITLE, szStringTwo, MAX_PATH);

		// The Capture window is a child window inside a parent window
		// The main UI window could be the parent, but we create a new window to hold the capture window
		// The Capture window is separate from the main UI window and has it's own WndProc() 
		// 
        hWndCap = CreateWindow(
            szStringOne, szStringTwo, 
            WS_OVERLAPPED,
            0, 0, 0, 0, 
            NULL, NULL,					// hWndParent, hMenu
            hInst, 
            NULL);
		hWndCapChild = capCreateCaptureWindow(
            szStringTwo, 
            WS_CHILD | WS_VISIBLE, 
            0, 0, 0, 0, 
            hWndCap, CAP_WINDOW_ID);	// hWndParent, hMenu
		// Check for creation failure of either window 
        if ((NULL == hWndCapChild) || (NULL == hWndCap))
        {
            DWORD dwError = GetLastError();
            winLog.writelog(_T("ERROR: capCreateCaptureWindow - Last Error 0x%.4x"), dwError);
            break;
        }

        // Enumerate available Vfw devices
		// If there are other Vfw drivers, you'll see them, but modern Windows typically only has the MSVfw mapper
        for (int nDex = 0; nDex < 10; ++nDex)
        {
            if (capGetDriverDescription(nDex,
                szStringOne, MAX_PATH,
                szStringTwo, MAX_PATH))
            {
                winLog.writelog(_T("Vfw device : %s"), szStringOne);
                winLog.writelog(_T("Version    : %s"), szStringTwo);
				break;
            }
        }

        // Connect the capture window to the driver
        // If multiple devices, start with first until succeed OR reach Vfw limit (9)
		// If there is more than 1 available device, the capDriverConnect will either:
		//   - do nothing 
		//   - put up the Vfw Source select device dialog
		// Vfw is designed primarily for the 1 device only case, and handles multiple devices less well
		// 
        for (int nCam = 0; (FALSE == bSucceed) && (9 != nCam); ++nCam)
        {
            bSucceed = capDriverConnect(hWndCapChild, nCam);
        }
        // No camera loaded
        if (FALSE == bSucceed)
        {
            DWORD dwError = GetLastError();
            winLog.writelog(_T("ERROR: capDriverConnect - Last Error 0x%.4x"), dwError);
            winLog.writelog(_T("capDriverConnect in Windows 7 Vfw will fail to load if there are multiple capture devices"));
            winLog.writelog(_T(""));
            break;
        }

        // Send message to get driver caps and set menu asynchronously
		SendMessage(hWnd, WM_COMMAND, ID_VIDEO_GETCAPABILITIES, 0);

        // Resize the capture window to show whole image
		// Note that the Capture window is inside another window, so ask to show them both
        ResizeCaptureWindow(hWndCap, hWndCapChild);
        ShowWindow(hWndCap, SW_SHOWNORMAL);
        ShowWindow(hWndCapChild, SW_SHOWNORMAL);
        
        // Set the video stream callback function (we don't use this, but we could)
        bSucceed = capSetCallbackOnVideoStream(hWndCapChild, VideoStreamCallback);

		// Create an event for the capture thread to indicate that it's done 
		// (Decided not to use semaphores - instead, the capture thread polls)
		//g_hCapEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
        
		// Enable menu items for this device
		EnableMenuItem(GetMenu(hWnd), ID_CAP_CAPTURE,			MF_ENABLED);
		EnableMenuItem(GetMenu(hWnd), ID_CAP_CAPTURE_FRAME,		MF_ENABLED);

		EnableMenuItem(GetMenu(hWnd), ID_VIDEO_GETCAPABILITIES,	MF_ENABLED);
		EnableMenuItem(GetMenu(hWnd), ID_CAP_GET_STATUS,		MF_ENABLED);
		EnableMenuItem(GetMenu(hWnd), ID_CAP_GET_PARAMETERS,	MF_ENABLED);

		EnableMenuItem(GetMenu(hWnd), ID_CAP_COMPRESSION,		MF_ENABLED);

		// Start preview and set preview rate in milliseconds
        bSucceed = capPreview(hWndCapChild, TRUE);
        bSucceed = capPreviewRate(hWndCapChild, 25);
        break;
        
    case ID_CAP_RELEASE:
		// This command releases the video device and disables the device specific menu items
        winLog.writelog(_T("Capture driver release"));

		// Shut off the video callback and preview, then release device
		capSetCallbackOnVideoStream(hWndCapChild, NULL);
        capPreview(hWndCapChild, FALSE);
        bSucceed = capDriverDisconnect(hWndCapChild);

		// Disable menu items for this device
		EnableMenuItem(GetMenu(hWnd), ID_CAP_CAPTURE,			MF_DISABLED);
		EnableMenuItem(GetMenu(hWnd), ID_CAP_CAPTURE_FRAME,		MF_DISABLED);

		EnableMenuItem(GetMenu(hWnd), ID_VIDEO_GETCAPABILITIES,	MF_DISABLED);
		EnableMenuItem(GetMenu(hWnd), ID_CAP_GET_STATUS,		MF_DISABLED);
		EnableMenuItem(GetMenu(hWnd), ID_CAP_GET_PARAMETERS,	MF_DISABLED);

		EnableMenuItem(GetMenu(hWnd), ID_CAP_SOURCE,			MF_DISABLED);
		EnableMenuItem(GetMenu(hWnd), ID_CAP_FORMAT,			MF_DISABLED);
		EnableMenuItem(GetMenu(hWnd), ID_CAP_DISPLAY,			MF_DISABLED);
		EnableMenuItem(GetMenu(hWnd), ID_CAP_COMPRESSION,		MF_DISABLED);

        // Done with the windows, so destroy in reverse creation order
        DestroyWindow(hWndCapChild);
        DestroyWindow(hWndCap);
        break;

    case ID_CAP_CAPTURE:
		// Capture to a file
		szStringOne = new TCHAR[MAX_PATH + 1];
		if (NULL == szStringOne)
		{
            DWORD dwError = GetLastError();
            winLog.writelog(_T("ERROR: memory allocation - Last Error 0x%.4x"), dwError);
            break;
		}
		memset(szStringOne, 0, (MAX_PATH * sizeof(szStringOne[0])) + 1);

		// What is is the current capture file?
        bSucceed = capFileGetCaptureFile(hWndCapChild, szStringOne, MAX_PATH);
        winLog.writelog(_T("Current capture file is %s"), szStringOne); 

        // Set Capture filename (starting at "VfwCap00.avi")
		if (! SetCaptureFilename(hWnd, hWndCapChild, szStringOne, &nCapFileIndex))
		{
			break;
		}
        bSucceed = capFileGetCaptureFile(hWndCapChild, szStringOne, MAX_PATH);
        winLog.writelog(_T("Now current capture file is %s"), szStringOne); 

        // Set the capture parameters
		{
			CAPTUREPARMS captureParms = { 0 };
			if (capCaptureGetSetup(hWndCapChild, &captureParms, sizeof(CAPTUREPARMS)))
			{
				winLog.writelog(_T("start capCaptureSequence to %s"), szStringOne);
				// Use the OK to Capture dialog
				captureParms.fMakeUserHitOKToCapture = TRUE;	
				captureParms.fYield = TRUE;
				capCaptureSetSetup(hWndCapChild, &captureParms, sizeof(CAPTUREPARMS));
			}
		}

		// Start a thread to display the capture parameters
		hCapThread = _beginthreadex(NULL,	// SECURITY_ATTRIBUTES structure (for child process inheritance)
			0,								// Thread function stack size
			&ShowCaptureStatusThread,		// Thread function
			&hWndCapChild,					// arglist (address of Capture Window)
			0,								// initflag (0 for running, CREATE_SUSPENDED for suspended)
			&threadID);						// Thread identifier
		winLog.writelog(_T("Capture thread ID is 0x%x"), threadID);

        // Start streaming capture to disk (Capture is terminated with ESC key)
        bSucceed = capCaptureSequence(hWndCapChild);

        break;

    case ID_CAP_RESET:
		// Reset the thread semaphore and disable the reset menu item
		// (Not using a semaphore. Our capture status thread just polls for capture running)
		//ResetEvent(g_hCapEvent);
		//EnableMenuItem(GetMenu(hWnd), ID_CAP_RESET, MF_DISABLED);
		break;

    case ID_CAP_CAPTURE_FRAME:
		// Capture a single frame to a sequentially numbered file
		szStringOne = new TCHAR[MAX_PATH + 1];
		if (NULL == szStringOne)
		{
            DWORD dwError = GetLastError();
            winLog.writelog(_T("ERROR: memory allocation - Last Error 0x%.4x"), dwError);
            break;
		}
		memset(szStringOne, 0, (MAX_PATH * sizeof(szStringOne[0])) + 1);

		// Set DIB Capture filename (default is "VfwCap00.dib")
		if (! SetCapDIBFilename(hWnd, szStringOne, &nCapDIBFileIndex))
		{
			break;
		}
        winLog.writelog(_T("capture a frame to %s"), szStringOne);

		// Grab a single frame
        bSucceed = capFileSaveDIB(hWndCapChild, szStringOne);

        break;

    case ID_CAP_SOURCE:
        bSucceed = capDlgVideoSource(hWndCapChild);
        break;
    case ID_CAP_FORMAT:
        bSucceed = capDlgVideoFormat(hWndCapChild);
        ResizeCaptureWindow(hWndCap, hWndCapChild);
        break;
    case ID_CAP_DISPLAY:
        bSucceed = capDlgVideoDisplay(hWndCapChild);
        break;
    case ID_CAP_COMPRESSION:
        bSucceed = capDlgVideoCompression(hWndCapChild);
        break;

	case ID_CAP_GET_STATUS:
        // Get the capture status
		{
			CAPSTATUS capStatus = { 0 };
			if (capGetStatus(hWndCapChild, &capStatus, sizeof(CAPSTATUS)))
			{
				dumpCAPSTATUS(&capStatus);
			}
		}
        break;

	case ID_CAP_GET_PARAMETERS:
        // Get the capture parameters
		{
			CAPTUREPARMS captureParms = { 0 };
			if (capCaptureGetSetup(hWndCapChild, &captureParms, sizeof(CAPTUREPARMS)))
			{
				dumpCAPTUREPARMS(&captureParms);
			}
		}
        break;

	case ID_VIDEO_GETCAPABILITIES:
		{
			CAPDRIVERCAPS caps = { 0 };
			if (capDriverGetCaps(hWndCapChild, &caps, sizeof(caps)))
			{
				// If driver supports a dialog, enable it in menu
				if (caps.fHasDlgVideoSource)
				{
					EnableMenuItem(GetMenu(hWnd), ID_CAP_SOURCE, MF_ENABLED);
				}
				if (caps.fHasDlgVideoFormat)
				{
					EnableMenuItem(GetMenu(hWnd), ID_CAP_FORMAT, MF_ENABLED);
				}
				if (caps.fHasDlgVideoDisplay)
				{
					EnableMenuItem(GetMenu(hWnd), ID_CAP_DISPLAY, MF_ENABLED);
				}
				dumpCAPDRIVERCAPS(&caps);
			}
		}
        break;

    default:
        break;
    }

	// Cleanup any allocations - this is safe, even if no allocation was made
	delete [] szStringOne;
	delete [] szStringTwo;
	
	return;
}

// Process all the Vfw Audio commands
void Cls_OnAudioCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify)
{
    BOOL		bSucceed = TRUE;
	HINSTANCE	hInst = NULL;
	static HWND	hWndMCI = NULL;		// handle to the MCI window

	switch (id)
	{
	case ID_MCI_CREATE:
		// This draws the MCI control in the main UI window.
		// Functional, but it looks sloppy!
		// 
        winLog.writelog(_T("MCI Create"));
		hInst = reinterpret_cast <HINSTANCE> (GetWindowLongPtr(hWnd, GWLP_HINSTANCE));
        hWndMCI = MCIWndCreate(hWnd, hInst, 
			MCIWNDF_SHOWALL, 
            L"\\\\COLOSSUS\\Users\\Public\\Music\\Sample Music\\Kalimba.mp3");
        InvalidateRect(hWnd, NULL, TRUE);
        break;
	case ID_MCI_PLAY:
        winLog.writelog(_T("MCI Play"));
        MCIWndPlay(hWndMCI);
        break;
	case ID_MCI_PAUSE:
        winLog.writelog(_T("MCI Pause"));
        MCIWndPause(hWndMCI);
        break;
	case ID_MCI_STOP:
        winLog.writelog(_T("MCI Stop"));
        MCIWndStop(hWndMCI);
        InvalidateRect(hWnd, NULL, TRUE);
        break;
	case ID_MCI_RELEASE:
        winLog.writelog(_T("MCI Release"));
        MCIWndDestroy(hWndMCI);
        InvalidateRect(hWnd, NULL, TRUE);
        break;

    default:
        break;
    }

	return;
}

