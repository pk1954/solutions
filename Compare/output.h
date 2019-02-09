// output.h - interface for output driver

#pragma once

#include <string>

class OutputDriver
{
public:
	OutputDriver( ) { }
	
	virtual ~OutputDriver( ) { }

	virtual void ResultFile     ( void ) { }
	virtual void Standard       ( void ) { }
	virtual void TerminateOutput( void ) { }

	virtual void StartParagraph ( int iCount = 1 )  { }
	virtual void Output         ( std::wstring const & ) { }
	virtual void Bold           ( std::wstring const & ) { }
	virtual void BoldUnderlined ( std::wstring const & ) { }
	virtual void Italics        ( std::wstring const & ) { }
};
