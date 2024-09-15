// BlokusWindow.cpp
//
// Blokus

module BlokusWindow;

import std;
import Color;
import Types;
import WinBasics;
import Direct2D;
import GraphicsWindow;
import Meeple;

using std::min;

fPixel const BORDER { 10.0_fPixel };

Color const COL_RED    { Color(1.0f, 0.2f, 0.2f) };
Color const COL_GREEN  { Color(0.0f, 1.0f, 0.0f) };
Color const COL_BLUE   { Color(0.4f, 0.4f, 1.0f) };
Color const COL_YELLOW { Color(0.8f, 0.8f, 0.0f) };

bool BlokusWindow::OnSize(PIXEL const width, PIXEL const height)
{
	GraphicsWindow::OnSize(width, height);
	m_fPixMinSize   = min(GetClientWidth(), GetClientHeight()) - BORDER;
	m_fPixFieldSize = m_fPixMinSize * 0.05f; // 20 fields
	Notify(false);
	return true;
}

void BlokusWindow::paintBoard()
{
	Color          const BOARD_COLOR   { Color(0.9f, 0.9f, 0.9f) };
	Color          const LINE_COLOR    { Color(0.2f, 0.2f, 0.2f) };
	fPixel         const fPixHalfSize  { m_fPixMinSize * 0.5f };
	fPixelPoint    const fPixPntCenter { Convert2fPixelPoint(GetClRectCenter()) };
	fPixelPoint    const fPixPntOrigin { fPixPntCenter - fPixelPoint(fPixHalfSize, fPixHalfSize) };
	fPixelRectSize const fPixBoardSize { fPixelRectSize(m_fPixMinSize, m_fPixMinSize) };
	m_upGraphics->FillRectangle(fPixelRect(fPixPntOrigin, fPixBoardSize), BOARD_COLOR);
	for (fPixel x = fPixPntOrigin.GetX(); x <= fPixPntOrigin.GetX() + m_fPixMinSize; x += m_fPixFieldSize)
		m_upGraphics->DrawLine
		(
			fPixelPoint(x, fPixPntOrigin.GetY()),
			fPixelPoint(x, fPixPntOrigin.GetY() + m_fPixMinSize),
			2.0_fPixel,
			LINE_COLOR
		);
	for (fPixel y = fPixPntOrigin.GetY(); y <= fPixPntOrigin.GetY() + m_fPixMinSize; y += m_fPixFieldSize)
		m_upGraphics->DrawLine
		(
			fPixelPoint(fPixPntOrigin.GetX(),                 y),
			fPixelPoint(fPixPntOrigin.GetX() + m_fPixMinSize, y),
			2.0_fPixel,
			LINE_COLOR
		);
};

void BlokusWindow::PaintGraphics()
{
	paintBoard();
	Meeple m1
	(
		{
			{
				{true,  true,  false, false, false},
				{false, true,  true,  false, false},
				{false, false, true,  false, false}
			}
		}
	);
	m1.Draw(*m_upGraphics.get(), fPixelPoint(100._fPixel, 100._fPixel), COL_RED, m_fPixFieldSize);

	Meeple m2
	(
		{
			{
				{true,  true,  true,  false, false},
				{true,  false, true,  false, false},
				{false, false, false, false, false}
			}
		}
	);
	m2.Draw(*m_upGraphics.get(), fPixelPoint(100._fPixel, 300._fPixel), COL_GREEN, m_fPixFieldSize);

	m_upGraphics->Push();
	m_upGraphics->Rotation(fPixelPoint(100._fPixel, 500._fPixel), 30.0f);

	Meeple m3
	(
		{
			{
				{true,  true,  true,  true,  false},
				{false, false, false, true,  false},
				{false, false, false, false, false}
			}
		}
	);
	m3.Draw(*m_upGraphics.get(), fPixelPoint(100._fPixel, 500._fPixel), COL_BLUE, m_fPixFieldSize);

	m_upGraphics->Pop();
	Meeple m4
	(
		{
			{
				{true,  true, true,  false, false},
				{false, true, false, false, false},
				{false, true, false, false, false}
			}
		}
	);
	m4.Draw(*m_upGraphics.get(), fPixelPoint(100._fPixel, 700._fPixel), COL_YELLOW, m_fPixFieldSize);

	//meepleSquare(fPixelPoint(100._fPixel, 100._fPixel), COL_RED);
	//meepleSquare(fPixelPoint(100._fPixel, 200._fPixel), COL_GREEN);
	//meepleSquare(fPixelPoint(100._fPixel, 300._fPixel), COL_BLUE);
	//meepleSquare(fPixelPoint(100._fPixel, 400._fPixel), COL_YELLOW);
};
