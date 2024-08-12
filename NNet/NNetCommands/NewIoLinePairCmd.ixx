// NewIoLinePairCmd.ixx
//
// NNetCommands

export module NNetCommands:NewIoLinePairCmd;

import std;
import IoUtil;
import :NNetCommand;

export class NewIoLinePairCmd : public NNetCommand
{
public:
	explicit NewIoLinePairCmd(MicroMeterPnt const & pos)
		: m_IoLinePair(pos)
	{}

	~NewIoLinePairCmd() final = default;

	void Do() final 
	{
		m_IoLinePair.Push(*m_pNMWI);
	}

	void Undo() final
	{ 
		m_IoLinePair.Pop(*m_pNMWI);
	}

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, &m_wrapper);
	}

	static void Push(MicroMeterPnt const& pos)
	{
		if (IsTraceOn())
			TraceStream() << NAME << pos << endl;
		PushCommand(make_unique<NewIoLinePairCmd>(pos));
	}

private:

	inline static const wstring NAME { L"NewIoLinePair" };

	inline static struct myWrapper : public Wrapper
	{
		using Wrapper::Wrapper;
		void operator() (Script& script) const final
		{
			NewIoLinePairCmd::Push(ScrReadMicroMeterPnt(script));
		}
	} m_wrapper { NAME };

	IoLinePair m_IoLinePair;
};
