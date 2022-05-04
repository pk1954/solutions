// NNetModel.cpp 
//
// NNetModel

#include "stdafx.h"
#include "MoreTypes.h"
#include "NobException.h"
#include "SimulationTime.h"
#include "NobType.h"
#include "Knot.h"
#include "Neuron.h"
#include "InputLine.h"
#include "OutputLine.h"
#include "NNetModel.h"

using namespace std::chrono;
using std::wcout;
using std::endl;

bool NNetModel::operator==(NNetModel const & rhs) const
{
	return
	(m_Nobs              == rhs.m_Nobs             ) &&
	(m_wstrModelFilePath == rhs.m_wstrModelFilePath) &&
	(m_monitorData       == rhs.m_monitorData      ) &&
	(m_param             == rhs.m_param            );
}

void NNetModel::CheckModel() const
{
#ifdef _DEBUG
	try
	{
		m_Nobs.CheckNobList();
	}
	catch (NNetException const& e)
	{
		NNetExceptionMessage(e);
		wcout << Scanner::COMMENT_SYMBOL << L"Model dump:" << endl;
		m_Nobs.Dump();	
	}
#endif
}

void NNetModel::CheckId(NobId const id) const 
{	
#ifdef _DEBUG
	if (IsUndefined(id) || ! m_Nobs.IsValidNobId(id))
		throw NobException(id, L"");
#endif
}

Nob const * NNetModel::GetConstNob(NobId const id) const 
{	
	CheckId(id);
	return m_Nobs.GetAt(id);
}

Nob * NNetModel::GetNob(NobId const id)
{	
	CheckId(id);
	return m_Nobs.GetAt(id);
}

void NNetModel::Reconnect(NobId const id)
{
	if (Nob * pNod { GetNob(id) })
		pNod->Reconnect();
}

void NNetModel::RecalcAllNobs() const
{ 
	m_Nobs.Apply2AllC
	(
		[](Nob & nob) 
		{ 
			//nob.Recalc(); 
		}
	);
} 

void NNetModel::ClearDynamicData()
{ 
	SimulationTime::Set();
	m_Nobs.Apply2AllC([](Nob & nob) { nob.ClearDynamicData(); });
	GetMonitorData().Apply2AllSignals([this](Signal & s) { s.Reset(m_Nobs); });
	m_sigGenList.Apply2All([](auto * p){ p->ClearDynamicData(); });
}

bool NNetModel::GetDescriptionLine(int const iLine, wstring & wstrLine) const
{
	return m_description.GetDescriptionLine(iLine, wstrLine);
}

void NNetModel::RecalcFilters()
{
	MonitorData & data = GetMonitorData();
	GetMonitorData().Apply2AllSignals([this](Signal const & s) { s.RecalcFilter(m_param); });
}

float NNetModel::SetParam
(
	ParamType::Value const param, 
	float            const fNewValue 
)
{
	float fOldValue { m_param.GetParameterValue(param) };
	m_param.SetParameterValue(param, fNewValue);
	if (param == ParamType::Value::filterSize)
		;  // RecalcFilters();
	else
		RecalcAllNobs();
	return fOldValue;
}

bool NNetModel::Compute()
{
	bool bStop { false };
	SimulationTime::Tick(m_param.TimeResolution());
	m_sigGenList.Apply2All([this](SignalGenerator * p){ p->Prepare(m_param); });
	m_Nobs.Apply2AllC(      [](Nob &s) { s.Prepare(); });
	m_Nobs.Apply2AllC([&bStop](Nob &s) { if (s.CompStep()) bStop = true; });
	return bStop;
}

void NNetModel::ResetModel()
{
	m_wstrModelFilePath = L""; 
	m_Nobs.Clear();
	m_monitorData.Reset();
	m_description.ClearDescription();
	SimulationTime::Set();
}

void NNetModel::SelectSubtree(BaseKnot & baseKnot, bool const bOn)
{
	baseKnot.Select(bOn);
	baseKnot.Apply2AllOutPipes
	(
		[this, bOn](Pipe & pipe) 
		{ 
			pipe.Select(bOn); 
			if (pipe.GetEndKnotPtr()->IsKnot())
				SelectSubtree(*pipe.GetEndKnotPtr(), bOn); 
		} 
	);
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
