// NNetModelReaderInterface.h 
//
// NNetModel
//
// Interface for reader threads

#pragma once

#include "MoreTypes.h"
#include "ShapeType.h"
#include "ShapeId.h"
#include "Shape.h"
#include "NNetModel.h"
#include "tHighlightType.h"

class DrawContext;
class ObserverInterface;
class Param;
class Pipe;

class NNetModelReaderInterface
{
public:

	void Start( NNetModel * const );
	void Stop(); 

	bool       const IsEqual( NNetModel const & ) const;

	bool       const AnyShapesSelected         ( )               const;	
	bool       const IsSelected                ( ShapeId const ) const;
	ShapeType  const GetShapeType              ( ShapeId const ) const;
	fHertz     const GetPulseFrequency         ( ShapeId const ) const;
	size_t     const GetNrOfSegments           ( ShapeId const ) const;
	SoundDescr const GetTriggerSound           ( ShapeId const ) const;
	mV         const GetVoltage                ( ShapeId const ) const;
	mV         const GetVoltage                ( ShapeId const, MicroMeterPoint const & ) const;
	bool       const HasIncoming               ( ShapeId const ) const;
	bool       const HasOutgoing               ( ShapeId const ) const;
	size_t     const GetNrOfOutgoingConnections( ShapeId const ) const;
	size_t     const GetNrOfIncomingConnections( ShapeId const ) const;
	bool       const ConnectsTo( ShapeId const, ShapeId const ) const;

	bool       const IsValidShapeId  ( ShapeId const id ) const { return m_pModel->IsValidShapeId(id); }
	bool       const IsInvalidShapeId( ShapeId const id ) const { return m_pModel->IsInvalidShapeId(id); }
	bool       const IsShapeNullPtr  ( ShapeId const id ) const { return m_pModel->IsShapeNullPtr(id); }
	long       const GetSizeOfShapeList( )                const { return m_pModel->GetSizeOfShapeList(); }

	MicroMeterPoint const OrthoVector( ShapeId const ) const;
	MicroMeterPoint const GetShapePos( ShapeId const ) const;
	MicroMeterRect  const GetEnclosingRect( ) const;

	fMicroSecs GetSimulationTime( ) const;

	template <typename T>
	unsigned long const GetNrOf( ) const
	{
		return T::GetCounter( );
	}

	void DrawExterior  ( ShapeId const, DrawContext const &, tHighlightType const ) const;
	void DrawInterior  ( ShapeId const, DrawContext const & ) const;
	void DrawNeuronText( ShapeId const, DrawContext const & ) const;

	ShapeId const FindShapeAt( MicroMeterPoint const &, ShapeCrit const & ) const;

	template <typename T> 
	bool IsOfType( ShapeId const id ) const 
	{ 
		return T::TypeFits( GetShapeType( id ) ); 
	}

	template <typename T>   // const version
	bool Apply2AllB( function<bool(T const &)> const & func ) const
	{
		bool bResult { false };
		for ( auto pShape : m_pModel->GetShapes() )
		{
			if ( pShape )
			{
				if ( m_pModel->HasType<T>( pShape ) )	
					bResult = func( static_cast<T const &>( * pShape ) );
				if ( bResult )
					break;
			}
		}
		return bResult;
	}

	template <typename T>    // const version
	void Apply2All( function<void(T const &)> const & func ) const
	{
		for (auto & pShape : m_pModel->GetShapes() )    
		{ 
			if ( pShape )
			{
				if ( m_pModel->HasType<T>(pShape) ) 
					func( static_cast<T const &>( * pShape) ); 
			}
		}
	}                        

	template <typename T>   // const version
	void Apply2AllInRect( MicroMeterRect const & r, function<void(T const &)> const & func ) const
	{
		Apply2All<T>( [&](T const & s) { if ( s.IsInRect(r) ) { func( s ); } } );
	}

	template <typename T>  // const version
	void Apply2AllSelected( function<void(T const &)> const & func ) const
	{
		Apply2All<T>( {	[&](T const & s) { if ( s.IsSelected() ) { func( s ); } } } );
	}

private:
	NNetModel const * m_pModel;

	bool const isConnectedTo    ( ShapeId const, ShapeId const ) const;
	bool const isConnectedToPipe( ShapeId const, ShapeId const ) const;
};
