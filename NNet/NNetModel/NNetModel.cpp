// NNetModel.cpp 
//
// NNetModel

#include "stdafx.h"
#include "MoreTypes.h"
#include "NobException.h"
#include "NobType.h"
#include "Knot.h"
#include "Neuron.h"
#include "InputNeuron.h"
#include "OutputNeuron.h"
#include "NNetModel.h"

using namespace std::chrono;
using std::wcout;
using std::endl;

bool NNetModel::operator==(NNetModel const & rhs) const
{
	return
	(m_Nobs              == rhs.m_Nobs             ) &&
	(m_timeStamp         == rhs.m_timeStamp        ) &&
	(m_wstrModelFilePath == rhs.m_wstrModelFilePath) &&
	(m_monitorData       == rhs.m_monitorData      ) &&
	(m_param             == rhs.m_param            );
}

void NNetModel::CheckModel() const
{
#ifdef _DEBUG
	m_Nobs.CheckNobList();
#endif
}

Nob const * NNetModel::GetConstNob(NobId const id) const 
{	
	if (IsUndefined(id) || ! m_Nobs.IsValidNobId(id))
		throw NobException(id, L"");
	return m_Nobs.GetAt(id);
}

void NNetModel::Reconnect(NobId const id) const
{
	if (Nob * pNod { m_Nobs.GetAt(id) })
		pNod->Reconnect();
}

void NNetModel::RecalcAllNobs() 
{ 
	m_Nobs.Apply2All([](Nob & nob) { nob.Recalc(); });
} 

void NNetModel::ClearDynamicData() 
{ 
	m_Nobs.Apply2All([](Nob & nob) { nob.ClearDynamicData(); }); 
}

bool NNetModel::GetDescriptionLine(int const iLine, wstring & wstrLine) const
{
	return m_description.GetDescriptionLine(iLine, wstrLine);
}

float NNetModel::SetParam
(
	ParamType::Value const param, 
	float            const fNewValue 
)
{
	float fOldValue { m_param.GetParameterValue(param) };
	m_param.SetParameterValue(param, fNewValue);
	RecalcAllNobs();
	return fOldValue;
}

bool NNetModel::Compute()
{
	bool bStop {false};
	m_timeStamp += m_param.TimeResolution();
	m_Nobs.Apply2All(      [](Nob &s) { s.Prepare(); });
	m_Nobs.Apply2All([&bStop](Nob &s) { if (s.CompStep()) bStop = true; });
	return bStop;
}

void NNetModel::ResetModel()
{
	m_wstrModelFilePath = L""; 
	m_Nobs.Clear();
	m_monitorData.Reset();
	m_description.ClearDescription();
	SetSimulationTime();
}

void NNetModel::SelectSubtree(BaseKnot * const pBaseKnot, bool const bOn)
{
	if (pBaseKnot)
	{
		pBaseKnot->Select(bOn);
		pBaseKnot->Apply2AllOutPipes
		(
			[&](Pipe & pipe) 
			{ 
				pipe.Select(bOn); 
				if (pipe.GetEndKnotPtr()->IsKnot())
					SelectSubtree(pipe.GetEndKnotPtr(), bOn); 
			} 
		);
	}
}

NobId NNetModel::FindNobAt
(
	MicroMeterPnt const & umPoint, 
	NobCrit       const & crit 
) const
{	
	NobId idRes { NO_NOB };

	idRes = m_Nobs.FindNobAt(umPoint, [&](Nob const & s) { return s.IsIoConnector() && crit(s); });
	if (IsDefined(idRes))
		return idRes;

	idRes = m_Nobs.FindNobAt(umPoint, [&](Nob const & s) { return s.IsAnyNeuron() && (!s.HasParentNob()) && crit(s); });
	if (IsDefined(idRes))
		return idRes;

	idRes = m_Nobs.FindNobAt(umPoint, [&](Nob const & s) { return s.IsKnot() && crit(s); }); 	
	if (IsDefined(idRes))
		return idRes;

	idRes = m_Nobs.FindNobAt(umPoint, [&](Nob const & s) { return s.IsPipe() && crit(s); });
	if (IsDefined(idRes))
		return idRes;

	return NO_NOB;
}

void NNetModel::DumpModel
(
	char const * const file,
	int  const         line 

) const
{
	wcout << Scanner::COMMENT_SYMBOL << L"--- Dump start (" << file << L" line " << line << L")" << endl;
	m_Nobs.Dump();
	wcout << Scanner::COMMENT_SYMBOL << L"--- Dump end ---" << endl;
}
