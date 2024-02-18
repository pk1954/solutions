// TextBuffer.ixx 
//
// Utilities

module;

#include <iostream>
#include <sstream> 
#include <string> 
#include <chrono>

export module TextBuffer;

import SaveCast;
import Raster;
import Types;

using std::wstring;
using std::wostringstream;
using std::chrono::microseconds;

export using TEXT_POSITION = NamedType<int, struct TEXT_POSITION_Parameter>;

export constexpr TEXT_POSITION operator"" _TEXT_POSITION(unsigned long long ull)
{
	return TEXT_POSITION(Cast2Int(ull));
}

export class TextBuffer
{
public:
	virtual ~TextBuffer() = default;

	void Initialize(PIXEL const, PIXEL const);

	virtual void PrintBuffer(std::wostringstream *, PIXEL const, PIXEL const) = 0;

	virtual void AlignLeft() = 0;
	virtual void AlignRight() = 0;

	virtual void StartPainting();

	void nextLine(TEXT_POSITION iHorPos = 1_TEXT_POSITION)     
	{ 
		setHorizontalPos(iHorPos);
		m_pixVerticalPos += m_pixVertRaster;
	}

	void setHorizontalPos(TEXT_POSITION pos)
	{
		m_pixHorizontalPos = LEFT_MARGIN + m_pixHorRaster * pos.GetValue();
	}

	void nextLine(wstring const & data, TEXT_POSITION iHorPos = 1_TEXT_POSITION)
	{
		nextLine(iHorPos);
		AlignRight();
		printString(data);
	}

	void header(wstring const & data)
	{
		nextLine();
		AlignLeft();
		printString(data);
	}

	void printNumber(auto const data)
	{
		m_wBuffer << data;
		printBuffer();
	}

	void printRasterPoint(RasterPoint const &pnt)
	{
		printNumber(pnt.m_x);
		printNumber(pnt.m_y);
	}

	void printString     (wstring const &);
	void printFloat      (float const);
	void printPercentage (unsigned int const);
	void printPercentage (unsigned int const, unsigned int const);
	void printSpan       (unsigned int const, unsigned int const);
	void printAsMillisecs(microseconds const);

private:
	void printBuffer();

	PIXEL const LEFT_MARGIN { 30_PIXEL };
	PIXEL const TOP_MARGIN  {  5_PIXEL };

	wostringstream m_wBuffer;

	PixelRect m_pixRect; // text buffer area 
	PIXEL     m_pixHorizontalPos;  
	PIXEL     m_pixVerticalPos;
	PIXEL     m_pixHorRaster;
	PIXEL     m_pixVertRaster;
};
