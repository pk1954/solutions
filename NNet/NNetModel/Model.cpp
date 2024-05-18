// NNetModel.cpp 
//
// Model

module;

#include <cassert>
#include <memory>
#include <string>
#include <iostream>
#include <iomanip>
#include <optional>

module NNetModel:Model;

import HiResTimer;

import Observable;
import Types;
import Signals;
import IoConstants;
import Raster;
import SimulationTime;
import :NobException;
import :NobType;
import :ParamType;
import :NobId;
import :Nob;
import :Knot;
import :PosNob;
import :Pipe;
import :Fork;
import :Synapse;
import :InputConnector;
import :OutputConnector;
import :SignalGenerator;
import :UPSigGenList;

import FixedPipeline;

using std::unique_ptr;
using std::make_unique;
using std::wcout;
using std::endl;
using std::wstring;
using std::setw;
using std::right;
using std::left;
using std::optional;

void Model::Initialize
(
	Observable * const pLockModelObservable,
	Observable * const pStaticModelObservable
)
{
	m_pStaticModelObservable = pStaticModelObservable;
}

Model::Model()
{
	m_upNobs       = make_unique<UPNobList>();
	m_upSigGenList = make_unique<UPSigGenList>();
	m_upRaster     = make_unique<Raster>();
	m_upParam      = make_unique<NNetParameters>(&m_signalParams, m_upRaster.get());
	if (m_pStaticModelObservable)
		m_pStaticModelObservable->RegisterObserver(*this);
	RejectImage(); 
}

Model::~Model() 
{
	if (m_pStaticModelObservable)
		m_pStaticModelObservable->UnregisterObserver(*this); 
}

unsigned int Model::printNobType
(
	unsigned int   siz,
	NobType::Value nobType
) const 
{ 
	unsigned int nr  { m_upNobs->GetCounter(nobType) };
	unsigned int sum { siz * nr };
	wcout << setw(16)   <<            left  << nobType
	      << L" size "  << setw(5) << right << siz
		  << L" count " << setw(5) << right << nr
		  << L" sum "   << setw(7) << right << sum
		  << L" bytes"  << endl;
	return sum;
}

void Model::PrintModelSize() const 
{ 
	wcout << L"sizeof(bool) "                    << sizeof(bool)                    << L" bytes" << endl;
	wcout << L"sizeof(Nob) "                     << sizeof(Nob)                     << L" bytes" << endl;
	wcout << L"sizeof(mV) "                      << sizeof(mV)                      << L" bytes" << endl;
	wcout << L"sizeof(NobType) "                 << sizeof(NobType)                 << L" bytes" << endl;
	wcout << L"sizeof(NobId) "                   << sizeof(NobId)                   << L" bytes" << endl;
	wcout << L"sizeof(unique_ptr<MicroSensor>) " << sizeof(unique_ptr<MicroSensor>) << L" bytes" << endl;
	wcout << L"sizeof(FixedPipeline<mV>) "       << sizeof(FixedPipeline<mV>)       << L" bytes" << endl;
	unsigned int sum { 0 };
	sum += printNobType(Fork           ::Size(), NobType::Value::fork);
	sum += printNobType(InputLine      ::Size(), NobType::Value::inputLine);
	sum += printNobType(OutputLine     ::Size(), NobType::Value::outputLine);
	sum += printNobType(Knot           ::Size(), NobType::Value::knot);
	sum += printNobType(InputConnector ::Size(), NobType::Value::inputConnector);

	unsigned int uiInputConnLists { 0 };
	m_upNobs->Apply2AllC<InputConnector>([&uiInputConnLists](InputConnector const& c) { uiInputConnLists += Cast2UnsignedInt(c.Size() * (sizeof(MicroMeterPnt) + sizeof(IoLine *))); });
	wcout << setw(44) << left << L"   input conn lists " << setw(7) << right << uiInputConnLists << L" bytes" << endl;
	sum += uiInputConnLists;

	sum += printNobType(OutputConnector::Size(), NobType::Value::outputConnector);

	unsigned int uiOutputConnLists { 0 };
	m_upNobs->Apply2AllC<OutputConnector>([&uiOutputConnLists](OutputConnector const& c) { uiOutputConnLists += Cast2UnsignedInt(c.Size() * (sizeof(MicroMeterPnt) + sizeof(IoLine *))); });
	wcout << setw(44) << left << L"   output conn lists " << setw(7) << right << uiOutputConnLists << L" bytes" << endl;
	sum += uiOutputConnLists;

	sum += printNobType(Neuron         ::Size(), NobType::Value::neuron);

	unsigned int uiPipelists { 0 };
	m_upNobs->Apply2AllC<Neuron>([&uiPipelists](Neuron const& neuron) { uiPipelists += Cast2UnsignedInt(neuron.GetNrOfInConns() * sizeof(Pipe*)); });
	wcout << setw(44) << left << L"   pipe lists " << setw(7) << right << uiPipelists << L" bytes" << endl;
	sum += uiPipelists;
	
	sum += printNobType(Synapse        ::Size(), NobType::Value::synapse);

	unsigned int uiBlocklists { 0 };
	m_upNobs->Apply2AllC<Synapse>([&uiBlocklists](Synapse const& synapse) { uiBlocklists += Cast2UnsignedInt(synapse.GetBlockList().size() * sizeof(fMicroSecs)); });
	wcout << setw(44) << left << L"   blocklists " << setw(7) << right << uiBlocklists << L" bytes" << endl;
	sum += uiBlocklists;
	
	sum += printNobType(Pipe           ::Size(), NobType::Value::pipe);

	unsigned int uiSegments { 0 };
	m_upNobs->Apply2AllC<Pipe>([&uiSegments](Pipe const& pipe) { uiSegments += Cast2UnsignedInt(pipe.GetNrOfSegments() * sizeof(mV)); });
	wcout << setw(44) << left << L"   segments "  << setw(7) << right << uiSegments << L" bytes" << endl;
	sum += uiSegments;
	
	wcout << setw(44) << left  << L"Model "     << setw(7) << right << sizeof(Model) << L" bytes" << endl;
	wcout << setw(44) << left  << L"ModelSize " << setw(7) << right << sum + sizeof(Model) << L" bytes" << endl;
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
	if (! m_upNobs->IsValidNobId(id))
		throw NobException(id, L"");
}

NobType Model::GetNobType(NobId const id) const
{
	auto p { GetNobConstPtr<Nob const *>(id) };
	return p ? p->GetNobType() : NobType::Value::undefined; 
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

bool Model::isConnectedToPipe(NobId const idNob, NobId const idPipe) const
{
	if (idNob == GetStartKnotId(idPipe))
		return true;
	if (idNob == GetEndKnotId(idPipe))
		return true;
	return false;
}

bool Model::isConnectedTo(NobId const idSrc, NobId const idDst) const
{
	if (GetNobType(idSrc).IsPipeType())
		return isConnectedToPipe(idDst, idSrc);
	if (GetNobType(idDst).IsPipeType())
		return isConnectedToPipe(idSrc, idDst);
	else
		return false;
}

bool Model::IsConnectionCandidate // IsConnectionCandidate: Sort out obvious non-candidates
(
	NobId const idSrc, 
	NobId const idDst
) const
{
	if (idSrc == idDst)
		return false; 
	if (isConnectedTo(idSrc, idDst)) // if already connected we cannot connect again
		return false;
	return true;
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
	m_monitorData.ClearDynamicData();
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

HiResTimer t1, t2, tC;

bool Model::Compute()
{
	//tC.BeforeAction();
	bool bStop { false };
	SimulationTime::Tick(m_upParam->TimeResolution());
	m_upSigGenList->Apply2All([this](SignalGenerator * p) { p->PrepareSigGen(*m_upParam.get()); });

	//t1.BeforeAction();
	m_upNobs->Apply2AllC([]      (Nob &s) { s.CollectInput(); });
	//t1.AfterAction();

	//t2.BeforeAction();
	m_upNobs->Apply2AllC([&bStop](Nob &s) { if (s.CompStep()) bStop = true; });
	//t2.AfterAction();

	//tC.AfterAction();

	//wcout << L"t1 = " << t1.Average2wstring() << endl;
	//wcout << L"t2 = " << t2.Average2wstring() << endl;
	//wcout << L"tC = " << tC.Average2wstring() << endl;

	return bStop;
}

void Model::ResetModel()
{
	m_monitorData.ResetMonitorData();  //caution: order of Reset/Clear commands matters!
	m_upNobs->Clear();
	m_upSigGenList->Clear();
	m_sensorList.Clear();
	m_description.ClearDescription();
	RejectImage();
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
	m_upRaster->SetScanArea(rect);
}

void Model::CreateImage()
{ 
	m_upRawImage = make_unique<RawImage>(m_upRaster->Size());
	if (m_pLockModelObservable)
		m_pLockModelObservable->NotifyAll();
}

void Model::ReplaceImage(unique_ptr<RawImage> up) 
{ 
	m_upRawImage = move(up); 
}

void Model::RejectImage()
{ 
	m_upRawImage.release(); 
	if (m_pLockModelObservable)
		m_pLockModelObservable->NotifyAll();
}

optional<CardPoint> Model::SelectScanAreaHandle
(
	DrawContext   const& context,
	MicroMeterPnt const& umCrsrPos
) const
{
	return m_scanMatrix.SelectScanAreaHandle
	(
		context, *m_upRaster.get(), umCrsrPos
	);
}

void Model::PrepareScanMatrix() { m_scanMatrix.Prepare(*m_upRaster.get(), *m_upNobs.get()); }
