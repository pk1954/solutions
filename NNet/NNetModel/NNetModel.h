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
#include "InputNeuron.h"
#include "Neuron.h"
#include "Knot.h"
#include "Pipe.h"

class EventInterface;
class Param;

using ShapeList = vector<Shape *>;

// non member function used in class definition 
MicroMeterRect ComputeEnclosingRect( ShapeList const & );

void ConnectIncoming ( Pipe * const, BaseKnot * const );
void ReplaceStartKnot( Pipe * const, BaseKnot * const );
void ReplaceEndKnot  ( Pipe * const, BaseKnot * const );

class NNetModel
{
public:

	void Initialize( Param * const, Observable * const, Observable * const, Observable * const );

	NNetModel()	{}

	NNetModel( NNetModel const & );

	virtual ~NNetModel( );

	bool IsEqual( NNetModel const & ) const;

	// readOnly functions

	template <typename T> bool HasType( Shape const * const pShape ) const 
	{ 
		return std::remove_pointer<T>::type::TypeFits( pShape->GetShapeType() ); 
	}

	template <typename T>
	T GetShapePtr( ShapeId const id ) 
	{
		Shape * const pShape { GetShape( id ) };
		return (pShape && HasType<T>(pShape)) ? static_cast<T>( pShape ) : nullptr;
	}

	template <typename T>
	T GetShapeConstPtr( ShapeId const id ) const
	{
		Shape const * const pShape { GetConstShape( id ) };
		return (pShape && HasType<T>(pShape)) ? static_cast<T>( pShape ) : nullptr;
	}

	template <typename T>
	long const GetNrOf( ) const
	{
		long lCounter = 0;
		Apply2All<T>( [&]( T const & s ) { ++ lCounter; } );
		return lCounter;
	}

	bool const AnyShapesSelected( ) const
	{
		return Apply2AllB<Shape>( [&]( Shape const & shape ) { return shape.IsSelected(); } );
	}

	bool IsSelected( ShapeId const idShape ) const
	{
		if ( Shape const * pShape { GetShapeConstPtr<Shape const *>( idShape ) } )
			return pShape->IsSelected();
		else 
			return false;
	}

	bool          IsShapeIdOk  ( ShapeId const id ) const { return IsDefined( id ) && IsValidShapeId( id ); }
	Shape       * GetShape     ( ShapeId const id )       { return IsShapeIdOk( id ) ? m_Shapes[id.GetValue()] : nullptr; }
	Shape const * GetConstShape( ShapeId const id ) const {	return IsShapeIdOk( id ) ? m_Shapes[id.GetValue()] : nullptr; }
	
	fHertz          const GetPulseRate  ( ShapeId const ) const;
	MicroMeterPoint const GetShapePos   ( ShapeId const ) const;
	bool            const IsValidShapeId( ShapeId const id  ) const { return id.GetValue() < m_Shapes.size(); }

	fMicroSecs      const GetSimulationTime ( ) const { return m_timeStamp; }
	long            const GetSizeOfShapeList( ) const { return CastToLong( m_Shapes.size() ); }

	BaseKnot * const GetStartKnotPtr(ShapeId const id) const { return GetShapeConstPtr<Pipe const *>(id)->GetStartKnotPtr(); }
	BaseKnot * const GetEndKnotPtr  (ShapeId const id) const { return GetShapeConstPtr<Pipe const *>(id)->GetEndKnotPtr  (); }

	ShapeId const GetStartKnotId (ShapeId const idPipe) const { return GetStartKnotPtr(idPipe)->GetId(); }
	ShapeId const GetEndKnotId   (ShapeId const idPipe) const { return GetEndKnotPtr  (idPipe)->GetId(); }

	// manipulating functions

	fMicroSecs SetSimulationTime( fMicroSecs const newVal = 0._MicroSecs )	
	{ 
		fMicroSecs currentValue;
		m_timeStamp = newVal; 
		m_pModelTimeObservable->NotifyAll( false );
		return currentValue;
	}

	template <typename T> 
	T * const NewShape( MicroMeterPoint const & pos ) 
	{ 
		auto pT { new T( pos ) };
		Add2ShapeList( pT );
		StaticModelChanged( );
		return pT;
	}

	template <typename T>
	bool Apply2AllB( function<bool(T &)> const & func ) const
	{
		bool bResult { false };
		for ( auto pShape : m_Shapes )
		{
			if ( pShape )
			{
				if ( HasType<T>( pShape ) )	
					bResult = func( static_cast<T &>( * pShape ) );
				if ( bResult )
					break;
			}
		}
		return bResult;
	}

	template <typename T>   // const version
	bool Apply2AllB( function<bool(T const &)> const & func ) const
	{
		bool bResult { false };
		for ( auto pShape : m_Shapes )
		{
			if ( pShape )
			{
				if ( HasType<T>( pShape ) )	
					bResult = func( static_cast<T const &>( * pShape ) );
				if ( bResult )
					break;
			}
		}
		return bResult;
	}

	template <typename T>
	void Apply2All( function<void(T &)> const & func ) const
	{
		for (auto & pShape : m_Shapes)    
		{ 
			if ( pShape )
			{
				if ( HasType<T>(pShape) ) func( static_cast<T &>( * pShape) ); 
			}
		}
	}                        

	template <typename T>    // const version
	void Apply2All( function<void(T const &)> const & func ) const
	{
		for (auto & pShape : m_Shapes)    
		{ 
			if ( pShape )
			{
				if ( HasType<T>(pShape) ) 
					func( static_cast<T const &>( * pShape) ); 
			}
		}
	}                        

	template <typename T>
	void Apply2AllInRect( MicroMeterRect const & r, function<void(T &)> const & func )
	{
		Apply2All<T>( [&](T & s) { if ( s.IsInRect(r) ) { func( s ); } } );
	}

	template <typename T>   // const version
	void Apply2AllInRect( MicroMeterRect const & r, function<void(T const &)> const & func )
	{
		Apply2All<T>( [&](T & const s) { if ( s.IsInRect(r) ) { func( s ); } } );
	}

	template <typename T>
	void Apply2AllSelected( function<void(T &)> const & func ) const
	{
		Apply2All<T>( {	[&](T & s) { if ( s.IsSelected() ) { func( s ); } } } );
	}

	template <typename T>  // const version
	void Apply2AllSelected( function<void(T const &)> const & func ) const
	{
		Apply2All<T>( {	[&](T const & s) { if ( s.IsSelected() ) { func( s ); } } } );
	}

	virtual bool Compute( );

	void CreateInitialShapes();
	void SetShape( Shape * const pShape, ShapeId const id )	{ m_Shapes[ id.GetValue() ] = pShape; }

	Pipe * const NewPipe( BaseKnot * const, BaseKnot      * const   );

	ShapeId const FindShapeAt( MicroMeterPoint const &, ShapeCrit const & ) const;

	void Disconnect         ( ShapeId const );
	void ToggleStopOnTrigger( ShapeId const );
	void RecalcAllShapes( );
	void ResetModel( );

	float SetParameter ( tParameter const, float const );
	void  SetNrOfShapes( long const lNrOfShapes ) { m_Shapes.resize( lNrOfShapes ); }

	MicroMeterRect GetEnclosingRect() const { return m_enclosingRect; }

	void ClearModel()                { Apply2All<Shape>( [&](Shape &s) { s.Clear( ); } ); }
	void SelectAll(tBoolOp const op) { Apply2All<Shape>( [&](Shape &s) { s.Select( op ); } ); }

	void CopySelection( );
	void MarkSelection( tBoolOp const );
	void DeleteSelection( );
	void GetSelectionList( ShapeList & ) const;
	void SetSelectionList( ShapeList const & );

	void DisconnectBaseKnot ( BaseKnot * const );
	void DeleteShape( Shape * const );

	void SelectBeepers() { Apply2All<Neuron>( [&](Neuron & n) { if (n.HasTriggerSound()) n.Select( tBoolOp::opTrue ); } ); }

	void ClearBeepers() 
	{	
		if ( AnyShapesSelected() )
			Apply2AllSelected<Neuron>( [&](Neuron & n) { clearTriggerSound( & n ); } );
		else
			Apply2All        <Neuron>( [&](Neuron & n) { clearTriggerSound( & n ); } );
	}

	void SelectSubtree ( BaseKnot * const, tBoolOp const );

	void MarkShape( ShapeId const idShape, tBoolOp const op )
	{
		if ( Shape * const pShape { GetShapePtr<Shape *>( idShape ) } )
			pShape->Mark( op );
	}

	void Add2ShapeList( Shape * const pNewShape )
	{
		pNewShape->SetId( ShapeId { GetSizeOfShapeList() } );
		m_Shapes.push_back( pNewShape );
	}

	void RemoveFromShapeList( Shape * const pShape )
	{
		long lIndex { pShape->GetId().GetValue() };
		assert( m_Shapes[ lIndex ] == pShape );
		m_Shapes[ lIndex ] = nullptr;
	}

	void Restore2ShapeList( Shape * const pShape )
	{
		long lIndex { pShape->GetId().GetValue() };
		m_Shapes[ lIndex ] = pShape;
	}

	void ReplaceInShapeList( Shape * const pRemove, Shape * pReplace )
	{
		long lIndex { pRemove->GetId().GetValue() };
		m_Shapes[ lIndex ] = pReplace;
	}

	void StaticModelChanged( )
	{ 
		m_pStaticModelObservable->NotifyAll( false );
		m_enclosingRect = ::ComputeEnclosingRect( m_Shapes );
	}

	MicroMeterPoint OrthoVector( ShapeId const idPipe ) const
	{
		return ::OrthoVector( GetShapeConstPtr<Pipe const *>( idPipe )->GetVector(), NEURON_RADIUS * 2.f );
	}

private:

	ShapeList      m_Shapes                  { };
	fMicroSecs     m_timeStamp               { 0._MicroSecs };
	Param        * m_pParam                  { nullptr };
	Observable   * m_pModelTimeObservable    { nullptr };
	Observable   * m_pStaticModelObservable  { nullptr };
	Observable   * m_pDynamicModelObservable { nullptr };
	MicroMeterRect m_enclosingRect           { };

	unsigned long m_nrOfShapes       { 0L };
	unsigned long m_nrOfPipes        { 0L };
	unsigned long m_nrOfKnots        { 0L };
	unsigned long m_nrOfNeurons      { 0L };
	unsigned long m_nrOfINputNeurons { 0L };

	// local functions

	void incTimeStamp( )
	{
		m_timeStamp += m_pParam->GetTimeResolution( );
		m_pModelTimeObservable->NotifyAll( false );
	}

	void dynamicModelChanged( ) const 
	{ 
		m_pDynamicModelObservable->NotifyAll( false );
	}

	Shape *       shallowCopy        ( Shape   const & ) const;
	bool          isEqual            ( Shape const &, Shape const & ) const;
	void          connectToNewShapes ( Shape const &, ShapeList const & ) const;
	void          setTriggerSound    ( Neuron * const, bool const, Hertz const, MilliSecs const );
	void          clearTriggerSound  ( Neuron * const );
	ShapeId const findShapeAt        ( MicroMeterPoint const, ShapeCrit const & ) const;
};
