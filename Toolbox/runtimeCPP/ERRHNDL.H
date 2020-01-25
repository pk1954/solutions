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

using std::wostream;

class Scanner;

class ScriptErrorHandler
{
public:

    static void throwError( short const, wstring const & );
    static void eofError( void );
    static void charError( void );
	static void tokenError( void );
	static void stringError( void );
	static void numericError( void );
    static void negativeError( void );
    static void symbolError( std::wstring const & );
	static void semanticError( std::wstring const & );
    static void typeError( void );
    static void funcNameError( void );
    static void inputFileError( void );
    static void charConstError( void );
    static void stringConstError( void );
    static void hexCharError( void );
    static void numericValueError( void );

    struct ScriptErrorInfo: public std::exception
    {
        short   m_sErrNr;         // error number               
        wstring m_wstrMessage;    // error message              
    };

    static void handleScriptError( ScriptErrorInfo const &, Scanner const & );
	static void PrintMarkerLine( Scanner const & );
    static void ScrSetOutputStream( wostream * const );

private:

    static wostream * m_pScriptTrace;
};
