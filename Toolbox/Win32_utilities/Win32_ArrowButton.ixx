// win32_arrowButton.ixx
//
// NNetWindows

export module ArrowButton;

import Color;
import Direct2D;
import WinBasics;
import Win32_Controls;
import GraphicsWindow;

export class ArrowButton : public GraphicsWindow
{
public:

	ArrowButton
	(
		HWND const hwndParent,
		int  const id
	)
		: m_idCommand(id)
	{
		GraphicsWindow::Initialize(hwndParent, L"ArrowBotton", WS_CHILD | WS_VISIBLE);
	}

	void SetDirection(bool const bDir)
	{
		m_bArrowDirUp = bDir;
		Trigger();
	}

private:

	void PaintGraphics() final
	{
		fPixelRect rect { Convert2fPixelRect(GetClPixelRect()) };
		rect = rect.ScaleRect(-2._fPixel);
		m_upGraphics->UpDownArrow(m_bArrowDirUp, rect, COL_BLACK);
		m_upGraphics->DrawRectangle(rect, COL_BLACK, 1._fPixel);
	}

	void OnDrawItem(WPARAM const wParam, DRAWITEMSTRUCT const * const pDiStruct) final
	{
		PaintGraphics();
	}

	bool OnLButtonUp(WPARAM const wParam, LPARAM const lParam) final
	{ 
		SendCommand2Parent(m_idCommand, 0);
		return true; 
	}

	void AddColorCtlMenu(HMENU const) final {}

	bool m_bArrowDirUp { true };
	int  m_idCommand;
};