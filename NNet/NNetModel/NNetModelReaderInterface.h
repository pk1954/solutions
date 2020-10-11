// NNetModelReaderInterface.h 
//
// NNetModel
//
// Interface for reader threads

#pragma once

#include "MoreTypes.h"
#include "ShapeType.h"
#include "ShapeId.h"
#include "NNetModel.h"
#include "tHighlightType.h"

class Shape;

class NNetModelReaderInterface
{
public:

	void Start( NNetModel * const pModel ) { m_pModel = pModel;	 }
	void Stop ( )                          { m_pModel = nullptr; }

	bool            const IsSelected                ( ShapeId const ) const;
	ShapeType       const GetShapeType              ( ShapeId const ) const;
	fHertz          const GetPulseFrequency         ( ShapeId const ) const;
	size_t          const GetNrOfSegments           ( ShapeId const ) const;
	SoundDescr      const GetTriggerSound           ( ShapeId const ) const;
	bool            const HasIncoming               ( ShapeId const ) const;
	bool            const HasOutgoing               ( ShapeId const ) const;
	size_t          const GetNrOfOutgoingConnections( ShapeId const ) const;
	size_t          const GetNrOfIncomingConnections( ShapeId const ) const;
	bool            const ConnectsTo ( ShapeId const, ShapeId const ) const;
	mV              const GetVoltage                ( ShapeId const ) const;
	mV              const GetVoltage                ( ShapeId const, MicroMeterPoint const & ) const;
			        
	bool            const   AnyShapesSelected( )               const { return m_pModel->GetShapes().AnyShapesSelected( ); }
	bool            const   IsValidShapeId( ShapeId const id ) const { return m_pModel->GetShapes().IsValidShapeId  (id); }
	bool            const   IsShapeNullPtr( ShapeId const id ) const { return m_pModel->IsShapeNullPtr              (id); }
	MicroMeterPoint const   GetShapePos   ( ShapeId const id ) const { return m_pModel->GetShapePos                 (id); }
	Shape           const * GetConstShape ( ShapeId const id ) const { return m_pModel->GetConstShape  ( id ); }
	long            const   GetSizeOfShapeList( )              const { return m_pModel->GetSizeOfShapeList( ); }
	fMicroSecs      const   GetSimulationTime( )               const { return m_pModel->GetSimulationTime ( ); }
	MicroMeterRect  const   GetEnclosingRect( )                const { return m_pModel->GetEnclosingRect  ( ); }

	MicroMeterPoint const OrthoVector( ShapeId const ) const;

	void DrawExterior  ( ShapeId const, DrawContext const &, tHighlightType const ) const;
	void DrawInterior  ( ShapeId const, DrawContext const & ) const;
	void DrawNeuronText( ShapeId const, DrawContext const & ) const;

	ShapeId const FindShapeAt( MicroMeterPoint const &, ShapeCrit const & ) const;

	template <typename T>
	T const GetConstShapePtr( ShapeId const id ) const
	{
		Shape const * const pShape { GetConstShape( id ) };
		return (pShape && HasType<T>( * pShape )) ? static_cast<T>( pShape ) : nullptr;
	}

	template <typename T> unsigned long const GetNrOf ( )                  const { return T::GetCounter( ); }
	template <typename T> bool          const IsOfType( ShapeId const id ) const { return T::TypeFits( GetShapeType( id ) ); }

	template <typename T>   // const version
	bool Apply2AllB( function<bool(T const &)> const & func ) const
	{
		bool bResult { false };
		for ( auto pShape : m_pModel->GetShapes() )
		{
			if ( pShape )
			{
				if ( HasType<T>( * pShape ) )	
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
		m_pModel->GetShapes().Apply2AllShapes
		( 
			[&](Shape & s) { if ( HasType<T>(s) ) func( static_cast<T const &>(s) ); }
		);
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

	void CheckModel() 
	{ 
#ifdef _DEBUG
		m_pModel->CheckModel(); 
#endif
	};

private:
	NNetModel const * m_pModel;

	bool const isConnectedTo    ( ShapeId const, ShapeId const ) const;
	bool const isConnectedToPipe( ShapeId const, ShapeId const ) const;
};
