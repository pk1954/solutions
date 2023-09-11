// NNetModel.cpp 
//
// Model

module;

#include <memory>
#include <string>
#include <iostream>
#include <optional>

module NNetModel:Model;

import Types;
import Signals;
import IoConstants;
import Raster;
import :NobException;
import :NobType;
import :ParamType;
import :NobId;
import :Nob;
import :PosNob;
import :Pipe;
import :Synapse;
import :SignalGenerator;
import :UPSigGenList;

using std::make_unique;
using std::wcout;
using std::endl;
using std::wstring;

Model::Model()
{
	m_upNobs       = make_unique<UPNobList>();
	m_upSigGenList = make_unique<UPSigGenList>();
	m_upParam      = make_unique<NNetParameters>(&m_signalParams);
	m_upRaster     = make_unique<Raster>
 					 ( 
						100._MicroMeter,
						MicroMeterPnt(0._MicroMeter, 0._MicroMeter),
						SizeType<int>(50, 50)
					 );
	std::optional<RasterPoint> p = m_upRaster->FindRasterPos(MicroMeterPnt(3500._MicroMeter, 280._MicroMeter));
	int x = 42;
}

void Model::CheckModel() const
{
#ifdef _DEBUG
	try
	{
		m_upNobs->CheckNobList();
	}
	catch (NNetException const& e)
	{
		NNetExceptionMessage(e);
		wcout << COMMENT_SYMBOL << L"Model dump:" << endl;
		m_upNobs->Dump();	
	}
#endif
}

void Model::CheckId(NobId const id) const 
{	
#ifdef _DEBUG
	if (! m_upNobs->IsValidNobId(id))
		throw NobException(id, L"");
#endif
}

Nob const * Model::GetConstNob(NobId const id) const 
{	
	CheckId(id);
	return m_upNobs->GetAt(id);
}

Nob * Model::GetNob(NobId const id)
{	
	CheckId(id);
	return m_upNobs->GetAt(id);
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

void Model::ClearDynamicData()
{ 
	SimulationTime::Set();
	GetMonitorData().ClearDynamicData();
	m_upNobs->Apply2AllC([](Nob & nob) { nob.ClearDynamicData(); });
	m_upSigGenList->Apply2All([](auto * p){ p->ClearDynamicData(); });
}

bool Model::GetDescriptionLine(int const iLine, wstring & wstrLine) const
{
	return m_description.GetDescriptionLine(iLine, wstrLine);
}

void Model::SetParam
(
	ParamType::Value const param, 
	float            const fNewValue 
)
{
	m_upParam->SetParameterValue(param, fNewValue);
	if (param == ParamType::Value::synapseDelay)
		m_upNobs->Apply2All<Synapse>([](Synapse& s) { s.RecalcDelayBuffer(); });
	else 
		m_upNobs->Apply2AllC([](Nob& nob) { nob.PosChanged(); });
}

bool Model::Compute()
{
	bool bStop { false };
	SimulationTime::Tick(m_upParam->TimeResolution());
	m_upSigGenList->Apply2All([this](SignalGenerator * p) { p->Prepare(*m_upParam.get()); });
	m_upNobs->Apply2AllC([]      (Nob &s) { s.CollectInput(); });
	m_upNobs->Apply2AllC([&bStop](Nob &s) { if (s.CompStep()) bStop = true; });
	return bStop;
}

void Model::ResetModel()
{
	m_monitorData.Reset();  //caution: order of Reset/Clear commands matters!
	m_upNobs->Clear();
	m_upSigGenList->Clear();
	m_sensorList.Clear();
	m_description.ClearDescription();
	SimulationTime::Set();
	m_wstrModelFilePath = L"";
}

void Model::DumpModel
(
	char const * const file,
	int  const         line 

) const
{
	wcout << COMMENT_SYMBOL << L"--- Dump start (" << file << L" line " << line << L")" << endl;
	m_upNobs->Dump();
	wcout << COMMENT_SYMBOL << L"--- Dump end ---" << endl;
}

void Model::SetScanArea(MicroMeterRect const& rect)
{
	m_upRaster->SetRasterRect(rect);
}
