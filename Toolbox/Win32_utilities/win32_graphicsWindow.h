// win32_modelWindow.h : 
//
// Win32_utilities

#pragma once

#include "Direct2D.h"
#include "win32_baseWindow.h"

class GraphicsWindow : public BaseWindow
{
public:
	virtual void Initialize(HWND const hwnd)
	{
		m_graphics.Initialize(hwnd);
		m_trackStruct.hwndTrack = hwnd;
	}

	virtual void Reset()
	{
		m_trackStruct.hwndTrack = HWND(0);
		(void)TrackMouseEvent(& m_trackStruct);
	}

	virtual void Stop()
	{
		Reset();
		m_graphics.ShutDown();
		DestroyWindow();
	}

	void TrackMouse()
	{
		(void)TrackMouseEvent(& m_trackStruct);
	}

	void OnPaint() override
	{
		if (IsWindowVisible())
		{
			PAINTSTRUCT ps;
			BeginPaint(&ps);
			if (m_graphics.StartFrame())
			{
				DoPaint();
				m_graphics.EndFrame();
			}
			EndPaint(&ps);
		}
	}

	bool OnSize(WPARAM const wParam, LPARAM const lParam) override
	{
		UINT width  = LOWORD(lParam);
		UINT height = HIWORD(lParam);
		m_graphics.Resize(width, height);
		return true;
	}

	virtual void DoPaint() = 0;

protected:
	D2D_driver      m_graphics { };

private:
	TRACKMOUSEEVENT m_trackStruct { sizeof(TRACKMOUSEEVENT), TME_LEAVE, HWND(0), 0L };
};