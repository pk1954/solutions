// Model.ixx
//
// NNetModel

module;

#include <cassert>
#include <memory>
#include <string>
#include <vector>
#include <algorithm>

export module NNetModel:Model;

import Observable;
import Util;
import Raster;
import Signals;
import BoolType;
import Vector2D;
import ColorLUT;
import DescriptionUI;
import :ParamType;
import :ModelDescription;
import :UPNobList;
import :UPSigGenList;
import :UPSensorList;
import :UPMicroSensorList;
import :PosNob;

using std::unique_ptr;
using std::make_unique;
using std::wstring;
using std::vector;
using std::move;

export using ScanImageRaw  = Vector2D<mV>;
export using ScanImageByte = Vector2D<ColIndex>;

export class Model
{
public:

	static void Initialize(Observable * const);

	Model();

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

	UPSigGenList      const & GetSigGenList     ()         const { return *m_upSigGenList.get(); }
	UPSigGenList            & GetSigGenList     ()               { return *m_upSigGenList.get(); }
	UPSensorList      const & GetSensorList     ()         const { return m_sensorList; }
	UPSensorList            & GetSensorList     ()               { return m_sensorList; }
	UPMicroSensorList const & GetMicroSensorList()         const { return m_microSensorList; }
	UPMicroSensorList       & GetMicroSensorList()               { return m_microSensorList; }
	UPNobList         const & GetUPNobs         ()         const { return *m_upNobs.get(); }
	UPNobList               & GetUPNobs         ()               { return *m_upNobs.get(); }
	unique_ptr<UPNobList>     MoveUPNobs        ()               { return move(m_upNobs); }
	MonitorData       const & GetMonitorData    ()         const { return m_monitorData; }
	MonitorData             & GetMonitorData    ()               { return m_monitorData; }
	NNetParameters    const & GetParams         ()         const { return *m_upParam.get(); }
	NNetParameters          & GetParams         ()               { return *m_upParam.get(); }
	SignalParameters  const & GetSignalParams   ()         const { return m_signalParams; }
	SignalParameters        & GetSignalParams   ()               { return m_signalParams; }
	MicroMeterRect            GetScanAreaRect   ()         const { return m_upRaster->GetRasterRect(); }
	SignalGenerator   const * GetSigGen(SigGenId const id) const { return m_upSigGenList->GetSigGen(id); } 
	MicroMeter                GetScanResolution()          const { return m_upRaster->Resolution(); }
	RasterPoint               GetScanAreaSize()            const { return m_upRaster->Size(); }
	Raster            const & GetScanRaster()              const { return *m_upRaster.get(); }
	ScanImageByte     const * GetScanImageC()              const { return m_upImage.get(); }
	ScanImageByte           * GetScanImage()                     { return m_upImage.get(); }

	// non const functions

	Nob * GetNob(NobId const);

	void PrintModelSize() const;
	bool Compute();
	void ResetModel();
	void SetParam(ParamType::Value const, float const);
	void Reconnect(NobId const);
	void SetScanArea(MicroMeterRect const&);
	void CreateRawScanImage();
	void ReplaceScanImage(unique_ptr<ScanImageByte>);
	void RejectScanImage();

	void DeselectAllNobs          ()               const { m_upNobs->DeselectAllNobs(); }
	void SetModelFilePath         (wstring const & wstr) { m_wstrModelFilePath = wstr; }
	void AddDescriptionLine       (wstring const & wstr) { m_description.AddDescriptionLine(wstr); }
	void DescriptionComplete      ()                     { m_description.DescriptionComplete(); }
	void SetDescriptionUI         (DescriptionUI &i)     { m_description.SetDescriptionUI(i); }
	void SetHighSigObservable     (Observable    &o)     { m_monitorData.SetHighSigObservable(o); }
	void SetActiveSigGenObservable(Observable    &o)     { m_upSigGenList->SetActiveSigGenObservable(o); }

private:
	unsigned int printNobType(unsigned int, NobType::Value) const;
	bool isConnectedToPipe(NobId const, NobId const) const;
	bool isConnectedTo    (NobId const, NobId const) const;

	inline static Observable * m_pLockModelObservable = nullptr;

	unique_ptr<UPNobList>      m_upNobs;
	unique_ptr<UPSigGenList>   m_upSigGenList;
	unique_ptr<NNetParameters> m_upParam;
	unique_ptr<ScanImageByte>  m_upImage;
	unique_ptr<Raster>         m_upRaster;
	SignalParameters           m_signalParams;
	UPSensorList               m_sensorList;
	UPMicroSensorList          m_microSensorList;
	ModelDescription           m_description;
	MonitorData                m_monitorData;
	wstring                    m_wstrModelFilePath;
};
