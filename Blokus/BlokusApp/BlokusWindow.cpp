// BlokusWindow.cpp
//
// Blokus

module BlokusWindow;

import Debug;
import Color;
import IoUtil;
import SaveCast;
import DrawContext;
import BoolPreferences;
import Resource;
import BlokusCommands;

using std::abs;
using std::min;
using std::wcout;
using std::endl;
using std::to_wstring;
using std::unique_ptr;
using std::make_unique;

void BlokusWindow::Start
(
	HWND                 const  hwndParent,
	MatchReaderInterface const &mri,
	Sound                      &sound
)
{
	m_pMRI             = &mri;
	m_pSound           = & sound;
	m_hTextFormat      = D2D_driver::NewTextFormat(24.f);
	m_hTextFormatSmall = D2D_driver::NewTextFormat(12.f);
	m_pPlayerVisible   = &m_pMRI->GetPlayerC(PlayerId(0));
	GraphicsWindow::Initialize
	(
		hwndParent, 
		L"ClassBlokusWindow", 
	    WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_VISIBLE
	);
	m_context.Start(m_upGraphics.get());
}

void BlokusWindow::ResetVisiblePlayer()
{
	m_pPlayerVisible = &m_pMRI->GetPlayerC(PlayerId(0));
}

bool BlokusWindow::OnSize(PIXEL const width, PIXEL const height)
{
	GraphicsWindow::OnSize(width, height);
	fPixel     const BORDER        { 10.0_fPixel };
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
		case ' ':
			if (m_pieceMotion.IsActive())
				PostCommand(IDD_NEXT_SHAPE);
			else 
				PostCommand(IDD_NEXT_MOVE);
			break;

		case 'r':
		case 'R':
			PostCommand2Parent(IDD_RESET);
			break;

		case 't':
		case 'T':
			PostCommand2Parent(IDD_START_TOURNAMENT);
			break;

		default:
			break;
	}
}

void BlokusWindow::NextVisiblePlayer()       
{ 
	m_pPlayerVisible = &m_pMRI->NextPlayerC(*m_pPlayerVisible);
	m_pPlayerVisible->Prepare();
	Notify(false);
}

void BlokusWindow::PrevVisiblePlayer()       
{ 
	m_pPlayerVisible = &m_pMRI->PrevPlayerC(*m_pPlayerVisible);
	m_pPlayerVisible->Prepare();
	Notify(false);
}

bool BlokusWindow::humanPlayersTurn () const 
{ 
	return m_pPlayerVisible->IsHuman() && 
		  !m_pPlayerVisible->HasFinished(); 
}

void BlokusWindow::performMove()
{
	NextMoveCmd::Push(m_move);     // may finish if no more valid moves
	if (!m_pPlayerVisible->HasFinished())
		NextVisiblePlayer();
}

void BlokusWindow::autoRun()
{
	if (BoolPreferences::IsActive(BlokusPreferences::m_bPrefAutoMove))
	{
		while (                                  // we run in automatic mode
				!m_pMRI->GameHasFinished() &&    // until game is over
				!humanPlayersTurn()              // or a human player has to act
			  )
		{
			if (m_pPlayerVisible->HasFinished())
				NextVisiblePlayer();
			else
			{
				m_move = m_pMRI->SelectMove(*m_pPlayerVisible);
				if (m_pPlayerVisible->HasFinished())
					NextVisiblePlayer();
				else
					performMove();
			}
		}
	}
}

MoveIter BlokusWindow::nextShape()
{
	MoveIter iterLast { m_iterActShape }; // stop after one complete roundtrip
	MoveIter iterRun  { m_iterActShape };
	do
	{
		if (++iterRun == m_subRangePiece.end())
			iterRun = m_subRangePiece.begin();
	} while ((iterRun != iterLast) && (iterRun->GetShapeId() == iterLast->GetShapeId()));
	return iterRun;
}

bool BlokusWindow::OnCommand(WPARAM const wParam, LPARAM const lParam, PixelPoint const pixPoint)
{
	switch (int const wmId { LoWord(wParam) } )
	{
	case IDD_NEXT_SHAPE:
		if (m_pieceMotion.IsActive())
		{
			m_iterActShape = nextShape();
			m_move = *m_iterActShape;
			Notify(true);
		}
		else 
			m_pSound->WarningSound();
		break;

	case IDD_NEXT_MOVE:   // user requests next move
  		if (m_pMRI->GameHasFinished())
			NextVisiblePlayer();
		else 
		{
			if (m_pPlayerVisible->HasFinished())
			{
				NextVisiblePlayer();
			}
			else if (m_pPlayerVisible->IsHuman())
			{
				m_pSound->WarningSound();    // no automatic move for human players
			}
			else                             // AI move
			{
				m_move = m_pMRI->SelectMove(*m_pPlayerVisible);
				performMove();
				autoRun();
			}
		}
		Notify(false);
		break;

	default:
		return GraphicsWindow::OnCommand(wParam, lParam, pixPoint);
	}

	return true;
}

bool BlokusWindow::OnLButtonUp(WPARAM const wParam, LPARAM const lParam)
{
	ReleaseCapture();
	if (m_pieceMotion.IsActive())
	{
		if (m_pMRI->IsValidPosition(m_move))
		{
			performMove();
			autoRun();
		}
		m_move.Reset();
		m_pieceMotion.Reset();
		Notify(false);
	}
	return GraphicsWindow::OnLButtonUp(wParam, lParam);
}

MicroMeterPnt BlokusWindow::getCrsrPos(LPARAM const lParam) const
{
	fPixelPoint   const fPixPosCrsr { GetCrsrPosFromLparamF(lParam) };
	MicroMeterPnt const umCrsrPos   { m_context.GetCoordC().Transform2logUnitPntPos(fPixPosCrsr) };
	return umCrsrPos;
}

bool BlokusWindow::OnLButtonDown(WPARAM const wParam, LPARAM const lParam)
{
	if (m_pPlayerVisible->IsHuman())
	{
		if (m_pieceMotion.FindPiece(*m_pPlayerVisible, getCrsrPos(lParam)))
		{
			m_subRangePiece = m_pPlayerVisible->GetMoves(m_pieceMotion.GetPieceTypeId());
			m_iterActShape  = m_subRangePiece.begin();
			m_move = *m_iterActShape;
			m_move.SetCoordPos(m_pieceMotion.GetCoordPos());
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
		{
			m_move.SetCoordPos(m_pieceMotion.GetCoordPos());
			CoordPos const coordPosBestFit { m_pMRI->FindBestFit(m_move) };
			if (IsDefined(coordPosBestFit))
				m_move.SetCoordPos(coordPosBestFit);
		}
		Notify(false);
	}
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
	m_pMRI->Apply2AllFreeCellsC
	(
		[this, &player](CoordPos const& coordPos) 
		{  
			if (player.IsBlocked(coordPos))
			{
				MicroMeterPnt  const umPos  { Convert2fCoord(coordPos) };
				MicroMeterRect const umRect { umPos, UM_CELL_SIZE };
				m_context.DrawRectangle(umRect, COL_BLACK, 2.0_fPixel);
				m_context.FillRectangle(umRect, Color(0.0f, 0.0f, 0.0f, 0.3f));
			}
		}
	);
}

void BlokusWindow::PaintGraphics()
{
	m_pPlayerVisible->Prepare();
	paintBoard();
	m_pMRI->DrawSetPieces(m_context);
	m_pPlayerVisible->DrawFreePieces(m_context, m_pieceMotion.GetPieceC());
	if (m_pieceMotion.IsActive())
	{
		if (m_move.IsCompletelyOnBoard())
		{
			Color const color { m_pMRI->IsValidPosition(m_move) ? m_pPlayerVisible->GetColor() : COL_BLACK};
			m_pMRI->DrawMovePiece(m_context, m_move, color);                                                     // the shadow
		}
		if (BoolPreferences::IsActive(BlokusPreferences::m_bPrefShowMoveDetail))
			m_pMRI->DrawMovePiece(m_context, m_move, m_pPlayerVisible->GetColor(), m_pieceMotion.GetPosition());  // the original piece
	}
	if (BoolPreferences::IsActive(BlokusPreferences::m_bPrefShowContactPnts))
		m_pPlayerVisible->DrawContactPnts(m_context);
 	if (m_pPlayerVisible->HasFinished())
		m_pPlayerVisible->DrawResult(m_context, m_hTextFormat);
	if (m_pMRI->GameHasFinished())
		drawFinishedMsg();
	if (BoolPreferences::IsActive(BlokusPreferences::m_bPrefShowCellNumbers))
		drawCellNumbers();
	if (BoolPreferences::IsActive(BlokusPreferences::m_bPrefShowBlockedCells))
		drawBlockedCells(*m_pPlayerVisible);
};
