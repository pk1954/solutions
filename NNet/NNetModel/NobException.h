// NobException.h 
//
// NNetModel

#pragma once

#include <exception>
#include "NobType.h"
#include "NobId.h"

using std::wstring;
using std::exception;

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
