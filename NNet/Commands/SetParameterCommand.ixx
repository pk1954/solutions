// SetParameterCommand.ixx
//
// Commands

module;

#include <iostream>

export module NNetCommands:SetParameterCommand;

import :NNetCommand;
import NNetModel;
import SaveCast;

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
		SymbolTable::ScrDefConst(NAME, &m_wrapper);
	}

	static void Push
	(
		ParamType::Value const param,
		float            const fNewValue
	)
	{
		//if (IsTraceOn())        // TODO: fix
		//	TraceStream() << NAME << SPACE << ParamType::GetName(param) << SPACE << fNewValue << endl;
		PushCommand(make_unique<SetParameterCommand>(param, fNewValue));
	}

private:

	inline static const wstring NAME { L"SetParameter" };

	inline static struct Wrapper : public WrapBase
	{
		using WrapBase::WrapBase;
		void operator() (Script& script) const final
		{
			ParamType::Value const param  { static_cast<ParamType::Value>(script.ScrReadUlong()) };
			float            const fValue { Cast2Float(script.ScrReadFloat()) };
			SetParameterCommand::Push(param, fValue);
		}
	} m_wrapper { NAME };

	ParamType::Value const m_param;
	float            const m_fOldValue;
	float            const m_fNewValue;
};
