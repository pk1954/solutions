// NNetModelReaderInterface.cpp 
//
// NNetModel

module;

#include <d2d1helper.h>
#include <cassert> 
#include <string> 
#include <compare> 

module NNetModel:NNetModelReaderInterface;

import Types;
import DrawContext;
import :tHighlight;
import :NobId;
import :Nob;
import :Synapse;
import :SignalGenerator;
import :InputLine;
import :IoConnector;
import :InputConnector;
import :PosNob;
import :Pipe;

using std::wstring;

bool NNetModelReaderInterface::IsSelected(NobId const id) const
{
	auto p { m_pModel->GetNobConstPtr<Nob const *>(id) };
	return p ? p->IsSelected() : false; 
}

NobType NNetModelReaderInterface::GetNobType(NobId const id) const
{
	auto p { m_pModel->GetNobConstPtr<Nob const *>(id) };
	return p ? p->GetNobType() : NobType::Value::undefined; 
}

Degrees NNetModelReaderInterface::GetDirection(NobId const id) const 
{ 
	auto p { m_pModel->GetNobConstPtr<IoConnector const *>(id) };
	return p ? Radian2Degrees(p->GetDir()) : Degrees::NULL_VAL(); 
}

size_t NNetModelReaderInterface::GetNrOfSegments(NobId const id) const
{
	auto p { m_pModel->GetNobConstPtr<Pipe const *>(id) };
	return p ? p->GetNrOfSegments() : 0; 
}

SignalGenerator const* NNetModelReaderInterface::GetSigGenC(NobId const id) const
{
	Nob const& nob { *m_pModel->GetConstNob(id) };
	if (nob.IsInputLine())
		return m_pModel->GetNobConstPtr<InputLine const*>(id)->GetSigGenC();
	else if (nob.IsInputConnector())
		return m_pModel->GetNobConstPtr<InputConnector const*>(id)->GetSigGenC();
	else
		return nullptr;
}

mV NNetModelReaderInterface::GetVoltage(NobId const id) const
{
	auto p { m_pModel->GetNobConstPtr<PosNob const *>(id) };
	return p ? p->GetVoltage() : mV::NULL_VAL(); 
}

mV NNetModelReaderInterface::GetVoltageAt(NobId const id, MicroMeterPnt const & umPoint) const
{
	auto p { m_pModel->GetNobConstPtr<Pipe const *>(id) };
	return p ? p->GetVoltageAt(umPoint) : mV::NULL_VAL(); 
}

size_t NNetModelReaderInterface::GetNrOfOutConns(NobId const id) const 
{ 
	auto p { m_pModel->GetNobConstPtr<PosNob const *>(id) };
	return p ? p->GetNrOfOutConns() : -1;
}

size_t NNetModelReaderInterface::GetNrOfInConns(NobId const id) const 
{ 
	auto p { m_pModel->GetNobConstPtr<PosNob const *>(id) };
	return p ? p->GetNrOfInConns() : -1;
}

bool NNetModelReaderInterface::GetDescriptionLine(int const iLine, wstring & wstrLine) const 
{
	return m_pModel->GetDescriptionLine(iLine, wstrLine);
};

// IsConnectionCandidate: Sort out obvious non-candidates

bool NNetModelReaderInterface::IsConnectionCandidate(NobId const idSrc, NobId const idDst) const
{
	if (idSrc == idDst)
		return false; 
	if (IsConnectedTo(idSrc, idDst)) // if already connected we cannot connect again
		return false;
	return true;
}

ConnectionType NNetModelReaderInterface::ConnectionResult(NobId const idSrc, NobId const idDst) const
{
	using enum ConnectionType;

	if (::IsUndefined(idDst))
		return ct_none;

	if (::IsUndefined(idSrc))
		return ct_none;

	if (!IsConnectionCandidate(idSrc, idDst))
		return ct_none;

	NobType const typeSrc { GetNobType(idSrc) };
	NobType const typeDst { GetNobType(idDst) };

	Nob const* pNobSrc { m_pModel->GetConstNob(idSrc) };
	Nob const* pNobDst { m_pModel->GetConstNob(idDst) };

	PosNob      const * pPosNobSrc { nullptr };
	PosNob      const * pPosNobDst { nullptr };
	Pipe        const * pPipeDst   { nullptr };
	IoConnector const * pConnSrc   { nullptr };
	IoConnector const * pConnDst   { nullptr };

	     if (typeSrc.IsPosNobType     ()) pPosNobSrc = static_cast<PosNob      const *>(pNobSrc);
	else if (typeSrc.IsIoConnectorType()) pConnSrc   = static_cast<IoConnector const *>(pNobSrc);
												
	     if (typeDst.IsPosNobType     ()) pPosNobDst = static_cast<PosNob      const *>(pNobDst);
	else if (typeDst.IsPipeType       ()) pPipeDst   = static_cast<Pipe        const *>(pNobDst);
	else if (typeDst.IsIoConnectorType()) pConnDst   = static_cast<IoConnector const *>(pNobDst);

	if (pPosNobSrc)
	{
		if (pPipeDst)
		{
			if ((pNobSrc->GetIoMode() == pNobDst->GetIoMode()) && (pNobSrc->GetIoMode() != NobIoMode::internal))
				return ct_connector; // case 12/13 - create Input/OutputConnector
			if (pPosNobSrc->IsDirectlyConnectedTo(*pPipeDst)) 
				return ct_none;
			if (typeSrc.IsInputLineType())
				return ct_fork;      // case 1
			if (typeSrc.IsOutputLineType())
				return ct_synapse;   // case 2
		}
		else if (pPosNobDst)
		{
			if (typeSrc.IsIoLineType() && typeDst.IsIoLineType())
			{
				assert(typeDst != typeSrc);
				return ct_knot;      // case 4/5 - Input and output line plugged together. Result is a knot.
			}

			if (typeSrc.IsOutputLineType() && typeDst.IsNeuronType())  // Output line plugged into neuron
				return ct_neuron;    // case 3                         // result is a neuron with with one more input

			return ct_none;
		}
	}

	if (pConnSrc && pConnDst && (typeDst != typeSrc) && (pConnSrc->Size() == pConnDst->Size()))
		return ct_plugConnectors;    // case 6

	return ct_none;
}

bool NNetModelReaderInterface::IsConnectedTo(NobId const idSrc, NobId const idDst) const
{
	if (GetNobType(idSrc).IsPipeType())
		return isConnectedToPipe(idDst, idSrc);
	if (GetNobType(idDst).IsPipeType())
		return isConnectedToPipe(idSrc, idDst);
	else
		return false;
}

bool NNetModelReaderInterface::isConnectedToPipe(NobId const idNob, NobId const idPipe) const
{
	if (idNob == m_pModel->GetStartKnotId(idPipe))
		return true;
    if (idNob == m_pModel->GetEndKnotId(idPipe))
		return true;
	return false;
}

void NNetModelReaderInterface::DrawExterior
(
	NobId       const   id,
	DrawContext const & context,
	tHighlight  const   type
) const
{
	if (Nob const * const p { m_pModel->GetNobConstPtr<Nob const *>(id) })
		p->DrawExterior(context, type);
}

void NNetModelReaderInterface::DrawInterior
(
	NobId       const   id, 
	DrawContext const & context,
	tHighlight  const   type
) 
const
{
	if (auto p { m_pModel->GetNobConstPtr<Nob const *>(id) })
		p->DrawInterior(context, type);
}

void NNetModelReaderInterface::DrawLine
(
	MicroMeterLine const & umLine, 
	DrawContext    const & context 
) const
{
	context.DrawLine(umLine.GetStartPoint(), umLine.GetEndPoint(), 10.0_MicroMeter, D2D1::ColorF::Brown);
}

MicroMeterPnt NNetModelReaderInterface::OrthoVector(NobId const idPipe) const
{
	MicroMeterPnt vector { m_pModel->GetNobConstPtr<Pipe const *>(idPipe)->GetVector() };
	return vector.OrthoVector().ScaledTo(NEURON_RADIUS*2.f);
}

fMicroSecs NNetModelReaderInterface::TotalScanTime() const 
{ 
	return GetParamsC().ScanTime() * Cast2Float(GetParamsC().NrOfScans() * GetScanRaster().NrOfPoints());
}
