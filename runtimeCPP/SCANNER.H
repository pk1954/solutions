//                                                                         
//  File:     SCANNER.H                                                    
//                                                                         
//
// scanner.h - interface for scanner
//
//

#pragma once
          
#include <string>
#include <stdio.h>
#include "Input.h"
   
using std::wstring;
                                            
enum class tTOKEN    // Return value of NextToken
{ 
    End,                  // end of file reached     
    Name,                 // name found               
    Number,               // natural number           
    Float,                // floating point constant  
    String,               // string constant          
    Character,            // character constant found
    Special               // special character found  
}; 

class Scanner
{   
public:
    explicit Scanner( );
    void OpenInputFile( wstring const & );
    void CloseInputFile( );
    ~Scanner( );

   // Get next token from input file 

    tTOKEN NextToken( bool );
   
   // Get actual token, type according to NextToken return value 

    unsigned long   GetUlong    ( ) const { return m_ulValue;   };
    wchar_t         GetCharacter( ) const { return m_wchValue;  };
    double          GetFloat    ( ) const { return m_dValue;    };
    wstring const & GetString   ( ) const { return m_wstrToken; }; 

   // Helper routines for error handlers 

    wstring const & GetActLine      ( ) const { return m_inbuf.GetActLine    ( ); };
    int             GetActLineNr    ( ) const { return m_inbuf.GetActLineNr  ( ); };
    int             GetActStartPos  ( ) const { return m_inbuf.GetActStartPos( ); };
    int             GetActEndPos    ( ) const { return m_inbuf.GetActEndPos  ( ); };
    wstring const & GetExpectedToken( ) const { return m_wstrExpected; }
    wstring const & GetActPath      ( ) const { return m_wstrPath;     };

    void SetExpectedToken( wstring const & wstrExp ) { m_wstrExpected = wstrExp; }

	wchar_t ReadOneOf ( wstring const & );

private:
    wchar_t SkipSpace    ( );
    tTOKEN  ScanName     ( wchar_t );
    tTOKEN  ScanUnsigned ( );
    tTOKEN  ScanString   ( );
    tTOKEN  ScanCharacter( );

    wstring       m_wstrPath;       // name of input script 
    wstring       m_wstrToken;
    InputBuffer   m_inbuf;          
    unsigned long m_ulValue;
    wchar_t       m_wchValue;
    double        m_dValue;
    wstring       m_wstrExpected;
};
