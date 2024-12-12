// ResetMatchCmd.ixx
//
// BlokusCommands

export module BlokusCommands:ResetMatchCmd;

import std;
import Types;
import Commands;
import WinCommand;
import WinManager;
import Win32_Util_Resource;
import BlokusCore;
import Resource;
import :BlokusCommand;

using std::wstring;
using std::make_unique;

export class ResetMatchCmd : public BlokusCommand 
{
public:

    ResetMatchCmd()
    {}

	void Do() final {}

	void Undo() final {}

    static void Register()
    {
        SymbolTable::ScrDefConst(NAME, &m_wrapper);
    }

    static void Push()
    {
        //if (IsTraceOn())
        //    TraceStream() << NAME << umAnimated << SPACE << bOn << endl;

   	    m_pMWI->Reset();
		ClearStack();
    }

private:
    inline static const wstring NAME { L"ResetMatchCmd" };

    inline static struct myWrapper : public Wrapper
    {
        using Wrapper::Wrapper;
        void operator() (Script& script) const final
        {
        }
    } m_wrapper { NAME };
};