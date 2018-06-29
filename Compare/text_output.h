// text_output.h - interface for text output driver

#pragma once

#include "output.h"

class TextOutput: public OutputDriver
{
public:
	virtual ~TextOutput( );

	virtual void ResultFile( void );
	virtual void Standard( void );

	virtual void StartParagraph( int );
	virtual void Output( wstring const & );
	virtual void Bold( wstring const & );
	virtual void BoldUnderlined( wstring const & );
	virtual void Italics( wstring const & );
};
