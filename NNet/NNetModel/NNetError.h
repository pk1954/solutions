// NNetError.h 
//
// NNetModel

#pragma once

#include <exception>
#include "script.h"
#include "errhndl.h"
#include "NNetModelWriterInterface.h"
#include "UPNobList.h"

using std::to_wstring;
using std::endl;

class NNetErrorHandler : public NobErrorHandler
{
public:
    NNetErrorHandler(Script * const pScript, UPNobList const * const pList)
        : m_pScript(pScript),
          m_pList(pList)
    {}

    static void CheckNobId
    (
        Script          & script, 
        UPNobList const & list,
        NobId     const   id 
   )
    {        
        wstring strNobId { to_wstring(id.GetValue()) };
        if (IsUndefined(id))
        {
            script.SetExpectedToken(L"NobId != NO_NOB");
            throw ScriptErrorHandler::ScriptException(999, wstring(L"Invalid nob id: ") + strNobId);
        }
        else if (! list.IsValidNobId(id))
        {
            script.SetExpectedToken(L"id < " + to_wstring(list.Size()));
            throw ScriptErrorHandler::ScriptException(999, wstring(L"Invalid nob id: ") + strNobId);
        }
        else if (! list.IsNobDefined(id))
        {
            script.SetExpectedToken(L"Defined NobId");
            throw ScriptErrorHandler::ScriptException(999, wstring(L"Nob is not defined: ") + strNobId);
        }
    };

    virtual void operator()(NobId const id) 
    {
        CheckNobId(* m_pScript, * m_pList, id);
    }

private:
    Script          * const m_pScript;
    UPNobList const * const m_pList;
};
