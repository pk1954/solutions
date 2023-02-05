// SetParameterCommand.ixx
//
// Commands

module;

#include <compare>
#include <iostream>

export module SetParameterCommand;

import NNetCommand;
import NNetModel;

export class SetParameterCommand : public NNetCommand
{
public:
	SetParameterCommand
	(
		ParamType::Value const param,
		float            const fNewValue
	)
	  : m_param    (param),
		m_fOldValue(m_pNMWI->GetParams().GetParameterValue(param)),
		m_fNewValue(fNewValue)
	{ }

	void Do() final
	{
		m_pNMWI->SetParam(m_param, m_fNewValue);
	}

	void Undo() final
	{
		m_pNMWI->SetParam(m_param, m_fOldValue);
	}

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, new Wrapper);
	}

	static void Push
	(
		ParamType::Value const param,
		float            const fNewValue
	)
	{
		if (IsTraceOn())
			TraceStream() << NAME << L" " << ParamType::GetName(param) << L" " << fNewValue << endl;
		m_pStack->PushCommand(make_unique<SetParameterCommand>(param, fNewValue));
	}

private:

	inline static const wstring NAME { L"SetParameter" };

	class Wrapper : public ScriptFunctor
	{
	public:
		void operator() (Script& script) const final
		{
			ParamType::Value const param  { static_cast<ParamType::Value>(script.ScrReadUlong()) };
			float            const fValue { Cast2Float(script.ScrReadFloat()) };
			SetParameterCommand::Push(param, fValue);
		}
	};

	ParamType::Value const m_param;
	float            const m_fOldValue;
	float            const m_fNewValue;
};
