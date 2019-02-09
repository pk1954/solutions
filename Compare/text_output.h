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
	virtual void Output( std::wstring const & );
	virtual void Bold( std::wstring const & );
	virtual void BoldUnderlined( std::wstring const & );
	virtual void Italics( std::wstring const & );
};
