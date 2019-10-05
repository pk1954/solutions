// NNetModel.h 
//
// NNetModel

#pragma once

#include <vector>
#include "util.h"
#include "MoreTypes.h"
#include "Segment.h"
#include "InputNeuron.h"
#include "Neuron.h"
#include "Pipeline.h"
#include "ModelInterface.h"

class ObserverInterface;
class EventInterface;

class NNetModel : public ModelInterface
{
public:
	NNetModel();

	virtual ~NNetModel( ) { };

	// readOnly functions

	BYTES const GetCoreSize() const 
	{ 
		return BYTES(sizeof(NNetModel)); 
	};

	microseconds GetSimulationTime( ) const 
	{
		return m_timeStamp;
	}

	Pipeline    const * GetPipeline( ) const { return & m_pipeline; }
	InputNeuron const * GetNeuron1( )  const { return & m_neuron1; }
	Knot        const * GetKnot( )     const { return & m_knot; }

	Shape * GetShape( ShapeId const id )
	{
		return ( id == NO_SHAPE ) ? nullptr : m_Shapes[ id.GetValue() - 1 ];
	}

	Shape const * GetShapeUnderPoint( MicroMeterPoint const ) const;

	// manipulating functions

	void HighlightShape( ShapeId const idHighlight )
	{
		if ( m_shapeHighlighted != NO_SHAPE )
		{
			GetShape( m_shapeHighlighted )->SetHighlightState( false );
		}

		if ( idHighlight != NO_SHAPE )
		{
			Shape * pShape = GetShape( idHighlight );
			assert( pShape );
			assert( pShape->GetId() == idHighlight );
			pShape->SetHighlightState( true );
		}

		m_shapeHighlighted = idHighlight;
	}

	ShapeId const AddShape( Shape & shape )
	{
		m_Shapes.push_back( & shape );                        // ShapeId 0 is reserved
		ShapeId id( CastToUnsignedLong( m_Shapes.size() ) );  // after first push_back, size = 1
		shape.SetId( id );
		return id;
	}

	virtual void CopyModelData( ModelInterface const * const src )
	{
		* this = * static_cast<NNetModel const *>( src );
	}

	virtual void Compute( );
	virtual void ResetAll( );

	// static functions  

	static NNetModel * CreateModel();
	static void        DestroyModel( NNetModel * );

	static mV           const PEAK_VOLTAGE;
	static microseconds const PEAK_TIME;

private:

	InputNeuron     m_neuron1;
	Knot            m_knot;
	Pipeline        m_pipeline;
	microseconds    m_timeStamp;
	vector<Shape *> m_Shapes;
	ShapeId         m_shapeHighlighted;
};
