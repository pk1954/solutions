// NNetModel.h 
//
// NNetModel

#pragma once

#include <vector>
#include "util.h"
#include "BoolOp.h"
#include "MoreTypes.h"
#include "ObserverInterface.h"
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

using ShapeList = vector<Shape *>;

// non member function used in class definition 
MicroMeterRect GetEnclosingRect( ShapeList const & );

class NNetModel
{
public:

	NNetModel( Param * const, ObserverInterface * const );

	virtual ~NNetModel( );

	// readOnly functions

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

	bool          IsShapeIdOk  ( ShapeId const id ) const { return IsDefined( id ) && IsValidShapeId( id ); }
	Shape       * GetShape     ( ShapeId const id )       { return IsShapeIdOk( id ) ? m_Shapes[id.GetValue()] : nullptr; }
	Shape const * GetConstShape( ShapeId const id ) const {	return IsShapeIdOk( id ) ? m_Shapes[id.GetValue()] : nullptr; }
	
	MicroMeterPoint const GetShapePos   ( ShapeId const id  ) const;
	ShapeType       const GetShapeType  ( ShapeId const id  ) const;
	bool            const HasOutgoing   ( ShapeId const id  ) const;
	bool            const HasIncoming   ( ShapeId const id  ) const;
	bool            const IsValidShapeId( ShapeId const id  ) const { return id.GetValue() < m_Shapes.size(); }

	fMicroSecs      const GetSimulationTime ( ) const { return m_timeStamp; }
	long            const GetSizeOfShapeList( ) const { return CastToLong( m_Shapes.size() ); }
	
	template <typename T>
	long const GetNrOf( ) const
	{
		long lCounter = 0;
		Apply2All<T>( [&]( T & s ) { ++ lCounter; } );
		return lCounter;
	}

	BaseKnot * const GetStartKnotPtr(ShapeId const id) const { return GetShapeConstPtr<Pipe const *>(id)->GetStartKnotPtr(); }
	BaseKnot * const GetEndKnotPtr  (ShapeId const id) const { return GetShapeConstPtr<Pipe const *>(id)->GetEndKnotPtr  (); }

	ShapeId const GetStartKnotId (ShapeId const idPipe) const { return GetStartKnotPtr(idPipe)->GetId(); }
	ShapeId const GetEndKnotId   (ShapeId const idPipe) const { return GetEndKnotPtr  (idPipe)->GetId(); }

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
		add2ShapeList( pT );
		modelHasChanged( );
		return pT;
	}

	template <typename T>
	void AppendShape( ShapeId const id )
	{
		if ( IsOfType<Knot>( id ) )
		{
			Connect( id, NewShape<T>( GetShapePos( id ) )->GetId() );
			modelHasChanged( );
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
	}                               // HasType checks for nullptr

	template <typename T>
	void Apply2AllInRect( MicroMeterRect const & r, function<void(T &)> const & func ) const
	{
		Apply2All<T>( {	[&](T & s) { if ( s.IsInRect(r) ) { func( s ); } } } );
	}

	template <typename T>
	void Apply2AllSelected( function<void(T &)> const & func ) const
	{
		Apply2All<T>( {	[&](T & s) { if ( s.IsSelected() ) { func( s ); } } } );
	}

	virtual void Compute( );

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
	void RecalcAllShapes( );
	void ResetModel( );
	void SetPulseRate   ( ShapeId const, float const );
	void SetParameter( tParameter const, float const );
	void SetNrOfShapes( long lNrOfShapes ) { m_Shapes.resize( lNrOfShapes ); }
	void SetTriggerSound( ShapeId const, Hertz const, MilliSecs const );
	void StopTriggerSound( ShapeId const id ) {	SetTriggerSound( id, 0_Hertz, 0_MilliSecs ); }

	void RemoveShape( ShapeId const id ) 
	{ 
		removeShape( GetShape( id ) ); 
		modelHasChanged( );
	}

	MicroMeterRect GetEnclosingRect() {	return ::GetEnclosingRect( m_Shapes ); }

	void ClearModel()                { Apply2All<Shape>( [&](Shape &s) { s.Clear( ); } ); }
	void CheckConsistency()          { Apply2All<Shape>( [&](Shape &s) { checkConsistency(&s); } ); }
	void SelectAll(tBoolOp const op) { Apply2All<Shape>( [&](Shape &s) { s.Select( op ); } ); }

	void CopySelection( );
	void MarkSelection( tBoolOp const );
	void DeleteSelection( );
	void MoveSelection( MicroMeterPoint const & );
	void SelectBeepers( ) {	Apply2All<Neuron>( [&](Neuron & n) { if (n.HasTriggerSound()) n.Select( tBoolOp::opTrue ); } ); }

	void RemoveBeepers( ) 
	{	
		if ( AnyShapesSelected() )
			Apply2AllSelected<Neuron>( [&](Neuron & n) { removeTriggerSound( & n ); } );
		else
			Apply2All        <Neuron>( [&](Neuron & n) { removeTriggerSound( & n ); } );
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

	ShapeList           m_Shapes          { };
	fMicroSecs          m_timeStamp       { 0._MicroSecs };
	Param             * m_pParam          { nullptr };
	ObserverInterface * m_pChangeObserver { nullptr };

	// local functions

	void add2ShapeList( Shape * const pNewShape )
	{
		pNewShape->SetId( ShapeId { GetSizeOfShapeList() } );
		m_Shapes.push_back( pNewShape );
	}

	MicroMeterPoint orthoVector        ( ShapeId const ) const;
	bool const      isConnectedTo      ( Shape const &, Shape const & ) const;
	bool const      isConnectedToPipe  ( Shape const &, Pipe  const & ) const;
	void            deletePipeEndPoints( Pipe  * const );
	void            deleteShape        ( Shape * const );
	void            checkConsistency   ( Shape * const );
	void            removeShape        ( Shape * const );
	Shape *         shallowCopy        ( Shape   const & );
	void            disconnectBaseKnot ( BaseKnot * const );
	void            selectSubtree      ( BaseKnot * const, tBoolOp    const );
	void            insertBaseKnot     ( Pipe     * const, BaseKnot * const );
	void            connectIncoming    ( Pipe     * const, BaseKnot * const );
	void            connectOutgoing    ( Pipe     * const, BaseKnot * const );
	Shape const   * findShapeAt        ( MicroMeterPoint const, function<bool(Shape const &)> const & ) const;
	void            connectToNewShapes ( Shape &, ShapeList & );
	void            modelHasChanged    ( ) const;
	void            setTriggerSound    ( Neuron * const, Hertz const, MilliSecs const );
	void            removeTriggerSound ( Neuron * const );
};
