// win32_textWindow.h 
//

#pragma once

#include "win32_baseWindow.h"
#include "win32_textBuffer.h"

class TextWindowThread;
class RefreshTimer;

class TextWindow : public BaseWindow
{
public:
    TextWindow();
	~TextWindow() override = default;

    void StartTextWindow
	(
		HWND             const, 
		PixelRect        const &, 
		LPCTSTR          const, 
		UINT             const, 
		bool             const, 
		function<bool()> const &
	);
	void StopTextWindow();

    virtual void DoPaint(TextBuffer &) = 0;

	void Trigger() final;

private:

	void OnPaint() final;

	RefreshTimer     * m_pRefreshTimer     { nullptr };
	TextWindowThread * m_pTextWindowThread { nullptr };
    HDC                m_hDC_Memory        { nullptr };
	HBITMAP            m_hBitmap           { nullptr };
};
