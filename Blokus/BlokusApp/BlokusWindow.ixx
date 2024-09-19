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

private:

	Game        m_game;

	fPixelPoint m_fPixPntOrigin;
	fPixel      m_fPixBoardSize;
	fPixel      m_fPixFieldSize;

	void showOrientations(PieceType const&, fPixel const, Color const) const;
	void paintBoard() const;
	void paintPieces() const;

	bool OnSize(PIXEL const, PIXEL const) final;
	void PaintGraphics() final;
};