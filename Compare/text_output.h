// text_output.h - interface for text output driver

#pragma once

#include "output.h"

class TextOutput: public OutputDriver
{
public:
	virtual void StartParagraph( int );
	virtual void Output( wstring const & );
	virtual void Bold( wstring const & );
	virtual void BoldUnderlined( wstring const & );
	virtual void Italics( wstring const & );
};
