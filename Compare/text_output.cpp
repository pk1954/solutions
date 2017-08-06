// text_output.cpp - output to test protocol file is simpla ASCII text

#include "stdafx.h"
#include <iostream>
#include "text_output.h"
            
void TextOutput::StartParagraph( int iCount )
{
	while ( --iCount >= 0 )
		wcout << L"\n"; 
	wcout.flush( );
}

void TextOutput::Output( wstring const & strOut )
{
	wcout << strOut; 
}

void TextOutput::Bold( wstring const & strOut )
{
	Output( strOut ); 
}

void TextOutput::BoldUnderlined( wstring const & strOut )
{
	Output( strOut ); 
}

void TextOutput::Italics( wstring const & strOut )
{
	Output( strOut ); 
}
