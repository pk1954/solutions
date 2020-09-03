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

    Pipe    * const   NewPipe( BaseKnot * const, BaseKnot * const );
    Shape   * const   GetShape( ShapeId const );

    void SelectBeepers();
    void SelectShape( ShapeId const, tBoolOp const );
    void MarkShape  ( ShapeId const, tBoolOp const );
    void SetShape   ( Shape * const, ShapeId const );	
    void SetNrOfShapes( long const );
    void SetShapeErrorHandler( ShapeErrorHandler * const );

    void StaticModelChanged( );

    template <typename T>
    T GetShapePtr( ShapeId const id ) 
    {
        Shape * const pShape { GetShape( id ) };
        return (pShape && m_pModel->HasType<T>(pShape)) ? static_cast<T>( pShape ) : nullptr;
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
        m_pModel->Apply2AllShapes
        ( 
            [&](Shape & s) 
            {  
                if ( m_pModel->HasType<T>(& s) ) func( static_cast<T &>(s) ); 
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

    void ResetModel( )                     { m_pModel->ResetModel(); }
    void ClearModel( )                     { m_pModel->Apply2AllShapes( [&](Shape  &s) { s.Clear( ); } ); }
    void SelectAllShapes(tBoolOp const op) { m_pModel->Apply2AllShapes( [&](Shape  &s) { s.Select( op ); } ); }

    void ReplaceInModel ( Shape * const p2BeReplaced, Shape * pShape ) { SetShape( pShape,  p2BeReplaced->GetId() ); }
    void Store2Model    ( Shape * const pShape )                       { SetShape( pShape,  pShape->GetId() ); }
    void RemoveFromModel( Shape * const pShape )                       { SetShape( nullptr, pShape->GetId() ); }
    void Add2Model      ( Shape * const pShape )                       
    { 
        ShapeId const idNewSlot { m_pModel->NewShapeListSlot( ) };
        pShape->SetId( idNewSlot );
        SetShape( pShape, idNewSlot );
    }

    MicroMeterPoint const OrthoVector( ShapeId const idPipe ) const
    {
        return ::OrthoVector( m_pModel->GetShapeConstPtr<Pipe const *>( idPipe )->GetVector(), NEURON_RADIUS * 2.f );
    }

    vector<Shape *> GetShapeList( ShapeCrit const & ) const;

    void SelectSubtree( BaseKnot * const pBaseKnot, tBoolOp const op ) { m_pModel->SelectSubtree( pBaseKnot, op ); }

    float SetParam( tParameter const param, float const fNewValue ) { return m_pModel->SetParam( param, fNewValue ); }

    void ToggleStopOnTrigger( ShapeId const );

private:

    ShapeId const newShapeListSlot( );

    NNetModel * m_pModel { nullptr };
}; 
