// Errhndl.cpp
//
// Toolbox\runtimeCPP

module ErrHndl;

import std;
import Scanner;

using std::wstring;
using std::wostream;
using std::to_wstring;
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

    *m_pScriptTrace << L" +++ ";

    for (iRun = 0; iRun < iStartPos; iRun++)
        * m_pScriptTrace << L' ' ;                          // leading blanks 

    for (; iRun < iEndPos; iRun++) 
        * m_pScriptTrace << L'^';                           // markers for faulty token

    * m_pScriptTrace << L' ' << endl;
}

void ScriptErrorHandler::Print(wstring const& text)
{
    *m_pScriptTrace << L" +++ " << text;
}

void ScriptErrorHandler::PrintNL(wstring const& text)
{
    Print(text);
    *m_pScriptTrace << endl;
}

void ScriptErrorHandler::PrintErrorInfo
(
    Scanner         const & scanner,
    ScriptException const & errInfo
)
{
    PrintNL();
    PrintNL(L"error " + to_wstring(errInfo.m_sErrNr));
    PrintNL(L"in file " + scanner.GetActPath());

    int const iLineNr { scanner.GetActLineNr() };
    if (iLineNr > 0)
        PrintNL(L"line " + to_wstring(iLineNr));

    if (!scanner.GetActLine().empty())
    {
        Print(scanner.GetActLine());
        PrintMarkerLine(scanner);
    }

    if (!errInfo.m_wstrMessage.empty())
        PrintNL(errInfo.m_wstrMessage);

    if (!scanner.GetExpectedToken().empty())
        PrintNL(L"expected \"" + scanner.GetExpectedToken() + L"\"");

    (void)m_pScriptTrace->flush();
}
