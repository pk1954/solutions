// BlokusWindow.ixx
//
// Blokus

export module BlokusWindow;

import std;
import HiResTimer;
import WinBasics;
import GraphicsWindow;
import BlokusAppMenu;
import BlokusCore;

using std::array;

export class BlokusWindow : public GraphicsWindow
{
public:
	void Start(HWND const);

	void OnChar(WPARAM const, LPARAM const) final;

private:

    HiResTimer        m_timer;
	Game              m_game;
	BlokusDrawContext m_context;
	TextFormatHandle  m_hTextFormat { nullptr };

	void paintBoard() const;

	bool OnSize   (PIXEL  const, PIXEL  const) final;
	bool OnCommand(WPARAM const, LPARAM const, PixelPoint const = PixelPoint::NULL_VAL()) final;

	void PaintGraphics() final;
	void nextPlayer();
	void drawFinishedMsg();
};