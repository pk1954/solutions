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
#include "tParameter.h"
#include "tHighlightType.h"
#include "InputNeuron.h"
#include "Neuron.h"
#include "Knot.h"
#include "Pipe.h"

using std::remove_pointer;

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
	}

	bool IsEqual( NNetModel const & ) const;

	// readOnly functions

	template <typename T> bool HasType( Shape const * const pShape ) const 
	{ 
		return remove_pointer<T>::type::TypeFits( pShape->GetShapeType() ); 
	}

	bool CheckShapeId( ShapeId const id ) const
	{
		if ( IsUndefined( id ) || IsInvalidShapeId( id ) )
		{
			CallErrorHandler( id );  
			return false;
		}
		return true;
	}

	bool const IsShapeNullPtr( ShapeId const id ) const
	{
		return m_Shapes[id.GetValue()] == nullptr;
	}

	Shape const * GetConstShape( ShapeId const id ) const 
	{	
		Shape * pShape { CheckShapeId( id )	? m_Shapes[id.GetValue()] : nullptr	};
		if ( pShape == nullptr )
			CallErrorHandler( id );
		return pShape;
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

	void Apply2AllShapes( function<void(Shape &)> const & func ) const
	{
		for (auto & pShape : m_Shapes)    
		{ 
			if ( pShape )
			{
				func( static_cast<Shape &>( * pShape) ); 
			}
		}
	}                        

	virtual bool Compute( );

	ShapeId const FindShapeAt( MicroMeterPoint const &, ShapeCrit const & ) const;

	void ToggleStopOnTrigger( Neuron * );
	void RecalcAllShapes( );
	void ResetModel( );

	NormalizedShapeList DuplicateShapes( vector<Shape *> const & ) const;

	float SetParam( tParameter const, float const );
	void  SetNrOfShapes( long const lNrOfShapes ) { m_Shapes.resize( lNrOfShapes ); }

	MicroMeterRect GetEnclosingRect() const { return m_enclosingRect; }

	void    SelectSubtree( BaseKnot * const, tBoolOp const );
	Shape * ShallowCopy( Shape const * const ) const;

	ShapeId const NewShapeListSlot( )
	{
		ShapeId idNewSlot { GetSizeOfShapeList() };
		m_Shapes.push_back( nullptr );
		return idNewSlot;
	}

	void SetShape( Shape * const pShape, ShapeId const id )	
	{
		CheckShapeId( id );
		
		if ( pShape )
			pShape->IncCounter();
		
		Shape ** const ppShapeOld { & m_Shapes[ id.GetValue() ] };
		
		if ( * ppShapeOld )
			(* ppShapeOld)->DecCounter();

		* ppShapeOld = pShape; 
	}

	void StaticModelChanged( );

	MicroMeterPoint OrthoVector( ShapeId const idPipe ) const
	{
		return ::OrthoVector( GetShapeConstPtr<Pipe const *>( idPipe )->GetVector(), NEURON_RADIUS * 2.f );
	}

	void SetShapeErrorHandler( ShapeErrorHandler * const pHandler )
	{	
		m_pShapeErrorHandler = pHandler;
	}

	NormalizedShapeList const & GetShapes( ) const { return m_Shapes; }

private:

	NormalizedShapeList m_Shapes                  { };
	fMicroSecs          m_timeStamp               { 0._MicroSecs };
	Param             * m_pParam                  { nullptr };
	Observable        * m_pModelTimeObservable    { nullptr };
	Observable        * m_pStaticModelObservable  { nullptr };
	Observable        * m_pDynamicModelObservable { nullptr };
	ShapeErrorHandler * m_pShapeErrorHandler      { nullptr };
	MicroMeterRect      m_enclosingRect           { };

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
