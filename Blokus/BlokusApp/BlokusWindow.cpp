// BlokusWindow.cpp
//
// Blokus

module BlokusWindow;

import std;
import Color;
import Types;
import SaveCast;
import WinBasics;
import Direct2D;
import GraphicsWindow;
import BlokusCore;

using std::min;

fPixel const BORDER { 10.0_fPixel };

Color const COL_RED    { Color(1.0f, 0.2f, 0.2f) };
Color const COL_GREEN  { Color(0.0f, 1.0f, 0.0f) };
Color const COL_BLUE   { Color(0.4f, 0.4f, 1.0f) };
Color const COL_YELLOW { Color(0.8f, 0.8f, 0.0f) };

void BlokusWindow::Start(HWND const hwndParent)
{
	GraphicsWindow::Initialize
	(
		hwndParent, 
		L"ClassBlokusWindow", 
	    WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_VISIBLE
	);
}

bool BlokusWindow::OnSize(PIXEL const width, PIXEL const height)
{
	GraphicsWindow::OnSize(width, height);
	fPixel const fPixAvailable { GetClientHeight() - BORDER * 2.0f };
	m_fPixFieldSize = fPixAvailable / Cast2Float(Components::BOARD_SIZE + 2); // board height + top and bottom reserve
	m_fPixBoardSize = m_fPixFieldSize * Cast2Float(Components::BOARD_SIZE);
	m_fPixPntOrigin = fPixelPoint(BORDER, BORDER + m_fPixFieldSize);
	Notify(false);
	return true;
}

void BlokusWindow::showOrientations
(
	PieceType const& pt,
	fPixel    const  fPosVert,
	Color     const  color
) const
{
	fPixelPoint fPos { m_fPixFieldSize, fPosVert };
	pt.Apply2AllOrientations
	(
		[this, &fPos, color](Shape const &s)
		{
			s.Draw(*m_upGraphics.get(), fPos, color, m_fPixFieldSize);
			fPos.Move2Horz(m_fPixFieldSize * 5.5f);
		}
	);
}

void BlokusWindow::paintBoard() const
{
	Color          const BOARD_COLOR       { Color(0.9f, 0.9f, 0.9f) };
	Color          const LINE_COLOR        { Color(0.2f, 0.2f, 0.2f) };
	fPixelRectSize const fPixRectSizeBoard { fPixelRectSize(m_fPixBoardSize, m_fPixBoardSize) };
	fPixelRect     const fPixBoardRect     { m_fPixPntOrigin, fPixRectSizeBoard };
	m_upGraphics->FillRectangle(fPixBoardRect, BOARD_COLOR);
	for (fPixel x = m_fPixPntOrigin.GetX(); x <= m_fPixPntOrigin.GetX() + m_fPixBoardSize; x += m_fPixFieldSize)
		m_upGraphics->DrawLine
		(
			fPixelPoint(x, m_fPixPntOrigin.GetY()),
			fPixelPoint(x, m_fPixPntOrigin.GetY() + m_fPixBoardSize),
			2.0_fPixel,
			LINE_COLOR
		);
	for (fPixel y = m_fPixPntOrigin.GetY(); y <= m_fPixPntOrigin.GetY() + m_fPixBoardSize; y += m_fPixFieldSize)
		m_upGraphics->DrawLine
		(
			fPixelPoint(m_fPixPntOrigin.GetX(),                   y),
			fPixelPoint(m_fPixPntOrigin.GetX() + m_fPixBoardSize, y),
			2.0_fPixel,
			LINE_COLOR
		);
};

void BlokusWindow::paintPieces() const
{
	fPixelPoint const fPixPntOriginPieces 
	{ 
		m_fPixPntOrigin.GetX() + m_fPixBoardSize + BORDER,
		m_fPixPntOrigin.GetY()
	};

	Components::Apply2AllPieceTypes
	(
		[this, &fPixPntOriginPieces](PieceType const& pt)
		{
			CoordPos const pos { pt.GetPos() };
			fPixelPoint const fPos 
			{ 
				m_fPixFieldSize * pos.GetXvalue() + m_fPixFieldSize * 0.5f,
				m_fPixFieldSize * pos.GetYvalue() + m_fPixFieldSize * 0.5f
			};
			pt.Draw
			(
				*m_upGraphics.get(),
				fPixPntOriginPieces + fPos,
				COL_RED,
				m_fPixFieldSize
			);
		}
	);
};

void BlokusWindow::PaintGraphics()
{
	paintBoard();
	paintPieces();
};
