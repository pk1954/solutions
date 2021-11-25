// NNetModelWriterInterface.h
//
// NNetModel

#pragma once

#include <string>
#include "BoolOp.h"
#include "ParameterType.h"
#include "MoreTypes.h"
#include "IoNeuron.h"
#include "NobId.h"
#include "NNetModel.h"
#include "NNetModelReaderInterface.h"

class Pipe;
class BaseKnot;
class MicroMeterPntVector;

using std::unique_ptr;
using std::move;

#define DUMP() DumpModel(__FILE__, __LINE__)

class NNetModelWriterInterface : public NNetModelReaderInterface
{
public:
    void        CreateInitialNobs();
    void        RemoveOrphans();
    void        SelectBeepers();
    void        SelectNob(NobId const, bool const);
    void        ToggleStopOnTrigger(NobId const);
    Nob * const GetNob(NobId const);

    UPNobList               & GetUPNobs()      { return m_pModel->GetUPNobs(); }
    Param                   & GetParams()      { return m_pModel->GetParams(); }
    MonitorData             & GetMonitorData() { return m_pModel->GetMonitorData(); }
    unique_ptr<vector<Nob *>> GetSelection()   { return move(GetUPNobs().GetAllSelected()); }

    void  ResetModel      () { m_pModel->ResetModel(); }
    void  ClearDynamicData() { m_pModel->ClearDynamicData(); }

    void  Reconnect(NobId const id) { m_pModel->Reconnect(id); }

    void  SelectSubtree(BaseKnot  * const p, bool  const b) { m_pModel->SelectSubtree(p, b); }
    float SetParam(ParamType::Value const p, float const f) { return m_pModel->SetParam(p, f); }

    void  SetModelFilePath  (wstring const & wstr) { m_pModel->SetModelFilePath  (wstr); }
    void  AddDescriptionLine(wstring const & wstr) { m_pModel->AddDescriptionLine(wstr); }
    void  DescriptionComplete()                    { m_pModel->DescriptionComplete(); }
    void  DeselectAllNobs()                        { m_pModel->DeselectAllNobs(); }

    template <Nob_t T>
    T GetNobPtr(NobId const id) 
    {
        Nob * const pNob { GetNob(id) };
        return (pNob && HasType<T>(*pNob)) ? static_cast<T>(pNob) : nullptr;
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

    template <Nob_t NEW>
    void Restore2Model(unique_ptr<NEW> up) 
    {
        assert(up);
        GetUPNobs().ReplaceNob(move(up));
    }

    template <Nob_t NEW, Nob_t OLD>
    unique_ptr<OLD> ReplaceInModel(unique_ptr<NEW> upNew) 
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

    void SetPosDir(NobId const, MicroMeterPosDir const &);
}; 
