// text_output.cpp - output to test protocol file is simpla ASCII text

#include "stdafx.h"
#include <iostream>
#include "text_output.h"
 
using std::wcout;
using std::wstring;

static wchar_t const ESC = '\033';

TextOutput::~TextOutput()
{
	wcout << ESC << L"[40m";
}

void TextOutput::ResultFile( void ) 
{ 
	wcout << ESC << L"[41m";
}

void TextOutput::Standard( void ) 
{ 
	wcout << ESC << L"[42m";
}

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
