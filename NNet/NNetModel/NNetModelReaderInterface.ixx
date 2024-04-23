// NNetModelReaderInterface.ixx
//
// NNetModel
//
// Interface for reader threads

module;

#include <string>
#include <optional>

export module NNetModel:NNetModelReaderInterface;

import Types;
import SaveCast;
import DrawContext;
import SoundInterface;
import Signals;
import Vector2D;
import :NNetParameters;
import :NobType;
import :tHighlight;
import :Model;
import :Nob;
import :PosNob;
import :NobId;
import :UPNobList;
import :UPSigGenList;
import :Sensor;
import :SignalGenerator;
import :InputLine;
import :InputConnector;

using std::wstring;
using std::optional;

export enum class ConnectionType
{
	ct_none,
	ct_synapse,
	ct_fork,
	ct_knot,
	ct_neuron,
	ct_connector,
	ct_plugConnectors
};

export class NNetModelReaderInterface
{
public:

	void SetModel(Model * const pModel) { m_pModel = pModel; }
	bool IsDefined() const { return m_pModel != nullptr; }

	void DumpModel(char const * const file, int const line) const { m_pModel->DumpModel(file, line); }
	void CheckModel() const { m_pModel->CheckModel(); };

	size_t Size() const { return m_pModel->Size(); }

	SignalId               FindSignalId         (NNetSignalSource const * const) const;
	ConnectionType         ConnectionResult     (NobId const, NobId const) const;
	mV                     GetVoltageAt         (NobId const, MicroMeterPnt const&) const;
	mV                     GetVoltage           (NobId const)     const;
	bool                   IsSelected           (NobId const)     const;
	size_t                 GetNrOfSegments      (NobId const)     const;
	size_t                 GetNrOfOutConns      (NobId const)     const;
	size_t                 GetNrOfInConns       (NobId const)     const;
	Degrees                GetDirection         (NobId const)     const;
	SignalGenerator const* GetSigGenC           (NobId const)     const;
	wstring                GetTypeName          (NobId const id)  const { return NobType::GetName(GetNobType(id).GetValue()); };
	NobType                GetNobType           (NobId const id)  const { return m_pModel->GetNobType(id); }
	MicroMeterPnt          GetNobPos            (NobId const id)  const { return m_pModel->GetNobConstPtr<Nob const*>(id)->GetPos(); }
	PosNob          const* GetConstPosNobPtr    (NobId const id)  const { return m_pModel->GetNobConstPtr<PosNob const*>(id); }
	Nob             const* GetConstNob          (NobId const id)  const { return m_pModel->GetConstNob(id); }
	MonitorData     const& GetMonitorDataC()                      const { return m_pModel->GetMonitorData(); }
	void                   PrintModelSize()                       const { return m_pModel->PrintModelSize(); }
	NNetParameters  const& GetParamsC()                           const { return m_pModel->GetParams(); };
	EventList       const& GetEventList()                         const { return m_pModel->GetEventList(); }
	fMicroSecs             TimeResolution()                       const { return m_pModel->GetParams().TimeResolution(); };
	fMicroSecs             PixelScanTime()                        const { return m_pModel->GetParams().PixelScanTime(); };
	wstring                GetModelFilePath()                     const { return m_pModel->GetModelFilePath(); }
	float                  GetParameter(ParamType::Value const p) const { return m_pModel->GetParameter(p); }
	bool                   IsNobInModel(Nob const & nob)          const { return m_pModel->GetConstNob(nob.GetId()); }
	UPNobList       const& GetUPNobsC()                           const { return m_pModel->GetUPNobs(); }
	MicroMeterRect         GetScanAreaRect()                      const { return m_pModel->GetScanAreaRect(); }
	unsigned int           GetNrOf(NobType const type)            const { return GetUPNobsC().GetCounter(type); }
	unsigned int           GetNrOfNobs()                          const { return GetUPNobsC().GetCounter(); }
	bool                   AnyNobsSelected()                      const { return GetUPNobsC().AnyNobsSelected(); }
	bool                   IsValidNobId(NobId const id)           const { return GetUPNobsC().IsValidNobId(id); }
	size_t                 GetSizeOfNobList()                     const { return GetUPNobsC().Size(); }
	SignalId               FindSignalId(auto const & crit)        const { return GetMonitorDataC().FindSignalId(crit); }
	SignalId               GetHighlightedSignalId()               const { return GetMonitorDataC().GetHighlightedSignalId(); }
	UPSigGenList    const& GetSigGenList()                        const { return m_pModel->GetSigGenList(); }
	SignalGenerator const* GetSigGenSelectedC()                   const { return GetSigGenList().GetSigGenSelected(); }
	SignalGenerator const* GetSigGenC(SigGenId const id)          const { return GetSigGenList().GetSigGen(id); }
	SigGenId               GetSigGenIdSelected()                  const { return GetSigGenList().GetSigGenIdSelected(); }
	bool                   IsInList(wstring const & name)         const { return GetSigGenList().IsInList(name); }
	wstring         const& GetSigGenName(SigGenId const id)       const { return GetSigGenList().GetSigGen(id)->GetName(); }
	UPSensorList    const& GetSensorList()                        const { return m_pModel->GetSensorList(); }
	Sensor          const* GetSensor(SensorId const id)           const { return GetSensorList().GetSensor(id); }
	MicroMeter             GetScanResolution()                    const { return m_pModel->GetScanResolution(); }
	RasterPoint            GetScanAreaSize()                      const { return m_pModel->GetScanAreaSize(); }
	Raster          const& GetScanRaster()                        const { return m_pModel->GetScanRaster(); }
	ScanImageByte   const* GetScanImageC()                        const { return m_pModel->GetScanImageC(); }
	bool                   ModelLocked()                          const { return m_pModel->GetScanImageC() != nullptr; }
	int                    GetNrOfScans()                         const { return Cast2Int(m_pModel->GetParameter(ParamType::Value::nrOfScans)); }
	bool                   HasMicroSensor(NobId const id)         const { return m_pModel->GetMicroSensorList().HasMicroSensor(id); }
	MicroSensor     const* GetMicroSensorC(NobId const id)        const { return m_pModel->GetMicroSensorList().GetMicroSensorC(id); }
	SignalId               SelectSignal(SignalId const id)        const { return m_pModel->GetMonitorData().SetHighlightedSignal(id); }
	bool                   IsAnySignalSelected()                  const { return m_pModel->GetMonitorData().IsAnySignalSelected(); }
	optional<RasterPoint>  FindRasterPos(MicroMeterPnt const pnt) const { return GetScanRaster().FindRasterPos(pnt); }
	MicroMeterRect         GetRasterRect(RasterPoint const& rp)   const { return GetScanRaster().GetPointRect(rp); }
	fMicroSecs             TotalScanTime()                        const;

	bool IsInputLine(NobId const id) const
	{
		Nob const* const pNob{ GetConstNob(id) };
		return pNob && pNob->IsInputLine();
	}

	bool IsOutputLine(NobId const id) const
	{
		Nob const* const pNob{ GetConstNob(id) };
		return pNob && pNob->IsOutputLine();
	}

	bool IsIoConnector(NobId const id) const
	{
		Nob const* const pNob { GetConstNob(id) };
		return pNob && pNob->IsIoConnector();
	}

	bool IsInputConnector(NobId const id) const
	{
		Nob const* const pNob { GetConstNob(id) };
		return pNob && pNob->IsInputConnector();
	}

	bool IsOutputConnector(NobId const id) const
	{
		Nob const* const pNob{ GetConstNob(id) };
		return pNob && pNob->IsOutputConnector();
	}

	bool IsPipe(NobId const id) const
	{
		Nob const* const pNob{ GetConstNob(id) };
		return pNob && pNob->IsPipe();
	}

	bool IsKnot(NobId const id) const
	{
		Nob const* const pNob { GetConstNob(id) };
		return pNob && pNob->IsKnot();
	}

	bool IsSynapse(NobId const id) const
	{
		Nob const* const pNob{ GetConstNob(id) };
		return pNob && pNob->IsSynapse();
	}

	bool HasParentNob(NobId const id) const
	{
		Nob const* const pNob{ GetConstNob(id) };
		return pNob && pNob->HasParentNob();
	}

	Nob const *GetParentNob(NobId const id) const
	{
		Nob const* const pNob{ GetConstNob(id) };
		return pNob ? pNob->GetParentNob() : nullptr;
	}

	NobId FindAnyNobAt           (MicroMeterPnt const&) const;
	NobId FindConnectionCandidate(MicroMeterPnt const&,	NobId const) const;

	bool GetDescriptionLine(int const, wstring&) const;

	void DrawExterior(NobId      const,   DrawContext const &, tHighlight const) const;
	void DrawInterior(NobId      const,   DrawContext const &, tHighlight const) const;
	void DrawLine(MicroMeterLine const &, DrawContext const &)                   const;

	MicroMeterPnt OrthoVector(NobId const) const;

	template <Nob_t T>
	bool IsOfType(NobId const id) const 
	{ 
		return T::TypeFits(GetNobType(id)); 
	}

	void Apply2AllC(auto const& func) const
	{
		GetUPNobsC().Apply2AllC(func);
	}

	template <Nob_t T>    // const version
	void Apply2AllC(auto const& func) const
	{
		GetUPNobsC().Apply2AllC<T>(func);
	}

	template <Nob_t T>   // const version
	void Apply2AllInRectC(MicroMeterRect const& r, auto const& func) const
	{
		GetUPNobsC().Apply2AllInRectC<T>(r, func);
	}

	void Apply2allEvents(EventType const type,	auto const& func) const
	{
		for (auto const& e : GetEventList())
			if (e->Type() == type)
				func(static_cast<StimulusEvent const*>(e.get()));
	}

protected:
	Model * m_pModel;
};
