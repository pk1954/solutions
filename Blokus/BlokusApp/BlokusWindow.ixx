// BlokusWindow.ixx
//
// Blokus

export module BlokusWindow;

import std;
import HiResTimer;
import WinBasics;
import GraphicsWindow;
import BlokusCore;

using std::array;

export class BlokusWindow : public GraphicsWindow
{
public:
	void Start(HWND const);

	void OnChar(WPARAM const, LPARAM const) final;

private:

    HiResTimer     m_timer;
	//bool           m_bMoveDone { false };
	Game           m_game;
	BlokusCoordSys m_coordSys;
	fPixel         m_fPixBoardSize;

	void paintBoard() const;

	bool OnSize(PIXEL  const, PIXEL  const) final;
	void PaintGraphics() final;
};