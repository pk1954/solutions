//                                                                        
// ErrHndl.ixx                       
//                                                                    
// interface for runtime lib error handler

export module RunTime:ErrHndl;

import std;
import :Scanner;

using std::wcout;
using std::wstring;
using std::wostream;
using std::exception;

export class ScriptErrorHandler
{
public:

    static void eofError(void);
    static void charError(void);
    static void tokenError(void);
    static void stringError(void);
    static void numericError(void);
    static void negativeError(void);
    static void symbolError(wstring const &);
    static void typeError(void);
    static void funcNameError(void);
    static void inputFileError(void);
    static void charConstError(void);
    static void stringConstError(void);
    static void hexCharError(void);
    static void numericValueError(void);

    struct ScriptException: public exception
    {
        ScriptException() = default;
        ScriptException(short const sErrNr, wstring const & msg)
            : m_sErrNr(sErrNr),
            m_wstrMessage(msg)
        {}
        short   m_sErrNr      { 0 };               // error number               
        wstring m_wstrMessage { L"undefined" };    // error message              
    };

    static void PrintErrorInfo(Scanner const &, ScriptException const &);
    static void PrintMarkerLine(Scanner const &);
    static void ScrSetOutputStream(wostream * const);
    static void Print(wstring const& = L"");
    static void PrintNL(wstring const& = L"");

private:
    inline static wostream * m_pScriptTrace { & wcout };
};
