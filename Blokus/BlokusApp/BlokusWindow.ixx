// BlokusWindow.ixx
//
// Blokus

export module BlokusWindow;

import std;
import Types;
import Animation;
import WinBasics;
import GraphicsWindow;
import SoundInterface;
import D2D_DrawContext;
import BlokusCore;

using std::array;
using std::unique_ptr;

export class BlokusWindow : public GraphicsWindow
{
public:
	void Reset();
	void Start(HWND const, Sound&);

	void OnChar(WPARAM const, LPARAM const) final;

private:
	//PosDir            m_posDirTarget;
	BlokusMove        m_move;
	Match             m_match;
	PieceMotion       m_pieceMotion;
	D2D_DrawContext   m_context;
	Sound           * m_pSound      { nullptr };
	bool              m_bAutoRun    { false };
	TextFormatHandle  m_hTextFormat { nullptr };
    Animation<PosDir> m_posDirAnimation;
	int               m_iAnimationPhase;

	bool OnSize       (PIXEL  const, PIXEL  const)                                            final;
	bool OnCommand    (WPARAM const, LPARAM const, PixelPoint const = PixelPoint::NULL_VAL()) final;
	void OnMouseMove  (WPARAM const, LPARAM const)                                            final;
	bool OnLButtonUp  (WPARAM const, LPARAM const)                                            final;
	bool OnLButtonDown(WPARAM const, LPARAM const)                                            final;
	void PaintGraphics() final;

	void          drawFinishedMsg();
	void          paintBoard() const;
	MicroMeterPnt getCrsrPos(LPARAM const) const;

	//void autoRun();
	//void nextMove();
	//void startRotationPhase(Degrees const);
};