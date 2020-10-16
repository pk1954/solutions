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

class NNetModelWriterInterface
{
public:

	void Start( NNetModel * const );
	void Stop(); 

    void CreateInitialShapes();

    NNetModel const & GetModel( ) const { return * m_pModel; }  // TODO: find better solution

    Pipe  * const NewPipe( BaseKnot * const, BaseKnot * const );
    Shape * const GetShape( ShapeId const );

    bool const IsPipe( ShapeId const id )
    {
        Shape * pShape { GetShapePtr<Shape *>( id ) };
        return pShape && pShape->IsPipe( );
    }

    void SelectBeepers();
    void SelectShape( ShapeId const, tBoolOp const );
    void MarkShape  ( ShapeId const, tBoolOp const );
    void IncShapeList( long const );
    void SetShapeErrorHandler( ShapeErrorHandler * const );

    void StaticModelChanged( );

    template <typename T>
    T GetShapePtr( ShapeId const id ) 
    {
        Shape * const pShape { GetShape( id ) };
        return (pShape && HasType<T>( * pShape )) ? static_cast<T>( pShape ) : nullptr;
    }

    template <typename T> 
    T * const NewBaseKnot( MicroMeterPoint const & pos ) 
    { 
        auto pT { new T( pos ) };
        pT->SetId( newShapeListSlot( ) );
        return pT;
    }

	template <typename T>
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

    template <typename T>
    void Apply2AllSelected( function<void(T &)> const & func ) const
    {
    	Apply2All<T>( {	[&](T & s) { if ( s.IsSelected() ) { func( s ); } } } );
    }

    template <typename T>
    void Apply2AllInRect( MicroMeterRect const & r, function<void(T &)> const & func )
    {
        Apply2All<T>( [&](T & s) { if ( s.IsInRect(r) ) { func( s ); } } );
    }

    MonitorData * GetMonitorData( ) { return m_pModel->GetMonitorData(); }

    void CheckModel( ) { m_pModel->CheckModel(); }
    void ResetModel( ) { m_pModel->ResetModel(); }
    void ClearModel( ) 
    {
        m_pModel->GetShapes().Apply2All( [&](Shape & s) { s.Clear( ); } ); 
    }

    void SelectAllShapes( tBoolOp const op ) { m_pModel->SelectAllShapes( op ); }

    void ReplaceInModel   ( Shape & replaceMe, Shape & shape ) { m_pModel->ReplaceInModel ( replaceMe, shape ); }
    void Store2Model      ( Shape * const pShape )             { m_pModel->Store2Model    ( * pShape ); }
    void RemoveFromModel  ( Shape * const pShape )             { m_pModel->RemoveFromModel( * pShape ); }
    void InsertAtModelSlot( Shape & shape, ShapeId const id )  { m_pModel->InsertAtModelSlot( shape, id ); }
    void Add2Model        ( Shape & shape )                    { InsertAtModelSlot( shape, m_pModel->NewShapeListSlot( ) ); }

    MicroMeterPoint const OrthoVector( ShapeId const idPipe ) const
    {
        return ::OrthoVector( m_pModel->GetShapeConstPtr<Pipe const *>( idPipe )->GetVector(), NEURON_RADIUS * 2.f );
    }

    void GetShapeList( ShapeList &, ShapeCrit const & ) const;

    void SelectSubtree( BaseKnot * const pBaseKnot, tBoolOp const op ) { m_pModel->SelectSubtree( pBaseKnot, op ); }

    float SetParam( tParameter const param, float const fNewValue ) { return m_pModel->SetParam( param, fNewValue ); }

    void ToggleStopOnTrigger( ShapeId const );

private:

    ShapeId const newShapeListSlot( );

    NNetModel * m_pModel { nullptr };
}; 
