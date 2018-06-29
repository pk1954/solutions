// output.cpp - output to test protocol file
/*
#include "stdafx.h"
#include "scanner.h"
#include "output.h"
            
static tOUTPUT output;

void InitOutput( tOUTPUT oType )
{                         
   output = oType;
   switch ( output )
   {
   case tOUTPUT::Nothing: 
	   break;
   case tOUTPUT::Text: 
	   break;
   case tOUTPUT::RTF:
       wcout << L"{\\rtf1\\pc{\\fonttbl{\\f0\\fmodern Courier New;}} ";
       wcout << L"\\f0\\fs20\\par";
       wcout << L"{\\colortbl;\\red192\\green192\\blue192;}";
       break;
   default: 
	   break;
   }
}

void StartParagraph( int iCount )
{
	while ( --iCount >= 0 )
	{
		switch ( output )
		{
		case tOUTPUT::Nothing: 
			break;
		case tOUTPUT::Text: 
			wcout << L"\n"; 
			break;
		case tOUTPUT::RTF: 
			wcout << L"\\par "; 
			break;
		default:
			break;
		}
	}
	wcout.flush( );
}

void Bold( wstring const & strOut )
{
	switch ( output )
	{
	case tOUTPUT::Nothing: 
		break;
	case tOUTPUT::Text: 
		Output( strOut ); 
		break;
	case tOUTPUT::RTF: 
		wcout << L"{\\b ";
		Output( strOut ); 
		wcout << L"}"; 
		break;
	default:break;
	}
}

void BoldUnderlined( wstring const & strOut )
{
	switch ( output )
	{
	case tOUTPUT::Nothing: 
		break;
	case tOUTPUT::Text: 
		Output( strOut ); 
		break;
	case tOUTPUT::RTF: 
		wcout << L"{\\b\\ul ";
		Output( strOut ); 
		wcout << L"}"; 
		break;
	default:      
		break;
	}
}

void Italics( wstring const & strOut )
{
	switch ( output )
	{
	case tOUTPUT::Nothing: 
		break;
	case tOUTPUT::Text: 
		Output( strOut ); 
		break;
	case tOUTPUT::RTF: 
		wcout << L"{\\i ";
		Output( strOut ); 
		wcout << L"}"; 
		break; 
	default:      
		break;
	}
}

void LightGrey( void )
{
	switch ( output )
	{
	case tOUTPUT::Nothing: 
		break;
	case tOUTPUT::Text: 
		break;
	case tOUTPUT::RTF: 
		wcout << L"\\cbpat1 "; 
		break;
	default:
		break;
	}
}

void Standard( void )
{
	switch ( output )
	{
	case tOUTPUT::Nothing: 
		break;
	case tOUTPUT::Text: 
		break;
	case tOUTPUT::RTF: 
		wcout << L"\\pard "; 
		break;
	default:      
		break;
	}
}

void Output( wstring const & strOut )
{
	switch ( output )
	{
	case tOUTPUT::Nothing: 
		break;
	case tOUTPUT::Text: 
		wcout << strOut; 
		break;
	case tOUTPUT::RTF: 
	{                        // double every backslash
	    for( auto c : strOut )
		{
		    wcout << c;
			if ( c == L'\\' )
			    wcout << c;
		}
	}
	default: 
		break;
	}
}

void TerminateOutput( void )
{                          
	switch ( output )	
	{
	case tOUTPUT::Nothing: 
		break;
	case tOUTPUT::Text: 
		break;
	case tOUTPUT::RTF: 
		StartParagraph( );
		wcout << L"\\sect}"; 
		break;
	default:      break;
	}
}
*/