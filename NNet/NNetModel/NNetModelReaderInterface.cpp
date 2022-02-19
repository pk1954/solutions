// NNetModelReaderInterface.cpp 
//
// NNetModel

#include "stdafx.h"
#include "Nob.h"
#include "DrawContext.h"
#include "NNetModel.h"
#include "InputConnector.h"
#include "IoConnector.h"
#include "Neuron.h"
#include "InputNeuron.h"
#include "NNetModelReaderInterface.h"

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

fHertz NNetModelReaderInterface::GetActFrequency(NobId const id) const 
{ 
	NobType const type { GetNobType(id) };
	if (type.IsInputConnectorType())
		return m_pModel->GetNobConstPtr<InputConnector const *>(id)->GetActFrequency();
	else if (type.IsInputNeuronType())
		return m_pModel->GetNobConstPtr<InputNeuron const *>(id)->GetActFrequency();
	else
		return fHertz::NULL_VAL(); 
}

size_t NNetModelReaderInterface::GetNrOfSegments(NobId const id) const
{
	auto p { m_pModel->GetNobConstPtr<Pipe const *>(id) };
	return p ? p->GetNrOfSegments() : 0; 
}

SoundDescr NNetModelReaderInterface::GetTriggerSound(NobId const id) const
{
	Nob const & nob { * m_pModel->GetConstNob(id) };
	return nob.IsAnyNeuron()
		   ? static_cast<Neuron const &>(nob).GetTriggerSound()
		   : SoundDescr(); 
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

bool NNetModelReaderInterface::CanConnectTo(NobId const idSrc, NobId const idDst) const
{
	assert(idSrc != idDst);
	assert(IsDefined(idSrc));
	assert(IsDefined(idDst));
	assert(!IsConnectedTo(idSrc, idDst));

	NobType const typeSrc { GetNobType(idSrc) };
	NobType const typeDst { GetNobType(idDst) };

	if (typeSrc.IsBaseKnotType() && typeDst.IsPipeType())
	{
		BaseKnot const & baseKnot { * m_pModel->GetNobConstPtr<BaseKnot const *>(idSrc) };
		Pipe     const & pipe     { * m_pModel->GetNobConstPtr<Pipe     const *>(idDst)     };
		return ! baseKnot.IsDirectlyConnectedTo(*pipe.GetStartKnotPtr()) &&
			   ! baseKnot.IsDirectlyConnectedTo(*pipe.GetEndKnotPtr  ());
	}

	if (typeSrc.IsBaseKnotType() && typeDst.IsBaseKnotType())
	{
		BaseKnot const & baseKnotSrc { * m_pModel->GetNobConstPtr<BaseKnot const *>(idSrc) };
		BaseKnot const & baseKnotDst { * m_pModel->GetNobConstPtr<BaseKnot const *>(idDst) };
		size_t   const   nrIn        { baseKnotSrc.GetNrOfInConns () + baseKnotDst.GetNrOfInConns () };
		size_t   const   nrOut       { baseKnotSrc.GetNrOfOutConns() + baseKnotDst.GetNrOfOutConns() };

		if (nrIn + nrOut == 0)
			return false;
		if (typeSrc.IsKnotType() && typeDst.IsKnotType())
		{
			if ((nrIn<=2) && (nrOut<=1))
				return true;                // Synapse
			if ((nrIn<=1) && (nrOut<=2))
				return true;                // Branch
			return false;
		}
		if (nrOut <= 1)   // OutputNeuron 
			return true;
		else 
			return false;
	}
	else if (typeSrc.IsIoConnectorType() && typeDst.IsIoConnectorType() && (typeDst != typeSrc))
	{
		IoConnector const & connSrc { * m_pModel->GetNobConstPtr<IoConnector const *>(idSrc) }; 
		IoConnector const & connDst { * m_pModel->GetNobConstPtr<IoConnector const *>(idDst) }; 
		if (connSrc.Size() == connDst.Size())
			return true;
	}
	return false;
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
