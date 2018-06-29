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
	virtual void Bold( wstring const & );
	virtual void BoldUnderlined( wstring const & );
	virtual void Italics( wstring const & );
	virtual void ResultFile( void );
	virtual void Standard( void );
	virtual void Output( wstring const & );
};
