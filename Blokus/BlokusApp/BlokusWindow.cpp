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
	initMeeples();
}

bool BlokusWindow::OnSize(PIXEL const width, PIXEL const height)
{
	GraphicsWindow::OnSize(width, height);
	fPixel const fPixAvailable { GetClientHeight() - BORDER * 2.0f };
	m_fPixFieldSize = fPixAvailable / 22.f; // 20 fields + top and bottom reserve
	m_fPixBoardSize = m_fPixFieldSize * 20.f;
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
	m_upGraphics->FillRectangle(fPixelRect(m_fPixPntOrigin, fPixRectSizeBoard), BOARD_COLOR);
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

	for (PieceType const& pt : m_pieceTypes)
	{
		Pos const pos { pt.GetPos() };
		fPixelPoint const fPos 
		{ 
			m_fPixFieldSize * pos.m_x.GetValue() + m_fPixFieldSize * 0.5f,
			m_fPixFieldSize * pos.m_y.GetValue() + m_fPixFieldSize * 0.5f
		};
		pt.Draw
		(
			*m_upGraphics.get(),
			fPixPntOriginPieces + fPos,
			COL_RED,
			m_fPixFieldSize
		);
	}
};

void BlokusWindow::PaintGraphics()
{
	paintBoard();
	paintPieces();
};

void BlokusWindow::initMeeples()
{
	m_pieceTypes[0] =
	{{
		{ true, true, true, true },
		{ true }
	}};
	m_pieceTypes[0].SetPos(4, 0);

	m_pieceTypes[1] =
	{{
		{ true,  true,  true,  true },
		{ false, true }
	}};
	m_pieceTypes[1].SetPos(8, 8);

	m_pieceTypes[2] =
	{{
		{ true, true, true, true, true }
	}};
	m_pieceTypes[2].SetPos(7, 11);

	m_pieceTypes[3] =
	{{
		{ true, true, true },
		{ true },
		{ true }
	}};
	m_pieceTypes[3].SetPos(0, 0);

	m_pieceTypes[4] =
	{{
		{ true },
		{ true,  true },
		{ false, true, true  }
	}};
	m_pieceTypes[4].SetPos(6, 2);

	m_pieceTypes[5] =
	{{
		{ false, true },
		{ true,  true,  true },
		{ false, true }
	}};
	m_pieceTypes[5].SetPos(9, 0);

	m_pieceTypes[6] =
	{{
		{ true },
		{ true,  true, true },
		{ false, true }
	}};
	m_pieceTypes[6].SetPos(0, 9);

	m_pieceTypes[7] =
	{{
		{ true,  true },
		{ false, true, true, true }
	}};
	m_pieceTypes[7].SetPos(4, 5);

	m_pieceTypes[8] =
	{{
		{ true },
		{ true, true, true },
		{ true }
	}};
	m_pieceTypes[8].SetPos(2, 2);

	m_pieceTypes[9] =
	{{
		{ true },
		{ true,  true,  true },
		{ false, false, true }
	}};
	m_pieceTypes[9].SetPos(0, 5);

	m_pieceTypes[10] =
	{{
		{ true, true },
		{ true },
		{ true, true }
	}};
	m_pieceTypes[10].SetPos(10, 4);

	m_pieceTypes[11] =
	{{
		{ true, true,  true },
		{ true, true  }
	}};
	m_pieceTypes[11].SetPos(4, 8);


	m_pieceTypes[12] =
	{{
		{ true,  true },
		{ true,  true }
	}};
	m_pieceTypes[12].SetPos(2, 16);

	m_pieceTypes[13] =
	{{
		{ true,  true },
		{ false, true, true }
	}};
	m_pieceTypes[13].SetPos(8, 16);

	m_pieceTypes[14] =
	{{
		{ true, true, true },
		{ true }
	}};
	m_pieceTypes[14].SetPos(0, 14);

	m_pieceTypes[15] =
	{{
		{ true },
		{ true, true },
		{ true }
	}};
	m_pieceTypes[15].SetPos(5, 14);

	m_pieceTypes[16] =
	{{
		{ true, true, true, true }
	}};
	m_pieceTypes[16].SetPos(8, 14);


	m_pieceTypes[17] =
	{{
		{ true },
		{ true, true }
	}};
	m_pieceTypes[17].SetPos(0, 18);

	m_pieceTypes[18] =
	{{
		{ true, true, true }
	}};
	m_pieceTypes[18].SetPos(4, 19);


	m_pieceTypes[19] =
	{{
		{ true, true }
	}};
	m_pieceTypes[19].SetPos(8, 19);


	m_pieceTypes[20] =
	{{
		{ true }
	}};
	m_pieceTypes[20].SetPos(11, 19);

}
