// Model.ixx
//
// NNetModel

module;

#include <cassert>
#include <ctime>
#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include <optional>

export module NNetModel:Model;

import Observable;
import ObserverInterface;
import Types;
import Util;
import Raster;
import Signals;
import BoolType;
import Vector2D;
import ColorLUT;
import DescriptionUI;
import TimestampList;
import :ParamType;
import :ModelDescription;
import :UPNobList;
import :UPSigGenList;
import :UPSensorList;
import :UPMicroSensorList;
import :NNetEvent;
import :ScanMatrix;
import :PosNob;

using std::unique_ptr;
using std::make_unique;
using std::wstring;
using std::vector;
using std::move;
using std::optional;

export using EventList = vector<unique_ptr<NNetEvent>>;

export class Model : public ObserverInterface
{
public:

	static void Initialize
	(
		Observable * const,  // lockModelObservable
		Observable * const   // staticModelObservable
	);

	Model();
	~Model(); 
	Model(const Model&)            = delete; // copy constructor
	Model(Model&&)                 = delete; // move constructor
	Model& operator=(const Model&) = delete; // copy assignment
	Model& operator=(Model&&)      = delete; // move assignment

    void Notify(bool const) final;

	// const functions

	template <Nob_t T>
	T GetNobConstPtr(NobId const id) const
	{
		Nob const* const pNob { GetConstNob(id) };
		return (pNob && HasType<T>(*pNob)) ? static_cast<T>(pNob) : nullptr;
	}

	NobType GetNobType(NobId const id) const
	{
		auto p { GetNobConstPtr<Nob const *>(id) };
		return p ? p->GetNobType() : NobType::Value::undefined; 
	}

	bool IsConnectionCandidate(NobId const, NobId const) const;

	void CheckModel() const;
	void CheckId(NobId const) const;
	void DumpModel(char const* const, int const) const;

	float GetParameter(ParamType::Value const p) const { return m_upParam->GetParameterValue(p); }

	Nob    const * GetConstNob   (NobId const) const;
	PosNob const * GetStartNobPtr(NobId const) const;
	PosNob const * GetEndNobPtr  (NobId const) const;

	NobId GetStartKnotId(NobId const) const;
	NobId GetEndKnotId  (NobId const) const;

	wstring GetModelFilePath() const { return m_wstrModelFilePath; }
	size_t  Size()             const { return m_upNobs->Size(); }

	bool  GetDescriptionLine(int const, wstring&) const;

	void PrintModelSize() const;

	NobId ModelFindNobAt(MicroMeterPnt const& umPoint, auto const& crit) const // TODO: Template!
	{
		NobId idRes{ NO_NOB };

		idRes = m_upNobs->FindNobAt(umPoint, [&crit](Nob const& s) { return s.IsIoConnector() && crit(s); });
		if (IsDefined(idRes))
			return idRes;

		idRes = m_upNobs->FindNobAt(umPoint, [&crit](Nob const& s) { return s.IsNeuron() && crit(s); });
		if (IsDefined(idRes))
			return idRes;

		idRes = m_upNobs->FindNobAt(umPoint, [&crit](Nob const& s) { return s.IsIoLine() && (!s.HasParentNob()) && crit(s); });
		if (IsDefined(idRes))
			return idRes;

		idRes = m_upNobs->FindNobAt(umPoint, [&crit](Nob const& s) { return s.IsKnot() && crit(s); });
		if (IsDefined(idRes))
			return idRes;

		idRes = m_upNobs->FindNobAt(umPoint, [&crit](Nob const& s) { return s.IsSynapse() && crit(s); });
		if (IsDefined(idRes))
			return idRes;

		idRes = m_upNobs->FindNobAt(umPoint, [&crit](Nob const& s) { return s.IsFork() && crit(s); });
		if (IsDefined(idRes))
			return idRes;

		idRes = m_upNobs->FindNobAt(umPoint, [&crit](Nob const& s) { return s.IsPipe() && crit(s); });
		if (IsDefined(idRes))
			return idRes;

		return NO_NOB;
	}

	// functions affecting Nob data, not model data

	void ClearDynamicData();

	// access functions to members 

	UPSigGenList      const & GetSigGenList     () const { return *m_upSigGenList.get(); }
	UPSensorList      const & GetSensorList     () const { return m_sensorList; }
	UPMicroSensorList const & GetMicroSensorList() const { return m_microSensorList; }
	UPNobList         const & GetUPNobs         () const { return *m_upNobs.get(); }
	MonitorData       const & GetMonitorData    () const { return m_monitorData; }
	NNetParameters    const & GetParams         () const { return *m_upParam.get(); }
	SignalParameters  const & GetSignalParams   () const { return m_signalParams; }
	MicroMeterRect            GetScanArea       () const { return m_upRaster->GetScanArea(); }
	MicroMeterRect            GetRasterRect     () const { return m_upRaster->GetRasterRect(); }
	MicroMeter                GetScanResolution () const { return m_upRaster->Resolution(); }
	RasterPoint               GetScanAreaSize   () const { return m_upRaster->Size(); }
	RasterIndex               GetScanAreaWidth  () const { return m_upRaster->RasterWidth(); }
	RasterIndex               GetScanAreaHeight () const { return m_upRaster->RasterHeight(); }
	Raster            const & GetScanRaster     () const { return *m_upRaster.get(); }
	ScanImageByte     const * GetScanImageC     () const { return m_upImage.get(); }
	EventList         const & GetEventList      () const { return m_events; }

	mV   Scan(RasterPoint const& rp)            const { return m_scanMatrix.Scan(rp); }
	void DensityCorrection(ScanImageRaw& image) const { m_scanMatrix.DensityCorrection(image); }
	void DeselectAllNobs  ()                    const { m_upNobs->DeselectAllNobs(); }

	SignalGenerator const * GetSigGen   (SigGenId const id)    const { return m_upSigGenList->GetSigGen(id); } 
	time_t                  GetTimestamp(wstring  const &name) const { return m_timestamps.GetTimestamp(name); }

	void Apply2AllTimestamps (auto const& f)  const { m_timestamps.Apply2All(f); }

	// non const functions

	Nob * GetNob(NobId const);

	ScanImageByte       * GetScanImage      () { return m_upImage.get(); }
	UPSigGenList        & GetSigGenList     () { return *m_upSigGenList.get(); }
	UPSensorList        & GetSensorList     () { return m_sensorList; }
	UPMicroSensorList   & GetMicroSensorList() { return m_microSensorList; }
	UPNobList           & GetUPNobs         () { return *m_upNobs.get(); }
	MonitorData         & GetMonitorData    () { return m_monitorData; }
	NNetParameters      & GetParams         () { return *m_upParam.get(); }
	SignalParameters    & GetSignalParams   () { return m_signalParams; }
	unique_ptr<UPNobList> MoveUPNobs        () { return move(m_upNobs); }

	bool Compute();
	void ResetModel();
	void SetParam(ParamType::Value const, float const);
	void Reconnect(NobId const);
	void SetScanArea(MicroMeterRect const&);
	void CreateScanImage();
	void ReplaceScanImage(unique_ptr<ScanImageByte>);
	void RejectScanImage();
    void DrawScanAreaBackground(DrawContext const&) const;
    void DrawScanArea
	(
		DrawContext const&, 
		ColorLUT const&, 
		bool const,
	    optional<CardPoint> const
	) const;

	optional<CardPoint> SelectScanAreaHandle(DrawContext const&, MicroMeterPnt const&) const;

	void SetTimestamp       (wstring const &s, time_t const t) { m_timestamps.SetTimestamp(s, t); }
	void SetTimestamp       (wstring const &s)                 { m_timestamps.SetTimestamp(s, 0); }
    void PrepareScanMatrix  ()                                 { m_scanMatrix.Prepare(*m_upRaster.get(), *m_upNobs.get()); }
	void SetModelFilePath   (wstring const &s)                 { m_wstrModelFilePath = s; }
	void AddDescriptionLine (wstring const &s)                 { m_description.AddDescriptionLine(s); }
	void DescriptionComplete()                                 { m_description.DescriptionComplete(); }
	void SetDescriptionUI   (DescriptionUI &i)                 { m_description.SetDescriptionUI(i); }

	void SetActiveSigGenObservable(Observable &o) { m_upSigGenList->SetActiveSigGenObservable(o); }

	EventList m_events;

private:
	unsigned int printNobType(unsigned int, NobType::Value) const;
	bool isConnectedToPipe(NobId const, NobId const) const;
	bool isConnectedTo    (NobId const, NobId const) const;

	inline static Observable* m_pLockModelObservable   { nullptr };
	inline static Observable* m_pStaticModelObservable { nullptr };

	unique_ptr<UPNobList>      m_upNobs;
	unique_ptr<UPSigGenList>   m_upSigGenList;
	unique_ptr<NNetParameters> m_upParam;
	unique_ptr<Raster>         m_upRaster;
	unique_ptr<ScanImageByte>  m_upImage;
	SignalParameters           m_signalParams;
	UPSensorList               m_sensorList;
	UPMicroSensorList          m_microSensorList;
	ModelDescription           m_description;
	MonitorData                m_monitorData;
	wstring                    m_wstrModelFilePath;
	ScanMatrix                 m_scanMatrix;
	TimestampList              m_timestamps;
};
