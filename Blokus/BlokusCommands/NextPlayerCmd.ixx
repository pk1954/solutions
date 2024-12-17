// NextPlayerCmd.ixx
//
// BlokusCommands

export module BlokusCommands:NextPlayerCmd;

import std;
import :BlokusCommand;

export class NextPlayerCmd : public BlokusCommand  
{
public:

    NextPlayerCmd()
    {}

	void Do() final 
	{
        m_pMWI->NextPlayer();
    }

	void Undo() final 
	{
        m_pMWI->PrevPlayer();
	}

    static void Register()
    {
        SymbolTable::ScrDefConst(NAME, &m_wrapper);
    }

    static void Push()
    {
        //if (IsTraceOn())
        //    TraceStream() << NAME << SPACE << endl;

        PushCommand(make_unique<NextPlayerCmd>());
    }

private:
    inline static const wstring NAME { L"NextPlayerCmd" };

    inline static struct myWrapper : public Wrapper
    {
        using Wrapper::Wrapper;
        void operator() (Script& script) const final
        {
        }
    } m_wrapper { NAME };
};