//                                                                          
// File:     ERRHNDL.C                                                     
//                                                                          
// errhndl.cpp - error handler
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "scanner.h"
#include "errhndl.h"

using namespace std;

wostream * ScriptErrorHandler::m_pScriptTrace = & std::wcout;

// throwError: General Error handler for input script errors

void ScriptErrorHandler::throwError
(
    short   const   sErrNr,      // error number            
    wstring const & wstrMessage  // error message           
)
{
    ScriptErrorInfo errInfo;             

    errInfo.m_sErrNr      = sErrNr;                 
    errInfo.m_wstrMessage = wstrMessage;                  

    throw errInfo;
}

void ScriptErrorHandler::inputFileError( )
{   
    throwError( 102, L"error reading input file" );
}
    
void ScriptErrorHandler::charConstError( )
{   
    throwError( 2021, L"character constant: closing ' expected" );
}
  
void ScriptErrorHandler::stringConstError( )
{   
    throwError( 2010, L"newline or end of file in string constant" );
}
  
void ScriptErrorHandler::hexCharError( )
{   
    throwError( 2020, L"bad char in hex notation for char const" );
}
  
void ScriptErrorHandler::numericValueError( )
{   
    throwError( 2000, L"illegal numeric value" );
}
               
//   eofError: Unexpected end of file in script file

void ScriptErrorHandler::eofError( )
{                    
   throwError( 900, L"unexpected end of file" );
}                                   
             
//   charError: Unknown character in script file

void ScriptErrorHandler::charError( )
{                    
   throwError( 930, L"unknown character" );
}
                 
//   ScriptTokenError: Unexpected token in script file

void ScriptErrorHandler::tokenError( )
{  
   throwError( 950, L"unexpected token" );
}
             
//   ScriptSymbolError: Unknown symbolic const in script file

void ScriptErrorHandler::symbolError( wstring const & wstrKey )
{  
   throwError( 970, L"unknown symbolic name: " + wstrKey );
}

//   semanticError

void ScriptErrorHandler::semanticError( std::wstring const & wstrText )
{  
   throwError( 1000, L"semantic error: "  + wstrText );
}

//   typeError: Unexpected type of symbolic const in script file

void ScriptErrorHandler::typeError( )
{  
   throwError( 960, L"bad type of symbolic const" );
}
            
//   numericError: Bad numeric value in script file

void ScriptErrorHandler::numericError( )
{                    
   throwError( 980, L"number too big" );
}
             
//   negativeError: Unexpected negative value in script file

void ScriptErrorHandler::negativeError( )
{                    
   throwError( 990, L"negative value found" );
}
             
//   funcNameError: Unknown function name in script file

void ScriptErrorHandler::funcNameError( )
{                    
   throwError( 4000, L"unknown function name" );
}

void ScriptErrorHandler::ScrSetOutputStream( std::wostream * const out )
{
    m_pScriptTrace = out;
}

void ScriptErrorHandler::PrintMarkerLine( Scanner const & scanner )
{
    int const iStartPos = scanner.GetActStartPos( );
    int const iEndPos   = scanner.GetActEndPos( );
    int       iRun;

    for ( iRun = 0; iRun < iStartPos; iRun++ ) 
        * m_pScriptTrace << L' ' ;                          // leading blanks 

    for ( ; iRun < iEndPos; iRun++ ) 
        * m_pScriptTrace << L'^';                           // markers for faulty token

    * m_pScriptTrace << L' ' << endl;
}

void ScriptErrorHandler::handleScriptError
( 
    ScriptErrorInfo const & errInfo,
    Scanner         const & scanner
)
{
    * m_pScriptTrace << endl;

	wstring actPath( scanner.GetActPath( ) );
    * m_pScriptTrace << L"+++ error " << errInfo.m_sErrNr << L" in file " << actPath.c_str() << endl;

    int const iLineNr = scanner.GetActLineNr( );
    if ( iLineNr > 0 )
        * m_pScriptTrace << L"+++ line " << iLineNr << endl;

	wstring const wstrActLine = scanner.GetActLine( );
	if ( ! wstrActLine.empty( ) )
		* m_pScriptTrace << wstrActLine;

	PrintMarkerLine( scanner );

    if ( ! errInfo.m_wstrMessage.empty() )
        * m_pScriptTrace << L"+++ " << errInfo.m_wstrMessage << endl;

    if ( !scanner.GetExpectedToken().empty() )
        * m_pScriptTrace << L"+++ expected " << scanner.GetExpectedToken().c_str() << endl;

    * m_pScriptTrace << L"+++ error exit +++" << endl;

    (void)m_pScriptTrace->flush( );
}
