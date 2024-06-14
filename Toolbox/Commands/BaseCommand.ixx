// BaseCommand.ixx
//
// Toolbox\Commands

module;

#include <memory>
#include <functional>
#include <vector>
#include <string>
#include <iostream>

export module Commands:BaseCommand;

import SaveCast;

using std::wcout;
using std::wostream;
using std::function;
using std::vector;
using std::unique_ptr;
using std::wstring;

export class BaseCommand
{
public:

    virtual ~BaseCommand() = default;

    virtual void UpdateUI() = 0;
    virtual void CallUI(bool const) = 0; // called by Animation

    virtual bool CombineCommands(BaseCommand const& src) { return false; };
    virtual bool IsAsyncCommand()                        { return false; };

    virtual void NextScriptCommand() const = 0;

    void TargetReached();

    virtual void Do();
    virtual void Undo();

protected:

    static bool      IsTraceOn()   { return m_bTrace; }
    static wostream& TraceStream() { return wcout; }

private:

    function<void(BaseCommand*)> m_targetReachedFunc { nullptr };

    inline static bool m_bTrace { true };
};
