// BlokusWindow.ixx
//
// Blokus

export module BlokusWindow;

import std;
import Types;
import Animation;
import WinBasics;
import GraphicsWindow;
import D2D_DrawContext;
import BlokusCore;

using std::array;
using std::unique_ptr;

export class BlokusWindow : public GraphicsWindow
{
public:
	void Reset();
	void Start(HWND const, Tournament *);

	void OnChar(WPARAM const, LPARAM const) final;

private:
	BlokusMove m_move;

	D2D_DrawContext  m_context;
	Match            m_match;
    Tournament     * m_pTournament { nullptr };
	bool             m_bAutoRun    { false };
	TextFormatHandle m_hTextFormat { nullptr };

    Animation<MicroMeterPnt> m_posAnimation;

	bool OnSize   (PIXEL  const, PIXEL  const) final;
	bool OnCommand(WPARAM const, LPARAM const, PixelPoint const = PixelPoint::NULL_VAL()) final;

	void PaintGraphics() final;
	void drawFinishedMsg();
	void paintBoard() const;
	void performMove();
	void autoRun();
	void nextMove();
};