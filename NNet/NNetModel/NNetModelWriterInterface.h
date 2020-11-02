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

    void CreateInitialShapes();

    NNetModel const & GetModel( ) const { return * m_pModel; }  // TODO: find better solution
    size_t    const   GetSizeOfShapeList( ) const { return m_pModel->GetSizeOfShapeList( ); }

    unique_ptr<Pipe> NewPipe( Pipe const & );
    unique_ptr<Pipe> NewPipe( BaseKnot * const, BaseKnot * const );
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
    unique_ptr<T> NewBaseKnot( BaseKnot const & p ) 
    { 
        unique_ptr<T> upT { make_unique<T>( p.GetPosition() ) };
        upT->SetId( newShapeListSlot( ) );
        return move(upT);
    }

    template <typename T> 
    unique_ptr<T> NewBaseKnot( MicroMeterPoint const & pos ) 
    { 
        unique_ptr<T> upT { make_unique<T>( pos ) };
        upT->SetId( newShapeListSlot( ) );
        return move(upT);
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

    void SelectAllShapes( tBoolOp const op ) 
    { 
        m_pModel->SelectAllShapes( op ); 
    }

    ShapeId const Add2Model( UPShape up ) 
    { 
        return m_pModel->Add2Model( move(up) ); 
    }

    template <typename T>
    unique_ptr<T> ReplaceInModel( unique_ptr<Shape> up ) 
    { 
        return m_pModel->ReplaceInModel<T>( move(up) ); 
    }

    template <typename T>
    unique_ptr<T> Store2Model( unique_ptr<T> up ) 
    { 
        return m_pModel->Store2Model<T>( move(up) ); 
    }

    template <typename T>
    unique_ptr<T> RemoveFromModel( ShapeId const id ) 
    { 
        return m_pModel->RemoveFromModel<T>( id ); 
    }

    template <typename T>
    void InsertAtModelSlot( unique_ptr<T> up, ShapeId const id ) 
    { 
        m_pModel->InsertAtModelSlot<T>( move(up), id ); 
    }

    MicroMeterPoint const OrthoVector( ShapeId const idPipe ) const
    {
        return ::OrthoVector( m_pModel->GetShapeConstPtr<Pipe const *>( idPipe )->GetVector(), NEURON_RADIUS * 2.f );
    }

    void SelectSubtree( BaseKnot * const pBaseKnot, tBoolOp const op ) { m_pModel->SelectSubtree( pBaseKnot, op ); }

    float SetParam( tParameter const param, float const fNewValue ) { return m_pModel->SetParam( param, fNewValue ); }

    void ToggleStopOnTrigger( ShapeId const );

private:

    ShapeId const newShapeListSlot( );

    NNetModel * m_pModel { nullptr };
}; 
