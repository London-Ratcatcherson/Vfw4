// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <windowsX.h>
#include <ole2.h>
#include <commctrl.h>
#include <shlwapi.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <process.h>	// for _beginthreadex, _endthreadex

// TODO: reference additional headers your program requires here

// Need to include these BEFORE strsafe.h
#include <vfw.h>
#include <string>
#include <vector>

#include <strsafe.h>
#include "WinLogger.h"
#include "resource.h"

// ID for the Preview window hMenu
#define CAP_WINDOW_ID 1297

