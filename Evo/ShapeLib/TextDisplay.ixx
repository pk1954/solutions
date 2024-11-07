// TextDisplay.ixx : 
//
// ShapeLib
// 
export module ShapeLib:TextDisplay;

import std;
import Types;
import WinBasics;
import EvoCoreLib;

using std::wostringstream;
using std::wstring;

export class TextDisplay
{
public:
	TextDisplay()
	  : m_pBuffer(nullptr),
		m_pEvoPixelCoords(nullptr)
	{ }

	void Start
	(
		wostringstream *pBuffer,
		EvoPixelCoords *pEvoPixelCoords
	)
	{
		m_pBuffer         = pBuffer;
		m_pEvoPixelCoords = pEvoPixelCoords;
	}

	void Clear()
	{
		m_pBuffer->str(wstring());
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

	virtual PixelRectSize CalcRectSize()                = 0;
	virtual void          DisplayText(PixelRect const&) = 0;
	
private:
	static COLORREF const CLR_WHITE = MakeRGB(255, 255, 255);

	wostringstream * m_pBuffer;
	EvoPixelCoords * m_pEvoPixelCoords;
};