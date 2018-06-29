// output.h - interface for output driver

#pragma once

#include <string>

using namespace std;

class OutputDriver
{
public:
	OutputDriver( ) { }
	
	virtual ~OutputDriver( ) { }

	virtual void ResultFile      ( void ) { }
	virtual void Standard       ( void ) { }
	virtual void TerminateOutput( void ) { }

	virtual void StartParagraph ( int iCount = 1 )  { }
	virtual void Output         ( wstring const & ) { }
	virtual void Bold           ( wstring const & ) { }
	virtual void BoldUnderlined ( wstring const & ) { }
	virtual void Italics        ( wstring const & ) { }
};
