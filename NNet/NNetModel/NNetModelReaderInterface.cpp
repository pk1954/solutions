// NNetModelReaderInterface.cpp 
//
// NNetModel

module;

#include <d2d1helper.h>
#include <cassert> 
#include <string> 
#include <compare> 
#include <optional>

module NNetModel:NNetModelReaderInterface;

import Types;
import DrawContext;
import :tHighlight;
import :NobId;
import :NNetSignal;
import :Nob;
import :Synapse;
import :SignalGenerator;
import :InputLine;
import :IoConnector;
import :InputConnector;
import :PosNob;
import :Pipe;

using std::wstring;
using std::optional;

bool NNetModelReaderInterface::IsSelected(NobId const id) const
{
	auto p { m_pModel->GetNobConstPtr<Nob const *>(id) };
	return p ? p->IsSelected() : false; 
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

ConnectionType NNetModelReaderInterface::ConnectionResult(NobId const idSrc, NobId const idDst) const
{
	using enum ConnectionType;

	if (::IsUndefined(idDst))
		return ct_none;

	if (::IsUndefined(idSrc))
		return ct_none;

	if (!m_pModel->IsConnectionCandidate(idSrc, idDst))
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
			if (pPosNobSrc->IsDirectlyConnectedTo(*pPipeDst)) 
				return ct_none;
			if (typeSrc.IsInputLineType())
				return ct_fork;      // case 1
			if (typeSrc.IsOutputLineType())
				return ct_synapse;   // case 2
		}
		if (pPosNobDst)
		{
			if (typeSrc.IsIoLineType() && typeDst.IsIoLineType())
			{
				if (typeSrc.IsInputLineType() && typeDst.IsInputLineType())
					return ct_connector; // case 12 - create InputConnector

				if (typeSrc.IsOutputLineType() && typeDst.IsOutputLineType())
					return ct_connector; // case 13 - create OutputConnector

				return ct_knot;      // case 4/5 - Input and output line plugged together. Result is a knot.
			}

			if (typeSrc.IsOutputLineType() && typeDst.IsNeuronType())  // Output line plugged into neuron
				return ct_neuron;    // case 3                         // result is a neuron with with one more input

			return ct_none;
		}
		else if (pConnDst)
		{
			if (typeSrc.IsInputLineType() && typeDst.IsInputConnectorType())
				return ct_connector; // case 14 - add InputLine to InputConnector

			if (typeSrc.IsOutputLineType() && typeDst.IsOutputConnectorType())
				return ct_connector; // case 15 - add OutputLine to OutputConnector

			return ct_none;
		}
	}
	if (pConnSrc && pConnDst)
	{
		if (typeDst == typeSrc)
			return ct_connector;         // case 16/17 - connect IO-Connectors of same type
		else if (pConnSrc->Size() == pConnDst->Size())
			return ct_plugConnectors;    // case 6 - IO-Connectors of opposite type with same size
	}

	return ct_none;
}

NobId NNetModelReaderInterface::FindAnyNobAt(MicroMeterPnt const& umPnt) const
{
	return m_pModel->ModelFindNobAt(umPnt, [](auto&) { return true; });
}

NobId NNetModelReaderInterface::FindConnectionCandidate
(
	MicroMeterPnt const& umPnt,
	NobId         const id
) const
{
	return m_pModel->ModelFindNobAt
	(
		umPnt, 
		[this, id](auto& s) 
		{ 
			return m_pModel->IsConnectionCandidate(id, s.GetId()); 
		}
	);
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

void NNetModelReaderInterface::DrawScanAreaBackground(DrawContext const& context) const 
{	
	m_pModel->m_scanMatrix.DrawScanAreaBackground(context, m_pModel->GetScanRaster()); 
}

void NNetModelReaderInterface::DrawScanRaster(DrawContext const& context) const
{
	m_pModel->m_scanMatrix.DrawScanRaster(context, m_pModel->GetScanRaster());
}

void NNetModelReaderInterface::DrawScanAreaHandles(DrawContext const& context, optional<CardPoint> const cp) const
{
	m_pModel->m_scanMatrix.DrawScanAreaHandles(context, m_pModel->GetScanRaster(), cp);
}

void NNetModelReaderInterface::DrawSensorDensityMap(DrawContext const& context) const
{
	m_pModel->m_scanMatrix.DrawSensorDensityMap(context, m_pModel->GetScanRaster(), m_pModel->GetUPNobsC());
}

void NNetModelReaderInterface::DrawScanImage(DrawContext const& context, ByteImage const* pByteImage, ColorLUT const& lut) const
{
	m_pModel->m_scanMatrix.DrawScanImage(context, m_pModel->GetScanRaster(), pByteImage, lut);
}

optional<CardPoint> NNetModelReaderInterface::SelectScanAreaHandle
(
	DrawContext   const& context, 
	MicroMeterPnt const& umCrsrPos
) const
{
	return m_pModel->SelectScanAreaHandle(context, umCrsrPos);
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

SignalId NNetModelReaderInterface::FindSignalId(NNetSignalSource const * const pSigSrc) const
{
    return FindSignalId
    (
        [this, pSigSrc](Signal const& signal)
        { 
			return static_cast<NNetSignal const &>(signal).GetSignalSource() == pSigSrc; 
		}
    );
}
