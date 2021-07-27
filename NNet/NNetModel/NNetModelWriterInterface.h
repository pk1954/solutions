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

class Pipe;
class BaseKnot;
class NobErrorHandler;
class MicroMeterPntVector;

using std::unique_ptr;
using std::move;

#define DUMP() DumpModel(__FILE__, __LINE__)

class NNetModelWriterInterface
{
public:
	void        Start(NNetModel * const);
	void        Stop(); 
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

    void CheckModel() { m_pModel->CheckModel(); }
    void ResetModel() { m_pModel->ResetModel(); }
    void ClearModel() { m_pModel->GetUPNobs().Apply2All([&](Nob & s) { s.Clear(); }); }

    void DumpModel(char const * const file, int const line) const { m_pModel->DumpModel(file, line); }

    void  SelectSubtree(BaseKnot  * const p, bool  const b) { m_pModel->SelectSubtree(p, b); }
    float SetParam(ParamType::Value const p, float const f) { return m_pModel->SetParam(p, f); }
    void  SetModelFilePath  ( wstring const wstr ) { m_pModel->SetModelFilePath  ( wstr ); }
    void  AddDescriptionLine( wstring const wstr ) { m_pModel->AddDescriptionLine( wstr ); }
    void  DescriptionComplete()                    { m_pModel->DescriptionComplete(); }

    wstring const GetModelFilePath() { return m_pModel->GetModelFilePath(); }

    bool const IsConnector( NobId const id )
    {
        Nob * pNob { GetNobPtr<Nob *>( id ) };
        return pNob && pNob->IsConnector();
    }

    bool const IsPipe( NobId const id )
    {
        Nob * pNob { GetNobPtr<Nob *>( id ) };
        return pNob && pNob->IsPipe();
    }

    bool const IsKnot( NobId const id )
    {
        Nob * pNob { GetNobPtr<Nob *>( id ) };
        return pNob && pNob->IsKnot();
    }

    template <Nob_t T>
    T GetNobPtr( NobId const id ) 
    {
        Nob * const pNob { GetNob( id ) };
        return (pNob && HasType<T>( * pNob )) ? static_cast<T>( pNob ) : nullptr;
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
        UPNob upNob { m_pModel->GetUPNobs().ExtractNob(id) }; 
        auto  pNob  { upNob.release() };
        wcout << L"RemoveFromModel " << id << endl;
        return move( unique_ptr<OLD>( static_cast<OLD*>(pNob) ) );
    }

    Nob * Restore2Model(unique_ptr<Nob>);

    template <Nob_t NEW, Nob_t OLD>
    unique_ptr<OLD> ReplaceInModel(unique_ptr<NEW> up) 
    {
        Nob  * pNobOld { Restore2Model(move(up)) };
        return move( unique_ptr<OLD>(static_cast<OLD*>(pNobOld)) );
    }

    /// Push2Model - PopFromModel

    NobId const Push2Model(UPNob upNob)
    {
        NobId const id { m_pModel->GetUPNobs().Push(move(upNob)) };
        Reconnect(id);
        return id;
    }

    template <Nob_t T>
    unique_ptr<T> PopFromModel() 
    { 
        return m_pModel->GetUPNobs().Pop<T>();
    }

    ///////////////////////////////////////////////////////////

    void Reconnect(NobId const id)
    {
        m_pModel->GetUPNobs().GetAt(id)->Reconnect();
    }

    void IncreaseSize(long const nr) { m_pModel->GetUPNobs().IncreaseSize(nr); }
    void ReduceSize  (long const nr) { m_pModel->GetUPNobs().ReduceSize(nr); }

    MicroMeterPnt const OrthoVector( NobId const idPipe ) const
    {
        MicroMeterPnt vector { m_pModel->GetNobConstPtr<Pipe const *>(idPipe)->GetVector() };
        return vector.OrthoVector().ScaledTo(NEURON_RADIUS*2.f);
    }

    void SetIoNeurons(MicroMeterPntVector const &, NobIdList    const &);
    void SetIoNeurons(MicroMeterPntVector const &, IoNeuronList const &);

    void SetPosDir(NobId const id, MicroMeterPosDir const& umPosDir)
    {
        GetNobPtr<Nob *>(id)->SetPosDir(umPosDir);
    }

#ifdef _DEBUG
    NNetModel const & GetModel()  const { return * m_pModel; }  // TODO: find better solution
#endif
private:

    NNetModel * m_pModel { nullptr };
}; 
