// BlokusWindow.ixx
//
// Blokus

export module BlokusWindow;

import std;
import WinBasics;
import GraphicsWindow;
import Meeple;

using std::array;

export class BlokusWindow : public GraphicsWindow
{
public:
	void Start(HWND const);

private:

	fPixel m_fPixMinSize;
	fPixel m_fPixFieldSize;

	array<Meeple, 21> m_meeples;

	void initMeeples();
	void showOrientations(Meeple const&, fPixel const, Color const) const;
	void paintBoard() const;

	bool OnSize(PIXEL const, PIXEL const) final;
	void PaintGraphics() final;
};