// NNetModel.h 
//
// NNetModel

#pragma once

#include <vector>
#include <exception>
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

// a NormalizedShapeList is a ShapeList with condition
// NormalizedShapeList[i] == nullptr || NormalizedShapeList[i]->GetId() == i

using NormalizedShapeList = vector<Shape *>;  

class ShapeErrorHandler
{
public:
	virtual void operator()( ShapeId const ) = 0;
};

// non member function used in class definition 
MicroMeterRect ComputeEnclosingRect( vector<Shape *> const & );

class NNetModel
{
public:
	void Initialize( Param * const, Observable * const, Observable * const, Observable * const );

	NNetModel()	{}

	NNetModel( NNetModel const & );

	virtual ~NNetModel( );

	void CallErrorHandler( ShapeId const id ) const
	{
		if ( m_pShapeErrorHandler )
		{
			(* m_pShapeErrorHandler)( id );
		}
		else
		{
			assert( false );
		}
	}

	bool IsEqual( NNetModel const & ) const;

	// readOnly functions

	template <typename T> bool HasType( Shape const * const pShape ) const 
	{ 
		return std::remove_pointer<T>::type::TypeFits( pShape->GetShapeType() ); 
	}

	void CheckShapeId( ShapeId const id ) const
	{
		if ( IsUndefined( id ) || IsInvalidShapeId( id ) )
			CallErrorHandler( id );
	}

	bool IsShapeNullPtr( ShapeId const id ) const
	{
		return m_Shapes[id.GetValue()] == nullptr;
	}

	Shape const * GetConstShape( ShapeId const id ) const 
	{	
		CheckShapeId( id ); 
		if ( Shape * pShape { m_Shapes[id.GetValue()] } )
		{
			return pShape;
		}
		else
		{
			CallErrorHandler( id );
			return nullptr;
		}
	}

	Shape * GetShape ( ShapeId const id )       
	{ 
		return const_cast<Shape *>(GetConstShape( id ) );
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
	
	fHertz          const GetPulseRate    ( ShapeId const ) const;
	MicroMeterPoint const GetShapePos     ( ShapeId const ) const;
	bool            const IsValidShapeId  ( ShapeId const id ) const { return id.GetValue() <  m_Shapes.size(); }
	bool            const IsInvalidShapeId( ShapeId const id ) const { return id.GetValue() >= m_Shapes.size(); }

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

	Pipe * const NewPipe(  BaseKnot * const pKnotStart, BaseKnot * const pKnotEnd )
	{
		Pipe * const pPipe { new Pipe( pKnotStart, pKnotEnd ) };
		pPipe->SetId( NewShapeListSlot( ) );
		return pPipe;
	}

	template <typename T> 
	T * const NewBaseKnot( MicroMeterPoint const & pos ) 
	{ 
		auto pT { new T( pos ) };
		pT->SetId( NewShapeListSlot( ) );
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

	ShapeId const FindShapeAt( MicroMeterPoint const &, ShapeCrit const & ) const;

	void ToggleStopOnTrigger( ShapeId const );
	void RecalcAllShapes( );
	void ResetModel( );

	NormalizedShapeList DuplicateShapes( vector<Shape *> const ) const;

	float SetParameter ( tParameter const, float const );
	void  SetNrOfShapes( long const lNrOfShapes ) { m_Shapes.resize( lNrOfShapes ); }

	MicroMeterRect GetEnclosingRect() const { return m_enclosingRect; }

	void    ClearModel()                { Apply2All<Shape>( [&](Shape &s) { s.Clear( ); } ); }
	void    SelectAll(tBoolOp const op) { Apply2All<Shape>( [&](Shape &s) { s.Select( op ); } ); }
	void    MarkShape( ShapeId const idShape, tBoolOp const op ) {	GetShapePtr<Shape *>( idShape )->Mark( op ); }
	void    SelectBeepers() { Apply2All<Neuron>( [&](Neuron & n) { if (n.HasTriggerSound()) n.Select( tBoolOp::opTrue ); } ); }
	void    SelectSubtree ( BaseKnot * const, tBoolOp const );
	Shape * ShallowCopy( Shape const & ) const;

	vector<Shape *> GetShapeList( ShapeCrit const& selector ) const
	{
		vector<Shape *> list;
		Apply2All<Shape>( [&](Shape &s) { if ( selector(s) ) list.push_back(&s); } );
		return list;
	}

	ShapeId const NewShapeListSlot( )
	{
		ShapeId idNewSlot { GetSizeOfShapeList() };
		m_Shapes.push_back( nullptr );
		return idNewSlot;
	}

	void SetShape( Shape * const pShape, ShapeId const id )	
	{
		CheckShapeId( id );
		m_Shapes[ id.GetValue() ] = pShape; 
	}

	void ReplaceInModel ( Shape * const p2BeReplaced, Shape * pShape ) { SetShape( pShape,  p2BeReplaced->GetId() ); }
	void Store2Model    ( Shape * const pShape )                       { SetShape( pShape,  pShape->GetId() ); }
	void RemoveFromModel( Shape * const pShape )                       { SetShape( nullptr, pShape->GetId() ); }

	void StaticModelChanged( )
	{ 
		m_pStaticModelObservable->NotifyAll( false );
		m_enclosingRect = ::ComputeEnclosingRect( m_Shapes );
	}

	MicroMeterPoint OrthoVector( ShapeId const idPipe ) const
	{
		return ::OrthoVector( GetShapeConstPtr<Pipe const *>( idPipe )->GetVector(), NEURON_RADIUS * 2.f );
	}

	void SetShapeErrorHandler( ShapeErrorHandler * const pHandler )
	{	
		m_pShapeErrorHandler = pHandler;
	}

private:

	NormalizedShapeList m_Shapes                  { };
	fMicroSecs          m_timeStamp               { 0._MicroSecs };
	Param             * m_pParam                  { nullptr };
	Observable        * m_pModelTimeObservable    { nullptr };
	Observable        * m_pStaticModelObservable  { nullptr };
	Observable        * m_pDynamicModelObservable { nullptr };
	MicroMeterRect      m_enclosingRect           { };

	unsigned long m_nrOfShapes       { 0L };
	unsigned long m_nrOfPipes        { 0L };
	unsigned long m_nrOfKnots        { 0L };
	unsigned long m_nrOfNeurons      { 0L };
	unsigned long m_nrOfINputNeurons { 0L };

	ShapeErrorHandler * m_pShapeErrorHandler { nullptr };

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

	bool          isEqual    ( Shape const &, Shape const & ) const;
	ShapeId const findShapeAt( MicroMeterPoint const, ShapeCrit const & ) const;
};
