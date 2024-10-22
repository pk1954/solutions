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
	D2D_DrawContext  m_context;
	BlokusMove       m_move;
	Match            m_match;
	PosDir           m_posDirTarget;
    Tournament     * m_pTournament { nullptr };
	bool             m_bAutoRun    { false };
	TextFormatHandle m_hTextFormat { nullptr };

    Animation<PosDir> m_posDirAnimation;
	int               m_iAnimationPhase;

	bool OnSize   (PIXEL  const, PIXEL  const) final;
	bool OnCommand(WPARAM const, LPARAM const, PixelPoint const = PixelPoint::NULL_VAL()) final;
	void PaintGraphics() final;

	PieceType const &getPieceTypeC() const;

	void drawFinishedMsg();
	void paintBoard() const;
	void performMove();
	void autoRun();
	void nextMove();
};