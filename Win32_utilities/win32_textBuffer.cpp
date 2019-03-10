// win32_textBuffer.cpp :
//

#include "stdafx.h"
#include <locale>
#include <iomanip>
#include "util.h"
#include "PixelTypes.h"
#include "win32_util.h"
#include "win32_textBuffer.h"

using std::wstring;
using std::setprecision;
using std::setw;

Win32_TextBuffer::Win32_TextBuffer
( 
	HDC     const hdc, 
	PIXEL_X const cxChar,
	PIXEL_Y const cyChar,
	PIXEL_X const horRaster
) : 
    m_hDC( hdc )
{ 
	Initialize( cxChar, cyChar, horRaster );
}

void Win32_TextBuffer::PrintBuffer
( 
	std::wostringstream * pwBuffer,
	PIXEL_X const xPos,
	PIXEL_Y const yPos
)
{
	wstring const wstr { pwBuffer->str() };
    (void)TextOut
	( 
		m_hDC, 
		xPos.GetBaseValue(), 
		yPos.GetBaseValue(), 
		wstr.c_str(), 
		static_cast<int>(wstr.size()) 
	);
}
