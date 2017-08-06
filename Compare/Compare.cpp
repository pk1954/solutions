// Compare.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <locale>
#include <codecvt>
#include <string>
#include <stdio.h>
#include "scanner.h"
#include "errhndl.h"
#include "negList.h"
#include "output.h"

static wstring strVersion = L"COMPARE V1.2";

static void UpperCase( wstring & str )
{
	for ( auto & c: str ) 
		c = toupper(c);
}

static void Error( wstring const & strMsg, wstring const & strString )
{
   Standard();
   StartParagraph( 2 );
   Italics( L"comparison failed" );
   StartParagraph();
   Italics( strMsg );
   StartParagraph();
   Bold( strString );
   StartParagraph( 2 );
   Italics( L"+++ not ok" );
   StartParagraph();
   exit(4);
}

// ReadOneOf: Try to read one of a group of given characters
   
static wchar_t ReadOneOf(
                           Scanner       & scanner, 
                           wstring const & strValid // expected characters
                        ) 
{ 
   tTOKEN token = scanner.NextToken( true );
   
   if ( token == tTOKEN::Special )
   {
      wchar_t chRes = scanner.GetCharacter( );                      
	  size_t pos = strValid.find( chRes );
      if ( NULL != wstring::npos )
         return chRes;
   }

   scanner.SetExpectedToken( L"one of \"" + strValid + L"\"" );

   if ( token == tTOKEN::End ) 
      ScriptErrorHandler::eofError( );
   else
      ScriptErrorHandler::tokenError( );

   return (wchar_t)0; 
}

static void PrintRestOfResFile( wifstream & ifResults )
{
	wstring wstrLine;

	while ( ! ifResults.eof() )
	{
		getline( ifResults, wstrLine );
   
		StartParagraph();
		Output( wstrLine );
	}        
	StartParagraph();
	StartParagraph();
}
      
static wstring GetResLine
( 
	wifstream          & ifResults,
	NegativeList const & negList 
)
{
	wstring wstrLine;

	if ( getline( ifResults, wstrLine ) )
    {               
		wstring strFound = negList.Check( wstrLine );
		if ( ! strFound.empty( ) )
		{
			std::string::size_type pos = wstrLine.find( strFound );
			StartParagraph();    	    
			Output( wstrLine.substr( 0, pos ) );                                       // print faulty line 
			BoldUnderlined( strFound );                                                // with with accentuation of found negative spec  
			Output( wstrLine.substr( pos + strFound.length( ), wstrLine.length( ) ) ); // rest of line 
			PrintRestOfResFile( ifResults );
			Error( L"Following string is not allowed here:", strFound );
		}
		StartParagraph();
		return wstrLine;
	}
	else
		return L"";
}

static void FindString
(
	wifstream          & ifResults,
	wstring      const & strPosSpec, /* string to be searched for */
	NegativeList const & negList	
)
{
	for (;;)
	{
	    wstring wstrLine = GetResLine( ifResults, negList );
		if ( wstrLine.empty( ) )
		{
			Error( L"Specified string not found:", strPosSpec );
		}

		wstring::size_type pos = wstrLine.find( strPosSpec );

		if ( wstring::npos == pos )
		{
			Output( wstrLine );
		}
		else
		{   
			Output( wstrLine.substr( 0, pos ) );                                         // print faulty line 
			Bold( strPosSpec );                                                          // with with accentuation of found positive spec  
			Output( wstrLine.substr( pos + strPosSpec.length( ), wstrLine.length( ) ) ); // rest of line 
			return;            // positive exit
		}
	}
}

static void SpecError
(
	Scanner       & scanner,
	wstring const & strMessage, // error message           
	wstring const & strExpected // expected syntax element
)
{                    
   const int     iLineNr = scanner.GetActLineNr( );
   const wstring strPath = scanner.GetActPath( );

   wcout << L"*** syntax error in spec file " << strPath << endl;

   if ( iLineNr > 0 )
      wcout << L"*** line " << iLineNr << endl;

   wcout << scanner.GetActLine( );

   ScriptErrorHandler::PrintMarkerLine( scanner );

   if ( ! strMessage.empty( ) )
      wcout << L"*** " << strMessage << endl;

   if ( ! strExpected.empty( ) )
      wcout << L"*** expected " << strExpected << endl;

   wcout << L"*** error exit" << endl << endl;
   exit( 3 );
}          

static wstring toWstring(char const * const pch )
{
	wstring_convert< std::codecvt_utf8_utf16<wchar_t> > converter;
	wstring wstr = converter.from_bytes( pch );
	return wstr;
}

static void ProcessSpecFile
( 
	wifstream     & ifResults,
	wstring const & strSpecFile 
)
{
	Scanner      spec;
    tTOKEN       token;
    NegativeList negList;

    spec.OpenInputFile( strSpecFile );
      
    while ( (token = spec.NextToken( false )) != tTOKEN::End )
    {
       if ( token == tTOKEN::Number )   
       {                                   
          unsigned long ulValue = spec.GetUlong( );
          wchar_t chFound = ReadOneOf( spec, L"+-=" );
          if ( chFound == '=' )
		  {
             spec.NextToken( true );
             if ( ! negList.Add( ulValue, spec.GetString( ) ) )
		        SpecError( spec, L"Redefinition of negative Spec", L"" );
		  }
          else
		  {
             if ( ! negList.SetActive( ulValue, chFound == '+' ) )
	   	       SpecError( spec, L"Number undefined", L"" );
		  }
       }
       else if ( token == tTOKEN::String )
       {
          FindString( ifResults, spec.GetString( ), negList );
       }
       else if ( (token == tTOKEN::Special) && (spec.GetCharacter( ) == '*') )  
       {                               
          negList.SetAllActive( ReadOneOf( spec, L"+-" ) == '+' );
       }
       else
       {
          SpecError( spec, L"bad token", L"number, string or '*'" );
       }
    }
   
	spec.CloseInputFile( );

	for (;;)
	{
	    wstring wstrLine = GetResLine( ifResults, negList );
		if ( wstrLine.empty( ) )
			break;
        StartParagraph();
        Output( wstrLine );
    }        

    StartParagraph( 2 );
}

int main( int argc, char * argv[] )
{             
	if  ( argc < 3 )
	{           
		wcout << strVersion << endl;
		wcout << L"syntax: compare res spec\n" << endl;
		return 1;
	}
    else
    {
		wstring strResFile  = toWstring( argv[1] );
		wstring strSpecFile = toWstring( argv[2] );
		wstring strOptions;

		tOUTPUT output = tOUTPUT::Nothing;

		if  ( argc >= 4 )
		{
			strOptions = toWstring( argv[3] );
			if ( L"/RTF" == strOptions )
				output = tOUTPUT::RTF;
			if ( L"/Text" == strOptions )
				output = tOUTPUT::Text;
		}

		UpperCase( strResFile );
		UpperCase( strSpecFile );

		InitOutput( output );
		atexit( TerminateOutput );

		StartParagraph();
		Italics( strVersion );
		StartParagraph();
		Italics( L"Spec file:   " + strSpecFile );
		StartParagraph( 3 );
		Italics( L"Result file: " + strResFile );
		StartParagraph();

		wifstream ifResults;

		ifResults.open( strResFile, ios::in  );
		if ( ! ifResults )
		{
			wcout << L"+++ Error: Result file not found: " << endl << strResFile << endl;
			return 2;
		}

		LightGrey();
		ProcessSpecFile( ifResults, strSpecFile );
		Standard();
		StartParagraph( 2 );
		Italics( L"+++ ok" );
		StartParagraph();
	}

	return 0;
}
