// ScriptStack.h
//
// runtimeCPP

#pragma once

#include <stack>
#include "SCRIPT.h"

using std::unique_ptr;
using std::make_unique;
using std::stack;

class ScriptStack
{
public:
    static Script * const OpenScript()
    {
        m_stack.push(make_unique<Script>());
        return GetScript();
    }

    static Script* const GetScript()
    {
        return m_stack.empty() ? nullptr : m_stack.top().get();
    }

    static bool const IsScriptActive()
    {
        return ! m_stack.empty();
    }

    static void CloseScript()
    {
        GetScript()->ScrClose();
        m_stack.pop();
    }

private:
    inline static stack<unique_ptr<Script>> m_stack;
};
