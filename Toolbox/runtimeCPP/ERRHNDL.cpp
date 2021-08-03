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

using std::wstring;
using std::wostream;
using std::endl;

void ScriptErrorHandler::inputFileError()
{   
    throw ScriptException(102, L"error reading input file");
}
    
void ScriptErrorHandler::charConstError()
{   
    throw ScriptException(2021, L"character constant: closing ' expected");
}
  
void ScriptErrorHandler::stringConstError()
{   
    throw ScriptException(2010, L"newline or end of file in string constant");
}
  
void ScriptErrorHandler::hexCharError()
{   
    throw ScriptException(2020, L"bad char in hex notation for char const");
}
  
void ScriptErrorHandler::numericValueError()
{   
    throw ScriptException(2000, L"illegal numeric value");
}
               
//   eofError: Unexpected end of file in script file

void ScriptErrorHandler::eofError()
{                    
   throw ScriptException(900, L"unexpected end of file");
}                                   
             
//   charError: Unknown character in script file

void ScriptErrorHandler::charError()
{                    
	throw ScriptException(930, L"unknown character");
}

void ScriptErrorHandler::stringError()
{                    
	throw ScriptException(940, L"unexpected string");
}

//   ScriptTokenError: Unexpected token in script file

void ScriptErrorHandler::tokenError()
{  
   throw ScriptException(950, L"unexpected token");
}
             
//   ScriptSymbolError: Unknown symbolic const in script file

void ScriptErrorHandler::symbolError(wstring const & wstrKey)
{  
   throw ScriptException(970, L"unknown symbolic name: " + wstrKey);
}

//   typeError: Unexpected type of symbolic const in script file

void ScriptErrorHandler::typeError()
{  
   throw ScriptException(960, L"bad type of symbolic const");
}
            
//   numericError: Bad numeric value in script file

void ScriptErrorHandler::numericError()
{                    
   throw ScriptException(980, L"number too big");
}
             
//   negativeError: Unexpected negative value in script file

void ScriptErrorHandler::negativeError()
{                    
   throw ScriptException(990, L"negative value found");
}
             
//   funcNameError: Unknown function name in script file

void ScriptErrorHandler::funcNameError()
{                    
   throw ScriptException(4000, L"unknown function name");
}

void ScriptErrorHandler::ScrSetOutputStream(std::wostream * const out)
{
    m_pScriptTrace = out;
}

void ScriptErrorHandler::PrintMarkerLine(Scanner const & scanner)
{
    int const iStartPos = scanner.GetActStartPos();
    int const iEndPos   = scanner.GetActEndPos();
    int       iRun;

    for (iRun = 0; iRun < iStartPos; iRun++) 
        * m_pScriptTrace << L' ' ;                          // leading blanks 

    for (; iRun < iEndPos; iRun++) 
        * m_pScriptTrace << L'^';                           // markers for faulty token

    * m_pScriptTrace << L' ' << endl;
}

void ScriptErrorHandler::HandleScriptError
(
    Scanner         const & scanner,
    ScriptException const & errInfo
)
{
    * m_pScriptTrace << endl << L"+++ error " << errInfo.m_sErrNr;
    printErrorMsg(scanner, errInfo.m_wstrMessage);
}

void ScriptErrorHandler::printErrorMsg
(
    Scanner const & scanner,
    wstring const   m_wstrMessage 
)
{
    * m_pScriptTrace << L" in file " << scanner.GetActPath().c_str() << endl;

    int const iLineNr = scanner.GetActLineNr();
    if (iLineNr > 0)
        * m_pScriptTrace << L"+++ line " << iLineNr << endl;

    wstring const wstrActLine = scanner.GetActLine();
    if (! wstrActLine.empty())
        * m_pScriptTrace << wstrActLine;

    PrintMarkerLine(scanner);

    if (! m_wstrMessage.empty())
        * m_pScriptTrace << L"+++ " << m_wstrMessage << endl;

    if (!scanner.GetExpectedToken().empty())
        * m_pScriptTrace << L"+++ expected \"" << scanner.GetExpectedToken().c_str() << L"\""<< endl;

    * m_pScriptTrace << L"+++ error exit" << endl;

    (void)m_pScriptTrace->flush();
}
