#ifndef TIMER_HEADER
#define TIMER_HEADER

#include <windows.h>

/*
This class is a simple wrapper of Windows timer functions.

Done by: Davide Di Stefano & Andrea Ingaglio
www.coders4fun.com

This is an example on how you can use the class:

void CALLBACK OnTimer(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	// timer events....
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
	// ...
	Timer * timer = new Timer(NULL, (TIMERPROC)OnTimer, 1, 1000);
	timer->StartTimer();
	// ..
}
*/
class Timer
{
	public:
		// pHandle = Window handle, 
		// procAd = pointer to the method to invoke when the timer elapses
		// timerID = the timer ID
		// interval = interval of timer in ms
		Timer(const HWND pHandle, const TIMERPROC procAd, const int timerID, 
			int interval = -1);
		~Timer();

		// Starts the timer
		bool StartTimer();
		// Stops the timer
		bool StopTimer();

		// Get the timer status
		bool GetStatus(){return m_status;}

		// Set the timer interval (it can't be changed if the timer is on)
		void SetInterval(const int Value);
		// Get the timer interval
		int GetInterval();
private:

		// Parent window handle
		HWND m_handle;
		// Timer ID
		UINT m_timerID;
		int m_interval;
		// method to invoke on timer
		TIMERPROC m_delegate;
		// true if timer is on
		bool m_status;

		void InitTimer(const HWND, const TIMERPROC, const int);
};

#endif

