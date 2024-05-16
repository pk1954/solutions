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

	SignalId                FindSignalId         (NNetSignalSource const * const) const;
	ConnectionType          ConnectionResult     (NobId const, NobId const) const;
	mV                      GetVoltageAt         (NobId const, MicroMeterPnt const&) const;
	mV                      GetVoltage           (NobId const)     const;
	bool                    IsSelected           (NobId const)     const;
	size_t                  GetNrOfSegments      (NobId const)     const;
	size_t                  GetNrOfOutConns      (NobId const)     const;
	size_t                  GetNrOfInConns       (NobId const)     const;
	Degrees                 GetDirection         (NobId const)     const;
	SignalGenerator  const* GetSigGenC           (NobId const)     const;    
	Raster           const& GetScanRaster()                        const { return *m_pModel->m_upRaster.get(); }
	NNetParameters   const& GetParamsC()                           const { return *m_pModel->m_upParam.get(); };
	UPNobList        const& GetUPNobsC()                           const { return *m_pModel->m_upNobs.get(); }
	UPSigGenList     const& GetSigGenList()                        const { return *m_pModel->m_upSigGenList.get(); }
	wstring                 GetTypeName          (NobId const id)  const { return NobType::GetName(GetNobType(id).GetValue()); };
	NobType                 GetNobType           (NobId const id)  const { return m_pModel->GetNobType(id); }
	MicroMeterPnt           GetNobPos            (NobId const id)  const { return m_pModel->GetNobConstPtr<Nob const*>(id)->GetPos(); }
	PosNob           const* GetConstPosNobPtr    (NobId const id)  const { return m_pModel->GetNobConstPtr<PosNob const*>(id); }
	Nob              const* GetConstNob          (NobId const id)  const { return m_pModel->GetConstNob(id); }
	SignalParameters const& GetSignalParamsC()                     const { return m_pModel->m_signalParams; }
	MonitorData      const& GetMonitorDataC()                      const { return m_pModel->m_monitorData; }
	void                    PrintModelSize()                       const { return m_pModel->PrintModelSize(); }
	fMicroSecs              TimeResolution()                       const { return GetParamsC().TimeResolution(); };
	fMicroSecs              PixelScanTime()                        const { return GetParamsC().PixelScanTime(); };
	float                   GetParameter(ParamType::Value const p) const { return GetParamsC().GetParameterValue(p); }
	wstring                 GetModelFilePath()                     const { return m_pModel->m_wstrModelFilePath; }
	bool                    IsNobInModel(Nob const & nob)          const { return m_pModel->GetConstNob(nob.GetId()); }
	unsigned int            GetNrOf(NobType const type)            const { return GetUPNobsC().GetCounter(type); }
	unsigned int            GetNrOfNobs()                          const { return GetUPNobsC().GetCounter(); }
	bool                    AnyNobsSelected()                      const { return GetUPNobsC().AnyNobsSelected(); }
	bool                    IsValidNobId(NobId const id)           const { return GetUPNobsC().IsValidNobId(id); }
	size_t                  GetSizeOfNobList()                     const { return GetUPNobsC().Size(); }
	SignalId                FindSignalId(auto const & crit)        const { return GetMonitorDataC().FindSignalId(crit); }
	SignalGenerator  const* GetSigGenSelectedC()                   const { return GetSigGenList().GetSigGenSelected(); }
	SignalGenerator  const* GetSigGenC(SigGenId const id)          const { return GetSigGenList().GetSigGen(id); }
	SigGenId                GetSigGenIdSelected()                  const { return GetSigGenList().GetSigGenIdSelected(); }
	bool                    IsInList(wstring const & name)         const { return GetSigGenList().IsInList(name); }
	wstring          const& GetSigGenName(SigGenId const id)       const { return GetSigGenList().GetSigGen(id)->GetName(); }
	UPSensorList     const& GetSensorList()                        const { return m_pModel->m_sensorList; }
	Sensor           const* GetSensor(SensorId const id)           const { return GetSensorList().GetSensor(id); }
	MicroMeter              GetScanResolution()                    const { return GetScanRaster().Resolution(); }
	RasterPoint             GetScanAreaSize()                      const { return GetScanRaster().Size(); }
	RasterIndex             GetScanAreaWidth()                     const { return GetScanRaster().RasterWidth(); }
	RasterIndex             GetScanAreaHeight()                    const { return GetScanRaster().RasterHeight(); }
	MicroMeterRect          GetScanArea()                          const { return GetScanRaster().GetScanArea(); }
	MicroMeterRect          GetRasterRect()                        const { return GetScanRaster().GetRasterRect(); }
	RawImage         const* GetScanImageC()                        const { return m_pModel->m_upRawImage.get(); }
	bool                    ModelLocked()                          const { return m_pModel->m_upRawImage.get() != nullptr; }
	int                     GetNrOfScans()                         const { return Cast2Int(GetParameter(ParamType::Value::nrOfScans)); }
	bool                    HasMicroSensor(NobId const id)         const { return m_pModel->m_microSensorList.HasMicroSensor(id); }
	MicroSensor      const* GetMicroSensorC(NobId const id)        const { return m_pModel->m_microSensorList.GetMicroSensorC(id); }
	optional<RasterPoint>   FindRasterPos(MicroMeterPnt const pnt) const { return GetScanRaster().FindRasterPos(pnt); }
	MicroMeterRect          GetPointRect(RasterPoint const& rp)    const { return GetScanRaster().GetPointRect(rp); }
	mV                      Scan(RasterPoint const& rp)            const { return m_pModel->m_scanMatrix.Scan(rp); }
    SigGenId                FindSigGen(wstring  const& n)          const { return GetSigGenList().FindSigGen(n); }
    bool                    IsValid   (SigGenId const id)          const { return GetSigGenList().IsValid(id); }
	void                    DensityCorrection(RawImage& image)     const { m_pModel->m_scanMatrix.DensityCorrection(image); }
	void                    Apply2AllTimestamps(auto const& func)  const { m_pModel->m_timestamps.Apply2All(func); }
	fMicroSecs              TotalScanTime()                        const;

	bool IsInputLine(NobId const id) const
	{
		Nob const* const pNob { GetConstNob(id) };
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

	void DrawExterior(NobId      const,  DrawContext const&, tHighlight const) const;
	void DrawInterior(NobId      const,  DrawContext const&, tHighlight const) const;
	void DrawLine(MicroMeterLine const&, DrawContext const&)                   const;

	void DrawScanAreaBackground(DrawContext const&) const; 
	void DrawScanRaster        (DrawContext const&) const;
    void DrawScanAreaHandles   (DrawContext const&, optional<CardPoint> const) const;
    void DrawSensorDensityMap  (DrawContext const&) const;
    void DrawScanImage         (DrawContext const&, RawImage const*, mV const, ColorLUT const&) const;
	void DrawScanProgress      (DrawContext const&, RasterPoint const& rpProgress) const;

	optional<CardPoint> SelectScanAreaHandle(DrawContext const&, MicroMeterPnt const&) const;

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

	void Apply2allEvents(EventType const type, auto const& func) const
	{
		for (auto const& e : m_pModel->m_events)
			if (e->Type() == type)
				func(static_cast<StimulusEvent const*>(e.get()));
	}

	void Apply2allEvents(auto const& func) const
	{
		for (auto const& e : m_pModel->m_events)
			func(e.get());
	}

protected:
	Model * m_pModel;
};
