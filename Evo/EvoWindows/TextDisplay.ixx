// TextDisplay.ixx
//
// EvoWindows

export module TextDisplay;

import std;
import EvoCoreLib;
import GraphicsInterface;

using std::wostringstream;

export class TextDisplay
{
public:
	TextDisplay()
	  : m_pGraphics(nullptr),
		m_pBuffer(nullptr),
		m_pEvoPixelCoords(nullptr)
	{ }

	void Start
	(
		GraphicsInterface* pGgraphicsInterface,
		wostringstream* pBuffer,
		EvoPixelCoords* pEvoPixelCoords
	)
	{
		m_pGraphics = pGgraphicsInterface;
		m_pBuffer = pBuffer;
		m_pEvoPixelCoords = pEvoPixelCoords;
	}

	void Clear()
	{
		m_pBuffer->str(std::wstring());
		m_pBuffer->clear();
	}

	wostringstream& Buffer()
	{
		return *m_pBuffer;
	}

	PIXEL GetFieldSize() const
	{
		return m_pEvoPixelCoords->GetFieldSize();
	}

	PixelPoint GetOffset(GridPoint const gp)
	{
		return m_pEvoPixelCoords->Grid2PixelPos(gp);
	}

	PixelPoint GetCenterOffset(GridPoint const gp)
	{
		return m_pEvoPixelCoords->Grid2PixelPosCenter(gp);
	}

	PixelRectSize CalcRectSize()
	{
		return m_pGraphics->CalcGraphicsRect(m_pBuffer->str()).GetSize();
	}

	void DisplayText(PixelRect const& rect)
	{
		m_pGraphics->DisplayGraphicsText(rect, m_pBuffer->str(), DT_LEFT, CLR_WHITE);
	}

private:
	static COLORREF const CLR_WHITE = RGB(255, 255, 255);

	GraphicsInterface * m_pGraphics;
	wostringstream    * m_pBuffer;
	EvoPixelCoords    * m_pEvoPixelCoords;
};
