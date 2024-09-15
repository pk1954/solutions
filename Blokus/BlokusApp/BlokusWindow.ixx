// BlokusWindow.ixx
//
// Blokus

export module BlokusWindow;

import std;
import WinBasics;
import GraphicsWindow;

export class BlokusWindow : public GraphicsWindow
{
public:
	void Start(HWND const hwndParent)
	{
		GraphicsWindow::Initialize
		(
			hwndParent, 
			L"ClassBlokusWindow", 
	    	WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_VISIBLE
		);
	}

private:

	fPixel m_fPixMinSize;
	fPixel m_fPixFieldSize;

	void paintBoard();

	bool OnSize(PIXEL const, PIXEL const) final;
	void PaintGraphics() final;
};