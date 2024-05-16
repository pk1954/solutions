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

	Nob    const * GetConstNob   (NobId const) const;
	PosNob const * GetStartNobPtr(NobId const) const;
	PosNob const * GetEndNobPtr  (NobId const) const;

	NobId GetStartKnotId(NobId const) const;
	NobId GetEndKnotId  (NobId const) const;

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

	// non const functions

	Nob * GetNob(NobId const);

	bool Compute();
	void ResetModel();
	void SetParam(ParamType::Value const, float const);
	void Reconnect(NobId const);
	void SetScanArea(MicroMeterRect const&);
	void CreateImage();
	void ReplaceImage(unique_ptr<RawImage>);
	void RejectImage();

	optional<CardPoint> SelectScanAreaHandle(DrawContext const&, MicroMeterPnt const&) const;

    void PrepareScanMatrix() { m_scanMatrix.Prepare(*m_upRaster.get(), *m_upNobs.get()); }

	EventList                  m_events;
	ScanMatrix                 m_scanMatrix;
	unique_ptr<Raster>         m_upRaster;
	unique_ptr<NNetParameters> m_upParam;
	unique_ptr<UPSigGenList>   m_upSigGenList;
	unique_ptr<UPNobList>      m_upNobs;
	unique_ptr<RawImage>       m_upRawImage;
	UPMicroSensorList          m_microSensorList;
	ModelDescription           m_description;
	TimestampList              m_timestamps;
	wstring                    m_wstrModelFilePath;
	UPSensorList               m_sensorList;
	MonitorData                m_monitorData;
	SignalParameters           m_signalParams;

private:
	unsigned int printNobType(unsigned int, NobType::Value) const;
	bool isConnectedToPipe(NobId const, NobId const) const;
	bool isConnectedTo    (NobId const, NobId const) const;

	inline static Observable* m_pLockModelObservable   { nullptr };
	inline static Observable* m_pStaticModelObservable { nullptr };
};
