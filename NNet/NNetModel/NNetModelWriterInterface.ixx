// NNetModelWriterInterface.ixx
//
// NNetModel

module;

#include <xutility>

export module NNetModel:NNetModelWriterInterface;

import std;
import Debug;
import BoolOp;
import Types;
import Signals;
import D2D_ColorLUT;
import DescriptionUI;
import :NNetModelReaderInterface;
import :NNetParameters;
import :ParamType;
import :SignalGenerator;
import :UPSigGenList;
import :Pipe;
import :NobId;
import :Nob;
import :PosNob;
import :MicroSensor;
import :ScanMatrix;
import :UPSensorList;
import :UPNobList;

export import :NNetModelReaderInterface;

using std::unique_ptr;
using std::wstring;
using std::vector;
using std::move;

export class NNetModelWriterInterface : public NNetModelReaderInterface
{
public:
    unique_ptr<Model> CreateNewModel();

    void  CreateInitialNobs();
    void  SelectNob          (NobId const, bool const);
    void  ToggleStopOnTrigger(NobId const);
    Nob * GetNob             (NobId const);
	void  AddEvent           (EventType const&);
	void  AddEvent           (EventType const&, fMicroSecs const); // For model import
	void  AddStimulusEvent   (fMicroSecs const, SigGenId const);   // For model import

    UPNobList              & GetUPNobs      () { return *m_pModel->m_upNobs.get(); }
    UPSigGenList           & GetSigGenList  () { return *m_pModel->m_upSigGenList.get(); }
    UPSensorList           & GetSensorList  () { return  m_pModel->m_sensorList; }
    NNetParameters         & GetParams      () { return *m_pModel->m_upParam.get(); }
    SignalParameters       & GetSignalParams() { return  m_pModel->m_signalParams; }
    MonitorData            & GetMonitorData () { return  m_pModel->m_monitorData; }
    unique_ptr<vector<Nob*>> GetSelection   () { return  GetUPNobs().GetAllSelected(); }

    SignalGenerator       * GetSigGen          (SigGenId const id)               { return GetSigGenList().GetSigGen(id); }
    SignalGenerator       * GetSigGen          (wstring  const& n)               { return GetSigGenList().GetSigGen(n); }
    UPSigGen                NewSigGen          ()                                { return GetSigGenList().NewSigGen(); }
    UPSigGen                NewSigGen          (wstring const & name)            { return GetSigGenList().NewSigGen(name); }
    SigGenId                PushSigGen         (UPSigGen upSigGen)               { return GetSigGenList().PushSigGen(move(upSigGen)); }
    SigGenId                SetSigGenActive    (SigGenId const id)               { return GetSigGenList().SetActive(id); }
    void                    InsertSigGen       (UPSigGen u, SigGenId const i)    { return GetSigGenList().InsertSigGen(move(u), i); }
    SigGenId                GetSigGenIdSelected()                                { return GetSigGenList().GetSigGenIdSelected(); }
    SignalGenerator       * GetSigGenSelected  ()                                { return GetSigGenList().GetSigGenSelected(); }
    UPSigGen                RemoveSigGen       (SigGenId const id)               { return GetSigGenList().RemoveSigGen(id); }
    UPSigGen                PopSigGen          ()                                { return GetSigGenList().PopSigGen(); }
    SignalNr                AddSignal(unique_ptr<Signal> up, TrackNr  const n)   { return GetMonitorData().AddSignal(move(up), n); }
    void                    AddSignal(unique_ptr<Signal> up, SignalId const id)  { return GetMonitorData().AddSignal(move(up), id); }
    unique_ptr<Signal>      RemoveSignal       (SignalId const& id)              { return GetMonitorData().RemoveSignal(id); }
    RawImage              * GetScanImage       ()                                { return m_pModel->m_upRawImage.get(); }
    MicroSensor           * GetMicroSensor     (NobId const id)                  { return m_pModel->m_microSensorList.GetMicroSensor(id); }
    unique_ptr<MicroSensor> RemoveMicroSensor  (NobId const id)                  { return m_pModel->m_microSensorList.RemoveMicroSensor(id); }
    SensorId                AddSensor          (unique_ptr<Sensor> up)           { return m_pModel->m_sensorList.AddSensor(move(up)); }
    UPSensor                RemoveSensor       (SensorId const id)               { return m_pModel->m_sensorList.RemoveSensor(id); }
    void                    AddSensor(UPSensor up, SensorId const id)            { return m_pModel->m_sensorList.AddSensor(move(up), id); }

    void AddMicroSensor     (unique_ptr<MicroSensor> up)              { m_pModel->m_microSensorList.AddMicroSensor(move(up)); }
    void Reconnect          (NobId const id) const                    { m_pModel->Reconnect(id); }
    void SetParam           (ParamType::Value const p, float const f) { m_pModel->SetParam(p, f); }
    void SetScanArea        (MicroMeterRect const& rect)              { m_pModel->SetScanArea(rect); }
    void SetDescriptionUI   (DescriptionUI & ui)                      { m_pModel->m_description.SetDescriptionUI(ui); }
    void AddDescriptionLine (wstring const & wstr)                    { m_pModel->m_description.AddDescriptionLine(wstr); }
    void DescriptionComplete()                                        { m_pModel->m_description.DescriptionComplete(); }
    void CreateImage        ()                                        { m_pModel->CreateImage(); }
	void ReplaceImage       (unique_ptr<RawImage> up)                 { m_pModel->ReplaceImage(move(up)); }
    void RejectImage        ()                                        { m_pModel->RejectImage(); }
    void ResetModel         ()                                        { m_pModel->ResetModel(); }
    void ClearEventList     ()                                        { m_pModel->m_events.clear(); }
    void ClearDynamicData   ()                                        { m_pModel->ClearDynamicData(); }
    void PrepareScanMatrix  ()                                        { m_pModel->PrepareScanMatrix(); }
    void AddOutgoing        (NobId const id, Pipe* p)                 { GetPosNob(id).AddOutgoing(p); }
    void AddIncoming        (NobId const id, Pipe* p)                 { GetPosNob(id).AddIncoming(p); }
    void DeselectAllNobs    ()                                        { GetUPNobs().DeselectAllNobs(); }
    void ClearScanImage     ()                                        { GetScanImage()->Set(mV::NULL_VAL()); }
    void SetSigGenName      (SigGenId const id, wstring const &n)     { GetSigGenList().SetName(id, n); }
    void InsertTrack        (TrackNr const trackNr)                   { GetMonitorData().InsertTrack(trackNr); }
    void DeleteTrack        (TrackNr const trackNr)                   { GetMonitorData().DeleteTrack(trackNr); }

    PosNob & GetPosNob(NobId const);

    template <Nob_t T>
    T GetNobPtr(NobId const id)
    {
        Nob * const pNob { GetNob(id) };
        return (pNob && HasType<T>(*pNob)) ? static_cast<T>(pNob) : nullptr;
    }

    template <Nob_t T>
    void Apply2All(auto const& func)
    {
        GetUPNobs().Apply2All<T>(func);
    }

    /// RemoveFromModel - ReplaceInModel/Restore2Model: Slot remains

    template <Nob_t OLD>
    unique_ptr<OLD> RemoveFromModel(Nob const & nob)
    {
        return RemoveFromModel<OLD>(nob.GetId());
    }

    template <Nob_t OLD>
    unique_ptr<OLD> RemoveFromModel(NobId const id)
    {
        UPNob upNob { GetUPNobs().ExtractNob(id) };
        auto  pNob  { upNob.release() };
        return unique_ptr<OLD>(static_cast<OLD*>(pNob));
    }

    void Restore2Model(UPNob up)
    {
        if (up)
            GetUPNobs().ReplaceNob(move(up));
    }

    template <Nob_t OLD>
    unique_ptr<OLD> ReplaceInModel(UPNob upNew)
    {
        NobId id      { upNew.get()->GetId() };
        Nob * pNobOld { GetUPNobs().ReplaceNob(move(upNew)) };
        Reconnect(id);
        return unique_ptr<OLD>(static_cast<OLD*>(pNobOld));
    }

    NobId Push2Model(UPNob upNob)
    {
        NobId const id { GetUPNobs().Push(move(upNob)) };
        Reconnect(id);
        return id;
    }

    template <Nob_t T>
    unique_ptr<T> PopFromModel()
    {
        return GetUPNobs().Pop<T>();
    }

    ///////////////////////////////////////////////////////////

    void IncreaseSize(long const nr) { GetUPNobs().IncreaseSize(nr); }
    void ReduceSize  (long const nr) { GetUPNobs().ReduceSize(nr); }

    bool Compute() { return m_pModel->Compute(); }
};
