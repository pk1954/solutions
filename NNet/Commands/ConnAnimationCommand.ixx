// ConnAnimationCommand.ixx
//
// Commands

module;

#include <vector>
#include <memory>
#include <iostream>

export module ConnAnimationCommand;

import Types;
import Symtab;
import Script;
import NNetWrapperHelpers;
import NNetCommand;
import NNetModel;

using std::vector;
using std::endl;
using std::wstring;
using std::unique_ptr;
using std::make_unique;

export class ConnAnimationCommand : public NNetCommand
{
public:
    ConnAnimationCommand(NobId const, NobId const);

    bool IsAsyncCommand() final { return true; };

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, new Wrapper);
	}

	static void Push(NobId const nobId1, NobId const nobId2)
	{
		if (IsTraceOn())
			TraceStream() << NAME << nobId1 << nobId2 << endl;
		m_pStack->PushCommand(make_unique<ConnAnimationCommand>(nobId1, nobId2));
	}

private:

    inline static const wstring NAME { L"ConnAnimation" };

    class Wrapper : public ScriptFunctor
    {
    public:
        void operator() (Script& script) const final
        {
            NobId const id1 { ScrReadNobId(script) };
            NobId const id2 { ScrReadNobId(script) };
            ConnAnimationCommand::Push(id1, id2);
        }
    };

    vector<IoLine*> m_nobsAnimated{};

    void add2nobsAnimated(NobId const);
    void sortNobsAnimated(MicroMeterLine const&);
};