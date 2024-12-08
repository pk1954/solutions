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

using std::min;
using std::wcout;
using std::endl;
using std::unique_ptr;
using std::make_unique;

fPixel const BORDER { 10.0_fPixel };

void BlokusWindow::Start(HWND const hwndParent)
{
	GraphicsWindow::Initialize
	(
		hwndParent, 
		L"ClassBlokusWindow", 
	    WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_VISIBLE
	);
	m_context.Start(m_upGraphics.get());
	m_hTextFormat = m_upGraphics->NewTextFormat(24.f);
   	m_match.Reset();
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

void BlokusWindow::Reset()
{
	m_match.Reset();
	Notify(false);
}

//void BlokusWindow::autoRun()
//{
//	while (m_move = m_match.NextMove(), !m_move.IsDefined())
//	{
//		if (m_match.HasFinished())
//		{
//			m_bAutoRun = false;
//			Notify(true);
//			return;
//		}
//		else
//		{
//			m_match.NextPlayer();
//			Notify(true);
//		}
//	}
//	NextMoveCmd::Push(m_move, m_match);
//}

//void BlokusWindow::performMove()
//{
//	PosDir &posDirAct { m_match.GetPosDir(m_move) };
////	m_posDirTarget = PosDir(Convert2fCoord(m_move.GetCoordPos()), shape.GetRotation());
//	m_posDirTarget = PosDir(Convert2fCoord(m_move.GetCoordPos()), 0._Degrees);
//	if (BlokusPreferences::m_bShowAnimation.Get())
//	{
//		m_iAnimationPhase = 1;
//		m_posDirAnimation.Start(&posDirAct, posDirAct, m_posDirTarget);
//	}
//	else
//	{
//		posDirAct = m_posDirTarget;
//		PostCommand(IDX_FINISH_MOVE);
//	}
//}

bool BlokusWindow::OnCommand(WPARAM const wParam, LPARAM const lParam, PixelPoint const pixPoint)
{
	switch (int const wmId { LoWord(wParam) } )
	{
	case IDD_RESET:
		if (!m_bAutoRun && !m_posDirAnimation.IsRunning())
			Reset();
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
		{
			NextMoveCmd::Push(m_match);
			Notify(true);
		}

		break;

	//case IDX_NEXT_AUTO_MOVE:
	//	autoRun();
	//	break;

	case IDX_ANIMATION_UPDATE:
		m_posDirAnimation.Update();
		Notify(true);
		break;

	//case IDX_FINISH_MOVE:
	//	if (m_iAnimationPhase == 1)
	//	{
	//		Degrees const degrees { m_match.GetRotation(m_move) };
	//		if (degrees != 0._Degrees)
	//		{
	//			startRotationPhase(degrees);
	//			break;
	//		}
	//	}
	//	finishMove();
	//	break;

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
	Piece const * const pPiece { m_pieceMotion.SelectPiece(m_match.ActivePlayer(), getCrsrPos(lParam)) };
	if (pPiece)
	{
		PieceType const &pieceType { pPiece->GetPieceTypeC() };
		m_move.SetPieceType(pieceType);
	    m_move.SetPlayerId(m_match.ActivePlayerId());
		m_move.SetShapeId(ShapeId(0));
		m_move.SetCoordPos(m_pieceMotion.GetPosition());
		SetCapture();
	}
	return GraphicsWindow::OnLButtonDown(wParam, lParam);
}

void BlokusWindow::OnMouseMove(WPARAM const wParam, LPARAM const lParam)
{
	if ((wParam & MK_LBUTTON) && isPieceSelected())
	{
		if (m_pieceMotion.MovePiece(getCrsrPos(lParam)))
		{
			m_move.SetCoordPos(m_pieceMotion.GetPosition());
			Notify(false);
		}
	}
}

bool BlokusWindow::OnLButtonUp(WPARAM const wParam, LPARAM const lParam)
{
	ReleaseCapture();
	if (isPieceSelected())
	{
		if (m_match.IsValidPosition(m_move))
			m_match.DoMove(m_move);
		else
			m_match.ResetPiece(m_move);
		m_move.Reset();
		m_pieceMotion.Reset();
		Notify(false);
	}
	return GraphicsWindow::OnLButtonUp(wParam, lParam);
}

//void BlokusWindow::startRotationPhase(Degrees const degrees)
//{
//	PosDir &posDirAct { m_match.GetPosDir(m_move) };
//	m_posDirTarget    = PosDir(Convert2fCoord(m_move.GetCoordPos()), degrees);
//	m_iAnimationPhase = 2;
//	m_posDirAnimation.Start(&posDirAct, posDirAct, m_posDirTarget);
//}

//void BlokusWindow::finishMove()
//{
//	m_match.FinishMove(m_move);
//	m_match.NextPlayer();
//	Notify(true);
//	if (m_bAutoRun)
//		PostCommand(IDX_NEXT_AUTO_MOVE);
//}

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
	Player const& player { m_match.ActivePlayerC() };
 	paintBoard();
	m_match.DrawSetPieces(m_context);
	m_match.DrawFreePieces(m_context, m_move);
	if (m_pieceMotion.IsActive())
		m_match.DrawMovePiece(m_context, m_move);
	if (BlokusPreferences::m_bShowContactPnts.Get())
		player.DrawContactPnts(m_context);
	if (player.HasFinished())
		player.DrawResult(m_context, m_hTextFormat);
	if (m_match.HasFinished())
		drawFinishedMsg();
};
