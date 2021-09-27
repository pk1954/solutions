//
//                                                                         
//  File:     Input.h                                                     
//                                                                        
//

#pragma once
         
#include <string>
#include <iostream>
#include <fstream>

using std::wstring;
using std::wifstream;
using std::streampos;

class InputBuffer
{
public:

    explicit InputBuffer();
    void Open(wstring const &);
    void Close();
    virtual ~InputBuffer();

    void                  SetStartMarker();
    void                  UnreadLastChar();
    wchar_t       const   ReadNextChar  ();
    double        const   ReadFloat     ();
    unsigned long const   ReadNumber    ();

    wstring       const & GetActLine    () const { return m_wstrLine; };
    int           const   GetActLineNr  () const { return m_iLineNr; };

    int           const   GetActStartPos() const;
    int           const   GetActEndPos  () const;
    bool          const   IsFloat       () const;

    bool          const   IsActive      () const { return m_ifstream.is_open(); }
    streampos     const   GetFilePos    ()       { return m_ifstream.tellg(); };

private:
    wstring   m_wstrLine;       // buffer for script line
    int       m_iLineNr;        // actual line number  
    wchar_t * m_pwchStart;      // pointer to start of current token 
    wchar_t * m_pwchRead;       // pointer to next char in line 
    wifstream m_ifstream;
};

