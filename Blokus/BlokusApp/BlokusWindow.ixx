// BlokusWindow.ixx
//
// Blokus

export module BlokusWindow;

import std;
import Types;
import Color;
import WinBasics;
import GraphicsWindow;
import SoundInterface;
import D2D_DrawContext;
import BlokusCore;

export class BlokusWindow : public GraphicsWindow
{
public:
	void Start(HWND const, MatchReaderInterface const &, Sound&);

	void OnChar(WPARAM const, LPARAM const) final;

private:
	BlokusMove                   m_move;
	MatchReaderInterface const * m_pMRI;
	PieceMotion                  m_pieceMotion;
	D2D_DrawContext              m_context;
    PlayerId                     m_idVisiblePlayer  { 0 };
	Sound                      * m_pSound           { nullptr };
	TextFormatHandle             m_hTextFormat      { nullptr };
	TextFormatHandle             m_hTextFormatSmall { nullptr };

	bool OnSize       (PIXEL  const, PIXEL  const)                                            final;
	bool OnCommand    (WPARAM const, LPARAM const, PixelPoint const = PixelPoint::NULL_VAL()) final;
	void OnMouseMove  (WPARAM const, LPARAM const)                                            final;
	bool OnLButtonUp  (WPARAM const, LPARAM const)                                            final;
	bool OnLButtonDown(WPARAM const, LPARAM const)                                            final;
	void PaintGraphics() final;

	void          performMove(BlokusMove);
	void          drawFinishedMsg();
	void		  drawCellNumbers();
	void          drawBlockedCells(Player const&);
	void          paintBoard() const;
	MicroMeterPnt getCrsrPos(LPARAM const) const;

	Player const &visiblePlayerC   () const { return m_pMRI->GetPlayerC(m_idVisiblePlayer); }
	void          nextVisiblePlayer()       { m_idVisiblePlayer = NextPlayer(m_idVisiblePlayer); }
	void          prevVisiblePlayer()       { m_idVisiblePlayer = PrevPlayer(m_idVisiblePlayer); }
};