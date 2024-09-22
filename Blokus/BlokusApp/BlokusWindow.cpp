// BlokusWindow.cpp
//
// Blokus

module BlokusWindow;

import Color;
import Types;
import SaveCast;
import Direct2D;

using std::min;

fPixel const BORDER { 10.0_fPixel };

void BlokusWindow::Start(HWND const hwndParent)
{
	GraphicsWindow::Initialize
	(
		hwndParent, 
		L"ClassBlokusWindow", 
	    WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_VISIBLE
	);
	m_game.Initialize();
}

bool BlokusWindow::OnSize(PIXEL const width, PIXEL const height)
{
	GraphicsWindow::OnSize(width, height);
	fPixel const fPixAvailable { GetClientHeight() - BORDER * 2.0f };
	fPixel const fPixFieldSize { fPixAvailable / Cast2Float(BOARD_SIZE + 2) }; // board height + top and bottom reserve
	m_fPixBoardSize = m_coordSys.CellSize() * Cast2Float(BOARD_SIZE);
	m_coordSys.Reset
	(
		fPixelPoint(BORDER, BORDER + fPixFieldSize),
		fPixFieldSize
	);
	Notify(false);
	return true;
}

void BlokusWindow::OnChar(WPARAM const wParam, LPARAM const lParam)
{
	if (m_bMoveDone)
	    m_game.NextPlayer();
	else
		m_game.NextMove();
	m_bMoveDone = !m_bMoveDone;
	Notify(false);
}

void BlokusWindow::paintBoard() const
{
	Color          const BOARD_COLOR       { Color(0.9f, 0.9f, 0.9f) };
	fPixelRectSize const fPixRectSizeBoard { fPixelRectSize(m_fPixBoardSize, m_fPixBoardSize) };
	fPixelRect     const fPixBoardRect     { m_coordSys.Offset(), fPixRectSizeBoard };
	m_upGraphics->FillRectangle(fPixBoardRect, BOARD_COLOR);

	Color const LINE_COLOR { Color(0.2f, 0.2f, 0.2f) };
	for (Coord x = 0_COORD; x <= COORD_BOARD_SIZE; ++x)
		m_upGraphics->DrawLine
		(
			m_coordSys.Transform2fPixelPos(CoordPos(x, 0_COORD)),
			m_coordSys.Transform2fPixelPos(CoordPos(x, COORD_BOARD_SIZE)),
			2.0_fPixel,
			LINE_COLOR
		);
	for (Coord y = 0_COORD; y <= COORD_BOARD_SIZE; ++y)
		m_upGraphics->DrawLine
		(
			m_coordSys.Transform2fPixelPos(CoordPos(0_COORD, y)),
			m_coordSys.Transform2fPixelPos(CoordPos(COORD_BOARD_SIZE, y)),
			2.0_fPixel,
			LINE_COLOR
		);
};

void BlokusWindow::PaintGraphics()
{
	m_game.ActivePlayerC().DrawFreePieces(*m_upGraphics.get(), m_coordSys);
 	paintBoard();
	if (!m_bMoveDone)
		m_game.ActivePlayerC().DrawContactPnts(*m_upGraphics.get(), m_coordSys);
	m_game.DrawSetPieces(*m_upGraphics.get(), m_coordSys);
};
