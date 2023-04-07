// SetSigGenStaticDataCmd.ixx
//
// Commands

module;

#include <iostream>

export module SetSigGenStaticDataCmd;

import Symtab;
import Types;
import Commands;
import NNetCommand;
import NNetModel;

export class SetSigGenStaticDataCmd : public NNetCommand
{
public:
	SetSigGenStaticDataCmd
	(
		SigGenId         const   id,
		SigGenStaticData const & data
	)
	  : m_sigGen(*m_pNMWI->GetSigGen(id)),
		m_dataOld(m_sigGen.GetStaticData()),
		m_dataNew(data)
	{ }

	void Do() final
	{ 
		m_sigGen.SetStaticData(m_dataNew);
	}

	void Undo() final
	{ 
		m_sigGen.SetStaticData(m_dataOld);
	}

	bool CombineCommands(Command const & src) final
	{ 
		SetSigGenStaticDataCmd const & srcCmd     { static_cast<SetSigGenStaticDataCmd const &>(src) };
		SignalGenerator        const * pSigGenSrc { &srcCmd.m_sigGen };
		if (&m_sigGen != pSigGenSrc)
			return false;
		m_dataNew = pSigGenSrc->GetStaticData();
		m_sigGen.SetStaticData(m_dataNew); 
		return true; 
	};

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, new Wrapper);
	}

	static void Push(SigGenId const id, SigGenStaticData const& data)
	{
		if (IsTraceOn())
			TraceStream() << NAME << id.GetValue() << /* data << */ endl;  //TODO: data causes compiler error
		m_pStack->PushCommand(make_unique<SetSigGenStaticDataCmd>(id, data));
	}

private:

	inline static const wstring NAME { L"SetSigGenStaticData" };

	class Wrapper : public ScriptFunctor
	{
	public:
		void operator() (Script& script) const final
		{
			SigGenId          const id   { ScrReadSigGenId(script) };
			SignalGenerator       & dst  { *m_pNMWI->GetSigGen(id) };
			SigGenStaticData const& data { ScrReadSigGenStaticData(script) };
			SetSigGenStaticDataCmd::Push(id, data);
		}
	};

	SignalGenerator      & m_sigGen;
	SigGenStaticData       m_dataNew;
	SigGenStaticData const m_dataOld;
};
