// win32_textWindow.h 
//

#pragma once

#include "win32_baseWindow.h"
#include "win32_textBuffer.h"

using std::unique_ptr;

class TextWindowThread;
class RefreshTimer;

class TextWindow : public BaseWindow
{
public:
    TextWindow();
	~TextWindow() override;

    void StartTextWindow
	(
		HWND      const, 
		PixelRect const &, 
		LPCTSTR   const, 
		UINT      const, 
		bool      const, 
		VisCrit   const &
	);
	void StopTextWindow();

    virtual void DoPaint(TextBuffer &) = 0;

	void Trigger() final;

private:

	void OnPaint() final;

	unique_ptr<TextWindowThread> m_upTextWindowThread;

	RefreshTimer * m_pRefreshTimer { nullptr };
    HDC            m_hDC_Memory    { nullptr };
	HBITMAP        m_hBitmap       { nullptr };
};
