// NNetModelReaderInterface.h 
//
// NNetModel
//
// Interface for reader threads

#pragma once

#include "MoreTypes.h"
#include "NobType.h"
#include "NobId.h"
#include "NNetModel.h"
#include "SoundInterface.h"
#include "tHighlightType.h"

class Nob;

#define DUMP() DumpModel(__FILE__, __LINE__)

class NNetModelReaderInterface
{
public:

	void Start(NNetModel * const pModel) { m_pModel = pModel;  }
	void Stop ()                         { m_pModel = nullptr; }

	void DumpModel(char const * const file, int const line) const { m_pModel->DumpModel(file, line); }
	void CheckModel() const { m_pModel->CheckModel(); };

	size_t const Size() const { return m_pModel->Size(); }

	bool          const   IsConnectionCandidate     (NobId const, NobId const) const;
	bool          const   CanConnectTo              (NobId const, NobId const) const;
	bool          const   IsConnectedTo             (NobId const, NobId const) const;
	bool          const   IsSelected                (NobId const) const;
	NobType       const   GetNobType                (NobId const) const;
	fHertz        const   GetPulseFrequency         (NobId const) const;
	size_t        const   GetNrOfSegments           (NobId const) const;
	SoundDescr    const   GetTriggerSound           (NobId const) const;
	bool          const   HasIncoming               (NobId const) const;
	bool          const   HasOutgoing               (NobId const) const;
	size_t        const   GetNrOfOutgoingConnections(NobId const) const;
	size_t        const   GetNrOfIncomingConnections(NobId const) const;
	size_t        const   GetNrOfConnections        (NobId const) const;
	mV            const   GetVoltage                (NobId const) const;
	mV            const   GetVoltage                (NobId const, MicroMeterPnt const &) const;
	Degrees       const   GetDirection              (NobId const) const; 

	MicroMeterPnt const   GetNobPos   (NobId const id)           const { return m_pModel->GetNobPos(id); }
	Nob           const * GetConstNob (NobId const id)           const { return m_pModel->GetConstNob(id); }
	fMicroSecs    const   GetSimulationTime()                    const { return m_pModel->GetSimulationTime(); }
	MonitorData   const & GetMonitorData()                       const { return m_pModel->GetMonitorData(); }
	fMicroSecs    const   GetTimeResolution()                    const { return m_pModel->GetParams().GetTimeResolution(); };
	wstring       const   GetModelFilePath()                     const { return m_pModel->GetModelFilePath(); }
	float         const   GetParameter(ParamType::Value const p) const { return m_pModel->GetParameter(p); }
	bool          const   IsNobInModel(Nob const & nob)          const { return m_pModel->GetConstNob(nob.GetId());	}
	UPNobList     const & GetUPNobs()                            const { return m_pModel->GetUPNobs(); }
	unsigned int  const   GetNrOf(NobType const type)            const { return GetUPNobs().GetCounter(type); }
	unsigned int  const   GetNrOfNobs()                          const { return GetUPNobs().GetCounter(); }
	bool          const   AnyNobsSelected()                      const { return GetUPNobs().AnyNobsSelected(); }
	bool          const   IsValidNobId(NobId const id)           const { return GetUPNobs().IsValidNobId(id); }
	size_t        const   GetSizeOfNobList()                     const { return GetUPNobs().Size(); }
	Signal      * const   FindSensor(MicroMeterPnt const & p)    const { return GetMonitorData().FindSensor(p); }
	SignalId      const   GetHighlightedSignalId()               const { return GetMonitorData().GetHighlightedSignalId(); }

	bool const IsInputConnector(NobId const id) const 
	{
		Nob const * const pNob { GetConstNob(id) };
		return pNob && pNob->IsInputConnector();
	}

	bool const IsOutputConnector(NobId const id) const
	{
		Nob const * const pNob { GetConstNob(id) };
		return pNob && pNob->IsOutputConnector();
	}

	bool const IsPipe(NobId const id) const
	{
		Nob const * const pNob { GetConstNob(id) };
		return pNob && pNob->IsPipe();
	}

	bool const IsKnot(NobId const id) const
	{
		Nob const * const pNob { GetConstNob(id) };
		return pNob && pNob->IsKnot();
	}

	bool  const GetDescriptionLine(int const, wstring &)                               const;
	NobId const FindNobAt(MicroMeterPnt const &, NobCrit const & = NobCritAlwaysTrue)  const;

	void DrawExterior  (NobId          const,   DrawContext const &, tHighlight const) const;
	void DrawInterior  (NobId          const,   DrawContext const &, tHighlight const) const;
	void DrawNeuronText(NobId          const,   DrawContext const &)                   const;
	void DrawLine      (MicroMeterLine const &, DrawContext const &)                   const;

	template <Nob_t T> bool const IsOfType(NobId const id) const { return T::TypeFits(GetNobType(id)); }

	MicroMeterPnt const OrthoVector(NobId const) const;

protected:
	NNetModel * m_pModel;

	bool const isConnectedToPipe(NobId const, NobId const) const;
	bool const onlyOneAxon(NobId const idSrc, NobId const idDst) const
	{
		return (GetNrOfOutgoingConnections(idSrc) + GetNrOfOutgoingConnections(idDst) <= 1);
	}
};
