#include "stdafx.h"
#include "timer.h"

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
Timer::Timer(const HWND pHandle, const TIMERPROC procAd, const int timerID, int interval)
{
	this->InitTimer(pHandle, procAd, timerID);
	m_interval = interval;
}

Timer::~Timer()
{
	this->StopTimer();
}

void Timer::InitTimer(const HWND pHandle, const TIMERPROC procAd, const int timerID)
{
	m_status = false;
	m_handle = pHandle;
	m_timerID = timerID;
	m_delegate = procAd;
}

// Starts the timer
bool Timer::StartTimer()
{
	if (m_interval < 0)
		return false;

	// stop the timer if it's active
	if (m_status)
		this->StopTimer();

	// create the timer
	if (m_handle == NULL)
	{
		m_timerID = SetTimer(m_handle, m_timerID, (UINT)m_interval, m_delegate);
		// if m_timer is 0, the timer creations isn't successfull
		m_status = m_timerID != 0;
	}
	else
		m_status = (SetTimer(m_handle, m_timerID, (UINT)m_interval, m_delegate) == TRUE);
	return m_status;
}

// Stops the timer
bool Timer::StopTimer()
{
	if (!m_status)
		return true;

	// destroy the timer
	m_status = !(KillTimer(m_handle, m_timerID) == TRUE);
	return !m_status;
}


void Timer::SetInterval(const int value)
{
	m_interval = value;
	StopTimer();
	StartTimer();
}

int Timer::GetInterval()
{
	return m_interval;
}

