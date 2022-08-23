// ScriptStack.ixx
//
// runtimeCPP

module;

#include <memory>
#include <stack>
#include "Script.h"

export module ScriptStack;

using std::unique_ptr;
using std::make_unique;
using std::stack;

export class ScriptStack
{
public:
    static Script* OpenScript()
    {
        m_stack.push(make_unique<Script>());
        return GetScript();
    }

    static Script* GetScript()
    {
        return m_stack.empty() ? nullptr : m_stack.top().get();
    }

    static bool IsScriptActive()
    {
        return !m_stack.empty();
    }

    static bool SingleStepMode()
    {
        return m_bSingleStepMode;
    }

    static void SetSingleStepMode(bool const bMode)
    {
        m_bSingleStepMode = bMode;
    }

    static void CloseScript()
    {
        GetScript()->ScrClose();
        m_stack.pop();
    }

private:
    inline static stack<unique_ptr<Script>> m_stack;
    inline static bool                      m_bSingleStepMode{ false };
};
