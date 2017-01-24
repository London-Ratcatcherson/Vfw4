// WinLogger.h : include file for the WinLogger object
//

#ifndef _WINLOGGER_
#define _WINLOGGER_

using std::vector;
using std::wstring;

// WinLogger records diagnostic/information messages.
// The log is a vector of wstring, with one input TCHAR buffer.
// 
// The critical section is for atomic I/O
//   
class WinLogger 
{
public:
    // c'tor
    WinLogger() : 
        m_bTimeStamp(false), m_bWriteLogfile(true), 
        m_wsEmpty(), m_wsLine(), m_dLength(0),
        iBegin(m_vsLog.begin()), iEnd(m_vsLog.end()),
        rBegin(m_vsLog.rbegin()), rEnd(m_vsLog.rend()),
        m_wsLogfile(), m_hLogfile(NULL), m_hInstance(NULL),
        m_hWnd(NULL)
    { 
        InitializeCriticalSection(&m_crit);
    }
    // d'tor
    ~WinLogger() 
    { 
        CloseHandle(m_hLogfile);
        DeleteCriticalSection(&m_crit);
    }

    // Initialize the logfile with user provided name
    // Default is module name with .txt extension
    //
    bool Init(const TCHAR *pOutputFilename, HINSTANCE hInstance)
    {
		TCHAR *pPath = NULL;

		// save the global instance
		m_hInstance = hInstance;

		// do .. while (false) loop to allow easy break for error paths
		do
		{
			// if user provided pOutputFilename is valid, use that and return
			m_hLogfile = CreateFile(pOutputFilename,        // filename
				GENERIC_WRITE,                              // open access
				FILE_SHARE_READ,                            // share access
				NULL,                                       // security descriptor
				CREATE_ALWAYS,                              // creation disposition
				FILE_ATTRIBUTE_NORMAL,                      // flags
				NULL);                                      // template
			if (INVALID_HANDLE_VALUE != m_hLogfile)
			{
				// The filename is valid
				m_wsLogfile = pOutputFilename;
				return true;
			}

			// Initialize a string for the path
			pPath = new TCHAR [MAX_PATH];
			if (NULL == pPath)
			{
				break;
			}
			memset(pPath, 0, MAX_PATH * sizeof(pPath[0]));

			// Get the base current directory
			DWORD dwRet = GetModuleFileName(hInstance, pPath, MAX_PATH);
			if ((0 == dwRet) || (ERROR_INSUFFICIENT_BUFFER == dwRet))
			{
				break;
			}

			// find the last period in the path
			size_t nDex = 0;
			StringCchLength(pPath, MAX_PATH, &nDex);
			if (0 == nDex)
			{
				break;
			}

			while ('.' != pPath[nDex - 1])
			{
				--nDex;
			}
			// Zero-term the path right after that period
			pPath[nDex] = 0;

			// Append the txt to module name
			StringCchCat(pPath, MAX_PATH, _T("txt"));

			// Create the log file
			m_hLogfile = CreateFile(pPath,					// filename
				GENERIC_WRITE,                              // open access
				FILE_SHARE_READ,                            // share access
				NULL,                                       // security descriptor
				CREATE_ALWAYS,                              // creation disposition
				FILE_ATTRIBUTE_NORMAL,                      // flags
				NULL);                                      // template
			if (INVALID_HANDLE_VALUE != m_hLogfile)
			{
				// We have a logfile
				m_wsLogfile = pPath;
				delete [] pPath;
				// This is the function exit
				return true;
			}
		} while (false);

		// Not a fatal error, but we won't have a logfile
		delete [] pPath;
        return false;
    }

    // write to log using a resource string
	// $$ Need to write this so that we don't repeat a lot of the TCHAR * writeLog
 //   void writelog(const UINT uID, ...)
	//{
	//	LPTSTR lpBuffer = NULL;
	//	int cchBufferMax = 0;

	//	LoadString(m_hInstance, uID, lpBuffer, cchBufferMax);
	//}

    // write to log using a string or formatted straing
    void writelog(const TCHAR *szFormat, ...)
    {
        // Lock the write
        EnterCriticalSection(&m_crit);

        // Clear input buffer and line
        memset(m_szLine, 0, (MAX_PATH) * sizeof(m_szLine[0]));
        m_wsLine.clear();

        // Preface time stamp, if requested
        if (m_bTimeStamp)
        {
            SYSTEMTIME stLocal;
            TCHAR szTime[MAX_PATH] = { 0 };
            DWORD dwSize = MAX_PATH;
            GetLocalTime(&stLocal);
            DWORD dwRet = StringCchPrintf(szTime, dwSize, 
                TEXT("%02d/%02d/%d  %02d:%02d:%02d    "),
                stLocal.wMonth, stLocal.wDay, stLocal.wYear,
                stLocal.wHour, stLocal.wMinute, stLocal.wSecond);
            if( S_OK == dwRet )
            {
                m_wsLine.append(szTime);
            }
        }

        // parse the string format if it's non-null
        if (szFormat)
        {
            try
            {
                va_list vargp;
                va_start(vargp, szFormat);
                _vsnwprintf_s(m_szLine, (MAX_PATH), (MAX_PATH) - 1, szFormat, vargp);
                va_end(vargp);
            }
            catch (...)
            {
                // Could not parse line, so just print a blank line
                m_szLine[0] = 0;
            }
        }

        // Now write the formated line to the log input line
        m_wsLine.append(m_szLine);
        AddLine();

        // Done
        LeaveCriticalSection(&m_crit);
        return;
    }

    // read from log
    LPCWSTR readlog(size_t lineNum)
    {
        // return empty string on out of range request
        if (lineNum >= m_vsLog.size())
        {
            return m_wsEmpty.c_str();
        }
        return m_vsLog[lineNum].c_str();
    }

    // return length of specified line
    size_t lineLength(size_t lineNum)
    {
        // return empty string on out of range request
        if (lineNum >= m_vsLog.size())
        {
            return m_wsEmpty.length();
        }
        return m_vsLog[lineNum].length();
    }

    // return number of lines in log
    size_t logSize()
    {
        return m_vsLog.size();
    }

    // Turn timestamping on/off
    bool TimeStamp(bool bStamp)
    {
        return (m_bTimeStamp = bStamp);
    }
    // Turn logfile write on/off
    bool WriteLogfile(bool bWriteLogfile)
    {
        return (m_bWriteLogfile = bWriteLogfile);
    }
    // Set the window to be written to
    void SetWindow(HWND hWnd)
    {
        m_hWnd = hWnd;
        return;
    }
    // Get longest line length
    size_t GetLongestLineLength()
    {
        return m_dLength;
    }

protected:
    // no copies
    WinLogger(const WinLogger&) { }
    // no assignment
    WinLogger& operator =(const WinLogger&) { }

    // Add completed line to log, update iters and write to file if flag set
    void AddLine()
    {
        // Add the completed wsLine to vsLog
        m_vsLog.push_back(m_wsLine);
        // Update forward and reverse iters
        iBegin = m_vsLog.begin();
        iEnd = m_vsLog.end();
        rBegin = m_vsLog.rbegin();
        rEnd = m_vsLog.rend();

        // Save length of longest line in log for horizontal scrolling
        size_t dLength = m_wsLine.size();
        if (dLength > m_dLength)
        {
            m_dLength = dLength;
        }

        if ((m_bWriteLogfile) && (m_hLogfile))
        {
            // Zero-init file write buffer and copy our wchar string
            memset(m_szFileWrite, 0, (MAX_PATH));
            size_t nCharLen(0);
            size_t sWideLen = m_wsLine.size() + 1;
            wcstombs_s(&nCharLen, m_szFileWrite, sWideLen, m_wsLine.c_str(), (MAX_PATH));

            // Add CR/LF to line, and write to file
            m_szFileWrite[nCharLen - 1] = '\r';
            m_szFileWrite[nCharLen++] = '\n';
            DWORD dwBytesWritten = 0;
            BOOL fSuccess = WriteFile(m_hLogfile, m_szFileWrite, static_cast<DWORD> (nCharLen), &dwBytesWritten, NULL);
        }
        // If we know the window we're writing to, ask it to update
        if (m_hWnd)
        {
            InvalidateRect(m_hWnd, NULL, true);
        }
        return;
    }

private:
    vector<wstring> m_vsLog;            // vector of strings to hold log
    wstring m_wsLine;                   // log input line
    wstring m_wsEmpty;                  // empty string for out of range returns

    // Use preallocated arrays to optimize for speed
    TCHAR m_szLine[MAX_PATH];       // the log input buffer
    char m_szFileWrite[MAX_PATH];   // output print buffer for ANSI writes
    size_t m_dLength;                   // longest log line length

    bool m_bTimeStamp;                  // Timestamp each line?
    bool m_bWriteLogfile;               // Write log to a file?

    HWND m_hWnd;                        // handle to window to write on
    HANDLE m_hLogfile;                  // handle to logfile
	HINSTANCE m_hInstance;				// handle to application instance
    wstring m_wsLogfile;                // Complete logfile name (including path)

    CRITICAL_SECTION m_crit;            // Critical section for multithread safety

    // Forward and reverse iterators
    vector<wstring>::iterator iBegin;           // FIRST element in arry 
    vector<wstring>::iterator iEnd;             // LAST element in array
    vector<wstring>::reverse_iterator rBegin;   // LAST element in array
    vector<wstring>::reverse_iterator rEnd;     // FIRST element in array
};

#endif
