// NNetModelReaderInterface.cpp 
//
// NNetModel

#include "stdafx.h"
#include "Nob.h"
#include "DrawContext.h"
#include "NNetModel.h"
#include "IoConnector.h"
#include "Neuron.h"
#include "InputNeuron.h"
#include "NNetModelReaderInterface.h"

bool const NNetModelReaderInterface::IsSelected(NobId const id) const
{
	auto p { m_pModel->GetNobConstPtr<Nob const *>(id) };
	return p ? p->IsSelected() : false; 
}

NobType const NNetModelReaderInterface::GetNobType(NobId const id) const
{
	auto p { m_pModel->GetNobConstPtr<Nob const *>(id) };
	return p ? p->GetNobType() : NobType::Value::undefined; 
}

Degrees const NNetModelReaderInterface::GetDirection(NobId const id) const 
{ 
	auto p { m_pModel->GetNobConstPtr<IoConnector const *>(id) };
	return p ? Radian2Degrees(p->GetDir()) : Degrees::NULL_VAL(); 
}

//fHertz const NNetModelReaderInterface::GetBaseFrequency(NobId const id) const 
//{ 
//	auto p { m_pModel->GetNobConstPtr<InputNeuron const *>(id) };
//	return p ? p->GetBaseFrequency() : fHertz::NULL_VAL(); 
//}
//
size_t const NNetModelReaderInterface::GetNrOfSegments(NobId const id) const
{
	auto p { m_pModel->GetNobConstPtr<Pipe const *>(id) };
	return p ? p->GetNrOfSegments() : 0; 
}

SoundDescr const NNetModelReaderInterface::GetTriggerSound(NobId const id) const
{
	Nob const & nob { * m_pModel->GetConstNob(id) };
	return nob.IsAnyNeuron()
		   ? static_cast<Neuron const &>(nob).GetTriggerSound()
		   : SoundDescr(); 
}

mV const NNetModelReaderInterface::GetVoltage(NobId const id) const
{
	auto p { m_pModel->GetNobConstPtr<Nob const *>(id) };
	return p ? p->GetVoltage() : mV::NULL_VAL(); 
}

mV const NNetModelReaderInterface::GetVoltage(NobId const id, MicroMeterPnt const & umPoint) const
{
	auto p { m_pModel->GetNobConstPtr<Pipe const *>(id) };
	return p ? p->GetVoltage(umPoint) : mV::NULL_VAL(); 
}

size_t const NNetModelReaderInterface::GetNrOfOutgoingConnections(NobId const id) const 
{ 
	auto p { m_pModel->GetNobConstPtr<BaseKnot const *>(id) };
	return p ? p->GetNrOfOutgoingConnections() : -1;
}

size_t const NNetModelReaderInterface::GetNrOfIncomingConnections(NobId const id) const 
{ 
	auto p { m_pModel->GetNobConstPtr<BaseKnot const *>(id) };
	return p ? p->GetNrOfIncomingConnections() : -1;
}

size_t const NNetModelReaderInterface::GetNrOfConnections(NobId const id) const 
{ 
	auto p { m_pModel->GetNobConstPtr<BaseKnot const *>(id) };
	return p ? p->GetNrOfConnections() : -1;
}

bool const NNetModelReaderInterface::HasIncoming(NobId const id) const
{
	auto p { m_pModel->GetNobConstPtr<BaseKnot const *>(id) };
	return p ? p->HasIncoming() : false; 
}

bool const NNetModelReaderInterface::HasOutgoing(NobId const id) const
{
	auto p { m_pModel->GetNobConstPtr<BaseKnot const *>(id) };
	return p ? p->HasOutgoing() : false; 
}

bool const NNetModelReaderInterface::GetDescriptionLine(int const iLine, wstring & wstrLine) const 
{
	return m_pModel->GetDescriptionLine(iLine, wstrLine);
};

// IsConnectionCandidate: Sort out obvious non-candidates

bool const NNetModelReaderInterface::IsConnectionCandidate(NobId const idSrc, NobId const idDst) const
{
	if (idSrc == idDst)
		return false; 
	if (IsConnectedTo(idSrc, idDst)) // if already connected we cannot connect again
		return false;
	return true;
}

bool const NNetModelReaderInterface::CanConnectTo(NobId const idSrc, NobId const idDst) const
{
	assert(idSrc != idDst);
	assert(IsDefined(idSrc));
	assert(IsDefined(idDst));
	assert(!IsConnectedTo(idSrc, idDst));

	NobType const typeSrc { GetNobType(idSrc) };
	NobType const typeDst { GetNobType(idDst) };

	if (typeSrc.IsKnotType() && typeDst.IsPipeType() )
	{
		return true;
	}
	else if (typeSrc.IsBaseKnotType() && typeDst.IsBaseKnotType())
	{
		size_t nrIn  = GetNrOfIncomingConnections(idSrc) + GetNrOfIncomingConnections(idDst);
		size_t nrOut = GetNrOfOutgoingConnections(idSrc) + GetNrOfOutgoingConnections(idDst);

		if (nrIn + nrOut == 0)
			return false;
		else if (typeSrc.IsKnotType() && typeDst.IsKnotType())
			return true;
		else if (nrOut <= 1)   // OutputNeuron 
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

bool const NNetModelReaderInterface::IsConnectedTo(NobId const idSrc, NobId const idDst) const
{
	if (GetNobType(idSrc).IsPipeType())
		return isConnectedToPipe(idDst, idSrc);
	if (GetNobType(idDst).IsPipeType())
		return isConnectedToPipe(idSrc, idDst);
	else
		return false;
}

bool const NNetModelReaderInterface::isConnectedToPipe(NobId const idNob, NobId const idPipe) const
{
	return (idNob == m_pModel->GetStartKnotId(idPipe)) || (idNob == m_pModel->GetEndKnotId(idPipe));
}

NobId const NNetModelReaderInterface::FindNobAt(MicroMeterPnt const & umPnt, NobCrit const & crit) const
{
	return m_pModel->FindNobAt(umPnt, crit);
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

void NNetModelReaderInterface::DrawNeuronText
(
	NobId       const   id, 
	DrawContext const & context
) 
const
{
	if (auto p { m_pModel->GetNobConstPtr<Neuron const *>(id) })
		if (p)
			p->DrawNeuronText(context);
}

void NNetModelReaderInterface::DrawLine
(
	MicroMeterLine const & umLine, 
	DrawContext    const & context 
) const
{
	context.DrawLine(umLine.GetStartPoint(), umLine.GetEndPoint(), 10.0_MicroMeter, D2D1::ColorF::Brown);
}

MicroMeterPnt const NNetModelReaderInterface::OrthoVector(NobId const idPipe) const
{
	MicroMeterPnt vector { m_pModel->GetNobConstPtr<Pipe const *>(idPipe)->GetVector() };
	return vector.OrthoVector().ScaledTo(NEURON_RADIUS*2.f);
}
