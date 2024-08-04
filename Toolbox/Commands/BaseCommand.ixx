// BaseCommand.ixx
//
// Toolbox\Commands

export module Commands:BaseCommand;

import std;
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

    virtual bool CombineCommands(BaseCommand const& src) { return false; };
    virtual bool IsAsyncCommand()                        { return false; };

    virtual void NextScriptCommand() const = 0;

    virtual void Do  () { UpdateUI(); }
    virtual void Undo() { UpdateUI(); }

protected:

    static bool      IsTraceOn()   { return m_bTrace; }
    static wostream& TraceStream() { return wcout; }

private:

    inline static bool m_bTrace { true };
};
