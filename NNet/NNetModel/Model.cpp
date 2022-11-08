// NNetModel.cpp 
//
// Model

module;

#include <string>
#include <iostream>

module NNetModel:Model;

import Types;
import SimulationTime;
import Scanner;
import :NobException;
import :NobType;
import :ParamType;
import :NobId;
import :Nob;
import :PosNob;
import :BaseKnot;
import :Pipe;
import :MonitorData;
import :SignalGenerator;
import :UPSigGenList;

using std::wcout;
using std::endl;
using std::wstring;

//bool Model::operator==(Model const & rhs) const
//{
//	return
//	(m_Nobs              == rhs.m_Nobs             ) &&
//	(m_wstrModelFilePath == rhs.m_wstrModelFilePath) &&
//	(m_monitorData       == rhs.m_monitorData      ) &&
//	(m_param             == rhs.m_param            );
//}
// TODO
void Model::CheckModel() const
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

void Model::CheckId(NobId const id) const 
{	
#ifdef _DEBUG
	if (IsUndefined(id) || ! m_Nobs.IsValidNobId(id))
		throw NobException(id, L"");
#endif
}

Nob const * Model::GetConstNob(NobId const id) const 
{	
	CheckId(id);
	return m_Nobs.GetAt(id);
}

Nob * Model::GetNob(NobId const id)
{	
	CheckId(id);
	return m_Nobs.GetAt(id);
}

PosNob const * Model::GetStartNobPtr(NobId const id) const 
{ 
	return static_cast<PosNob const*>(GetNobConstPtr<Pipe const *>(id)->GetStartNobPtr());
}

PosNob const * Model::GetEndNobPtr(NobId const id) const
{ 
	return static_cast<PosNob const*>(GetNobConstPtr<Pipe const *>(id)->GetEndNobPtr());
}

NobId Model::GetStartKnotId(NobId const idPipe) const 
{ 
	return GetStartNobPtr(idPipe)->GetId(); 
}

NobId Model::GetEndKnotId(NobId const idPipe) const 
{ 
	return GetEndNobPtr(idPipe)->GetId(); 
}

void Model::Reconnect(NobId const id)
{
	if (Nob * pNod { GetNob(id) })
		pNod->Reconnect();
}

void Model::RecalcAllNobs() const
{ 
	m_Nobs.Apply2AllC
	(
		[](Nob & nob) 
		{ 
			nob.Recalc(); 
		}
	);
} 

void Model::ClearDynamicData()
{ 
	SimulationTime::Set();
	GetMonitorData().ClearDynamicData();
	m_Nobs.Apply2AllC([](Nob & nob) { nob.ClearDynamicData(); });
	m_sigGenList.Apply2All([](auto * p){ p->ClearDynamicData(); });
}

bool Model::GetDescriptionLine(int const iLine, wstring & wstrLine) const
{
	return m_description.GetDescriptionLine(iLine, wstrLine);
}

float Model::SetParam
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

bool Model::Compute()
{
	bool bStop { false };
	SimulationTime::Tick(m_param.TimeResolution());
	m_sigGenList.Apply2All([this](SignalGenerator * p) { p->Prepare(m_param); });
	m_Nobs.Apply2AllC([]      (Nob &s) { s.CollectInput(); });
	m_Nobs.Apply2AllC([&bStop](Nob &s) { if (s.CompStep()) bStop = true; });
	return bStop;
}

void Model::ResetModel()
{
	m_wstrModelFilePath = L""; 
	m_Nobs.Clear();
	m_monitorData.Reset();
	m_description.ClearDescription();
	SimulationTime::Set();
}

void Model::SelectSubtree(PosNob & posNob, bool const bOn)
{
	posNob.Select(bOn);
	posNob.Apply2AllOutPipes
	(
		[this, bOn](Pipe & pipe) 
		{ 
			pipe.Select(bOn); 
			PosNob& posNob { *static_cast<PosNob*>(pipe.GetEndNobPtr()) };
			if (posNob.IsKnot() || posNob.IsFork() || posNob.IsSynapse())
				SelectSubtree(posNob, bOn);
		} 
	);
}

void Model::DumpModel
(
	char const * const file,
	int  const         line 

) const
{
	wcout << Scanner::COMMENT_SYMBOL << L"--- Dump start (" << file << L" line " << line << L")" << endl;
	m_Nobs.Dump();
	wcout << Scanner::COMMENT_SYMBOL << L"--- Dump end ---" << endl;
}
