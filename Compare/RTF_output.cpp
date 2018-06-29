// RTF_output.cpp - output to test protocol file is RTF

#include "stdafx.h"
#include <iostream>
#include "RTF_output.h"
 
RTFOutput::RTFOutput( )
{                         
	wcout << L"{\\rtf1\\pc{\\fonttbl{\\f0\\fmodern Courier New;}} ";
	wcout << L"\\f0\\fs20\\par";
	wcout << L"{\\colortbl;\\red192\\green192\\blue192;}";
}

RTFOutput::~RTFOutput( )
{                          
	StartParagraph( );
	wcout << L"\\sect}"; 
}

void RTFOutput::StartParagraph( int iCount )
{
	while ( --iCount >= 0 )
		wcout << L"\\par "; 
	wcout.flush( );
}

void RTFOutput::Bold( wstring const & strOut )
{
	wcout << L"{\\b ";
	Output( strOut ); 
	wcout << L"}"; 
}

void RTFOutput::BoldUnderlined( wstring const & strOut )
{
	wcout << L"{\\b\\ul ";
	Output( strOut ); 
	wcout << L"}"; 
}

void RTFOutput::Italics( wstring const & strOut )
{
	wcout << L"{\\i ";
	Output( strOut ); 
	wcout << L"}"; 
}

void RTFOutput::ResultFile( void )
{
	wcout << L"\\cbpat1 "; 
}

void RTFOutput::Standard( void )
{
	wcout << L"\\pard "; 
}

void RTFOutput::Output( wstring const & strOut )
{
	for( auto c : strOut )
	{
		wcout << c;
		if ( c == L'\\' )
			wcout << c;        // double every backslash
	}
}
