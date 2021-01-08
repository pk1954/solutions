// NNetModelWriterInterface.h
//
// NNetModel

#pragma once

#include <string>
#include "boolOp.h"
#include "tParameter.h"
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

    void DumpModel( ) const { m_pModel->DumpModel(); }

    void CreateInitialShapes();

    ShapeList & GetShapes() { return m_pModel->GetShapes(); }
    Param     & GetParams() { return m_pModel->GetParams(); }

    NNetModel const & GetModel( )           const { return * m_pModel; }  // TODO: find better solution
    size_t    const   GetSizeOfShapeList( ) const { return m_pModel->GetSizeOfShapeList( ); }

    Shape * const GetShape( ShapeId const );

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

    void SelectBeepers();
    void SelectShape( ShapeId const, tBoolOp const );
    void StaticModelChanged( );

    template <Shape_t T>
    T GetShapePtr( ShapeId const id ) 
    {
        Shape * const pShape { GetShape( id ) };
        return (pShape && HasType<T>( * pShape )) ? static_cast<T>( pShape ) : nullptr;
    }

    template <Shape_t T>
	void Apply2All( function<void(T &)> const & func ) const
	{
        m_pModel->GetShapes().Apply2All
        ( 
            [&](Shape & s) 
            {  
                if ( HasType<T>(s) ) 
                    func( static_cast<T &>(s) ); 
            }
        );
	}                        

    template <Shape_t T>
    void Apply2AllSelected( function<void(T &)> const & func ) const
    {
    	Apply2All<T>( {	[&](T & s) { if ( s.IsSelected() ) { func( s ); } } } );
    }

    template <Shape_t T>
    void Apply2AllInRect( MicroMeterRect const & r, function<void(T &)> const & func )
    {
        Apply2All<T>( [&](T & s) { if ( s.IsInRect(r) ) { func( s ); } } );
    }

    MonitorData & GetMonitorData( ) { return m_pModel->GetMonitorData(); }

    void CheckModel( ) { m_pModel->CheckModel(); }
    void ResetModel( ) { m_pModel->ResetModel(); }
    void ClearModel( ) 
    {
        m_pModel->GetShapes().Apply2All( [&](Shape & s) { s.Clear( ); } ); 
    }

    ShapeId const Push2Model( UPShape up ) 
    { 
        return m_pModel->Push2Model( move(up) ); 
    }

    template <Shape_t T>
    unique_ptr<T> PopFromModel( ) { return m_pModel->PopFromModel<T>(); }

    template <Shape_t NEW, typename OLD>
    unique_ptr<OLD> ReplaceInModel( unique_ptr<NEW> up ) 
    { 
        return m_pModel->ReplaceInModel<NEW, OLD>( move(up) ); 
    }

    template <Shape_t T>
    unique_ptr<T> RemoveFromModel( ShapeId const id ) 
    { 
        return m_pModel->RemoveFromModel<T>( id ); 
    }

    MicroMeterPoint const OrthoVector( ShapeId const idPipe ) const
    {
        return ::OrthoVector( m_pModel->GetShapeConstPtr<Pipe const *>( idPipe )->GetVector(), NEURON_RADIUS * 2.f );
    }

    void SelectSubtree( BaseKnot * const pBaseKnot, tBoolOp const op ) { m_pModel->SelectSubtree( pBaseKnot, op ); }

    float SetParam( tParameter const param, float const fNewValue ) { return m_pModel->SetParam( param, fNewValue ); }

    void  SetModelFilePath  ( wstring const wstr ) { m_pModel->SetModelFilePath  ( wstr ); }
    void  AddDescriptionLine( wstring const wstr ) { m_pModel->AddDescriptionLine( wstr ); }
    void  SetUnsavedChanges ( bool const bState )  { m_pModel->SetUnsavedChanges( bState ); }

    void ToggleStopOnTrigger( ShapeId const );

    void RemoveOrphans( );

private:

    NNetModel * m_pModel { nullptr };
}; 
