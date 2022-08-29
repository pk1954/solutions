// NobException.ixx
//
// NNetModel

module;

#include <iostream>
#include <exception>
#include <source_location>

export module NobException;

import Scanner;
import NobId;
import NobType;

using std::endl;
using std::wcout;
using std::wstring;
using std::exception;
using std::source_location;

export struct NobException : public exception
{
    NobException(NobId const id, wstring const msg)
      : m_id(id),
        m_wstrMessage(msg)
    {}
    NobId   m_id{ 0 };
    wstring m_wstrMessage{ L"" };
};

export struct NobTypeException : public exception
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

export struct NNetException : public exception
{
    NNetException
    (
        char const* const file,
        int          const line
    )
      : m_szFile(file),
        m_iLineNr(line)
    {}

    char const* const m_szFile;
    int                m_iLineNr;
};

export void NNetExceptionMessage(NNetException const& e)
{
    wcout << Scanner::COMMENT_SYMBOL << L"CheckModel failed" << endl;
    wcout << Scanner::COMMENT_SYMBOL << L"File: " << e.m_szFile << endl;
    wcout << Scanner::COMMENT_SYMBOL << L"Line: " << e.m_iLineNr << endl;
}
