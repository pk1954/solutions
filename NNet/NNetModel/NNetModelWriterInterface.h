// NNetModelWriterInterface.h
//
// NNetModel

#pragma once

#include <string>
#include "boolOp.h"
#include "ParameterType.h"
#include "MoreTypes.h"
#include "ShapeId.h"
#include "NNetModel.h"

class Pipe;
class BaseKnot;
class ShapeErrorHandler;

using std::unique_ptr;
using std::move;

class NNetModelWriterInterface
{
public:
	void Start( NNetModel * const );
	void Stop(); 

    void          CreateInitialShapes();
    void          RemoveOrphans( );
    void          SelectBeepers();
    void          SelectShape( ShapeId const, tBoolOp const );
    Shape * const GetShape( ShapeId const );

    ShapeList & GetShapes() { return m_pModel->GetShapes(); }
    Param     & GetParams() { return m_pModel->GetParams(); }

    void              DumpModel( ) const { m_pModel->DumpModel(); }
    NNetModel const & GetModel( )  const { return * m_pModel; }  // TODO: find better solution

    bool const IsPipe( ShapeId const id )
    {
        Shape * pShape { GetShapePtr<Shape *>( id ) };
        return pShape && pShape->IsPipe( );
    }

    bool const IsKnot( ShapeId const id )
    {
        Shape * pShape { GetShapePtr<Shape *>( id ) };
        return pShape && pShape->IsKnot( );
    }

    template <Shape_t T>
    T GetShapePtr( ShapeId const id ) 
    {
        Shape * const pShape { GetShape( id ) };
        return (pShape && HasType<T>( * pShape )) ? static_cast<T>( pShape ) : nullptr;
    }

    MonitorData & GetMonitorData( ) { return m_pModel->GetMonitorData(); }

    void CheckModel( ) { m_pModel->CheckModel(); }
    void ResetModel( ) { m_pModel->ResetModel(); }
    void ClearModel( ) { m_pModel->GetShapes().Apply2All( [&](Shape & s) { s.Clear( ); } ); }

    template <Shape_t NEW, Shape_t OLD>
    unique_ptr<OLD> ReplaceInModel( unique_ptr<NEW> up ) 
    {
        Shape * pShape { m_pModel->GetShapes().ReplaceShape( up.get()->GetId(), move(up) ) }; 
        return move( unique_ptr<OLD>( static_cast<OLD*>(pShape) ) );
    }

    template <Shape_t OLD>
    unique_ptr<OLD> RemoveFromModel( ShapeId const id ) 
    { 
        Shape * pShape { m_pModel->GetShapes().RemoveShape( id ) }; 
        return move( unique_ptr<OLD>( static_cast<OLD*>(pShape) ) );
    }

    MicroMeterPoint const OrthoVector( ShapeId const idPipe ) const
    {
        return ::OrthoVector( m_pModel->GetShapeConstPtr<Pipe const *>(idPipe)->GetVector(), NEURON_RADIUS*2.f );
    }

    void SelectSubtree(BaseKnot* const p, tBoolOp const o) { m_pModel->SelectSubtree(p, o); }

    float SetParam(ParameterType::Value const p, float const f) { return m_pModel->SetParam(p, f); }

    void SetModelFilePath  ( wstring const wstr ) { m_pModel->SetModelFilePath  ( wstr ); }
    void AddDescriptionLine( wstring const wstr ) { m_pModel->AddDescriptionLine( wstr ); }

    void ToggleStopOnTrigger( ShapeId const );

private:

    NNetModel * m_pModel { nullptr };
}; 
