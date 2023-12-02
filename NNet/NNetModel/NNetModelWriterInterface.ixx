// NNetModelWriterInterface.ixx
//
// NNetModel

module;

#include <memory>
#include <string>
#include <cassert>
#include <vector>
#include <xutility>

export module NNetModel:NNetModelWriterInterface;

import BoolOp;
import Types;
import Signals;
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

    UPNobList              & GetUPNobs      () { return m_pModel->GetUPNobs(); }
    NNetParameters         & GetParams      () { return m_pModel->GetParams(); }
    SignalParameters       & GetSignalParams() { return m_pModel->GetSignalParams(); }
    MonitorData            & GetMonitorData () { return m_pModel->GetMonitorData(); }
    unique_ptr<vector<Nob*>> GetSelection   () { return GetUPNobs().GetAllSelected(); }

    void ResetModel()             { m_pModel->ResetModel(); }
    void ClearDynamicData() const { m_pModel->ClearDynamicData(); }

    SigGenId          FindSigGen       (wstring  const & name)   const { return m_pModel->GetSigGenList().FindSigGen(name); }
    bool              IsValid          (SigGenId const   id)     const { return m_pModel->GetSigGenList().IsValid(id); }
    UPSigGen          NewSigGen        ()                              { return m_pModel->GetSigGenList().NewSigGen(); }
    UPSigGen          NewSigGen        (wstring const & name)          { return m_pModel->GetSigGenList().NewSigGen(name); }
    SigGenId          PushSigGen       (UPSigGen upSigGen)             { return m_pModel->GetSigGenList().PushSigGen(move(upSigGen)); }
    SigGenId          SetSigGenActive  (SigGenId const id)             { return m_pModel->GetSigGenList().SetActive(id); }
    void              InsertSigGen     (UPSigGen u, SigGenId const i)  { return m_pModel->GetSigGenList().InsertSigGen(move(u), i); }
    SigGenId          GetSigGenIdSelected()                            { return m_pModel->GetSigGenList().GetSigGenIdSelected(); }
    SignalGenerator * GetSigGenSelected()                              { return m_pModel->GetSigGenList().GetSigGenSelected(); }
    SignalGenerator * GetSigGen        (SigGenId const id)       const { return m_pModel->GetSigGenList().GetSigGen(id); }
    SignalGenerator * GetSigGen        (wstring const& name)     const { return m_pModel->GetSigGenList().GetSigGen(name); }
    UPSigGen          RemoveSigGen     (SigGenId const id)             { return m_pModel->GetSigGenList().RemoveSigGen(id); }
    UPSigGen          PopSigGen        ()                              { return m_pModel->GetSigGenList().PopSigGen(); }
    ScanImage       * GetScanImage     ()                              { return m_pModel->GetScanImage(); }

    UPSigGenList & GetSigGenList() { return m_pModel->GetSigGenList(); }
    UPSensorList & GetSensorList() { return m_pModel->GetSensorList(); }

    void SetSigGenName (SigGenId const id, wstring const &n)     { GetSigGenList().SetName(id, n); }
    void Reconnect     (NobId const id) const                    { m_pModel->Reconnect(id); }
    void SetParam      (ParamType::Value const p, float const f) { m_pModel->SetParam(p, f); }
    void SetScanArea   (MicroMeterRect const& rect)              { m_pModel->SetScanArea(rect); }
    void SetScanRunning(bool const b)                            { m_pModel->SetScanRunning(b); }

    void SetDescriptionUI   (DescriptionUI & ui)       { m_pModel->SetDescriptionUI(ui); }
    void SetModelFilePath   (wstring const & wstr)     { m_pModel->SetModelFilePath(wstr); }
    void AddDescriptionLine (wstring const & wstr)     { m_pModel->AddDescriptionLine(wstr); }
    void DescriptionComplete()                         { m_pModel->DescriptionComplete(); }
    void DeselectAllNobs    () const                   { m_pModel->DeselectAllNobs(); }
    void CreateImage        ()                         { m_pModel->CreateImage(); }
    void RejectImage        ()                         { m_pModel->RejectImage(); }
	void SetScanImage       (unique_ptr<ScanImage> up) { m_pModel->SetScanImage(move(up)); }

    void AddOutgoing(NobId const, Pipe *);
    void AddIncoming(NobId const, Pipe *);

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
        return move(unique_ptr<OLD>(static_cast<OLD*>(pNob)));
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
        return move(unique_ptr<OLD>(static_cast<OLD*>(pNobOld)));
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
        return move(GetUPNobs().Pop<T>());
    }

    ///////////////////////////////////////////////////////////

    void IncreaseSize(long const nr) { GetUPNobs().IncreaseSize(nr); }
    void ReduceSize  (long const nr) { GetUPNobs().ReduceSize(nr); }

    bool Compute() { return m_pModel->Compute(); }
};
