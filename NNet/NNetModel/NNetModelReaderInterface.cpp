// NNetModelReaderInterface.cpp 
//
// NNetModel

#include <cassert> 
#include "Nob.h"
#include "NNetModel.h"
#include "InputConnector.h"
#include "IoConnector.h"
#include "InputLine.h"
#include "NNetModelReaderInterface.h"

import BasicTypes;
import DrawContext;
import Neuron;

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

SignalGenerator const * NNetModelReaderInterface::GetSigGenC(NobId const id) const
{
	Nob const & nob { * m_pModel->GetConstNob(id) };
	if (nob.IsInputLine())
		return m_pModel->GetNobConstPtr<InputLine const *>(id)->GetSigGenC(); 
	else if (nob.IsInputConnector())
		return m_pModel->GetNobConstPtr<InputConnector const *>(id)->GetSigGenC(); 
	else
		return nullptr;
}

mV NNetModelReaderInterface::GetVoltage(NobId const id) const
{
	auto p { m_pModel->GetNobConstPtr<BaseKnot const *>(id) };
	return p ? p->GetVoltage() : mV::NULL_VAL(); 
}

mV NNetModelReaderInterface::GetVoltageAt(NobId const id, MicroMeterPnt const & umPoint) const
{
	auto p { m_pModel->GetNobConstPtr<Pipe const *>(id) };
	return p ? p->GetVoltageAt(umPoint) : mV::NULL_VAL(); 
}

size_t NNetModelReaderInterface::GetNrOfOutConns(NobId const id) const 
{ 
	auto p { m_pModel->GetNobConstPtr<BaseKnot const *>(id) };
	return p ? p->GetNrOfOutConns() : -1;
}

size_t NNetModelReaderInterface::GetNrOfInConns(NobId const id) const 
{ 
	auto p { m_pModel->GetNobConstPtr<BaseKnot const *>(id) };
	return p ? p->GetNrOfInConns() : -1;
}

size_t NNetModelReaderInterface::GetNrOfConnections(NobId const id) const 
{ 
	auto p { m_pModel->GetNobConstPtr<BaseKnot const *>(id) };
	return p ? p->GetNrOfConnections() : -1;
}

bool NNetModelReaderInterface::HasIncoming(NobId const id) const
{
	auto p { m_pModel->GetNobConstPtr<BaseKnot const *>(id) };
	return p ? p->HasIncoming() : false; 
}

bool NNetModelReaderInterface::HasOutgoing(NobId const id) const
{
	auto p { m_pModel->GetNobConstPtr<BaseKnot const *>(id) };
	return p ? p->HasOutgoing() : false; 
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

	assert(idSrc != idDst);
	assert(::IsDefined(idSrc));
	assert(!IsConnectedTo(idSrc, idDst));

	NobType const typeSrc { GetNobType(idSrc) };
	NobType const typeDst { GetNobType(idDst) };

	BaseKnot    const * pBaseKnotSrc { nullptr };
	BaseKnot    const * pBaseKnotDst { nullptr };
	Pipe        const * pPipeDst     { nullptr };
	IoConnector const * pConnSrc     { nullptr };
	IoConnector const * pConnDst     { nullptr };

	     if (typeSrc.IsBaseKnotType   ()) pBaseKnotSrc = m_pModel->GetNobConstPtr<BaseKnot    const *>(idSrc);
	else if (typeSrc.IsIoConnectorType()) pConnSrc     = m_pModel->GetNobConstPtr<IoConnector const *>(idSrc);

	     if (typeDst.IsBaseKnotType   ()) pBaseKnotDst = m_pModel->GetNobConstPtr<BaseKnot    const *>(idDst);
	else if (typeDst.IsPipeType       ()) pPipeDst     = m_pModel->GetNobConstPtr<Pipe        const *>(idDst);
	else if (typeDst.IsIoConnectorType()) pConnDst     = m_pModel->GetNobConstPtr<IoConnector const *>(idDst);

	if (pBaseKnotSrc)
	{
		if (pPipeDst)
		{
			if (pBaseKnotSrc->IsDirectlyConnectedTo(*pPipeDst)) 
				return ct_none;
			if (typeSrc.IsInputLineType())
				return ct_pipe;
			if (typeSrc.IsOutputLineType() && (pBaseKnotSrc->GetNrOfInConns() == 1))
				return ct_pipe;
		}
		else if (pBaseKnotDst)
		{
			size_t const nrIn  { pBaseKnotSrc->GetNrOfInConns () + pBaseKnotDst->GetNrOfInConns () };
			size_t const nrOut { pBaseKnotSrc->GetNrOfOutConns() + pBaseKnotDst->GetNrOfOutConns() };

			if ((typeSrc.IsOutputLineType() && typeDst.IsOutputLineType()) && (nrIn == 2))
				return ct_outputline;

			if (typeSrc.IsIoLineType() && typeDst.IsIoLineType() && (typeDst != typeSrc))
				return ct_ioLine;

			if (
				  (nrIn == 2) &&
				  (
					(typeSrc.IsNeuronType() && typeDst.IsOutputLineType()) || 
					(typeDst.IsNeuronType() && typeSrc.IsOutputLineType()) 
				  )
			   )
				return ct_neuron;

			if ((typeSrc.IsKnotType() && typeDst.IsIoLineType()) || (typeDst.IsKnotType() && typeSrc.IsIoLineType()))
			{
				if ((nrIn==2) && (nrOut==1))
					return ct_knot;                // Synapse
				if ((nrIn==1) && (nrOut==2))
					return ct_knot;                // Branch
				return ct_none;
			}
		}
	}

	if (pConnSrc && pConnDst && (typeDst != typeSrc) && (pConnSrc->Size() == pConnDst->Size()))
		return ct_ioConnector;

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
	return (idNob == m_pModel->GetStartKnotId(idPipe)) || (idNob == m_pModel->GetEndKnotId(idPipe));
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
