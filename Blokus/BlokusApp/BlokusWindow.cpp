// BlokusWindow.cpp
//
// Blokus

module BlokusWindow;

import Color;
import Types;
import IoUtil;
import SaveCast;
import DrawContext;
import Resource;

using std::min;
using std::wcout;
using std::endl;

fPixel const BORDER { 10.0_fPixel };

void BlokusWindow::Start
(
	HWND   const hwndParent,
	Tournament * pTournament
)
{
	m_pTournament = pTournament;
	GraphicsWindow::Initialize
	(
		hwndParent, 
		L"ClassBlokusWindow", 
	    WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_VISIBLE
	);
	m_match.Initialize();
	m_context.Start(m_upGraphics.get());
	m_hTextFormat = m_upGraphics->NewTextFormat(24.f);
}

bool BlokusWindow::OnSize(PIXEL const width, PIXEL const height)
{
	GraphicsWindow::OnSize(width, height);
	fPixel     const fPixAvailable { GetClientHeight() - BORDER * 2.0f };
	fPixel     const fPixFieldSize { fPixAvailable / Cast2Float(BOARD_SIZE + 2) }; // board height + top and bottom reserve
	MicroMeter const umHeightAvail { UM_CELL_SIZE * static_cast<float>(BOARD_SIZE + 2) };
	MicroMeter const umPixelSize   { umHeightAvail / fPixAvailable.GetValue() };
	m_context.SetPixelOffset(fPixelPoint(-BORDER, -(BORDER + fPixFieldSize)));
	m_context.SetPixelSize(umPixelSize);
	Notify(false);
	return true;
}

void BlokusWindow::OnChar(WPARAM const wParam, LPARAM const lParam)
{
	switch (char cKeyPressed{ static_cast<char>(wParam) })
	{
		case 'i':
		case 'I':
			PostCommand(IDD_INITIALIZE);
			break;

		case 'r':
		case 'R':
			PostCommand(IDD_RUN_TIL_END);
			break;

		case 't':
		case 'T':
			SendCommand2Parent(IDD_START_TOURNAMENT);
			PostCommand(IDD_START_TOURNAMENT);
			break;

		default:
			PostCommand(IDD_NEXT_PLAYER);
	}
}

bool BlokusWindow::OnCommand(WPARAM const wParam, LPARAM const lParam, PixelPoint const pixPoint)
{
	switch (int const wmId { LoWord(wParam) } )
	{
	case IDD_INITIALIZE:
		m_match.Initialize();
		Notify(false);
		break;

	case IDD_RUN_TIL_END:
		m_bAutoRun = true;
		SendCommand(IDD_NEXT_PLAYER);
		break;

	case IDD_NEXT_PLAYER:
		m_match.NextPlayer();
		Notify(true);
		if (m_match.MatchFinished())
			PostCommand(IDD_MATCH_FINISHED);
		else if (m_bAutoRun)
			PostCommand(IDD_NEXT_PLAYER);
		break;

	case IDD_START_TOURNAMENT:
		m_bAutoRun = true;
		m_pTournament->Start(100);
		PostCommand(IDD_NEXT_MATCH);
		break;

	case IDD_NEXT_MATCH:
		m_match.Initialize();
		PostCommand(IDD_NEXT_PLAYER);
		break;

	case IDD_MATCH_FINISHED:
		if (m_pTournament->Active())
		{
			m_pTournament->MatchFinished(m_match.WinnerId());
			if (m_pTournament->Finished())
				PostCommand(IDD_TOURNAMENT_FINISHED);
			else
				PostCommand(IDD_NEXT_MATCH);
		}
		break;

	case IDD_TOURNAMENT_FINISHED:
		m_bAutoRun = false;
		break;

	default:
		return GraphicsWindow::OnCommand(wParam, lParam, pixPoint);
	}

	return true;
}
void BlokusWindow::paintBoard() const
{
	Color     const BOARD_COLOR { Color(0.8f, 0.8f, 0.8f) };
	CoordRect const rect        { CoordPos(0_COORD, 0_COORD), CoordPos(COORD_BOARD_SIZE, COORD_BOARD_SIZE) };
	m_context.FillRectangle(Convert2fCoord(rect), BOARD_COLOR);

	Color const LINE_COLOR { Color(0.6f, 0.6f, 0.6f) };
	for (Coord x = 0_COORD; x <= COORD_BOARD_SIZE; ++x)
		m_context.DrawLine
		(
			Convert2fCoord(CoordPos(x, 0_COORD)),
			Convert2fCoord(CoordPos(x, COORD_BOARD_SIZE)),
			0._MicroMeter,
			LINE_COLOR
		);
	for (Coord y = 0_COORD; y <= COORD_BOARD_SIZE; ++y)
		m_context.DrawLine
		(
			Convert2fCoord(CoordPos(0_COORD, y)),
			Convert2fCoord(CoordPos(COORD_BOARD_SIZE, y)),
			0._MicroMeter,
			LINE_COLOR
		);
};

void BlokusWindow::drawFinishedMsg()
{
	MicroMeterRect rect
	{
		MicroMeterPnt( 0._MicroMeter, -UM_CELL_SIZE),
		MicroMeterPnt( UM_BOARD_SIZE, 0._MicroMeter)
	};
	m_context.DisplayText
	(
		rect, 
		L"Match finished. The winner is " + m_match.Winner().GetName() + L".",
		m_hTextFormat
	);
}

void BlokusWindow::PaintGraphics()
{
	if (m_pTournament->Active())
	{
 	//	paintBoard();
		//m_match.DrawSetPieces(m_context);
	}
	else
	{
		Player const& player { m_match.ActivePlayerC() };
		player.DrawFreePieces(m_context);
 		paintBoard();
		m_match.DrawSetPieces(m_context);
		if (BlokusPreferences::m_bShowContactPnts.Get())
			m_match.ActivePlayerC().DrawContactPnts(m_context);
		if (player.HasFinished())
			player.DrawResult(m_context, m_hTextFormat);
		if (m_match.MatchFinished())
			drawFinishedMsg();
	}
};
