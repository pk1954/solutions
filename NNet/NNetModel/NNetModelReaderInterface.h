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

	size_t Size() const { return m_pModel->Size(); }

	bool       IsConnectionCandidate     (NobId const, NobId const) const;
	bool       CanConnectTo              (NobId const, NobId const) const;
	bool       IsConnectedTo             (NobId const, NobId const) const;
	bool       IsSelected                (NobId const) const;
	NobType    GetNobType                (NobId const) const;
	size_t     GetNrOfSegments           (NobId const) const;
	SoundDescr GetTriggerSound           (NobId const) const;
	bool       HasIncoming               (NobId const) const;
	bool       HasOutgoing               (NobId const) const;
	size_t     GetNrOfOutgoingConnections(NobId const) const;
	size_t     GetNrOfIncomingConnections(NobId const) const;
	size_t     GetNrOfConnections        (NobId const) const;
	mV         GetVoltage                (NobId const) const;
	mV         GetVoltageAt              (NobId const, MicroMeterPnt const &) const;
	Degrees    GetDirection              (NobId const) const; 
	fHertz     GetActFrequency           (NobId const) const; 

	wstring             GetTypeName (NobId const id)           const { return NobType::GetName(GetNobType(id).GetValue()); };
	MicroMeterPnt       GetNobPos   (NobId const id)           const { return m_pModel->GetNobPos(id); }
	Nob         const * GetConstNob (NobId const id)           const { return m_pModel->GetConstNob(id); }
	fMicroSecs          GetSimulationTime()                    const { return m_pModel->GetSimulationTime(); }
	MonitorData const & GetConstMonitorData()                  const { return m_pModel->GetMonitorData(); }
	fMicroSecs          TimeResolution()                       const { return m_pModel->GetParams().TimeResolution(); };
	wstring             GetModelFilePath()                     const { return m_pModel->GetModelFilePath(); }
	float               GetParameter(ParamType::Value const p) const { return m_pModel->GetParameter(p); }
	bool                IsNobInModel(Nob const & nob)          const { return m_pModel->GetConstNob(nob.GetId());	}
	UPNobList   const & GetUPNobsC()                           const { return m_pModel->GetUPNobs(); }
	unsigned int        GetNrOf(NobType const type)            const { return GetUPNobsC().GetCounter(type); }
	unsigned int        GetNrOfNobs()                          const { return GetUPNobsC().GetCounter(); }
	bool                AnyNobsSelected()                      const { return GetUPNobsC().AnyNobsSelected(); }
	bool                IsValidNobId(NobId const id)           const { return GetUPNobsC().IsValidNobId(id); }
	size_t              GetSizeOfNobList()                     const { return GetUPNobsC().Size(); }
	Signal            * FindSensor(MicroMeterPnt const & p)    const { return GetConstMonitorData().FindSensor(p); }
	SignalId            GetHighlightedSignalId()               const { return GetConstMonitorData().GetHighlightedSignalId(); }

	bool IsInputConnector(NobId const id) const 
	{
		Nob const * const pNob { GetConstNob(id) };
		return pNob && pNob->IsInputConnector();
	}

	bool IsOutputConnector(NobId const id) const
	{
		Nob const * const pNob { GetConstNob(id) };
		return pNob && pNob->IsOutputConnector();
	}

	bool IsPipe(NobId const id) const
	{
		Nob const * const pNob { GetConstNob(id) };
		return pNob && pNob->IsPipe();
	}

	bool IsKnot(NobId const id) const
	{
		Nob const * const pNob { GetConstNob(id) };
		return pNob && pNob->IsKnot();
	}

	bool  GetDescriptionLine(int const, wstring &)                               const;
	NobId FindNobAt(MicroMeterPnt const &, NobCrit const & = NobCritAlwaysTrue)  const;

	void DrawExterior(NobId          const,   DrawContext const &, tHighlight const) const;
	void DrawInterior(NobId          const,   DrawContext const &, tHighlight const) const;
	void DrawLine    (MicroMeterLine const &, DrawContext const &)                   const;

	MicroMeterPnt OrthoVector(NobId const) const;

	template <Nob_t T> bool IsOfType(NobId const id) const { return T::TypeFits(GetNobType(id)); }

	template <Nob_t T>    // const version
	void Apply2All(function<void(T const&)> const& func) const
	{
		GetUPNobsC().Apply2All<T>(func);
	}

	template <Nob_t T>   // const version
	void Apply2AllInRect(MicroMeterRect const & r, function<void(T const &)> const & func) const
	{
		GetUPNobsC().Apply2AllInRect<T>(r, func);
	}

protected:
	NNetModel * m_pModel;

private:
	bool isConnectedToPipe(NobId const, NobId const) const;
};
