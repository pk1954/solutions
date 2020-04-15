// NNetModel.h 
//
// NNetModel

#pragma once

#include <vector>
#include "util.h"
#include "BoolOp.h"
#include "MoreTypes.h"
#include "Observable.h"
#include "Segment.h"
#include "tParameter.h"
#include "tHighlightType.h"
#include "Neuron.h"
#include "Knot.h"
#include "Pipe.h"

class ObserverInterface;
class EventInterface;
class ComputeThread;
class InputNeuron;
class Param;

class NNetModel
{
public:

	NNetModel( Param * const );

	virtual ~NNetModel( );

	// readOnly functions

	bool IsShapeIdOk( ShapeId const id ) const
	{
		return IsDefined( id ) && IsValidShapeId( id );
	}

	Shape * GetShape( ShapeId const id )
	{
		return IsShapeIdOk( id ) ? m_Shapes[id.GetValue()] : nullptr;
	}

	Shape const * GetConstShape( ShapeId const id ) const
	{
		return IsShapeIdOk( id ) ? m_Shapes[id.GetValue()] : nullptr;
	}

	template <typename T> bool IsOfType( ShapeId const id ) const 
	{ 
		return T::TypeFits( GetShapeType( id ) ); 
	}

	template <typename T> bool HasType( Shape const * const pShape ) const 
	{ 
		return pShape && std::remove_pointer<T>::type::TypeFits( pShape->GetShapeType() ); 
	}

	template <typename T>
	T GetShapePtr( ShapeId const id ) 
	{
		Shape * const pShape { GetShape( id ) };
		return HasType<T>( pShape )	? static_cast<T>( pShape ) : nullptr;
	}

	template <typename T>
	T GetShapeConstPtr( ShapeId const id ) const
	{
		Shape const * const pShape { GetConstShape( id ) };
		return HasType<T>( pShape ) ? static_cast<T>( pShape ) : nullptr;
	}

	MicroMeterPoint const GetShapePos   ( ShapeId const id  ) const;
	ShapeType       const GetShapeType  ( ShapeId const id  ) const;
	bool            const HasOutgoing   ( ShapeId const id  ) const;
	bool            const HasIncoming   ( ShapeId const id  ) const;
	bool            const IsValidShapeId( ShapeId const id  ) const { return id.GetValue() < m_Shapes.size(); }

	fMicroSecs      const GetSimulationTime ( ) const { return m_timeStamp; }
	long            const GetSizeOfShapeList( ) const { return CastToLong( m_Shapes.size() ); }
	bool            const HasModelChanged   ( ) const { return m_bUnsavedChanges; }
	long            const GetNrOfShapes     ( ) const;

	BaseKnot * const GetStartKnotPtr( ShapeId const idPipe ) const 
	{ 
		return GetShapeConstPtr<Pipe const *>( idPipe )->GetStartKnotPtr(); 
	}

	BaseKnot * const GetEndKnotPtr( ShapeId const idPipe ) const 
	{ 
		return GetShapeConstPtr<Pipe const *>( idPipe )->GetEndKnotPtr(); 
	}

	ShapeId const GetStartKnotId( ShapeId const idPipe ) const 
	{ 
		return GetStartKnotPtr( idPipe )->GetId(); 
	}

	ShapeId const GetEndKnotId( ShapeId const idPipe ) const 
	{ 
		return GetEndKnotPtr( idPipe )->GetId(); 
	}

	size_t const GetNrOfOutgoingConnections( ShapeId const ) const;
	size_t const GetNrOfIncomingConnections( ShapeId const ) const; 

	Shape const * FindShapeAt( MicroMeterPoint const, function<bool(Shape const &)> const & ) const;

	float const GetPulseRate( InputNeuron const * ) const;

	bool const ConnectsTo( ShapeId const, ShapeId const ) const;

	// manipulating functions

	void ResetSimulationTime( )	{ m_timeStamp = 0._MicroSecs; }

	template <typename T> 
	T * const NewShape( MicroMeterPoint const & pos ) 
	{ 
		auto pT { new T( pos ) };
		pT->SetId( ShapeId { GetSizeOfShapeList() } );
		m_Shapes.push_back( pT );
		return pT;
	}

	template <typename T>
	void AppendShape( ShapeId const id )
	{
		if ( IsOfType<Knot>( id ) )
		{
			Connect( id, NewShape<T>( GetShapePos( id ) )->GetId() );
		}
	}

	template <typename T>
	bool Apply2AllB( function<bool(T &)> const & func ) const
	{
		bool bResult { false };
		for ( auto pShape : m_Shapes )
		{
			if ( HasType<T>( pShape ) )
			{
				bResult = func( static_cast<T &>( * pShape ) );
				if ( bResult )
					break;
			}
		}
		return bResult;
	}

	template <typename T>
	void Apply2All( function<void(T &)> const & func ) const
	{
		for (auto p : m_Shapes) { if ( HasType<T>(p) ) func( static_cast<T &>(*p) ); }
	}

	template <typename T>
	void Apply2AllInRect( MicroMeterRect const & r, function<void(T &)> const & func ) const
	{
		Apply2All<T>( {	[&](T & s) { if ( s.IsInRect(r) ) { func( s ); } } } );
	}

	void CreateInitialShapes();
	void SetShape( Shape * const pShape, ShapeId const id )	{ m_Shapes[ id.GetValue() ] = pShape; }
	void NewPipe( BaseKnot * const, BaseKnot * const );

	Knot   * const InsertKnot  ( ShapeId const, MicroMeterPoint const & );
	Neuron * const InsertNeuron( ShapeId const, MicroMeterPoint const & );
	void           MoveShape   ( ShapeId const, MicroMeterPoint const & );

	void AddOutgoing2Knot( ShapeId const, MicroMeterPoint const & );
	void AddIncoming2Knot( ShapeId const, MicroMeterPoint const & );
	void AddOutgoing2Pipe( ShapeId const, MicroMeterPoint const & );
	void AddIncoming2Pipe( ShapeId const, MicroMeterPoint const & );

	void Connect( ShapeId const, ShapeId const );
	void Convert2Neuron( ShapeId const );
	void Convert2InputNeuron( ShapeId const );
	void Disconnect( ShapeId const );
	void RemoveShape( ShapeId const );
	void RecalcAllShapes( );
	void ResetModel( );
	void ClearModel( );
	void ModelSaved  ( ) const;
	void ModelChanged( ) const;
	void SetPulseRate( ShapeId    const, float const );
	void SetParameter( tParameter const, float const );
	void SetNrOfShapes( long lNrOfShapes ) { m_Shapes.resize( lNrOfShapes ); }

	MicroMeterRect GetEnclosingRect( );

	void CheckConsistency() { Apply2All<Shape>( [&]( Shape & shape ) { checkConsistency( & shape ); } ); }

	virtual void Compute( );

	void SelectAll( tBoolOp const op )
	{
		Apply2All<Shape>( [&]( Shape & shape ) { shape.Select( op ); } );
	}

	void SelectSubtree( ShapeId const idBaseKnot, tBoolOp const op )
	{
		selectSubtree( GetShapePtr<BaseKnot *>( idBaseKnot ), op );
	}

	void SelectShape( ShapeId const idShape, tBoolOp const op )
	{
		if ( Shape * const pShape { GetShapePtr<Shape *>( idShape ) } )
			pShape->Select( op );
	}

	void MarkShape( ShapeId const idShape, tBoolOp const op )
	{
		if ( Shape * const pShape { GetShapePtr<Shape *>( idShape ) } )
			pShape->Mark( op );
	}

	void MarkSelection( tBoolOp const op )
	{
		Apply2All<Shape>( [&]( Shape & shape ) { if ( shape.IsSelected() ) shape.Mark( op ); } );
	}

	void CopySelection( )
	{
	}

	void DeleteSelection( )
	{
		Apply2All<Shape>( [&]( Shape & shape ) { if ( shape.IsSelected() ) removeShape( & shape ); } );
	}

	bool AnyShapesSelected( )
	{
		return Apply2AllB<Shape>( [&]( Shape & shape ) { return shape.IsSelected(); } );
	}

	bool IsSelected( ShapeId const idShape )
	{
		if ( Shape const * pShape { GetShapePtr<Shape *>( idShape ) } )
			return pShape->IsSelected();
		else 
			return false;
	}

private:

	Param         * m_pParam          { nullptr };
	vector<Shape *> m_Shapes          { };
	fMicroSecs      m_timeStamp       { 0._MicroSecs };
	mutable bool    m_bUnsavedChanges { false };  // can be changed in const functions

	// local functions

	Shape const   * findShapeAt( MicroMeterPoint const, function<bool(Shape const &)> const & ) const;
	void            checkConsistency( Shape * );
	MicroMeterPoint orthoVector( ShapeId const ) const;
	void            disconnectBaseKnot( BaseKnot * const );
	void            deletePipe( ShapeId const );
	void            insertBaseKnot( Pipe * const, BaseKnot * const );
	void            deleteShape( ShapeId const );
	void            removeShape( Shape const * );
	bool const      isConnectedTo( ShapeId, ShapeId ) const;
	bool const      isConnectedToPipe( ShapeId const, Pipe const * const ) const;
	void            connectIncoming( Pipe * const, BaseKnot * const );
	void            connectOutgoing( Pipe * const, BaseKnot * const );
	void            selectSubtree( BaseKnot * const, tBoolOp const op );

};

MicroMeterRect GetEnclosingRect( vector<Shape*> const & );
