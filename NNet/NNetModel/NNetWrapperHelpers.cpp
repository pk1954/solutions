// NNetWrapperHelpers.cpp : wrapper functions for unit tests and scripting of application
//
// NNetModel

module NNetWrapperHelpers;

import std;
import SaveCast;
import Types;
import IoUtil;
import IoConstants;
import RunTime;
import NNetModel;

using std::unique_ptr;
using std::make_unique;
using std::wstring;

SignalId ScrReadSignalId(Script& script)
{
	script.ScrReadSpecial(OPEN_BRACKET);
	TrackNr  const trackNr(script.ScrReadInt());
	script.ScrReadSpecial(SEPARATOR);
	SignalNr const signalNr(script.ScrReadInt());
	script.ScrReadSpecial(CLOSE_BRACKET);
	return SignalId(trackNr, signalNr);
}

NobId ScrReadNobId(Script& script)
{
	return NobId(script.ScrReadShort());
}

SigGenId ScrReadSigGenId(Script& script)
{
	return SigGenId(script.ScrReadInt());
}

NobType ScrReadNobType(Script& script)
{
	unsigned int uiVal { script.ScrReadUint() };
	if (uiVal >= NobType::NR_OF_NOB_TYPES)
		throw ScriptErrorHandler::ScriptException(998, wstring(L"Wrong nob type") );
	return static_cast<NobType::Value>(uiVal);
}

unique_ptr<NobIdList> ScrReadNobIdList(Script& script)
{
	unique_ptr<NobIdList> upNobIds  { make_unique<NobIdList>() };
	script.ScrReadSpecial(CURLY_OPEN_BRACKET);
	int const iNrOfElements { script.ScrReadInt() };
	script.ScrReadSpecial(NR_SEPARATOR);
	for (int i = 0;;)
	{
		upNobIds->Push(ScrReadNobId(script));
		if (++i == iNrOfElements)
			break;
		script.ScrReadSpecial(ID_SEPARATOR);
	}
	script.ScrReadSpecial(CURLY_CLOSE_BRACKET);
	return upNobIds;
}

ParamType::Value ScrReadParamType(Script& script)
{
	unsigned long const ulParamType { script.ScrReadUlong() };
	if (ulParamType > static_cast<unsigned long>(ParamType::Value::tParameterLast))
		throw ScriptErrorHandler::ScriptException(999, wstring(L"unknown parameter type"));
	ParamType::Value param { static_cast<ParamType::Value>(ulParamType) };
	return param;
}

mV ScrReadVoltage(Script& script)
{
	double const value { script.ScrReadFloat() };
	return mV(Cast2Float(value));
}

SigGenStaticData ScrReadSigGenStaticData(Script& script)
{
	SigGenStaticData sigGenData;
	sigGenData.SetFreq    (ScrReadBasePeak<fHertz>    (script));
	sigGenData.SetAmpl    (ScrReadBasePeak<mV>        (script));
	sigGenData.SetPeakTime(ScrRead        <fMicroSecs>(script));
	return sigGenData;
}
