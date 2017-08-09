// Compare.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <locale>
#include <codecvt>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <stdio.h>
#include <exception>
#include "scanner.h"
#include "errhndl.h"
#include "negList.h"
#include "output.h"
#include "RTF_output.h"
#include "text_output.h"
#include "util.h"
#include "win32_util.h"

static wstring strVersion = L"COMPARE V1.2";

class CompareError: public std::exception
{
public:
	CompareError
	( 
		wstring wstrMsg,
		wstring wstrSpec,
		int     iRetCode
	) :
		m_wstrMsg( wstrMsg ),
		m_wstrSpec( wstrSpec ),
		m_iRetCode( iRetCode )
	{}

	wstring m_wstrMsg;
	wstring m_wstrSpec;
	int     m_iRetCode;
};

class CompareEngine
{
public:

	CompareEngine( wstring wstrOutputFormat )
	{
		if ( L"/RTF" == wstrOutputFormat )
			pOut = new RTFOutput( );
		else if ( L"/Text" == wstrOutputFormat )
			pOut = new TextOutput;
		else 
			pOut = new OutputDriver;

	}

	~CompareEngine()
	{
		delete pOut;
	}

	void PrintRestOfResFile( wifstream & ifResults )
	{
		wstring wstrLine;

		while ( ! ifResults.eof() )
		{
			getline( ifResults, wstrLine );
   
			pOut->StartParagraph();
			pOut->Output( wstrLine );
		}        
		pOut->StartParagraph();
		pOut->StartParagraph();
	}
      
	bool GetResLine
	( 
		wifstream          & ifResults,
		NegativeList const & negList
	)
	{
		if ( getline( ifResults, wstrLine ) )
		{               
			wstring strFound = negList.Check( wstrLine );
			if ( ! strFound.empty( ) )
			{
				string::size_type pos = wstrLine.find( strFound );
				pOut->StartParagraph();    	    
				pOut->Output( wstrLine.substr( 0, pos ) );                                       // print faulty line 
				pOut->BoldUnderlined( strFound );                                                // emphasize found negative spec  
				pOut->Output( wstrLine.substr( pos + strFound.length( ), wstrLine.length( ) ) ); // rest of line 
				PrintRestOfResFile( ifResults );
				throw CompareError( L"Following string is not allowed here:", strFound, 3 );
			}
			pOut->StartParagraph();
			return true;
		}
		else
			return false;
	}

	void FindString
	(
		wifstream          & ifResults,
		wstring      const & strPosSpec, // string to be searched for
		NegativeList const & negList	
	)
	{
		for (;;)
		{
			if ( wstrLine.empty( ) && ! GetResLine( ifResults, negList ) )
				throw CompareError( L"Specified string not found:", strPosSpec, 4 );

			wstring::size_type pos = wstrLine.find( strPosSpec );

			if ( wstring::npos == pos )
			{
				pOut->Output( wstrLine );
				wstrLine.clear( );
			}
			else // pos spec found
			{   
				pOut->Output( wstrLine.substr( 0, pos ) );       // print leading part of line 
				pOut->Bold( strPosSpec );                        // emphasize found positive spec  
				wstrLine.erase( 0, pos + strPosSpec.length( ) ); // keep rest of line
				break;            // positive exit
			}
		}
	}

	void SpecError
	(
		Scanner       & scanner,
		wstring const & strMessage, // error message           
		wstring const & strExpected // expected syntax element
	)
	{                    
		scanner.SetExpectedToken( strExpected );
		ScriptErrorHandler::throwError( 125, strMessage );
	}          

	int DoCompare
	( 
		wstring   const & strSpecFile,
		wifstream       & ifResults
	)
	{
		Scanner spec;

		pOut->LightGrey();

		try 
		{  
			tTOKEN       token;
			NegativeList negList;

			spec.OpenInputFile( strSpecFile );
      
			while ( (token = spec.NextToken( false )) != tTOKEN::End )
			{
				if ( token == tTOKEN::Number )   
				{                                   
					unsigned long ulValue = spec.GetUlong( );
					wchar_t chFound = spec.ReadOneOf( L"+-=" );
					if ( chFound == '=' )
					{
						if ( spec.NextToken( true ) != tTOKEN::String )
							SpecError( spec, L"unexpected token", L"string" );

						if ( ! negList.Add( ulValue, spec.GetString( ) ) )
							SpecError( spec, L"Redefinition of negative Spec", L"" );
					}
					else if ( ! negList.SetActive( ulValue, (chFound == '+') ) )    // + or -
	   					SpecError( spec, L"Number undefined", L"" );
				}
				else if ( token == tTOKEN::String )
				{
					FindString( ifResults, spec.GetString( ), negList );
				}
				else if ( (token == tTOKEN::Special) && (spec.GetCharacter( ) == '*') )  
				{                               
					negList.SetAllActive( spec.ReadOneOf( L"+-" ) == '+' );
				}
				else
				{
					SpecError( spec, L"bad token", L"number, string or '*'" );
				}
			}
   
			spec.CloseInputFile( );

			pOut->Output( wstrLine );
			while ( GetResLine( ifResults, negList ) )
			{
				pOut->StartParagraph();
				pOut->Output( wstrLine );
			}        

			pOut->StartParagraph( 2 );
		}
		catch ( ScriptErrorHandler::ScriptErrorInfo const & errInfo )
		{
			ScriptErrorHandler::handleScriptError( errInfo, spec );
			return errInfo.m_sErrNr;
		}
		catch ( CompareError err )
		{
			pOut->Standard();
			pOut->StartParagraph( 2 );
			pOut->Italics( L"+++ comparison failed" );
			pOut->StartParagraph();
			pOut->Italics( err.m_wstrMsg );
			pOut->StartParagraph();
			pOut->Bold( err.m_wstrSpec );
			pOut->StartParagraph();
			pOut->Italics( L"+++ not ok"  );
			pOut->StartParagraph();
			return err.m_iRetCode;
		}

		pOut->Standard();
		pOut->StartParagraph( 2 );
		pOut->Italics( L"*** ok"  );
		pOut->StartParagraph();
		return 0;
	}

	OutputDriver * pOut;
	wstring        wstrLine;
};

int main( int argc, char * argv[] )
{             
	if  ( argc < 3 )
	{           
		wcout << strVersion << endl;
		wcout << L"syntax: compare res spec\n" << endl;
		return 1;
	}

	wstring_convert< std::codecvt_utf8_utf16<wchar_t> > converter;

	wstring strResFile  = converter.from_bytes( argv[1] );
	wstring strSpecFile = converter.from_bytes( argv[2] );

	CompareEngine ce( ( argc >= 4 ) ? converter.from_bytes( argv[3] ) : L"" );

	ScriptErrorHandler::ScrSetOutputStream( & wcout );

	UpperCase( strResFile );
	UpperCase( strSpecFile );

	ce.pOut->StartParagraph();
	ce.pOut->Italics( strVersion );
	ce.pOut->StartParagraph( );
	ce.pOut->Italics( L"Test performed: " + Util::GetCurrentDateAndTime( ) );
	ce.pOut->StartParagraph( );
	ce.pOut->Italics( L"Spec file:   " + strSpecFile );
	ce.pOut->StartParagraph( );
	ce.pOut->Italics( L"Result file: " + strResFile );
	ce.pOut->StartParagraph( );

	wifstream ifResults;

	ifResults.open( strResFile, ios::in );
	if ( ! ifResults )
	{
		ce.pOut->StartParagraph( 2 );
		ce.pOut->Italics( L"+++ Error: Result file not found: " + strResFile  );
		ce.pOut->StartParagraph();
		return 2;
	}

	int iRes = ce.DoCompare( strSpecFile, ifResults );
	
	return iRes;
}
