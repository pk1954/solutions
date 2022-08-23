// NobException.h 
//
// NNetModel

#pragma once

#include <ostream>
#include <exception>
#include <source_location>
#include "NobType.h"
#include "NobId.h"
#include "Scanner.h"

using std::endl;
using std::wcout;
using std::wstring;
using std::exception;
using std::source_location;

struct NobException: public exception
{
    NobException(NobId const id, wstring const msg)
      : m_id(id),
        m_wstrMessage(msg)
    {}
    NobId   m_id          { 0 };
    wstring m_wstrMessage { L"" };    
};

#define ThrowNobTypeException(nob) { throw NobTypeException(nob.GetNobType(), __FILE__, __LINE__); }

struct NobTypeException: public exception
{
    NobTypeException
    (
        NobType      const type, 
        char const * const file,
        int          const line
    )
      : m_type(type),
        m_szFile(file),
        m_iLineNr(line)
    {}

    NobType            m_type;
    char const * const m_szFile;
    int                m_iLineNr;    
};

struct NNetException: public exception
{
    NNetException
    (
        char const * const file,
        int          const line
    )
      : m_szFile(file),
        m_iLineNr(line)
    {}

    char const * const m_szFile;
    int                m_iLineNr;    
};

#define ThrowNNetException() { throw NNetException(source_location::current().file_name(), source_location::current().line()); }

#define NNetAssert(CONDITION) \
if (!(CONDITION))  \
    ThrowNNetException();

inline void NNetExceptionMessage(NNetException const & e)
{
    wcout << Scanner::COMMENT_SYMBOL << L"CheckModel failed" << endl;
    wcout << Scanner::COMMENT_SYMBOL << L"File: " << e.m_szFile  << endl;
    wcout << Scanner::COMMENT_SYMBOL << L"Line: " << e.m_iLineNr << endl;
}
