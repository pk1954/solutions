// win32_arrowButton.h
//
// NNetWindows

#pragma once

#include "win32_controls.h"
#include "win32_graphicsWindow.h"

import Direct2D;

class ArrowButton : public GraphicsWindow
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

	void SetBackgroundColor(D2D1::ColorF const col)
	{
		m_colBackground = col;
	}

	void SetDirection(bool const bDir)
	{
		m_bArrowDirUp = bDir;
	}

private:

	void DoPaint() final
	{
		m_upGraphics->FillBackground(m_colBackground);
		m_upGraphics->UpDownArrow
		(
			m_bArrowDirUp,
			Convert2fPixelRect(GetClPixelRect()),
			D2D1::ColorF::Black
		);
	};

	void OnDrawItem(WPARAM const wParam, DRAWITEMSTRUCT const * const pDiStruct) final
	{
		DoPaint();
	};

	bool OnLButtonUp(WPARAM const wParam, LPARAM const lParam) final
	{ 
		SendCommand2Parent(m_idCommand, 0);
		return true; 
	};

	D2D1::ColorF m_colBackground { D2D1::ColorF::White };
	bool         m_bArrowDirUp   { true };
	int          m_idCommand;
};