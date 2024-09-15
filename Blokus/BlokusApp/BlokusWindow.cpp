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
import Shape;

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
	m_fPixMinSize   = min(GetClientWidth(), GetClientHeight()) - BORDER;
	m_fPixFieldSize = m_fPixMinSize * 0.05f; // 20 fields
	Notify(false);
	return true;
}

void BlokusWindow::showOrientations
(
	Meeple const& meeple,
	fPixel const  fPosVert,
	Color  const  color
) const
{
	fPixelPoint fPos { m_fPixFieldSize, fPosVert };
	meeple.Apply2AllOrientations
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
	showOrientations(m_meeples[16], m_fPixFieldSize *  1.0f, COL_RED);
	showOrientations(m_meeples[17], m_fPixFieldSize *  6.0f, COL_GREEN);
	showOrientations(m_meeples[18], m_fPixFieldSize * 11.0f, COL_BLUE);
	showOrientations(m_meeples[19], m_fPixFieldSize * 16.0f, COL_YELLOW);
	showOrientations(m_meeples[20], m_fPixFieldSize * 20.0f, COL_YELLOW);

	//m_upGraphics->Push();
	//m_upGraphics->Rotation(fPixelPoint(100._fPixel, 500._fPixel), 30.0f);
	//m_upGraphics->Pop();
};

void BlokusWindow::initMeeples()
{
	m_meeples[0] =
	{{
		{ true },
		{ true,  true },
		{ false, true, true  }
	}};
	m_meeples[1] =
	{{
		{ true, true },
		{ true },
		{ true, true }
	}};
	m_meeples[2] =
	{{
		{ true, true, true, true },
		{ true }
	}};
	m_meeples[3] =
	{{
		{ true },
		{ true, true, true },
		{ true }
	}};
	m_meeples[4] =
	{{
		{ true,  true,  true,  true },
		{ false, true }
	}};
	m_meeples[5] =
	{{
		{ true, true, true, true, true }
	}};
	m_meeples[6] =
	{{
		{ true,  true },
		{ false, true, true, true }
	}};
	m_meeples[7] =
	{{
		{ true },
		{ true,  true, true },
		{ false, true }
	}};
	m_meeples[8] =
	{{
		{ true, true, true },
		{ true },
		{ true }
	}};
	m_meeples[9] =
	{{
		{ true, true,  true },
		{ true, true  }
	}};
	m_meeples[10] =
	{{
		{ true },
		{ true,  true,  true },
		{ false, false, true }
	}};
	m_meeples[11] =
	{{
		{ false, true },
		{ true,  true,  true },
		{ false, true }
	}};

	m_meeples[12] =
	{{
		{ true,  true },
		{ true,  true }
	}};
	m_meeples[13] =
	{{
		{ true,  true },
		{ false, true, true }
	}};
	m_meeples[14] =
	{{
		{ true, true, true },
		{ true }
	}};
	m_meeples[15] =
	{{
		{ true },
		{ true, true },
		{ true }
	}};
	m_meeples[16] =
	{{
		{ true, true, true, true }
	}};

	m_meeples[17] =
	{{
		{ true, true },
		{ true }
	}};
	m_meeples[18] =
	{{
		{ true, true, true }
	}};

	m_meeples[19] =
	{{
		{ true, true }
	}};

	m_meeples[20] =
	{{
		{ true }
	}};
}
