#pragma once
#ifndef _TnbDOXYGEN	//Document作成用シンボル

//TNB;Inline化しました

/************************************************************
	Module Name: MeteredSection.h
	Author: Dan Chou
	Description: Defines the metered section synchronization object
************************************************************/


#define MAX_METSECT_NAMELEN 128


// Shared info needed for metered section
typedef struct _METSECT_SHARED_INFO 
{
	BOOL   fInitialized;     // Is the metered section initialized?
	LONG   lSpinLock;        // Used to gain access to this structure
	LONG   lThreadsWaiting;  // Count of threads waiting
	LONG   lAvailableCount;  // Available resource count
	LONG   lMaximumCount;    // Maximum resource count
} METSECT_SHARED_INFO, *LPMETSECT_SHARED_INFO;

// The opaque Metered Section data structure
typedef struct _METERED_SECTION 
{
	HANDLE hEvent;           // Handle to a kernel event object
	HANDLE hFileMap;         // Handle to memory mapped file
	LPMETSECT_SHARED_INFO lpSharedInfo;
} METERED_SECTION, *LPMETERED_SECTION;


/************************************************************
	Module Name: MeteredSection.
	Author: Dan Chou
	Description: Implements the metered section synchronization object
************************************************************/


/*
 * CreateMetSectEvent
 */
inline BOOL CreateMetSectEvent(LPMETERED_SECTION lpMetSect, 
		 LPCTSTR lpName, BOOL bOpenOnly)
{
	TCHAR sz[MAX_PATH];
	if (lpName)
	{
		wsprintf(sz, _T("DKC_MSECT_EVT_%s"), lpName);

#ifndef _WIN32_WCE
		if (bOpenOnly)
		{
			lpMetSect->hEvent = OpenEvent(0, FALSE, sz);
		}
		else
		{
			// Create an auto-reset named event object
			lpMetSect->hEvent = CreateEvent(NULL, FALSE, FALSE, sz);
		}
#else
		// Create an auto-reset named event object
		lpMetSect->hEvent = CreateEvent(NULL, FALSE, FALSE, sz);
#endif
	}
	else
	{
		// Create an auto-reset unnamed event object
		lpMetSect->hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	}
	return (lpMetSect->hEvent ? TRUE : FALSE);
}

/*
 * CreateMetSectFileView
 */
inline BOOL CreateMetSectFileView(LPMETERED_SECTION lpMetSect, 
		 LONG lInitialCount, LONG lMaximumCount,
						   LPCTSTR lpName, BOOL bOpenOnly)
{
	TCHAR sz[MAX_PATH];
	DWORD dwLastError; 

	if (lpName)
	{
		wsprintf(sz, _T("DKC_MSECT_MMF_%s"), lpName);

#ifndef _WIN32_WCE
		if (bOpenOnly)
		{
			lpMetSect->hFileMap = OpenFileMapping(0, FALSE, sz);
		}
		else
		{
			// Create a named file mapping
			lpMetSect->hFileMap = CreateFileMapping(INVALID_HANDLE_VALUE, 
				  NULL, PAGE_READWRITE, 0, sizeof(METSECT_SHARED_INFO), sz);
		}
#else
		// Create a named file mapping
		lpMetSect->hFileMap = CreateFileMapping(INVALID_HANDLE_VALUE, 
			  NULL, PAGE_READWRITE, 0, sizeof(METSECT_SHARED_INFO), sz);
#endif
	}
	else
	{
		// Create an unnamed file mapping
		lpMetSect->hFileMap = CreateFileMapping(INVALID_HANDLE_VALUE, 
		 NULL, PAGE_READWRITE, 0, sizeof(METSECT_SHARED_INFO), NULL);
	}

	// Map a view of the file
	if (lpMetSect->hFileMap)
	{
		dwLastError = GetLastError();
		lpMetSect->lpSharedInfo = (LPMETSECT_SHARED_INFO) 
		 MapViewOfFile(lpMetSect->hFileMap, FILE_MAP_WRITE, 0, 0, 0);
		if (lpMetSect->lpSharedInfo)
		{
			if (dwLastError != ERROR_ALREADY_EXISTS)
			{
				lpMetSect->lpSharedInfo->lSpinLock = 0;
				lpMetSect->lpSharedInfo->lThreadsWaiting = 0;
				lpMetSect->lpSharedInfo->lAvailableCount = lInitialCount;
				lpMetSect->lpSharedInfo->lMaximumCount = lMaximumCount;
				InterlockedExchange(( long *)&(lpMetSect->lpSharedInfo->fInitialized), TRUE);
			}
			else
			{   // Already exists; wait for it to be initialized by the creator
			  while (!lpMetSect->lpSharedInfo->fInitialized) Sleep(0);
			}
			return TRUE;
		}
	}
	return FALSE;
}

/*
 * InitMeteredSection
 */
inline BOOL InitMeteredSection(LPMETERED_SECTION lpMetSect, 
		 LONG lInitialCount, LONG lMaximumCount,
						LPCTSTR lpName, BOOL bOpenOnly)
{
	// Try to create the event object
	if (CreateMetSectEvent(lpMetSect, lpName, bOpenOnly))
	{
		// Try to create the memory mapped file
		if (CreateMetSectFileView(lpMetSect, 
		 lInitialCount, lMaximumCount, lpName, bOpenOnly))
		{
			return TRUE;
		}
	}

	// Error occured, return FALSE so the caller knows to clean up
	return FALSE;
}

/*
 * CloseMeteredSection
 */
inline void CloseMeteredSection(LPMETERED_SECTION lpMetSect)
{
	if (lpMetSect)
	{
		// Clean up
		if (lpMetSect->lpSharedInfo) 
		 UnmapViewOfFile(lpMetSect->lpSharedInfo);
		if (lpMetSect->hFileMap) CloseHandle(lpMetSect->hFileMap);
		if (lpMetSect->hEvent) CloseHandle(lpMetSect->hEvent);
		free(lpMetSect);
	}
}

/*
 * CreateMeteredSection
 */
inline LPMETERED_SECTION CreateMeteredSection(LONG lInitialCount, 
	  LONG lMaximumCount, LPCTSTR lpName)
{
	LPMETERED_SECTION lpMetSect;

	// Verify the parameters
	if ((lMaximumCount < 1)             ||
		(lInitialCount > lMaximumCount) ||
		(lInitialCount < 0)             ||
		((lpName) && (_tcslen(lpName) > MAX_METSECT_NAMELEN)))
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return NULL;
	}

	// Allocate memory for the metered section
	lpMetSect = (LPMETERED_SECTION)malloc(sizeof(METERED_SECTION));

	// If the memory for the metered section was allocated okay, initialize it
	if (lpMetSect)
	{
		if (!InitMeteredSection(lpMetSect, lInitialCount, lMaximumCount, lpName, FALSE))
		{
			CloseMeteredSection(lpMetSect);
			lpMetSect = NULL;
		}
	}
	return lpMetSect;
}

/*
 * OpenMeteredSection
 */
#ifndef _WIN32_WCE
inline LPMETERED_SECTION OpenMeteredSection(LPCTSTR lpName)
{
	LPMETERED_SECTION lpMetSect = NULL;

	if (lpName)
	{
		lpMetSect = (LPMETERED_SECTION)malloc(sizeof(METERED_SECTION));

		// If the memory for the metered section was allocated okay
		if (lpMetSect)
		{
			if (!InitMeteredSection(lpMetSect, 0, 0, lpName, TRUE))
			{
				// Metered section failed to initialize
				CloseMeteredSection(lpMetSect);
				lpMetSect = NULL;
			}
		}
	}
	return lpMetSect;
}
#endif

/*
 * GetMeteredSectionLock
 */
inline void GetMeteredSectionLock(LPMETERED_SECTION lpMetSect)
{
	// Spin and get access to the metered section lock
	while (InterlockedExchange(&(lpMetSect->lpSharedInfo->lSpinLock), 1) != 0)
		Sleep(0);
}

/*
 * ReleaseMeteredSectionLock
 */
inline void ReleaseMeteredSectionLock(LPMETERED_SECTION lpMetSect)
{
	InterlockedExchange(&(lpMetSect->lpSharedInfo->lSpinLock), 0);
}

/*
 * EnterMeteredSection
 */
inline DWORD EnterMeteredSection(LPMETERED_SECTION lpMetSect, DWORD dwMilliseconds)
{
	while (TRUE)
	{
		GetMeteredSectionLock(lpMetSect);

		// We have access to the metered section, everything we do now will be atomic
		if (lpMetSect->lpSharedInfo->lAvailableCount >= 1)
		{
			lpMetSect->lpSharedInfo->lAvailableCount--;
			ReleaseMeteredSectionLock(lpMetSect);
			return WAIT_OBJECT_0;
		}

		// Couldn't get in. Wait on the event object
		lpMetSect->lpSharedInfo->lThreadsWaiting++;
		ResetEvent(lpMetSect->hEvent);
		ReleaseMeteredSectionLock(lpMetSect);
		if (WaitForSingleObject(lpMetSect->hEvent, 
		 dwMilliseconds) == WAIT_TIMEOUT)
		{
			return WAIT_TIMEOUT;
		}
	}
}

/*
 * LeaveMeteredSection
 */
inline BOOL LeaveMeteredSection(LPMETERED_SECTION lpMetSect, LONG lReleaseCount, 
		 LPLONG lpPreviousCount)
{
	int iCount;
	GetMeteredSectionLock(lpMetSect);

	// Save the old value if they want it
	if (lpPreviousCount)
	{
		*lpPreviousCount = lpMetSect->lpSharedInfo->lAvailableCount;
	}

	// We have access to the metered section, everything we do now will be atomic
	if ((lReleaseCount < 0) ||
		(lpMetSect->lpSharedInfo->lAvailableCount+lReleaseCount >
		 lpMetSect->lpSharedInfo->lMaximumCount))
	{
		ReleaseMeteredSectionLock(lpMetSect);
		SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}
	lpMetSect->lpSharedInfo->lAvailableCount += lReleaseCount;

	// Set the event the appropriate number of times
	lReleaseCount = 
	  min(lReleaseCount,lpMetSect->lpSharedInfo->lThreadsWaiting);
	if (lpMetSect->lpSharedInfo->lThreadsWaiting)
	{
		for (iCount=0; iCount < lReleaseCount ; iCount++)
		{
			lpMetSect->lpSharedInfo->lThreadsWaiting--;
			// この行をコメントアウト  SetEvent(lpMetSect->hEvent);
		}
		ReleaseMeteredSectionLock(lpMetSect);
		SetEvent(lpMetSect->hEvent);
	} else {
		ReleaseMeteredSectionLock(lpMetSect);
	}
	return TRUE;
}

#endif // _TnbDOXYGEN
