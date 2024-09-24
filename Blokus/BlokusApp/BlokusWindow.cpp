// BlokusWindow.cpp
//
// Blokus

module BlokusWindow;

import Color;
import Types;
import SaveCast;
import Direct2D;
import Resource;

using std::min;
using std::wcout;
using std::endl;

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
	m_hTextFormat = m_upGraphics->NewTextFormat(24.f);
}

void BlokusWindow::nextPlayer()
{
	if (!m_game.ActivePlayer().HasFinished())
	{
		m_timer.BeforeAction();
		m_game.NextMove();
		m_timer.AfterAction();
		Ticks const ticks { m_timer.GetSingleActionTicks() };
		wcout << L"complete move:"<< PerfCounter::Ticks2wstring(ticks) << endl;
	}
	m_game.NextPlayer();
	m_game.FindContactPnts();
	Notify(true);
}

bool BlokusWindow::OnSize(PIXEL const width, PIXEL const height)
{
	GraphicsWindow::OnSize(width, height);
	fPixel const fPixAvailable { GetClientHeight() - BORDER * 2.0f };
	fPixel const fPixFieldSize { fPixAvailable / Cast2Float(BOARD_SIZE + 2) }; // board height + top and bottom reserve
	m_fPixBoardSize = m_coordSys.CellSize() * Cast2Float(BOARD_SIZE);
	m_coordSys.Reset(fPixelPoint(BORDER, BORDER + fPixFieldSize), fPixFieldSize);
	Notify(false);
	return true;
}

void BlokusWindow::OnChar(WPARAM const wParam, LPARAM const lParam)
{
	switch (char cKeyPressed{ static_cast<char>(wParam) })
	{
		case 'i':
		case 'I':
			SendCommand(IDM_INITIALIZE);
			break;

		case 'r':
		case 'R':
			SendCommand(IDM_RUN_TIL_END);
			break;

		default:
			SendCommand(IDM_NEXT_PLAYER);
	}
}

bool BlokusWindow::OnCommand(WPARAM const wParam, LPARAM const lParam, PixelPoint const pixPoint)
{
	switch (int const wmId { LoWord(wParam) } )
	{
	case IDM_INITIALIZE:
		m_game.Initialize();
		Notify(false);
		break;

	case IDM_NEXT_PLAYER:
		nextPlayer();
		break;

	case IDM_RUN_TIL_END:
		SendCommand(IDM_NEXT_PLAYER);
		if (!m_game.GameFinished())
			PostCommand(wParam);
		break;

	default:
		return GraphicsWindow::OnCommand(wParam, lParam, pixPoint);
	}

	return true;
}
void BlokusWindow::paintBoard() const
{
	Color          const BOARD_COLOR       { Color(0.8f, 0.8f, 0.8f) };
	fPixelRectSize const fPixRectSizeBoard { fPixelRectSize(m_fPixBoardSize, m_fPixBoardSize) };
	fPixelRect     const fPixBoardRect     { m_coordSys.Offset(), fPixRectSizeBoard };
	m_upGraphics->FillRectangle(fPixBoardRect, BOARD_COLOR);

	Color const LINE_COLOR { Color(0.6f, 0.6f, 0.6f) };
	for (Coord x = 0_COORD; x <= COORD_BOARD_SIZE; ++x)
		m_upGraphics->DrawLine
		(
			m_coordSys.Transform2fPixelPos(CoordPos(x, 0_COORD)),
			m_coordSys.Transform2fPixelPos(CoordPos(x, COORD_BOARD_SIZE)),
			1.0_fPixel,
			LINE_COLOR
		);
	for (Coord y = 0_COORD; y <= COORD_BOARD_SIZE; ++y)
		m_upGraphics->DrawLine
		(
			m_coordSys.Transform2fPixelPos(CoordPos(0_COORD, y)),
			m_coordSys.Transform2fPixelPos(CoordPos(COORD_BOARD_SIZE, y)),
			1.0_fPixel,
			LINE_COLOR
		);
};

void BlokusWindow::drawFinishedMsg()
{
	fPixelRect fPixRect 
	{ 
		m_coordSys.Transform2fPixelPos(CoordPos(          0_COORD, -1_COORD)),
		m_coordSys.Transform2fPixelPos(CoordPos( COORD_BOARD_SIZE,  0_COORD))
	};
	m_upGraphics->DisplayText
	(
		fPixRect, 
		L"Game finished. The winner is " + m_game.Winner().GetName() + L".",
		m_hTextFormat
	);
}

void BlokusWindow::PaintGraphics()
{
	Player const& player { m_game.ActivePlayerC() };
	player.DrawFreePieces(*m_upGraphics.get(), m_coordSys);
 	paintBoard();
	m_game.DrawSetPieces(*m_upGraphics.get(), m_coordSys);
	if (BlokusPreferences::m_bShowContactPnts.Get())
		m_game.ActivePlayerC().DrawContactPnts(*m_upGraphics.get(), m_coordSys);
	if (player.HasFinished())
		player.DrawResult(*m_upGraphics.get(), m_coordSys, m_hTextFormat);
	if (m_game.GameFinished())
		drawFinishedMsg();
};
