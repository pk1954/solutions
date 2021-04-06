//                                                                        
//  File:     ERRHNDL.H                             
//                                                                    
//
// errhndl.h - interface for errhndl.c
//
//

#pragma once

#include <string>
#include <exception>
#include "scanner.h"

using std::wcout;
using std::wstring;
using std::wostream;
using std::exception;

class Scanner;

class ScriptErrorHandler
{
public:

    static void eofError( void );
    static void charError( void );
	static void tokenError( void );
	static void stringError( void );
	static void numericError( void );
    static void negativeError( void );
    static void symbolError( wstring const & );
    static void typeError( void );
    static void funcNameError( void );
    static void inputFileError( void );
    static void charConstError( void );
    static void stringConstError( void );
    static void hexCharError( void );
    static void numericValueError( void );

    struct ScriptException: public exception
    {
        ScriptException() {};
        ScriptException( short const sErrNr, wstring const msg )
          : m_sErrNr(sErrNr),
            m_wstrMessage( msg )
        {}
        short   m_sErrNr      { 0 };               // error number               
        wstring m_wstrMessage { L"undefined" };    // error message              
    };

    static void HandleScriptError  ( Scanner const &, ScriptException const & );
    static void PrintMarkerLine    ( Scanner const & );
    static void ScrSetOutputStream( wostream * const );

private:
    static void printErrorMsg( Scanner const &, wstring const  );

    inline static wostream * m_pScriptTrace { & wcout };
};
