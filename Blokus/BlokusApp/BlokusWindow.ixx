// BlokusWindow.ixx
//
// Blokus

export module BlokusWindow;

import std;
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

	Game           m_game;
	BlokusCoordSys m_coordSys;
	fPixel         m_fPixBoardSize;

	void paintBoard() const;

	bool OnSize(PIXEL  const, PIXEL  const) final;
	void PaintGraphics() final;
};