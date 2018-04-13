//
// Count from 0 to 100 repeatedly and display the count.
//

#include "stdafx.h"
#include "counter.h"

// constructor
CCounter::CCounter()
{
	// window's update region
	rect.left   = 256;
	rect.top    = 96;
	rect.right  = rect.left + 80;
	rect.bottom = rect.top + 55;

	// position of text to be drawn
	x = rect.left + 10;
	y = rect.top + 10;

	// initialize handle
	hThread    = NULL;
	hStopEvent = NULL;

	// create event
    hStopEvent = CreateEvent(NULL,                  // no security attributes
							 TRUE,                  // manual-reset event(TRUE-manual FALSE-auto)
							 FALSE,                 // initial state is non-signaled (TRUE-signal FALSE-nonsignal)
							 L"counterstopevent");  // object name

	// error checking display in output window
    if (hStopEvent == NULL) 
        OutputDebugString(L"Create event failed.\n");
}

// destructor
CCounter::~CCounter()
{
	// close all handles
	if(hThread != NULL)    CloseHandle(hThread);
	if(hStopEvent != NULL) CloseHandle(hStopEvent);
}

// thread function
DWORD WINAPI CCounter::ThreadFunc(LPVOID lpParam)
{
	// Cast the parameter lpParam to the class CCounter.
	CCounter* p = (CCounter*)lpParam;

	DWORD dwWaitResult, dwTimeOut;
	wchar_t szText[100];
	bool loop;

	// display thread status to output window
	swprintf_s(szText, 100, L"THE THREAD 0x%x HAS STARTED.\n", p->dwThreadId);
	OutputDebugString(szText);

	p->count = 0;
	dwTimeOut = 100;
	loop = true;
	while (loop)
	{
		// increment count from 0 to 100 repeatedly
		++p->count;
		if (p->count == 101) p->count = 0;

		// redraw text
		InvalidateRect(p->hWnd, &p->rect, TRUE);

		// exit loop if event handle is set
		dwWaitResult = WaitForSingleObject(p->hStopEvent, dwTimeOut);
		if (dwWaitResult == WAIT_OBJECT_0) loop = false;

	}

	// display thread status to output window
	swprintf_s(szText, 100, L"THE THREAD 0x%x HAS STOPED.\n", p->dwThreadId);
	OutputDebugString(szText);

	return 0;
}

// run thread
void CCounter::Run(HWND hWnd)
{
	// make a copy of window handle
	this->hWnd = hWnd;	

	// reset event handle to prevent stoping the loop
	ResetEvent(hStopEvent);

	// close thread handle
	if(hThread != NULL) CloseHandle(hThread);

	// create thread
	hThread = CreateThread(NULL,         // no security attributes 
						   0,            // use default stack size 
						   ThreadFunc,   // thread function 
						   this,         // this class, argument to thread function 
						   0,            // use default creation flags 
						   &dwThreadId); // returns the thread identifier 

	// error checking display in output window
   if (hThread == NULL) 
      OutputDebugString(L"Create thread failed.\n" ); 
}

// exit thread
void CCounter::Exit()
{
	// set event to stop the loop
	SetEvent(hStopEvent);
}

//
void CCounter::Draw(HDC hDC)
{
	HFONT hFont;
	wchar_t str[10];

	// draw rectangle
	Rectangle(hDC, rect.left, rect.top, rect.right, rect.bottom);

	// create and set font
	hFont = CreateFont(-32, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_MODERN, L"Courier New");
	SelectObject(hDC,hFont);

	// draw text
	swprintf_s(str, 10, L"%3d", count);
	TextOut(hDC, x, y, str, wcslen(str));

	// free resources
	DeleteObject(hFont);
}

//