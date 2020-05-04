// NNetModelInterface.h 
//
// NNetModel
//
// Interface for reader threads

#pragma once

#include "MoreTypes.h"
#include "ShapeType.h"
#include "ShapeId.h"
#include "Shape.h"
#include "tHighlightType.h"

class PixelCoordsFp;
class D2D_driver;
class ObserverInterface;
class NNetModel;
class Param;
class Pipe;

class NNetModelInterface
{
public:
	NNetModelInterface( NNetModel const * pModel )
		: m_pModel ( pModel )
	{}

	bool      const AnyShapesSelected( ) const;
		
	bool      const IsSelected                ( ShapeId const ) const;
	ShapeType const GetShapeType              ( ShapeId const ) const;
	fHertz    const GetPulseFreq              ( ShapeId const ) const;
	size_t    const GetNrOfSegments           ( ShapeId const ) const;
	bool      const HasTriggerSound           ( ShapeId const ) const;
	Hertz     const GetTriggerSoundFrequency  ( ShapeId const ) const;
	MilliSecs const GetTriggerSoundDuration   ( ShapeId const ) const;
	mV        const GetVoltage                ( ShapeId const ) const;
	mV        const GetVoltage                ( ShapeId const, MicroMeterPoint const & ) const;
	bool      const HasIncoming               ( ShapeId const ) const;
	bool      const HasOutgoing               ( ShapeId const ) const;
	size_t    const GetNrOfOutgoingConnections( ShapeId const ) const;
	size_t    const GetNrOfIncomingConnections( ShapeId const ) const;
	bool      const ConnectsTo( ShapeId const, ShapeId const ) const;

	MicroMeterRect const GetEnclosingRect( ) const;

	fMicroSecs GetSimulationTime( ) const;

//	long const GetNrOf( ShapeType const ) const;

	void DrawExterior( ShapeId const, D2D_driver const &, PixelCoordsFp const &, tHighlightType const ) const;
	void DrawInterior( ShapeId const, D2D_driver const &, PixelCoordsFp const & ) const;

	void DrawExterior( MicroMeterRect const &, D2D_driver const &, PixelCoordsFp const & ) const;
	void DrawInterior( MicroMeterRect const &, D2D_driver const &, PixelCoordsFp const &, ShapeCrit const & = ShapeCritAlwaysTrue ) const;

	void DrawNeuronText( MicroMeterRect const &, D2D_driver const &, PixelCoordsFp const & ) const;

	ShapeId const FindShapeAt( MicroMeterPoint const, ShapeCrit const & = ShapeCritAlwaysTrue ) const;

	template <typename T> bool IsOfType( ShapeId const id ) const 
	{ 
		return T::TypeFits( GetShapeType( id ) ); 
	}

private:
	NNetModel const * const m_pModel;

	bool const isConnectedTo    ( ShapeId const, ShapeId const ) const;
	bool const isConnectedToPipe( ShapeId const, ShapeId const ) const;
};
