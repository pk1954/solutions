// BlokusWindow.ixx
//
// Blokus

export module BlokusWindow;

import std;
import WinBasics;
import GraphicsWindow;
import D2D_DrawContext;
import BlokusAppMenu;
import BlokusCore;

using std::array;

export class BlokusWindow : public GraphicsWindow
{
public:
	void Start(HWND const);

	void OnChar(WPARAM const, LPARAM const) final;

private:

	Game             m_game;
	Tournament       m_tournament;
	D2D_DrawContext  m_context;
	TextFormatHandle m_hTextFormat { nullptr };

	void paintBoard() const;

	bool OnSize   (PIXEL  const, PIXEL  const) final;
	bool OnCommand(WPARAM const, LPARAM const, PixelPoint const = PixelPoint::NULL_VAL()) final;

	void PaintGraphics() final;
	void drawFinishedMsg();
};