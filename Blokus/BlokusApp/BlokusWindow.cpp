// BlokusWindow.cpp
//
// Blokus

module BlokusWindow;

import Color;
import IoUtil;
import SaveCast;
import DrawContext;
import Resource;
import BlokusCommands;

using std::abs;
using std::min;
using std::wcout;
using std::endl;
using std::to_wstring;
using std::unique_ptr;
using std::make_unique;

fPixel const BORDER { 10.0_fPixel };

void BlokusWindow::Start
(
	HWND                 const  hwndParent,
	MatchReaderInterface const &mri,
	Sound                      &sound
)
{
	m_pMRI   = &mri;
	m_pSound = & sound;
	GraphicsWindow::Initialize
	(
		hwndParent, 
		L"ClassBlokusWindow", 
	    WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_VISIBLE
	);
	m_context.Start(m_upGraphics.get());
	m_hTextFormat      = m_upGraphics->NewTextFormat(24.f);
	m_hTextFormatSmall = m_upGraphics->NewTextFormat(12.f);
	m_posDirAnimation.SetUpdateLambda
	(
		[this](bool const bTargetReached)
		{
			PostCommand(IDX_ANIMATION_UPDATE);
			if (bTargetReached)
				PostCommand(IDX_FINISH_MOVE);
		}
	);
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
		case 'n':
		case 'N':
			PostCommand(IDD_NEXT_MOVE);
			break;

		case ' ':
			PostCommand(IDD_NEXT_SHAPE);
			break;

		//case 'a':
		//case 'A':
		//	PostCommand(IDD_START_AUTO_RUN);
		//	break;

		case 'r':
		case 'R':
			PostCommand(IDD_RESET);
			break;

		case 't':
		case 'T':
			PostCommand2Parent(IDD_START_TOURNAMENT);
			break;

		default:
			break;
	}
}

bool BlokusWindow::OnCommand(WPARAM const wParam, LPARAM const lParam, PixelPoint const pixPoint)
{
	switch (int const wmId { LoWord(wParam) } )
	{
	case IDD_RESET:
		if (!m_bAutoRun && !m_posDirAnimation.IsRunning())
			ResetMatchCmd::Push();
		break;

	case IDD_START_AUTO_RUN:
		//if (!m_bAutoRun && !m_posDirAnimation.IsRunning())
		//{
		//	m_bAutoRun = true;
		//	autoRun();
		//}
		break;

	case IDD_NEXT_MOVE:
//		if (!m_bAutoRun && !m_posDirAnimation.IsRunning())
		if (m_pMRI->ActivePlayerC().IsHuman())
		{
			m_pSound->WarningSound();    // no automatic move for human players
		}
		else if (!m_pMRI->ActivePlayerC().HasFinished())
		{
			BlokusMove move { m_pMRI->SelectMove() };
			if (move.IsDefined())
			{
				Assert(m_pMRI->IsValidPosition(move));
				NextMoveCmd::Push(move);  // may finish if no more valid moves
			}
		}
		NextPlayerCmd::Push();
		Notify(true);
		break;

	case IDD_NEXT_SHAPE:
//		if (!m_bAutoRun && !m_posDirAnimation.IsRunning())
		if (m_pieceMotion.IsActive())
		{
			m_move.NextShape();
			Notify(true);
		}
		else 
			m_pSound->WarningSound();
		break;

	//case IDX_NEXT_AUTO_MOVE:
	//	autoRun();
	//	break;

	case IDX_ANIMATION_UPDATE:
		m_posDirAnimation.Update();
		Notify(true);
		break;

	default:
		return GraphicsWindow::OnCommand(wParam, lParam, pixPoint);
	}

	return true;
}

MicroMeterPnt BlokusWindow::getCrsrPos(LPARAM const lParam) const
{
	fPixelPoint   const fPixPosCrsr { GetCrsrPosFromLparamF(lParam) };
	MicroMeterPnt const umCrsrPos   { m_context.GetCoordC().Transform2logUnitPntPos(fPixPosCrsr) };
	return umCrsrPos;
}

bool BlokusWindow::OnLButtonDown(WPARAM const wParam, LPARAM const lParam)
{
	if (m_pMRI->ActivePlayerC().IsHuman())
	{
		if (m_pieceMotion.FindPiece(m_pMRI->ActivePlayerC(), getCrsrPos(lParam)))
		{
			m_move.SetPieceType(m_pieceMotion.GetPieceTypeC());
			m_move.SetPlayerId (m_pMRI->ActivePlayerId());
			m_move.SetShapeId  (ShapeId(0));
			m_move.SetCoordPos (m_pieceMotion.GetCoordPos());
			SetCapture();
		}
	}
	return GraphicsWindow::OnLButtonDown(wParam, lParam);
}

void BlokusWindow::OnMouseMove(WPARAM const wParam, LPARAM const lParam)
{
	if ((wParam & MK_LBUTTON) && m_pieceMotion.IsActive())
	{
		if (m_pieceMotion.MovePiece(getCrsrPos(lParam)))
			m_move.SetCoordPos(m_pieceMotion.GetCoordPos());
		Notify(false);
	}
}

bool BlokusWindow::OnLButtonUp(WPARAM const wParam, LPARAM const lParam)
{
	ReleaseCapture();
	if (m_pieceMotion.IsActive())
	{
		if (m_pMRI->IsValidPosition(m_move))
		{
			NextMoveCmd::Push(m_move);
			NextPlayerCmd::Push();
		}
		m_move.Reset();
		m_pieceMotion.Reset();
		Notify(false);
	}
	return GraphicsWindow::OnLButtonUp(wParam, lParam);
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
		L"Match finished. The winner is " + m_pMRI->Winner().GetName() + L".",
		m_hTextFormat	
	);
}

void BlokusWindow::drawCellNumbers()
{
	Apply2AllBoardCells
	(
		[this](CoordPos const& coordPos) 
		{  
			MicroMeterPnt  const umPos  { Convert2fCoord(coordPos) };
			MicroMeterRect const umRect { umPos, UM_CELL_SIZE };
			m_context.DisplayText
			(
				umRect, 
				to_wstring(coordPos.GetXvalue()) + SEPARATOR + to_wstring(coordPos.GetYvalue()),
				m_hTextFormatSmall
			);
		}
	);
}

void BlokusWindow::drawBlockedCells(Player const& player)
{
	Apply2AllBoardCells
	(
		[this, &player](CoordPos const& coordPos) 
		{  
			if (player.IsBlocked(coordPos))
			{
				MicroMeterPnt  const umPos  { Convert2fCoord(coordPos) };
				MicroMeterRect const umRect { umPos, UM_CELL_SIZE };
				m_context.DrawRectangle(umRect, Color(0.0f, 0.0f, 0.0f), 2.0_fPixel);
			}
		}
	);
}

void BlokusWindow::PaintGraphics()
{
	Player const& player { m_pMRI->ActivePlayerC() };
 	paintBoard();
	m_pMRI->DrawSetPieces(m_context, m_hTextFormatSmall);
	player.DrawFreePieces(m_context, m_pieceMotion.GetPieceC(), m_hTextFormatSmall);
	if (m_pieceMotion.IsActive())
	{
		if (m_move.IsCompletelyOnBoard())
			m_pMRI->DrawMovePiece(m_context, m_move, m_hTextFormatSmall);
		if (BlokusPreferences::m_bShowMoveDetail.Get())
			m_pMRI->DrawMovePiece(m_context, m_move, m_pieceMotion.GetPosition(), m_hTextFormatSmall);
	}
	if (BlokusPreferences::m_bShowContactPnts.Get())
		player.DrawContactPnts(m_context);
	if (player.HasFinished())
		player.DrawResult(m_context, m_hTextFormat);
	if (m_pMRI->HasFinished())
		drawFinishedMsg();
	if (BlokusPreferences::m_bShowCellNumbers.Get())
		drawCellNumbers();
	if (BlokusPreferences::m_bShowBlockedCells.Get())
		drawBlockedCells(player);
};
