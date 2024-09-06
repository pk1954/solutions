// Model.ixx
//
// NNetModel

export module NNetModel:Model;

import std;
import Debug;
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

	static void Initialize(Observable * const);   // staticModelObservable

	Model();
	~Model(); 
	Model(const Model&)            = delete; // copy constructor
	Model(Model&&)                 = delete; // move constructor
	Model& operator=(const Model&) = delete; // copy assignment
	Model& operator=(Model&&)      = delete; // move assignment

    void Notify(bool const) final {	PrepareScanMatrix(); }
	
	template <Nob_t T>
	T GetNobConstPtr(NobId const id) const
	{
		Nob const* const pNob { GetConstNob(id) };
		return (pNob && HasType<T>(*pNob)) ? static_cast<T>(pNob) : nullptr;
	}

	Nob    const* GetConstNob   (NobId const) const;
	PosNob const* GetStartNobPtr(NobId const) const;
	PosNob const* GetEndNobPtr  (NobId const) const;

	Nob   * GetNob(NobId const);
	NobType GetNobType(NobId const) const;
	bool    IsConnectionCandidate(NobId const, NobId const) const;
	void    CheckModel() const;
	void    CheckId(NobId const) const;
	void    DumpModel(char const* const, int const) const;
	NobId   GetStartKnotId(NobId const) const;
	NobId   GetEndKnotId  (NobId const) const;
	bool    GetDescriptionLine(int const, wstring&) const;
	void    PrintModelSize() const;
	void    ClearDynamicData();
	bool    Compute();
	void    ResetModel();
	void    SetParam(ParamType::Value const, float const);
	void    Reconnect(NobId const);
	void    SetScanArea(MicroMeterRect const&);
	void    CreateImage();
	void    ReplaceImage(unique_ptr<RawImage>);
    void    PrepareScanMatrix();
	void    RejectImage();

	optional<CardPoint> SelectScanAreaHandle(DrawContext const&, MicroMeterPnt const&) const;

	EventList                  m_events;
	unique_ptr<ScanMatrix>     m_upScanMatrix;
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
