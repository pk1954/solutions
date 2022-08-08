// NNetModelReaderInterface.h 
//
// NNetModel
//
// Interface for reader threads

#pragma once

#include "NobType.h"
#include "NobId.h"
#include "NNetModel.h"
#include "Pipe.h"
#include "tHighlightType.h"

import MoreTypes;
import SoundInterface;

class Nob;

#define DUMP() DumpModel(__FILE__, __LINE__)

enum class ConnectionType
{
	ct_none,
	ct_knot,
	ct_neuron,
	ct_outputline,
	ct_pipe,
	ct_ioLine,
	ct_ioConnector
};

class NNetModelReaderInterface
{
public:

	void SetModel(NNetModel * const pModel) { m_pModel = pModel; }
	bool IsDefined() const { return m_pModel != nullptr; }

	void DumpModel(char const * const file, int const line) const { m_pModel->DumpModel(file, line); }
	void CheckModel() const { m_pModel->CheckModel(); };

	size_t Size() const { return m_pModel->Size(); }

	bool                    IsConnectionCandidate(NobId const, NobId const) const;
	ConnectionType          ConnectionResult     (NobId const, NobId const) const;
	bool                    IsConnectedTo        (NobId const, NobId const) const;
	bool                    IsSelected           (NobId const) const;
	NobType                 GetNobType           (NobId const) const;
	size_t                  GetNrOfSegments      (NobId const) const;
	bool                    HasIncoming          (NobId const) const;
	bool                    HasOutgoing          (NobId const) const;
	size_t                  GetNrOfOutConns      (NobId const) const;
	size_t                  GetNrOfInConns       (NobId const) const;
	size_t                  GetNrOfConnections   (NobId const) const;
	mV                      GetVoltage           (NobId const) const;
	mV                      GetVoltageAt         (NobId const, MicroMeterPnt const &) const;
	Degrees                 GetDirection         (NobId const) const; 
	SignalGenerator const * GetSigGenC           (NobId const) const;

	wstring                 GetTypeName (NobId const id)           const { return NobType::GetName(GetNobType(id).GetValue()); };
	MicroMeterPnt           GetNobPos   (NobId const id)           const { return m_pModel->GetNobConstPtr<Nob const *>(id)->GetPos(); }
	Nob             const * GetConstNob (NobId const id)           const { return m_pModel->GetConstNob(id); }
	MonitorData     const & GetMonitorDataC()                      const { return m_pModel->GetMonitorData(); }
	Param           const & GetParamsC()                           const { return m_pModel->GetParams(); };
	fMicroSecs              TimeResolution()                       const { return m_pModel->GetParams().TimeResolution(); };
	fMicroSecs              FilterSize()                           const { return m_pModel->GetParams().FilterSize(); };
	wstring                 GetModelFilePath()                     const { return m_pModel->GetModelFilePath(); }
	float                   GetParameter(ParamType::Value const p) const { return m_pModel->GetParameter(p); }
	bool                    IsNobInModel(Nob const & nob)          const { return m_pModel->GetConstNob(nob.GetId());	}
	UPNobList       const & GetUPNobsC()                           const { return m_pModel->GetUPNobs(); }
	unsigned int            GetNrOf(NobType const type)            const { return GetUPNobsC().GetCounter(type); }
	unsigned int            GetNrOfNobs()                          const { return GetUPNobsC().GetCounter(); }
	bool                    AnyNobsSelected()                      const { return GetUPNobsC().AnyNobsSelected(); }
	bool                    IsValidNobId(NobId const id)           const { return GetUPNobsC().IsValidNobId(id); }
	size_t                  GetSizeOfNobList()                     const { return GetUPNobsC().Size(); }
	Signal          const * FindSensor(MicroMeterPnt const & p)    const { return GetMonitorDataC().FindSensor(p); }
	SignalId                GetHighlightedSignalId()               const { return GetMonitorDataC().GetHighlightedSignalId(); }
	UPSigGenList    const & GetSigGenList()                        const { return m_pModel->GetSigGenList(); }
	SignalGenerator const * GetSigGenSelectedC()                   const { return m_pModel->GetSigGenList().GetSigGenSelected(); }
	SignalGenerator       * GetSigGenStandard()                    const { return m_pModel->GetSigGenList().StdSigGen(); }
	SignalGenerator const * GetSigGenC(SigGenId const id)          const { return m_pModel->GetSigGenList().GetSigGen(id); }
	SigGenId                GetSigGenIdSelected()                  const { return m_pModel->GetSigGenList().GetSigGenIdSelected(); }
	bool                    IsInList(wstring const & name)         const { return m_pModel->GetSigGenList().IsInList(name); }
	Sensor          const * GetSensorSelectedC()                   const { return m_pModel->GetSensorList().GetSensorSelected(); }
	SensorId                GetSensorIdSelected()                  const { return m_pModel->GetSensorList().GetSensorIdSelected(); }
	bool                    IsAnySensorSelected()                  const { return m_pModel->GetSensorList().IsAnySensorSelected(); }

	bool IsAnyNeuron(NobId const id) const 
	{
		Nob const * const pNob { GetConstNob(id) };
		return pNob && pNob->IsAnyNeuron();
	}

	bool IsInputLine(NobId const id) const 
	{
		Nob const * const pNob { GetConstNob(id) };
		return pNob && pNob->IsInputLine();
	}

	bool IsOutputLine(NobId const id) const 
	{
		Nob const * const pNob { GetConstNob(id) };
		return pNob && pNob->IsOutputLine();
	}

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

	NobId FindNobAt(MicroMeterPnt const & umPnt) const
	{
		return m_pModel->FindNobAt(umPnt, [](auto &) { return true; });
	}

	NobId FindNobAt(MicroMeterPnt const & umPnt, auto const & crit) const
	{
		return m_pModel->FindNobAt(umPnt, crit);
	}

	bool GetDescriptionLine(int const, wstring &) const;

	void DrawExterior(NobId          const,   DrawContext const &, tHighlight const) const;
	void DrawInterior(NobId          const,   DrawContext const &, tHighlight const) const;
	void DrawLine    (MicroMeterLine const &, DrawContext const &)                   const;

	MicroMeterPnt OrthoVector(NobId const) const;

	template <Nob_t T> bool IsOfType(NobId const id) const { return T::TypeFits(GetNobType(id)); }

	template <Nob_t T>    // const version
	void Apply2AllC(auto const& func) const
	{
		GetUPNobsC().Apply2AllC<T>(func);
	}

	template <Nob_t T>   // const version
	void Apply2AllInRectC(MicroMeterRect const & r, auto const & func) const
	{
		GetUPNobsC().Apply2AllInRectC<T>(r, func);
	}

protected:
	NNetModel * m_pModel;

private:
	bool isConnectedToPipe(NobId const, NobId const) const;
};
