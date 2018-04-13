//
// Count from 0 to 100 repeatedly and display the count.
//

#pragma once
class CCounter
{
private:
	HANDLE hStopEvent;
	HANDLE hThread;
	DWORD dwThreadId;
	HWND hWnd;
	RECT rect;
	int x, y, count;

private:
	static DWORD WINAPI ThreadFunc(LPVOID lpParam);

public:
	CCounter();
	~CCounter();

	void Run(HWND hWnd);
	void Exit();

	void Draw(HDC hDC);
};

