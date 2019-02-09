// RTF_output.h - interface for RTF output driver

#pragma once

#include <string>
#include "output.h"

class RTFOutput: public OutputDriver
{
public:
	RTFOutput( );

	virtual ~RTFOutput( );

	virtual void StartParagraph( int iCount = 1 );
	virtual void Bold( std::wstring const & );
	virtual void BoldUnderlined( std::wstring const & );
	virtual void Italics( std::wstring const & );
	virtual void ResultFile( void );
	virtual void Standard( void );
	virtual void Output( std::wstring const & );
};
